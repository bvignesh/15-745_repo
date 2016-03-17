// 15-745 S15 Assignment 1: FunctionInfo.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/ValueTracking.h"

#include <iostream>
#include <functional>

#include "dominators.cpp"

using namespace llvm;

namespace {
  class LICM : public LoopPass {
  public:
    static char ID;
    LICM() : LoopPass(ID) { }
    ~LICM() { }

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
    	AU.addRequired<LoopInfo>();
    	AU.addRequired<Dominators>();
      //AU.setPreservesAll();
    }

    // Do some initialization
    bool doInitialization (Loop *L, LPPassManager &LPM) override {
    	//outs() << "init\t\t\t\t\t" << *L;
      return false;
    }

    // Print output for each function
    bool runOnLoop (Loop *L, LPPassManager &LPM) override {
    	LoopInfo &LI=getAnalysis<LoopInfo>();
    	Dominators &Dom=getAnalysis<Dominators>();
    	BasicBlock *pre = L->getLoopPreheader();
    	BasicBlock *head = L->getHeader();
    	SmallVector<BasicBlock *,5> ExitBlocks;
    	if(pre==NULL){
    		//outs() << "Got no loop preheader!\n";
    		return false;
    	}else{
    		//outs() << "Got preheader:\t" << *pre;
    	}
    	if(head==NULL){
    		//outs() << "Got no header!\n";
    		return false;
    	}else{
    		//outs() << "Got header:\t" << *head;
    	}
    	L->getExitingBlocks(ExitBlocks);
    	//outs() << "Processing..................................................\n";
    	std::vector<Instruction *>invariantInstruction;
    	bool moreInvInst = true;
    	while(moreInvInst){
		  	moreInvInst = false;
		  	for (Loop::block_iterator BI = L->block_begin(), BE = L->block_end();BI != BE; ++BI) {
					BasicBlock *loopBB = *BI;
					if(LI.getLoopFor(loopBB)==L){
						for (BasicBlock::iterator BBi = loopBB->begin(), BBe = loopBB->end(); BBi!=BBe; ++BBi){
							Instruction *Inst = BBi;
							if(std::find(invariantInstruction.begin(), invariantInstruction.end(), Inst)!=invariantInstruction.end()||
								isa<BranchInst>(Inst)||isa<PHINode>(Inst)){
								continue;
							}
							bool isInvar = true;
							for(int i=0, len=Inst->getNumOperands(); i<len; ++i){
								Value *val=Inst->getOperand(i);
								if(Instruction *operand = dyn_cast<Instruction>(val)){
									if(std::find(invariantInstruction.begin(), invariantInstruction.end(), operand)==invariantInstruction.end()&&
										LI.getLoopFor(operand->getParent())==L){
											//outs() << Inst->getName() << " is not loop invariant due to " << operand->getName() << "\n";
											isInvar=false;
											break;
										}
								}
							}
							if(isInvar){
								invariantInstruction.push_back(Inst);
								moreInvInst=true;
							}
						}
				  }
				}
			}
			std::vector<Instruction *>canMove;
			for(Instruction *Inst : invariantInstruction){
				bool canBeMoved=true;
				//Do as the handout says
				if(!isSafeToSpeculativelyExecute(Inst)||Inst->mayReadFromMemory()||isa<LandingPadInst>(Inst)){
					canBeMoved=false;
					continue;
				}
				//See if it dominates all exit blocks(the head can't be dominated obviously)
				for(BasicBlock *exitBlock : ExitBlocks){
					if(exitBlock!=head){
						if(!Dom.getPassInfo(exitBlock,Inst->getParent())){
							//outs() << Inst->getName() << " cant be moved not dominating " << exitBlock->getName() << "\n";
							canBeMoved=false;
							break;
						}
					}else{
						for (auto it = pred_begin(head), et = pred_end(head); it != et; ++it){
							BasicBlock* predecessor = *it;
							if(LI.getLoopFor(predecessor)==L){
								if(!Dom.getPassInfo(predecessor,Inst->getParent())){
									//outs() << Inst->getName() << " cant be moved not dominating " << exitBlock->getName() << "\n";
									canBeMoved=false;
									break;
								}
							}
						}
					}
				}
				if(!canBeMoved){
					continue;
				}
				//See if have PHINode with another use defined in loop
				for(auto UI = Inst->use_begin(), UE = Inst->use_end(); UI!=UE; ++UI){
					if (PHINode *phi = dyn_cast<PHINode>(*UI)) {
						for(int i=0, len = phi->getNumIncomingValues(); i<len; ++i){
					    BasicBlock *phiBB = phi->getIncomingBlock(i);
					    if (phiBB != Inst->getParent() ){
								//Other definitions of this instruction occured in the loop
								if(LI.getLoopFor(phiBB)==L){
									//outs() << Inst->getName() << " can't be moved other def in " << phiBB->getName() << "\n";
									canBeMoved=false;
									break;
								}
							}
					  }
					}
				}
				if(canBeMoved){
					canMove.push_back(Inst);
				}
			}
			/*		
			outs() << "Invariant instructions:\n";
			for(auto inst : invariantInstruction){
				outs() << *inst <<"\n";
			}
    	outs() << "Invariant instruction end\n";
    	outs() << "canMove instructions:\n";
			for(auto inst : canMove){
				outs() << *inst <<"\n";
			}
    	outs() << "canMove instruction end\n";
    	*/
    	
			//We only change the structure if there is something to be optimized
			if(canMove.size()>0){
				
				/*
				ValueToValueMapTy VMap;
				BasicBlock *preCond = CloneBasicBlock(head, VMap,"Landpad",head->getParent());
				BasicBlock *preHead = CloneBasicBlock(pre, VMap,"pre",head->getParent());
				preCond->moveBefore(head);
				BasicBlock *landpad = BasicBlock::Create(preCond->getContext(),"landpad",head->getParent(),head);
				preHead->moveBefore(head);
				std::vector<PHINode *>toRemove;
				//Remove all the PHINodes from the cloned cond basic block
				for (BasicBlock::iterator BBi = preCond->begin(), e = preCond->end(); BBi!=e; ++BBi) {
					//Each instruction is a value
					Instruction *Inst = BBi;
					if(PHINode *phi = dyn_cast<PHINode>(Inst)){
						toRemove.push_back(phi);
					}else if(BranchInst *br = dyn_cast<BranchInst>(Inst)){
						//If the instruction is a branch instruction
						outs() << "got branch inst:" << *br <<"\n";
						for(int i=0, len = br->getNumSuccessors(); i<len; ++i){
					    BasicBlock *brBB = br->getSuccessor(i);
					    if(LI.getLoopFor(brBB)==L){
					    	br->setSuccessor(i,landpad);
					    }else{
					    	br->setSuccessor(i,preHead);
					    }
					  }
					}
					if(User *user = dyn_cast<User>(Inst)){
						//If the Value is a user (Instruction?)
						for(int i=0, len=user->getNumOperands(); i<len; ++i){
							Value *val=user->getOperand(i);
							if((val=VMap[val])!=NULL){
								user->setOperand(i, val);
							}
						}
					}
				}
				for(PHINode *phi : toRemove){
					Value *subValue=NULL;
					for(int i=0, len = phi->getNumIncomingValues(); i<len; ++i){
				    BasicBlock *phiBB = phi->getIncomingBlock(i);
				    if (phiBB == pre){
							subValue=phi->getIncomingValue(i);
							break;
						}
				  }
				  if(subValue!=NULL){
				  	phi->replaceAllUsesWith(subValue);
						phi->eraseFromParent();
					}
				}
				//Fix the PHINodes of the cond basic block
				for (BasicBlock::iterator BBi = head->begin(), e = head->end(); BBi!=e; ++BBi) {
					//Each instruction is a value
					Value *Val = BBi;
					if(PHINode *phi = dyn_cast<PHINode>(Val)){
						//If the instruction is a phinode
						//Value *incoming = NULL;
					  for(int i=0, len = phi->getNumIncomingValues(); i<len; ++i){
					    BasicBlock *phiBB = phi->getIncomingBlock(i);
					    if (phiBB == pre){
								phi->setIncomingBlock(i,preHead);
								//incoming=phi->getIncomingValue(i);
							}
					  }
					  //if(incoming!=NULL){
					  //	phi->addIncoming(Val, preCond);
					  //}
					}else{
						break;
					}
				}
				//Build the landpad basicblock
				*/
				//Add the instructions to the preheader
				Instruction *lastInst = &(pre->back());
				for(Instruction *inst : canMove){
					inst->removeFromParent();
					//landpad->getInstList().push_back(inst);
					inst->insertBefore(lastInst);
				}
				/*
				//And make it jump to the cond
				BranchInst::Create(preHead,landpad);
				//Change the prehead to jump to the cond of the landpad
				for (BasicBlock::iterator BBi = pre->begin(), e = pre->end(); BBi!=e; ++BBi) {
					//Each instruction is a value
					Value *Val = BBi;
					if(BranchInst *br = dyn_cast<BranchInst>(Val)){
						//If the instruction is a branch instruction
						for(int i=0, len = br->getNumSuccessors(); i<len; ++i){
							BasicBlock *brBB = br->getSuccessor(i);
							if (brBB == head ){
								br->setSuccessor(i,preCond);
							}
						}
					}
				}
				//Finially we add the newly added BasicBlock
				Loop *parentLoop = LI.getLoopFor(pre);
				if(parentLoop!=NULL){
					parentLoop->addBasicBlockToLoop(preCond,LI.getBase());
					parentLoop->addBasicBlockToLoop(preHead,LI.getBase());
					parentLoop->addBasicBlockToLoop(landpad,LI.getBase());
				}
				*/
				/*
			  outs() << "~~~~~~Function~~~~~~\n";
	
				outs() << *(L->getHeader()->getParent());
				outs() << "~~~~~~Function Ends~~~~~~\n";
				*/
				return true;
			}
			//outs() << "~~~~~~Nothing changed~~~~~~\n";
			
      return false;
    }
	private:
		Module *myModule;
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
				retVal |= action(myIter);
				if(myPtr==myLastPtr){
					break;
				}
				myPtr = givenList.getNext(myPtr);
			}
			return retVal;
		}
  };
}

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LICM::ID = 0;
static RegisterPass<LICM> X("loop-invariant-code-motion", "15745: Loop Invariant Code Motion", false, false);
