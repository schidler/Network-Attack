#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
static ofstream show_file("ans.html5"); ; 
int arrow();
int begin_draw()
{
	show_file<<"<!DOCTYPE HTML>"<<endl;
	show_file<<"	<html>"<<endl;
	show_file<<"<head>"<<endl;
	show_file<<"	<style>"<<endl;
	show_file<<"	body {"<<endl;
	show_file<<"margin: 10px;"<<endl;
	show_file<<"padding: 10px;"<<endl;
	show_file<<"	}"<<endl;
	show_file<<"#myCanvas {"<<endl;
	show_file<<"border: 1px solid #9C9898;"<<endl;
	show_file<<"}"<<endl;
	show_file<<"</style>"<<endl;
	show_file<<"<script>"<<endl;
	show_file<<"window.onload = function() {"<<endl;
	show_file<<"var canvas = document.getElementById(\"myCanvas\");"<<endl;

	return 0;
}
int end_draw()
{

	show_file<<"};"<<endl;
	arrow();
	show_file<<"</script>"<<endl;
	show_file<<"</head>"<<endl;
	show_file<<"<body>"<<endl;
	show_file<<"<canvas id=\"myCanvas\" width=\"800\" height=\"500\"></canvas>"<<endl;
	show_file<<"</body>"<<endl;
	show_file<<"</html>"<<endl;
	return 0;
}

int draw_point(const char *context, int center_x, int center_y, int radius, int line_width)
{
	show_file<<"var "<<context<<" = canvas.getContext(\"2d\");"<<endl;
	show_file<<context<<".beginPath();"<<endl;
	show_file<<context<<".arc("<<center_x<<", "<<center_y<<", "<<radius<<", 0, 2 * Math.PI, false);"<<endl;
	show_file<<context<<".fillStyle = \"#8ED6FF\";"<<endl;
	show_file<<context<<".fill();"<<endl;
	show_file<<context<<".lineWidth = "<<line_width<<";"<<endl;
	show_file<<context<<".strokeStyle = \"black\";"<<endl;
	show_file<<context<<".stroke();"<<endl;
}

int arrow(){
show_file<<"function arrow2(canId,ox,oy,x1,y1,x2,y2)\
{\
  var sta = new Array(x1,y1);\
  var end = new Array(x2,y2);\
\
  var lineWidth = 1;\
  var canvas = document.getElementById(canId);\
  if(canvas == null)return false;\
  var ctx = canvas.getContext(\'2d\');\
  ctx.strokeStyle = \"blue\";\
  ctx.beginPath(); \
  ctx.translate(ox,oy,0); \
  ctx.moveTo(sta[0],sta[1]); \
  ctx.lineWidth = lineWidth;\
  ctx.lineTo(end[0],end[1]); \
  ctx.fill();\
  ctx.stroke(); \
  ctx.save();\
\
ctx.moveTo(sta[0] + (end[0]-sta[0])*2/3,sta[1] +(end[1] - sta[1])*2/3);\
ctx.translate(sta[0] + (end[0]-sta[0])*2/3,sta[1] +(end[1] - sta[1])*2/3);\
\
  var ang=(end[0]-sta[0])/(end[1]-sta[1]);\
  ang=Math.atan(ang);\
  if(end[1]-sta[1] >= 0){\
	ctx.rotate(-ang);\
  }else{\
	ctx.rotate(Math.PI-ang);\
  } \
  ctx.lineTo(-5*lineWidth*2,-10*lineWidth*2); \
  ctx.lineTo(0,-5*lineWidth); \
  ctx.lineTo(5*lineWidth*2,-10*lineWidth*2); \
  ctx.lineTo(0,0); \
  ctx.fillStyle = \"#FF0000\";\
\
  ctx.fill(); \
  ctx.restore();\
  ctx.closePath(); \
}"<<endl;
return 0;
}
int draw_arrow(int x1, int y1, int x2, int y2)
{
	show_file<<"arrow2(\"myCanvas\", 0, 0, "<<x1<<","\
		<<y1<<","<<x2<<","<<y2<<")"<<endl;
}
int draw_text(int x, int y, int font, const char *text)
{
	show_file<<"var context_draw_text = canvas.getContext(\"2d\");"<<endl;
	show_file<<"context_draw_text.font = \"italic "<<font<<"pt Calibri\""<<endl;
	show_file<<"context_draw_text.fillStyle = \'red\';"<<endl;
	show_file<<"context_draw_text.fillText(\""<<text<<"\", "<<x<<", "<<y<<");";
	return 0;
}
/*
int main()
{
	begin_draw();
	draw_point("context1", 100, 100, 50, 10);
	draw_arrow(10, 10, 400, 400);
	draw_text(100, 100, 40, "nidaye");
	end_draw();
	return 0;
}*/
