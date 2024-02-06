#ifndef WMCV_LOG_SYSTEM_H_INCLUDED
#define WMCV_LOG_SYSTEM_H_INCLUDED

namespace wmcv
{
class LogSink;

struct LogSystem
{
	virtual ~LogSystem() = default;
	virtual void PushSink(LogSink&& sink) = 0;
	virtual void LogMessage(const std::string_view text) = 0;

	LogSystem& operator=(const LogSystem&) = delete;
	LogSystem(const LogSystem&) = delete;
	LogSystem& operator=(LogSystem&&) = default;
	LogSystem(LogSystem&&) = default;

protected:
	LogSystem() = default;
};

void CreateDefaultLogSystem() noexcept;
void SetLogSystem(LogSystem* system) noexcept;
auto GetLogSystem() noexcept -> LogSystem&;

} // namespace wmcv
#endif //WMCV_LOG_SYSTEM_H_INCLUDED