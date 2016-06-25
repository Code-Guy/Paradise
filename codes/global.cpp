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

//加载文件，返回文件内容（char *）和文件大小
unsigned char* LoadFile(const string filePath, int &size)
{
	FILE *file = fopen(filePath.c_str(), "rb");

	if (file == NULL)//打开文件错误，例如文件路径不存在，直接返回NULL
	{
		return NULL;
	}

	//先获取文件大小
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	//回到文件头
	fseek(file, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, size, 1, file);
	data[size] = 0;//最后一个字符为0？\0

	fclose(file);

	return (unsigned char *)data;
}

long long GetCurrentTimeMillis()
{
	return GetTickCount();
}