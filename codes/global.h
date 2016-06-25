#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#define WIDTH 1000
#define HEIGHT 600

#define LABEL_IMAGE_SIZE 80

#define INVALID_VALUE 0xffffffff
#define INVALID_MATERIAL 0xffffffff
#define INVALID_UNIFORM_LOCATION 0xffffffff

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SNPRINTF _snprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define PI 3.1415926
#define ANGLE_TO_RADIAN_RATIO (PI / 180.0f)
#define TO_RADIAN(angle) (angle * ANGLE_TO_RADIAN_RATIO)

#define GLExitIfError                                                          \
{                                                                               \
	GLenum Error = glGetError();                                                \
	\
	if (Error != GL_NO_ERROR) {                                                 \
	printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
	exit(0);                                                                \
	}                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

void Error(const char* pFileName, unsigned int line, const char* pError);
void FileError(const char* pFileName, unsigned int line, const char* pFileError);

#define COMMON_ERROR(error) Error(__FILE__, __LINE__, error);
#define FILE_ERROR(fileError) FileError(__FILE__, __LINE__, fileError);

bool ReadFile(const char* pFileName, std::string& outFile);
unsigned char* LoadFile(const std::string filePath, int &size);//�����ļ��������ļ����ݣ�char *��

long long GetCurrentTimeMillis();

//����������
struct AppSet
{
	AppSet(glm::ivec2 resolution = glm::ivec2(1000, 600),
	bool isFullScreen = false,
	bool isVSync = true,
	int msaa = -1,
	int musicVolume = 50,
	int audioVolume = 50)
	{

	}

	glm::ivec2 resolution;//�ֱ���
	bool isFullScreen;//�Ƿ�ȫ��
	bool isVSync;//�Ƿ�����ֱͬ��
	int msaa;//MSAA���� -1Ϊ����
	int musicVolume;//��������
	int audioVolume;//��Ч����
};

extern AppSet appSet;//���������� ȫ�ֱ���

#endif //_GLOBAL_H
