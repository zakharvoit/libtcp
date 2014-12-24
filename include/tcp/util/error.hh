#ifndef TCP_UTIL_ERROR_HH
#define TCP_UTIL_ERROR_HH

#include <stdexcept>
#include <cstring>

namespace tcp
{
	namespace util
	{
		struct tcp_exception;

		struct error_code
		{
			error_code(int err)
				: error(err) {}

			int get_errno() const
			{
				return error;
			}

			bool operator==(error_code const& e)
			{
				return error == e.error;
			}

			void invalidate()
			{
				error = -1;
			}

			void raise();

		private:
			int error;
		};

		struct tcp_exception : std::runtime_error
		{
			tcp_exception(error_code const& error)
				: std::runtime_error(strerror(error.get_errno())),
				  error(error) {}

			error_code get_error() const
			{
				return error;
			}

		private:
			error_code error;
		};
	}
}

#endif
