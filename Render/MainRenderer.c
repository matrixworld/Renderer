#include <Windows.h>
#include <windef.h>

//宏定义、结构体的定义
#include "Renderer_Header.h"

//精简版win32程序，不使用MFC相关东西
#define WIN32_LEAN_AND_MEAN

//全局变量
//储存渲染结果的设备上下文
HDC buffer_dc;
HBITMAP bmp;
//三角形顶点数组(不重复)
//30, 45, 500, 200, 200, 450
VERTEX2P TriangleVertexList[] = { { 30, 45 }, { 500, 200 }, { 200, 450 } };

//三角形顶点索引数组
//缠绕方向 顺时针
int TriangleVertexIndex[] = { 0, 1, 2 };

//函数预先声明
void DrawLine_Algo1(int,int,int,int);
void DrawTriangle(int *);
void RenderToScreen();
//////////////////////////////////////////////////////


//交换数据
void swap(int *a,int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void DrawTriangle(int *VertexIndex)
{
	DrawLine_Algo1(TriangleVertexList[0].x, TriangleVertexList[0].y, TriangleVertexList[1].x, TriangleVertexList[1].y);
	DrawLine_Algo1(TriangleVertexList[1].x, TriangleVertexList[1].y, TriangleVertexList[2].x, TriangleVertexList[2].y);
	DrawLine_Algo1(TriangleVertexList[2].x, TriangleVertexList[2].y, TriangleVertexList[0].x, TriangleVertexList[0].y);
}
//布雷森汉姆直线算法
void DrawLine_Algo1(int x0, int y0, int x1, int y1)
{
	//直线斜率是否大于1
	BOOL steep = ABS(y1 - y0) > ABS(x1 - x0);
	//如果大于1
	//将直线沿 y=x 翻转输出
	if (steep)
	{
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x0 > x1)
	{
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	int dx = x1 - x0;
	int dy = ABS(y1 - y0);
	
	int err = dx / 2;

	//y的增量
	int ystep = (y0 < y1) ? 1 : -1;
	//用于绘画的 y 数值
	int painter_y = y0;

	for (int i = x0; i <= x1; i++) {
		if (steep)
		{
			SetPixel(buffer_dc, painter_y, i, RGB(0, 0, 0));
		}
		else
		{
			SetPixel(buffer_dc, i, painter_y, RGB(0, 0, 0));
		}
		err -= dy;
		if (err < 0) {
			painter_y += ystep;
			err += dx;
		}
	}
}

//消息处理函数
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect = { 0, 0, 512, 512 };
	switch (Msg)
	{
	//窗口创建时
	case WM_CREATE:
		bmp = CreateCompatibleBitmap(GetDC(hWnd), 512, 512);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);
		FillRect(buffer_dc, &rect, CreateSolidBrush(RGB(255,255,255)));
		break;
	//窗口重绘时
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, 512, 512, buffer_dc, 0, 0, SRCCOPY);
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
			FillRect(buffer_dc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			//绘画
			DrawTriangle(TriangleVertexIndex);
			DrawLine_Algo1(0, 0, 511, 511);
			
			//强制重绘整个窗口
			BitBlt(GetDC(hWnd), 0, 0, 512, 512, buffer_dc, 0, 0, SRCCOPY);
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
		WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, NULL, NULL, hInstance, NULL);

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