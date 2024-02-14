#ifndef WMCV_CLOCK_H_INCLUDED
#define WMCV_CLOCK_H_INCLUDED

namespace wmcv
{
	class Clock
	{
	public:
		Clock();

		void nextFrame();

		float dt() const { return m_dt; }
		float elapsed() const;

	private:
		std::chrono::system_clock::time_point m_start;
		std::chrono::system_clock::time_point m_last;

		float m_dt;
	};
}
#endif //WMCV_CLOCK_H_INCLUDED