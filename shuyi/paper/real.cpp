#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <queue>
#include <algorithm>
#define MAXN 15
#define INF (1<<20)

int show();
int create_pot(int n);
int create_line(int id, int x, int y, const char *str = NULL);

using namespace std;
struct price_all{
	vector<int> path;
	int time_for_courier;
	int avg_time_for_customer;
	int loss;
};

//from low to high [begin, end)
struct customer_loss_range{
	int begin;
	int end;
	int cost;
};
vector<customer_loss_range> clr;
struct price_all p_for_courier;
struct price_all p_for_customer;
struct price_all p_for_less_loss;
int num_of_pot, dis[MAXN][MAXN];
vector<int> floyd_path[MAXN][MAXN];
int num_of_road;
int courier_cost_per_hour = 1;
int num_of_range;
int init()
{
	memset(&p_for_courier, 0, sizeof(p_for_courier));
	memset(&p_for_customer, 0, sizeof(p_for_customer));
	memset(&p_for_less_loss, 0, sizeof(p_for_less_loss));

	p_for_courier.time_for_courier = INF * MAXN;
	p_for_customer.avg_time_for_customer = INF * MAXN;
	p_for_less_loss.loss = INF * MAXN;
	int i = 0, j = 0;
	for(i=0; i < MAXN; i++)
		for(j=0; j < MAXN; j++){
			dis[i][j] = INF;
			if(i == j) 
				dis[i][j] = 0;
		}

	for(i = 0; i < MAXN; i++)
		for(j = 0; j < MAXN; j++){
			floyd_path[i][j].push_back(i);
			floyd_path[i][j].push_back(j);
		}
	return 0;
}

int get_customer_loss_range()
{
	scanf("%d", &courier_cost_per_hour);
	scanf("%d", &num_of_range);
	struct customer_loss_range tmp;
	for(int i = 0; i < num_of_range; i++){
		scanf("%d%d%d", &tmp.begin, &tmp.end, &tmp.cost);
		if(tmp.begin >= tmp.end){
			fprintf(stderr, "begin must small than end !!!\n");
			exit(1);
		}
		if(i > 0 && tmp.begin != clr[clr.size() - 1].end){
			fprintf(stderr, "Price range data out of order !!!\n");
		}
		clr.push_back(tmp);
	}
	return 0;
}

int show_vector_path(vector<int> &path)
{
	static int id = 100007;
	static char buf[1024];
	for(int i = 0; i < path.size() - 1; i++){
		sprintf(buf, "%d->%d:dis %d", path[i], path[i+0], dis[path[i]][path[i+1]]);
		sleep(3);
		create_line(id++, path[i], path[i + 1], buf);
	}
	for(int i = 1; i < path.size(); i++)
		printf("->%d", path[i]);
	return 0;
}


int show_ans(struct price_all p_a, const char *p = NULL)
{
	printf("%s\n", p);
	printf("Courier spend:%d\n", p_a.time_for_courier * courier_cost_per_hour);
	printf("Avg customer wait time:%d\n", p_a.avg_time_for_customer/(num_of_pot - 1));
	printf("Loss:%d\n", p_a.loss);
	printf("Path(Total):0");
	for(int i = 0; i < p_a.path.size(); i++)
		printf("->%d", p_a.path[i]);
	printf("->0\n");
	printf("Path detail:0");
	show_vector_path(floyd_path[0][p_a.path[0]]);
	for(int i = 1; i < p_a.path.size() ; i++){
		show_vector_path(floyd_path[p_a.path[i - 1]][p_a.path[i]]);
	}
	show_vector_path(floyd_path[p_a.path[p_a.path.size() - 1]][0]);
	printf("\n");
	return 0;
}

int merge_path(vector<int> &a, vector<int> &b, vector<int> &c)
{
	a.clear();
	a = b;
	for(int i = 1; i < c.size(); i++)
		a.push_back(c[i]);
	return 0;
}

int main()
{
	int a = 0, b = 0, r_len = 0, i = 0, j = 0, k = 0;
	init();
	/*输入顶点的个数，包括起点，起点编号为0*/
	scanf("%d", &num_of_pot);
	create_pot(num_of_pot);
	if(num_of_pot > MAXN){
		fprintf(stderr, "Too much pot here !!!\n");
		exit(1);
	}
	/*输入路径条数*/
	scanf("%d", &num_of_road);
	/*输入路径，从节点a 到节点 b的长度为r_len*/
	for(i = 0; i < num_of_road; i++){
		scanf("%d%d%d", &a, &b, &r_len);
		if(a >= MAXN || b >= MAXN || a < 0 || b < 0){
			fprintf(stderr, "Illegal pot number !!!\n");
			exit(1);
		}
		dis[a][b] = r_len;
	} 
	get_customer_loss_range();
	for(k = 0; k < num_of_pot; k++)
		for(i = 0; i < num_of_pot; i++)
			for(j = 0; j < num_of_pot; j++){
				if(dis[i][j] > dis[i][k] + dis[k][j]){
					dis[i][j] = dis[i][k] + dis[k][j];
					merge_path(floyd_path[i][j], floyd_path[i][k], floyd_path[k][j]);
				}
			}
	/*
	for(i=0; i < num_of_pot; i++){
		for(j = 0; j < num_of_pot; j++){
			printf("%d ", dis[i][j]);
		}
		printf("\n");
	}*/
	vector<int> permutation;
	for(i = 1; i < num_of_pot; i++) 
		permutation.push_back(i);
	struct price_all tmp;
	int pos = 0;
	do{
		
		int dis_for_courier = dis[0][permutation[0]];
		int avg_time_for_customer = 0, wait_time = dis[0][permutation[0]];
		int loss = 0;//Just stand here,use later
		avg_time_for_customer += wait_time;

		if(wait_time < clr[0].begin || wait_time >= clr[clr.size() - 1].end){
			fprintf(stderr, "Customer time cost out of range\n");
			exit(1);
		}

		while(pos < clr.size() -1){
			if(wait_time >= clr[pos].begin && wait_time < clr[pos].end)
				break;
			else 
				pos ++;
		}

		loss += clr[pos].cost;
		for(i = 1; i < num_of_pot - 1; i++){
			dis_for_courier += dis[permutation[i - 1]][permutation[i]];
			wait_time += dis[permutation[i - 1]][permutation[i]];
			avg_time_for_customer += wait_time;
			if(wait_time < clr[0].begin || wait_time >= clr[clr.size() - 1].end){
				fprintf(stderr, "Customer time cost out of range\n");
				exit(1);
			}
			while(pos < clr.size() -1){
				if(wait_time >= clr[pos].begin && wait_time < clr[pos].end)
					break;
				else 
					pos ++;
			}
			loss += clr[pos].cost;
		}

		dis_for_courier += dis[permutation[permutation.size() - 1]][0];
		loss += dis_for_courier * courier_cost_per_hour;
		tmp.path = permutation;
		tmp.time_for_courier = dis_for_courier;
		tmp.loss = loss;
		tmp.avg_time_for_customer = avg_time_for_customer;

		if(dis_for_courier < p_for_courier.time_for_courier){
			p_for_courier = tmp;
		}

		if(avg_time_for_customer < p_for_customer.avg_time_for_customer){
			p_for_customer = tmp;
		}

		if(loss < p_for_less_loss.loss){
			p_for_less_loss = tmp;
		}
		/*permutation genernate algorithm in STL 全排*/
	}while(next_permutation(permutation.begin(), permutation.end()));

	show_ans(p_for_courier, "Best path for courier");
	show_ans(p_for_customer, "Best path for customer");
	show_ans(p_for_less_loss, "Best path for both");
	return 0;
}


