#include "pch.h"
#include "wmcv_log/wmcv_log_system.h"
#include "wmcv_log/wmcv_log_sink.h"

namespace wmcv
{

namespace
{
LogSystem* s_system = nullptr;
}

class DefaultLogSystem final : public LogSystem
{
public:
	void LogMessage(const std::string_view text) override;
	void PushSink(LogSink&& sink) override;

	std::vector<LogSink> m_sinks;
};


void DefaultLogSystem::LogMessage(const std::string_view text)
{
	for (auto& sink : m_sinks)
	{
		Log(sink, text);
	}
}

void DefaultLogSystem::PushSink(LogSink&& sink)
{
	m_sinks.emplace_back(std::move(sink));
}

void CreateDefaultLogSystem() noexcept
{
	static DefaultLogSystem defaultLogSystem;
	s_system = &defaultLogSystem;
}

void SetLogSystem(LogSystem* system) noexcept
{
	s_system = system;
}

LogSystem& GetLogSystem() noexcept
{
	assert(s_system);
	return *s_system;
}
}