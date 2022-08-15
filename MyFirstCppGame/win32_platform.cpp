#include "utilities.cpp"
#include <windows.h>


//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{

//}

static bool running = true;

//void* buff_memory;
//int buff_width;
//int buff_height;
//BITMAPINFO buff_bitmapInfo;

struct Render_State {
	int height;
	int width;
	void* memory;

	BITMAPINFO bitmapInfo;
};

static Render_State renderState;

#include "platformCommon.cpp";
#include "renderer.cpp"
#include "game.cpp"



LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: {		//window was destroyed
			running = false;
		} break;

		case WM_SIZE:		//means window changed size
		{
			RECT rect;
			GetClientRect(hwnd, &rect);		//gets the client rectangle, takes in window and ptr
											//to rect struct
			renderState.width = rect.right - rect.left;
			renderState.height = rect.bottom - rect.top;

			int size = renderState.width * renderState.height * sizeof(unsigned int);

			if (renderState.memory) 
			{
				VirtualFree(renderState.memory, 0, MEM_RELEASE);
			}

			renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader); //(bmi stands for "bit map info")
			renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
			renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
			renderState.bitmapInfo.bmiHeader.biPlanes = 1;
			renderState.bitmapInfo.bmiHeader.biBitCount = 32;
			renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
		
			renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			
		} break;

		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int __stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window";
	window_class.lpfnWndProc = window_callback;

	// Register Class
	RegisterClass(&window_class);

	// Create Window
	HWND window = CreateWindow(window_class.lpszClassName, L"My First Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window); //get "Device Context"

	Input input = {};

	float deltaTime = 0.016666;
	LARGE_INTEGER frameBeginTime;
	QueryPerformanceCounter(&frameBeginTime);

	float performanceFreq;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFreq = (float)perf.QuadPart;
	}

	while (running) {
		// Input
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].change = false;
		}
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					uint vkCode = (uint)message.wParam;
					bool isDown = ((message.lParam & (1 << 31)) == 0); //don't understand this line...

#define processButton(x, vk)\
case vk: {\
input.buttons[x].change = isDown != input.buttons[x].down;\
input.buttons[x].down = isDown;\
} break;\

					switch (vkCode)
					{
						processButton(BUTTON_UP, VK_UP);
						processButton(BUTTON_DOWN, VK_DOWN);
					//	processButton(BUTTON_LEFT, VK_LEFT);
					//	processButton(BUTTON_RIGHT, VK_RIGHT);
						processButton(BUTTON_W, 'W');
						processButton(BUTTON_S, 'S');
					}
				} break;

				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}


		}

		// Simulate
		simulateGame(&input, deltaTime);


		// Render
		// after you have the memory, have to send it to the windows
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory,
			&renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFreq;
		frameBeginTime = frameEndTime;
	}

}