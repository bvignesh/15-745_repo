// 15-745 S15 Assignment 1: FunctionInfo.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <iostream>

using namespace llvm;

std::map<StringRef,int> Call_map;

namespace {
  class FunctionInfo : public FunctionPass {
  public:
    static char ID;
    FunctionInfo() : FunctionPass(ID) { }
    ~FunctionInfo() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }

    // Do some initialization
    bool doInitialization(Module &M) override {
      errs() << "15745 Function Information Pass\n"; // TODO: remove this.
      // outs() << "Name,\tArgs,\tCalls,\tBlocks,\tInsns\n";
      for (auto &F:M)
	{
			for(auto &B:F)
				{
						for(auto &I:B)
						{
							if (CallInst* CI = dyn_cast<CallInst>(&I))
                        			{
                                			Function* F_call = CI->getCalledFunction();  //capturing explicit function calls in bitcode
                                			Call_map[F_call->getName()]++;  //The map structure maintains number of calls

                                	}
						}
				 }
	}
     
      return false;
    }

    // Print output for each function
    bool runOnFunction(Function &F) override {
    
    int bb_ctr = 0;  //counter for number of basic blocks within a function
    int inst_ctr = 0; //counter for number of instructions within a function
    for (auto &B:F)
	{
			bb_ctr++;
			for (auto &I:B)
			{
				inst_ctr++;	
			}
	}
    
  


    int arg_no;			//variable that holds number of arguments to a function
    if (F.isVarArg())
	outs() << "name " << F.getName() << ",\t\t" << "args *,\t\t" << "calls " << Call_map[F.getName()] << ",\t\t" << "bbs " << bb_ctr << ",\t\t" << "insts " << inst_ctr << "\n";
    else
	{
			arg_no = F.arg_size();
			outs() << "name " << F.getName() << ",\t\t" << "args " << arg_no << ",\t\t" << "calls " << Call_map[F.getName()] << ",\t\t" << "bbs " << bb_ctr << ",\t\t" << "insts " << inst_ctr << "\n";	
    
   	} 
    return false;
    }
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char FunctionInfo::ID = 0;
static RegisterPass<FunctionInfo> X("function-info", "15745: Function Information", false, false);
