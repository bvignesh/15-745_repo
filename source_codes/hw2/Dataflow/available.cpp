// 15-745 S16 Assignment 2: available.cpp
// Group:junhanz vigneshb
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"
#include "available-support.h"

using namespace llvm;
using namespace std;

namespace {
  class AvailableExpressions : public DataFlowAnalysis {

  public:
    static char ID;

    AvailableExpressions() : DataFlowAnalysis(ID) { }

	private:
		//For each instruction in the function, calls this function at start to see what are included in the domain
		virtual std::vector<Value *> addToDomain(Value *eval){
		  std::vector<Value *>myVal=std::vector<Value *>();
		  //Domain is binary expression
		  if (BinaryOperator *BI = dyn_cast<BinaryOperator>(eval)) {
		  	Expression Ex = Expression(BI);
		  	for(Value *I : Domain){
		  		Expression addedEx = Expression(cast<Instruction>(I));
		  		if(addedEx==Ex){
		  			//Expression already in domain
		  			return myVal;
		  		}
		  	}
				myVal.push_back(eval);
			}
		  return myVal;
		}
		//get if the pass is a forward pass or a backword pass
		virtual bool isForwardPass(){
		  return true;
		}
		
		//The transfer function, given bitvector of last result and current instruction yield bitvector of next result
		virtual BitVector transferFunction(BitVector &lastResult, Value &currentInst){
		  return DataFlowAnalysis::genKillTransferFunction(lastResult, currentInst, 
					[this](Value& myInst) -> std::vector<Value *> {
						std::vector<Value *> genList = std::vector<Value *>();
					  //The genFunc body, given instruction see what are generated
					  if(BinaryOperator *BI = dyn_cast<BinaryOperator>(&myInst)){
					  	Expression Ex = Expression(BI);
							for(Value *I : Domain){
								Expression toAdd = Expression(cast<Instruction>(I));
								if(toAdd==Ex){
									genList.push_back(I);
									break;
								}
							}
					  }
					  return genList;
					},
					[this](Value& myInst, Value& potentialTarget) -> bool {
					  //the killFunc body, given instruction see if it kills the second given element
					  if(BinaryOperator *BI = dyn_cast<BinaryOperator>(&potentialTarget)){
							if(BI->getOperand(0)==&myInst||BI->getOperand(1)==&myInst){
								//One of the variables used in the expression is defined here
								return true;
							}
					  }
					  return false;
					},Domain);
		}
		
		virtual BitVector phiTransferFunction(BitVector &lastResult, Value &currentInst, BasicBlock *currentBB){
			//As is phi instuction, will not add any new instructions, just sees which one should be killed
			return transferFunction(lastResult, currentInst);
		}
		
		//The meet operator, gives two bitvector and returns the meet result as another bitvector
		virtual BitVector meet(const BitVector &vec1, const BitVector &vec2){
		  BitVector result = BitVector(vec1);
		  result &=vec2;
		  return result;
		}
		
		//The boundry init value bitvector
		virtual bool boundaryCondition(){
		  return false;
		}
		
		//The initial value for the in/out 
		virtual bool initialInterior(){
		  return true;
		}
		
		//print out the current value as the appearence required
		virtual std::string toString(Value &V){
		  return Expression(cast<Instruction>(&V)).toString();
		}
  };

  char AvailableExpressions::ID = 0;
  RegisterPass<AvailableExpressions> X("available",
    "15745 Available Expressions");
}
