#include "GameManager.h"
#include "DataBaseManager.h"
#include "Server.h"
#include "Enums.hpp"
#include "MenuManager.h"


int main(int argc, char* argv[])
{
	ParchisState state = ParchisState::GAME_SERVER;
		GameManager* gm = new GameManager();
		MenuManager* mM = new MenuManager();
		
	//Server server;

	switch (state)
	{
	case ParchisState::CLIENT:
		//std::cout << "Iniciando cliente..." << std::endl;
		//Client client("127.0.0.1", 50000); // bootstrap IP y puerto
		//client.Run();
		break;
	case ParchisState::GAME_SERVER:
		//std::cout << "Iniciando Game Server en puerto 55000 y registrando en bootstrap..." << std::endl;
		//GameServer server(55000, "127.0.0.1", 50000); // puerto local y bootstrap IP:puerto
		mM->Init();
		mM->Render();
		gm->Init();
		gm->Run();
		break;
	case ParchisState::BOOTSTRAP:
		//std::cout << "Iniciando Bootstrap Server en puerto 50000..." << std::endl;
		//BootstrapServer server(50000);
		//server.Run();
		break;
	default:
		std::cout << "Modo inválido. Usa: bootstrap | gameserver | client" << std::endl;
		break;
	}
	return 0;
}