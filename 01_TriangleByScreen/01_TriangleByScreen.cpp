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
// 各種定義
//==============================================================================
// 頂点フォーマット
struct VertexFormat
{
	GLfloat pos[2];
//	GLfloat col[3];
};

//==============================================================================
// 定数
//==============================================================================
static const char*	WINDOW_TITLE		= "01_TriangleByScreen";	// ウィンドウ名
static const bool	IS_FULLSCREEN_MODE	= false;					// フルスクリーンモード（trueでフルスクリーン）
static const int	WINDOW_WIDTH		= 1280;						// ウィンドウの横幅
static const int	WINDOW_HEIGHT		= 720;						// ウィンドウの縦幅

//==============================================================================
// グローバル変数
//==============================================================================
GLuint	g_program;	// シェーダプログラム
GLuint	g_vao;		// 頂点配列オブジェクト
GLuint	g_vbo;		// 頂点バッファオブジェクト
GLuint	g_ibo;		// インデックスバッファオブジェクト

std::vector<VertexFormat> g_vertexBuffer =
{
	//	頂点座標
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
//	//	頂点座標				色
//	{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
//	{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
//	{ { 0.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }
//};

//==============================================================================
// リソースの初期化
//==============================================================================
bool initResource()
{
	// シェーダ作成
	// 頂点シェーダ読み込み
	GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	if(readAndCompileShaderSource(vertexShaderObj, "D:/MySamples/OpenGLTest/01_TriangleByScreen/triangle.vert") == false)
	{
		printf_s("頂点シェーダ読み込み失敗");
		return false;
	}

	// 頂点シェーダ読み込み
	GLuint fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	if(readAndCompileShaderSource(fragmentShaderObj, "D:/MySamples/OpenGLTest/01_TriangleByScreen/triangle.frag") == false)
	{
		printf_s("フラグメントシェーダ読み込み失敗");
		return false;
	}

	// シェーダオブジェクトの取り付け
	g_program = glCreateProgram();
	glAttachShader(g_program, vertexShaderObj);
	glDeleteShader(vertexShaderObj);
	glAttachShader(g_program, fragmentShaderObj);
	glDeleteShader(fragmentShaderObj);

	// プログラムオブジェクトのリンク
	glBindAttribLocation(g_program, 0, "pos");
	glBindFragDataLocation(g_program, 0, "outputColor");
	glLinkProgram(g_program);

	// 頂点バッファオブジェクトセットアップ
	// 頂点配列オブジェクト作成
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	// 頂点バッファオブジェクト作成
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, g_vertexBuffer.size() * sizeof(VertexFormat), g_vertexBuffer.data()->pos, GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを attribute 変数から参照できるようにする
	glVertexAttribPointer(0, ARRAYSIZE(g_vertexBuffer.data()->pos), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 頂点バッファオブジェクトと頂点配列オブジェクトの結合を解除する
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

//==============================================================================
// リソースの破棄
//==============================================================================
void destroyResource()
{
	// 頂点バッファオブジェクト解放
	glDeleteBuffers(1, &g_vbo);

	// 頂点配列オブジェクト解放
	glDeleteVertexArrays(1, &g_vao);
}

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

	// シェーダプログラムの使用
	glUseProgram(g_program);

	// 図形の描画
	glBindVertexArray(g_vao);
	glDrawArrays(GL_QUADS, 0, g_vertexBuffer.size());
	glBindVertexArray(0);

	glUseProgram(0);
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
	GLenum error = glewInit();
	if(error != GLEW_OK)
	{
		char str[0xff];
		sprintf_s(str, "%s", glewGetErrorString(error));

		MessageBox(NULL, str, str, 0);
	}

	glfwSwapInterval(0);

	// リソースの初期化
	if(initResource() == false)
	{
		return 1;
	}

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

	// リソースの破棄
	destroyResource();

	// GLFW終了
	glfwTerminate();

	return 0;
}
