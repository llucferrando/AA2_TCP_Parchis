#pragma once

enum class GameState {
	SplashScreen,
	LoginMenu,
	MatchmakingMenu,
	Gameplay,
	GameOver,
	None
};

enum class MessageType 
{
	PLAYER_PROFILE,
	ROLL_DICE,
	MOVE_REQUEST,
	DICE_RESULT,
	TURN_CHANGE,
	GAME_OVER,
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