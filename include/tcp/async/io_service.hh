#ifndef SERVICE_HH
#define SERVICE_HH

namespace tcp
{
	namespace async
	{
		struct io_service
		{
			void start();
			void stop();
		};
	}
}

#endif
