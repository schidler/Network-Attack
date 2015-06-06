#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;
int main()
{
	ofstream data_file("data");
	srand(time(NULL));
	int i  = 0, tmp = 0, num_of_pot = 0;
	num_of_pot = 11;//rand() % 7 + 5;
	data_file<<num_of_pot<<endl;
	for(i = 0; i < num_of_pot; i++)
		data_file<<rand() % 800<<' '<<rand() % 500<<endl;
	tmp = rand() % ((num_of_pot * num_of_pot)*2/3);
	tmp += 5;
	data_file<<tmp<<endl;
	for(i = 0; i < tmp; i++)
		data_file<<rand() % num_of_pot<<' '<<rand() % num_of_pot<<endl;
	data_file<<rand() % 10<<endl;
	tmp = rand() % 20;
	tmp += 5;
	data_file<<tmp<<endl;
	for(i = 0; i < tmp - 1; i++){
		data_file<<i * 50<<' '<<(i + 1)*50<<' '<< i<<endl;
	}
	data_file<<i*50<<' '<<(i+10000)*50<<' '<<i<<endl;
	return 0;
}
