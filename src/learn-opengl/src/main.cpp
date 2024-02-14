#include "pch.h"
#include "application.h"

#include "wmcv_log\wmcv_log.h"
#include "wmcv_log\sinks\wmcv_sink_outputdbgstring.h"

auto main() -> int
{
	wmcv::CreateDefaultLogSystem();
	wmcv::GetLogSystem().PushSink(wmcv::LogSinkOutputDebugString{});

	auto app = wmcv::IApplication::Create();
	return app->run();
}
