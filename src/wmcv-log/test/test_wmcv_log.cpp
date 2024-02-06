#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "wmcv_log/wmcv_log.h"

struct IAbstractSink
{
	virtual ~IAbstractSink() = default;
	virtual void onLog(const std::string_view message) noexcept = 0;
};

struct TestSink : public IAbstractSink
{
	TestSink(std::string& s, int& c)
		: result(s)
		, counter(c)
	{
	}
	
	void onLog(const std::string_view message) noexcept override
	{
		result = message;
		++counter;
	}

	std::string& result;
	int& counter;
};


auto Log(IAbstractSink& sink, const std::string_view message) noexcept -> void
{
	sink.onLog(message);
}

class LogFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		wmcv::CreateDefaultLogSystem();
	}

	void TearDown()
	{
	}
};

TEST_F(LogFixture, test_sink)
{
	const std::string expected = "Custom Log Message 1234 5678.000000";
	std::string result;
	int counter = 0;

	wmcv::GetLogSystem().PushSink(TestSink{result, counter});
	wmcv::LogMessage("Custom Log Message {} {}", 1234, 5678.f);
	EXPECT_EQ(expected, result);
	EXPECT_EQ(counter, 1);
}