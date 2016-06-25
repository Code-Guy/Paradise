#include "advanceoglset.h"
#include <GL/wglext.h>
#include <stdio.h>

typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;

bool WGLisExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == NULL)
			return false;															// No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"

		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return true;															// Match
	}
}

bool InitMultisample(HWND hWnd, int sampleNum, int &pixelFormat)
{
	// See If The String Exists In WGL!
	if (!WGLisExtensionSupported("WGL_ARB_multisample"))
	{
		return false;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
	if (!wglChoosePixelFormatARB) 
	{
		return false;
	}

	// Get Our Current Device Context
	HDC hDC = GetDC(hWnd);

	int		valid;
	UINT	numFormats;
	float	fAttributes[] = {0,0};

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	int iAttributes[] =
	{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,24,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,16,
		WGL_STENCIL_BITS_ARB,0,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,sampleNum,
		0,0
	};

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);

	// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		return true;
	}
	return false;
}

bool InitVSync()
{
	char *extensions = (char *)glGetString(GL_EXTENSIONS);
	if (strstr(extensions, "WGL_EXT_swap_control"))
	{
		wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		return (wglSwapIntervalEXT != NULL) && (wglGetSwapIntervalEXT != NULL);
	}
	return false;
}

bool IsVSyncEnabled()
{
	return wglGetSwapIntervalEXT() > 0;
}

void SetVSync(bool enable)
{
	wglSwapIntervalEXT(enable);
}