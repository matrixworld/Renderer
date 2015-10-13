#include <Windows.h>
#include <windef.h>

#define RENDER_X (512)
#define RENDER_Y (512)

//���ڴ�СΪ600x600
//ֻ��ֹ���������ص�
//ƫ��һ��
#define OFFSET(x) ((x) + 35)
//�����ֵAbsolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//��¼��Ⱦ����Ļ���
COLORREF buffer[RENDER_X*RENDER_Y] = { 0 };

//��������
void swap(int *a,int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void InitBuffer()
{
	for (int lop = 0; lop < RENDER_X; lop++)
	{
		for (int lop2 = 0; lop2 < RENDER_Y; lop2++)
		{
			buffer[lop*RENDER_X + lop2] = RGB(255, 255, 255);
		}
	}
}

//����ɭ��ķֱ���㷨
void DrawLine_Algo1(int x0, int y0, int x1, int y1)
{
	//ֱ��б���Ƿ����1
	BOOL steep = ABS(y1 - y0) > ABS(x1 - x0);
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
	
	float err = 0;	

	//y������
	int ystep = (y0 < y1) ? 1 : -1;
	//���ڻ滭�� y ��ֵ
	int painter_y = y0;
	//err������
	float derr = (float)dy / (float)dx;

	for (int i = x0; i <= x1; i++) {
		if (steep)
		{
			buffer[painter_y* RENDER_X + i] = RGB(0, 0, 0);
		}
		else
		{
			buffer[i* RENDER_X + painter_y] = RGB(0, 0, 0);
		}
		err += derr;
		if (err >= 0.5 || err <= -0.5) {
			painter_y += ystep;
			err -= 1.0;
		}
	}
}

void RenderToScreen(PAINTSTRUCT *paint)
{
	int lop, lop2;
	//�������
	for (lop = -1; lop <= 512; lop++)
	{
		SetPixel(paint->hdc, OFFSET(lop), OFFSET(-1), RGB(255, 0, 0));
		SetPixel(paint->hdc, OFFSET(lop), OFFSET(512), RGB(255, 0, 0));
	}
	for (lop = 0; lop <= 511; lop++)
	{
		SetPixel(paint->hdc, OFFSET(-1), OFFSET(lop), RGB(255, 0, 0));
		SetPixel(paint->hdc, OFFSET(512), OFFSET(lop), RGB(255, 0, 0));
	}
	//�����������ͼ��
	for (lop = 0; lop < RENDER_X; lop++)
	{
		for (lop2 = 0; lop2 < RENDER_Y; lop2++)
		{
			SetPixel(paint->hdc, OFFSET(lop), OFFSET(lop2), buffer[lop*RENDER_X + lop2]);
		}
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paint;

	switch (Msg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &paint);
		SelectObject(paint.hdc, GetStockObject(DC_BRUSH));

		RenderToScreen(&paint);

		EndPaint(hWnd, &paint);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		//��Ӧ����F5����
		if (wParam == VK_F5)
		{
			//˳ʱ�뻭������8�������ֱ��
			//Up
			DrawLine_Algo1(256, 256, 256, 512);
			//Up-Right
			DrawLine_Algo1(256, 256, 512, 512);
			//Right
			DrawLine_Algo1(256, 256, 512, 256);
			//Down-Right
			DrawLine_Algo1(256, 256, 512, 0);
			//Down
			DrawLine_Algo1(256, 256, 256, 0);
			//Down-Left
			DrawLine_Algo1(256, 256, 0, 0);
			//Left
			DrawLine_Algo1(256, 256, 0, 256);
			//Up-Left
			DrawLine_Algo1(256, 256, 0, 512);
			
			//ǿ���ػ���������
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd) 
{
	InitBuffer();

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
		WS_OVERLAPPEDWINDOW, 100, 100, 600, 630, NULL, NULL, hInstance, NULL);

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