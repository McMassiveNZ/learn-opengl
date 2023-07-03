#include "opengl.h"

namespace ogl_starter
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

ogl_starter::OpenGL oglsCreateOpenGL(ogl_starter::OpenGLCreateParams)
{
	return ogl_starter::NullOpenGLImpl{};
}
