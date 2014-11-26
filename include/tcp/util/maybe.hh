#ifndef ASYNC_VALUE_HH
#define ASYNC_VALUE_HH

#include "tcp/util/nothing.hh"

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
				: has_value(true), value(value) {}
			
            maybe(T&& value)
                    : has_value(true), value(std::move(value))
            {}

            maybe(std::exception* err)
                    : has_value(false), err(err)
            {
			}

            maybe(maybe<T>&& m)
                : has_value(std::move(m.has_value))
            {
                if (has_value) {
                    value = std::move(m.value);
                } else {
                    err = std::move(m.err);
                }
				m.has_value = false;
				m.err = nullptr;
            }

            ~maybe()
            {
                if (has_value) value.~T();
                else delete err;
            }

            T get()
            {
                if (has_value) return std::move(value);
				auto res = err;
				err = nullptr;
				throw res;
            }

            bool ok() const
            {
                return has_value;
            }

        private:
            bool has_value;
            union
            {
                std::exception* err;
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

        template <typename T, typename E>
        inline maybe<T> error(E const& err)
        {
            return maybe<T>(new E(err));
        }
    }
}

#endif
