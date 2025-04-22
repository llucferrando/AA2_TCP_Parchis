#include "GameManager.h"
#include "DataBaseManager.h"
#include "Server.h"

int main()
{
	Server server;
	GameManager* gm = new GameManager();

	//server.Run();
	gm->Init();
	gm->Run();
		
	
}