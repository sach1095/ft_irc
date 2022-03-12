#pragma once

# define SUCCESS 0
# define FAIL 1

# include <iostream>
# include <memory>
# include <algorithm>
# include <array>
# include <iterator>
# include <ios>
# include <istream>
# include <iomanip>
# include <cstring>
# include <sstream>
# include <string>
# include <cctype>
# include <cstdlib>
# include <vector>

# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>

# include <unistd.h>
# include <fcntl.h>

#include "bot.hpp"

void	rep_1(Bot &bot);
void	rep_2(Bot &bot);
void	rep_3(Bot &bot);
void	rep_4(Bot &bot);
void	rep_5(Bot &bot);
void	rep_6(Bot &bot);
void	rep_7(Bot &bot);
void	rep_8(Bot &bot);

bool	quest_1(std::string buf);
bool	quest_2(std::string buf);
bool	quest_3(std::string buf);
bool	quest_4(std::string buf);
bool	quest_5(std::string buf);
bool	quest_6(std::string buf);
bool	quest_7(std::string buf);
bool	quest_8(Bot &bot, std::string buf);
