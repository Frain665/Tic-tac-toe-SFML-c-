#include "main_window.h"

MainWindow::MainWindow() : m_isOpen(false) {
    DEBUG_LOG("MainWindow constructor called");
}

MainWindow::~MainWindow() {
    if (m_isOpen) {
        close();
    }
    DEBUG_LOG("MainWindow destructor called");
}

bool MainWindow::create(int width, int height, const std::string& title) 
{
    DEBUG_LOG("Creating window: " + title);

    m_window.create(sf::VideoMode(sf::Vector2u(width, height)), title,
        sf::Style::Titlebar | sf::Style::Close);

    if (!m_window.isOpen()) {
        DEBUG_LOG("Failed to create window");
        return false;
    }

    m_window.setFramerateLimit(60);
    m_isOpen = true;

    DEBUG_LOG("Window created successfully");
    return true;
}

void MainWindow::close() {
    if (m_isOpen) {
        DEBUG_LOG("Closing window");
        m_window.close();
        m_isOpen = false;
    }
}

bool MainWindow::isOpen() const {
    return m_window.isOpen() && m_isOpen;
}

std::optional<sf::Event> MainWindow::pollEvent()
{
    return m_window.pollEvent();
}

void MainWindow::clear(const sf::Color& color) {
    m_window.clear(color);
}

void MainWindow::display() {
    m_window.display();
}

void MainWindow::draw(const sf::Drawable& drawable) {
    m_window.draw(drawable);
}

void MainWindow::drawLine(const sf::Vector2f& start, const sf::Vector2f& end,
    const sf::Color& color, float thickness) {
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length == 0) return;

    sf::RectangleShape line;
    line.setSize(sf::Vector2f(length, thickness));
    line.setPosition(start);
    line.setFillColor(color);
    
    // Вычисление угла поворота
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;
    line.setRotation(sf::degrees(angle));

    m_window.draw(line);
}

void MainWindow::drawRectangle(const sf::Vector2f& position, const sf::Vector2f& size,
    const sf::Color& fillColor,
    const sf::Color& outlineColor,
    float outlineThickness) {
    sf::RectangleShape rectangle;
    rectangle.setPosition(position);
    rectangle.setSize(size);
    rectangle.setFillColor(fillColor);
    rectangle.setOutlineColor(outlineColor);
    rectangle.setOutlineThickness(outlineThickness);

    m_window.draw(rectangle);
}

void MainWindow::drawCircle(const sf::Vector2f& center, float radius,
    const sf::Color& fillColor,
    const sf::Color& outlineColor,
    float outlineThickness) {
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setPosition(sf::Vector2f(center.x - radius, center.y - radius));
    circle.setFillColor(fillColor);
    circle.setOutlineColor(outlineColor);
    circle.setOutlineThickness(outlineThickness);

    m_window.draw(circle);
}