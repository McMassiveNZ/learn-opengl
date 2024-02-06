#include "pch.h"
#include "wmcv_format.h"

namespace wmcv
{
void IFormatStream::unpack_format_args(std::string_view& format)
{
	push(format);
	format = {};
}

auto IFormatStream::parse_format_string(std::string_view& formatString) -> bool
{
	auto start = formatString.begin();
	auto curr = start;
	auto openBrace = formatString.end();
	for ( ; curr != formatString.end(); ++curr)
	{
		if (*curr == '{')
		{
			openBrace = curr;
		}

		if (*curr == '}' && openBrace != formatString.end())
		{
			const size_t length = std::distance(start, openBrace);
			if (length > 0)
			{
				push(formatString.substr(0, length));
			}
			formatString = formatString.substr(length + 2);
			return true;
		}
	}

	const size_t length = std::distance(start, curr);
	push(formatString.substr(0, length));
	formatString = formatString.substr(length);
	return false;
}

auto FormatString::push(std::string_view str) -> IFormatStream&
{
	m_buffer += str;
	return *this;
}

} // namespace wmcv