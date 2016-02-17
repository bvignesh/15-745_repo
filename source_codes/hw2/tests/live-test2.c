#include <stdio.h>

void main()
{
	int x = 100;
	int a,c,d,b;
	switch(x){
	case 1:
		a = 2;
		b = a + 2;
		d = b + a;
		int g = c + b + a + d;
		b = 4;
		break;
	case 2:
		d = b - 1;
		c = 3;
		b = b + 4;
		c = a + b; 
		break;
	default:
		b = d*a;
	}
	a = b + c;
	c = c + d;
	x = x - d;
	return;
}
