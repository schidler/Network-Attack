#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;
int main()
{
	srand(time(NULL));
	int i  = 0;
	printf("%d\n" , 11);
	for(i = 0; i < 11; i++)
		printf("%d %d\n", rand() % 800, rand() % 500);
	printf("%d\n" , 40);
	for(i = 0; i < 40; i++)
		printf("%d %d\n", rand() % 11, rand() % 11);
	return 0;
}
