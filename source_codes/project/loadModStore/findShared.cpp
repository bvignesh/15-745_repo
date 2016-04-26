// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "loadModStore.cpp"

using namespace llvm;

namespace {
  class findShared : public ModulePass {
  public:
    static char ID;
    findShared() : ModulePass(ID) { }
    ~findShared() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
    	AU.addRequired<LoopInfo>();
    	AU.addRequired<loadModStore>();
      AU.setPreservesAll();
    }

    // Print output for each function
    bool runOnModule(Module &M) override {
			loadModStore &LMSinfo=getAnalysis<loadModStore>();
			std::vector<LoadInst *> *loadInstVector = LMSinfo.getLoadInstVector();
			std::vector<Instruction *> *commInstVector = LMSinfo.getCommInstVector();
			std::vector<StoreInst *> *storeInstVector = LMSinfo.getStoreInstVector();
			
			//outs() << "Load instructions:\n";
			
			for(LoadInst *LI : *loadInstVector){
				//outs() << *LI << "\n";
				Value *val = getOrigPointer(LI);
				if(GlobalVariable *GV = dyn_cast<GlobalVariable>(val)){
					//The globlal variable is a single global variable
					addSharedVariable(GV);
				}
			}
			
			outs() << "All shared variables found:\n";
			
			for(GlobalVariable *GV : sharedVariables){
				outs() << *GV << "\n";
			}
			outs() << "\n";
			
      return false;
    }
    
    std::vector<GlobalVariable *> *getSharedVariables(){
    	return &sharedVariables;
    }
    
	private:
		std::vector<GlobalVariable *> sharedVariables;
		
		void addSharedVariable(GlobalVariable *toAdd){
			if(std::find(sharedVariables.begin(),sharedVariables.end(),toAdd)==sharedVariables.end()){
				sharedVariables.push_back(toAdd);
			}
		}
		
		Value *getOrigPointer(LoadInst *orig){
			Value *current = orig;
			while(true){
				if(LoadInst *LI = dyn_cast<LoadInst>(current)){
					current = LI->getPointerOperand();
					continue;
				}
				if(GetElementPtrInst *EP = dyn_cast<GetElementPtrInst>(current)){
					current = EP->getPointerOperand();
					continue;
				}
				if(CastInst *CI = dyn_cast<CastInst>(current)){
					current = CI->getOperand(0);
					continue;
				}
				break;
			}
			return current;
		}
		
		
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char findShared::ID = 0;
static RegisterPass<findShared> X("findShared", "15745: Find shared variable pass", false, false);
