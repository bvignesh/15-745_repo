// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "findShared.cpp"

using namespace llvm;

namespace {
  class useCount : public ModulePass {
  public:
    static char ID;
    useCount() : ModulePass(ID) { }
    ~useCount() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
    	AU.addRequired<LoopInfo>();
    	AU.addRequired<loadModStore>();
    	AU.addRequired<findShared>();
      AU.setPreservesAll();
    }

    // Print output for each function
    bool runOnModule(Module &M) override {
			findShared &Sinfo=getAnalysis<findShared>();
			std::vector<GlobalVariable *> *sharedVariables = Sinfo.getSharedVariables();
			
			for(GlobalVariable *GV : *sharedVariables){
				outs() << GV->getName() << " is used " << GV->getNumUses() << " times.\n";
			}
			
      return false;
    }
	private:

  };

}
// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char useCount::ID = 0;
static RegisterPass<useCount> Y("useCount", "15745: Count uses for each shared variable pass", false, false);
