// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "loadModStore.cpp"

using namespace llvm;

namespace {
  class LMStest : public ModulePass {
  public:
    static char ID;
    LMStest() : ModulePass(ID) { }
    ~LMStest() { }

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
			
			outs() << "Load instructions:\n";
			
			for(LoadInst *LI : *loadInstVector){
				outs() << *LI << "\n";
			}
			
			outs() << "Op instructions:\n";
			
			for(Instruction *LI : *commInstVector){
				outs() << *LI << "\n";
			}
			
			outs() << "Store instructions:\n";
			
			for(StoreInst *LI : *storeInstVector){
				outs() << *LI << "\n";
			}
			
			
      return false;
    }
	private:

  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LMStest::ID = 0;
static RegisterPass<LMStest> X("LMStest", "15745: Load Modify Store pass test", false, false);
