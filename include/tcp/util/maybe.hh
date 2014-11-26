#ifndef ASYNC_VALUE_HH
#define ASYNC_VALUE_HH

#include "tcp/util/nothing.hh"

#include <stdexcept>

namespace tcp
{
    namespace util
    {
        template <typename T>
        struct maybe
        {
            maybe(T&& value)
                    : has_value(true), value(std::move(value))
            {}

            maybe(std::exception&& err)
                    : has_value(false), err(std::move(err))
            {}

            maybe(maybe<T>&& m)
                : has_value(std::move(m.has_value))
            {
                if (has_value) {
                    value = std::move(m.value);
                } else {
                    err = std::move(m.err);
                }
            }

            ~maybe()
            {
                if (has_value) value.~T();
                else err.~exception();
            }

            T get()
            {
                if (has_value) return std::move(value);
                throw err;
            }

            bool ok() const
            {
                return has_value;
            }

        private:
            bool has_value;
            union
            {
                std::exception err;
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
        inline maybe<T> error(std::exception& err)
        {
            return maybe<T>(std::move(err));
        }

        template <typename T>
        inline maybe<T> error(std::exception&& err)
        {
            return maybe<T>(std::move(err));
        }
    }
}

#endif
