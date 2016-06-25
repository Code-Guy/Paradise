#include "cubemaptexture.h"
#include "global.h"
#include <FreeImage/FreeImage.h>
#include <iostream>

using namespace std;

static const GLenum CubeMapOrientation[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

CubemapTexture::CubemapTexture(const string& Directory, std::string suffix)
{
	string::const_iterator it = Directory.end();
	it--;
	string BaseDir = (*it == '/') ? Directory : Directory + "/";

	m_fileNames[0] = BaseDir + "right." + suffix;
	m_fileNames[1] = BaseDir + "left." + suffix;
	m_fileNames[2] = BaseDir + "bottom." + suffix;
	m_fileNames[3] = BaseDir + "top." + suffix;
	m_fileNames[4] = BaseDir + "front." + suffix;
	m_fileNames[5] = BaseDir + "back." + suffix;

	m_textureObj = 0;
}

CubemapTexture::~CubemapTexture()
{
	if (m_textureObj != 0) {
		glDeleteTextures(1, &m_textureObj);
	}
}

bool CubemapTexture::Load()
{
	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(CubeMapOrientation) ; i++) {
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(m_fileNames[i].c_str(), 0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, m_fileNames[i].c_str());

		FIBITMAP* temp = imagen;
		imagen = FreeImage_ConvertTo32Bits(imagen);
		FreeImage_Unload(temp);

		int w = FreeImage_GetWidth(imagen);
		int h = FreeImage_GetHeight(imagen);
		cout << "The size of the image is: " << m_fileNames[i] << " es " << w << "*" << h << endl; //Some debugging code

		GLubyte* textura = new GLubyte[4 * w * h];
		char* pixeles = (char*)FreeImage_GetBits(imagen);
		//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

		for(int j= 0; j<w*h; j++)
		{
			textura[j*4+0]= pixeles[j*4+2];
			textura[j*4+1]= pixeles[j*4+1];
			textura[j*4+2]= pixeles[j*4+0];
			textura[j*4+3]= pixeles[j*4+3];
		}

		glTexImage2D(CubeMapOrientation[i], 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);
	}    

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);           

	return true;
}


void CubemapTexture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
}
