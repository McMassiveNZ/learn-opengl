#ifndef WMCV_FORMAT_H_INCLUDED
#define WMCV_FORMAT_H_INCLUDED

namespace wmcv
{

class IFormatStream
{
public:
	virtual ~IFormatStream() = default;
	virtual auto push(std::string_view str) -> IFormatStream& = 0;

private:
	template <typename Arg>
	void apply_formatting(Arg&& arg);

	template <typename Arg, typename... Args>
	void unpack_format_args(std::string_view& format, Arg&& arg, Args&&... args);

	void unpack_format_args(std::string_view& format);
	auto parse_format_string(std::string_view& format) -> bool;

	template <typename... Args>
	friend void format(IFormatStream& input, std::string_view format, Args&&... args);
};

class FormatString final : public IFormatStream
{
public:
	IFormatStream& push(std::string_view str);

	[[nodiscard]] auto begin() -> std::string::iterator { return m_buffer.begin(); }
	[[nodiscard]] auto end() -> std::string::iterator { return m_buffer.end(); }
	[[nodiscard]] auto cbegin() const -> std::string::const_iterator { return m_buffer.cbegin(); }
	[[nodiscard]] auto cend() const -> std::string::const_iterator { return m_buffer.cend(); }

	[[nodiscard]] auto c_str() const -> const char* { return m_buffer.c_str(); }
	[[nodiscard]] auto view() const -> std::string_view { return std::string_view{m_buffer}; }

private:
	std::string m_buffer;
};

template <size_t N>
class InplaceFormatString : public IFormatStream
{
public:
	IFormatStream& push(std::string_view str)
	{
		const size_t count = std::min(str.length(), N - m_offset);
		std::copy_n(str.begin(), count, m_buffer.begin() + m_offset);
		m_offset += count;
		return *this;
	}

	[[nodiscard]] auto begin() -> std::string::iterator { return m_buffer.begin(); }
	[[nodiscard]] auto end() -> std::string::iterator { return m_buffer.end(); }
	[[nodiscard]] auto cbegin() const -> std::string::const_iterator { return m_buffer.cbegin(); }
	[[nodiscard]] auto cend() const -> std::string::const_iterator { return m_buffer.cend(); }

	[[nodiscard]] auto c_str() const -> const char* { return m_buffer.data(); }
	[[nodiscard]] auto view() const -> std::string_view { return std::string_view{m_buffer, N}; }

private:
	std::array<char, N> m_buffer = {};
	size_t m_offset = 0llu;
};

template <typename... Args>
void format(IFormatStream& input, std::string_view format, Args&&... args)
{
	input.unpack_format_args(format, std::forward<Args>(args)...);
}

template <typename StringType>
concept has_c_str = requires(StringType t) {
	t.c_str();
};

template <typename T, typename Enabled = void>
struct Formatter
{
	static void format(IFormatStream& s, const T&)
	{
		s.push("<?>");
	}
};

template <typename T>
struct Formatter<T, typename std::enable_if_t<has_c_str<T>>>
{
	static void format(IFormatStream& s, const T& val)
	{
		s.push(val.c_str());
	}
};

template <>
struct Formatter<bool>
{
	static void format(IFormatStream& s, bool val)
	{
		s.push(val ? "true" : "false");
	}
};

template <>
struct Formatter<int8_t>
{
	static void format(IFormatStream& f, int8_t val)
	{
		std::array<char, 5> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRId8, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<char>
{
	static void format(IFormatStream& f, char val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%c", val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<int16_t>
{
	static void format(IFormatStream& f, int16_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRId16, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<int32_t>
{
	static void format(IFormatStream& f, int32_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRId32, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<int64_t>
{
	static void format(IFormatStream& f, int64_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRIi64, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<uint8_t>
{
	static void format(IFormatStream& f, uint8_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRIu8 , val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<uint16_t>
{
	static void format(IFormatStream& f, uint16_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRIu16, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<uint32_t>
{
	static void format(IFormatStream& f, uint32_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRIu32, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<uint64_t>
{
	static void format(IFormatStream& f, uint64_t val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%" PRIu64, val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<float>
{
	static void format(IFormatStream& f, float val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%f", static_cast<double>(val));
		f.push(buffer.data());
	}
};

template <>
struct Formatter<double>
{
	static void format(IFormatStream& f, double val)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "%f", val);
		f.push(buffer.data());
	}
};

template <>
struct Formatter<const char*>
{
	static void format(IFormatStream& f, const char* val)
	{
		f.push(val);
	}
};

template <>
struct Formatter<char*>
{
	static void format(IFormatStream& f, const char* val)
	{
		f.push(val);
	}
};

template <>
struct Formatter<std::nullptr_t>
{
	static void format(IFormatStream& f, std::nullptr_t)
	{
		std::array<char, 64> buffer = {};
		snprintf(buffer.data(), buffer.size(), "0x0000000000000000");
		f.push(buffer.data());
	}
};

template <typename T>
struct Formatter<T*>
{
	static void format(IFormatStream& f, T* ptr)
	{
		if (!ptr)
		{
			f.push("0x0000000000000000");
		}
		else
		{
			std::array<char, 64> buffer = {};
			snprintf(buffer.data(), buffer.size(), "%#" PRIxPTR, reinterpret_cast<uint64_t>(ptr));
			f.push(buffer.data());
		}
	}
};


template <typename Arg>
void IFormatStream::apply_formatting(Arg&& arg)
{
	Formatter<typename std::decay_t<Arg>>::format(*this, std::forward<Arg>(arg));
}

template <typename Arg, typename... Args>
void IFormatStream::unpack_format_args(std::string_view& format, Arg&& arg, Args&&... args)
{
	while (parse_format_string(format))
	{
		apply_formatting(std::forward<Arg>(arg));
		if constexpr (sizeof...(args) > 0)
			unpack_format_args(format, std::forward<Args>(args)...);
	}
}

} // namespace wmcv

#endif // WMCV_FORMAT_H_INCLUDED