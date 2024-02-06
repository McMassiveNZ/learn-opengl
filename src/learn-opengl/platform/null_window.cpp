#include "pch.h"
#include "window.h"

namespace wmcv
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

wmcv::Window oglsCreateWindow(wmcv::WindowCreateParams)
{
	return wmcv::NullWindowImpl{};
}
