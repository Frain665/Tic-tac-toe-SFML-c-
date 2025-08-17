#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"

enum class STYLE_APPLICATION
{
	DARK,
	LIGHT,
	MODERN,

	DEFAULT
};

enum class CellState
{
	EMPTY,

	X,
	O
};

enum class GameState
{
	PLAYING,

	X_WON,
	O_WON,

	DRAW
};

class MainWindow
{
public:
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(MainWindow&) = delete;
	MainWindow(MainWindow&&) = delete;
	
	static MainWindow& GetInstance();

	void Run();

private:
	MainWindow();
	MainWindow() = default;

	void InitSystemVariables();    // Инициализация системных переменных
	void InitWindow();             // Создание окна
	void InitInterface();          // Инициализация интерфейса
	void InitGame();

	// === ОСНОВНЫЕ МЕТОДЫ ИГРОВОГО ЦИКЛА === //
	void HandleEvents();
	void Update(float time);
	void Render();
	
	// == МЕТОДЫ ОТРИСОВКИ == //
	void DrawBackGround();
	void DrawGridPanel(sf::Vector2f gridSize);
	void DrawGrid();
	void DrawCells();
	void DrawUI();
	void DrawGameStatus();

	// == ИГРОВАЯ ЛОГИКА == //
	void HandleCellClick(int x, int y);
	bool isValidMove(int x, int y) const;
	void MakeMove(int x, int y);
	void CheckWinCondition();
	void ResetGame();
	void SwitchPlayer();

	// == ВСПОМОГАТЕЛЬНЫЕ == //
	sf::Vector2i GetCellFromMousePos(sf::Vector2i mousePos) const;
	sf::Vector2f GetCellCenter(int x, int y) const;
	sf::FloatRect GetCellBounds(int x, int y) const;
	
private:
	// === ОСНОВНЫЕ ОБЪЕКТЫ === //
	std::unique_ptr<sf::RenderWindow> window_;
	sf::Clock clock_;
	sf::Font font_;

	// === ОСНОВНЫЕ ОБЪЕКТЫ === //
	std::array<std::array<CellState, 3>, 3> gameGrid_;
	GameState currentGameState_;
	CellState currentPlayer_;

	// === НАСТРОЙКИ ИГРОВОГО ПОЛЯ === //
	sf::Vector2f gridPosition_;
	sf::Vector2f gridSize_;
	
	float cellSize_;
	float lineThickness_;

	// === ВИЗУАЛЬНЫЕ ЭЛЕМЕНТЫ === //

	sf::RectangleShape backGroundPanel_;
	sf::Text statusText_;
	sf::Text instructionText_;

	// === НАСТРОЙКИ ОКНА === //
	static constexpr int WINDOW_HEIGHT = 600;
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int GRID_SIZE = 3;

	// === СТИЛИ И ЦВЕТА === //
	STYLE_APPLICATION currentStyle_;

	sf::Color backGroundColor_;
	sf::Color gridColor_;
	sf::Color xColor_;
	sf::Color oColor_;
	sf::Color textColor_;

};

#endif // !MAIN_WINDOW_H