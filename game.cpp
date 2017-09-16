#include "game.h"
#include "paddle.h"
#include "ball.h"

Game::Game()
{
    init();
    startPage_ = al_load_bitmap(PATH_TO_STARTPAGE_BITMAP.c_str());
    //Загружаем шрифты
    smallFont_ = al_load_font(PATH_TO_FONT.c_str(), 18, 0);
    largeFont_ = al_load_font(PATH_TO_FONT.c_str(), 60, 0);
    //Создаем таймеры
    paddleTimer_ = al_create_timer(PADDLE_TIMER_TIMEOUT);
    ballTimer_ = al_create_timer(BALL_TIMER_TIMEOUT);
    //Загружаем звуковые эффекты
    shot_ = al_load_sample(PATH_TO_SHOT_SOUND.c_str());
    al_reserve_samples(1);
    //Создаем персонажей
    player1_ = new Paddle(LEFT_LIMIT, TOP_LIMIT, 10, Direction::DOWN, 15, 100);
    player2_ = new Paddle(RIGHT_LIMIT - 15, BOTTOM_LIMIT - 100, 10, Direction::UP, 15, 100);
    ball_ = new Ball(RIGHT_LIMIT - 20, TOP_LIMIT - 10, 10, Direction::LEFT_DOWN, 10);}

Game::~Game()
{
    al_destroy_display(display_);
    al_destroy_sample(shot_);
    al_destroy_bitmap(startPage_);
    al_destroy_font(smallFont_);
    al_destroy_font(largeFont_);
    al_destroy_timer(paddleTimer_);
    al_destroy_timer(ballTimer_);
    delete player1_;
    delete player2_;
    delete ball_;
}

void Game::startPage(GameMode &gameMode, bool &maybeStartGame)
{
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *blinkTimer = al_create_timer(0.1f);
    al_register_event_source(event_queue, al_get_display_event_source(display_));
    al_register_event_source(event_queue, al_get_timer_event_source(blinkTimer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    bool flag = false;
    int sourceX = 0, sourceY = 0;
    int pageRegionWidth = 1000, pageRegionHeight = WINDOW_HEIGHT;
    al_start_timer(blinkTimer);
    while(!flag)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap_region(startPage_, sourceX * pageRegionWidth,
        sourceY * pageRegionHeight, pageRegionWidth, pageRegionHeight, 0, 0, 0);
        al_flip_display();
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        al_get_keyboard_state(&keyState);
        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
            if(events.keyboard.keycode == ALLEGRO_KEY_H)
            {
                maybeStartGame = true;
                gameMode = GameMode::HUMAN_VS_HUMAN;
                flag = true;
            }
            else if(events.keyboard.keycode == ALLEGRO_KEY_M)
            {
                maybeStartGame = true;
                gameMode = GameMode::HUMAN_VS_MACHINE;
                flag = true;
            }
            else if(events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                maybeStartGame = false;
                flag = true;
            }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            maybeStartGame = false;
            flag = true;
        }
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            if(events.timer.source == blinkTimer)
                sourceX = (sourceX == 0) ? 1 : 0;
        }
    }
    al_destroy_timer(blinkTimer);
    al_destroy_event_queue(event_queue);
}

void Game::go(GameMode gameMode)
{
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(paddleTimer_));
    al_register_event_source(event_queue, al_get_timer_event_source(ballTimer_));
    al_register_event_source(event_queue, al_get_display_event_source(display_));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    startAllTimers();
    bool done = false;
    //El bucle principal del juego
    while(!done)
    {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        al_get_keyboard_state(&keyState);
        bool draw = false;
        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
            if(events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                done = true;
            }
            else if(events.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                if(gameState_ == GameState::PLAY)
                {
                    gameState_ = GameState::PAUSE;
                    stopAllTimers();
                }
                else if(gameState_ == GameState::PAUSE)
                {
                    gameState_ = GameState::PLAY;
                    startAllTimers();
                }
                draw = true;
            }
            else if(events.keyboard.keycode == ALLEGRO_KEY_N)
            {
                prepareNewGame();
                startAllTimers();
            }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            if(events.timer.source == ballTimer_)
            {
                onBallTimerEvent();
            }
            else if(events.timer.source == paddleTimer_)
            {
                if(al_key_down(&keyState, ALLEGRO_KEY_Q))
                {
                    player1_->setDirection(Direction::UP);
                    if(player1_->getY() - player1_->getSpeed() >= TOP_LIMIT)
                        player1_->move();
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_A))
                {
                    player1_->setDirection(Direction::DOWN);
                    if(player1_->getY() + player1_->getSpeed()<= BOTTOM_LIMIT - player1_->getHeight())
                        player1_->move();
                }
                if(gameMode == GameMode::HUMAN_VS_HUMAN)
                {
                    if(al_key_down(&keyState, ALLEGRO_KEY_O))
                    {
                        player2_->setDirection(Direction::UP);
                        if(player2_->getY() - player2_->getSpeed() >= TOP_LIMIT)
                            player2_->move();
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_L))
                    {
                        player2_->setDirection(Direction::DOWN);
                        if(player2_->getY() + player2_->getSpeed() < BOTTOM_LIMIT - player2_->getHeight())
                            player2_->move();
                    }
                }
                else if(gameMode == GameMode::HUMAN_VS_MACHINE)
                {
                    if(ball_->getX() > WINDOW_WIDTH / 2)
                        player2_->moveCPU(ball_, TOP_LIMIT, BOTTOM_LIMIT);
                }
            }
            if(player1_->getScore() >= WIN_SCORE)
            {
                gameState_ = GameState::LEFT_WIN;
                stopAllTimers();
            }
            else if(player2_->getScore() >= WIN_SCORE)
            {
                gameState_ = GameState::RIGHT_WIN;
                stopAllTimers();
            }
            draw = true;
        }

        if(draw)
        {
            drawField();
            drawScore();
            drawCharacters();
            drawGameState();
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_event_queue(event_queue);
}

void Game::init()
{
    if(!al_init())
        al_show_native_message_box(nullptr, nullptr, nullptr, "Could not initialize Allegro 5", nullptr, 0);
    display_ = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!display_)
        al_show_native_message_box(nullptr, nullptr, nullptr, "Could not create Allegro window.", nullptr, 0);
    al_set_window_position(display_, WINDOW_LEFT, WINDOW_TOP);
    al_set_window_title(display_, WINDOW_TITLE.c_str());
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();
}

void Game::startAllTimers()
{
    al_start_timer(paddleTimer_);
    al_start_timer(ballTimer_);
}

void Game::stopAllTimers()
{
    al_stop_timer(paddleTimer_);
    al_stop_timer(ballTimer_);
}

void Game::onBallTimerEvent()
{
    ball_->move();
    if(ball_->getY() <= TOP_LIMIT + ball_->getRadius())
    {
        if(ball_->getDirection() == Direction::LEFT_UP)
            ball_->setDirection(Direction::LEFT_DOWN);
        else if(ball_->getDirection() == Direction::RIGHT_UP)
            ball_->setDirection(Direction::RIGHT_DOWN);
    }
    else if(ball_->getY() >= BOTTOM_LIMIT - ball_->getRadius())
    {
        if(ball_->getDirection() == Direction::LEFT_DOWN)
            ball_->setDirection(Direction::LEFT_UP);
        else if(ball_->getDirection() == Direction::RIGHT_DOWN)
            ball_->setDirection(Direction::RIGHT_UP);
    }
    if(ball_->getX() <= LEFT_LIMIT)
    {
        int newX = player1_->getX() + player1_->getWidth() + 11;
        int newY = player1_->getY() + player1_->getHeight() / 2;
        Direction newDir = (newY < WINDOW_HEIGHT) / 2 ? Direction::RIGHT_DOWN : Direction::RIGHT_UP;
        al_play_sample(shot_, 1.0f, 0.0f, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
        ball_->setPosition(newX, newY, newDir);
        player2_->incScore();
    }
    else if(ball_->getX() >= RIGHT_LIMIT - ball_->getRadius())
    {
        int newX = player2_->getX() - 11;
        int newY = player2_->getY() + player2_->getHeight() / 2;
        Direction newDir = (newY < WINDOW_HEIGHT / 2) ? Direction::LEFT_DOWN : Direction::LEFT_UP;
        ball_->setPosition(newX, newY, newDir);
        al_play_sample(shot_, 1.0f, 0.0f, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
        player1_->incScore();
    }

    if(ball_->isCollision(player1_))
    {
        al_play_sample(shot_, 1.0f, 0.0f, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
        if(player1_->getDirection() == Direction::UP)
            ball_->setDirection(Direction::RIGHT_UP);
        else if(player1_->getDirection() == Direction::DOWN)
                ball_->setDirection(Direction::RIGHT_DOWN);
    }
    else if(ball_->isCollision(player2_))
    {
        al_play_sample(shot_, 1.0f, 0.0f, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
        if(player2_->getDirection() == Direction::UP)
            ball_->setDirection(Direction::LEFT_UP);
        else if(player2_->getDirection() == Direction::DOWN)
               ball_->setDirection(Direction::LEFT_DOWN);
        else if(player2_->getDirection() == Direction::STOP)
        {
            if(ball_->getDirection() == Direction::RIGHT_DOWN)
                ball_->setDirection(Direction::LEFT_DOWN);
            else if(ball_->getDirection() == Direction::RIGHT_UP)
                    ball_->setDirection(Direction::LEFT_UP);
        }
    }
}

void Game::prepareNewGame()
{
    player1_->reset();
    player2_->reset();
    ball_->setPosition(RIGHT_LIMIT - 20, TOP_LIMIT - 10, Direction::LEFT_DOWN);
    gameState_ = GameState::PLAY;
}

void Game::drawCharacters() const
{
    ball_->draw();
    player1_->draw();
    player2_->draw();
}

void Game::drawField() const
{
    al_draw_text(largeFont_, al_map_rgb(255, 0, 0), 350, 10, 0, "PINGPONG");
    al_draw_filled_rectangle(LEFT_LIMIT - 5, TOP_LIMIT - 5, RIGHT_LIMIT + 5, BOTTOM_LIMIT + 5, al_map_rgb(143, 189, 84));
        al_draw_rectangle(LEFT_LIMIT - 5, TOP_LIMIT - 5, RIGHT_LIMIT + 5, BOTTOM_LIMIT + 5, al_map_rgb(255, 255, 0), 5);
    al_draw_circle(WINDOW_WIDTH / 2, TOP_LIMIT + (WINDOW_HEIGHT - TOP_LIMIT) / 2 - 5, 75, al_map_rgb(255, 255, 255), 5);
    al_draw_line(WINDOW_WIDTH / 2, TOP_LIMIT - 5, WINDOW_WIDTH / 2, WINDOW_HEIGHT, al_map_rgb(255, 255, 255), 5);
}

void Game::drawScore() const
{
    int leftScore = player1_->getScore();
    std::stringstream ss1;
    ss1 << leftScore;
    int rightScore = player2_->getScore();
    std::stringstream ss2;
    ss2 << rightScore;
    al_draw_text(largeFont_, al_map_rgb(0, 255, 0), 30, 10, 0, ss1.str().c_str());
    al_draw_text(largeFont_, al_map_rgb(0, 0, 255), 800, 10, 0, ss2.str().c_str());
}

void Game::drawGameState() const
{
    switch(gameState_)
    {
        case GameState::PAUSE :
            al_draw_text(largeFont_, al_map_rgb(100, 150, 220), 310, 250, 0, PAUSE_TEXT.c_str());
            break;
        case GameState::LEFT_WIN :
            al_draw_text(largeFont_, al_map_rgb(0, 255, 0), 200, 250, 0, LEFT_WIN_TEXT.c_str());
            break;
        case GameState::RIGHT_WIN :
            al_draw_text(largeFont_, al_map_rgb(0, 0, 255), 200, 250, 0, RIGHT_WIN_TEXT.c_str());
            break;
        case GameState::PLAY :
        default :
            break;
    }
}
