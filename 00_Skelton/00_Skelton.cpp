#define GLFW_INCLUDE_GLU

#if defined(_WIN32)
#pragma comment(lib, "OpenGL32.lib")
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const char*	WINDOW_TITLE		= "00_Skelton";	// ウィンドウ名
static const bool	IS_FULLSCREEN_MODE	= false;		// フルスクリーンモード（trueでフルスクリーン）
static const int	WINDOW_WIDTH		= 1280;			// ウィンドウの横幅
static const int	WINDOW_HEIGHT		= 720;			// ウィンドウの縦幅

// エントリーポイント
int main()
{
	// GLFW初期化
	glfwInit();

	// GLEW初期化
	glewInit();

	// ウィンドウ作成
	GLFWwindow* pWindow = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		(IS_FULLSCREEN_MODE)? glfwGetPrimaryMonitor() : nullptr,
		nullptr);

	// 現在のウィンドウをスレッドに渡す
	glfwMakeContextCurrent(pWindow);

	// OpenGL描画ループ
	while(glfwWindowShouldClose(pWindow) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 1, 1);

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	// GLFW終了
	glfwTerminate();

	return 0;
}
