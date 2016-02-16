// 15-745 S16 Assignment 2: dataflow.h
// Group:
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__

#include <stdio.h>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/Support/raw_ostream.h"


namespace llvm {
// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.
	typedef struct BBinfo{
		int beginIndx;
		int endIndx;
		std::vector<Value *> FollowingPHINodes; //List of all the PHINodes involving this basic block in successors
	}BBinfo;

	class DataFlowAnalysis : public FunctionPass {
		public:
			DataFlowAnalysis(char &ID);
			virtual bool runOnFunction(Function& F);
			virtual void getAnalysisUsage(AnalysisUsage& AU) const;
			//function template for transfer function with the gen|(current-kill) type, in this case only have to provide the gen and kill function
			static BitVector genKillTransferFunction(BitVector &lastResult, Value &currentInst, 
						std::function<std::vector<Value *> (Value&)> genFunc, std::function<bool (Value&, Value&)> killFunc,
						std::vector<Value *> &Domain); 
		protected:
			std::vector<Value *> Domain;
			int domainSize;
			std::vector<BitVector> programPoints;
			int ppNum; //programPointsNumber
			std::map<BasicBlock*,BBinfo> BBinfoMap;
			//For each instruction in the function, calls this function at start to see what are included in the domain
			virtual std::vector<Value *> addToDomain( Value *eval) = 0;
			//get if the pass is a forward pass or a backword pass
			virtual bool isForwardPass() = 0;
			//The transfer function, given bitvector of last result and current instruction yield bitvector of next result
			virtual BitVector transferFunction(BitVector &lastResult, Value &currentInst) =0;
			//A special transfer function to deal with PHINodes, given bitvector of last result and current instruction yield bitvector of next result
			virtual BitVector phiTransferFunction(BitVector &lastResult, Value &currentInst, BasicBlock *currentBB) =0;
			//The meet operator, gives two bitvector and returns the meet result as another bitvector
			virtual BitVector meet(const BitVector &vec1, const BitVector &vec2) =0;
			//The boundry init value bitvector
			virtual bool boundaryCondition() = 0;
			//The initial value for the in/out 
			virtual bool initialInterior() = 0;
			//print out the current value as the appearence required
			virtual std::string toString(Value &V) =0;
		private:
			//reset the pass inner data
			void resetObject();
			//Checks if the giving value is not already in domian, if not then add to domain
			void checkAndAddToDomain(Value *v);
			//does a forward pass return true if something have been updated
			bool forwardPass(Function& F);
			//does a backward pass return true if somethinng have been updated
			bool backwardPass(Function& F);
			//prints out the result of the current set
			void printSet(BitVector& BV);
			//prints out the result of the analysis pass
			void printResult(Function& F);
	};
}

#endif
