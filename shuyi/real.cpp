#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <queue>
#include <algorithm>
#define MAXN 15
#define INF (1<<20)
using namespace std;
struct price_all{
	vector<int> path;
	int time_for_courier;
	int avg_time_for_customer;
	int loss;
};

struct price_all p_for_courier;
struct price_all p_for_customer;
struct price_all p_for_less_loss;
int num_of_pot, dis[MAXN][MAXN];
int num_of_road;
int courier_cost_per_hour = 1;

int init()
{
	memset(&p_for_courier, 0, sizeof(p_for_courier));
	memset(&p_for_customer, 0, sizeof(p_for_customer));
	memset(&p_for_less_loss, 0, sizeof(p_for_less_loss));
	p_for_courier.time_for_courier = INF*MAXN;
	p_for_customer.avg_time_for_customer = INF*MAXN;
	p_for_less_loss.loss = INF*MAXN;
	int i = 0, j = 0;
	for(i=0; i < MAXN; i++)
		for(j=0; j < MAXN; j++){
			dis[i][j] = INF;
		}
	return 0;
}
int show_ans(struct price_all p_a)
{
	printf("Courier spend:%d\n", p_a.time_for_courier * courier_cost_per_hour);
	printf("Avg customer wait time:%d\n", p_a.avg_time_for_customer/(num_of_pot - 1));
	printf("Loss:%d\n", p_a.loss);
	printf("Path:0");
	for(int i = 0; i < p_a.path.size(); i++)
		printf("->%d", p_a.path[i]);
	printf("->0\n\n");
	return 0;
}
int main()
{
	int a = 0, b = 0, r_len = 0, i = 0, j = 0, k = 0;
	init();
	/*输入顶点的个数，包括起点，起点编号为0*/
	scanf("%d", &num_of_pot);
	if(num_of_pot > MAXN){
		fprintf(stderr, "Too much pot here!!!\n");
		exit(1);
	}
	/*输入路径条数*/
	scanf("%d", &num_of_road);
	/*输入路径，从节点a 到节点 b的长度为r_len*/
	for(i = 0; i < num_of_road; i++){
		scanf("%d%d%d", &a, &b, &r_len);
		if(a >= MAXN || b >= MAXN || a < 0 || b < 0){
			fprintf(stderr, "Illegal pot number!!!\n");
			exit(1);
		}
		dis[a][b] = r_len;
	}
	for(k = 0; k < num_of_pot; k++)
		for(i = 0; i < num_of_pot; i++)
			for(j = 0; j < num_of_pot; j++){
				dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
			}

	vector<int> permutation;
	for(i = 1; i < num_of_pot; i++) 
		permutation.push_back(i);
	struct price_all tmp;
	do{
		int dis_for_courier = dis[0][permutation[0]];
		int avg_time_for_customer = 0, now_time = dis[0][permutation[0]];
		int loss = 0;//Just stand here,use later
		avg_time_for_customer += now_time;
		for(i = 1; i < num_of_pot; i++){
			dis_for_courier += dis[permutation[i - 1]][permutation[i]];
			now_time += dis[permutation[i - 1]][permutation[i]];
			avg_time_for_customer += now_time;
		}

		tmp.path = permutation;
		tmp.time_for_courier = dis_for_courier;
		tmp.loss = loss;
		tmp.avg_time_for_customer = avg_time_for_customer;

		if(dis_for_courier < p_for_courier.time_for_courier){//Here can be a function
			p_for_courier = tmp;
		}

		if(avg_time_for_customer < p_for_customer.avg_time_for_customer){
			p_for_customer = tmp;
		}

		if(loss < p_for_less_loss.loss){
			p_for_less_loss = tmp;
		}
	}while(next_permutation(permutation.begin(), permutation.end()));

	show_ans(p_for_courier);
	show_ans(p_for_customer);
	show_ans(p_for_less_loss);
	return 0;
}


