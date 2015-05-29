#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;
int main()
{
	srand(time(NULL));
	int i  = 0, tmp = 0;
	printf("%d\n" , 11);
	for(i = 0; i < 11; i++)
		printf("%d %d\n", rand() % 800, rand() % 500);
	tmp = rand() % 100;
	tmp += 5;
	printf("%d\n" , tmp);
	for(i = 0; i < tmp; i++)
		printf("%d %d\n", rand() % 11, rand() % 11);
	printf("%d\n", rand() % 10);
	tmp = rand() % 20;
	tmp += 5;
	printf("%d\n" ,tmp);
	for(i = 0; i < tmp - 1; i++){
		printf("%d %d %d\n", i * 50, (i + 1)*50, i);
	}
	printf("%d %d %d\n", i*50, (i+1000)*50, i);
	return 0;
}
