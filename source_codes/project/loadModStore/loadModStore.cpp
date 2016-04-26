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
  class loadModStore : public ModulePass {
  public:
    static char ID;
    loadModStore() : ModulePass(ID) { }
    ~loadModStore() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
    	AU.addRequired<LoopInfo>();
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
    	counter=0;
    	for(Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI){
		  	Function* F = MI;
		  	if (F->isDeclaration()){
		  		//outs() << *F << "\n";
					continue;
				}
    		LoopInfo &LInfo=getAnalysis<LoopInfo>(*F);
				for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
					BasicBlock* block = FI;
					int lockCount=0;
					for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
						//Each instruction is a value
						Instruction *I = i;
						if(CallInst *CI = dyn_cast<CallInst>(I)){
							Function *calleeFunc = CI->getCalledFunction();
							if(calleeFunc->getName().equals(StringRef("pthread_mutex_lock"))){
								lockCount++;
							}else if(calleeFunc->getName().equals(StringRef("pthread_mutex_unlock"))){
								lockCount--;
							}
						}
						if(lockCount<1){
							continue;
						}
						if(LoadInst *LI = dyn_cast<LoadInst>(I)){
					    //outs() << (LI->getPointerOperand()->getName()) <<"\t:\t" << *LI << "\n";
					    if(LI->hasOneUse()){
					    	//The loaded value only gets to be used one time
					    	User *Uload = LI->user_back();
					    	if(Instruction *commI = dyn_cast<Instruction>(Uload)){
					    		if(commI->isCommutative()&&commI->hasOneUse()){
					    			User *Ustore = commI->user_back();
					    			if(StoreInst *Store = dyn_cast<StoreInst>(Ustore)){
					    				if(Store->getPointerOperand()==LI->getPointerOperand()){
					    					//The current instance of the load have a load-modify-store pattern
					    					Loop *currentLoop = LInfo.getLoopFor(I->getParent());
					    					Value *Addr = LI->getPointerOperand();
					    					bool isLoopIncrement = false;
					    					if(currentLoop!=NULL){
					    						int addrUseCount=0;
							  					for(auto UI = Addr->user_begin(), Uend = Addr->user_end(); UI!=Uend; ++UI){
							  						++addrUseCount;
							  						if (LoadInst *loadUse = dyn_cast<LoadInst>(*UI)) {
															if(loadUse->hasOneUse()){
																User *oneUse = loadUse->user_back();
																if(BranchInst *oneBranch = dyn_cast<BranchInst>(oneUse)){
																	if(LInfo.getLoopFor(oneBranch->getParent())==currentLoop){
																		isLoopIncrement=true;
																		break;
																	}
																}else if(CmpInst *oneCmp = dyn_cast<CmpInst>(oneUse)){
																	if(oneCmp->hasOneUse()){
																		User *oneCmpUse = oneCmp->user_back();
																		if(BranchInst *oneBranch = dyn_cast<BranchInst>(oneCmpUse)){
																			if(LInfo.getLoopFor(oneBranch->getParent())==currentLoop){
																				isLoopIncrement=true;
																				break;
																			}
																		}	
																	}
																}
															}
														}
							  					}
							  				}
							  				if(!isLoopIncrement){
							  					counter++;
							  					outs()  << *LI <<"\n" << *Uload << "\n" << *Store <<"\n\n";
							  					loadInstVector.push_back(LI);
							  					commInstVector.push_back(commI);
							  					storeInstVector.push_back(Store);
							  				}
					    				}
					    			}
					    		}
					    	}
					    }
						}
					}
				}	
			}
    	outs() << counter << "\n";
      return false;
    }
    
    std::vector<LoadInst *> *getLoadInstVector(){
    	return &loadInstVector;
    }
    
    std::vector<Instruction *> *getCommInstVector(){
    	return &commInstVector;
    }
    
    std::vector<StoreInst *> *getStoreInstVector(){
    	return &storeInstVector;
    }
	private:
		int counter;
		std::vector<LoadInst *> loadInstVector;
		std::vector<Instruction *> commInstVector;
		std::vector<StoreInst *> storeInstVector;
  };

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char loadModStore::ID = 0;
static RegisterPass<loadModStore> Y("loadModStore", "15745: Find load modify store pattern", false, false);
}
