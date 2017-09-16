#ifndef GAME_H
#define GAME_H

#include "utils.h"

class Paddle;
class Ball;

class Game
{
public:
    Game();
    ~Game();
    void startPage(GameMode &gameMode, bool &maybeStartGame);
    void go(GameMode gameMode);
private:
    enum class GameState {LEFT_WIN, RIGHT_WIN, PLAY, PAUSE};

    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 500;
    const int WINDOW_LEFT = 150;
    const int WINDOW_TOP = 150;
    const int LEFT_LIMIT = 10;
    const int RIGHT_LIMIT = WINDOW_WIDTH - 10;
    const int TOP_LIMIT = 90;
    const int BOTTOM_LIMIT = WINDOW_HEIGHT - 5;
    const std::string WINDOW_TITLE = "PingPong";
    const std::string PATH_TO_FONT = "Resources/Fonts/DroidSansMono.ttf";
    const int WIN_SCORE = 5;
    const std::string LEFT_WIN_TEXT = "LEFT PLAYER WON!";
    const std::string RIGHT_WIN_TEXT = "RIGHT PLAYER WON!";
    const std::string PAUSE_TEXT = "GAME PAUSED";
    const float PADDLE_TIMER_TIMEOUT = 0.1f;
    const float BALL_TIMER_TIMEOUT = 0.05f;
    const std::string PATH_TO_SHOT_SOUND = "Resources/Sounds/shot.wav";
    ALLEGRO_SAMPLE *shot_;
    GameState gameState_ = GameState::PLAY;
    GameMode gameMode_;
    ALLEGRO_DISPLAY *display_;
    const std::string PATH_TO_STARTPAGE_BITMAP = "Resources/Images/startPage3.png";
    ALLEGRO_BITMAP *startPage_;
    ALLEGRO_FONT *smallFont_;
    ALLEGRO_FONT *largeFont_;
    ALLEGRO_TIMER *paddleTimer_;
    ALLEGRO_TIMER *ballTimer_;
    Paddle *player1_;
    Paddle *player2_;
    Ball *ball_;
    void init();
    void startAllTimers();
    void stopAllTimers();
    void onBallTimerEvent();
    void prepareNewGame();
    void drawCharacters() const;
    void drawField() const;
    void drawScore() const;
    void drawGameState() const;
};

#endif // GAME_H
