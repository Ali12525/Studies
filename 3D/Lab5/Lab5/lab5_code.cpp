#define UNICODE
#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#include <SDKDDKVer.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>

#define szTitle L" "
#define szWindowClass L" "

#define dim 4
#define point_count 30

int windowWidth = 1200, windowHeight = 1000;

double points[point_count][dim] = { 
		{0.5, 0, 0, 1}, {1, 0, 0.5, 1},
		{1, 0, 0.5, 1}, {0.75, 0, 1, 1},
		{0.75, 0, 1, 1}, {0.25, 0, 1, 1},	
		{0.25, 0, 1, 1}, {0, 0, 0.5, 1},
		{0, 0, 0.5, 1}, {0.5, 0, 0, 1},	

		{0.5, 1, 0, 1}, {1, 1, 0.5, 1},
		{1, 1, 0.5, 1}, {0.75, 1, 1, 1},
		{0.75, 1, 1, 1}, {0.25, 1, 1, 1},
		{0.25, 1, 1, 1}, {0, 1, 0.5, 1},
		{0, 1, 0.5, 1}, {0.5, 1, 0, 1},

		{0.5, 0, 0, 1}, {0.5, 1, 0, 1},
		{1, 0, 0.5, 1}, {1, 1, 0.5, 1},
		{0.75, 0, 1, 1}, {0.75, 1, 1, 1},
		{0.25, 0, 1, 1}, {0.25, 1, 1, 1},
		{0, 0, 0.5, 1}, {0, 1, 0.5, 1},
};

double crds[point_count][dim] = { 0 };
double crds_calc[point_count][dim] = { 0 };

double deltaX = 0;
double deltaY = 0;
double deltaZ = 0;

double scaleX = 300;
double scaleY = 300;
double scaleZ = 300;

double angleX = 0;
double angleY = 0;

double R = 300;

HINSTANCE hInst = NULL;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEXW wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"Menu";
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);

	RegisterClassEx(&wcex);
	hInst = hInstance;

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (!InitInstance(hInstance, nCmdShow))
		return 1;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

double radians(double deg) {
	return deg * M_PI / 180.0;
}

void mult_2d_matrix(double matrix1[point_count][dim], double matrix2[point_count][dim], double matrix_res[point_count][dim]) {
	for (unsigned int y1 = 0; y1 < point_count; y1++) {
		for (unsigned int x2 = 0; x2 < dim; x2++) {
			matrix_res[y1][x2] = 0;
			for (unsigned int y2 = 0; y2 < dim; y2++) {
				matrix_res[y1][x2] += matrix1[y1][y2] * matrix2[y2][x2];
			}
		}
	}
}

void calculate() {
	double scale_matrix[dim][dim] = {
		{-scaleX, 0, 0, 0},
		{0, scaleY, 0, 0},
		{0, 0, scaleZ, 0},
		{0, 0, 0, 1}
	};

	double angle1 = radians(angleX);
	double angle2 = radians(angleY);
	

	double view_matrix[dim][dim] = {
		{-sin(angle1), 	-cos(angle2) * cos(angle1), -sin(angle2) * cos(angle1),	0},
		{cos(angle1), 	-cos(angle2) * sin(angle1), -sin(angle2) * sin(angle1),	0},
		{0, sin(angle2), -cos(angle2), 0},
		{0, 0, R, 1} 
	};

	deltaX = windowWidth / 2;
	deltaY = windowHeight / 2;
	double perspective_matrix[dim][dim] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{deltaX, deltaY, deltaZ, 1} 
	};

	mult_2d_matrix(points, scale_matrix, crds);
	mult_2d_matrix(crds, view_matrix, crds_calc);
	mult_2d_matrix(crds_calc, perspective_matrix, crds);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		break;
	}

	case WM_PAINT: {
		RECT rect;
		GetWindowRect(hWnd, &rect);
		windowWidth = rect.right - rect.left;
		windowHeight = rect.bottom - rect.top;

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		calculate();
		for (int i = 0; i < point_count; i += 2) {
			MoveToEx(hdc, crds[i][0], crds[i][1], NULL);
			LineTo(hdc, crds[i + 1][0], crds[i + 1][1]);
		}

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_KEYDOWN: {
		int scaleDelta = 5;
		int rotateDelta = 5;
		
		if (wParam == VK_OEM_PLUS) {
			scaleX += scaleDelta;
			scaleY += scaleDelta;
			scaleZ += scaleDelta;
		}
		else if (wParam == VK_OEM_MINUS) {
			scaleX -= scaleDelta;
			scaleY -= scaleDelta;
			scaleZ -= scaleDelta;
		}

		if (wParam == VK_LEFT)
			angleX -= rotateDelta;
		else if (wParam == VK_RIGHT)
			angleX += rotateDelta;
		else if (wParam == VK_UP)
			angleY -= rotateDelta;
		else if (wParam == VK_DOWN)
			angleY += rotateDelta;

		RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}

	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}
