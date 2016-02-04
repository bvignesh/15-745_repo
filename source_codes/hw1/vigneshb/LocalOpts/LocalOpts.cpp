// 15-745 S15 Assignment 1: FunctionInfo.cpp
// Group:junhanz vigneshb
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <functional>

using namespace llvm;

namespace {
  class LocalOpts : public BasicBlockPass {
  public:
    static char ID;
    LocalOpts() : BasicBlockPass(ID) { }
    ~LocalOpts() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }

    bool runOnBasicBlock(BasicBlock &BB) override {
    	bool returnVal = false;
			int iden, fold, reduc;
			iden=fold=reduc=0;
			returnVal = returnVal |
			applyOnIplistElement<Instruction>(BB.getInstList(),[&iden,&fold,&reduc,this](Instruction &Inst) -> bool{
				User *U = (User *)&Inst;
				unsigned i =0;
				for(i=0;i< U->getNumOperands(); i++){
					Value *V = U->getOperand(i);
				}
				///@NOTE It only support interger instruction and not floating point instructions
				if(BinaryOperator::classof(&Inst)){
					BinaryOperator *BB = (BinaryOperator *)&Inst;
					//deletes instruction if folded, no need to go further
					if(optConstFold(*BB)){
						fold++;
						return true;
					}
					//deletes instruction if is identity, no need to go further
					if(optAlgIden(*BB)){
						iden++;
						return true;;
					}
					if(optStrenReduc(*BB)){
						reduc++;
						return true;;
					}
				}
				return false;;
			});
			outs() << "Transformations applied:\n";
			outs() << "\tAlgebraic identities: " << iden <<"\n";
			outs() << "\tConstant folding: " << fold << "\n";
			outs() << "\tStrength reduction: " << reduc << "\n";
      return returnVal;
    }
	private:
		//Do action with each element in the given iplist
		//Returns true if any of the action returns true, else return false
		template <class T> bool applyOnIplistElement 
		(iplist<T> &givenList, std::function<bool (T&)> action){
			bool retVal = false;
			T &myFirst = givenList.front();
			T *myPtr = &myFirst;
			T &myLast = givenList.back();
			T *myLastPtr = &myLast;
			while(myPtr!=nullptr){
				T &myIter = *myPtr;
				//Get the next block here in case action() removes current block from list
				T *nextPtr = givenList.getNext(myPtr);
				retVal |= action(myIter);
				if(myPtr==myLastPtr){
					break;
				}
				myPtr = nextPtr;
			}
			return retVal;
		}
		//Checks if the given binary operator instruction is an algebraic identity
		//If yes returns true and simplify the instruction, else return false
		bool optAlgIden(BinaryOperator &I){
			if(I.getNumOperands()!=2){
				//we only deal with two operand instructions here
				return false;
			}
			User *U = (User *)&I;
			Value *Op1=U->getOperand(0);
			Value *Op2=U->getOperand(1);
			switch(I.getOpcode()){
				case Instruction::Add:{
					if(ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isZero()){
						I.replaceAllUsesWith(Op2);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isZero()){
						I.replaceAllUsesWith(Op1);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::Sub:{
					if(Op1==Op2){
						Constant *zeroVal = ConstantInt::get(I.getType(),0);
						I.replaceAllUsesWith((Value *)zeroVal);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isZero()){
						I.replaceAllUsesWith(Op1);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::Mul:{
					if((ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isZero())||
						 (ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isZero())){
						Constant *zeroVal = ConstantInt::get(I.getType(),0);
						I.replaceAllUsesWith((Value *)zeroVal);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isOne()){
						I.replaceAllUsesWith(Op2);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isOne()){
						I.replaceAllUsesWith(Op1);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::UDiv:
				case Instruction::SDiv:{
					if(Op1==Op2){
						Constant *oneVal = ConstantInt::get(I.getType(),1);
						I.replaceAllUsesWith((Value *)oneVal);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isZero()){
						Constant *zeroVal = ConstantInt::get(I.getType(),0);
						I.replaceAllUsesWith((Value *)zeroVal);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isOne()){
						I.replaceAllUsesWith(Op1);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::URem:
				case Instruction::SRem:{
					if((Op1==Op2)||
						(ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isZero())||
						(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isOne())){
						Constant *zeroVal = ConstantInt::get(I.getType(),0);
						I.replaceAllUsesWith((Value *)zeroVal);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::Shl:
				case Instruction::LShr:
				case Instruction::AShr:{
					if(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isZero()){
						I.replaceAllUsesWith(Op1);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::And:{
					if((ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isZero())||
						 (ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isZero())){
						Constant *zeroVal = ConstantInt::get(I.getType(),0);
						I.replaceAllUsesWith((Value *)zeroVal);
						I.eraseFromParent();
						return true;
					}else if(Op1==Op2){
						I.replaceAllUsesWith(Op2);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::Or:{
					if((ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isMaxValue(false))||
						 (ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isMaxValue(false))){
						Constant *maxVal = ConstantInt::get(I.getType(),-1,false);
						I.replaceAllUsesWith((Value *)maxVal);
						I.eraseFromParent();
						return true;
					}else if(Op1==Op2){
						I.replaceAllUsesWith(Op2);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::Xor:{
					if(Op1==Op2){
						Constant *zeroVal = ConstantInt::get(I.getType(),0);
						I.replaceAllUsesWith((Value *)zeroVal);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op1)&&((ConstantInt *)Op1)->isZero()){
						I.replaceAllUsesWith(Op2);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&((ConstantInt *)Op2)->isZero()){
						I.replaceAllUsesWith(Op1);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				default:
					return false;
			}
			return false;
		}
		//Checks if the given binary operator instruction can do constant folding
		//If yes returns true and folds the instruction, else return false
		bool optConstFold(BinaryOperator &I){
			//First check that the given operation operates on two constant int values
			if(I.getNumOperands()==2){
				User *U = (User *)&I;
				if(ConstantInt::classof(U->getOperand(0))&&ConstantInt::classof(U->getOperand(1))){
					Constant *newValue = nullptr;
					//Some opcodes works differently based on signed or unsigned types
					if(I.getOpcode()==Instruction::SDiv||I.getOpcode()==Instruction::SRem||
						I.getOpcode()==Instruction::AShr){
						int64_t OprandA = ((ConstantInt *)(U->getOperand(0)))->getSExtValue();
						int64_t OprandB = ((ConstantInt *)(U->getOperand(1)))->getSExtValue();
						int64_t result;
						switch(I.getOpcode()){
							case Instruction::SDiv:
								result = OprandA/OprandB;
								break;
							case Instruction::SRem:
								result = OprandA%OprandB;
								break;
							case Instruction::AShr:
								result = OprandA>>OprandB;
								break;
							default:
								return false;
						}
						newValue = ConstantInt::get(I.getType(),result,true);
					}else{
						uint64_t OprandA = ((ConstantInt *)(U->getOperand(0)))->getZExtValue();
						uint64_t OprandB = ((ConstantInt *)(U->getOperand(1)))->getZExtValue();
						uint64_t result;
						switch(I.getOpcode()){
							case Instruction::Add:
								result = OprandA+OprandB;
								break;
							case Instruction::Sub:
								result = OprandA-OprandB;
								break;
							case Instruction::Mul:
								result = OprandA*OprandB;
								break;
							case Instruction::UDiv:
								result = OprandA/OprandB;
								break;
							case Instruction::URem:
								result = OprandA%OprandB;
								break;
							case Instruction::Shl:
								result = OprandA<<OprandB;
								break;
							case Instruction::LShr:
								result = OprandA>>OprandB;
								break;
							case Instruction::And:
								result = OprandA&OprandB;
								break;
							case Instruction::Or:
								result = OprandA|OprandB;
								break;
							case Instruction::Xor:
								result = OprandA^OprandB;
								break;
							default:
								return false;
						}
						newValue = ConstantInt::get(I.getType(),result,false);
					}
					I.replaceAllUsesWith(newValue);
					((Instruction &)I).eraseFromParent();
					return true;
				}
			}
			return false;
		}
		//Checks if the given binary operator instruction can do strength reduction
		//If yes returns true and reduce the instruction, else return false
		bool optStrenReduc(BinaryOperator &I){
			if(I.getNumOperands()!=2){
				//we only deal with two operand instructions here
				return false;
			}
			User *U = (User *)&I;
			Value *Op1=U->getOperand(0);
			Value *Op2=U->getOperand(1);
			unsigned logVal;
			switch(I.getOpcode()){
				case Instruction::Mul:{
					if(ConstantInt::classof(Op1)&&(logVal=getLog2(((ConstantInt *)Op1)->getZExtValue()))>0){
						Constant *sftVal = ConstantInt::get(I.getType(),logVal);
						BinaryOperator *nBO = BinaryOperator::Create(Instruction::Shl,Op2,sftVal);
						nBO->takeName(&I);
						I.replaceAllUsesWith(nBO);
						nBO->insertBefore(&I);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&(logVal=getLog2(((ConstantInt *)Op2)->getZExtValue()))>0){
						Constant *sftVal = ConstantInt::get(I.getType(),logVal);
						BinaryOperator *nBO = BinaryOperator::Create(Instruction::Shl,Op1,sftVal);
						nBO->takeName(&I);
						I.replaceAllUsesWith(nBO);
						nBO->insertBefore(&I);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::UDiv:{
					if(ConstantInt::classof(Op1)&&(logVal=getLog2(((ConstantInt *)Op1)->getZExtValue()))>0){
						Constant *sftVal = ConstantInt::get(I.getType(),logVal);
						BinaryOperator *nBO = BinaryOperator::Create(Instruction::LShr,Op2,sftVal);
						nBO->takeName(&I);
						I.replaceAllUsesWith(nBO);
						nBO->insertBefore(&I);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&(logVal=getLog2(((ConstantInt *)Op2)->getZExtValue()))>0){
						Constant *sftVal = ConstantInt::get(I.getType(),logVal);
						BinaryOperator *nBO = BinaryOperator::Create(Instruction::LShr,Op1,sftVal);
						nBO->takeName(&I);
						I.replaceAllUsesWith(nBO);
						nBO->insertBefore(&I);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				case Instruction::SDiv:{
					if(ConstantInt::classof(Op1)&&(logVal=getLog2(((ConstantInt *)Op1)->getZExtValue()))>0){
						Constant *sftVal = ConstantInt::get(I.getType(),logVal);
						BinaryOperator *nBO = BinaryOperator::Create(Instruction::AShr,Op2,sftVal);
						nBO->takeName(&I);
						I.replaceAllUsesWith(nBO);
						nBO->insertBefore(&I);
						I.eraseFromParent();
						return true;
					}else if(ConstantInt::classof(Op2)&&(logVal=getLog2(((ConstantInt *)Op2)->getZExtValue()))>0){
						Constant *sftVal = ConstantInt::get(I.getType(),logVal);
						BinaryOperator *nBO = BinaryOperator::Create(Instruction::AShr,Op1,sftVal);
						nBO->takeName(&I);
						I.replaceAllUsesWith(nBO);
						nBO->insertBefore(&I);
						I.eraseFromParent();
						return true;
					}
					break;
				}
				default:
					return false;
			}
			return false;
		}
		//Returns 0 is the given number is not an exponential of 2
		//else return the Log2 value of the given number
		unsigned getLog2(unsigned num){
			//see if is exponential of 2
			if(num&(num-1)!=0){
				return 0;
			}
			unsigned result = 0;
			while(num){
				result++;
				num=num>>1;
			}
			result--;
			return result;
		}
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
static RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizing", false, false);
