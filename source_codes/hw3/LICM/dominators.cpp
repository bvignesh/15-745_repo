// 15-745 S16 Assignment 3: dominators.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflowBB.h"

using namespace llvm;

namespace {

  class Dominators : public DataFlowAnalysisBB {
  public:
    static char ID;

    Dominators() : DataFlowAnalysisBB(ID) { }
    
    //Sees if the BB val dominates BB place
		virtual bool getPassInfo(BasicBlock* place, BasicBlock* val){
			int index = BBinfoMap[place].beginIndx;
			auto valIdx = std::find(Domain.begin(), Domain.end(), val);
			if(valIdx!=Domain.end()){
			  return (programPoints[index])[valIdx-Domain.begin()];
			}
			return false;
		}
  

  private:
  		//For each instruction in the function, calls this function at start to see what are included in the domain
			virtual std::vector<Value *> addToDomain(Value *eval){
			  std::vector<Value *> genList = std::vector<Value *>();
			  genList.push_back(eval);
			  return genList;
			}
			//get if the pass is a forward pass or a backword pass
			virtual bool isForwardPass(){
			  return true;
			}
			
			//The transfer function, given bitvector of last result and current instruction yield bitvector of next result
			virtual BitVector transferFunction(BitVector &lastResult, Value &currentInst){
			  BitVector currentResult = BitVector(lastResult);
			  auto index = std::find(Domain.begin(), Domain.end(), &currentInst);
				if(index!=Domain.end()){
					currentResult[index-Domain.begin()]=true;
				}
				return currentResult;
			}
			
			//Shouldn't be called here
			virtual BitVector phiTransferFunction(BitVector &lastResult, Value &currentInst, BasicBlock *currentBB){
				assert(0);
				return lastResult;
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
			  if(BasicBlock *BB = dyn_cast<BasicBlock>(&V)){
			    return BB->getName();
				}
				return "dom";
			}
  };

  char Dominators::ID = 0;
  RegisterPass<Dominators> X("dominators", "15745 Dominators");
}
