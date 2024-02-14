#include "pch.h"
#include "clock.h"

using system_clock = std::chrono::system_clock;

namespace wmcv
{
Clock::Clock()
	: m_start(system_clock::now())
{
}

float Clock::elapsed() const
{
	return std::chrono::duration<float>(system_clock::now() - m_start).count();
}

void Clock::nextFrame()
{
	const auto now = system_clock::now();
	m_dt = std::chrono::duration<float>(now - m_last).count();
	m_last = now;
}

}