#ifndef __FILE_NO_H__
#define __FILE_NO_H__

/* Thanks to: http://www.ginac.de/~kreckel/fileno/ */

#include <iosfwd>

template <typename charT, typename traits>
int fileno(const std::basic_ios<charT, traits>& stream);

#endif /* __FILE_NO_H__ */
