#ifndef ASYNC_VALUE_HH
#define ASYNC_VALUE_HH

#include "tcp/util/nothing.hh"
#include "tcp/util/error.hh"

#include <stdexcept>
#include <iostream>

namespace tcp
{
	namespace util
	{
		using namespace std;
		template <typename T>
		struct maybe
		{
			maybe(T const& value)
				: has_value(true),
				  value(value) {}
			
			maybe(T&& value)
				: has_value(true),
				  value(std::move(value)) {}

			maybe(error_code const& error)
				: has_value(false),
				  error(error) {}
	    
			maybe(maybe<T>&& m)
				: has_value(std::move(m.has_value))
			{
				if (has_value) {
					value = std::move(m.value);
				} else {
					error = m.error;
				}
				m.has_value = false;
				m.error.invalidate();
			}

			~maybe()
			{
				if (has_value) value.~T();
			}

			T get()
			{
				if (has_value) return std::move(value);
				error.raise();

				// Really this code is unreachable, just to avoid
				// compiler warnings
				return std::move(value);
			}

			error_code get_error() const
			{
				return error;
			}

			void raise()
			{
				if (!has_value) {
					error.raise();
				}
			}

			operator bool()
			{
				return has_value;
			}

		private:
			bool has_value;
			union
			{
				error_code error;
				T value;
			};
		};

		template <typename T>
		inline maybe<T> success(T& arg)
		{
			return maybe<T>(std::move(arg));
		}

		template <typename T>
		inline maybe<T> success(T&& arg)
		{
			return maybe<T>(std::move(arg));
		}

		inline maybe<nothing> success()
		{
			return maybe<nothing>(nothing());
		}

		template <typename T>
		inline maybe<T> error(int const& err)
		{
			return maybe<T>(error_code(err));
		}

		template <typename T>
		inline maybe<T> error(error_code const& err)
		{
			return maybe<T>(err);
		}
	}
}

#endif
