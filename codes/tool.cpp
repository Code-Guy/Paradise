#include "tool.h"
#include <stdio.h>
#include <Windows.h>

using namespace std;

void TraverseFilesInDirectory(string directory, vector<string> &fileNames, bool needPath)
{
	char directory_ch[MAX_PATH_LEN];
	sprintf(directory_ch, "%s/*.*", directory.c_str());

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(directory_ch, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (needPath)
				{
					fileNames.push_back(directory + "/" + fd.cFileName);
				}
				else
				{
					fileNames.push_back(fd.cFileName);
				}
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
}

int RandIntToInt(int lower, int upper)
{
	if (lower == upper)
	{
		return lower;
	}
	//note:不包括上边界
	return lower + rand() % (upper - lower);
}

float RandZeroToOne()
{
	return ((float)rand()) / RAND_MAX;
}

float RandFloatToFloat(float lower, float upper)
{
	return RandZeroToOne() * (upper - lower) + lower;
}

glm::vec3 RandVec3ToVec3(const glm::vec3 &lower, const glm::vec3 &upper)
{
	return glm::vec3(RandFloatToFloat(lower.x, upper.x),
		RandFloatToFloat(lower.y, upper.y),
		RandFloatToFloat(lower.z, upper.z));
}

QColor RandColorToColor(const QColor &lower, const QColor &upper)
{
	return QColor(RandIntToInt(lower.red(), upper.red()),
		RandIntToInt(lower.green(), upper.green()),
		RandIntToInt(lower.blue(), upper.blue()),
		RandIntToInt(lower.alpha(), upper.alpha()));
}

QPoint RandPos(QPoint uiSize)
{
	return QPoint(RandIntToInt(0, uiSize.x()), RandIntToInt(0, uiSize.y()));
}

glm::vec3 Vec3Lerp(const glm::vec3& lower, const glm::vec3& upper, float t)
{
	return lower * (1 - t) + upper * t;
}

glm::vec4 Vec4Lerp(const glm::vec4& lower, const glm::vec4& upper, float t)
{
	return lower * (1 - t) + upper * t;
}

bool IsFloatEqual(float lhs, float rhs)
{
	return abs(lhs - rhs) < FLOAT_EQUAL_EPSILON;
}

int Clamp(int x, int a, int b)
{
	if (x < a)
	{
		return a;
	}
	if (x > b)
	{
		return b;
	}
	return x;
}

//标准椭球分布(minEmitterRange为0：整个椭球 minEmitterRange为1：椭球壳)
glm::vec3 UniformEllipsoidSample(const glm::vec3 & ellipsoid, float minEmitterRange)
{
	//先计算标准正球分布
	float a = RandFloatToFloat(-180.0f, 180.0f) * (3.1415926 / 180.0f);
	float b = acos(2 * RandZeroToOne() - 1);
	float r = pow(RandZeroToOne() * (1 - pow(minEmitterRange, 3) + pow(minEmitterRange, 3)), 1 / 3);

	glm::vec3 sampleP;
	sampleP.x = r * sin(b) * cos(a);
	sampleP.y = r * sin(b) * sin(a);
	sampleP.z = r * cos(b);

	return sampleP * ellipsoid;
}