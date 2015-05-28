#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
int arrow();
int begin_draw()
{
	cout<<"<!DOCTYPE HTML>"<<endl;
	cout<<"	<html>"<<endl;
	cout<<"<head>"<<endl;
	cout<<"	<style>"<<endl;
	cout<<"	body {"<<endl;
	cout<<"margin: 10px;"<<endl;
	cout<<"padding: 10px;"<<endl;
	cout<<"	}"<<endl;
	cout<<"#myCanvas {"<<endl;
	cout<<"border: 1px solid #9C9898;"<<endl;
	cout<<"}"<<endl;
	cout<<"</style>"<<endl;
	cout<<"<script>"<<endl;
	cout<<"window.onload = function() {"<<endl;
	cout<<"var canvas = document.getElementById(\"myCanvas\");"<<endl;

	return 0;
}
int end_draw()
{

	cout<<"};"<<endl;
	arrow();
	cout<<"</script>"<<endl;
	cout<<"</head>"<<endl;
	cout<<"<body>"<<endl;
	cout<<"<canvas id=\"myCanvas\" width=\"800\" height=\"500\"></canvas>"<<endl;
	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
	return 0;
}

int draw_point(const char *context, int center_x, int center_y, int radius, int line_width)
{
	cout<<"var "<<context<<" = canvas.getContext(\"2d\");"<<endl;
	cout<<context<<".beginPath();"<<endl;
	cout<<context<<".arc("<<center_x<<", "<<center_y<<", "<<radius<<", 0, 2 * Math.PI, false);"<<endl;
	cout<<context<<".fillStyle = \"#8ED6FF\";"<<endl;
	cout<<context<<".fill();"<<endl;
	cout<<context<<".lineWidth = "<<line_width<<";"<<endl;
	cout<<context<<".strokeStyle = \"black\";"<<endl;
	cout<<context<<".stroke();"<<endl;
}

int arrow(){
cout<<"function arrow2(canId,ox,oy,x1,y1,x2,y2)\
{\
  var sta = new Array(x1,y1);\
  var end = new Array(x2,y2);\
\
  var lineWidth = 5;\
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
  ctx.translate(end[0],end[1]);\
\
  var ang=(end[0]-sta[0])/(end[1]-sta[1]);\
  ang=Math.atan(ang);\
  if(end[1]-sta[1] >= 0){\
	ctx.rotate(-ang);\
  }else{\
	ctx.rotate(Math.PI-ang);\
  } \
  ctx.lineTo(-5*lineWidth,-10*lineWidth); \
  ctx.lineTo(0,-5*lineWidth); \
  ctx.lineTo(5*lineWidth,-10*lineWidth); \
  ctx.lineTo(0,0); \
  ctx.fillStyle = \"#8ED6FF\";\
\
  ctx.fill(); \
  ctx.restore();\
  ctx.closePath(); \
}"<<endl;
return 0;
}
int draw_arrow(int x1, int y1, int x2, int y2)
{
	cout<<"arrow2(\"myCanvas\", 0, 0, "<<x1<<","\
		<<y1<<","<<x2<<","<<y2<<")"<<endl;
}
int draw_text(int x, int y, int font, const char *text)
{
	cout<<"var context_draw_text = canvas.getContext(\"2d\");"<<endl;
	cout<<"context_draw_text.font = \"italic "<<font<<"pt Calibri\""<<endl;
	cout<<"context_draw_text.fillText(\""<<text<<"\", "<<x<<", "<<y<<");";
	return 0;
}
int main()
{
	begin_draw();
	draw_point("context1", 100, 100, 50, 10);
	draw_arrow(10, 10, 400, 400);
	draw_text(100, 100, 40, "nidaye");
	end_draw();
	return 0;
}
