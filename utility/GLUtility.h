//==============================================================================
// OpenGL Utility
//==============================================================================
#pragma once

#include <fstream>
#include <limits>
#include <cassert>

// シェーダーを読み込みコンパイルする
bool readAndCompileShaderSource(GLuint shader, const char *filename)
{
	std::ifstream openFile;

	// ファイルを開く
	openFile.open(filename, std::ios::in);
	if(openFile.fail())
	{
		printf_s("ファイルオープン失敗");
		return false;
	}

	// ファイルサイズ取得
	openFile.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = openFile.gcount();
	openFile.clear();
	openFile.seekg(0, std::ios_base::beg);

	// ファイルサイズ分のメモリを確保
	GLchar* source = new GLchar[length + 1];
	if(source == nullptr)
	{
		printf_s("メモリ確保失敗");
		return false;
	}

	// ファイルの中身を読み込む
	openFile.read(source, length);
	openFile.close();

	source[length] = '\0';	// 念のため終端を付け加える

	// ※何故か変な文字列になる
	std::string check = source;

	// シェーダのソースプログラムのシェーダオブジェクトへの読み込み
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// 確保したメモリの開放
	delete source;

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	// ログを取得
	if(result == GL_FALSE)
	{
		// サイズを取得
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		// 文字列を取得
		GLsizei length;
		GLchar* log = new GLchar[log_length];
		glGetShaderInfoLog(shader, 0xffffff, &length, log);
		assert(false);
		delete log;
	}

	return true;
}

// シェーダの情報を表示する
void printShaderInfoLog(GLuint shader)
{
	GLsizei bufSize;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if(bufSize > 1)
	{
		GLchar* infoLog = new GLchar[bufSize];

		if(infoLog != nullptr)
		{
			GLsizei length;

			glGetShaderInfoLog(shader, bufSize, &length, infoLog);
			fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
			delete infoLog;
		}
		else
		{
			printf_s("Could not allocate InfoLog buffer.\n");
		}
	}
}

// プログラムの情報を表示する
void printProgramInfoLog(GLuint program)
{
	GLsizei bufSize;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if(bufSize > 1)
	{
		GLchar* infoLog = new GLchar[bufSize];

		if(infoLog != nullptr)
		{
			GLsizei length;

			glGetProgramInfoLog(program, bufSize, &length, infoLog);
			printf_s("InfoLog:\n%s\n\n", infoLog);
			delete infoLog;
		}
		else
		{
			printf_s("Could not allocate InfoLog buffer.\n");
		}
	}
}
