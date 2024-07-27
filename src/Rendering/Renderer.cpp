#include "Rendering/Renderer.hpp"
#include "Rendering/Raycaster.hpp"

#include <iostream>

namespace Rendering
{
    Renderer::Renderer()
        : map(nullptr), screenWidth(0), screenHeight(0)
    {}

    Renderer::~Renderer()
    {}

    sf::Vector2f Renderer::mathToSFML(const Math::Vector2f& vec) const
    {
        return sf::Vector2f(vec.getX(), vec.getY());    // Convert Math::Vector2f to sf::Vector2f
    }

    sf::Vector2f Renderer::gridToWindow(const Math::Vector2f& vec) const
    {
        static int cellWidth = screenWidth / map->getWidth();
        static int cellHeight = screenHeight / map->getHeight();
        return sf::Vector2f(vec.getX() * cellWidth, vec.getY() * cellHeight);    // Convert Math::Vector2f to sf::Vector2f
    }

    void Renderer::drawLine(sf::Vector2f initialPos, sf::Vector2f endingPos, sf::Color color)
    {
        sf::VertexArray line(sf::Lines, 2);

        line[0].position = initialPos;
        line[0].color = color; // Start point color
        line[1].position = endingPos;
        line[1].color = color; // End point color

        window.draw(line);
    }

    void Renderer::renderTopView()
    {
        // Render map grid
        int cellWidth = screenWidth / map->getWidth();
        int cellHeight = screenHeight / map->getHeight();

        for(int i = 0; i < map->getHeight(); i++)
        {
            for(int j = 0; j < map->getWidth(); j++)
            {
                sf::RectangleShape rect(sf::Vector2f(cellWidth - CELL_SEPARATOR_THICKNESS, cellHeight - CELL_SEPARATOR_THICKNESS));
                rect.setPosition(j * (cellWidth), i * (cellHeight));
                if(map->getTile(j, i) == ' ')
                    rect.setFillColor(sf::Color::Yellow);
                else
                    rect.setFillColor(sf::Color::Blue);
                window.draw(rect);
            }
        }
    }

    void Renderer::renderPlayerBall()
    {
        sf::CircleShape ball(5.0f);
        ball.setOrigin(5.0f, 5.0f);
        ball.setFillColor(sf::Color::Red);
        ball.setPosition(gridToWindow(player->getPosition()));
        window.draw(ball);
    }

    void Renderer::castRaysTopView()
    {
        // Conversion from math vector class to sfml 
        sf::Vector2f playerPos = gridToWindow(player->getPosition());
        player->normalizeAngle();
        Math::Angle ang = player->getAngle();

        ang -= fov / 2.0f;

        for(int i = 0; i < nRays; ++i)
        {
            ang.normalize();

            Math::Vector2f snappedPoint = Raycaster::castedRayHitPoint(player->getPosition(), ang, map);
            sf::Vector2f snappedPointSFML = gridToWindow(snappedPoint);

            // Draw line from player to hit point
            drawLine(playerPos, snappedPointSFML, sf::Color::Black);
        
            ang += fov / (nRays - 1);
        }
    }

    // Draws a column of pixels
    void Renderer::drawPixelColumn(int x, float dist/*, sf::Color color*/)
    {
        float height = screenHeight / dist;
        sf::RectangleShape rect(sf::Vector2f(screenWidth/nRays, height));
        rect.setFillColor(sf::Color(0, 255, 0, (dist > 1.0f) ? 255 / dist : 255));
        rect.setPosition(sf::Vector2f(x, (screenHeight - height) / 2));
        window.draw(rect);
    }

    // Draws columns of pixels taking into account the distance of the wall
    void Renderer::render3d()
    {
        // Conversions from math vector class to sfml 
        sf::Vector2f playerPos = gridToWindow(player->getPosition());
        player->normalizeAngle();
        Math::Angle ang = player->getAngle();

        ang -= fov / 2.0f;

        for(int i = 0; i < nRays; ++i)
        {
            ang.normalize();

            //float dist = Raycaster::castedRayDist(player->getPosition(), ang, map) * cos((player->getAngle() - ang).get());
            drawPixelColumn(i * screenWidth / nRays, Raycaster::castedRayDist(player->getPosition(), ang, map));
        
            ang += fov / (nRays - 1);
        } 
    }

    Renderer& Renderer::getInstance()
    {
        static Renderer instance;
        return instance;
    }

    void Renderer::init(Game::Map* mp, Game::Player* pl, int width, int height, const char* title, float fieldOfView, int raysN)
    {
        map = mp;
        player = pl;
        screenWidth = width;
        screenHeight = height;
        fov = fieldOfView;
        nRays = raysN;

        window.create(sf::VideoMode(screenWidth, screenHeight), title);
        window.setFramerateLimit(60);
    }

    void Renderer::render()
    {
        window.clear();

        /* 3D CODE */
        // render3d();
        
        /* TOPVIEW CODE */
        renderTopView();
        castRaysTopView();
        renderPlayerBall();

        window.display();
    }

    void Renderer::pollEvent()
    {
        sf::Event event;
        while(window.pollEvent(event))
            if(event.type == sf::Event::Closed)
                window.close();
        
        player->handleInput();
    }

    bool Renderer::isOpen() const
    {
        return window.isOpen();
    }

    void Renderer::setFPSlimit(unsigned int fps)
    {
        window.setFramerateLimit(fps);
    }
}