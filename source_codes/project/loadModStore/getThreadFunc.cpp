// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <functional>

using namespace llvm;

namespace {
  class getThreadFunc : public ModulePass {
  public:
    static char ID;
    getThreadFunc() : ModulePass(ID) { }
    ~getThreadFunc() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
    /*
    bool doInitialization(Module &M) override{
    	counter=0;
    }
    
    bool doFinalization(Module &M) override{
    	outs() << counter << "\n";
    }
		*/

    bool runOnModule(Module &M) override {
    	for(Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI){
		  	Function* F = MI;
		  	if (F->isDeclaration()){
		  		//outs() << *F << "\n";
					continue;
				}
				for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
					BasicBlock* block = FI;
					for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
						//Each instruction is a value
						Instruction *I = i;
						if(CallInst *CI = dyn_cast<CallInst>(I)){
							Function *calleeFunc = CI->getCalledFunction();
							if(calleeFunc->getName().equals(StringRef("pthread_create"))){
								//outs() << *CI << "\n";
								Value *arg3 = (CI->getArgOperandUse(2));
								if(Function *pFunc = dyn_cast<Function>(arg3)){
									outs() << pFunc->getName() << "\n";
									pthreadFunction.push_back(pFunc);
								}
							}
						}
					}
				}	
			}
      return false;
    }
    
    std::vector<Function *> *getPthreadFunction(){
    	return &pthreadFunction;
    }
    
	private:
		std::vector<Function *> pthreadFunction;
  };

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char getThreadFunc::ID = 0;
static RegisterPass<getThreadFunc> Z("getThreadFunc", "15745: Find functions used by pthread_create", false, false);
}
