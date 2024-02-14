#ifndef WMCV_APPLICATION_H_INCLUDED
#define WMCV_APPLICATION_H_INCLUDED

#include "input.h"

namespace wmcv
{
	class IApplication
	{
	public:
		virtual ~IApplication() = default;
		virtual int run() = 0;
		static std::unique_ptr<IApplication> Create();
	};
}

#endif //WMCV_APPLICATION_H_INCLUDED