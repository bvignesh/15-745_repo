// 15-745 S15 Assignment 1: FunctionInfo.cpp
// Group:vigneshb junhanz
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
			for(auto &I:B)
				{
					if (CallInst* CI = dyn_cast<CallInst>(&I))
                        		{
                                		Function* F_call = CI->getCalledFunction();
                                		Call_map[F_call->getName()]++;

                                		//outs() << "encountered a call to " << F_call->getName() << " from " << F.getName() << "\n";
                                		//outs() << "for the called function, Call_map = " << Call_map[F_call->getName()] << "\n";
                        		}     
      return false;
    }

    // Print output for each function
    bool runOnFunction(Function &F) override {
    //Call_map[F.getName()] = 0;
    int bb_ctr = 0;
    int inst_ctr = 0;
    for (auto &B:F)
	{
		bb_ctr++;
		for (auto &I:B)
		{
			inst_ctr++;	
		}
	}
    
    int arg_no;		//string to hold the below case 
    if (F.isVarArg())
	arg_no = (int)'*';
    else
	arg_no = F.arg_size();

    int call_ctr = 0;
    for (Function::use_iterator UI = F.use_begin(); UI != F.use_end(); ++UI)
	call_ctr++;
    
    outs() << "name" << F.getName() << ",\t" << "args " << arg_no  <<",\t" << "calls " << Call_map[F.getName()] << ",\t" << "bbs" << bb_ctr << ",\t" << "insts" << inst_ctr << "\n";
    
    outs() << "Call_map = " << Call_map[F.getName()] << "\n";
      return false;
    }
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char FunctionInfo::ID = 0;
static RegisterPass<FunctionInfo> X("function-info", "15745: Function Information", false, false);
