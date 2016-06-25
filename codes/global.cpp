#include "global.h"

#include <iostream>
#include <fstream>
#include <Windows.h>

AppSet appSet;//global

using namespace std;

void Error(const char* pFileName, unsigned int line, const char* pError)
{
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: %s", pFileName, line, pError);
	MessageBoxA(NULL, msg, NULL, 0);    
}

void FileError(const char* pFileName, unsigned int line, const char* pFileError)
{
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError);
	MessageBoxA(NULL, msg, NULL, 0); 
}

bool ReadFile(const char* pFileName, string& outFile)
{
	ifstream f(pFileName);

	bool ret = false;

	if (f.is_open()) {
		string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		FILE_ERROR(pFileName);
	}

	return ret;
}

//�����ļ��������ļ����ݣ�char *�����ļ���С
unsigned char* LoadFile(const string filePath, int &size)
{
	FILE *file = fopen(filePath.c_str(), "rb");

	if (file == NULL)//���ļ����������ļ�·�������ڣ�ֱ�ӷ���NULL
	{
		return NULL;
	}

	//�Ȼ�ȡ�ļ���С
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	//�ص��ļ�ͷ
	fseek(file, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, size, 1, file);
	data[size] = 0;//���һ���ַ�Ϊ0��\0

	fclose(file);

	return (unsigned char *)data;
}

long long GetCurrentTimeMillis()
{
	return GetTickCount();
}