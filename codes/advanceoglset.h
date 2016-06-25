#ifndef _MSAA_H
#define _MSAA_H

#include <GL/glew.h>
#include <Windows.h>

bool WGLisExtensionSupported(const char *extension);
bool InitMultisample(HWND hWnd, int sampleNum, int &pixelFormat);
bool InitVSync();
bool IsVSyncEnabled();
void SetVSync(bool enable);

#endif //_MSAA_H