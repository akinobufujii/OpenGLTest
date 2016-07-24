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

static const char*	WINDOW_TITLE		= "00_Skelton";	// �E�B���h�E��
static const bool	IS_FULLSCREEN_MODE	= false;		// �t���X�N���[�����[�h�itrue�Ńt���X�N���[���j
static const int	WINDOW_WIDTH		= 1280;			// �E�B���h�E�̉���
static const int	WINDOW_HEIGHT		= 720;			// �E�B���h�E�̏c��

//==============================================================================
// �`��
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
// �G���g���[�|�C���g
//==============================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// GLFW������
	glfwInit();

	// �E�B���h�E�쐬
	GLFWwindow* pWindow = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		(IS_FULLSCREEN_MODE)? glfwGetPrimaryMonitor() : nullptr,
		nullptr);

	// ���݂̃E�B���h�E���X���b�h�ɓn��
	glfwMakeContextCurrent(pWindow);

	// GLEW������
	glewInit();

	glfwSwapInterval(0);

	FPSCounter fpsCounter; // FPS�J�E���^�[

	// OpenGL�`�惋�[�v
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

	// GLFW�I��
	glfwTerminate();

	return 0;
}
