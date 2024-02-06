#include "pch.h"
#include "wmcv_log/sinks/wmcv_sink_outputdbgstring.h"

namespace wmcv
{
	auto Log(LogSinkOutputDebugString&, const std::string_view message) noexcept -> void
	{
#ifdef _WIN32
		OutputDebugStringA(message.data());
#else
		printf("%s", message.data());
#endif
	}
}