#ifndef TRIDIMENSIONAL_TRIDIMENSIONAL_MATH_H
#define TRIDIMENSIONAL_TRIDIMENSIONAL_MATH_H

#include "types.h"

#define near_z 0.1
#define far_z 1000.0

#define matrixVectorMultiplication(m,v) transformFromHomogeneous(matrixhVectorMultiplication(m,transformToHomogeneous(v)))

mat4 getPerspectiveMatrix(double aspect, double fov);
hvec3 matrixhVectorMultiplication(mat4 m, hvec3 v);

vec3 perspectiveProjectionMatrix(vec3 toTransform, vec3 camera, double aspect, double fov);
triangle perspectiveProjectTriangle(triangle t, vec3 camera, double aspect, double fov);

hvec3 transformToHomogeneous(vec3 toTransform);
vec3 transformFromHomogeneous(hvec3 toTransform);

vec3 addVector(vec3 v1, vec3 v2);
vec3 subtractVector(vec3 v1, vec3 v2);
vec3 scaleVector(vec3 toScale, double scalingFactor);

vec3 cross_product(vec3 v1, vec3 v2);
vec3 triangle_normal(triangle t);
vec3 normaliseVector(vec3 toNormalise);

double dot_product(vec3 v1, vec3 v2);
double vectorMagnitude(vec3 v);

mat4 getRotY(double tetha);
mat4 getRotX(double tetha);
mat4 getRotZ(double tetha);
mat4 getIdentity();

triangle scaleTriangle(triangle toScale, double scalingFactor);
mat4 multiplyMatrix(mat4 m1, mat4 m2);

int compareTriangles(const void * t1, const void * t2);

#endif //TRIDIMENSIONAL_TRIDIMENSIONAL_MATH_H
