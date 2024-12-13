#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "tridimensional_math.h"
#include "windows_math.h"
#include "model.h"

#define CHAR_OFFSET (cyChar*2)

/*World coordinates & Perspective coordinates:
 *      +y^
 *        |
 *        |
 *        |
 *        |
 *        |
 *        |______________>
 *       /              +x
 *      /
 *     /
 *    /+z
 */

const char szAppName[] = "Tridimensional";
const char modelDir[] = "../models/";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

control initializeControl();
settings createSettings();
void drawInfo(HDC hdc, RECT graph, settings renderSettings);
void drawControls(HWND hwnd, RECT options, settings *renderSettings, int cyChar);
void drawModel(HDC hdc, vec3 camera, model model1, settings renderSettings, double fov, double aspectRatio, double rad);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
    MSG msg;
    HWND hwnd;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hCursor = NULL;
    wndclass.hIcon = NULL;
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if(!RegisterClass(&wndclass)){
        MessageBox(NULL, TEXT("Window class registration failed!"), szAppName, MB_ICONERROR);
    }

    hwnd = CreateWindow(szAppName,
                        TEXT("Tridimensional"),
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

    HDC hdc;
    PAINTSTRUCT ps;
    static settings renderSettings;
    static vec3 camera = {0,0,3};
    static double fov = M_PI/2;
    static RECT view;
    static RECT options;
    static RECT graph;
    static RECT controlArea;
    static double aspectRatio;
    static double rad = 0;
    static model loadedModel;
    TEXTMETRIC tm;
    int cyChar;
    char buf[64];
    char finalPath[74];
    int oldNum;

    switch(message) {
        case WM_CREATE:
            renderSettings = createSettings();
            return 0;
        case WM_SIZE:
            GetClientRect(hwnd, &view);
            graph = options = view;
            graph.right = view.right - (view.right * 0.2);
            options.left = graph.right;
            aspectRatio = (double) graph.right / graph.bottom;
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            GetTextMetrics(hdc, &tm);
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            drawInfo(hdc, graph, renderSettings);
            drawControls(hwnd, options, &renderSettings, cyChar);

            SetViewportOrgEx(hdc, graph.right / 2, graph.bottom / 2, NULL);
            SetMapMode(hdc, MM_ANISOTROPIC);
            SetViewportExtEx(hdc, 1, -1, NULL);
            SetWindowExtEx(hdc, 1, 1, NULL);

            if(loadedModel.triangleCount != 0)
                drawModel(hdc, camera, loadedModel, renderSettings, fov, aspectRatio, rad);
            EndPaint(hwnd, &ps);
            return 0;
        case WM_DESTROY:
            if(loadedModel.triangles != NULL)
                free(loadedModel.triangles);
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            switch(wParam) {
                case VK_RIGHT:
                    rad += M_PI/32;
                    InvalidateRect(hwnd, &graph, 1);
                    break;
                case VK_LEFT:
                    rad -= M_PI/32;
                    InvalidateRect(hwnd, &graph, 1);
                    break;
                case 'W':
                    camera.z -= .2;
                    InvalidateRect(hwnd, &graph, 1);
                    break;
                case 'S':
                    camera.z += .2;
                    InvalidateRect(hwnd, &graph, 1);
                    break;
                case VK_SPACE:
                    camera.y += .2;
                    InvalidateRect(hwnd, &graph, 1);
                    break;
                case VK_SHIFT:
                    camera.y -= .2;
                    InvalidateRect(hwnd, &graph, 1);
                    break;
            }
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case LIGHT:
                    renderSettings.controls[LIGHT].value = !renderSettings.controls[LIGHT].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case XRAY:
                    renderSettings.controls[XRAY].value = !renderSettings.controls[XRAY].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case BORDERS:
                    renderSettings.controls[BORDERS].value = !renderSettings.controls[BORDERS].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case SCREENXY:
                    renderSettings.controls[SCREENXY].value = !renderSettings.controls[SCREENXY].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case ROTX:
                    renderSettings.controls[ROTX].value = !renderSettings.controls[ROTX].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case ROTY:
                    renderSettings.controls[ROTY].value = !renderSettings.controls[ROTY].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case ROTZ:
                    renderSettings.controls[ROTZ].value = !renderSettings.controls[ROTZ].value;
                    InvalidateRect(hwnd, &graph, 1);
                    SetFocus(hwnd);
                    break;
                case MODEL:
                    memset(buf, 0, sizeof(buf));
                    memset(finalPath,0,sizeof(buf));
                    GetWindowText(renderSettings.controls[MODEL].hwnd, buf, 64);
                    snprintf(finalPath, sizeof(finalPath), "%s%s", modelDir, buf);
                    oldNum = loadedModel.triangleCount;
                    loadedModel = loadModel(finalPath);
                    if(loadedModel.triangleCount != oldNum) // little hack to not create a subprocedure for the EDIT control, might require 2 clicks on the EDIT
                        SetFocus(hwnd);
                    InvalidateRect(hwnd, &graph, 1);
                    break;
            }
    }

    return DefWindowProc(hwnd,message,wParam,lParam);
}

void drawInfo(HDC hdc, RECT graph, settings renderSettings)
{
    MoveToEx(hdc, graph.right, graph.top, NULL);
    LineTo(hdc, graph.right, graph.bottom);

    if(renderSettings.controls[SCREENXY].value == CONTROL_ON)
    {
        MoveToEx(hdc, graph.right/2, graph.top, NULL);
        LineTo(hdc, graph.right/2, graph.bottom);

        MoveToEx(hdc, graph.left, graph.bottom/2, NULL);
        LineTo(hdc, graph.right, graph.bottom/2);
    }

}

void drawModel(HDC hdc, vec3 camera, model model1, settings renderSettings, double fov, double aspectRatio, double rad)
{
    int toRasterNum = 0;
    vec3 light_direction = normaliseVector((vec3) {0,0,1});

    triangle *pTriangles = (triangle*) malloc(sizeof(triangle) * model1.triangleCount);
    triangle *toRaster = NULL;

    mat4 rotationMatrix = getIdentity();

    if(renderSettings.controls[ROTX].value == CONTROL_ON)
        rotationMatrix = multiplyMatrix(rotationMatrix, getRotX(rad));
    if(renderSettings.controls[ROTY].value == CONTROL_ON)
        rotationMatrix = multiplyMatrix(rotationMatrix, getRotY(rad));
    if(renderSettings.controls[ROTZ].value == CONTROL_ON)
        rotationMatrix = multiplyMatrix(rotationMatrix, getRotZ(rad));

    for(int i = 0; i < model1.triangleCount; i++)
    {
        pTriangles[i].p[0] = model1.triangles[i].p[0];
        pTriangles[i].p[1] = model1.triangles[i].p[1];
        pTriangles[i].p[2] = model1.triangles[i].p[2];

        pTriangles[i].p[0] = matrixVectorMultiplication(rotationMatrix, pTriangles[i].p[0]);
        pTriangles[i].p[1] = matrixVectorMultiplication(rotationMatrix, pTriangles[i].p[1]);
        pTriangles[i].p[2] = matrixVectorMultiplication(rotationMatrix, pTriangles[i].p[2]);

        pTriangles[i].normal = triangle_normal(pTriangles[i]);

        vec3 ctraslated = subtractVector(pTriangles[i].p[0], camera);

        if(renderSettings.controls[XRAY].value == CONTROL_OFF)
        {
            if(dot_product(pTriangles[i].normal,ctraslated) >= 0)
            {
                continue;
            }
        }

        toRasterNum++;
        if(toRaster == NULL)
        {
            toRaster = malloc(sizeof(triangle) * toRasterNum);
        }
        else
        {
            toRaster = realloc(toRaster, sizeof(triangle) * toRasterNum);
        }
        toRaster[toRasterNum-1] = pTriangles[i];
    }

    qsort(toRaster, toRasterNum, sizeof(triangle), compareTriangles);

    for(int i = 0; i<toRasterNum; i++)
    {
        double dl;
        if(renderSettings.controls[LIGHT].value == CONTROL_OFF)
        {
            dl = dot_product(toRaster[i].normal, light_direction);
        }
        else
        {
            dl = 1;
        }

        toRaster[i] = perspectiveProjectTriangle(toRaster[i], camera, aspectRatio, fov);
        toRaster[i] = scaleTriangle(toRaster[i], 1000);

        if(renderSettings.controls[XRAY].value == CONTROL_OFF)
            drawFilledTriangle(hdc, toRaster[i], RGB(255 * dl, 255 * dl, 255 * dl));

        if(renderSettings.controls[BORDERS].value != CONTROL_ON)
            drawTriangle(hdc, toRaster[i]);
    }

    if(toRaster) free(toRaster);
    if(pTriangles) free(pTriangles);
}

settings createSettings()
{
    settings newSettings;
    for(int i = 0; i<SETTINGS_NUM; i++)
    {
        newSettings.controls[i] = initializeControl();
    }
    return newSettings;
}

control initializeControl()
{
    control new;
    new.hwnd = NULL;
    new.label_hwnd = NULL;
    new.value = CONTROL_OFF;
    return new;
}

void drawControls(HWND hwnd, RECT options, settings *renderSettings, int cyChar) {
    int label_width = (options.right - options.left) * 0.5;
    int control_width = (options.right - options.left) * 0.5;
    int label_start = options.left;
    int control_start = label_start + label_width;

    if (renderSettings->controls[0].hwnd == NULL)
    {
        for(int i = 0; i<SETTINGS_NUM-1; i++)
        {

            renderSettings->controls[i].label_hwnd = CreateWindow(
                    "STATIC",
                    controls_names[i],
                    WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
                    label_start, i*CHAR_OFFSET,
                    label_width, CHAR_OFFSET,
                    hwnd,
                    (HMENU) (UINT_PTR)SETTINGS_NUM+i,
                    NULL,
                    NULL
            );
            renderSettings->controls[i].hwnd = CreateWindow(
                    "BUTTON",
                    "flip",
                    WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
                    control_start, i*CHAR_OFFSET,
                    control_width, CHAR_OFFSET,
                    hwnd,
                    (HMENU) (UINT_PTR)i,
                    NULL,
                    NULL
            );
        }
        renderSettings->controls[MODEL].label_hwnd = CreateWindow(
                "STATIC",
                controls_names[MODEL],
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
                label_start, MODEL*CHAR_OFFSET,
                label_width, CHAR_OFFSET,
                hwnd,
                (HMENU) (UINT_PTR)SETTINGS_NUM+MODEL,
                NULL,
                NULL
        );
        renderSettings->controls[MODEL].hwnd = CreateWindow(
                TEXT("EDIT"),
                "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                control_start, MODEL*CHAR_OFFSET,
                control_width, CHAR_OFFSET,
                hwnd,
                (HMENU) (UINT_PTR)MODEL,
                GetModuleHandle(NULL),
                NULL
        );
    }
    else
    {
        for(int i = 0; i<SETTINGS_NUM; i++)
        {
            MoveWindow(renderSettings->controls[i].label_hwnd,
                       label_start, i*CHAR_OFFSET,
                       label_width, CHAR_OFFSET,
                       1);

            MoveWindow(renderSettings->controls[i].hwnd,
                       control_start, i*CHAR_OFFSET,
                       control_width, CHAR_OFFSET,
                       1);
        }
    }
}