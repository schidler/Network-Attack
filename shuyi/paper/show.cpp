#include <UbigraphAPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define MAXN 15 
static int edge_id = 100007;
extern int dis[MAXN][MAXN];
int create_pot(int n)
{
	const char *buf[] ={"1asd", "2sdfas", "3", "4", "5"};
	ubigraph_clear();
	for(int i = 0; i < n; i ++){
	//	sprintf(buf, "No.%d --", i);
		ubigraph_new_vertex_w_id(i);
		ubigraph_set_vertex_style_attribute(i, "shape", "sphere");
		ubigraph_set_vertex_style_attribute(i, "size", "0.3");
		//ubigraph_set_vertex_style_attribute(i, "label", "caca");
		//sleep(5);
		//ubigraph_set_vertex_style_attribute(i, "label", "haha");

		ubigraph_new_edge_w_id(edge_id, i, i);

		ubigraph_set_edge_attribute(edge_id, "label", buf[i]);	
		edge_id ++;
		printf("%s\n", buf);
	}
	return 0;
}

int create_line(int id, int x, int y, const char *str = NULL)
{
	ubigraph_new_edge_w_id(id, x, y);

	//ubigraph_set_edge_attribute(id, "spline", "true");	
	ubigraph_set_edge_attribute(id, "arrow", "true");
	ubigraph_set_edge_attribute(id, "arrow_radius", "0.3");
	ubigraph_set_edge_attribute(id, "arrow_position", "0.8");
	ubigraph_set_edge_attribute(id, "showstrain", "true");	

	ubigraph_set_edge_attribute(id, "label", str);	
	//printf("%d\n", ubigraph_new_edge(x, y));
	return 0;
}

int show()
{
	ubigraph_clear();
	const int N = 5;
	int i;
	for (i=0; i < N; ++i)
		ubigraph_new_vertex_w_id(i);
	for (i=0; i < N; ++i)
	{
		char tbuf[20];
		int r = (int)( i / (float) N * 255);
		sprintf(tbuf, "#%02x%02x%02x", r, 255-r, 255);
		ubigraph_set_vertex_attribute(i, "color", tbuf);

		ubigraph_new_edge(i, (i+1)%(N/2));
	}
}

