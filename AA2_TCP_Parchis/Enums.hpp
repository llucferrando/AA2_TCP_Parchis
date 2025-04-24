#pragma once

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

enum class FichaState {
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