#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <array>
#include "debugger.h"

// ������������ ��� ��������� ����
enum class GameState {
	MENU,
	PLAYING,
	GAME_OVER,
	EXIT
};

// ������������ ��� �������
enum class Player {
	NONE = 0,
	X = 1,
	O = 2
};

// ������������ ��� ���������� ����
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

	// �������� ������ ����������
	bool initialize();
	void run();
	void shutdown();

private:
	// ���������
	static const int WINDOW_WIDTH = 600;
	static const int WINDOW_HEIGHT = 700;
	static const int BOARD_SIZE = 3;
	static const int CELL_SIZE = 150;
	static const int BOARD_OFFSET_X = 75;
	static const int BOARD_OFFSET_Y = 150;

	// SFML �������
	sf::RenderWindow m_window;
	sf::Font m_font;
	sf::Text m_titleText;
	sf::Text m_statusText;
	sf::Text m_restartText;

	// ������� ������
	std::array<std::array<Player, BOARD_SIZE>, BOARD_SIZE> m_board;
	Player m_currentPlayer;
	GameState m_gameState;
	GameResult m_gameResult;

	// ������ ��������� �������
	void handleEvents();
	void handleMouseClick(int mouseX, int mouseY);

	// ������ ���������� � ���������
	void update(float deltaTime);
	void render();

	// ������ ���������
	void drawBoard();
	void drawCell(int row, int col);
	void drawX(int row, int col);
	void drawO(int row, int col);
	void drawUI();

	// ������� ������
	void initializeGame();
	void resetGame();
	bool makeMove(int row, int col);
	GameResult checkGameResult();
	bool checkWinner(Player player);
	bool isBoardFull();
	void switchPlayer();

	// ��������������� ������
	bool loadResources();
	void getCellPosition(int row, int col, float& x, float& y);
	bool isValidMove(int row, int col);
	std::pair<int, int> getGridPosition(int mouseX, int mouseY);
};