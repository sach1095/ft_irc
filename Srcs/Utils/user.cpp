 #include "../../Includes/lib.hpp"

user::user(int sd, sockaddr_in address): _sd(sd), _address(address)
{
}

user::~user()
{
}

std::string	user::getNick() const
{
	return _nick;
}

std::string	user::getLogin() const
{
	return _login;
}

std::string	user::getRealName() const
{
	return _realName;
}

std::string	user::getBuffer() const
{
	return _buffer;
}

int			user::getSd() const
{
	return _sd;
}

sockaddr_in	user::getAddress() const
{
	return _address;
}

void	user::setNick(std::string nick)
{
	_nick = nick;
}

void	user::setLogin(std::string login)
{
	_login = login;
}

void	user::setRealName(std::string realName)
{
	_realName = realName;
}

void	user::setBuffer(std::string buffer)
{
	_buffer = buffer;
}

void	user::cleanBuffer()
{
	_buffer.clear();
}

void	user::setSd(int sd)
{
	_sd = sd;
}

void	user::setAddr(sockaddr_in address)
{
	_address = address;
}

std::string	user::getIp() const
{
	std::string ip = inet_ntoa(_address.sin_addr);
	return ip;
}
