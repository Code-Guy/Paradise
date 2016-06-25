#ifndef _PRIMITIVERENDERER_H
#define _PRIMITIVERENDERER_H

#include "lineshader.h"
#include "ray.h"
#include "camera.h"
#include <vector>

class PrimitiveRenderer
{
public:
	static void Init();
	static void DrawCubeWireFrame(glm::vec3 position, glm::vec4 color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));//绘制网格模式的block 
	static void DrawRay(const Ray &ray, glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), float length = 10.0f);//画射线
	static void Release();

private:
	static unsigned int VB_ID;
	static LineShader *lineShader;
	static const Camera *camera;
	static void DrawBase(const std::vector<glm::vec3> &vertices, glm::vec4 color);
};

#endif //_PRIMITIVERENDERER_H