#include "GameManager.h"
#include "DataBaseManager.h"

int main()
{
	DataBaseManager* myDB = new DataBaseManager();
	GameManager* gm = new GameManager();

	myDB->ConnectDatabase();

	gm->Init();
	gm->Run();
		
	
}