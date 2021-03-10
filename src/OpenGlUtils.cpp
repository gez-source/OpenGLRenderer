#include "OpenGlUtils.h"

void OpenGlUtils::Antialias(bool enable)
{
	if (enable)
	{
		glEnable(GL_MULTISAMPLE);
	}
	else if (!enable) 
	{
		glDisable(GL_MULTISAMPLE);
	}
}

void OpenGlUtils::EnableAlphaBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGlUtils::EnableAdditiveBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void OpenGlUtils::DisableBlending()
{
	glDisable(GL_BLEND);
}

void OpenGlUtils::EnableDepthTesting(bool enable)
{
	if (enable)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);		// depth-testing interprets a smaller value as "closer"
	}
	else if (!enable)
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void OpenGlUtils::CullBackFaces(bool cull)
{
	if (cull)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else if (!cull)
	{
		glDisable(GL_CULL_FACE);
	}
}

void OpenGlUtils::GoWireframe(bool goWireframe)
{
	if (goWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (!goWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}