#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

model loadModel(char* filename)
{
    char buf[256];
    FILE *fptr;
    model loaded = {.triangleCount = 0, .triangles = NULL};
    unsigned int vertexNum = 0;
    unsigned int vertexnNum = 0;
    int trianglesNum = 0;
    vec3 *lv = (vec3*) malloc(sizeof(vec3)*1);
    vec3 *ln = (vec3*) malloc(sizeof(vec3)*1);
    triangle *triangles = (triangle*) malloc(sizeof(triangle)*1);

    fptr = fopen(filename, "r");
    if(fptr == NULL)
        return loaded;

    memset(buf, 0, sizeof(buf));
    while(fgets(buf,sizeof(buf),fptr))
    {
        if(strstr(buf, "v "))
        {
            vertexNum++;
            lv = (vec3*) realloc(lv, sizeof(vec3)*vertexNum);
            if(lv == NULL)
            {
                free(lv);
                free(ln);
                free(triangles);
                return loaded;
            }
            sscanf(buf, "v %lf %lf %lf", &lv[vertexNum-1].x, &lv[vertexNum-1].y, &lv[vertexNum-1].z);
        }
        else if(strstr(buf, "vn "))
        {
            vertexnNum++;
            ln = (vec3*) realloc(ln, sizeof(vec3)*vertexnNum);
            if(ln == NULL)
            {
                free(lv);
                free(ln);
                free(triangles);
                return loaded;
            }
            sscanf(buf, "vn %lf %lf %lf", &ln[vertexnNum-1].x, &ln[vertexnNum-1].y, &ln[vertexnNum-1].z);
        }
        else if(strstr(buf, "f "))
        {
            int v1,v2,v3;
            trianglesNum++;
            triangles = (triangle*) realloc(triangles, sizeof(triangle)*trianglesNum);
            if(triangles == NULL)
            {
                free(lv);
                free(ln);
                free(triangles);
                return loaded;
            }
            sscanf(buf, "f %d %d %d",&v1,&v2,&v3);

            triangles[trianglesNum-1].p[0] = lv[v1-1];
            triangles[trianglesNum-1].p[1] = lv[v2-1];
            triangles[trianglesNum-1].p[2] = lv[v3-1];
        }
    }

    loaded.triangles = triangles;
    loaded.triangleCount = trianglesNum;

    return loaded;
}
