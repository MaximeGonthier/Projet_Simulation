#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define Lambda 0.9
#define Mu 1
#define N 10

void intensiteTrafficM1(){
	double p, p0, rho;
	for (int i = 0; i < 10; ++i)
	{
		p = (double) i/(N*Mu);
		printf("intensité pour %d : %lf\n",i,p);
	}
}

int main(){
	intensiteTrafficM1();
	return 0;
}