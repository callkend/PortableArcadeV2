/*
   Kendall Callister
   2/11/2018
   Snake
   The goal of snake is to get the snake as long as possible without running off the edge of the map or colliding with yourself.
   The snake gets longer as the snake eats the cherries.
 */

//Libraries
#include <stdlib.h>

#include "../../PixiPusher/PixiGFX.h"

#include "../PortableArcade.h"
#include "../Menu.h"
#include "Games.h"

#define MAX_SNAKE_SIZE 256

#define SNAKE_GAME_OFFSET_X 1
#define SNAKE_GAME_OFFSET_Y 1
#define SNAKE_GAME_SIZE_X 30
#define SNAKE_GAME_SIZE_Y 22

typedef enum {
    EASY,
    HARD,
} Difficulty_e;

typedef enum {
    NO_COLLISION,
    BODY_COLLISION,
    CHERRY_COLLISION,
    GOBBLE_COLLISION,
} SnakeCollision_e;

typedef union {

    struct {
        uint8_t X : 8;
        uint8_t Y : 8;
    };

    struct {
        uint16_t A : 16;
    };

} SnakeLocation_t;

typedef struct {
    uint8_t Length;
    SnakeLocation_t Head;
} Snake_t;

extern PixiGFX *graphics;
extern const PGfxFont Font1;

// COOOOLOURS!!!

// Sets GREEN, RED, BLUE rotating colors for scrolling text.
// const Color colors[] = {   Red,
//                            Green,
//                            Blue};

//Sets Snake Color
const Color SNAKE_COLOR = {.R = 255, .G = 102, .B = 0, .A = 0xFF};
//Sets Cherry Color
const Color CHERRY_COLOR = {.R = 255, .G = 0, .B = 0, .A = 0xFF};
//Sets the Eat Self Color for Hard Mode.
const Color GOBBLE_COLOR = {.R = 91, .G = 215, .B = 213, .A = 0xFF};

const Color SNAKE_BACKGROUND_COLOR = {.R = 0, .G = 0, .B = 0, .A = 0xFF};
const Color SNAKE_BORDER_COLOR = {.R = 16, .G = 16, .B = 16, .A = 0xFF};

// Static Variables
Snake_t Snake;
SnakeLocation_t SnakeBody[MAX_SNAKE_SIZE];
uint16_t SnakeHeadIndex = 0;

SnakeLocation_t Gobble, Cherry;

GameState_e SnakeGameState = PRE_GAME;
Difficulty_e Difficulty = EASY;

int score, gobblesStored, hold;

Direction_e SnakeDirection = NO_DIRECTION;
Direction_e NextSnakeDirection = NO_DIRECTION;
Direction_e LastDirection = NO_DIRECTION;

Color GetSnakePixel(SnakeLocation_t location) {
    return PG_GetPixel(graphics, location.X, location.Y);
}

void SetPixel(SnakeLocation_t location, Color color) {
    PG_SetPixel(graphics, location.X, location.Y, color);
}

bool LocationsEqual(SnakeLocation_t left, SnakeLocation_t right) {
    return left.A == right.A;
}

SnakeCollision_e GetSnakeCollision(SnakeLocation_t location) {

    if (LocationsEqual(location, Cherry)) {
        return CHERRY_COLLISION;
    } else if (Difficulty == HARD && LocationsEqual(location, Gobble)) {
        return GOBBLE_COLLISION;
    } else if (GetSnakePixel(location).RGB == SNAKE_BACKGROUND_COLOR.RGB) {
        return NO_COLLISION;
    } else {
        return BODY_COLLISION;
    }
}

SnakeLocation_t GetRandomLocation(void) {
    SnakeLocation_t result;
    result.X = SNAKE_GAME_OFFSET_X + (rand() % SNAKE_GAME_SIZE_X);
    result.Y = SNAKE_GAME_OFFSET_Y + (rand() % SNAKE_GAME_SIZE_Y);
    return result;
}

SnakeLocation_t DrawRandomLocation(Color color) {
    SnakeLocation_t result = GetRandomLocation();
    SetPixel(result, color);
    return result;
}

void ResetGame(void) {
    ResetArcade(true);

    PG_Fill(graphics, SNAKE_BACKGROUND_COLOR);
    // Draw the game edge
    PG_DrawRectangle(graphics, SNAKE_GAME_OFFSET_X - 1, SNAKE_GAME_OFFSET_Y - 1,
            SNAKE_GAME_OFFSET_X + SNAKE_GAME_SIZE_X, SNAKE_GAME_OFFSET_Y + SNAKE_GAME_SIZE_Y,
            SNAKE_BORDER_COLOR);
    // matrix.show();

    LastDirection = NO_DIRECTION;
    SnakeDirection = NextSnakeDirection = DOWN;

    Snake.Head.X = SNAKE_GAME_OFFSET_X + (SNAKE_GAME_SIZE_X >> 1);
    Snake.Head.Y = SNAKE_GAME_OFFSET_Y + (SNAKE_GAME_SIZE_Y >> 1);
    Snake.Length = 4;

    score = gobblesStored = 0;
    hold = 40;

    Cherry = DrawRandomLocation(CHERRY_COLOR);

    if (Difficulty == HARD) {
        Gobble = DrawRandomLocation(GOBBLE_COLOR);
    }
}

MenuResult snakeSetup(PixiGFX *graphics) {
    // put your setup code here, to run once:

    // Randomizes where the cherry starts
    srand(1); // TODO: Seed this thing!

    //Initializes the LED matrix, clears it, and setups the IO
    ResetArcade(true);

    MenuResult result = {.MenuReturn = Exit, .NextDelay = 20};
    return result;
}

Menu_t snakeMenu[];

MenuResult snakeLoop(PixiGFX *graphics) {
    MenuResult result = { .MenuReturn = Continue, .NextDelay = 10 };

    //Changes the Direction of the snake.
    Direction_e activeDirection = GetDirection();

    if (activeDirection != NO_DIRECTION) {
        LastDirection = activeDirection;

        // Only allow changes from Vertical to Horizontal or vise versa
        if (SnakeDirection == UP || SnakeDirection == DOWN) {
            if (activeDirection == LEFT || activeDirection == RIGHT) {
                NextSnakeDirection = activeDirection;
            }
        } else {
            if (activeDirection == UP || activeDirection == DOWN) {
                NextSnakeDirection = activeDirection;
            }
        }
    }

    //Allows controls to be input separately from movement in game.
    static int loopCount;
    if (++loopCount >= hold) {
        loopCount = 0;

        switch (SnakeGameState) {
            default:
            case PRE_GAME:
            {
                /*
                 * Hard Mode adds shedding to the game. If the Player hits the she--dded skin it will also end the game.
                 * An new item is added to the game that allows the player to eat themselves and the shedded skin, but
                 * eating the item will increase the speed of the snake without increasing the score.
                 */

                Difficulty = snakeMenu[1].ActiveSubMenuIndex == 0 ? EASY : HARD;
                SnakeGameState = START_GAME;
                LastDirection = NO_DIRECTION;
                break;
            }

            case START_GAME:
            {
                ResetGame();
                SnakeGameState = RUNNING_GAME;
                break;
            }

            case RUNNING_GAME:
            {
                SnakeDirection = NextSnakeDirection;

                // Makes sure the snake is not outside of the boundaries.
                SnakeLocation_t collisionLocation = Snake.Head;

                switch (SnakeDirection) {
                    case UP:
                        --collisionLocation.Y;
                        if (Snake.Head.Y <= SNAKE_GAME_OFFSET_Y) {
                            SnakeGameState = END_GAME;
                        }
                        break;
                    case DOWN:
                        ++collisionLocation.Y;
                        if (Snake.Head.Y >= ((SNAKE_GAME_OFFSET_Y - 1) + SNAKE_GAME_SIZE_Y)) {
                            SnakeGameState = END_GAME;
                        }
                        break;
                    case LEFT:
                        --collisionLocation.X;
                        if (Snake.Head.X <= SNAKE_GAME_OFFSET_X) {
                            SnakeGameState = END_GAME;
                        }
                        break;
                    case RIGHT:
                        ++collisionLocation.X;
                        if (Snake.Head.X >= ((SNAKE_GAME_OFFSET_X - 1) + SNAKE_GAME_SIZE_X)) {
                            SnakeGameState = END_GAME;
                        }
                        break;
                    default:    
                        break;
                }

                if (SnakeGameState == END_GAME) {
                    hold = 0;
                    result = GameoverLoop(true);
                    break;
                }

                // Adds the color of the snake to the board and detects collision with the snake.

                switch (GetSnakeCollision(collisionLocation)) {
                    case BODY_COLLISION:
                        if (gobblesStored == 0) {
                            SnakeGameState = END_GAME;
                            hold = 0;
                            result = GameoverLoop(false);
                        } else {
                            UpdateBonusBoard(--gobblesStored);
                            ++Snake.Length;
                        }
                        break;

                    case CHERRY_COLLISION:

                        Cherry = DrawRandomLocation(CHERRY_COLOR);

                        ++Snake.Length;
                        UpdateScoreBoard(++score);
                        UpdateBonusBoard(gobblesStored);  // TODO: Why is the score getting cleared here?
                        if (hold > 10) {
                            hold -= 1;
                        }
                        break;

                    case GOBBLE_COLLISION:

                        Gobble = DrawRandomLocation(GOBBLE_COLOR);
                        UpdateScoreBoard(score);        // TODO: Why is the score getting cleared here?
                        UpdateBonusBoard(++gobblesStored);
                        break;
                        
                    case NO_COLLISION:
                        // Nothing to do sir
                        break;
                }

                SetPixel(collisionLocation, SNAKE_COLOR);

                // Erases the snakes tail.
                if (++SnakeHeadIndex >= MAX_SNAKE_SIZE) {
                    SnakeHeadIndex = 0;
                }

                SnakeBody[SnakeHeadIndex] = Snake.Head = collisionLocation;

                int16_t tailPointer = SnakeHeadIndex - Snake.Length;

                if (tailPointer < 0 && Difficulty == EASY) {
                    tailPointer += MAX_SNAKE_SIZE;
                }

                if (tailPointer >= 0) {
                    SnakeLocation_t clearLocation = SnakeBody[tailPointer];

                    Color color = SNAKE_BACKGROUND_COLOR;

                    if (LocationsEqual(clearLocation, Cherry)) {
                        color = CHERRY_COLOR;
                    } else if (Difficulty == HARD && LocationsEqual(clearLocation, Gobble)) {
                        color = GOBBLE_COLOR;
                    }

                    SetPixel(clearLocation, color);
                }

                // matrix.show();
            }
                break;

            case END_GAME:
            {
                hold = 0;
                result = GameoverLoop(false);

                if (result.MenuReturn == Exit) {
                    SnakeGameState = PRE_GAME;
                }
            }
                break;
        }
    }
    return result;
}

Menu_t snakeDifficultyMenu[] = {
    DEFINE_MENU_FUNCTION("Easy", snakeSetup, snakeLoop),    
    DEFINE_MENU_FUNCTION("Hard", snakeSetup, snakeLoop),
    DEFINE_EMPTY_MENU(),
};

Menu_t snakeMenu[] = {
    DEFINE_MENU_FUNCTION("Start", snakeSetup, snakeLoop),    
    DEFINE_MENU("Difficulty", snakeDifficultyMenu),
    DEFINE_EMPTY_MENU(),
};