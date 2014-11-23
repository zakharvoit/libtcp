#ifndef ASYNC_VALUE_HH
#define ASYNC_VALUE_HH

#include <stdexcept>

namespace tcp
{
	namespace util
	{
		template <typename T>
		struct async_value
		{
			async_value(T value)
				: has_value(true)
				, value(value) {}

			async_value(std::exception err)
				: has_value(false)
				, err(err) {}

			~async_value() {
				if (has_value) value.~T();
				else err.~exception();
			}

			T get() const {
				if (has_value) return value;
				throw err;
			}

		private:
			bool has_value;
			union
			{
				std::exception err;
				T value;
			};
		};
	}
}

#endif
