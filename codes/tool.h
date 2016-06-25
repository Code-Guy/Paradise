#ifndef _TOOL_H
#define _TOOL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <math.h>
#include <QColor>
#include <QPoint>

#define MAX_PATH_LEN 1024
#define FLOAT_EQUAL_EPSILON 0.001

void TraverseFilesInDirectory(std::string directory, std::vector<std::string> &fileNames, bool needPath);

int RandIntToInt(int lower, int upper);
float RandZeroToOne();
float RandFloatToFloat(float lower, float upper);
glm::vec3 RandVec3ToVec3(const glm::vec3 &lower, const glm::vec3 &upper);
QColor RandColorToColor(const QColor &lower, const QColor &upper);
QPoint RandPos(QPoint uiSize);
glm::vec3 Vec3Lerp(const glm::vec3& lower, const glm::vec3& upper, float t);
glm::vec4 Vec4Lerp(const glm::vec4& lower, const glm::vec4& upper, float t);
bool IsFloatEqual(float lhs, float rhs);
int Clamp(int x, int a, int b);

//标准椭球分布(minEmitterRange为0：整个椭球 minEmitterRange为1：椭球壳)
glm::vec3 UniformEllipsoidSample(const glm::vec3 & ellipsoid, float minEmitterRange);

#endif //_TOOL_H