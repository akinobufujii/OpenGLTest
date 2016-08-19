//==============================================================================
// OpenGL Utility
//==============================================================================
#pragma once

// �V�F�[�_�[��ǂݍ��݃R���p�C������
bool readAndCompileShaderSource(GLuint shader, const char *file)
{
	FILE* fp;
	const GLchar* source;
	GLsizei length;
	
	// �t�@�C�����J��
	if(fopen_s(&fp, file, "r"))
	{
		perror(file);
		return false;
	}

	// �t�@�C���T�C�Y�l��
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);

	// �t�@�C���T�C�Y���̃��������m��
	source = new GLchar[length];
	if(source == nullptr)
	{
		printf_s("�������m�ێ��s");
		return false;
	}

	// �t�@�C����擪����ǂݍ���
	fseek(fp, 0L, SEEK_SET);
	bool ret = fread((void *)source, 1, length, fp) != (size_t)length;
	fclose(fp);

	// �V�F�[�_�̃\�[�X�v���O�����̃V�F�[�_�I�u�W�F�N�g�ւ̓ǂݍ���
	if(ret)
	{
		printf_s("�V�F�[�_�[�t�@�C����ǂݍ��߂܂���ł���:%s", file);
	}
	else
	{
		glShaderSource(shader, 1, &source, &length);
		glCompileShader(shader);
	}

	// �m�ۂ����������̊J��
	delete source;

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
