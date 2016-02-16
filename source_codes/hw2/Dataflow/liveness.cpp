// 15-745 S16 Assignment 2: liveness.cpp
// Group:junhanz vigneshb
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"
#include "available-support.h"

using namespace llvm;

namespace {

  class Liveness : public DataFlowAnalysis {
  public:
    static char ID;

    Liveness() : DataFlowAnalysis(ID) { }

  private:
  		//For each instruction in the function, calls this function at start to see what are included in the domain
			virtual std::vector<Value *> addToDomain(Value *eval){
			  std::vector<Value *> myVal (1,eval);
			  //Domain is variable, thus every instuction is a variable
			  if(User *Inst = dyn_cast<User>(eval)){
			  	int numOp = Inst->getNumOperands();
			  	for(int i=0; i<numOp; i++){
			  		//Each use of a variable makes it in the gen list
			  		Value * oprand = Inst->getOperand(i);
			  		if((!isa<Constant>(oprand))&&(!isa<BasicBlock>(oprand))){
			  			myVal.push_back(oprand);
			  		}
			  	}
			  }
			  return myVal;
			}
			//get if the pass is a forward pass or a backword pass
			virtual bool isForwardPass(){
			  return false;
			}
			
			//The transfer function, given bitvector of last result and current instruction yield bitvector of next result
			virtual BitVector transferFunction(BitVector &lastResult, Value &currentInst){
			  return DataFlowAnalysis::genKillTransferFunction(lastResult, currentInst, 
						[this](Value& myInst) -> std::vector<Value *> {
							std::vector<Value *> genList = std::vector<Value *>();
						  //The genFunc body, given instruction see what are generated
						  if(User *Inst = dyn_cast<User>(&myInst)){
								for(auto OI = Inst->op_begin(); OI != Inst->op_end(); ++OI){
									//Each use of a variable makes it in the gen list
									Value * oprand = *OI;
									if((isa<Instruction>(oprand))||(isa<Argument>(oprand))){
										genList.push_back(oprand);
									}
								}
						  }
						  return genList;
						},
						[this](Value& myInst, Value& potentialTarget) -> bool {
						  //the killFunc body, given instruction see if it kills the second given element
						  //In liveness analysis, if the target is defined here then it's killed, aka ==
						  return (&myInst==&potentialTarget);
						},Domain);
			}
			
			virtual BitVector phiTransferFunction(BitVector &lastResult, Value &currentInst, BasicBlock *currentBB){
				BitVector result = BitVector(lastResult);
				auto killIndex = std::find(Domain.begin(), Domain.end(), &currentInst);
				if(killIndex!=Domain.end()){
					result[killIndex-Domain.begin()]=false;
				}
				if(PHINode *phi = dyn_cast<PHINode>(&currentInst)){
			    //This should always hold true
		      for(unsigned i=0; i < phi->getNumIncomingValues() ; i++){
		      	if(phi->getIncomingBlock(i)==currentBB){
		      		Value *toAdd = phi->getIncomingValue(i);
		      		auto index = std::find(Domain.begin(), Domain.end(), toAdd);
							if(index!=Domain.end()){
								result[index-Domain.begin()]=true;
							}
		      	}
		      }
			  }
				return result;
			}
			
			//The meet operator, gives two bitvector and returns the meet result as another bitvector
			virtual BitVector meet(const BitVector &vec1, const BitVector &vec2){
			  BitVector result = BitVector(vec1);
			  result |=vec2;
			  return result;
			}
			
			//The boundry init value bitvector
			virtual bool boundaryCondition(){
			  return false;
			}
			
			//The initial value for the in/out 
			virtual bool initialInterior(){
			  return false;
			}
			
			//print out the current value as the appearence required
			virtual std::string toString(Value &V){
			  return getShortValueName(&V);
			}
  };

  char Liveness::ID = 0;
  RegisterPass<Liveness> X("liveness", "15745 Liveness");
}
