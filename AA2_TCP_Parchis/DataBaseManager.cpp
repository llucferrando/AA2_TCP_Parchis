#include "DataBaseManager.h"

#define SERVER "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "12345"

DataBaseManager::DataBaseManager()
{
	_driver = nullptr;
	_con = nullptr;
}

DataBaseManager::~DataBaseManager()
{
	if (_con) {
		_con->close();
		delete _con;
	}
}

void DataBaseManager::ConnectDatabase()
{
	sql::Driver* driver;
	sql::Connection* con;

	try {
		driver = get_driver_instance();
		con = driver->connect(SERVER, USERNAME, PASSWORD);
		std::cout << "Connection done" << std::endl;

		con->close();
		std::cout << "Connection close" << std::endl;
	}
	catch (sql::SQLException e) {
		std::cout << "Couldn't connect to server" << e.what() << std::endl;
		system("pause");
		exit(1);

	}
	delete con;
}

bool DataBaseManager::RegisterUser(const std::string& nickname, const std::string& password)
{
	return false;
}

bool DataBaseManager::LoginUser(const std::string& nickname, const std::string& password)
{
	return false;
}
