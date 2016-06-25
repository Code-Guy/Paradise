#ifndef _OVERLAY2D_H
#define _OVERLAY2D_H

#include "overlay2dshader.h"
#include "texture2d.h"
#include <glm/glm.hpp>
#include <string>

class Overlay2D
{
public:
	static bool Init(std::string textTexFileName);
	static void Print(std::string text, int x, int y, int size, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float spanRatio = 0.2f);
	static void Draw(Texture2D *tex, int x, int y, glm::vec2 sizeRatio = glm::vec2(1.0f, 1.0f), glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	static void Draw(Texture2D *tex, int x, int y, int width, int height, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	static void Release();
	static void Resize(int width, int height);

	static Overlay2DShader *overlay2dShader;

private:
	static Texture2D *textTex;

	static unsigned int VB_ID;
	static unsigned int UVB_ID;

	static glm::vec2 ratio;
	static glm::vec2 screenSize;
};

#endif _OVERLAY2D_H