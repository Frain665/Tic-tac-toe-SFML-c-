#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <array>
#include "debugger.h"

// Перечисление для состояния игры
enum class GameState {
	MENU,
	PLAYING,
	GAME_OVER,
	EXIT
};

// Перечисление для игроков
enum class Player {
	NONE = 0,
	X = 1,
	O = 2
};

// Перечисление для результата игры
enum class GameResult {
	CONTINUE,
	PLAYER_X_WINS,
	PLAYER_O_WINS,
	DRAW
};

class App {
public:
	App();
	~App();

	// Основные методы приложения
	bool initialize();
	void run();
	void shutdown();

private:
	// Константы
	static const int WINDOW_WIDTH = 600;
	static const int WINDOW_HEIGHT = 700;
	static const int BOARD_SIZE = 3;
	static const int CELL_SIZE = 150;
	static const int BOARD_OFFSET_X = 75;
	static const int BOARD_OFFSET_Y = 150;

	// SFML объекты
	sf::RenderWindow m_window;
	sf::Font m_font;
	sf::Text m_titleText;
	sf::Text m_statusText;
	sf::Text m_restartText;

	// Игровая логика
	std::array<std::array<Player, BOARD_SIZE>, BOARD_SIZE> m_board;
	Player m_currentPlayer;
	GameState m_gameState;
	GameResult m_gameResult;

	// Методы обработки событий
	void handleEvents();
	void handleMouseClick(int mouseX, int mouseY);

	// Методы обновления и отрисовки
	void update(float deltaTime);
	void render();

	// Методы отрисовки
	void drawBoard();
	void drawCell(int row, int col);
	void drawX(int row, int col);
	void drawO(int row, int col);
	void drawUI();

	// Игровая логика
	void initializeGame();
	void resetGame();
	bool makeMove(int row, int col);
	GameResult checkGameResult();
	bool checkWinner(Player player);
	bool isBoardFull();
	void switchPlayer();

	// Вспомогательные методы
	bool loadResources();
	void getCellPosition(int row, int col, float& x, float& y);
	bool isValidMove(int row, int col);
	std::pair<int, int> getGridPosition(int mouseX, int mouseY);
};