#include <iostream>

#define ANSI_TXT_BLK "\033[0;30m"
#define ANSI_TXT_RED "\033[0;31m"
#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_YLW "\033[0;33m"
#define ANSI_TXT_BLU "\033[0;34m"
#define ANSI_TXT_MGT "\033[0;35m"
#define ANSI_TXT_CYA "\033[0;36m"
#define ANSI_TXT_WHI "\033[0;37m"
#define ANSI_TXT_DFT "\033[0;0m"    // Console default
#define GTEST_BOX "[     cout ] "
#define COUT_GTEST std::cout << ANSI_TXT_GRN << GTEST_BOX << ANSI_TXT_DFT
#define COUT std::cout
#define ENDL ANSI_TXT_DFT << '\n'