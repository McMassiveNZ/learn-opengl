#include <gtest/gtest.h>
#include "wmcv_log/wmcv_format.h"

TEST(Format, test_format_string_no_args)
{
	wmcv::FormatString str;
	wmcv::format(str, "A String");

	EXPECT_STREQ(str.c_str(), "A String");
}

TEST(Format, test_format_string_no_args_left_brace_only)
{
	wmcv::FormatString str;
	wmcv::format(str, "{");
	
	EXPECT_STREQ(str.c_str(), "{");
}

TEST(Format, test_format_string_no_args_left_brace_only_with_string_left)
{
	wmcv::FormatString str;
	wmcv::format(str, "A String{");
	
	EXPECT_STREQ(str.c_str(), "A String{");
}

TEST(Format, test_format_string_no_args_left_brace_only_with_string_right)
{
	wmcv::FormatString str;
	wmcv::format(str, "{A String");
	
	EXPECT_STREQ(str.c_str(), "{A String");
}

TEST(Format, test_format_string_no_args_right_brace_only)
{
	wmcv::FormatString str;
	wmcv::format(str, "}");
	
	EXPECT_STREQ(str.c_str(), "}");
}

TEST(Format, test_format_string_no_args_right_brace_only_with_string_left)
{
	wmcv::FormatString str;
	wmcv::format(str, "A String}");
	
	EXPECT_STREQ(str.c_str(), "A String}");
}

TEST(Format, test_format_string_no_args_right_brace_only_with_string_right)
{
	wmcv::FormatString str;
	wmcv::format(str, "}A String");
	
	EXPECT_STREQ(str.c_str(), "}A String");
}

TEST(Format, test_format_string_extra_args)
{
	wmcv::FormatString str;
	wmcv::format(str, "}A String", 1, 'b', true, 4.f);
	
	EXPECT_STREQ(str.c_str(), "}A String");
}

TEST(Format, test_format_string_braces_within_braces)
{
	wmcv::FormatString str;
	wmcv::format(str, "{{{{}}}}", "A string");
	
	EXPECT_STREQ(str.c_str(), "{{{A string}}}");
}

TEST(Format, test_format_string_bool)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}", true, false);
	
	EXPECT_STREQ(str.c_str(), "true, false");
}

TEST(Format, test_format_string_char)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}", 'a', 'b');
	
	EXPECT_STREQ(str.c_str(), "a, b");
}

TEST(Format, test_format_string_int8)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}, {}", int8_t{0x7F}, int8_t{0x7E}, int8_t{-1});
	
	EXPECT_STREQ(str.c_str(), "127, 126, -1");
}

TEST(Format, test_format_string_int16)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}, {}", int16_t{0x7FFF}, int16_t{0x7FFE}, int16_t{-5554});
	
	EXPECT_STREQ(str.c_str(), "32767, 32766, -5554");
}

TEST(Format, test_format_string_int32)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}, {}", int32_t{0x7FFFFFFF}, int32_t{0x7FFFFFFE}, int32_t{-2147483646});
	
	EXPECT_STREQ(str.c_str(), "2147483647, 2147483646, -2147483646");
}

TEST(Format, test_format_string_int64)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}, {}", 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFE, -9223372036854775806ll);
	
	EXPECT_STREQ(str.c_str(), "9223372036854775807, 9223372036854775806, -9223372036854775806");
}

TEST(Format, test_format_string_uint8)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}", uint8_t{0xFF}, uint8_t{0xFE});
	
	EXPECT_STREQ(str.c_str(), "255, 254");
}

TEST(Format, test_format_string_uint16)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}", uint16_t{0xFFFFu}, uint16_t{0xFFFE});
	
	EXPECT_STREQ(str.c_str(), "65535, 65534");
}

TEST(Format, test_format_string_uint32)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}", 0xFFFFFFFF, 0xFFFFFFFE);
	
	EXPECT_STREQ(str.c_str(), "4294967295, 4294967294");
}

TEST(Format, test_format_string_uint64)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}, {}", 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE);
	
	EXPECT_STREQ(str.c_str(), "18446744073709551615, 18446744073709551614");
}

TEST(Format, test_format_string_nullptr)
{
	wmcv::FormatString str;
	wmcv::format(str, "{}", nullptr);
	
	EXPECT_STREQ(str.c_str(), "0x0000000000000000");
}

TEST(Format, test_format_string_ptr)
{
	int* p = new int{255};
	wmcv::FormatString str;
	wmcv::format(str, "{}", p);
	
	EXPECT_STRNE(str.c_str(), "");
}

TEST(Format, test_inplace_format_string_no_args)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "A String");

	EXPECT_STREQ(str.c_str(), "A String");
}

TEST(Format, test_inplace_format_string_no_args_left_brace_only)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{");
	
	EXPECT_STREQ(str.c_str(), "{");
}

TEST(Format, test_inplace_format_string_no_args_left_brace_only_with_string_left)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "A String{");
	
	EXPECT_STREQ(str.c_str(), "A String{");
}

TEST(Format, test_inplace_format_string_no_args_left_brace_only_with_string_right)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{A String");
	
	EXPECT_STREQ(str.c_str(), "{A String");
}

TEST(Format, test_inplace_format_string_no_args_right_brace_only)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "}");
	
	EXPECT_STREQ(str.c_str(), "}");
}

TEST(Format, test_inplace_format_string_no_args_right_brace_only_with_string_left)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "A String}");
	
	EXPECT_STREQ(str.c_str(), "A String}");
}

TEST(Format, test_inplace_format_string_no_args_right_brace_only_with_string_right)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "}A String");
	
	EXPECT_STREQ(str.c_str(), "}A String");
}

TEST(Format, test_inplace_format_string_extra_args)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "}A String", 1, 'b', true, 4.f);
	
	EXPECT_STREQ(str.c_str(), "}A String");
}

TEST(Format, test_inplace_format_string_braces_within_braces)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{{{{}}}}", "A string");
	
	EXPECT_STREQ(str.c_str(), "{{{A string}}}");
}

TEST(Format, test_inplace_format_string_bool)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}", true, false);
	
	EXPECT_STREQ(str.c_str(), "true, false");
}

TEST(Format, test_inplace_format_string_char)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}", 'a', 'b');
	
	EXPECT_STREQ(str.c_str(), "a, b");
}

TEST(Format, test_inplace_format_string_int8)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}, {}", int8_t{0x7F}, int8_t{0x7E}, int8_t{-1});
	
	EXPECT_STREQ(str.c_str(), "127, 126, -1");
}

TEST(Format, test_inplace_format_string_int16)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}, {}", int16_t{0x7FFF}, int16_t{0x7FFE}, int16_t{-5554});
	
	EXPECT_STREQ(str.c_str(), "32767, 32766, -5554");
}

TEST(Format, test_inplace_format_string_int32)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}, {}", int32_t{0x7FFFFFFF}, int32_t{0x7FFFFFFE}, int32_t{-2147483646});
	
	EXPECT_STREQ(str.c_str(), "2147483647, 2147483646, -2147483646");
}

TEST(Format, test_inplace_format_string_int64)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}, {}", 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFE, int64_t{-9223372036854775806});
	
	EXPECT_STREQ(str.c_str(), "9223372036854775807, 9223372036854775806, -9223372036854775806");
}

TEST(Format, test_inplace_format_string_uint8)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}", uint8_t{0xFF}, uint8_t{0xFE});
	
	EXPECT_STREQ(str.c_str(), "255, 254");
}

TEST(Format, test_inplace_format_string_uint16)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}", uint16_t{0xFFFFu}, uint16_t{0xFFFE});
	
	EXPECT_STREQ(str.c_str(), "65535, 65534");
}

TEST(Format, test_inplace_format_string_uint32)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}", 0xFFFFFFFF, 0xFFFFFFFE);
	
	EXPECT_STREQ(str.c_str(), "4294967295, 4294967294");
}

TEST(Format, test_inplace_format_string_uint64)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}, {}", 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE);
	
	EXPECT_STREQ(str.c_str(), "18446744073709551615, 18446744073709551614");
}

TEST(Format, test_inplace_format_string_nullptr)
{
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}", nullptr);
	
	EXPECT_STREQ(str.c_str(), "0x0000000000000000");
}

TEST(Format, test_inplace_format_string_ptr)
{
	int* p = new int{255};
	wmcv::InplaceFormatString<512> str;
	wmcv::format(str, "{}", p);
	
	EXPECT_STRNE(str.c_str(), "");
}
