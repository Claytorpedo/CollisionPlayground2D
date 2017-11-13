#pragma once
#ifdef DEBUG
#include <iostream>
#define DBG_ERR(x)  do { std::cerr << "\033[31;1mERROR:\033[0m "   << x << "\n"; } while (0)
#define DBG_WARN(x) do { std::cerr << "\033[33;1mWARNING:\033[0m " << x << "\n"; } while (0)
#define DBG_LOG(x)  do { std::cout << "\033[32;1mLOG:\033[0m "     << x << "\n"; } while (0)
#else
#define DBG_ERR(x)  do {} while(0)
#define DBG_WARN(x) do {} while(0)
#define DBG_LOG(x)  do {} while(0)
#endif // DEBUG
