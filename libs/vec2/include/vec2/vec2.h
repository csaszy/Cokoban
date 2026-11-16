#ifndef VEC2_H
#define VEC2_H

typedef struct{
	double x, y;
}Vec2;

Vec2 vec2_create(double x, double y);
Vec2 vec2_add(Vec2 v1, Vec2 v2);
Vec2 vec2_sub(Vec2 v1, Vec2 v2);
Vec2 vec2_scale(Vec2 v, double s);
double vec2_scalar_mult(Vec2 v1, Vec2 v2);
double vec2_length(Vec2 v);

typedef struct{
	int x, y;
}i32Vec2;

i32Vec2 i32vec2_create(int x, int y);
i32Vec2 i32vec2_add(i32Vec2 v1, i32Vec2 v2);
i32Vec2 i32vec2_sub(i32Vec2 v1, i32Vec2 v2);


#endif
