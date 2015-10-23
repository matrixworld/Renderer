////////////
//ϵͳͷ�ļ�//
////////////

#include <Windows.h>
#include <windef.h>
//�����win32���򣬲�ʹ��MFC��ض���
#define WIN32_LEAN_AND_MEAN

//////////////
//�Զ���ͷ�ļ�//
//////////////

#include "MagicMath.h"
#include "UsefulFunc.h"

///////////
//ȫ�ֱ���//
//////////

//������Ⱦ������豸������
HDC buffer_dc;
HBITMAP bmp;
//ɾ���������
FLOAT2D p0, p1;
CAMERA camera;

//��������
//��8����
//Դ���������8������
OBJECT CubePoints;

//������
void FunctionTest()
{
	InitObject(&CubePoints, 0, 0, 0, 0, 0, 0);
	InitCamera(&camera, 200, 150, -600, 0, -10, 0, 50, 1024, 90, 90);

	//�����������ӿھ���
	MATRIX4 WorldToView = { 0 };
	Matrix4SetZero(&WorldToView);
	WorldToView = GetWorldToViewMatrix4(&camera);
	//����������ת���ӿ�����ϵ
	SingleObjectToViewTransform(&CubePoints, WorldToView);

	//�����ӿڵ���μ��ÿռ�
	MATRIX4 ViewToHomo = { 0 };
	Matrix4SetZero(&ViewToHomo);
	ViewToHomo = GetViewToHomoMatrix4(&camera);

	//���ӿ������µ�������б任
	SingleObectFromViewToHomoTransform(&CubePoints, ViewToHomo);


	for (int lop = 0; lop < CubePoints.model.vertexNum; lop++)
	{
		CubePoints.model.vertexList[lop].x = (CubePoints.model.vertexList[lop].x + 1.0f) * 300;
		CubePoints.model.vertexList[lop].y = (CubePoints.model.vertexList[lop].y + 1.0f) * 300;
	
		CubePoints.model.vertexList[lop].z = (CubePoints.model.vertexList[lop].z + 1.0f) * 300;
	}
}

///////////
//�����㷨//
///////////

//����ɭ��ķֱ���㷨
void DrawLine_Algo01(FLOAT2D p0, FLOAT2D p1)
{
	//��ֱ�߽��м���
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
	int dx = (int)(p1.x - p0.x);
	int dy = (int)ABS(p1.y - p0.y);

	int err = dx / 2;

	//y������
	int ystep = (p0.y < p1.y) ? 1 : -1;
	//���ڻ滭�� y ��ֵ
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

//�������Ϣ������
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect = { 0, 0, RENDER_X, RENDER_Y};
	switch (Msg)
	{
	//���ڴ���ʱ
	case WM_CREATE:
		//��������ʼ������
		bmp = CreateCompatibleBitmap(GetDC(hWnd),RENDER_X, RENDER_Y);
		buffer_dc = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(buffer_dc, bmp);
		FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));
		break;
	//�����ػ�ʱ
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
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
			FillRect(buffer_dc, &rect, CreateSolidBrush(BGCOLOR));

			//TODO
			//��ά�ռ仭��
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

			//ǿ���ػ���������
			BitBlt(GetDC(hWnd), 0, 0, RENDER_X, RENDER_Y, buffer_dc, 0, 0, SRCCOPY);
		}
		break;
	case WM_DESTROY:
		//����ģ�ͣ�����
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