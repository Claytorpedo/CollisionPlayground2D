#pragma once
#ifdef DEBUG

/* --------------- Set logging colours --------------- */
#ifdef NO_CONSOLE_COLOUR
#define ERR_COL  ""
#define WARN_COL ""
#define LOG_COL  ""
#define RST_COL  ""
#else
#define ERR_COL  "\033[31;1m"
#define WARN_COL "\033[33;1m"
#define LOG_COL  "\033[32;1m"
#define RST_COL  "\033[0m"
#endif
/* --------------------------------------------------- */

#include <iostream>
#define DBG_ERR(x)  do { std::cerr << ERR_COL  "ERROR:"   RST_COL " " << x << "\n"; } while (0)
#define DBG_WARN(x) do { std::cerr << WARN_COL "WARNING:" RST_COL " " << x << "\n"; } while (0)
#define DBG_LOG(x)  do { std::cout << LOG_COL  "LOG:"     RST_COL " " << x << "\n"; } while (0)

#else
#define DBG_ERR(x)  do {} while(0)
#define DBG_WARN(x) do {} while(0)
#define DBG_LOG(x)  do {} while(0)
#endif // DEBUG
