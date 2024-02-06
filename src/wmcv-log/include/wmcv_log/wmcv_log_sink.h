#ifndef WMCV_LOG_SINK_H_INCLUDED
#define WMCV_LOG_SINK_H_INCLUDED

namespace wmcv
{

template <typename Sink>
concept IsLogSink = requires(Sink sink, const std::string_view message) {
	Log(sink, message);
};

class LogSink
{
public:
	template <IsLogSink T>
	LogSink(T&& t)
		: self{std::make_unique<model_t<T>>(std::move(t))}
	{
	}

	~LogSink() = default;
	LogSink(LogSink&&) = default;
	LogSink& operator=(LogSink&&) = default;
	LogSink(const LogSink&) = delete;
	LogSink operator=(const LogSink&) = delete;

	friend auto Log(LogSink& logger, const std::string_view msg) -> void { logger.self->Log_(msg); }

private:
	struct concept_t
	{
		virtual ~concept_t() = default;
		virtual auto Log_(const std::string_view msg) -> void = 0;

		concept_t& operator=(concept_t&&) = default;
		concept_t(concept_t&&) = default;
		concept_t& operator=(const concept_t&) = delete;
		concept_t(const concept_t&) = delete;

	protected:
		concept_t() = default;
	};

	template <typename T>
	struct model_t final : concept_t
	{
		model_t(T&& data) : m_data(std::move(data)) {}
		auto Log_(const std::string_view msg) -> void { Log(m_data, msg); }

		T m_data;
	};

	std::unique_ptr<concept_t> self;
};

}

#endif
