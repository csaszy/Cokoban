#ifndef Vec2_H
#define Vec2_H

typedef struct{
	double x, y;
}Vec2;

Vec2 vec2_create(double x, double y);
Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec2 vec2_sub(Vec2 v1, Vec2 v2);
Vec2 vec2_scale(Vec2 v, double s);
double vec2_scalar_mult(Vec2 v1, Vec2 v2);
double vec2_length(Vec2 v);


#endif
