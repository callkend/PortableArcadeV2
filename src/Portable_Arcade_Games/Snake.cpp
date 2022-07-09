/*
   Kendall Callister
   2/11/2018
   Snake
   The goal of snake is to get the snake as long as possible without running off the edge of the map or colliding with yourself.
   The snake gets longer as the snake eats the cherries.
*/

//Libaries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "portableArcade.h"
#include "games.h"

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define ZigZag

#define MAX_SNAKE_SIZE 256

typedef enum
{
  EASY,
  HARD,
} Difficulty_e;

typedef enum
{
  NO_COLLISION,
  BODY_COLLISION,
  CHERRY_COLLISION,
  GOBBLE_COLLISION,
} SnakeCollision_e;

typedef union
{
  struct
  {
    uint8_t X : 4;
    uint8_t Y : 4;
  };

  struct
  {
    uint8_t A : 8;
  };

} SnakeLocation_t;

typedef struct
{
  uint8_t Length;
  SnakeLocation_t Head;
} Snake_t;

extern Adafruit_NeoMatrix matrix;

// COOOOLOURS!!!
//Sets Snake Color
const uint16_t COLOR_RED = matrix.Color(255, 0, 0);
//Sets Cherry Color
const uint16_t COLOR_GREEN = matrix.Color(0, 255, 0);
//Sets the Eat Self Color for Hard Mode.
const uint16_t COLOR_BLUE = matrix.Color(0, 0, 255);

// Sets GREEN, RED, BLUE rotating colors for scrolling text.
const uint16_t colors[] = {COLOR_RED,
                           COLOR_GREEN,
                           COLOR_BLUE};

//Sets Snake Color
const uint16_t SNAKE_COLOR = matrix.Color(255, 102, 0);
//Sets Cherry Color
const uint16_t CHERRY_COLOR = matrix.Color(255, 0, 0);
//Sets the Eat Self Color for Hard Mode.
const uint16_t GOBBLE_COLOR = matrix.Color(91, 215, 213);

const uint16_t BACKGROUND_COLOR = matrix.Color(0, 0, 0);

// Static Variables
Snake_t Snake;
SnakeLocation_t SnakeBody[MAX_SNAKE_SIZE];
uint8_t SnakeHeadIndex = 0;

SnakeLocation_t Gobble, Cherry;

GameState_e SnakeGameState = PRE_GAME;
Difficulty_e Difficulty = EASY;

int score, gobblesStored, hold;

Direction_e SnakeDirection = NO_DIRECTION;
Direction_e NextSnakeDirection = NO_DIRECTION;
Direction_e LastDirection = NO_DIRECTION;

boolean PRINT = true;
int cursorIndex = matrix.width();

int GetSnakePixel(SnakeLocation_t location)
{
  uint8_t address = location.A;

#ifdef ZigZag
  // Correct for the ZigZag in the LED Array
  if (!bitRead(address, 4))
  {
    address ^= 0x0F;
  }
#endif
  //matrix.setPixelColor(address, COLOR_BLUE);
  return matrix.getPixelColor(address);
}

int SetPixel(SnakeLocation_t location, int color)
{
  matrix.drawPixel(location.X, location.Y, color);
}

inline boolean LocationsEqual(SnakeLocation_t left, SnakeLocation_t right)
{
  return left.A == right.A;
}

SnakeCollision_e GetSnakeCollision(SnakeLocation_t location)
{

  if (LocationsEqual(location, Cherry))
  {
    return CHERRY_COLLISION;
  }
  else if (Difficulty == HARD && LocationsEqual(location, Gobble))
  {
    return GOBBLE_COLLISION;
  }
  else if (GetSnakePixel(location) == BACKGROUND_COLOR)
  {
    return NO_COLLISION;
  }
  else
  {
    return BODY_COLLISION;
  }
}

SnakeLocation_t GetRandomLocation(void)
{
  SnakeLocation_t result;
  result.X = random(16);
  result.Y = random(16);
  return result;
}

SnakeLocation_t DrawRandomLocation(int color)
{
  SnakeLocation_t result = GetRandomLocation();
  SetPixel(result, color);
  return result;
}

void ResetGame(void)
{
  ResetScoreAndBonus();

  matrix.fillScreen(BACKGROUND_COLOR);
  matrix.show();

  LastDirection = NO_DIRECTION;
  SnakeDirection = NextSnakeDirection = DOWN;

  Snake.Head.X = 5;
  Snake.Head.Y = 5;
  Snake.Length = 4;

  score = gobblesStored = 0;
  hold = 500;

  Cherry = DrawRandomLocation(CHERRY_COLOR);

  if (Difficulty == HARD)
  {
    Gobble = DrawRandomLocation(GOBBLE_COLOR);
  }
}

void snakeSetup()
{
  // put your setup code here, to run once:

  // Randomizes where the cherry starts
  randomSeed(analogRead(0));

  //Initializes the LED matrix, clears it, and setups the IO
  initPortableArcade(&matrix);

  // Allows for the arduino to talk to the computer.
  //Serial.begin(9600);
}

bool snakeLoop()
{
  // put your main code here, to run repeatedly:
  delay(1);

  //Changes the Direction of the snake.
  Direction_e activeDirection = GetDirection();

  if (activeDirection != NO_DIRECTION)
  {
    LastDirection = activeDirection;

    // Only allow changes from Vertical to Horizontal or vise versa
    if (SnakeDirection == UP || SnakeDirection == DOWN)
    {
      if (activeDirection == LEFT || activeDirection == RIGHT)
      {
        NextSnakeDirection = activeDirection;
      }
    }
    else
    {
      if (activeDirection == UP || activeDirection == DOWN)
      {
        NextSnakeDirection = activeDirection;
      }
    }
  }

  //Allows controls to be input separately from movement in game.
  static int loopCount;
  if (++loopCount >= hold)
  {
    loopCount = 0;

    switch (SnakeGameState)
    {
    default:
    case PRE_GAME:
    {
      hold = 100;

      matrix.show();
      matrix.fillScreen(0);
      matrix.setCursor(cursorIndex, 0);

      // Displays how to select the hard difficulty.
      matrix.setTextColor(matrix.Color(255, 93, 21));
      matrix.print("Hard");

      int arrowCenter = cursorIndex + 27;
      // Draw up arrow
      matrix.drawLine(arrowCenter, 0, arrowCenter, 6, COLOR_RED);
      matrix.drawLine(arrowCenter, 0, arrowCenter - 2, 2, COLOR_RED);
      matrix.drawLine(arrowCenter, 0, arrowCenter + 2, 2, COLOR_RED);
      matrix.setCursor(cursorIndex, 8);

      // Displays how to select the easy difficulty.
      matrix.print("Easy");
      matrix.drawLine(arrowCenter, 15, arrowCenter, 8, COLOR_RED);
      matrix.drawLine(arrowCenter, 15, arrowCenter - 2, 13, COLOR_RED);
      matrix.drawLine(arrowCenter, 15, arrowCenter + 2, 13, COLOR_RED);

      if (--cursorIndex < -29)
      {
        cursorIndex = matrix.width();
      }

      // Grabs the player input if they select hard.
      /*
        * Hard Mode adds shedding to the game. If the Player hits the she--dded skin it will also end the game.
        * An new item is added to the game that allows the player to eat themselves and the shedded skin, but
        * eating the item will increase the speed of the snake without increasing the score.
        */

      switch (LastDirection)
      {
      case UP:
        Difficulty = HARD;
        SnakeGameState = START_GAME;
        break;

      case DOWN:
        Difficulty = EASY;
        SnakeGameState = START_GAME;
        break;
        case LEFT:
        return false;
      }

      LastDirection = NO_DIRECTION;
    }
    break;

    case START_GAME:
      ResetGame();
      SnakeGameState = RUNNING_GAME;
      break;

    case RUNNING_GAME:
    {
      SnakeDirection = NextSnakeDirection;

      // Makes sure the snake is not outside of the boundaries.
      SnakeLocation_t collisionLocation = Snake.Head;

      switch (SnakeDirection)
      {
      case UP:
        --collisionLocation.Y;
        if (Snake.Head.Y == 0)
        {
          SnakeGameState = END_GAME;
        }
        break;
      case DOWN:
        ++collisionLocation.Y;
        if (Snake.Head.Y == 15)
        {
          SnakeGameState = END_GAME;
        }
        break;
      case LEFT:
        --collisionLocation.X;
        if (Snake.Head.X == 0)
        {
          SnakeGameState = END_GAME;
        }
        break;
      case RIGHT:
        ++collisionLocation.X;
        if (Snake.Head.X == 15)
        {
          SnakeGameState = END_GAME;
        }
        break;
      }

      if (SnakeGameState == END_GAME)
      {
        break;
      }

      // Adds the color of the snake to the board and detects collision with the snake.

      switch (GetSnakeCollision(collisionLocation))
      {
      case BODY_COLLISION:
        if (gobblesStored == 0)
        {
          SnakeGameState = END_GAME;
        }
        else
        {
          SetBonus(--gobblesStored);
          ++Snake.Length;
        }
        break;

      case CHERRY_COLLISION:

        Cherry = DrawRandomLocation(CHERRY_COLOR);

        ++Snake.Length;
        SetScore(++score);

        if (hold > 100)
        {
          hold -= 10;
        }
        break;

      case GOBBLE_COLLISION:

        Gobble = DrawRandomLocation(GOBBLE_COLOR);
        SetBonus(++gobblesStored);
        break;

      case NO_COLLISION:
        // Nothing to do sir
        break;
      }

      SetPixel(collisionLocation, SNAKE_COLOR);

      // Erases the snakes tail.
      if (++SnakeHeadIndex >= MAX_SNAKE_SIZE)
      {
        SnakeHeadIndex = 0;
      }

      SnakeBody[SnakeHeadIndex] = Snake.Head = collisionLocation;

      int16_t tailPointer = SnakeHeadIndex - Snake.Length;

      if (tailPointer < 0 && Difficulty == EASY)
      {
        tailPointer += MAX_SNAKE_SIZE;
      }

      if (tailPointer >= 0)
      {
        SnakeLocation_t clearLocation = SnakeBody[tailPointer];

        int color = BACKGROUND_COLOR;

        if (LocationsEqual(clearLocation, Cherry))
        {
          color = CHERRY_COLOR;
        }
        else if (Difficulty == HARD && LocationsEqual(clearLocation, Gobble))
        {
          color = GOBBLE_COLOR;
        }

        SetPixel(clearLocation, color);
      }

      matrix.show();
    }
    break;

    case END_GAME:

      static byte loopTimes = 0;
      hold = 100;

      // delay the matrix then repeat.
      if ((loopTimes >= 1 && (LastDirection == RIGHT || LastDirection == LEFT)) || loopTimes >= 3)
      {
        loopTimes = 0;
        SnakeGameState = PRE_GAME;
      }
      else
      {
        LastDirection = NO_DIRECTION;

        matrix.fillScreen(0);
        matrix.setCursor(cursorIndex, 0);
        matrix.print(F("Gameover"));

        if (--cursorIndex < -46)
        {
          cursorIndex = matrix.width();

          matrix.setTextColor(colors[loopTimes]);
          ++loopTimes;
        }
        matrix.setCursor(0, 8);
        matrix.print(score);
        matrix.show();
      }
      break;
    }
  }
  return true;
}