#pragma once

#include <SFML/Graphics.hpp>
#include "debugger.h"

class MainWindow 
{
public:
    MainWindow();
    ~MainWindow();

    // �������� ������ ����
    bool create(int width, int height, const std::string& title);
    void close();
    bool isOpen() const;

    // ������ ��������� �������
    std::optional<sf::Event> pollEvent();

    // ������ ���������
    void clear(const sf::Color& color = sf::Color::Black);
    void display();

    // ������ ��������� ����������
    void draw(const sf::Drawable& drawable);
    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end,
        const sf::Color& color = sf::Color::White, float thickness = 2.0f);
    void drawRectangle(const sf::Vector2f& position, const sf::Vector2f& size,
        const sf::Color& fillColor = sf::Color::Transparent,
        const sf::Color& outlineColor = sf::Color::White,
        float outlineThickness = 1.0f);
    void drawCircle(const sf::Vector2f& center, float radius,
        const sf::Color& fillColor = sf::Color::Transparent,
        const sf::Color& outlineColor = sf::Color::White,
        float outlineThickness = 1.0f);

    // ��������� �������� ����
    sf::Vector2u getSize() const;

    // ��������� ������� ����
    sf::Vector2i getMousePosition() const;

private:
    sf::RenderWindow m_window;
    bool m_isOpen;
};