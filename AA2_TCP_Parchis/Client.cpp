#include "Client.h"

Client::Client(sf::TcpSocket* socket)
{
	std::ostringstream ss;
	ss << "C" << std::rand();
	_guid = ss.str();
}

sf::TcpSocket* Client::GetSocket()
{
	return _socket;
}

std::string Client::GetGUID() const
{
	return _guid;
}

std::string Client::GetNickname() const
{
	return _nickname;
}

void Client::SetNickname(const std::string& nickname)
{
	_nickname = nickname;
}
