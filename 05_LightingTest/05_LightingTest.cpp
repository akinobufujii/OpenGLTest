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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

#include "../utility/GLUtility.h"
#include "../utility/FPSCounter.h"
#include "../utility/MeshImporter.h"

//==============================================================================
// 各種定義
//==============================================================================
// ユニフォームバッファ用構造体
struct UB_MATRICES
{
	glm::mat4	projection;	// 投影行列
	glm::mat4	view;		// 視線行列
	glm::mat4	world;		// ワールド行列
};

//==============================================================================
// 定数
//==============================================================================
static const char*	WINDOW_TITLE		= "05_LightingTest";		// ウィンドウ名
static const bool	IS_FULLSCREEN_MODE	= false;					// フルスクリーンモード（trueでフルスクリーン）
static const int	WINDOW_WIDTH		= 1280;						// ウィンドウの横幅
static const int	WINDOW_HEIGHT		= 720;						// ウィンドウの縦幅

//==============================================================================
// グローバル変数
//==============================================================================
GLuint	g_program;			// シェーダプログラム
GLuint	g_vao;				// 頂点配列オブジェクト
GLuint	g_vbo;				// 頂点バッファオブジェクト
GLuint	g_ibo;				// インデックスバッファオブジェクト
GLuint	g_ubo;				// ユニフォームバッファオブジェクト
GLuint	g_blockIndex;		// ユニフォームバッファのブロックインデックス
GLuint	g_indexBufferSize;	// インデックスバッファサイズ
GLuint	g_texture;			// テクスチャ

// ユニフォームバッファ用変数
UB_MATRICES g_ubGlobalValue;

//==============================================================================
// リソースの初期化
//==============================================================================
bool initResource()
{
	// シェーダ作成
	// 頂点シェーダ読み込み
	GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	if(readAndCompileShaderSource(vertexShaderObj, "mesh.vert") == false)
	{
		printf_s("頂点シェーダ読み込み失敗");
		return false;
	}

	// 頂点シェーダ読み込み
	GLuint fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	if(readAndCompileShaderSource(fragmentShaderObj, "mesh.frag") == false)
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

	// プログラムオブジェクトのリンク(各シェーダーへ引き渡すパラメータを登録する)
	glBindAttribLocation(g_program, 0, "inPos");
	glBindAttribLocation(g_program, 1, "inUV");
	glBindAttribLocation(g_program, 2, "inNormal");
	glBindAttribLocation(g_program, 3, "inColor");
	glBindFragDataLocation(g_program, 0, "outputColor");
	glLinkProgram(g_program);

	// メッシュ読み込み
	MeshImporter meshData;
	if(meshData.loadMesh("Cerberus_LP.FBX", 0.015f) == false)
	{
		return false;
	}

	// 頂点配列オブジェクト作成
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	// 頂点バッファオブジェクト作成
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, meshData.getMeshDatum().front().vertices.size() * sizeof(MeshImporter::MeshData::VertexFormat), meshData.getMeshDatum().front().vertices.data(), GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを attribute 変数から参照できるようにする
	size_t offset = 0;
	glVertexAttribPointer(0, sizeof(MeshImporter::MeshData::VertexFormat::m_pos) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(MeshImporter::MeshData::VertexFormat), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(0);
	offset += sizeof(MeshImporter::MeshData::VertexFormat::m_pos);

	glVertexAttribPointer(1, sizeof(MeshImporter::MeshData::VertexFormat::m_uv) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(MeshImporter::MeshData::VertexFormat), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(1);
	offset += sizeof(MeshImporter::MeshData::VertexFormat::m_uv);

	glVertexAttribPointer(2, sizeof(MeshImporter::MeshData::VertexFormat::m_normal) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(MeshImporter::MeshData::VertexFormat), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(2);
	offset += sizeof(MeshImporter::MeshData::VertexFormat::m_normal);

	glVertexAttribPointer(3, sizeof(MeshImporter::MeshData::VertexFormat::m_color) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(MeshImporter::MeshData::VertexFormat), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(3);
	offset += sizeof(MeshImporter::MeshData::VertexFormat::m_color);

	// インデックスバッファオブジェクト作成
	glGenBuffers(1, &g_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.getMeshDatum().front().indices.size() * sizeof(unsigned int), meshData.getMeshDatum().front().indices.data(), GL_STATIC_DRAW);

	g_indexBufferSize = meshData.getMeshDatum()[0].indices.size();

	// テクスチャ作成
	int width, height, bpp;
	stbi_uc* pPixelData = stbi_load(meshData.getMeshDatum().front().textureName.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	glGenTextures(1, &g_texture);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	stbi_image_free(pPixelData);

	// ユニフォームバッファオブジェクトを作成
	glGenBuffers(1, &g_ubo);
	
	// 各種バインドを解除する
	glBindTexture(g_texture, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

//==============================================================================
// リソースの破棄
//==============================================================================
void destroyResource()
{
	// ユニフォームバッファオブジェクト解放
	glDeleteBuffers(1, &g_ubo);

	// インデックスバッファオブジェクト解放
	glDeleteBuffers(1, &g_ibo);

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
	// 深度テスト有効化
	glEnable(GL_DEPTH_TEST);

	static float count = 0;
	count += 0.0001f;
	count = fmodf(count, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, count, 1, 1);
	glClearDepth(1.0f);

	// 使用するシェーダを設定
	glUseProgram(g_program);

	// 各種行列を更新
	g_ubGlobalValue.projection = glm::perspective(glm::radians(60.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 1000.f);

	g_ubGlobalValue.view = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -2.5));

	static float rotateY = 0.0f;
	rotateY += 0.1f;
	rotateY = fmodf(rotateY, 360.0f);
	g_ubGlobalValue.world = glm::mat4();
	g_ubGlobalValue.world = glm::rotate(g_ubGlobalValue.world, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	
	// ユニフォームバッファを設定
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UB_MATRICES), &g_ubGlobalValue, GL_DYNAMIC_DRAW);
	GLuint blockIndex = glGetUniformBlockIndex(g_program, "MATRICES");
	glUniformBlockBinding(g_program, blockIndex, 0);

	// テクスチャを設定
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	// 頂点配列オブジェクトを設定
	glBindVertexArray(g_vao);

	// インデックスバッファオブジェクトを設定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);

	// 描画
	glDrawElements(GL_TRIANGLES, g_indexBufferSize, GL_UNSIGNED_INT, nullptr);

	// 各種設定を外す
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
