//==============================================================================
// OpenGL Utility
//==============================================================================
#pragma once

// シェーダーを読み込みコンパイルする
bool readAndCompileShaderSource(GLuint shader, const char *file)
{
	FILE* fp;
	const GLchar* source;
	GLsizei length;
	
	// ファイルを開く
	if(fopen_s(&fp, file, "r"))
	{
		perror(file);
		return false;
	}

	// ファイルサイズ獲得
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);

	// ファイルサイズ分のメモリを確保
	source = new GLchar[length];
	if(source == nullptr)
	{
		printf_s("メモリ確保失敗");
		return false;
	}

	// ファイルを先頭から読み込む
	fseek(fp, 0L, SEEK_SET);
	bool ret = fread((void *)source, 1, length, fp) != (size_t)length;
	fclose(fp);

	// シェーダのソースプログラムのシェーダオブジェクトへの読み込み
	if(ret)
	{
		printf_s("シェーダーファイルを読み込めませんでした:%s", file);
	}
	else
	{
		glShaderSource(shader, 1, &source, &length);
		glCompileShader(shader);
	}

	// 確保したメモリの開放
	delete source;

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
