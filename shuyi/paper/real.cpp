#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>
#include <string.h>
#include <queue>
#include <algorithm>
#define MAXN 15
#define INF (1<<20)
using namespace std;
int draw_point(const char *context, int center_x, int center_y, int radius, int line_width);

int draw_arrow(int x1, int y1, int x2, int y2, int width = 1, const char *color = "#8ED6FF", const char *linecolor = "blue");
int draw_text(int x, int y, int font, const char *text);
int begin_draw();
int end_draw();

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

struct point{
	int x;
	int y;
};
int my_dis(point a, point b);
vector<point> all_point;
vector<point> path;
vector<int> path_ans;

vector<customer_loss_range> clr;
struct price_all p_for_courier;
struct price_all p_for_customer;
struct price_all p_for_less_loss;
int num_of_pot, dis[MAXN][MAXN];
vector<int> floyd_path[MAXN][MAXN];
int num_of_road;
int courier_cost_per_hour = 1;
int num_of_range;
int max_path_length = 0;

const char *context = "context1";
int draw_figure()
{
	static char buf[1024];
	int i = 0;
	for(i = 0; i < all_point.size(); i++){
		draw_point(context, all_point[i].x, all_point[i].y, 10, 3);
		if(0 == i)
			sprintf(buf, "Original:%d", i);
		else
			sprintf(buf, "%d", i);
		draw_text(all_point[i].x, all_point[i].y - 20, 20, buf);
	}
	return 0;
}

int draw_all_line()
{
	int i = 0;
	static char buf[1024];
	point a, b;
	for(i = 0; i < path.size(); i ++){
		a = all_point[path[i].x];
		b = all_point[path[i].y];
		draw_arrow(a.x, a.y, b.x, b.y, 2);//, "#000000", "red");
		sprintf(buf,"dis:%d", my_dis(a, b));
		draw_text(a.x + (b.x - a.x) * 2 / 3, a.y + (b.y - a.y) * 2 / 3, 2, buf);
	}
	return 0;
}
int init()
{
	memset(&p_for_courier, 0, sizeof(p_for_courier));
	memset(&p_for_customer, 0, sizeof(p_for_customer));
	memset(&p_for_less_loss, 0, sizeof(p_for_less_loss));

	p_for_courier.time_for_courier = INF * MAXN;
	p_for_customer.avg_time_for_customer = INF * MAXN;
	p_for_less_loss.loss = INF * MAXN;
	max_path_length = 0;
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
			fprintf(stderr, "Begin must small than end !!!\n");
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
	for(int i = 1; i < path.size(); i++){
		path_ans.push_back(path[i]);
		printf("->%d", path[i]);
	}
	return 0;
}

int draw_last_real_ans()
{
	static bool flag = true;
	if(!flag)
		return 0;
	flag = false;
	static char buf[1024];
	int i = 0;
	for(i = 1; i < path_ans.size(); i++){
		draw_arrow(all_point[path_ans[i-1]].x, all_point[path_ans[i-1]].y, all_point[path_ans[i]].x, all_point[path_ans[i]].y, 5, "#000000", "red" );
		sprintf(buf, "%d->%d", path_ans[i-1], path_ans[i]);
		draw_text((all_point[path_ans[i-1]].x + all_point[path_ans[i]].x)/2, (all_point[path_ans[i-1]].y + all_point[path_ans[i]].y)/2, 15, buf);
	}
	return 0;
}

int show_ans(struct price_all p_a, const char *p = NULL)
{
	path_ans.clear();
	printf("%s\n", p);
	printf("Courier spend:%d\n", p_a.time_for_courier * courier_cost_per_hour);
	printf("Avg customer wait time:%d\n", p_a.avg_time_for_customer/(num_of_pot - 1));
	printf("Loss:%d\n", p_a.loss);
	printf("Path(Total):0");
	for(int i = 0; i < p_a.path.size(); i++)
		printf("->%d", p_a.path[i]);
	printf("->0\n");
	printf("Path detail:0");
	path_ans.push_back(0);
	show_vector_path(floyd_path[0][p_a.path[0]]);
	for(int i = 1; i < p_a.path.size() ; i++){
		show_vector_path(floyd_path[p_a.path[i - 1]][p_a.path[i]]);
	}
	show_vector_path(floyd_path[p_a.path[p_a.path.size() - 1]][0]);
	printf("\n");
	/*注释掉这一行表示不画出最终结果，否则就用红线画出最终结果，这里的最终结果只针对快递员最优*/
	draw_last_real_ans(); 
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

int my_dis(point a, point b)
{
	return sqrt((a.x - b.x) * ( a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
int main()
{
	int a = 0, b = 0, r_len = 0, i = 0, j = 0, k = 0;
	init();
	/*输入顶点的个数，包括起点，起点编号为0*/
	scanf("%d", &num_of_pot);
	point tmp_point;
	for(i = 0; i < num_of_pot; i++){
		scanf("%d%d", &tmp_point.x, &tmp_point.y);
		all_point.push_back(tmp_point);
	}

	if(num_of_pot > MAXN){
		fprintf(stderr, "Too much pot here !!!\n");
		exit(1);
	}
	/*输入路径条数*/
	scanf("%d", &num_of_road);
		/*输入路径，从节点a 到节点 b的长度为r_len*/
	for(i = 0; i < num_of_road; i++){
		scanf("%d%d", &a, &b);
		if(a >= MAXN || b >= MAXN || a < 0 || b < 0 || a >= num_of_pot || b >= num_of_pot){
			fprintf(stderr, "Illegal pot number !!!\n");
			exit(1);
		}
		r_len = my_dis(all_point[a], all_point[b]);
		tmp_point.x = a;
		tmp_point.y = b;
		path.push_back(tmp_point);
		dis[a][b] = r_len;
		max_path_length += r_len;
	} 

	begin_draw();
	draw_figure();
	draw_all_line();

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
	//Check if courier can back to original point or not
	bool check_path = true;
	for(i = 0; i < num_of_pot; i++){
		if(dis[i][0] > max_path_length){
			check_path = false;
			printf("From %d courier can not back to original point\n", i);
		}
	}

	if(!check_path){
		end_draw();
		exit(1);
	}

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
			fprintf(stderr, "Customer time cost out of range, please check your input data\n");
			end_draw();
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
				fprintf(stderr, "Customer time cost out of range, wait time %d\n", wait_time);
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
	
	end_draw();
	return 0;
}


