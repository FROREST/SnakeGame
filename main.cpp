#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

const int width = 20;
const int height = 10;
const int blockSize = 20;
const int maxTailSize = 100;

int snakeX, snakeY;
int fruitX, fruitY;
int tailX[maxTailSize], tailY[maxTailSize];
int nTail;
int score;

enum Direction { STOP, LEFT, RIGHT, UP, DOWN };
Direction dir;

bool gameOver;
bool gameOverMenu;
sf::Font font;
sf::Text scoreText;
sf::Text menuText;
chrono::steady_clock::time_point lastTime;
const double interval = 0.5;

void Setup();
void Draw(sf::RenderWindow& window);
void Input();
void Logic();
void ProcessEvents(sf::RenderWindow& window);
void HandleKeyPress(sf::Keyboard::Key key);
void DrawTail(sf::RenderWindow& window);
void GenerateFruit();
void ShowGameOverMenu(sf::RenderWindow& window);
void RestartGame();

int main()
{
    sf::RenderWindow window(sf::VideoMode(width * blockSize, height * blockSize), "Snake Game");
    Setup();

    while (window.isOpen())
    {
        ProcessEvents(window);

        if (!gameOver)
        {
            auto currentTime = chrono::steady_clock::now();
            chrono::duration<double> elapsed = chrono::duration_cast<chrono::duration<double>>(currentTime - lastTime);

            if (elapsed.count() >= interval)
            {
                Input();
                Logic();
                Draw(window);
                lastTime = chrono::steady_clock::now();
            }
        }
        else
        {
            ShowGameOverMenu(window);
        }
    }

    return 0;
}

/**
 * @brief Инициализация начальных параметров игры
 */
void Setup()
{
    srand(time(nullptr));

    gameOver = false;
    gameOverMenu = false;
    dir = STOP;
    snakeX = width / 2;
    snakeY = height / 2;
    nTail = 0;
    score = 0;

    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font." << endl;
        return;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    menuText.setFont(font);
    menuText.setCharacterSize(30);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(145, 40);

    lastTime = chrono::steady_clock::now();

    GenerateFruit();
}

/**
 * @brief Отрисовка элементов на экране
 * @param window Окно для отрисовки
 */
void Draw(sf::RenderWindow& window)
{
    window.clear();

    if (!gameOver)
    {
        sf::RectangleShape snakeSegment(sf::Vector2f(blockSize, blockSize));
        snakeSegment.setFillColor(sf::Color::Green);

        snakeSegment.setPosition(snakeX * blockSize, snakeY * blockSize);
        window.draw(snakeSegment);

        DrawTail(window);

        sf::RectangleShape fruit(sf::Vector2f(blockSize, blockSize));
        fruit.setFillColor(sf::Color::Red);
        fruit.setPosition(fruitX * blockSize, fruitY * blockSize);
        window.draw(fruit);

        scoreText.setString("Score: " + to_string(score));
        window.draw(scoreText);
    }
    else
    {
        ShowGameOverMenu(window);
    }

    window.display();
}

/**
 * @brief Обработка пользовательского ввода
 */
void Input()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir != RIGHT)
        dir = LEFT;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir != LEFT)
        dir = RIGHT;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir != DOWN)
        dir = UP;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir != UP)
        dir = DOWN;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        gameOver = true;
}

/**
 * @brief Логика игры
 */
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = snakeX;
    tailY[0] = snakeY;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        snakeX--;
        break;
    case RIGHT:
        snakeX++;
        break;
    case UP:
        snakeY--;
        break;
    case DOWN:
        snakeY++;
        break;
    default:
        break;
    }

    if (snakeX < 0 || snakeX >= width || snakeY < 0 || snakeY >= height)
        gameOver = true;

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == snakeX && tailY[i] == snakeY)
            gameOver = true;
    }

    if (snakeX == fruitX && snakeY == fruitY)
    {
        nTail++;
        score += 10;
        GenerateFruit();
    }
}

/**
 * @brief Обработка событий окна
 * @param window Окно для обработки событий
 */
void ProcessEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::KeyPressed)
            HandleKeyPress(event.key.code);
    }
}

/**
 * @brief Обработка нажатия клавиши
 * @param key Код клавиши
 */
void HandleKeyPress(sf::Keyboard::Key key)
{
    if (!gameOver)
    {
        switch (key)
        {
        case sf::Keyboard::Left:
            dir = LEFT;
            break;
        case sf::Keyboard::Right:
            dir = RIGHT;
            break;
        case sf::Keyboard::Up:
            dir = UP;
            break;
        case sf::Keyboard::Down:
            dir = DOWN;
            break;
        case sf::Keyboard::Escape:
            gameOver = true;
            break;
        default:
            break;
        }
    }
    else
    {
        if (key == sf::Keyboard::Up)
        {
            if (menuText.getString() == "Game Over\n\n1. Restart\n2. Quit")
                menuText.setString("Game Over\n\n1. Restart");
        }
        else if (key == sf::Keyboard::Down)
        {
            if (menuText.getString() == "Game Over\n\n1. Restart")
                menuText.setString("Game Over\n\n1. Restart\n2. Quit");
        }
        else if (key == sf::Keyboard::Enter)
        {
            if (menuText.getString() == "Game Over\n\n1. Restart")
            {
                RestartGame();
            }
            else if (menuText.getString() == "Game Over\n\n1. Restart\n2. Quit")
            {
                exit(EXIT_SUCCESS);
            }

            gameOverMenu = false;
        }
    }
}

/**
 * @brief Отрисовка хвоста змеи
 * @param window Окно для отрисовки
 */
void DrawTail(sf::RenderWindow& window)
{
    sf::RectangleShape snakeSegment(sf::Vector2f(blockSize, blockSize));
    snakeSegment.setFillColor(sf::Color::Blue);

    for (int i = 0; i < nTail; i++)
    {
        snakeSegment.setPosition(tailX[i] * blockSize, tailY[i] * blockSize);
        window.draw(snakeSegment);
    }
}

/**
 * @brief Генерация координат нового фрукта
 */
void GenerateFruit()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribX(0, width - 1);
    uniform_int_distribution<int> distribY(0, height - 1);

    fruitX = distribX(gen);
    fruitY = distribY(gen);
}

/**
 * @brief Отображение меню после окончания игры
 * @param window Окно для отрисовки
 */
void ShowGameOverMenu(sf::RenderWindow& window)
{
    menuText.setString("Game Over\n1. Restart\n2. Quit");
    window.draw(menuText);
    gameOverMenu = true;
}

/**
 * @brief Перезапуск игры
 */
void RestartGame()
{
    snakeX = width / 2;
    snakeY = height / 2;
    nTail = 0;
    score = 0;
    dir = STOP;
    gameOver = false;
    gameOverMenu = false;
    GenerateFruit();
}
