#ifndef TRIDIMENSIONAL_TYPES_H
#define TRIDIMENSIONAL_TYPES_H

#include <windows.h>

//window-related types, constants

#define CONTROL_ON 1
#define CONTROL_OFF 0

#define SETTINGS_NUM 8

#define LIGHT 0
#define BORDERS 1
#define XRAY 2
#define SCREENXY 3
#define ROTX 4
#define ROTY 5
#define ROTZ 6
#define MODEL 7

extern const char controls_names[SETTINGS_NUM][10];

typedef struct control{
    int value;
    HWND hwnd;
    HWND label_hwnd;
}control;

typedef struct settings{
    control controls[SETTINGS_NUM];
}settings;

//math-related types
typedef struct mat4{
    double m[4][4];
}mat4;

typedef struct vec3{
    double x;
    double y;
    double z;
}vec3;

typedef struct hvec3{
    vec3 vec3;
    double w;
}hvec3;

typedef struct triangle{
    vec3 p[3];
    vec3 normal;
}triangle;

typedef struct model{
    triangle *triangles;
    int triangleCount;
}model;

#endif //TRIDIMENSIONAL_TYPES_H
