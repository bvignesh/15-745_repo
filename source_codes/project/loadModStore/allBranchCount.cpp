// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "findShared.cpp"
#include <set>

using namespace llvm;

namespace {
  class allBranchCount : public ModulePass {
  public:
    static char ID;
    allBranchCount() : ModulePass(ID) { }
    ~allBranchCount() { }

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
			
			//See in which basic block are each shared variable used
			int numGV = sharedVariables->size();
			std::set<BasicBlock *> **usedSets = (std::set<BasicBlock *> **)malloc(sizeof(std::set<BasicBlock *>*)*numGV);
			assert(usedSets);
			for(int i=0;i< numGV; i++){
				usedSets[i] = new std::set<BasicBlock *>();
				GlobalVariable *GV = (*sharedVariables)[i];
				for(auto UI = GV->user_begin(), Uend = GV->user_end(); UI!=Uend; ++UI){
					if (Instruction *GVuser = dyn_cast<Instruction>(*UI)) {
						if(isa<StoreInst>(GVuser)){
							continue;
						}
						BasicBlock *usedBB = GVuser->getParent();
						usedSets[i]->insert(usedBB);
					}
				}
			}
			
			//Count in which shared variables is used in all passes prev to a meet
			int *varCount = (int *)calloc(sizeof(int), numGV);
			assert(varCount);
			for(Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI){
		  	Function* F = MI;
		  	if (F->isDeclaration()){
					continue;
				}
				LoopInfo &LInfo=getAnalysis<LoopInfo>(*F);
				for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
					BasicBlock* block = FI;
					if(block==&(F->front())){
						continue;
					}
					//if(LInfo.getLoopFor(block)){
					//	continue;
					//}
					if(block->getSinglePredecessor()!=NULL){
						continue;
					}
					//We only count the path where there are more then one predecessor
					for(int i=0;i<numGV;i++){
						varCount[i]++;
						for (auto it = pred_begin(block), et = pred_end(block); it != et; ++it){
							BasicBlock* predecessor = *it;
							if(usedSets[i]->count(predecessor)==0){
								varCount[i]--;
								break;
							}
						}
					}
				}
			}
			
			for(int i=0;i< numGV; i++){
				GlobalVariable *GV = (*sharedVariables)[i];
				outs() << GV->getName() << " is used in all path before a meet " << varCount[i] << " times.\n";
			}
			
      return false;
    }
	private:

  };

}
// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char allBranchCount::ID = 0;
static RegisterPass<allBranchCount> Y("allBranchCount", "15745: Count all pass uses for each shared variable pass", false, false);
