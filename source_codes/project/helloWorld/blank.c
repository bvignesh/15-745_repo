#include <stdio.h>
#include <stdlib.h>

int arg1;

int main(int argc, char **argv){
	int i=128;
	i=argc;
	//i=atoi(argv[1]);
	printf("Beep [%d]\n",i);
	return 250;
}
