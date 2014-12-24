#include "tcp/util/buffer.hh"

#include <gtest/gtest.h>

#include <string>
#include <bits/stl_algo.h>

using namespace std;
using namespace tcp::util;

TEST(buffer, creation)
{
	string test_string = "Just a test string!";
	buffer buf((void*) test_string.c_str(), test_string.length() + 1);
	auto other_buf = buf;

	ASSERT_EQ(string(*buf), test_string);
	ASSERT_EQ(string(*buf), string(*other_buf));
	ASSERT_TRUE(*buf != *other_buf);

	auto ptr = *buf;

	auto move_buf = std::move(buf);
	ASSERT_EQ(*move_buf, ptr);
}

TEST(buffer, operations)
{
	string test = "Test string";
	for (int i = 0; i < 7; i++) test += test;
	size_t count = test.length() + 1;

	buffer buf((void*) test.c_str(), count);
	ASSERT_EQ(count, buf.rest_length());

	int delta = 10;
	for (size_t i = 0; i < test.length(); i += delta) {
		count -= delta;
		buf += delta;
		ASSERT_EQ(count, buf.rest_length());
	}
}
