#include "shadowmapfbo.h"
#include <stdio.h>

ShadowMapFBO::ShadowMapFBO()
{
	m_fbo = 0;
	m_shadowMap = 0;
}

ShadowMapFBO::~ShadowMapFBO()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
	if(m_shadowMap != 0)
	{
		glDeleteTextures(1, &m_shadowMap);
	}
}

bool ShadowMapFBO::Init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_fbo);

	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void ShadowMapFBO::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void ShadowMapFBO::BindForReading(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

void ShadowMapFBO::ResizeTexture(int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}
