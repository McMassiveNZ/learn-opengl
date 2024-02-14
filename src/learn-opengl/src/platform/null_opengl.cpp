#include "pch.h"
#include "opengl.h"

namespace wmcv
{
struct NullOpenGLImpl final
{
};

void ClearBuffers(const NullOpenGLImpl&)
{
}

void Present(const NullOpenGLImpl&)
{
}

void DrawScene(const NullOpenGLImpl&)
{
}

void Destroy(const NullOpenGLImpl&)
{
}
} // namespace ogl_starter

wmcv::OpenGL oglsCreateOpenGL(wmcv::OpenGLCreateParams)
{
	return wmcv::NullOpenGLImpl{};
}
