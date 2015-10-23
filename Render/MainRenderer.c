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

//储存渲染结果的设备上下文
HDC buffer_dc;
HBITMAP bmp;
//删了下面这个
FLOAT2D p0, p1;
CAMERA camera;

//创建物体
//有8个点
//源自立方体的8个顶点
OBJECT CubePoints;

//测试用
void FunctionTest()
{
	InitObject(&CubePoints, 0, 0, 0, 0, 0, 0);
	InitCamera(&camera, 200, 150, -600, 0, -10, 0, 50, 1024, 90, 90);

	//生成世界至视口矩阵
	MATRIX4 WorldToView = { 0 };
	Matrix4SetZero(&WorldToView);
	WorldToView = GetWorldToViewMatrix4(&camera);
	//将所有物体转至视口坐标系
	SingleObjectToViewTransform(&CubePoints, WorldToView);

	//生成视口到齐次剪裁空间
	MATRIX4 ViewToHomo = { 0 };
	Matrix4SetZero(&ViewToHomo);
	ViewToHomo = GetViewToHomoMatrix4(&camera);

	//对视口坐标下的物体进行变换
	SingleObectFromViewToHomoTransform(&CubePoints, ViewToHomo);


	for (int lop = 0; lop < CubePoints.model.vertexNum; lop++)
	{
		CubePoints.model.vertexList[lop].x = (CubePoints.model.vertexList[lop].x + 1.0f) * 300;
		CubePoints.model.vertexList[lop].y = (CubePoints.model.vertexList[lop].y + 1.0f) * 300;
	
		CubePoints.model.vertexList[lop].z = (CubePoints.model.vertexList[lop].z + 1.0f) * 300;
	}
}

///////////
//画线算法//
///////////

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
			SetPixel(buffer_dc, painter_y, RENDER_Y - i, BLACKCOLOR);
		}
		else
		{
			SetPixel(buffer_dc, i, RENDER_Y - painter_y, BLACKCOLOR);
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
	RECT rect = { 0, 0, RENDER_X, RENDER_Y};
	switch (Msg)
	{
	//窗口创建时
	case WM_CREATE:
		//创建并初始化画布
		bmp = CreateCompatibleBitmap(GetDC(hWnd),RENDER_X, RENDER_Y);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);
		FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));
		break;
	//窗口重绘时
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
		break;
	//有键被按下时
	case WM_KEYDOWN:
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
			FunctionTest();

			p0.x = CubePoints.model.vertexList[0].x;
			p0.y = CubePoints.model.vertexList[0].y;
			p1.x = CubePoints.model.vertexList[1].x;
			p1.y = CubePoints.model.vertexList[1].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[1].x;
			p0.y = CubePoints.model.vertexList[1].y;
			p1.x = CubePoints.model.vertexList[2].x;
			p1.y = CubePoints.model.vertexList[2].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[2].x;
			p0.y = CubePoints.model.vertexList[2].y;
			p1.x = CubePoints.model.vertexList[3].x;
			p1.y = CubePoints.model.vertexList[3].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[3].x;
			p0.y = CubePoints.model.vertexList[3].y;
			p1.x = CubePoints.model.vertexList[0].x;
			p1.y = CubePoints.model.vertexList[0].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[0].x;
			p0.y = CubePoints.model.vertexList[0].y;
			p1.x = CubePoints.model.vertexList[4].x;
			p1.y = CubePoints.model.vertexList[4].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[1].x;
			p0.y = CubePoints.model.vertexList[1].y;
			p1.x = CubePoints.model.vertexList[5].x;
			p1.y = CubePoints.model.vertexList[5].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[2].x;
			p0.y = CubePoints.model.vertexList[2].y;
			p1.x = CubePoints.model.vertexList[6].x;
			p1.y = CubePoints.model.vertexList[6].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[3].x;
			p0.y = CubePoints.model.vertexList[3].y;
			p1.x = CubePoints.model.vertexList[7].x;
			p1.y = CubePoints.model.vertexList[7].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[4].x;
			p0.y = CubePoints.model.vertexList[4].y;
			p1.x = CubePoints.model.vertexList[5].x;
			p1.y = CubePoints.model.vertexList[5].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[5].x;
			p0.y = CubePoints.model.vertexList[5].y;
			p1.x = CubePoints.model.vertexList[6].x;
			p1.y = CubePoints.model.vertexList[6].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[6].x;
			p0.y = CubePoints.model.vertexList[6].y;
			p1.x = CubePoints.model.vertexList[7].x;
			p1.y = CubePoints.model.vertexList[7].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertexList[4].x;
			p0.y = CubePoints.model.vertexList[4].y;
			p1.x = CubePoints.model.vertexList[7].x;
			p1.y = CubePoints.model.vertexList[7].y;
			DrawLine_Algo01(p0, p1);

			//强制重绘整个窗口
			BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
		}
		break;
	case WM_DESTROY:
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
	wchar_t Name[] = TEXT("Output");
	WNDCLASSEX wnd = {0};
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

	HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, Name, TEXT("Render Output"), 
		WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_X, WINDOW_Y, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}
	return msg.message;
}