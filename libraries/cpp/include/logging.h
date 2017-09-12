#ifndef _LOGGING_H_
#define _LOGGING_H_
#include <string>
#include <iostream>
#include <vector>

extern std::ostream &operator<<(std::ostream &os, const std::vector<char> &msg);

extern std::string nowstr();
extern void log_print(std::ostream &os, const char *msg,
                      const char *filename, int lineno,
                      const char *function);
extern std::ostream &log_stream(std::ostream &os,
                                const char *filename, int lineno,
                                const char *function);


#define LOG_PRINT(os, msg) do															 \
		{																											 \
			log_print(os, msg, __FILE__, __LINE__, __func__);		 \
		} while (0)

#define LOG_STREAM(os) log_stream(os, __FILE__, __LINE__, __func__)

#define DEBUG_LOG       0x0001
#define DEBUG_SOCKET_IO 0x0002
#define DEBUG_ERROR_LOG 0x0004
#define DEBUG_GAME      0x0008

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL     (DEBUG_ERROR_LOG)
#endif

// If test is known at compile-time, this will compile away.
#define DEBUG_IF(test, outputstream)						\
	do																						\
		{																						\
			if(test)																	\
				{																				\
					outputstream;													\
				}																				\
		} while (0)


#define DBGP(level, outputstream)										\
	do																								\
		{																								\
			DEBUG_IF(DEBUG_LEVEL & level, outputstream);	\
		} while(0)

#define DBGP_E(outputstream)												\
	do																								\
		{																								\
			DBGP(DEBUG_ERROR_LOG, outputstream);					\
		} while(0)

#define DBGP_L(outputstream)												\
	do																								\
		{																								\
			DBGP(DEBUG_LOG, outputstream);								\
		} while(0)

#define DBGP_SOCKET(outputstream)										\
	do																								\
		{																								\
			DBGP(DEBUG_SOCKET_IO, outputstream);					\
		} while(0)

#define DBGP_GAME(outputstream)											\
	do																								\
		{																								\
			DBGP(DEBUG_GAME, outputstream);								\
		} while(0)


#endif /* _LOGGING_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
