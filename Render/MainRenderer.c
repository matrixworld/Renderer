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
	InitCamera(&camera, 300, 350, -400, 45, -45, 0, 50, 500, 90, 90);

	//生成世界至视口矩阵
	MATRIX4 WorldToView = { 0 };
	WorldToView = GetWorldToViewMatrix4(&camera);

	//为物体填入立方体的模型
	MODEL points;
	IniteModelWithCube22(&points);
	InitObject(&CubePoints, points, 0, 0, 0, 0, 0, 0);

	SingleObjectToViewTransform(&CubePoints, WorldToView);

	/*******************************
	*以上是正交投影
	*下面尝试将观察空间变换至齐次剪裁空间
	*以进行透视投影
	********************************/
	float r = 25.0f, l = -25.0f, t = 25.0f, b = -25.0f;
	float z;
	MATRIX4 hMatrix4 = { 0.0f };
	Matrix4SetZero(&hMatrix4);

	hMatrix4.var[0][0] = (2 * camera.NearZ) / (r - l);
	hMatrix4.var[1][1] = (2 * camera.NearZ) / (t - b);
	hMatrix4.var[2][0] = (r + l) / (r - l);
	hMatrix4.var[2][1] = (t + b) / (t - b);
	hMatrix4.var[2][2] = -(camera.FarZ + camera.NearZ) / (camera.FarZ - camera.NearZ);
	hMatrix4.var[2][3] = -1;
	hMatrix4.var[3][2] = -(2 * camera.NearZ*camera.FarZ) / (camera.FarZ - camera.NearZ);




	for (int lop = 0; lop < 8; lop++)
	{
		z = -CubePoints.model.vertex[lop].z;
		VectorTransform(&CubePoints.model.vertex[lop], hMatrix4);
		CubePoints.model.vertex[lop].x /= z;
		CubePoints.model.vertex[lop].y /= z;

		CubePoints.model.vertex[lop].x = (CubePoints.model.vertex[lop].x + 1.0f) * 300;
		CubePoints.model.vertex[lop].y = (CubePoints.model.vertex[lop].y + 1.0f) * 300;
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
			SetPixel(buffer_dc, painter_y, i, BLACKCOLOR);
		}
		else
		{
			SetPixel(buffer_dc, i, painter_y, BLACKCOLOR);
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
	RECT rect = { 0, 0, RENDER_X - 1, RENDER_Y - 1 };
	switch (Msg)
	{
	//窗口创建时
	case WM_CREATE:
		//创建并初始化画布
		bmp = CreateCompatibleBitmap(GetDC(hWnd),RENDER_X, RENDER_Y);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);
		FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));

		//初始化摄像机信息
		//InitCamera(&camera, 0, 0, -256, 0, 0, 1, 256, 1000, 90, 90);
		break;
	//窗口重绘时
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, RENDER_X - 1, RENDER_Y - 1, buffer_dc, 0, 0, SRCCOPY);
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

			//画出线条
			p0.x = CubePoints.model.vertex[0].x;
			p0.y = CubePoints.model.vertex[0].y;
			p1.x = CubePoints.model.vertex[1].x;
			p1.y = CubePoints.model.vertex[1].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[1].x;
			p0.y = CubePoints.model.vertex[1].y;
			p1.x = CubePoints.model.vertex[2].x;
			p1.y = CubePoints.model.vertex[2].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[2].x;
			p0.y = CubePoints.model.vertex[2].y;
			p1.x = CubePoints.model.vertex[3].x;
			p1.y = CubePoints.model.vertex[3].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[3].x;
			p0.y = CubePoints.model.vertex[3].y;
			p1.x = CubePoints.model.vertex[0].x;
			p1.y = CubePoints.model.vertex[0].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[0].x;
			p0.y = CubePoints.model.vertex[0].y;
			p1.x = CubePoints.model.vertex[4].x;
			p1.y = CubePoints.model.vertex[4].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[1].x;
			p0.y = CubePoints.model.vertex[1].y;
			p1.x = CubePoints.model.vertex[5].x;
			p1.y = CubePoints.model.vertex[5].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[2].x;
			p0.y = CubePoints.model.vertex[2].y;
			p1.x = CubePoints.model.vertex[6].x;
			p1.y = CubePoints.model.vertex[6].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[3].x;
			p0.y = CubePoints.model.vertex[3].y;
			p1.x = CubePoints.model.vertex[7].x;
			p1.y = CubePoints.model.vertex[7].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[4].x;
			p0.y = CubePoints.model.vertex[4].y;
			p1.x = CubePoints.model.vertex[5].x;
			p1.y = CubePoints.model.vertex[5].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[5].x;
			p0.y = CubePoints.model.vertex[5].y;
			p1.x = CubePoints.model.vertex[6].x;
			p1.y = CubePoints.model.vertex[6].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[6].x;
			p0.y = CubePoints.model.vertex[6].y;
			p1.x = CubePoints.model.vertex[7].x;
			p1.y = CubePoints.model.vertex[7].y;
			DrawLine_Algo01(p0, p1);

			p0.x = CubePoints.model.vertex[7].x;
			p0.y = CubePoints.model.vertex[7].y;
			p1.x = CubePoints.model.vertex[4].x;
			p1.y = CubePoints.model.vertex[4].y;
			DrawLine_Algo01(p0, p1);

			//强制重绘整个窗口
			BitBlt(GetDC(hWnd), 0, 0, RENDER_X - 1, RENDER_Y - 1, buffer_dc, 0, 0, SRCCOPY);
		}
		break;
	case WM_DESTROY:
		//销毁
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