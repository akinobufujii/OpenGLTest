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

#include <vector>

#include "../utility/GLUtility.h"
#include "../utility/FPSCounter.h"

//==============================================================================
// �e���`
//==============================================================================
// ���_�t�H�[�}�b�g
struct VertexFormat
{
	GLfloat pos[2];
//	GLfloat col[3];
};

//==============================================================================
// �萔
//==============================================================================
static const char*	WINDOW_TITLE		= "01_TriangleByScreen";	// �E�B���h�E��
static const bool	IS_FULLSCREEN_MODE	= false;					// �t���X�N���[�����[�h�itrue�Ńt���X�N���[���j
static const int	WINDOW_WIDTH		= 1280;						// �E�B���h�E�̉���
static const int	WINDOW_HEIGHT		= 720;						// �E�B���h�E�̏c��

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
GLuint	g_program;	// �V�F�[�_�v���O����
GLuint	g_vao;		// ���_�z��I�u�W�F�N�g
GLuint	g_vbo;		// ���_�o�b�t�@�I�u�W�F�N�g
GLuint	g_ibo;		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g

std::vector<VertexFormat> g_vertexBuffer =
{
	//	���_���W
	//{ 1.0f,  1.0f, 0.0f, 0.0f },
	//{ -1.0f, 1.0f, 0.0f, 0.0f },
	//{ 0.0f, -1.0f, 0.0f, 0.0f },
	{ -0.5f, -0.5f },
	{ 0.5f, -0.5f },
	{ 0.5f,  0.5f },
	{ -0.5f,  0.5f }
};

//std::vector<VertexFormat> vertexBuffer =
//{
//	//	���_���W				�F
//	{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
//	{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
//	{ { 0.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
//};

//==============================================================================
// ���\�[�X�̏�����
//==============================================================================
bool initResource()
{
	// �V�F�[�_�쐬
	// ���_�V�F�[�_�ǂݍ���
	GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	if(readAndCompileShaderSource(vertexShaderObj, "D:/MySamples/OpenGLTest/01_TriangleByScreen/triangle.vert") == false)
	{
		printf_s("���_�V�F�[�_�ǂݍ��ݎ��s");
		return false;
	}

	// ���_�V�F�[�_�ǂݍ���
	GLuint fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	if(readAndCompileShaderSource(fragmentShaderObj, "D:/MySamples/OpenGLTest/01_TriangleByScreen/triangle.frag") == false)
	{
		printf_s("�t���O�����g�V�F�[�_�ǂݍ��ݎ��s");
		return false;
	}

	// �V�F�[�_�I�u�W�F�N�g�̎��t��
	g_program = glCreateProgram();
	glAttachShader(g_program, vertexShaderObj);
	glDeleteShader(vertexShaderObj);
	glAttachShader(g_program, fragmentShaderObj);
	glDeleteShader(fragmentShaderObj);

	// �v���O�����I�u�W�F�N�g�̃����N
	glBindAttribLocation(g_program, 0, "pos");
	glBindFragDataLocation(g_program, 0, "outputColor");
	glLinkProgram(g_program);

	// ���_�o�b�t�@�I�u�W�F�N�g�Z�b�g�A�b�v
	// ���_�z��I�u�W�F�N�g�쐬
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	// ���_�o�b�t�@�I�u�W�F�N�g�쐬
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, g_vertexBuffer.size() * sizeof(VertexFormat), g_vertexBuffer.data()->pos, GL_STATIC_DRAW);

	// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� attribute �ϐ�����Q�Ƃł���悤�ɂ���
	glVertexAttribPointer(0, ARRAYSIZE(g_vertexBuffer.data()->pos), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// ���_�o�b�t�@�I�u�W�F�N�g�ƒ��_�z��I�u�W�F�N�g�̌�������������
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

//==============================================================================
// ���\�[�X�̔j��
//==============================================================================
void destroyResource()
{
	// ���_�o�b�t�@�I�u�W�F�N�g���
	glDeleteBuffers(1, &g_vbo);

	// ���_�z��I�u�W�F�N�g���
	glDeleteVertexArrays(1, &g_vao);
}

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

	// �V�F�[�_�v���O�����̎g�p
	glUseProgram(g_program);

	// �}�`�̕`��
	glBindVertexArray(g_vao);
	glDrawArrays(GL_QUADS, 0, g_vertexBuffer.size());
	glBindVertexArray(0);

	glUseProgram(0);
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
	GLenum error = glewInit();
	if(error != GLEW_OK)
	{
		char str[0xff];
		sprintf_s(str, "%s", glewGetErrorString(error));

		MessageBox(NULL, str, str, 0);
	}

	glfwSwapInterval(0);

	// ���\�[�X�̏�����
	if(initResource() == false)
	{
		return 1;
	}

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

	// ���\�[�X�̔j��
	destroyResource();

	// GLFW�I��
	glfwTerminate();

	return 0;
}
