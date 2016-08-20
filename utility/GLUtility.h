//==============================================================================
// OpenGL Utility
//==============================================================================
#pragma once

#include <fstream>
#include <limits>
#include <cassert>

// �V�F�[�_�[��ǂݍ��݃R���p�C������
bool readAndCompileShaderSource(GLuint shader, const char *filename)
{
	std::ifstream openFile;

	// �t�@�C�����J��
	openFile.open(filename, std::ios::in);
	if(openFile.fail())
	{
		printf_s("�t�@�C���I�[�v�����s");
		return false;
	}

	// �t�@�C���T�C�Y�擾
	openFile.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = openFile.gcount();
	openFile.clear();
	openFile.seekg(0, std::ios_base::beg);

	// �t�@�C���T�C�Y���̃��������m��
	GLchar* source = new GLchar[length + 1];
	if(source == nullptr)
	{
		printf_s("�������m�ێ��s");
		return false;
	}

	// �t�@�C���̒��g��ǂݍ���
	openFile.read(source, length);
	openFile.close();

	source[length] = '\0';	// �O�̂��ߏI�[��t��������

	// �����̂��ςȕ�����ɂȂ�
	std::string check = source;

	// �V�F�[�_�̃\�[�X�v���O�����̃V�F�[�_�I�u�W�F�N�g�ւ̓ǂݍ���
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// �m�ۂ����������̊J��
	delete source;

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	// ���O���擾
	if(result == GL_FALSE)
	{
		// �T�C�Y���擾
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		// ��������擾
		GLsizei length;
		GLchar* log = new GLchar[log_length];
		glGetShaderInfoLog(shader, 0xffffff, &length, log);
		assert(false);
		delete log;
	}

	return true;
}

// �V�F�[�_�̏���\������
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

// �v���O�����̏���\������
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
