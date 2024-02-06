#ifndef WMCV_LOG_H_INCLUDED
#define WMCV_LOG_H_INCLUDED

#include "wmcv_log_sink.h"
#include "wmcv_log_system.h"
#include "wmcv_format.h"

namespace wmcv
{
    template< typename... Args >
    auto LogMessage( std::string_view fmt, Args&&... args ) -> void
    {
		FormatString fmt_str;
		format(fmt_str, fmt, std::forward<Args>(args)...);
		GetLogSystem().LogMessage(fmt_str.c_str());
    }
}

#endif
