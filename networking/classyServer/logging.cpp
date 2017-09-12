#include "logging.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <cerrno>
#include <cstring>

std::ostream &operator<<(std::ostream &os, const std::vector<char> &msg)
{
	size_t i;
	for(i = 0; i < msg.size(); i++)
		{
			os << msg[i];
		}
	return os;
}


std::string nowstr()
{
	time_t rawtime;
  struct tm * timeinfo;
	std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);

	char timestr[128];
	strftime(timestr, 128, "%F %T", timeinfo);
	return std::string(timestr);
}

void log_print(std::ostream &os, const char *msg,
							 const char *filename, int lineno,
							 const char *function)
{
  log_stream(os, filename, lineno, function) << msg << std::endl;
}

std::ostream &log_stream(std::ostream &os,
                         const char *filename, int lineno,
                         const char *function)
{
	std::string now = nowstr();
	if(errno)
		{
			os << now << " :: "
				 << filename << ":" << lineno << " "
				 << function << " :: "
				 << "[errno=" << errno << "]"
				 << std::strerror(errno)
				 << std::endl;
			errno = 0;
		}
	os << now << " :: "
		 << filename << ":" << lineno << " "
		 << function << " :: ";
  return os;
}
