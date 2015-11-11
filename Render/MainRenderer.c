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
//是否锁定鼠标
int centerCursor;
//是否退出
int quit;
char screen_keys[512];
//渲染区域（客户坐标系）
RECT rectRender;
//储存指针位置
//屏幕坐标系
POINT cursor;
//储存渲染结果的设备上下文
HDC hdcBuffer;
HDC hdcTexture;
HBITMAP bmpTexture;
HBITMAP bmpBackground;
HBRUSH brushBackground;
HBRUSH brushTexture;

CAMERA camera;
OBJECT Triangle;

////////////////////
//被大量重复创建的变量//
////////////////////
//鼠标指针在delta时间内移动的距离
POINT cursorRectModifi;

///////////
//预先声明//
//////////
//在世界中移动摄像机
void CameraControl();
//渲染完整的一帧
void RenderFrame(OBJECT *, int);
//根据点的索引表顺序绘画三角形
void DrawModelListIndex(FLOAT3D *, int *,MODELVERTEX*);

//填充三角形的几种方法
//Line Equations
void FillTriangleAbandoned(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc);
//Scanline
void FillTriangle(FLOAT3D, FLOAT3D, FLOAT3D);
//销毁素材
void HandleDestroy();

///////////
//函数定义//
//////////
void CameraControl()
{
	if (centerCursor)
	{
		GetCursorPos(&cursorRectModifi);
		cursorRectModifi.x -= cursor.x;
		cursorRectModifi.y -= cursor.y;
		//上下
		// -90 <-> 90
		camera.rotation.x += (float)cursorRectModifi.y / 15.0f;
		//左右
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

	//操作部分
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

void RenderFrame(OBJECT* objectList, int ObjectTotalNum)
{
	//用画刷填充背景
	FillRect(hdcBuffer, &rectRender, brushBackground);
	//生成世界到视口矩阵
	//所有的物体的点都要乘以该矩阵
	MATRIX4 WorldToView = GetWorldToViewMatrix4(camera);
	//生成视口到齐次剪裁空间
	MATRIX4 ViewToHomo = GetViewToHomoMatrix4(camera);

	FLOAT3D *SingleObjectTmpVertexes;
	for (int lop = 0; lop < ObjectTotalNum; lop++)
	{
		SingleObjectTmpVertexes = SingleObjectLocalToHomo(objectList[lop], WorldToView, ViewToHomo);

		//将所有点转换到屏幕的二维坐标系
		for (int lop2 = 0; lop2 < objectList[lop].model.vertexNum; lop2++)
		{
			SingleObjectTmpVertexes[lop2].x =
				(SingleObjectTmpVertexes[lop2].x + 1.0f)* rectRender.right / 2.0f;
			SingleObjectTmpVertexes[lop2].y =
				(SingleObjectTmpVertexes[lop2].y + 1.0f) * rectRender.bottom / 2.0f;
		}
		//画出点和线
		DrawModelListIndex(SingleObjectTmpVertexes, objectList[lop].model.vertexListIndex, objectList[lop].model.vertexList);

		free(SingleObjectTmpVertexes);
		SingleObjectTmpVertexes = NULL;
	}
}

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

		if (TriangleBackCull(a, b, c))
		{
			continue;
		}

		//TODO
		FillTriangleAbandoned(a, modelVertexList[listIndex[index]].U, modelVertexList[listIndex[index]].V,
			b, modelVertexList[listIndex[index + 1]].U, modelVertexList[listIndex[index + 1]].V,
			c, modelVertexList[listIndex[index + 2]].U, modelVertexList[listIndex[index + 2]].V);
	}
}

//寻找纹理映射的写法
//测试算法
void FillTriangleAbandoned(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	float x1 = a.x;
	float x2 = b.x;
	float x3 = c.x;

	float y1 = a.y;
	float y2 = b.y;
	float y3 = c.y;

	int minx = (int)Min3(x1, x2, x3);
	int miny = (int)Min3(y1, y2, y3);
	int maxx = (int)Max3(x1, x2, x3);
	int maxy = (int)Max3(y1, y2, y3);

	//用于直接从载入的纹理采样的UV坐标
	float u, v;
	//左右两点的UV坐标
	float uLeft, uRight, vLeft, vRight;
	//UV变化量
	float uStep, vStep;
	//左右两点的 x 范围
	float xLeft, xRight;


	//速度优化
	float x12 = x1 - x2;
	float y12 = y1 - y2;
	float x23 = x2 - x3;
	float y23 = y2 - y3;
	float x31 = x3 - x1;
	float y31 = y3 - y1;

	//y - a.y
	float t_YsubAY;
	//(y - a.y) / (c.y - a.y)
	float t_YAYCYAY;
	//(y - a.y) / (b.y - a.y)
	float t_YAYBYAY;

	//双重循环
	for (int y = miny; y <= maxy; y++)
	{
		t_YsubAY = y - a.y;
		t_YAYCYAY = t_YsubAY / (c.y - a.y);
		t_YAYBYAY = t_YsubAY / (b.y - a.y);

		xLeft = t_YAYCYAY * (c.x - a.x) + a.x;
		xRight = t_YAYBYAY * (b.x - a.x) + a.x;

		uLeft = t_YAYCYAY * (Uc - Ua) + Ua;
		uRight = t_YAYBYAY * (Ub - Ua) + Ua;

		vLeft = t_YAYCYAY * (Vc - Va) + Va;
		vRight = t_YAYBYAY * (Vb - Va) + Va;

		uStep = (uRight - uLeft) / (xRight - xLeft);
		vStep = (vRight - vLeft) / (xRight - xLeft);

		int x = 0;
		for (x = (int)xLeft, u = uLeft, v = vLeft; x <= (int)xRight; x++, u += uStep, v += vStep)
		{
			if (x12*(y - y1) - y12*(x - x1) > 0 &&
				x23*(y - y2) - y23*(x - x2) > 0 &&
				x31*(y - y3) - y31*(x - x3) > 0)
			{
				SetPixel(hdcBuffer, x, rectRender.bottom - y, GetPixel(hdcTexture, (int)u, (int)v));
			}
		}
	}
}

void FillTriangle(FLOAT3D a, FLOAT3D b, FLOAT3D c)
{
	//TODO
}

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
//程序的消息处理函数//
//////////////////
LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT tmptexture;
	tmptexture.left = tmptexture.top = 0;
	tmptexture.right = tmptexture.bottom = 512;
	switch (Msg)
	{
	//右键按下时锁定指针
	case WM_RBUTTONDOWN:
		//载入贴图
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
		//需重新计算渲染空间大小及高宽比
		//需重新计算鼠标中心位置

		//取得新屏幕的大小
		rectRender.right = LOWORD(lParam);
		rectRender.bottom = HIWORD(lParam);
		rectRender.left = 0;
		rectRender.top = 0;
		//删除原来大小的画布
		HandleDestroy();

		//载入背景
		bmpBackground = (HBITMAP)LoadImage(NULL, TEXT("Background.bmp"), IMAGE_BITMAP, rectRender.right, rectRender.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		//创建背景画刷
		brushBackground = CreatePatternBrush(bmpBackground);
		hdcBuffer = CreateCompatibleDC(GetDC(hWnd));
		SelectObject(hdcBuffer, bmpBackground);
		//高宽比
		camera.screenAspect = (float)rectRender.right / (float)rectRender.bottom;

	//不需要break
	//移动窗口仅需重新计算指针的位置
	case WM_MOVE:
		//重新计算鼠标的中心位置
		cursor.x = rectRender.right / 2;
		cursor.y = rectRender.bottom / 2;
		ClientToScreen(hWnd, &cursor);
		break;
	case WM_CREATE:
		//初始化世界物体及摄像机
		InitCamera(&camera, 0, 0, -500, 0, 0, 0, 10, 1500, 70, (float)rectRender.right / (float)rectRender.bottom, 5.0f);
		InitObject(&Triangle, 0, 0, 0, 0, 0, 0);
		break;
	//窗口重绘时
	case WM_PAINT:
		BitBlt(GetDC(hWnd), 0, 0, rectRender.right, rectRender.bottom, hdcBuffer, 0, 0, SRCCOPY);
		break;
	//有键被按下时
	case WM_KEYDOWN:
		screen_keys[wParam & 511] = 1;
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		if (wParam == VK_F1)
		{
			//ShowCursor(TRUE)
			;
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
		//销毁模型！！！
		DeleteModel(&Triangle.model);
		HandleDestroy();

		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/////////
//主函数//
/////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd)
{
	int WINDOW_X = GetSystemMetrics(SM_CXSCREEN);
	int WINDOW_Y = GetSystemMetrics(SM_CYSCREEN);
	//全局变量赋值
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

	//计算实际窗口大小，算上边框和标题栏
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
	//消息循环部分
	//响应用户操作
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

		CameraControl();

		RenderFrame(&Triangle, 1);
		BitBlt(GetDC(hWnd), 0, 0, rectRender.right, rectRender.bottom, hdcBuffer, 0, 0, SRCCOPY);

		if (centerCursor)
		{
			SetCursorPos(cursor.x, cursor.y);
		}
		Sleep(10);
	}
	return msg.message;
}