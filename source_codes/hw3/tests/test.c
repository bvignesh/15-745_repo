
int multiply(int a, int b){
	int i,j,k;
	int answer=0;
	int AisPos=0;
	int BisPos;
	for(i=0;i<a;i++){
		AisPos=1;
		k=2;
		for(j=0;j<b;j++){
			int temp1=i;
			int temp2=b;
			temp1=temp1+5;
			temp2=temp2+5;
			BisPos=temp2/temp1;
			answer++;
		}
	}
	answer=answer*BisPos;
	return answer;
}

int main(){
	return multiply(42,24);
}
