// 15-745 Project
// Pass - 2
// The second pass is a search through the LLVM bytecode to mark all the places 
// where an access to an approximation target takes place
////////////////////////////////////////////////////////////////////////////////



#include "llvm/Pass.h"		//All passes must include this
#include "llvm/IR/Function.h"	//If we use the runOnFunction module then we need this
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/InstVisitor.h"	//This provides a neat way to iterate over and compare the instructions in a bb
#include "llvm/Support/raw_ostream.h"    //For printing 
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include "loadModStore.cpp"

using namespace llvm;

//std::map<StringRef,int> Call_map;

namespace {
  class Pass2 : public ModulePass {
  public:
    static char ID;
    Pass2() : ModulePass(ID) {}
    ~Pass2() {}

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
        AU.addRequired<LoopInfo>();
	AU.addRequired<loadModStore>();  //Pass 1 written by Junhan
	AU.setPreservesAll();
     
    }

    std::vector<Value* > unique_targets; //So as to not double count different operations on the same shared variable
    std::vector<Instruction* > duplication_vector;  //Replace the malloc with a cmalloc call for perf and the 
    //std::vector<Instruction* > merge_and_barrier_vector;
    std::vector<Instruction* > merge_vector;
    std::vector<Instruction* > init_vector;
    std::vector<Instruction* > lock_vector;
    std::vector<Instruction* > unlock_vector;
    std::vector<LoadInst* > Load_for_targets;

    int done_init = 0;
    int next_phase = 0;

//    bool doInitialization(Module &M) {
//	loadModStore &prevPass = getAnalysis<loadModStore>();
//	std::vector<LoadInst* > *Load_vector = prevPass.getLoadInstVector();
//	for(LoadInst *LI : *Load_vector)
//                {
//                        unique_targets.push_back(LI->getPointerOperand());
//                        //outs() << *LI <<"\n";
//                        Load_for_targets.push_back(LI);
//
//                }
//                auto last = std::unique(unique_targets.begin(), unique_targets.end());
//                outs() << "found unique elements\n";
//                unique_targets.erase(last, unique_targets.end());
//		auto last_l = std::unique(Load_for_targets.begin(),Load_for_targets.end());
//                Load_for_targets.erase(last_l, Load_for_targets.end());
//
//                //for(Module::global_iterator gi = M.global_begin(); gi != M.global_end(); ++gi)
//                //      outs() << *gi << "\n";
//                for (int ii = 0; ii < unique_targets.size(); ii++)
//                        outs() << unique_targets[ii] << "\n";
//
//	
//	for (LoadInst *Li : *Load_vector) {
//	for (Module::iterator mi = M.begin(); mi != M.end(); ++mi)
//	for (Function::iterator fi = mi->begin(); fi != mi->end(); ++fi)
//		for (BasicBlock::iterator bi = fi->begin(); bi != fi->end(); ++bi)
//		{
//			Instruction* Inst = bi;
//                                if(LoadInst* ll = dyn_cast<LoadInst>(Inst))
//                                if(Li == ll)
//                                {
//                                        //outs() << "the load in LoadVector is\n";
//                                        //outs() << *ii << "\n";
//                                        --bi;
//                                        --bi;
//                                        //outs() << "the load before LoadVector is " <<*ii << "\n";
//                                        //possible_mallocLi->getPointerOperand()	
//					 outs() << "the address accessed by above load is " << Li->getPointerOperand() << "\n";
//                                        break;
//                                }
//		}
//	}	
//    	return false;
//    }
    // Print output for each function
    bool runOnModule(Module &M) override {      
        int find_malloc = 0;  
	loadModStore &Pass1info = getAnalysis<loadModStore>();
		std::vector<LoadInst* > *Load_vector = Pass1info.getLoadInstVector();
		std::vector<Instruction* > *Modify_vector = Pass1info.getCommInstVector();
		std::vector<StoreInst* > *Store_vector = Pass1info.getStoreInstVector();
	   	std::vector<Value* > interesting_malloc;	
	if(done_init == 0) {								//Initialization stuff done only once for a progrm
		//outs() << "read information from previous pass\n";
		/*Find unique locations from the previous pass*/
		for(LoadInst *LI : *Load_vector)
		{
			unique_targets.push_back(LI->getPointerOperand());
			//outs() << *LI <<"\n";
			Load_for_targets.push_back(LI);
			
		}	
		auto last = std::unique(unique_targets.begin(), unique_targets.end());
		//outs() << "found unique elements\n";
		unique_targets.erase(last, unique_targets.end());
		
		auto last_l = std::unique(Load_for_targets.begin(),Load_for_targets.end());
		Load_for_targets.erase(last_l, Load_for_targets.end());
		
	        //for(Module::global_iterator gi = M.global_begin(); gi != M.global_end(); ++gi)
		//	outs() << *gi << "\n";
		//for (int ii = 0; ii < unique_targets.size(); ii++)
		//{
		//	outs() << unique_targets[ii] << "\n";
		//	outs() << *Load_for_targets[ii] << "\n";
		//}
		done_init = 1;
		
	}
	
	if (done_init == 1)   //If initialization is done proceed with analysis
	{
		//outs() << "DEBUG: entered post init. phase\n";
		for (LoadInst* Li : Load_for_targets) {
			 for (Module::iterator mi = M.begin(); mi != M.end(); ++mi)
		         for (Function::iterator fi = mi->begin(); fi != mi->end(); ++fi)
			for(BasicBlock::iterator ii = fi->begin(); ii != fi->end(); ++ii)
			{
				Instruction* Inst = ii;
				if(LoadInst* ll = dyn_cast<LoadInst>(Inst))
				if(Li == ll)
				{
					//outs() << "the load in LoadVector is\n";
					//outs() << *ii << "\n";
					--ii;
					--ii;
					//outs() << "the load before LoadVector is " <<*ii << "\n";
					if(LoadInst* LL = dyn_cast<LoadInst>(ii)) {
					//outs() << "the address accessed by above load is " << LL->getPointerOperand() << "\n";
					interesting_malloc.push_back(LL->getPointerOperand());
					}
					break;
				}
			}
		}
		//outs() << "contents of interesting_malloc are \n";
		//for(int iii = 0; iii < interesting_malloc.size(); iii++)
		//	outs() << interesting_malloc[iii] << "\n";
		next_phase = 1;
	}
	if (next_phase == 1) {
		for (Module::iterator m2 = M.begin(); m2 != M.end(); ++m2)
                         for (Function::iterator f2 = m2->begin(); f2 != m2->end(); ++f2)
		for (BasicBlock::iterator b2 = f2->begin(); b2 != f2->end(); ++b2)
		{
			Instruction* I = b2;
			if (CallInst *CI = dyn_cast<CallInst>(I))
			{
				StringRef fname = CI->getCalledFunction()->getName();	
				StringRef comparee1 = "pthread_mutex_lock";
      				StringRef comparee2 = "pthread_mutex_unlock";
      			        StringRef comparee3 = "pthread_create";
				StringRef comparee4 = "malloc";
				StringRef comparee5 = "do_merge";
				if (fname.compare(comparee4) == 0) { 
					find_malloc = 1;					
				}
				else if (fname.compare(comparee5) == 0)   
     					merge_vector.push_back(I);
				else if (fname.compare(comparee1) == 0)
					lock_vector.push_back(I);
				else if (fname.compare(comparee2) == 0)
					unlock_vector.push_back(I);
				else if (fname.compare(comparee3) == 0)
                                        init_vector.push_back(I);
			}
			if (find_malloc == 1) {
				//outs() << "DEBUG:entered the malloc search portion\n";
				if(StoreInst* SI = dyn_cast<StoreInst>(I)) {
			        	for(int nn = 0; nn < interesting_malloc.size(); nn++) {
						//outs() << "The value of interesting malloc is " << interesting_malloc[nn] << "\n";
						if (interesting_malloc[nn] == SI->getPointerOperand()){
							//outs() << "store instruction " << *SI << " with the store address and value being "<< SI->getPointerOperand() <<" and " << SI->getValueOperand() << "\n";	
							BasicBlock::iterator itemp = b2;
							--itemp; --itemp;
							duplication_vector.push_back(itemp);
						}
					}
				find_malloc = 0;
				}
				
				
			}
		}	
/* //TESTING
	outs() << "THe contents of dupvec\n";
	for (int a = 0; a< duplication_vector.size(); a++)
		outs() << *duplication_vector[a] << "\n";
	outs() << "THe contents of mergevec\n";
        for (int a = 0; a< merge_vector.size(); a++)
                outs() << *merge_vector[a] << "\n";
	outs() << "THe contents of lockvec\n";
        for (int a = 0; a< lock_vector.size(); a++)
                outs() << *lock_vector[a] << "\n";
	outs() << "THe contents of unlockvec\n";
        for (int a = 0; a< unlock_vector.size(); a++)
                outs() << *unlock_vector[a] << "\n";
	outs() << "THe contents of initvec\n";
        for (int a = 0; a< init_vector.size(); a++)
                outs() << *init_vector[a] << "\n";
*/
	}
    
    return false;
    }

/* THe API of this pass is the following five functions */
    std::vector<Instruction* > *getDuplicationPoints(){
	return &duplication_vector;
    }

    std::vector<Instruction* > *getMergePoints(){
	return &merge_vector;
    }
     std::vector<Instruction* > *getInitPoints(){
        return &init_vector;
    }
     std::vector<Instruction* > *getLockPoints(){
        return &lock_vector;
    }
     std::vector<Instruction* > *getUnlockPoints(){
        return &unlock_vector;
    }

  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char Pass2::ID = 0;
static RegisterPass<Pass2> X("pass2", "15745: Function Information", false, false);   //This command registers the passname
