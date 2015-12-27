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
OBJECT Triangle[2];

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

//填充三角形的几个函数
void FillTriangleTopFlat(FLOAT3D, int, int, FLOAT3D, int, int, FLOAT3D, int, int);
void FillTriangleBottomFlat(FLOAT3D, int, int, FLOAT3D, int, int, FLOAT3D, int, int);
void FillTriangle(FLOAT3D, int, int, FLOAT3D, int, int, FLOAT3D, int, int);
//销毁素材
void HandleDestroy();

///////////
//函数定义//
//////////

//控制摄像机
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
//渲染一帧
void RenderFrame(OBJECT* objectList, int ObjectTotalNum)
{
	//用背景画刷填充背景
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
		//画出三角形
		DrawModelListIndex(SingleObjectTmpVertexes, objectList[lop].model.vertexListIndex, objectList[lop].model.vertexList);

		free(SingleObjectTmpVertexes);
		SingleObjectTmpVertexes = NULL;
	}
}

//根据顶点索引渲染具体的三角形
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

		//背面消隐
		if (TriangleBackCull(a, b, c))
		{
			continue;
		}

		FillTriangle(a, modelVertexList[listIndex[index]].U, modelVertexList[listIndex[index]].V,
			b, modelVertexList[listIndex[index + 1]].U, modelVertexList[listIndex[index + 1]].V,
			c, modelVertexList[listIndex[index + 2]].U, modelVertexList[listIndex[index + 2]].V);
	}
}

//插值渲染顶部为水平线的三角形（下三角形）
void FillTriangleTopFlat(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	float x1 = a.x;
	float x2 = b.x;
	float x3 = c.x;

	float y1 = a.y;
	float y2 = b.y;
	float y3 = c.y;

	//确定三角形的范围
	int miny = (int)y3;
	int maxy = (int)y1;

	//最终算出的UV坐标
	//范围 [0, 512]
	int u, v;
	//x轴左右两点
	float xLeft, xRight;


	//透视正确的插值计算
	float oneoverz_Left, oneoverz_Right;
	float oneoverz_Top, oneoverz_Bottom;
	float oneoverz, oneoverz_Step;
	//UV 与 1/z
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

	//双重循环
	for (int y = miny; y <= maxy; y++)
	{
		//将多次用到的数据先算出来
		t_YAYCYAY = (y - y1) / (y3 - y1);
		t_YBYCYBY = (y - y2) / (y3 - y2);

		xLeft = t_YAYCYAY * (x3 - x1) + x1;
		xRight = t_YBYCYBY* (x3 - x2) + x2;

		//透视正确的插值 值的计算
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

//插值渲染底部为水平线的三角形（上三角形）
void FillTriangleBottomFlat(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	float x1 = a.x;
	float x2 = b.x;
	float x3 = c.x;

	float y1 = a.y;
	float y2 = b.y;
	float y3 = c.y;

	//确定三角形的范围
	int miny = (int)y2;
	int maxy = (int)y1;

	//用于直接从载入的纹理采样的UV坐标
	int u, v;
	//左右两点的 x 范围
	float xLeft, xRight;

	//透视正确的插值
	float oneoverz_Left, oneoverz_Right;
	float oneoverz_Top, oneoverz_Bottom;
	float oneoverz, oneoverz_Step;
	//UV 与 1/z
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

	//双重循环
	//一行一行的渲染
	for (int y = miny; y <= maxy; y++)
	{
		//将多次用到的数据先算出来
		t_YAYCYAY = (y - y1) / (y3 - y1);
		t_YAYBYAY = (y - y1) / (y2 - y1);

		//当前行的左右端点
		xLeft = t_YAYCYAY * (x3 - x1) + x1;
		xRight = t_YAYBYAY * (x2 - x1) + x1;

		//透视正确的插值 值的计算
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

//将传入的三角形分割为上三角和下三角
//方便渲染
void FillTriangle(FLOAT3D a, int Ua, int Va, FLOAT3D b, int Ub, int Vb, FLOAT3D c, int Uc, int Vc)
{
	FLOAT3D tmpPoint;
	int tmpU, tmpV;
	//将三角形三点按照从上到下ABC的顺序重新排列
	//A在最上面
	if (a.y > b.y && a.y > c.y) {
		if (b.y < c.y) {
			tmpPoint = b; tmpU = Ub; tmpV = Vb;
			b = c; Ub = Uc; Vb = Vc;
			c = tmpPoint; Uc = tmpU; Vc = tmpV;
		}
	}
	else if (b.y > a.y && b.y > c.y) {
		//B 最上面
		//A B 互换
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
		//C 最上面
		//A C 互换
		tmpPoint = c; tmpU = Uc; tmpV = Vc;
		c = a; Uc = Ua; Vc = Va;
		a = tmpPoint; Ua = tmpU; Va = tmpV;
		if (b.y < c.y) {
			tmpPoint = b; tmpU = Ub; tmpV = Vb;
			b = c; Ub = Uc; Vb = Vc;
			c = tmpPoint; Uc = tmpU; Vc = tmpV;
		}
	}

	//如果已经是平底的三角形
	if (ABS(b.y - a.y) < 0.001) {
		FillTriangleTopFlat(a, Ua, Va, b, Ub, Vb, c, Uc, Vc);
		return;
	}
	if (ABS(b.y - c.y) < 0.001) {
		FillTriangleBottomFlat(a, Ua, Va, b, Ub, Vb, c, Uc, Vc);
		return;
	}

	//TODO-BUG
	//感觉BUG还是在这
	//给了错误的UV坐标

	//将三角形分为上下三角形

	/*
	     A
	    /|
	  /  |
	/    |
  B---D    <==新分割的D点将三角形分为上下三角形
    \    |
      \  |
	    C
	*/

	//新分割的D点
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

	//如果新隔出来的D点在B点左边
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

//销毁
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
		InitCamera(&camera, 0, 0, -200, 0, 0, 0, 10, 1500, 70, (float)rectRender.right / (float)rectRender.bottom, 5.0f);
		InitObject(&Triangle[0], 0, 0, 0, 0, 0, 0, 1);
		InitObject(&Triangle[1], 0, 0, 0, 0, 0, 0, 0);
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
		//销毁模型！！！
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