#define GLFW_INCLUDE_GLU

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const char*	WINDOW_TITLE		= "00_Skelton";	// �E�B���h�E��
static const bool	IS_FULLSCREEN_MODE	= false;		// �t���X�N���[�����[�h�itrue�Ńt���X�N���[���j
static const int	WINDOW_WIDTH		= 1280;			// �E�B���h�E�̉���
static const int	WINDOW_HEIGHT		= 720;			// �E�B���h�E�̏c��

// �G���g���[�|�C���g
int main()
{
	// GLFW������
	glfwInit();

	// GLEW������
	glewInit();

	// �E�B���h�E�쐬
	GLFWwindow* pWindow = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		(IS_FULLSCREEN_MODE)? glfwGetPrimaryMonitor() : nullptr,
		nullptr);

	// OpenGL�`�惋�[�v
	while(glfwWindowShouldClose(pWindow) == 0)
	{
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	// GLFW�I��
	glfwTerminate();

	return 0;
}
