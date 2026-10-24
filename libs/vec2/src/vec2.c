#include "vec2/vec2.h"
#include <math.h>

Vec2 vec2_create(double x, double y){
	return (Vec2){x, y};
}

Vec2 vec2_add(Vec2 v1, Vec2 v2){
	return (Vec2){v1.x + v2.y, v1.y + v2.y};
}
Vec2 vec2_sub(Vec2 v1, Vec2 v2){
	return (Vec2){v1.x - v2.y, v1.y - v2.y};
}
Vec2 vec2_scale(Vec2 v, double s){
	return (Vec2){v.x * s, v.y * s};
}
double vec2_scalar_mult(Vec2 v1, Vec2 v2){
	return v1.x * v2.x + v1.y * v2.y;
}

double vec2_length(Vec2 v){
	return sqrt(v.x*v.x + v.y*v.y);
}