#define GLFW_INCLUDE_GLU
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define NOMINMAX

#if defined(_WIN32)
#pragma comment(lib, "OpenGL32.lib")
#endif

#include <tchar.h>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../utility/GLUtility.h"
#include "../utility/FPSCounter.h"

//==============================================================================
// �e���`
//==============================================================================
// ���_�t�H�[�}�b�g
struct VertexFormat
{
	GLfloat pos[3];		// ���W
	GLfloat color[4];	// �F
};

// ���j�t�H�[���o�b�t�@�p�\����
struct UB_MATRICES
{
	glm::mat4	projection;	// ���e�s��
	glm::mat4	view;		// �����s��
	glm::mat4	world;		// ���[���h�s��
};

//==============================================================================
// �萔
//==============================================================================
static const char*	WINDOW_TITLE		= "02_TriangleByWorld";		// �E�B���h�E��
static const bool	IS_FULLSCREEN_MODE	= false;					// �t���X�N���[�����[�h�itrue�Ńt���X�N���[���j
static const int	WINDOW_WIDTH		= 1280;						// �E�B���h�E�̉���
static const int	WINDOW_HEIGHT		= 720;						// �E�B���h�E�̏c��

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
GLuint	g_program;		// �V�F�[�_�v���O����
GLuint	g_vao;			// ���_�z��I�u�W�F�N�g
GLuint	g_vbo;			// ���_�o�b�t�@�I�u�W�F�N�g
GLuint	g_ibo;			// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g
GLuint	g_ubo;			// ���j�t�H�[���o�b�t�@�I�u�W�F�N�g
GLuint	g_blockIndex;	// ���j�t�H�[���o�b�t�@�̃u���b�N�C���f�b�N�X

// ���_�o�b�t�@
std::vector<VertexFormat> g_vertexBuffer =
{
	//	���_���W			// �F
	{ { 0.0f,  1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 1.0f,  -1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
};

// �C���f�b�N�X�o�b�t�@
std::vector<unsigned int> g_indexBuffer =
{
	0, 1, 2
};

// ���j�t�H�[���o�b�t�@�p�ϐ�
UB_MATRICES g_ubGlobalValue;

//==============================================================================
// ���\�[�X�̏�����
//==============================================================================
bool initResource()
{
	// �V�F�[�_�쐬
	// ���_�V�F�[�_�ǂݍ���
	GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	if(readAndCompileShaderSource(vertexShaderObj, "triangle.vert") == false)
	{
		printf_s("���_�V�F�[�_�ǂݍ��ݎ��s");
		return false;
	}

	// ���_�V�F�[�_�ǂݍ���
	GLuint fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	if(readAndCompileShaderSource(fragmentShaderObj, "triangle.frag") == false)
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

	// �v���O�����I�u�W�F�N�g�̃����N(�e�V�F�[�_�[�ֈ����n���p�����[�^��o�^����)
	glBindAttribLocation(g_program, 0, "pos");
	glBindAttribLocation(g_program, 1, "color");
	glBindFragDataLocation(g_program, 0, "outputColor");
	glLinkProgram(g_program);

	// ���_�z��I�u�W�F�N�g�쐬
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	// ���_�o�b�t�@�I�u�W�F�N�g�쐬
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, g_vertexBuffer.size() * sizeof(VertexFormat), g_vertexBuffer.data(), GL_STATIC_DRAW);

	// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� attribute �ϐ�����Q�Ƃł���悤�ɂ���
	size_t offset = 0;
	glVertexAttribPointer(0, ARRAYSIZE(VertexFormat::pos), GL_FLOAT, GL_FALSE, sizeof(VertexFormat), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(0);
	offset += sizeof(VertexFormat::pos);

	glVertexAttribPointer(1, ARRAYSIZE(VertexFormat::color), GL_FLOAT, GL_FALSE, sizeof(VertexFormat), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(1);
	offset += sizeof(VertexFormat::color);

	// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�쐬
	glGenBuffers(1, &g_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_indexBuffer.size() * sizeof(unsigned int), g_indexBuffer.data(), GL_STATIC_DRAW);

	// ���j�t�H�[���o�b�t�@�I�u�W�F�N�g���쐬
	glGenBuffers(1, &g_ubo);
	
	// �e��o�C���h����������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

//==============================================================================
// ���\�[�X�̔j��
//==============================================================================
void destroyResource()
{
	// ���j�t�H�[���o�b�t�@�I�u�W�F�N�g���
	glDeleteBuffers(1, &g_ubo);

	// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g���
	glDeleteBuffers(1, &g_ibo);

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

	// �g�p����V�F�[�_��ݒ�
	glUseProgram(g_program);

	// �e��s����X�V
	g_ubGlobalValue.projection = glm::perspective(glm::radians(60.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 1000.f);

	g_ubGlobalValue.view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -2.5));

	static float rotateY = 0.0f;
	rotateY += 0.1f;
	rotateY = fmodf(rotateY, 360.0f);
	g_ubGlobalValue.world = glm::mat4();
	g_ubGlobalValue.world = glm::rotate(g_ubGlobalValue.world, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	
	// ���j�t�H�[���o�b�t�@��ݒ�
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UB_MATRICES), &g_ubGlobalValue, GL_DYNAMIC_DRAW);
	GLuint blockIndex = glGetUniformBlockIndex(g_program, "MATRICES");
	glUniformBlockBinding(g_program, blockIndex, 0);

	// ���_�z��I�u�W�F�N�g��ݒ�
	glBindVertexArray(g_vao);

	// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g��ݒ�
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);

	// �`��
	glDrawElements(GL_TRIANGLES, g_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

	// �e��ݒ���O��
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
