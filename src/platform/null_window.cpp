#include "window.h"

namespace ogl_starter
{

struct NullWindowImpl final
{
};

void PumpMessages(NullWindowImpl&)
{
}

bool ShouldClose(const NullWindowImpl&)
{
	return false;
}

void* GetNativeHandle(const NullWindowImpl&)
{
	return nullptr;
}

} // namespace ogl_starter

ogl_starter::Window oglsCreateWindow(ogl_starter::WindowCreateParams)
{
	return ogl_starter::NullWindowImpl{};
}
