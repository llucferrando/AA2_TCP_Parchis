#pragma once

enum class GameState {
	SplashScreen,
	LoginMenu,
	MatchmakingMenu,
	Gameplay
};

enum class ParchisState {
	CLIENT,
	GAME_SERVER,
	BOOTSTRAP
};

enum class BootstrapMessageType {
	LIST_GAMES,
	REGISTER_GAME,
	UNREGISTER_GAME
};

enum class MessageType {
	ROLL_DICE,
	MOVE_REQUEST,
	MOVE_CONFIRMED,
	DICE_RESULT,
	TURN_CHANGE,
	GAME_OVER,
	FICHA_DELETED
};

enum class TokenState {
	IN_HOME,
	IN_GAME,
	IN_END
};

enum class PlayerColor
{
	RED,
	BLUE,
	GREEN, 
	YELLOW
};