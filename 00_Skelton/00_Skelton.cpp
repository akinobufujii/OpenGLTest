#define GLFW_INCLUDE_GLU
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#if defined(_WIN32)
#pragma comment(lib, "OpenGL32.lib")
#endif

#include <tchar.h>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../utility/FPSCounter.h"

static const char*	WINDOW_TITLE		= "00_Skelton";	// ウィンドウ名
static const bool	IS_FULLSCREEN_MODE	= false;		// フルスクリーンモード（trueでフルスクリーン）
static const int	WINDOW_WIDTH		= 1280;			// ウィンドウの横幅
static const int	WINDOW_HEIGHT		= 720;			// ウィンドウの縦幅

//==============================================================================
// 描画
//==============================================================================
void Render()
{
	static float count = 0;
	count += 0.0001f;
	count = fmodf(count, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, count, 1, 1);
}

//==============================================================================
// エントリーポイント
//==============================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// GLFW初期化
	glfwInit();

	// ウィンドウ作成
	GLFWwindow* pWindow = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		(IS_FULLSCREEN_MODE)? glfwGetPrimaryMonitor() : nullptr,
		nullptr);

	// 現在のウィンドウをスレッドに渡す
	glfwMakeContextCurrent(pWindow);

	// GLEW初期化
	glewInit();

	glfwSwapInterval(0);

	FPSCounter fpsCounter; // FPSカウンター

	// OpenGL描画ループ
	while(glfwWindowShouldClose(pWindow) == 0)
	{
		fpsCounter.beginCount();

		Render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();

		fpsCounter.endCount();

		std::string windowTitle = std::string(WINDOW_TITLE) + " - " + std::to_string(fpsCounter.getLastFPS()) + " FPS";
		SetWindowText((glfwGetWin32Window(pWindow)), windowTitle.c_str());
	}

	// GLFW終了
	glfwTerminate();

	return 0;
}
