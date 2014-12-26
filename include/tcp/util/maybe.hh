#ifndef ASYNC_VALUE_HH
#define ASYNC_VALUE_HH

#include "tcp/util/nothing.hh"
#include "tcp/util/error.hh"

#include <stdexcept>
#include <iostream>
#include <type_traits>

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
			{
				has_value = m.has_value;
				if (has_value) {
					new (&value) T(std::move(m.value));
				} else {
					error = m.error;
				}
				m.has_value = false;
				m.error.invalidate();
			}

			maybe(maybe<T> const& m)
			{
				has_value = m.has_value;
				if (has_value) {
					new (&value) T(m.value);
				} else {
					error = m.error;
				}
			}

			~maybe()
			{
				if (has_value) value.~T();
			}

			T get()
			{
				if (!has_value) {
					error.raise();
				}

				return std::move(value);
			}

			T get() const
			{
				if (!has_value) {
					error.raise();
				}

				// We need copy constructor in type T to implement get() const.
				return value;
			}

			error_code get_error() const
			{
				return error;
			}

			void raise() const
			{
				if (!has_value) {
					error.raise();
				}
			}

			operator bool() const
			{
				return has_value;
			}

			bool operator!() const
			{
				return !bool(*this);
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
