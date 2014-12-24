#include "tcp/util/buffer.hh"
#include <cstddef>
#include <cstring>
#include <iostream>

using namespace std;
using namespace tcp::util;

buffer::buffer(void* data, size_t count)
	: data(new char[count]),
	  begin(this->data),
	  count(count)
{
	memcpy(this->data, data, count);
}

buffer::buffer(size_t count)
	: data(new char[count]),
	  begin(data),
	  count(count)
{
}

buffer::buffer(buffer const& other)
	: data(new char[other.count]),
	  begin(data),
	  count(other.count)
{
	memcpy(data, other.data, count);
}

buffer::buffer(buffer&& other)
	: data(other.data),
	  begin(other.begin),
	  count(other.count)
{
	other.data = nullptr;
}

buffer::~buffer()
{
	delete[] data;
}

buffer& buffer::operator=(buffer const& other)
{
	if (this == &other) return *this;

	char* new_data = new char[other.count];
	delete[] data;
	data = new_data;
	begin = data;
	count = other.count;
	memcpy(data, other.data, count);

	return *this;
}

void buffer::reset()
{
	begin = data;
}

size_t buffer::rest_length() const
{
	return count - (begin - data);
}

char* buffer::operator*()
{
	return begin;
}

void buffer::operator+=(int x)
{
	begin += x;
}

