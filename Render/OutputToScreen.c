#include <Windows.h>
#include <windef.h>

#define RENDER_X (512)
#define RENDER_Y (512)

//���ڴ�СΪ600x600
//ֻ��ֹ���������ص�
//ƫ��һ��
#define OFFSET(x) ((x) + 20)
//�����ֵAbsolute
#define ABS(x) (((x) >= 0)?(x):-(x))

//ȫ�ֱ���
//������Ⱦ������豸������
HDC buffer_dc;
HBITMAP bmp;

//����Ԥ������
void DrawLine_Algo1(int,int,int,int);
void DrawTriangle(int, int, int, int, int, int);
void RenderToScreen();
//////////////////////////////////////////////////////


//��������
void swap(int *a,int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void DrawTriangle(int x0,int y0,int x1,int y1,int x2,int y2)
{
	DrawLine_Algo1(x0, y0, x1, y1);
	DrawLine_Algo1(x1, y1, x2, y2);
	DrawLine_Algo1(x2, y2, x0, y0);
}
//����ɭ��ķֱ���㷨
void DrawLine_Algo1(int x0, int y0, int x1, int y1)
{
	//ֱ��б���Ƿ����1
	BOOL steep = ABS(y1 - y0) > ABS(x1 - x0);
	//�������1
	//��ֱ���� y=x ��ת���
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
			SetPixel(buffer_dc, painter_y, i, RGB(0, 0, 0));
		}
		else
		{
			SetPixel(buffer_dc, i, painter_y, RGB(0, 0, 0));
		}
		err += derr;
		if (err >= 0.5) {
			painter_y += ystep;
			err -= 1.0;
		}
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect = { 0, 0, 512, 512 };
	switch (Msg)
	{
	case WM_CREATE:
		bmp = CreateCompatibleBitmap(GetDC(hWnd), 512, 512);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);
		FillRect(buffer_dc, &rect, CreateSolidBrush(RGB(255,255,255)));
		break;
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, 512, 512, buffer_dc, 0, 0, SRCCOPY);
		break;
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
			DrawTriangle(30, 45, 500, 200, 200, 450);
			DrawLine_Algo1(0, 0, 511, 511);
			
			//ǿ���ػ���������
			BitBlt(GetDC(hWnd), 0, 0, 512, 512, buffer_dc, 0, 0, SRCCOPY);
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