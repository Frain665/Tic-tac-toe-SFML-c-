#include "app.h"
#include <iostream>
#include <cmath>

App::App()
    : m_currentPlayer(Player::X)
    , m_gameState(GameState::PLAYING)
    , m_gameResult(GameResult::CONTINUE)
{
    DEBUG_LOG("App constructor called");

    if (!m_font.openFromFile("resources/Fonts/DefaultFont/DefaultFont.ttf")) {
        DEBUG_LOG("Could not load default font");
        // Обработка ошибки
    }
}

App::~App() 
{
    DEBUG_LOG("App destructor called");
}

bool App::initialize() {
    DEBUG_LOG("Initializing application...");

    // Создание окна
    m_window.create(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
        "Tic-Tac-Toe",
        sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(60);

    if (!m_window.isOpen()) {
        DEBUG_LOG("Failed to create window");
        return false;
    }

    // Загрузка ресурсов
    if (!loadResources()) {
        DEBUG_LOG("Failed to load resources");
        return false;
    }

    // Инициализация игры
    initializeGame();

    DEBUG_LOG("Application initialized successfully");
    return true;
}

bool App::loadResources() {
    // Попытка загрузить шрифт из разных возможных путей
    std::vector<std::string> fontPaths = {
        "resources/Fonts/DefaultFont/DefaultFont.ttf",
        "resources/Fonts/DefaultFont/DefaultFont.ttf",
        "resources/Fonts/DefaultFont/DefaultFont.ttf"
    };

    bool fontLoaded = false;
    for (const auto& path : fontPaths) 
    {
        if (m_font.openFromFile(path)) 
        {
            fontLoaded = true;
            DEBUG_LOG("Font loaded from: " + path);
            break;
        }
    }

    // Если не удалось загрузить пользовательский шрифт, используем системный
    if (!fontLoaded) {
        DEBUG_LOG("Could not load custom font, trying default system font");
        // Для Windows можно попробовать Arial
#ifdef _WIN32

        if (!m_font.openFromFile("C:/Windows/Fonts/DefaultFont.ttf")) {  // SFML 3.0
            DEBUG_LOG("Could not load default font");
            return false;
        }
#else
    // Для Linux/Mac попробуем стандартные пути
        if (!m_font.loadFromFile("/System/Library/Fonts/DefaultFont.ttf") &&  // ИСПРАВЛЕНО
            !m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {  // ИСПРАВЛЕНО
            DEBUG_LOG("Could not load default font");
            return false;
        }
#endif
    }

    // Настройка текстов UI
    m_titleText.setFont(m_font);
    m_titleText.setString("TIC-TAC-TOE");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f - 150, 20));

    m_statusText.setFont(m_font);
    m_statusText.setCharacterSize(24);
    m_statusText.setFillColor(sf::Color::Yellow);
    m_statusText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f - 100, 90));

    m_restartText.setFont(m_font);
    m_restartText.setString("Press R to restart");
    m_restartText.setCharacterSize(18);
    m_restartText.setFillColor(sf::Color::Green);
    m_restartText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f - 80, WINDOW_HEIGHT - 50));

    return true;
}

void App::initializeGame() {
    DEBUG_LOG("Initializing game...");

    // Очистка доски
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            m_board[i][j] = Player::NONE;
        }
    }

    m_currentPlayer = Player::X;
    m_gameState = GameState::PLAYING;
    m_gameResult = GameResult::CONTINUE;
}

void App::resetGame() 
{
    DEBUG_LOG("Resetting game...");
    initializeGame();
}

void App::run() 
{
    DEBUG_LOG("Starting main game loop...");

    sf::Clock clock;

    while (m_window.isOpen() && m_gameState != GameState::EXIT) 
    {
        float deltaTime = clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }
}

void App::handleEvents()
{
    while (auto event = m_window.pollEvent())
    {
        if (const auto* closeEvent = event->getIf<sf::Event::Closed>())
        {
            m_window.close();
            m_gameState = GameState::EXIT;
        }
        else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                handleMouseClick(mouseEvent->position.x, mouseEvent->position.y);
            }
        }
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyEvent->code == sf::Keyboard::Key::R)
            {
                resetGame();
            }
            else if (keyEvent->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
                m_gameState = GameState::EXIT;
            }
        }
    }
}

void App::handleMouseClick(int mouseX, int mouseY)
{
    if (m_gameState != GameState::PLAYING)
    {
        return;
    }

    auto [row, col] = getGridPosition(mouseX, mouseY);

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        if (makeMove(row, col))
        {
            m_gameResult = checkGameResult();

            if (m_gameResult != GameResult::CONTINUE)
            {
                m_gameState = GameState::GAME_OVER;
                DEBUG_LOG("Game finished");
            }
            else
            {
                switchPlayer();
            }
        }
    }
}

std::pair<int, int> App::getGridPosition(int mouseX, int mouseY)
{
    int col = (mouseX - BOARD_OFFSET_X) / CELL_SIZE;
    int row = (mouseY - BOARD_OFFSET_Y) / CELL_SIZE;

    // Проверяем границы
    if (col < 0 || col >= BOARD_SIZE || row < 0 || row >= BOARD_SIZE)
    {
        return { -1, -1 };
    }

    return { row, col };
}

bool App::makeMove(int row, int col)
{
    if (!isValidMove(row, col))
    {
        return false;
    }

    m_board[row][col] = m_currentPlayer;
    DEBUG_LOG_FORMAT("Player %c made move at (%d, %d)",
        (m_currentPlayer == Player::X) ? 'X' : 'O', row, col);
    return true;
}

bool App::isValidMove(int row, int col)
{
    return row >= 0 && row < BOARD_SIZE &&
        col >= 0 && col < BOARD_SIZE &&
        m_board[row][col] == Player::NONE;
}

GameResult App::checkGameResult()
{
    // Проверка победы игрока X
    if (checkWinner(Player::X))
    {
        return GameResult::PLAYER_X_WINS;
    }

    // Проверка победы игрока O
    if (checkWinner(Player::O))
    {
        return GameResult::PLAYER_O_WINS;
    }

    // Проверка ничьей
    if (isBoardFull())
    {
        return GameResult::DRAW;
    }

    return GameResult::CONTINUE;
}

bool App::checkWinner(Player player)
{
    // Проверка строк
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        bool rowWin = true;
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (m_board[i][j] != player)
            {
                rowWin = false;
                break;
            }
        }
        if (rowWin) return true;
    }

    // Проверка столбцов
    for (int j = 0; j < BOARD_SIZE; ++j)
    {
        bool colWin = true;
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            if (m_board[i][j] != player)
            {
                colWin = false;
                break;
            }
        }
        if (colWin) return true;
    }

    // Проверка главной диагонали
    bool diag1Win = true;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        if (m_board[i][i] != player)
        {
            diag1Win = false;
            break;
        }
    }
    if (diag1Win) return true;

    // Проверка побочной диагонали
    bool diag2Win = true;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        if (m_board[i][BOARD_SIZE - 1 - i] != player)
        {
            diag2Win = false;
            break;
        }
    }

    return diag2Win;
}

bool App::isBoardFull()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (m_board[i][j] == Player::NONE)
            {
                return false;
            }
        }
    }
    return true;
}

void App::switchPlayer()
{
    m_currentPlayer = (m_currentPlayer == Player::X) ? Player::O : Player::X;
}

void App::update(float deltaTime)
{
    // Обновление статуса игры
    switch (m_gameResult)
    {
    case GameResult::CONTINUE:
        if (m_currentPlayer == Player::X)
        {
            m_statusText.setString("Player X's turn");
        }
        else
        {
            m_statusText.setString("Player O's turn");
        }
        m_statusText.setFillColor(sf::Color::Yellow);
        break;

    case GameResult::PLAYER_X_WINS:
        m_statusText.setString("Player X Wins!");
        m_statusText.setFillColor(sf::Color::Green);
        break;

    case GameResult::PLAYER_O_WINS:
        m_statusText.setString("Player O Wins!");
        m_statusText.setFillColor(sf::Color::Green);
        break;

    case GameResult::DRAW:
        m_statusText.setString("It's a Draw!");
        m_statusText.setFillColor(sf::Color::Cyan);
        break;
    }
}

void App::render()
{
    m_window.clear(sf::Color::Black);

    drawBoard();
    drawUI();

    m_window.display();
}

void App::drawBoard()
{
    // Отрисовка сетки
    sf::RectangleShape line;
    line.setFillColor(sf::Color::White);

    // Вертикальные линии
    for (int i = 1; i < BOARD_SIZE; ++i)
    {
        line.setSize(sf::Vector2f(4, CELL_SIZE * BOARD_SIZE));
        line.setPosition(sf::Vector2f(BOARD_OFFSET_X + i * CELL_SIZE - 2, BOARD_OFFSET_Y));
        m_window.draw(line);
    }

    // Горизонтальные линии
    for (int i = 1; i < BOARD_SIZE; ++i)
    {
        line.setSize(sf::Vector2f(CELL_SIZE * BOARD_SIZE, 4));
        line.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE - 2));
        m_window.draw(line);
    }

    // Отрисовка рамки доски
    sf::RectangleShape border;
    border.setSize(sf::Vector2f(CELL_SIZE * BOARD_SIZE + 4, CELL_SIZE * BOARD_SIZE + 4));
    border.setPosition(sf::Vector2f(BOARD_OFFSET_X - 2, BOARD_OFFSET_Y - 2));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::White);
    border.setOutlineThickness(2);
    m_window.draw(border);

    // Отрисовка X и O
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (m_board[i][j] == Player::X)
            {
                drawX(i, j);
            }
            else if (m_board[i][j] == Player::O)
            {
                drawO(i, j);
            }
        }
    }
}

void App::drawX(int row, int col)
{
    float x, y;
    getCellPosition(row, col, x, y);

    sf::RectangleShape line1, line2;
    float lineLength = CELL_SIZE * 0.6f;
    float thickness = 6.0f;

    // Первая диагональная линия (\)
    line1.setSize(sf::Vector2f(lineLength * std::sqrt(2), thickness));
    line1.setPosition(sf::Vector2f({ x + CELL_SIZE * 0.2f, y + CELL_SIZE * 0.2f }));
    line1.setRotation(sf::degrees(45));  // ИСПРАВЛЕНО: убрано sf::degrees
    line1.setFillColor(sf::Color::Red);

    // Вторая диагональная линия (/)
    line2.setSize(sf::Vector2f(lineLength * std::sqrt(2), thickness));
    line2.setPosition(sf::Vector2f({ x + CELL_SIZE * 0.8f, y + CELL_SIZE * 0.2f }));
    line2.setRotation(sf::degrees(35));  // ИСПРАВЛЕНО: убрано sf::degrees
    line2.setFillColor(sf::Color::Red);

    m_window.draw(line1);
    m_window.draw(line2);
}

void App::drawO(int row, int col)
{
    float x, y;
    getCellPosition(row, col, x, y);

    sf::CircleShape circle;
    float radius = CELL_SIZE * 0.3f;
    circle.setRadius(radius);
    circle.setPosition(sf::Vector2f(x + CELL_SIZE * 0.2f, y + CELL_SIZE * 0.2f));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Blue);
    circle.setOutlineThickness(6);

    m_window.draw(circle);
}

void App::getCellPosition(int row, int col, float& x, float& y)
{
    x = BOARD_OFFSET_X + col * CELL_SIZE;
    y = BOARD_OFFSET_Y + row * CELL_SIZE;
}

void App::drawUI()
{
    m_window.draw(m_titleText);
    m_window.draw(m_statusText);
    m_window.draw(m_restartText);

}

void App::shutdown()
{
    DEBUG_LOG("Shutting down application...");
    m_window.close();
}