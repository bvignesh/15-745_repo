// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "findShared.cpp"
#include "getThreadFunc.cpp"

using namespace llvm;

namespace {
  class funcSVUseCount : public ModulePass {
  public:
    static char ID;
    funcSVUseCount() : ModulePass(ID) { }
    ~funcSVUseCount() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
    	AU.addRequired<LoopInfo>();
    	AU.addRequired<loadModStore>();
    	AU.addRequired<findShared>();
    	AU.addRequired<getThreadFunc>();
      AU.setPreservesAll();
    }

    // Print output for each function
    bool runOnModule(Module &M) override {
			findShared &Sinfo=getAnalysis<findShared>();
			getThreadFunc &Finfo=getAnalysis<getThreadFunc>();
			std::vector<GlobalVariable *> *sharedVariables = Sinfo.getSharedVariables();
			std::vector<Function *> *pthreadFunc = Finfo.getPthreadFunction();
			
			for(Function *F : *pthreadFunc){
				int counter = 0;
				for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
					BasicBlock* block = FI;
					for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
						//Each instruction is a value
						Instruction *I = i;
						if(LoadInst *LI = dyn_cast<LoadInst>(I)){
							Value *ptr = LI->getPointerOperand();
							if(GlobalVariable *GV = dyn_cast<GlobalVariable>(ptr)){
								if(std::find(sharedVariables->begin(),sharedVariables->end(),GV)!=sharedVariables->end()){
									counter++;
								}
							}
						}
					}
				}	
				outs() << "Function " << F->getName() << " have " << counter << " uses of shared variables\n";
			}
			
      return false;
    }
	private:

  };

}
// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char funcSVUseCount::ID = 0;
static RegisterPass<funcSVUseCount> Y("funcSVUseCount", "15745: Count uses for each shared variable pass", false, false);
