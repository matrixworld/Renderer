////////////
//系统头文件//
////////////

#include <Windows.h>
#include <windef.h>
//精简版win32程序，不使用MFC相关东西
#define WIN32_LEAN_AND_MEAN

//////////////
//自定义头文件//
//////////////

#include "MagicMath.h"
#include "UsefulFunc.h"

///////////
//全局变量//
//////////
int quit;
char screen_keys[512];
RECT rect;
//储存渲染结果的设备上下文
HDC buffer_dc;
HBITMAP bmp;

CAMERA camera;
OBJECT CubePoints;

//预先声明
void DrawModelListIndex(FLOAT3D *, int *);
void DrawLine_Algo01(FLOAT2D, FLOAT2D);

//测试用
void Render()
{
	OBJECT object = CubePoints;
	InitModelWithCube22(&object.model);
	CAMERA camera_tmp = camera;
	//生成世界至视口矩阵
	MATRIX4 WorldToView = { 0 };
	Matrix4SetZero(&WorldToView);
	WorldToView = GetWorldToViewMatrix4(&camera_tmp);
	//将所有物体转至视口坐标系
	SingleObjectToViewTransform(&object, WorldToView);

	//生成视口到齐次剪裁空间
	MATRIX4 ViewToHomo = { 0 };
	Matrix4SetZero(&ViewToHomo);
	ViewToHomo = GetViewToHomoMatrix4(&camera_tmp);

	//对视口坐标下的物体进行变换
	SingleObectFromViewToHomoTransform(&object, ViewToHomo);


	for (int lop = 0; lop < object.model.vertexNum; lop++)
	{
		object.model.vertexList[lop].x = (object.model.vertexList[lop].x + 1.0f)*camera_tmp.aspect * 170;
		object.model.vertexList[lop].y = (object.model.vertexList[lop].y + 1.0f) * 170;
	}
	DrawModelListIndex(object.model.vertexList, object.model.verterListIndex);
}

///////////
//画线算法//
///////////

void DrawModelListIndex(FLOAT3D *vertexList, int *listIndex)
{
	FLOAT2D p0, p1, p2;
	FLOAT3D a, b, c;
	int index = 0;
	while (listIndex[index] != -1)
	{
		a.x = p0.x = vertexList[listIndex[index]].x;
		a.y = p0.y = vertexList[listIndex[index]].y;

		b.x = p1.x = vertexList[listIndex[index + 1]].x;
		b.y = p1.y = vertexList[listIndex[index + 1]].y;

		c.x = p2.x = vertexList[listIndex[index + 2]].x;
		c.y = p2.y = vertexList[listIndex[index + 2]].y;

		a.z = vertexList[listIndex[index]].z;
		b.z = vertexList[listIndex[index + 1]].z;
		c.z = vertexList[listIndex[index + 2]].z;

		index += 3;

		if (TriangleBackCull(a, b, c))
		{
			continue;
		}

		DrawLine_Algo01(p0, p1);
		DrawLine_Algo01(p1, p2);
		DrawLine_Algo01(p0, p2);
	}
}

//布雷森汉姆直线算法
void DrawLine_Algo01(FLOAT2D p0, FLOAT2D p1)
{
	//对直线进行剪裁
	if (!LiangBarskyLineClipping(&p0, &p1)) { return; }
	//直线斜率是否大于1
	BOOL steep = ABS(p1.y - p0.y) > ABS(p1.x - p0.x);
	//如果大于1
	//将直线沿 y=x 翻转输出
	if (steep)
	{
		swap(&p0.x, &p0.y);
		swap(&p1.x, &p1.y);
	}
	if (p0.x > p1.x)
	{
		swap(&p0.x, &p1.x);
		swap(&p0.y, &p1.y);
	}
	int dx = (int)(p1.x - p0.x);
	int dy = (int)ABS(p1.y - p0.y);

	int err = dx / 2;

	//y的增量
	int ystep = (p0.y < p1.y) ? 1 : -1;
	//用于绘画的 y 数值
	int painter_y = (int)p0.y;

	for (int i = (int)p0.x; i <= p1.x; i++) {
		if (steep)
		{
			SetPixel(buffer_dc, painter_y, RENDER_Y - 1 - i, BLACKCOLOR);
		}
		else
		{
			SetPixel(buffer_dc, i, RENDER_Y - 1 - painter_y, BLACKCOLOR);
		}
		err -= dy;
		if (err < 0) {
			painter_y += ystep;
			err += dx;
		}
	}
}

//程序的消息处理函数
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	//窗口创建时
	case WM_CREATE:
		//创建并初始化画布
		bmp = CreateCompatibleBitmap(GetDC(hWnd),RENDER_X, RENDER_Y);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);
		FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));

		//初始化世界物体及摄像机
		InitCamera(&camera, 200, 200, -200, 50, -45, 0, 10, 700, 70, 1.77f);
		InitObject(&CubePoints, 0, 0, 0, 0, 0, 0);

		break;
	//窗口重绘时
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
		break;
	//有键被按下时
	case WM_KEYDOWN:
		screen_keys[wParam & 511] = 1;
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		//响应键盘F5按键
		if (wParam == VK_F5)
		{
			//重新用白色覆盖memory Device Contexts
			FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));
			//TODO
			//三维空间画点
			Render();

			//强制重绘整个窗口
			BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
		}
		break;
	case WM_KEYUP:
		screen_keys[wParam & 511] = 0; 
		break;
	case WM_DESTROY:
		quit = 1;
		//销毁模型！！！
		DeleteModel(&CubePoints.model);

		DeleteDC(buffer_dc);
		DeleteObject(bmp);

		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd)
{
	//全局变量赋值
	rect.top = 0; rect.left = 0; rect.bottom = RENDER_Y; rect.right = RENDER_X;
	quit = 0;
	for (int lop = 0; lop < 512; lop++)
	{
		screen_keys[lop] = 0;
	}

	wchar_t Name[] = TEXT("Output");
	WNDCLASSEX wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = NULL;
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WinProc;
	wnd.lpszClassName = Name;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wnd);

	HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, Name, TEXT("Render"),
		WS_OVERLAPPEDWINDOW, 100, 50, WINDOW_X, WINDOW_Y, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (!screen_keys[VK_ESCAPE] && !quit) 
	{
		//没消息就会跳出
		while (1)
		{
			if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (screen_keys['A']) { camera.POS.x -= 3; }
		if (screen_keys['D']) { camera.POS.x += 3; }
		if (screen_keys['W']) { camera.POS.z += 3; }
		if (screen_keys['S']) { camera.POS.z -= 3; }
		if (screen_keys['Q']) { camera.POS.y += 3; }
		if (screen_keys['E']) { camera.POS.y -= 3; }

		if (screen_keys[VK_UP]) { camera.rotation.x -= 1; }
		if (screen_keys[VK_DOWN]) { camera.rotation.x += 1; }
		if (screen_keys[VK_LEFT]) { camera.rotation.y -= 1; }
		if (screen_keys[VK_RIGHT]) { camera.rotation.y += 1; }

		FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));
		Render();
		//强制重绘整个窗口
		BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
		Sleep(10);

	}
	return msg.message;
}