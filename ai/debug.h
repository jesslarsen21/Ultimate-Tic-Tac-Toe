#include <iostream>

#define ShowMessages 1

#if ShowMessages
#define MSG(X) std::cout << X << std::endl;
#else
#define MSG(X);
#endif