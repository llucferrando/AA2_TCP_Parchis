#include "DataBaseManager.h"
#include "bcrypt.h"

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

	try {
		_driver = get_driver_instance();
		_con = _driver->connect(SERVER, USERNAME, PASSWORD);
		_con->setSchema("videogame");
		std::cout << "Connection with database done" << std::endl;

	}
	catch (sql::SQLException e) {
		std::cerr << "[DB] Error al conectar: " << e.what()
			<< " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << ")" << std::endl;

	}
}

bool DataBaseManager::RegisterUser(const std::string& nickname, const std::string& password)
{
	//We check for empty fields
	if (nickname.empty() || password.empty()) {
		std::cerr << "Nickname or password empty on field \n";
		return false;
	}

	try {
		std::string query = "INSERT INTO users(nickname, password) VALUES (?,?)";
		std::string hashedPassword = bcrypt::generateHash(password); //Generate a hashed future password
		std::unique_ptr<sql::PreparedStatement> stmt(_con->prepareStatement(query)); //We can avoid to free memory

		stmt->setString(1, nickname);
		stmt->setString(2, hashedPassword);
		stmt->execute();

		

		std::cout << "User registered correctly" << std::endl;
		return true;
	}
	catch(sql::SQLException& e){
		std::cerr<<"Error while trying to register user" << e.what() << std::endl;
		return false;
	}
	
}

bool DataBaseManager::LoginUser(const std::string& nickname, const std::string& password)
{
	//We check for empty fields
	if (nickname.empty() || password.empty()) {
		std::cerr << "Nickname or password empty on field \n";
		return false;
	}
	try {
		std::string query = "SELECT password FROM users WHERE nickname = ?"; 
		std::unique_ptr<sql::PreparedStatement> stmt(_con->prepareStatement(query)); //We can avoid to free memory
		stmt->setString(1, nickname);
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

		if (res->next()) {
			std::string storedHash = res->getString("password"); 
			if (bcrypt::validatePassword(password, storedHash)) { 
				std::cout << "Correct login for user: " << nickname << std::endl;
				return true;
			}
		}

		std::cout << "Login failed for user: " << nickname << std::endl;
		return false;
	}
	catch (sql::SQLException& e) {
		std::cerr << "Login error" << e.what() << std::endl;
		return false;
	}
}
