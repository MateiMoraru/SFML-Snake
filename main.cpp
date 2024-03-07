#include <iostream>
#include <SFML/Graphics.hpp>
#include <deque>
#define WIDTH 810
#define HEIGHT 600

/*
 * TODO: Check whether the snake's head collides with the snake's body;
 * TODO: Check whether the apple spawned within the snake;
 */

class Snake
{
    private:
        std::deque<sf::Vector2f> snake;
        float size = 30.0;
        int score = 0;
        char direction = 'u';
    public:
        Snake()
        {
            make_snake();
        };

        int get_score()
        {
            return score;
        }

        float get_size()
        {
            return size;
        }

        void make_snake()
        {
            for(int i = 0; i < 3; i++)
            {
                snake.push_back(sf::Vector2f(size * 2, size * 2 + size * i));
            }
        }

        void draw(sf::RenderWindow &window)
        {
            sf::RectangleShape rectangle(sf::Vector2f(size - 2, size - 2));
            for(auto pos : snake)
            {
                rectangle.setPosition(pos.x + 1, pos.y + 1);
                window.draw(rectangle);
            }
        }
        
        void move()
        {
            sf::Vector2f head = snake.front();
            sf::Vector2f new_pos;
            switch(direction)
            {
                case 'u':
                    new_pos = sf::Vector2f(head.x, head.y - size);
                    if(new_pos.y < 0)
                        new_pos.y = HEIGHT - size;
                    snake.push_front(new_pos);
                    snake.pop_back();
                    break;
                case 'd':
                    new_pos = sf::Vector2f(head.x, head.y + size);
                    if(new_pos.y > HEIGHT)
                        new_pos.y = 0;
                    snake.push_front(new_pos);
                    snake.pop_back();
                    break;
                case 'l':
                    new_pos = sf::Vector2f(head.x - size, head.y);
                    if(new_pos.x < 0)
                        new_pos.x = WIDTH - size;
                    snake.push_front(new_pos);
                    snake.pop_back();
                    break;
                case 'r':
                    new_pos = sf::Vector2f(head.x + size, head.y);
                    if(new_pos.x > WIDTH)
                        new_pos.x = 0;
                    snake.push_front(new_pos);
                    snake.pop_back();
                    break;
            }

        }

        void set_direction(char dir)
        {
            if(direction == 'l' && dir == 'r' ||
               direction == 'r' && dir == 'l' ||
               direction == 'u' && dir == 'd' ||
               direction == 'd' && dir == 'u')
                return;
            direction = dir;
        }

        bool check_eat_apple(sf::Vector2f apple_pos)
        {
            sf::Vector2f pos = snake.front();
            bool collide = apple_pos.x >= pos.x && 
               apple_pos.x <= pos.x + size &&
               apple_pos.y >= pos.y &&
               apple_pos.y <= pos.y + size;
            return collide;
        }

        void increase_score()
        {
            ++score;
            sf::Vector2f new_pos = snake.back();
            switch(direction)
            {
                case 'u':
                    new_pos.y += size;
                    break;
                case 'd':
                    new_pos.y -= size;
                    break;
                case 'l':
                    new_pos.x += size;
                    break;
                case 'r':
                    new_pos.x -= size;
                    break;
            }
            snake.push_back(new_pos);
        }

};

sf::Vector2f generate_apple(float size);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake");
    window.setFramerateLimit(60);
    int fps_count = 0;   
    
    Snake snake = Snake();

    sf::Vector2f apple_pos = generate_apple(snake.get_size());
    sf::RectangleShape apple;
    apple.setSize(sf::Vector2f(snake.get_size(), snake.get_size()));
    apple.setPosition(apple_pos);
    apple.setFillColor(sf::Color(0, 255, 0));
    
    while(window.isOpen())
    {
        ++fps_count;
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
                switch(event.key.code)
                {
                    case sf::Keyboard::W:
                        snake.set_direction('u');
                        break;
                    case sf::Keyboard::S:
                        snake.set_direction('d');
                        break;
                    case sf::Keyboard::A:
                        snake.set_direction('l');
                        break;
                    case sf::Keyboard::D:
                        snake.set_direction('r');
                        break;
                }
        }
   
        if(fps_count % 10 == 0)
        {
            snake.move();
            if(snake.check_eat_apple(sf::Vector2f(apple_pos.x + 10, apple_pos.y + 10)))
            { 
                apple_pos = generate_apple(snake.get_size());
                apple.setPosition(apple_pos);
                snake.increase_score();
            }       
        }

        if(fps_count > 60)
            fps_count = 0;


        window.clear();
        snake.draw(window);
        window.draw(apple);
        window.display();
    }
}

sf::Vector2f generate_apple(float size)
{
    sf::Vector2f max = sf::Vector2f(WIDTH / size + 1, HEIGHT / size + 1);
    sf::Vector2f result = sf::Vector2f(0, 0);
    
    result.x = (rand() % (int)max.x) * size;
    result.y = (rand() % (int)max.y) * size;

    return result;
}
