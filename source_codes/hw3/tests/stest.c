
int multiply(int a, int b){
	int i,j;
	int answer=0;
	int AisPos=0;
	for(i=0;i<a;i++){
		AisPos=1;
		answer=answer+b;
	}
	answer=answer*AisPos;
	return answer;
}

int main(){
	return multiply(42,24);
}
