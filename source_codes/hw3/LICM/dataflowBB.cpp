// 15-745 S16 Assignment 2: dataflow.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////

#include "dataflowBB.h"

namespace llvm {

  // Create the analysis object
  DataFlowAnalysisBB::DataFlowAnalysisBB(char &ID) : FunctionPass(ID){
  	//Reset inner data elements
  	resetObject();
  }

  //The main analysis pass
  bool DataFlowAnalysisBB::runOnFunction(Function& F){
  
  	//clear all the information for the current function
  	resetObject();
  
  	//First do a scan of the function to know how many program pointers and what the domain is
		for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
			BasicBlock* block = FI;
			//Add each basic block to the domain
			checkAndAddToDomain(block);
			//Map where are the entrance and exit for this basic block are
			BBinfoMap[block]={ppNum, ppNum+1, std::vector<Value *>()};
			//There is an extra program point after a basic block
			ppNum=ppNum+2;;
		}
		


		//Next init the programPoints, we now know the domain size, aka the BitVector Size
		//set the initial value for the BitVector
  	BitVector initVal = BitVector(domainSize, initialInterior());
  	//and create the BitVector
  	programPoints = std::vector<BitVector>(ppNum,initVal);
  	//boundry value
  	BitVector bdyVal = BitVector(domainSize, boundaryCondition());
  	
  	//Then is the main part, the actual analysis pass
  	if(isForwardPass()){
  		//The direction is forward
  		programPoints[0]=bdyVal;
  		//keep iterating until nothing changes
  		while(forwardPass(F)){
  		  //printResult(F);
  		  //outs() << "\n\n";
  		}
  	}else{
  		//The direction is backward
  		programPoints[ppNum-1]=bdyVal;
  		//keep iterating until nothing changes
  		while(backwardPass(F)){
  		  //printResult(F);
  		  //outs() << "\n\n";
  		}
  	}
  	
  	//prints the result
  	printResult(F);
  	//Analysis pass does not change anything
  	return false;
  }
  
  		
	void DataFlowAnalysisBB::getAnalysisUsage(AnalysisUsage& AU) const {
    AU.setPreservesAll();
  }
	
	BitVector DataFlowAnalysisBB::genKillTransferFunction(BitVector &lastResult, Value &currentInst, 
						std::function<std::vector<Value *> (Value&)> genFunc, std::function<bool (Value&, Value&)> killFunc,
						std::vector<Value *> &Domain){
		BitVector currentResult = BitVector(lastResult);
		
		//See if there are any elements to kill
		for(int i=0;i<currentResult.size();i++){
			if(currentResult[i]){
				//If the current bit is set, sees if it can remain to be set
				if(killFunc(currentInst,*(Domain[i]))){
					//If the kill function decides to kill this element
					currentResult[i]=false;
				}
			}
		}
		
		//See if there are any elements to add
		std::vector<Value *> genVals = genFunc(currentInst);
		for(Value *toAdd : genVals){
			auto index = std::find(Domain.begin(), Domain.end(), toAdd);
			if(index!=Domain.end()){
			  currentResult[index-Domain.begin()]=true;
			}
		} 
		
		return currentResult;
	}
  
  //reset the pass inner data
	void DataFlowAnalysisBB::resetObject(){
		Domain.clear();
		domainSize=0;
		programPoints.clear();
		ppNum=0;
		BBinfoMap.clear();
	}
	
	//Checks if the giving value is not already in domian, if not then add to domain
	void DataFlowAnalysisBB::checkAndAddToDomain(Value *v){
		if(std::find(Domain.begin(),Domain.end(),v)==Domain.end()){
			//Not already in Domain add it to domain
			Domain.push_back(v);
			domainSize++;
		}
	}
	
	//does a forward pass return true if something have been updated
	bool DataFlowAnalysisBB::forwardPass(Function& F){
		int currentppNum=0;
		bool changed = false;
		for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
			BasicBlock* block = FI;
			if(BBinfoMap[block].beginIndx!=0){
				//If the basic block isn't the first basic block, we have to get entry value
				BitVector newVal = BitVector(domainSize, initialInterior());
				for (pred_iterator PI = pred_begin(block), E = pred_end(block); PI != E; ++PI) {
					BasicBlock *Pred = *PI;
					//merge the values with the exit values of all the predecessors
					newVal=meet(newVal, programPoints[BBinfoMap[Pred].endIndx]);
				}
				//See if anything have changed
				changed |= (newVal!=programPoints[currentppNum]);
				//update the might be new values
				programPoints[currentppNum]=newVal;
			}
			//Transfer function computes the exit point of each instruction
			currentppNum++;
			//Gets the new value of the current program point
			BitVector newVal = transferFunction(programPoints[currentppNum-1], *block);
			//See if anything have changed
			changed |= (newVal!=programPoints[currentppNum]);
			//update the might be new values
			programPoints[currentppNum]=newVal;
			//There is a program point for every instruction
			currentppNum++;
		}
		return changed;
	}
	
	//does a backward pass return true if somethinng have been updated
	bool DataFlowAnalysisBB::backwardPass(Function& F){
		int currentppNum=ppNum;
		bool changed = false;
		for (Function::iterator FI = F.end(), FE = F.begin(); FI != FE;) {
			//decrements here as no reverse iterator is provided and can follow the end begin comparison
			--FI;
			BasicBlock* block = FI;
			//Transfer function computes the entry point of each instruction
			--currentppNum;
			if(BBinfoMap[block].endIndx!=ppNum-1){
				//If the basic block isn't the last basic block, we have to get exit value
				BitVector newVal = BitVector(domainSize, initialInterior());
				for (succ_iterator PI = succ_begin(block), E = succ_end(block); PI != E; ++PI) {
					BasicBlock *Succ = *PI;
					//merge the values with the entry values of all the successors
					newVal=meet(newVal, programPoints[BBinfoMap[Succ].beginIndx]);
				}
				//See if anything have changed
				changed |= (newVal!=programPoints[currentppNum]);
				//update the might be new values
				programPoints[currentppNum]=newVal;
			}
			//There is a program point for every instruction
			--currentppNum;
			//Gets the new value of the current program point
			BitVector newVal = transferFunction(programPoints[currentppNum+1], *block);
			//See if anything have changed
			changed |= (newVal!=programPoints[currentppNum]);
			//update the might be new values
			programPoints[currentppNum]=newVal;
		}
		return changed;
	}
	//prints out the result of the set
	void DataFlowAnalysisBB::printSet(BitVector& BV){
		outs() << "{";
		bool isFirst = true;
		for(int i=0;i<domainSize;i++){
			if(BV[i]){
				if(isFirst){
					isFirst=false;
				}else{
					outs() << " , ";
				}
				outs() << toString(*(Domain[i]));
			}
		}
		outs() << "}\n";
	}
	
	void DataFlowAnalysisBB::printResult(Function& F){
		int currentppNum=0;
		if(!isForwardPass()){
			currentppNum++;
		}
		for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
			BasicBlock* block = FI;
			BitVector BV = programPoints[currentppNum];
			for(int i=0;i<domainSize;i++){
				if(BV[i]){
					outs() << toString(*(Domain[i])) << " " <<toString(F) << " " << toString(*block) <<"\n";
				}
			}
			currentppNum=currentppNum+2;
		}
	}
	
}
