// 15-745 S15 project
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <functional>

using namespace llvm;

namespace {
  class getHello : public ModulePass {
  public:
    static char ID;
    getHello() : ModulePass(ID) { }
    ~getHello() { }

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
    	GlobalVariable* threadCount = new GlobalVariable(M,
    					Type::getInt32Ty(M.getContext()),
    					false,
    					GlobalValue::ExternalLinkage,
    					Constant::getNullValue(Type::getInt32Ty(M.getContext())),
    					Twine("threadCount"),
    					M.global_begin());
    	threadCount->setAlignment(4);
    	for(Module::iterator MI = M.begin(), ME = M.end(); MI != ME; ++MI){
		  	Function* F = MI;
		  	if (F->isDeclaration()){
					continue;
				}
				if(!F->getName().equals(StringRef("main"))){
					continue;
				}
    		LoopInfo &LInfo=getAnalysis<LoopInfo>(*F);
				for (Function::iterator FI = F->begin(), FE = F->end(); FI != FE; ++FI) {
					BasicBlock* block = FI;
					for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
						//Each instruction is a value
						Instruction *I = i;
						if(CallInst *CI = dyn_cast<CallInst>(I)){
							LoadInst *currentVal = new LoadInst(threadCount, Twine("currentVal"),I);
							currentVal->setAlignment(4);
							CI->setArgOperand(1,currentVal);
							break;
						}
						//outs() << *I << "\n";
					}
				}
				BasicBlock &entry = F->getEntryBlock();
				BasicBlock::iterator i = entry.begin();
				while(isa<AllocaInst>(i)){
					++i;
				}
				Instruction *insertBefore = i;
				Function::arg_iterator arg = F->arg_begin();
				++arg;//We want the second argument
				Value *argv = arg;
				//Get the elementpointer instruction
				Value *indexList[1] = {ConstantInt::get(Type::getInt32Ty(M.getContext()), 1)};
				GetElementPtrInst *argIndex = GetElementPtrInst::CreateInBounds(argv,indexList,Twine("argIndex"),insertBefore);
				//Load instruction
				LoadInst *arg2 = new LoadInst(argIndex, Twine("arg2"), insertBefore);
				arg2->setAlignment(4);
				//Call atoi instruction
				Type *result = Type::getInt32Ty(M.getContext());
				std::vector<Type*> args;
				args.push_back(PointerType::getUnqual(Type::getInt8Ty(M.getContext())));
				FunctionType *FT=FunctionType::get(result, args, false);
				Constant *atoiFunc = M.getOrInsertFunction(StringRef("atoi"),FT);
				std::vector<Value *> argument;
				argument.push_back(arg2);
				CallInst *atoiResult =CallInst::Create(atoiFunc,argument,"atoiResult",insertBefore);
				//Store instruction
				StoreInst *store = new StoreInst(atoiResult,threadCount,insertBefore);
				store->setAlignment(4);
			}
			
			
    	//outs() << counter << "\n";
      return true;
    }
	private:
		int counter;
  };

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char getHello::ID = 0;
static RegisterPass<getHello> X("getHello", "15745: Add a hello world to the given program", false, false);
}
