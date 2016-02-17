#include <stdio.h>
#include <stdlib.h>	
int main(int argc, char** argv)
{
	int x = atoi(argv[1]);
		int a,b,c;
	while (x > 0)
	{
	
		a = 10; b = 20; c = 90;
		int z = a - 2;
		c = c + 2;
		int d = a + b - c;
		if (d < 10)
		{
			int l = a + c;
			a = 4;
		}
		else
		{
			int m =  b - c;
			b = b - c;
		}
		x = x -1;
	}
	int j = b + 2;
	printf("%d",a);
	return 0;
	
}
