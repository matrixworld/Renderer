#include <Windows.h>
#include <windef.h>

//�궨�塢�ṹ��Ķ���
#include "Renderer_Header.h"
#include "MathLine.h"

//�����win32���򣬲�ʹ��MFC��ض���
#define WIN32_LEAN_AND_MEAN

//ȫ�ֱ���
//������Ⱦ������豸������
HDC buffer_dc;
HBITMAP bmp;
//�����ζ�������(���ظ�)
//30, 45, 500, 200, 200, 450
VECTOR2D TriangleVertexList[] = { { 0, 0 }, { 500, 200 }, { 200, 450 } };

//�����ζ�����������
//���Ʒ��� ˳ʱ��
int TriangleVertexIndex[] = { 0, 1, 2 };

///////////////////////////////////////////////////////
//����Ԥ������
void DrawLine_Algo1(VECTOR2D,VECTOR2D);
//////////////////////////////////////////////////////


//��������
void swap(int *a,int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void DrawTriangle(int *VertexIndex)
{
	DrawLine_Algo1(TriangleVertexList[0], TriangleVertexList[1]);
	DrawLine_Algo1(TriangleVertexList[1], TriangleVertexList[2]);
	DrawLine_Algo1(TriangleVertexList[2], TriangleVertexList[0]);
}

//����ɭ��ķֱ���㷨
void DrawLine_Algo1(VECTOR2D p0, VECTOR2D p1)
{
	if (!LiangBarskyLineClipping(&p0, &p1)) { return; }
	//ֱ��б���Ƿ����1
	BOOL steep = ABS(p1.y - p0.y) > ABS(p1.x - p0.x);
	//�������1
	//��ֱ���� y=x ��ת���
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
	int dx = p1.x - p0.x;
	int dy = ABS(p1.y - p0.y);

	int err = dx / 2;

	//y������
	int ystep = (p0.y < p1.y) ? 1 : -1;
	//���ڻ滭�� y ��ֵ
	int painter_y = p0.y;

	for (int i = p0.x; i <= p1.x; i++) {
		if (steep)
		{
			SetPixel(buffer_dc, painter_y, RENDER_Y - 1 - i, RGB(0, 0, 0));
		}
		else
		{
			SetPixel(buffer_dc, i, RENDER_Y - 1 - painter_y, RGB(0, 0, 0));
		}
		err -= dy;
		if (err < 0) {
			painter_y += ystep;
			err += dx;
		}
	}
}

//��Ϣ������
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect = { 0, 0, RENDER_X - 1, RENDER_Y - 1 };
	switch (Msg)
	{
	//���ڴ���ʱ
	case WM_CREATE:
		bmp = CreateCompatibleBitmap(GetDC(hWnd),RENDER_X, RENDER_Y);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);

		FillRect(buffer_dc, &rect, CreateSolidBrush(RGB(255,255,255)));
		break;
	//�����ػ�ʱ
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, RENDER_X - 1, RENDER_Y - 1, buffer_dc, 0, 0, SRCCOPY);
		break;
	//�м�������ʱ
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		//��Ӧ����F5����
		if (wParam == VK_F5)
		{
			//�����ð�ɫ����memory Device Contexts
			FillRect(buffer_dc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
			//�滭
			DrawTriangle(TriangleVertexIndex);
			//ǿ���ػ���������
			BitBlt(GetDC(hWnd), 0, 0, RENDER_X - 1, RENDER_Y - 1, buffer_dc, 0, 0, SRCCOPY);
		}
		break;
	case WM_DESTROY:
		//����
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