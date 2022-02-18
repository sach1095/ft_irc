#include "../../Includes/lib.hpp"

void	delete_user(data<user *> data, user *cursor)
{
	std::vector<user *>::iterator it = data.users.begin();
	while (it != data.users.end())
	{
		if (*it == cursor)
			break ;
		it++;
	}
	delete cursor;
	data.users.erase(it);
}