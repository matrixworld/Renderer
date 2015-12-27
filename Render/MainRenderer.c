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
//�Ƿ��������
int centerCursor;
//�Ƿ��˳�
int quit;
char screen_keys[512];
//��Ⱦ���򣨿ͻ�����ϵ��
RECT rectRender;
//����ָ��λ��
//��Ļ����ϵ
POINT cursor;
//������Ⱦ������豸������
HDC hdcBuffer;
HDC hdcTexture;
HBITMAP bmpTexture;
HBITMAP bmpBackground;
HBRUSH brushBackground;
HBRUSH brushTexture;

CAMERA camera;
OBJECT Triangle[2];

////////////////////
//�������ظ������ı���//
////////////////////
//���ָ����deltaʱ�����ƶ��ľ���
POINT cursorRectModifi;

///////////
//Ԥ������//
//////////
//���������ƶ������
void CameraControl();
//��Ⱦ������һ֡
void RenderFrame(OBJECT *, int);
//���ݵ��������˳��滭������
void DrawModelListIndex(FLOAT3D *, int *,MODELVERTEX*);

//��������εļ�������
void FillTriangleTopFlat(FLOAT3D, int, int, FLOAT3D, int, int, FLOAT3D, int, int);
void FillTriangleBottomFlat(FLOAT3D, int, int, FLOAT3D, int, int, FLOAT3D, int, int);
void FillTriangle(FLOAT3D, int, int, FLOAT3D, int, int, FLOAT3D, int, int);
//�����ز�
void HandleDestroy();

///////////
//��������//
//////////

//���������
void CameraControl()
{
	if (centerCursor)
	{
		GetCursorPos(&cursorRectModifi);
		cursorRectModifi.x -= cursor.x;
		cursorRectModifi.y -= cursor.y;
		//����
		// -90 <-> 90
		camera.rotation.x += (float)cursorRectModifi.y / 15.0f;
		//����
		// -180 <-> 180
		camera.rotation.y += (float)cursorRectModifi.x / 15.0f;
	}

	if (screen_keys[VK_UP]) { camera.rotation.x -= 2; }
	if (screen_keys[VK_DOWN]) { camera.rotation.x += 2; }
	if (screen_keys[VK_LEFT]) { camera.rotation.y -= 2; }
	if (screen_keys[VK_RIGHT]) { camera.rotation.y += 2; }

	if (camera.rotation.x < -90.0f) { camera.rotation.x = -90.0f; }
	if (camera.rotation.x > 90.0f) { camera.rotation.x = 90.0f; }
	if (camera.rotation.y < -180.0f) { camera.rotation.y += 360.0f; }
	if (camera.rotation.y > 180.0f) { camera.rotation.y -= 360.0f; }

	//��������
	if (screen_keys['W'] || screen_keys['A'] || screen_keys['S'] || screen_keys['D'])
	{
		FLOAT3D MovingDirection = { 0.0f ,0.0f,0.0f };
		if (screen_keys['A']) { MovingDirection.x = -1.0f; }
		if (screen_keys['D']) { MovingDirection.x = 1.0f; }
		if (screen_keys['W']) { MovingDirection.z = 1.0f; }
		if (screen_keys['S']) { MovingDirection.z = -1.0f; }
		MATRIX4 POSMatrix4 = { 0.0f };
		POSMatrix4 = MatrixMul4(Rotation_SingleAxis('x', camera.rotation.x), Rotation_SingleAxis('y', camera.rotation.y));

		VectorTransform(MovingDirection, &MovingDirection, POSMatrix4);
		VectorUnify(&MovingDirection);

		camera.POS.x += MovingDirection.x*camera.speed;
		camera.POS.y += MovingDirection.y*camera.speed;
		camera.POS.z += MovingDirection.z*camera.speed;
	}
	if (screen_keys['Q']) { camera.POS.y += camera.speed; return; }
	if (screen_keys['E']) { camera.POS.y -= camera.speed; return; }
}
//��Ⱦһ֡
void RenderFrame(OBJECT* objectList, int ObjectTotalNum)
{
	//�ñ�����ˢ��䱳��
	FillRect(hdcBuffer, &rectRender, brushBackground);
	//�������絽�ӿھ���
	//���е�����ĵ㶼Ҫ���Ըþ���
	MATRIX4 WorldToView = GetWorldToViewMatrix4(camera);
	//�����ӿڵ���μ��ÿռ�
	MATRIX4 ViewToHomo = GetViewToHomoMatrix4(camera);

	FLOAT3D *SingleObjectTmpVertexes;
	for (int lop = 0; lop < ObjectTotalNum; lop++)
	{
		SingleObjectTmpVertexes = SingleObjectLocalToHomo(objectList[lop], WorldToView, ViewToHomo);

		//�����е�ת������Ļ�Ķ�ά����ϵ
		for (int lop2 = 0; lop2 < objectList[lop].model.vertexNum; lop2++)
		{
			SingleObjectTmpVertexes[lop2].x =
				(SingleObjectTmpVertexes[lop2].x + 1.0f)* rectRender.right / 2.0f;
			SingleObjectTmpVertexes[lop2].y =
				(SingleObjectTmpVertexes[lop2].y + 1.0f) * rectRender.bottom / 2.0f;
		}
		//����������
		DrawModelListIndex(SingleObjectTmpVertexes, objectList[lop].model.vertexListIndex, objectList[lop].model.vertexList);

		free(SingleObjectTmpVertexes);
		SingleObjectTmpVertexes = NULL;
	}
}

//���ݶ���������Ⱦ�����������
void DrawModelListIndex(FLOAT3D *vertexList, int *listIndex,MODELVERTEX *modelVertexList)
{
	FLOAT3D a, b, c;
	int index = 0;
	for (index = 0; listIndex[index] != -1; index += 3)
	{
		if (listIndex[index] == -2)
		{
			continue;
		}
		a.x = vertexList[listIndex[index]].x;
		a.y = vertexList[listIndex[index]].y;

		b.x = vertexList[listIndex[index + 1]].x;
		b.y = vertexList[listIndex[index + 1]].y;

		c.x = vertexList[listIndex[index + 2]].x;
		c.y = vertexList[listIndex[index + 2]].y;

		a.z = vertexList[listIndex[index]].z;
		b.z = vertexList[listIndex[index + 1]].z;
		c.z = vertexList[listIndex[index + 2]].z;

		//��������
		if (TriangleBackCull(a, b, c))
		{
			continue;
		}

		FillTriangle(a, modelVertexList[listIndex[index]].U, modelVertexList[listIndex[index]].V,
			b, modelVertexList[listIndex[index + 1]].U, modelVertexList[listIndex[index + 1]].V,
			c, modelVertexList[listIndex[index + 2]].U, modelVertexList[listIndex[index + 2]].V);
	}
}

//��ֵ��Ⱦ����Ϊˮƽ�ߵ������Σ��������Σ�
void FillTriangleTopFlat(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	float x1 = a.x;
	float x2 = b.x;
	float x3 = c.x;

	float y1 = a.y;
	float y2 = b.y;
	float y3 = c.y;

	//ȷ�������εķ�Χ
	int miny = (int)y3;
	int maxy = (int)y1;

	//���������UV����
	//��Χ [0, 512]
	int u, v;
	//x����������
	float xLeft, xRight;


	//͸����ȷ�Ĳ�ֵ����
	float oneoverz_Left, oneoverz_Right;
	float oneoverz_Top, oneoverz_Bottom;
	float oneoverz, oneoverz_Step;
	//UV �� 1/z
	float uoverz_Top, uoverz_Bottom;
	float uoverz_Left, uoverz_Right;
	float voverz_Top, voverz_Bottom;
	float voverz_Left, voverz_Right;

	float uoverz, uoverz_Step;
	float voverz, voverz_Step;


	//(y - y1) / (y3 - y1)
	float t_YAYCYAY;
	// (y - y2) / (y3 - y2)
	float t_YBYCYBY;

	//˫��ѭ��
	for (int y = miny; y <= maxy; y++)
	{
		//������õ��������������
		t_YAYCYAY = (y - y1) / (y3 - y1);
		t_YBYCYBY = (y - y2) / (y3 - y2);

		xLeft = t_YAYCYAY * (x3 - x1) + x1;
		xRight = t_YBYCYBY* (x3 - x2) + x2;

		//͸����ȷ�Ĳ�ֵ ֵ�ļ���
		//1 / z
		oneoverz_Top = 1 / a.z;
		oneoverz_Bottom = 1 / c.z;
		oneoverz_Left = t_YAYCYAY* (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Top = 1 / b.z;
		oneoverz_Right = t_YBYCYBY * (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Step = (oneoverz_Right - oneoverz_Left) / (xRight - xLeft);
		//U / z
		uoverz_Top = Ua / a.z;
		uoverz_Bottom = Uc / c.z;
		uoverz_Left = t_YAYCYAY* (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Top = Ub / b.z;
		uoverz_Right = t_YBYCYBY* (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Step = (uoverz_Right - uoverz_Left) / (xRight - xLeft);
		//V / z
		voverz_Top = Va / a.z;
		voverz_Bottom = Vc / c.z;
		voverz_Left = t_YAYCYAY * (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Top = Vb / b.z;
		voverz_Right = t_YBYCYBY* (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Step = (voverz_Right - voverz_Left) / (xRight - xLeft);


		int x = 0;
		for (x = (int)xLeft, oneoverz = oneoverz_Left, uoverz = uoverz_Left, voverz = voverz_Left;
		x <= xRight;
			x++, oneoverz += oneoverz_Step, uoverz += uoverz_Step, voverz += voverz_Step)
		{
			u = (int)(uoverz / oneoverz);
			v = (int)(voverz / oneoverz);
			SetPixel(hdcBuffer, x, rectRender.bottom - y, GetPixel(hdcTexture, u, v));
		}
	}
}

//��ֵ��Ⱦ�ײ�Ϊˮƽ�ߵ������Σ��������Σ�
void FillTriangleBottomFlat(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	float x1 = a.x;
	float x2 = b.x;
	float x3 = c.x;

	float y1 = a.y;
	float y2 = b.y;
	float y3 = c.y;

	//ȷ�������εķ�Χ
	int miny = (int)y2;
	int maxy = (int)y1;

	//����ֱ�Ӵ���������������UV����
	int u, v;
	//��������� x ��Χ
	float xLeft, xRight;

	//͸����ȷ�Ĳ�ֵ
	float oneoverz_Left, oneoverz_Right;
	float oneoverz_Top, oneoverz_Bottom;
	float oneoverz, oneoverz_Step;
	//UV �� 1/z
	float uoverz_Top, uoverz_Bottom;
	float uoverz_Left, uoverz_Right;
	float voverz_Top, voverz_Bottom;
	float voverz_Left, voverz_Right;

	float uoverz, uoverz_Step;
	float voverz, voverz_Step;

	//(y - a.y) / (c.y - a.y)
	float t_YAYCYAY;
	//(y - a.y) / (b.y - a.y)
	float t_YAYBYAY;

	//˫��ѭ��
	//һ��һ�е���Ⱦ
	for (int y = miny; y <= maxy; y++)
	{
		//������õ��������������
		t_YAYCYAY = (y - y1) / (y3 - y1);
		t_YAYBYAY = (y - y1) / (y2 - y1);

		//��ǰ�е����Ҷ˵�
		xLeft = t_YAYCYAY * (x3 - x1) + x1;
		xRight = t_YAYBYAY * (x2 - x1) + x1;

		//͸����ȷ�Ĳ�ֵ ֵ�ļ���
		//1 / z
		oneoverz_Top = 1 / a.z;
		oneoverz_Bottom = 1 / c.z;
		oneoverz_Left = t_YAYCYAY * (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Bottom = 1 / b.z;
		oneoverz_Right = t_YAYBYAY * (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Step = (oneoverz_Right - oneoverz_Left) / (xRight - xLeft);
		//U / z
		uoverz_Top = Ua / a.z;
		uoverz_Bottom = Uc / c.z;
		uoverz_Left = t_YAYCYAY * (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Bottom = Ub / b.z;
		uoverz_Right = t_YAYBYAY * (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Step = (uoverz_Right - uoverz_Left) / (xRight - xLeft);
		//V / z
		voverz_Top = Va / a.z;
		voverz_Bottom = Vc / c.z;
		voverz_Left = t_YAYCYAY * (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Bottom = Vb / b.z;
		voverz_Right = t_YAYBYAY * (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Step = (voverz_Right - voverz_Left) / (xRight - xLeft);


		int x = 0;
		for (x = (int)xLeft, oneoverz = oneoverz_Left, uoverz = uoverz_Left, voverz = voverz_Left;
		x <= xRight;
			x++, oneoverz += oneoverz_Step, uoverz += uoverz_Step, voverz += voverz_Step)
		{
			u = (int)(uoverz / oneoverz);
			v = (int)(voverz / oneoverz);
			SetPixel(hdcBuffer, x, rectRender.bottom - y, GetPixel(hdcTexture, u, v));
		}
	}
}

//������������ηָ�Ϊ�����Ǻ�������
//������Ⱦ
void FillTriangle(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	FLOAT3D tmpPoint;
	int tmpU, tmpV;
	//�����������㰴�մ��ϵ���ABC��˳����������
	//A��������
	if (a.y > b.y && a.y > c.y) {
		if (b.y < c.y) {
			tmpPoint = b; tmpU = Ub; tmpV = Vb;
			b = c; Ub = Uc; Vb = Vc;
			c = tmpPoint; Uc = tmpU; Vc = tmpV;
		}
	}
	else if (b.y > a.y && b.y > c.y) {
		//B ������
		//A B ����
		tmpPoint = b; tmpU = Ub; tmpV = Vb;
		b = a; Ub = Ua; Vb = Va;
		a = tmpPoint; Ua = tmpU; Va = tmpV;
		if (b.y < c.y) {
			tmpPoint = b; tmpU = Ub; tmpV = Vb;
			b = c; Ub = Uc; Vb = Vc;
			c = tmpPoint; Uc = tmpU; Vc = tmpV;
		}
	}
	else if (c.y > a.y && c.y > b.y) {
		//C ������
		//A C ����
		tmpPoint = c; tmpU = Uc; tmpV = Vc;
		c = a; Uc = Ua; Vc = Va;
		a = tmpPoint; Ua = tmpU; Va = tmpV;
		if (b.y < c.y) {
			tmpPoint = b; tmpU = Ub; tmpV = Vb;
			b = c; Ub = Uc; Vb = Vc;
			c = tmpPoint; Uc = tmpU; Vc = tmpV;
		}
	}

	//����Ѿ���ƽ�׵�������
	if (ABS(b.y - a.y) < 0.001) {
		FillTriangleTopFlat(a, Ua, Va, b, Ub, Vb, c, Uc, Vc);
		return;
	}
	if (ABS(b.y - c.y) < 0.001) {
		FillTriangleBottomFlat(a, Ua, Va, b, Ub, Vb, c, Uc, Vc);
		return;
	}

	//TODO-BUG
	//�о�BUG��������
	//���˴����UV����

	//�������η�Ϊ����������

	/*
	     A
	    /|
	  /  |
	/    |
  B---D    <==�·ָ��D�㽫�����η�Ϊ����������
    \    |
      \  |
	    C
	*/

	//�·ָ��D��
	FLOAT3D d;
	int Ud, Vd;

	float t_BYAYCYAY = (b.y - a.y) / (c.y - a.y);
	d.x = t_BYAYCYAY * (c.x - a.x) + a.x;
	d.y = b.y;
	d.z = t_BYAYCYAY * (c.z - a.z) + a.z;

	float oneoverz = t_BYAYCYAY * (1 / c.z - 1 / a.z) + 1 / a.z;
	float uvoverz = t_BYAYCYAY * (Uc / c.z - Ua / a.z) + Ua / a.z;
	Ud = (int)(uvoverz / oneoverz);
	uvoverz = t_BYAYCYAY * (Vc / c.z - Va / a.z) + Va / a.z;
	Vd = (int)(uvoverz / oneoverz);

	//����¸�������D����B�����
	if (d.x < b.x)
	{
		FillTriangleBottomFlat(a, Ua, Va, b, Ub, Vb, d, Ud, Vd);
		FillTriangleTopFlat(d, Ud, Vd, b, Ub, Vb, c, Uc, Vc);
	}
	else {
		FillTriangleBottomFlat(a, Ua, Va, d, Ud, Vd, b, Ub, Vb);
		FillTriangleTopFlat(b, Ub, Vb, d, Ud, Vd, c, Uc, Vc);
	}
}

//����
void HandleDestroy()
{
	DeleteDC(hdcBuffer);
	DeleteDC(hdcTexture);

	DeleteObject(bmpTexture);
	DeleteObject(bmpBackground);

	DeleteObject(brushBackground);
	DeleteObject(brushTexture);
}

///////////////////
//�������Ϣ������//
//////////////////
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT tmptexture;
	tmptexture.left = tmptexture.top = 0;
	tmptexture.right = tmptexture.bottom = 512;
	switch (Msg)
	{
	//�Ҽ�����ʱ����ָ��
	case WM_RBUTTONDOWN:
		//������ͼ
		bmpTexture = (HBITMAP)LoadImage(NULL, TEXT("CheckBroadTexture.bmp"), IMAGE_BITMAP, 512, 512, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		brushTexture = CreatePatternBrush(bmpTexture);
		hdcTexture = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(hdcTexture, bmpTexture);
		FillRect(hdcTexture, &tmptexture, brushTexture);
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		SetCursorPos(cursor.x, cursor.y);
		centerCursor = !centerCursor;
		if (centerCursor)
			ShowCursor(FALSE);
		else
			ShowCursor(TRUE);
		break;
	case WM_SIZE:
		//�����¼�����Ⱦ�ռ��С���߿��
		//�����¼����������λ��

		//ȡ������Ļ�Ĵ�С
		rectRender.right = LOWORD(lParam);
		rectRender.bottom = HIWORD(lParam);
		rectRender.left = 0;
		rectRender.top = 0;
		//ɾ��ԭ����С�Ļ���
		HandleDestroy();

		//���뱳��
		bmpBackground = (HBITMAP)LoadImage(NULL, TEXT("Background.bmp"), IMAGE_BITMAP, rectRender.right, rectRender.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		//����������ˢ
		brushBackground = CreatePatternBrush(bmpBackground);
		hdcBuffer = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(hdcBuffer, bmpBackground);
		//�߿��
		camera.screenAspect = (float)rectRender.right / (float)rectRender.bottom;

	//����Ҫbreak
	//�ƶ����ڽ������¼���ָ���λ��
	case WM_MOVE:
		//���¼�����������λ��
		cursor.x = rectRender.right / 2;
		cursor.y = rectRender.bottom / 2;
		ClientToScreen(hWnd, &cursor);
		break;
	case WM_CREATE:
		//��ʼ���������弰�����
		InitCamera(&camera, 0, 0, -200, 0, 0, 0, 10, 1500, 70, (float)rectRender.right / (float)rectRender.bottom, 5.0f);
		InitObject(&Triangle[0], 0, 0, 0, 0, 0, 0, 1);
		InitObject(&Triangle[1], 0, 0, 0, 0, 0, 0, 0);
		break;
	//�����ػ�ʱ
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, rectRender.right, rectRender.bottom, hdcBuffer, 0, 0, SRCCOPY);
		break;
	//�м�������ʱ
	case WM_KEYDOWN:
		screen_keys[wParam & 511] = 1;
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		if (wParam == VK_F1)
		{
			//TODO
		}
		break;
	case WM_KILLFOCUS:
		centerCursor = 0;
		ShowCursor(TRUE);
		break;
	case WM_KEYUP:
		screen_keys[wParam & 511] = 0; 
		break;
	case WM_DESTROY:
		quit = 1;
		//����ģ�ͣ�����
		DeleteModel(&Triangle[0].model);
		DeleteModel(&Triangle[1].model);
		HandleDestroy();

		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/////////
//������//
/////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd)
{
	int WINDOW_X = GetSystemMetrics(SM_CXSCREEN);
	int WINDOW_Y = GetSystemMetrics(SM_CYSCREEN);
	//ȫ�ֱ�����ֵ
	rectRender.top = 0; rectRender.left = 0; rectRender.bottom = RENDER_Y; rectRender.right = RENDER_X;
	centerCursor = 0;
	quit = 0;
	for (int lop = 0; lop < 512; lop++)
	{
		screen_keys[lop] = 0;
	}

	wchar_t Name[] = TEXT("Software Renderer");
	WNDCLASSEX wnd = { 0 };
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = NULL;
	wnd.hIconSm = NULL;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WinProc;
	wnd.lpszClassName = Name;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wnd);

	//����ʵ�ʴ��ڴ�С�����ϱ߿�ͱ�����
	AdjustWindowRectEx(&rectRender, WS_OVERLAPPEDWINDOW, 0, WS_EX_CLIENTEDGE);
	HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, Name, TEXT("Software Renderer"),
		WS_OVERLAPPEDWINDOW, (WINDOW_X - rectRender.right + rectRender.left) / 2, (WINDOW_Y - rectRender.bottom + rectRender.top) / 2,
		rectRender.right - rectRender.left, rectRender.bottom - rectRender.top, NULL, NULL, hInstance, NULL);
	rectRender.top = 0; rectRender.left = 0; rectRender.bottom = RENDER_Y; rectRender.right = RENDER_X;
	if (!hWnd) {
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	//��Ϣѭ������
	//��Ӧ�û�����
	while (!screen_keys[VK_ESCAPE] && !quit) 
	{
		//û��Ϣ�ͻ�����
		while (1)
		{
			if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CameraControl();

		RenderFrame(Triangle, 2);
		BitBlt(GetDC(hWnd), 0, 0, rectRender.right, rectRender.bottom, hdcBuffer, 0, 0, SRCCOPY);

		if (centerCursor)
		{
			SetCursorPos(cursor.x, cursor.y);
		}
		Sleep(10);
	}
	return msg.message;
}