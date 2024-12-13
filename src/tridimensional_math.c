#include "tridimensional_math.h"
#include <math.h>
#include <stdio.h>

hvec3 matrixhVectorMultiplication(mat4 m, hvec3 v)
{
    hvec3 result;
    result.vec3.x = v.vec3.x * m.m[0][0] + v.vec3.y * m.m[1][0] + v.vec3.z * m.m[2][0] + v.w * m.m[3][0];
    result.vec3.y = v.vec3.x * m.m[0][1] + v.vec3.y * m.m[1][1] + v.vec3.z * m.m[2][1] + v.w * m.m[3][1];
    result.vec3.z = v.vec3.x * m.m[0][2] + v.vec3.y * m.m[1][2] + v.vec3.z * m.m[2][2] + v.w * m.m[3][2];
    result.w = v.vec3.x * m.m[0][3] + v.vec3.y * m.m[1][3] + v.vec3.z * m.m[2][3] + v.w * m.m[3][3];
    return result;
}

mat4 getPerspectiveMatrix(double aspect, double fov)
{
    double fovC = 1/tan(fov/2);
    mat4 pm = { (1/aspect)*fovC, 0, 0, 0,
                0, fovC, 0, 0,
                0, 0, -(far_z+near_z) / (far_z-near_z), -1,
                0, 0, -(2*far_z*near_z) / (far_z-near_z), 0};
    return pm;
}

vec3 perspectiveProjectionMatrix(vec3 toTransform, vec3 camera, double aspect, double fov)
{
    mat4 pm = getPerspectiveMatrix(aspect, fov);
    toTransform = subtractVector(toTransform, camera);
    hvec3 transformed = matrixhVectorMultiplication(pm, transformToHomogeneous(toTransform));

    return transformFromHomogeneous(transformed);
}

vec3 transformFromHomogeneous(hvec3 toTransform)
{
    if(toTransform.w == 0.0f)
        return toTransform.vec3;

    vec3 transformed = {toTransform.vec3.x/toTransform.w ,
                        toTransform.vec3.y/toTransform.w ,
                        toTransform.vec3.z/toTransform.w};
    return transformed;
}

hvec3 transformToHomogeneous(vec3 toTransform)
{
    hvec3 result;
    result.vec3 = toTransform;
    result.w = 1;
    return result;
}

vec3 scaleVector(vec3 toScale, double scalingFactor)
{
    vec3 result;
    result = toScale;
    result.x *= scalingFactor;
    result.y *= scalingFactor;
    result.z *= scalingFactor;
    return result;
}

vec3 subtractVector(vec3 v1, vec3 v2)
{
    vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

mat4 getRotY(double tetha)
{
    mat4 ry = {cos(tetha),0,sin(tetha),0,
               0,1,0,0,
               -sin(tetha),0,cos(tetha),0,
               0,0,0,1};
    return ry;
}

mat4 getRotX(double tetha)
{
    mat4 rx = {1,0,0,0,
               0,cos(tetha),-sin(tetha),0,
               0,sin(tetha),cos(tetha),0,
               0,0,0,1};
    return rx;
}

mat4 getRotZ(double tetha)
{
    mat4 rz = {cos(tetha),-sin(tetha),0,0,
               sin(tetha),cos(tetha),0,0,
               0,0,1,0,
               0,0,0,1};
    return rz;
}

vec3 cross_product(vec3 v1, vec3 v2)
{
    vec3 result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

triangle perspectiveProjectTriangle(triangle t, vec3 camera, double aspect, double fov)
{
    triangle result;
    for(int i = 0; i<3; i++)
    {
        result.p[i] = perspectiveProjectionMatrix(t.p[i], camera, aspect, fov);
    }
    return result;
}

triangle scaleTriangle(triangle toScale, double scalingFactor)
{
    triangle result;
    result = toScale;

    result.p[0] = scaleVector(result.p[0], scalingFactor);
    result.p[1] = scaleVector(result.p[1], scalingFactor);
    result.p[2] = scaleVector(result.p[2], scalingFactor);

    return result;
}

vec3 triangle_normal(triangle t)
{
    vec3 normal;
    vec3 v1, v2;

    v1 = subtractVector(t.p[1], t.p[0]);
    v2 = subtractVector(t.p[2], t.p[0]);

    normal = cross_product(v1,v2);
    normal = normaliseVector(normal);

    return normal;
}

vec3 normaliseVector(vec3 toNormalise)
{
    double magnitude;

    magnitude = vectorMagnitude(toNormalise);

    if(magnitude == 0)
        return toNormalise;

    toNormalise.x /= magnitude;
    toNormalise.y /= magnitude;
    toNormalise.z /= magnitude;

    return toNormalise;
}

double dot_product(vec3 v1, vec3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

mat4 multiplyMatrix(mat4 m1, mat4 m2)
{
    mat4 result;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[j][i] = m1.m[j][0] * m2.m[0][i] + m1.m[j][1] * m2.m[1][i] + m1.m[j][2] * m2.m[2][i] + m1.m[j][3] * m2.m[3][i];


    return result;
}

double vectorMagnitude(vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 addVector(vec3 v1, vec3 v2)
{
    vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

int compareTriangles(const void * p1, const void * p2)
{
    double d1, d2;

    triangle *t1 = (triangle*) p1;
    triangle *t2 = (triangle*) p2;

    d1 = (t1->p[0].z + t1->p[1].z + t1->p[2].z)/2;
    d2 = (t2->p[0].z + t2->p[1].z + t2->p[2].z)/2;

    return d1>d2;
}

mat4 getIdentity()
{
    mat4 result = { 0 };

    result.m[0][0] = result.m[1][1] = result.m[2][2] = result.m[3][3] = 1;

    return result;
}