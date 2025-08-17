#include "main_window.h"

MainWindow::MainWindow()
{
	InitSystemVariables();
	InitWindow();
	InitInterface();
	InitGame();
}

void MainWindow::InitSystemVariables()
{
	currentStyle_	=	STYLE_APPLICATION::MODERN;
	currentGameState_ = GameState::PLAYING;
	currentPlayer_ =	CellState::X;

	cellSize_ =		 120.0f;
	lineThickness_ = 4.0f;
	gridSize_ = sf::Vector2f(cellSize_ * 3, cellSize_ * 3);

	gridPosition_ = sf::Vector2f(
		(WINDOW_HEIGHT - gridSize_.y) / 2.0f - 50.0f,
		(WINDOW_WIDTH - gridSize_.x) / 2.0f
	);

	backGroundColor_ = sf::Color(45, 45, 45);
	gridColor_ = sf::Color(200, 200, 200);
	xColor_ = sf::Color(255, 100, 100);
	oColor_ = sf::Color(100, 150, 255);
	textColor_ = sf::Color(255, 255, 255);

	std::cout << "[INFO] Системные переменные инициализированы" << std::endl;
}

void MainWindow::InitWindow()
{
	window_ = std::make_unique<sf::RenderWindow>(
		sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT),
		"Tic-Tac-Toe Game",
		sf::Style::Close | sf::Style::Titlebar
	);
	
	window_->setFramerateLimit(60);

	std::cout << "[INFO] Окно создано: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
}

MainWindow& MainWindow::GetInstance()
{
	static MainWindow instance;
	return instance;
}

void MainWindow::Run()
{
	std::cout << "[INFO] Запуск главного цикла игры..." << std::endl;

	while (window_->isOpen())
	{
		float deltaTime = clock_.restart().asSeconds();

		HandleEvent();
		Update(deltaTime);
		Render();
	}

	std::cout << "[INFO] Игра завершена" << std::endl;
}

void MainWindow::InitInterface()
{
	if (!font_.openFromFile("resources/Fonts/DefaultFont/DefaultFont.ttf"))
	{
		std::cout << "[WARNING] Не удалось загрузить шрифт arial.ttf, используется стандартный" << std::endl;
	}

	backGroundPanel_.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	backGroundPanel_.setFillColor(backGroundColor_);
	backGroundPanel_.setPosition(sf::Vector2f(0, 0));

	statusText_.setFont(font_);
	statusText_.setCharacterSize(32);
	statusText_.setFillColor(sf::Color(180, 180, 180));
	statusText_.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 100));
	instructionText_.setString("Кликните по клетке для хода\nR - новая игра, ESC - выход");

	instructionText_.setFont(font_);
	instructionText_.setCharacterSize(16);
	instructionText_.setFillColor(sf::Color(180, 180, 180));
	instructionText_.setPosition(sf::Vector2f(50, WINDOW_HEIGHT - 100));
	instructionText_.setString("Кликните по клетке для хода\nR - новая игра, ESC - выход");

	std::cout << "[INFO] Интерфейс инициализирован" << std::endl;

}

void MainWindow::InitGame()
{
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			gameGrid_[i][j] = CellState::EMPTY;
		}
	}

	currentGameState_ = GameState::PLAYING;
	currentPlayer_ = CellState::X;

	std::cout << "[INFO] Игра инициализирована, начинает игрок X" << std::endl;
}

void MainWindow::DrawBackGround()
{
}

void MainWindow::DrawGridPanel(sf::Vector2f gridSize)
{
}

void MainWindow::DrawGrid()
{
}

void MainWindow::DrawCells()
{
}

void MainWindow::DrawUI()
{
}

void MainWindow::DrawGameStatus()
{
}

void MainWindow::HandleCellClick(int x, int y)
{
}

bool MainWindow::isValidMove(int x, int y) const
{
	return false;
}

void MainWindow::MakeMove(int x, int y)
{
}

void MainWindow::CheckWinCondition()
{
}

void MainWindow::ResetGame()
{
}

void MainWindow::SwitchPlayer()
{
}

sf::Vector2i MainWindow::GetCellFromMousePos(sf::Vector2i mousePos) const
{
	return sf::Vector2i();
}

sf::Vector2f MainWindow::GetCellCenter(int x, int y) const
{
	return sf::Vector2f();
}

sf::FloatRect MainWindow::GetCellBounds(int x, int y) const
{
	return sf::FloatRect();
}

void MainWindow::DrawGridPanel(sf::Vector2f gridSize)
{
}


void MainWindow::HandleEvents()
{
	sf::Event event;
	
	while (window_->pollEvent(event))
	{
		switch (event.type)
		{
		case: 


		default:
			break;
		}
	}
}

void MainWindow::Update(float time)
{
	
}

void MainWindow::Render()
{
}
