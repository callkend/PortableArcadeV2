/*
Kendall and Wyatt
Tetris
*/

#include <stdlib.h>

#include "../../PixiPusher/PixiGFX.h"

#include "../PortableArcade.h"
#include "../Menu.h"
#include "Games.h"

#define POINTS_PER_SHAPE 4

#define PreviewOffsetX 13
#define PreviewOffsetY 1
#define PreviewSizeX 3
#define PreviewSizeY 4

#define GameOffsetX 1
#define GameOffsetY 2
#define GameSizeX 10
#define GameSizeY 20

#define PlayerStartingX (GameOffsetX + 4)
#define PlayerStartingY (GameOffsetY + 1)
#define StartingDownBeat 300

extern PixiGFX *graphics;

GameState_e TetrisGameState = PRE_GAME;

const Color LINE_COLOR =         { .R =  17, .G = 157, .B = 242, .A = 0xFF };
const Color NORMAL_L_COLOR =     { .R = 235, .G =  74, .B =   2, .A = 0xFF };
const Color BACKWARD_L_COLOR =   { .R =  17, .G =  44, .B = 233, .A = 0xFF };
const Color NORMAL_Z_COLOR =     { .R =  65, .G = 173, .B =   1, .A = 0xFF };
const Color BACKWARD_Z_COLOR =   { .R = 221, .G =   4, .B =  31, .A = 0xFF };
const Color NORMAL_T_COLOR =     { .R = 184, .G =  24, .B = 134, .A = 0xFF };
const Color SQUARE_COLOR =       { .R = 252, .G = 189, .B =  26, .A = 0xFF };
const Color BACKGROUND_COLOR =   { .R =   0, .G =   0, .B =   0, .A = 0xFF };
const Color WALL_COLOR =         { .R = 119, .G = 119, .B = 119, .A = 0xFF };
const Color TEXT_COLOR =         { .R = 255, .G =  94, .B =  21, .A = 0xFF };

/** @brief A flags enum used to indicate where a shape has collided */
typedef enum
{
    NO_COLLISION = 0,
    COLLISION_ON_RIGHT = 1,
    COLLISION_ON_LEFT = 2,
    COLLISION_ON_BOTTOM = 4,
} Collision_e;

/** @brief An enum of the names of all the shapes 
 *  @remark This isn't really needed but makes shapes easier to follow
 */
typedef enum
{
    LINE,
    NORMAL_L,
    BACKWARDS_L,
    NORMAL_Z,
    BACKWARDS_Z,
    NORMAL_T,
    SQUARE,
    NO_SHAPE,
} Shape_e;

/** @brief  A point that can be negative or positive. Very useful when trying 
 *          to center stuff.
*/
typedef struct
{
    int8_t X;
    int8_t Y;
} ShapePoint_t;

// /** @brief A positive integer location on the array. */
typedef struct
{
    int8_t X;
    int8_t Y;

} Location_t;

/** @brief Stores all the data needed to draw a shape */
typedef struct
{
    Shape_e Name;
    Color Color;

    ShapePoint_t Points[POINTS_PER_SHAPE];
} Shape_t;

/** @brief Returns a random shape ready to manipulate 
 *  @returns A random shape
 */
Shape_t GetRandomShape(void)
{

    Shape_t result;
    result.Name = (Shape_e)(rand() % 7);

    switch (result.Name)
    {
    case LINE:
        result.Color = LINE_COLOR;
        result.Points[0].X = 0; result.Points[0].Y = 3;
        result.Points[1].X = 0; result.Points[1].Y = 1;
        result.Points[2].X = 0; result.Points[2].Y = -1;
        result.Points[3].X = 0; result.Points[3].Y = -3;
        break;
    case NORMAL_L:
        result.Color = NORMAL_L_COLOR;
        result.Points[0].X = -1; result.Points[0].Y = -2;
        result.Points[1].X = -1; result.Points[1].Y = 0;
        result.Points[2].X = -1; result.Points[2].Y = 2;
        result.Points[3].X = 1; result.Points[3].Y = 2;
        break;
    case BACKWARDS_L:
        result.Color = BACKWARD_L_COLOR;
        result.Points[0].X = 1; result.Points[0].Y = -2;
        result.Points[1].X = 1; result.Points[1].Y = 0;
        result.Points[2].X = -1; result.Points[2].Y = 2;
        result.Points[3].X = 1; result.Points[3].Y = 2;
        break;
    case NORMAL_T:
        result.Color = NORMAL_T_COLOR;
        result.Points[0].X = 0; result.Points[0].Y = -1;
        result.Points[1].X = -2; result.Points[1].Y = 1;
        result.Points[2].X = 0; result.Points[2].Y = 1;
        result.Points[3].X = 2; result.Points[3].Y = 1;
        break;
    case NORMAL_Z:
        result.Color = NORMAL_Z_COLOR;
        result.Points[0].X = -2; result.Points[0].Y = -1;
        result.Points[1].X = 0; result.Points[1].Y = -1;
        result.Points[2].X = 0; result.Points[2].Y = 1;
        result.Points[3].X = 2; result.Points[3].Y = 1;
        break;
    case BACKWARDS_Z:
        result.Color = BACKWARD_Z_COLOR;
        result.Points[0].X = 0; result.Points[0].Y = -1;
        result.Points[1].X = 2; result.Points[1].Y = -1;
        result.Points[2].X = -2; result.Points[2].Y = 1;
        result.Points[3].X = 0; result.Points[3].Y = 1;
        break;
    case SQUARE:
        result.Color = SQUARE_COLOR;
        result.Points[0].X = 1; result.Points[0].Y = -1;
        result.Points[3].X = 1; result.Points[3].Y = 1;
        result.Points[1].X = -1; result.Points[1].Y = -1;
        result.Points[2].X = -1; result.Points[2].Y = 1;
        break;
    case NO_SHAPE:
        // TODO: Display some error
        break;
    }
    return result;
}

/** @brief Swaps the X and Y value of a point
 *  @returns A new point with X and Y swapped
 */
ShapePoint_t TransposePoint(ShapePoint_t point)
{
    int8_t temp = point.X;
    point.X = point.Y;
    point.Y = temp;
    return point;
}

/** @brief Takes a point an rotates it clockwise around 0,0 
 *  @param point The point to rotate
 *  @returns The rotated point
 */
ShapePoint_t RotatePointClockwise(ShapePoint_t point)
{

    point.Y *= -1;
    point = TransposePoint(point);
    return point;
}

/** @brief Takes a point an rotates it counter-clockwise around 0,0 
 *  @param point The point to rotate
 *  @remark This method isn't used in the current build
 *  @returns The rotated point
 */
ShapePoint_t RotatePointAntiClockwise(ShapePoint_t point)
{

    point = TransposePoint(point);
    point.Y *= -1;
    return point;
}

/** @brief Rotates all the points in a shape around 0,0
 *  @param shape A pointer to a shape that will have all its points rotated
 */
void RotateShape(Shape_t *shape)
{
    ShapePoint_t *point;

    for (int y = 0; y < POINTS_PER_SHAPE; ++y)
    {
        point = &shape->Points[y];
        *point = RotatePointClockwise(*point);
    }
}

/**
 * @brief  Takes a given location and shape and returns the points
 *         that this shape will occupy
 * @param center The point the shape will be centered about
 * @param shape  A collection of points that make up the shape
 * @return The points to color to make the shape
 */
Location_t *PlotShape(Location_t center, Shape_t shape)
{

    static Location_t result[POINTS_PER_SHAPE];

    for (int y = 0; y < POINTS_PER_SHAPE; ++y)
    {
        ShapePoint_t location = shape.Points[y];
        location.X = location.X >> 1;
        location.Y = location.Y >> 1;

        result[y].X = (int8_t)(location.X + (int8_t)center.X);
        result[y].Y = (int8_t)(location.Y + (int8_t)center.Y);
    }

    return result;
}

/** @brief Draws a collection of points on the matrix
 * @param points A pointer to an array of points
 * @param count The number of points in the array to draw
 * @param color The color to set the specified points
 */
void DrawPoints(Location_t *points, int count, Color color)
{

    for (int y = 0; y < count; y++)
    {
        Color c = PG_GetPixel(graphics, points->X, points->Y);

        // Don't draw on the boarder
        if (c.RGB != WALL_COLOR.RGB){
            PG_SetPixel(graphics, points->X, points->Y, color);
        }

        ++points;
    }
}

/** @brief Draws a given shape on the matrix
 * @param center Where the shape should be centered about
 * @param shape The shape to draw
 */
void DrawShape(Location_t center, Shape_t shape)
{
    DrawPoints(PlotShape(center, shape), POINTS_PER_SHAPE, shape.Color);
}

/** @brief Draws the given shape in the preview window
 *  @param shape The shape to draw
 */
void DrawPreview(Shape_t shape)
{
    PG_FillRectangle(graphics, PreviewOffsetX, PreviewOffsetY,
                    PreviewOffsetX + PreviewSizeX, PreviewOffsetY + PreviewSizeY, 
                    BACKGROUND_COLOR);

    const Location_t previewCenter = {PreviewOffsetX + (PreviewSizeX >> 1),
                                      PreviewOffsetY + (PreviewSizeY >> 1)};

    DrawShape(previewCenter, shape);
}

/** @brief The same as DrawShape, but defaults to the BACKGROUND_COLOR
 * @param center Where the shape should be centered about
 * @param shape The shape to draw
 */
void ClearShape(Location_t center, Shape_t shape)
{
    DrawPoints(PlotShape(center, shape), POINTS_PER_SHAPE, BACKGROUND_COLOR);
}

/** @brief gets the RAW color from a given location 
 * @param location The location to get the RAW color from
 * @returns The raw color from the given location
 */
Color GetPixel(Location_t location)
{
    return PG_GetPixel(graphics, location.X, location.Y);
}

/** @brief Checks if any lines in the play area are filled, clears the full rows,
 *         shifts the rows above down, and returns the number of rows cleared
 * @returns The number of rows cleared
 */
uint8_t LineErase()
{
    uint8_t linesCleared = 0;
    int fillCount;

    for (int y = (GameOffsetY + GameSizeY - 1); y >= GameOffsetY; --y)
    {
        fillCount = 0;
        for (int x = GameOffsetX; x < (GameOffsetX + GameSizeX); ++x)
        {
            Color pixel = PG_GetPixel(graphics, x, y);

            if (pixel.RGB != BACKGROUND_COLOR.RGB)
            {
                ++fillCount;
            }

            if (linesCleared > 0)
            {
                int8_t shiftY = y + linesCleared;
                
                // Shift pixel
                PG_SetPixel(graphics, x, shiftY, pixel);
            }
        }

        if (fillCount >= GameSizeX)
        {
            ++linesCleared;
        }
    }

    return linesCleared;
}

/** @brief Checks if a given flags enum contains a flag
 * @param input The flags enum to check
 * @param flag The flag to check in the enum
 * @returns True if the input contains the flag
 */
bool ContainsFlag(int input, int flag)
{
    return (input & flag) > 0;
}

/** @brief Checks if a given set of point contains a matching point
 * @param points The collection to check
 * @param point The point to look for
 *  @returns true if points contains the given point
 */
bool ContainsPoint(Location_t *points, Location_t point)
{
    for (int i = 0; i < POINTS_PER_SHAPE; ++i)
    {
        Location_t location = points[i];

        if (location.X == point.X && location.Y == point.Y)
        {
            return true;
        }
    }

    return false;
}

/** @brief Checks the matrix to see if anything is occupying the given
 *         location, excluding points
 * @param points Points in the matrix that don't need checked
 * @param point The point to in question
 * @returns True if the given point collided with any colored block
 */
bool PointCollides(Location_t *points, Location_t point)
{

    if (ContainsPoint(points, point))
    {
        // Return false if the point collides with its self
        return false;
    }
    else
    {
        return GetPixel(point).RGB != BACKGROUND_COLOR.RGB;
    }
}

/** @brief Checks if a given shape is touching anything in the play area
 * @param playerPostion Where the shape is in the matrix
 * @param shape The shape in question
 * @returns The types of collisions the given shape will encounter
 *          at a given location
*/
Collision_e DetectCollision(Location_t playerPostion, Shape_t shape)
{
    Location_t *points = PlotShape(playerPostion, shape);

    int result = NO_COLLISION;

    for (int i = 0; i < POINTS_PER_SHAPE; ++i)
    {
        Location_t location = points[i];

        // Check for bottom collisions
        if (!ContainsFlag(result, COLLISION_ON_BOTTOM))
        {
            if (location.Y >= (GameOffsetY + GameSizeY - 1))
            {
                result |= COLLISION_ON_BOTTOM;
            }
            else
            {
                ++location.Y;

                if (PointCollides(points, location))
                {
                    result |= COLLISION_ON_BOTTOM;
                }

                --location.Y;
            }
        }

        // Check for left collisions
        if (!ContainsFlag(result, COLLISION_ON_LEFT))
        {
            if (location.X <= GameOffsetX)
            {
                result |= COLLISION_ON_LEFT;
            }
            else
            {
                --location.X;

                if (PointCollides(points, location))
                {
                    result |= COLLISION_ON_LEFT;
                }

                ++location.X;
            }
        }

        // Check for right collisions
        if (!ContainsFlag(result, COLLISION_ON_RIGHT))
        {
            if (location.X >= (GameOffsetX + GameSizeX - 1))
            {
                result |= COLLISION_ON_RIGHT;
            }
            else
            {
                ++location.X;

                if (PointCollides(points, location))
                {
                    result |= COLLISION_ON_RIGHT;
                }

                --location.X;
            }
        }
    }

    return (Collision_e)result;
}

/**
 * @brief   Checks if the given shape would collide would override pixels
 * @param playerPostion Where the shape is located in the play area
 * @param shape The shape in question
 * @return  True if the shape will fit without conflict
 */
bool CheckFit(Location_t playerPostion, Shape_t shape)
{
    Location_t *points = PlotShape(playerPostion, shape);

    for (uint8_t i = 0; i < POINTS_PER_SHAPE; ++i)
    {

        Location_t point = points[i];

        if (point.X < GameOffsetX)
        {
            return false;
        }
        else if (point.X > (GameOffsetX + GameSizeX))
        {
            return false;
        }
        else if (point.Y > (GameOffsetY + GameSizeY))
        {
            return false;
        }
        else if (GetPixel(point).RGB != BACKGROUND_COLOR.RGB)
        {
            return false;
        }
    }

    return true;
}

MenuResult tetrisSetup(PixiGFX *graphics)
{
    //Serial.begin(9600);
    srand(1);   // TODO: Seed this thing!
    TetrisGameState = PRE_GAME;
    //Initializes the LED matrix, clears it, and setups the IO
    ResetArcade(true);

    MenuResult result = { .MenuReturn = Exit, .NextDelay = 20 };
    return result;
}

MenuResult tetrisLoop(PixiGFX *graphics)
{
    static Shape_t currentShape;
    static Shape_t nextShape;
    static Location_t playerOffset;

    static Direction_e lastDirection;
    static Location_t lastOffset;
    static Shape_t lastShape;

    // Tracks what frame of the game we are on
    static uint16_t downCount = 0;
    // Tracks how often to force a shape down one
    static uint16_t downBeat;

    static uint16_t score;

    static uint16_t linesCleared;

    MenuResult result = { .MenuReturn = Continue, .NextDelay = 3 };

    switch (TetrisGameState)
    {
    case PRE_GAME:
    {
        PG_Fill(graphics, BACKGROUND_COLOR);

        TetrisGameState = START_GAME;
        result.NextDelay = 50;
    }

    break;
    default:
    case START_GAME:
        if (GetDirection() == NO_DIRECTION) {
            playerOffset.X = PlayerStartingX;
            playerOffset.Y = PlayerStartingY;

            PG_Fill(graphics, BACKGROUND_COLOR);
            nextShape = GetRandomShape();
            DrawPreview(nextShape);

            // Draw the game boarder
            PG_DrawRectangle(graphics, GameOffsetX - 1,  GameOffsetY - 1, GameSizeX + GameOffsetX , GameOffsetY + GameSizeY, WALL_COLOR);
            PG_FillRectangle(graphics, GameSizeX + GameOffsetX + 1, PreviewSizeY + 2, GameSizeX - 1, GameSizeY - 1, WALL_COLOR);
            currentShape = GetRandomShape();

            TetrisGameState = RUNNING_GAME;
            lastShape.Name = NO_SHAPE;

            score = 0;
            linesCleared = 0;

            downBeat = StartingDownBeat;

            ResetArcade(true);
        }
        
        result.NextDelay = 50;

        break;

    case RUNNING_GAME:
    {
        // Move the shape left and right
        Direction_e currentDirection = GetDirection();
        Collision_e collision = DetectCollision(playerOffset, currentShape); 

        bool updateShape = false;

        if (currentDirection != lastDirection)
        {
            lastDirection = currentDirection;

            switch (currentDirection)
            {
            case LEFT:
                if (!ContainsFlag(collision, COLLISION_ON_LEFT))
                {
                    --playerOffset.X;
                    updateShape = true;
                }
                break;

            case RIGHT:
                if (!ContainsFlag(collision, COLLISION_ON_RIGHT))
                {
                    ++playerOffset.X;
                    updateShape = true;
                }
                break;

            case DOWN:
                while (!ContainsFlag(collision, COLLISION_ON_BOTTOM))
                {
                    ++playerOffset.Y;
                    collision = DetectCollision(playerOffset, currentShape);
                }
                downCount = downBeat - 1;
                updateShape = true;
                break;

            case UP:
            {
                // Clear the shape from the display to prevent collisions with self
                ClearShape(playerOffset, currentShape);

                // Save the current position incase the shape fails to rotate without
                // collision
                Shape_t fallbackShape = currentShape;
                Location_t fallbackLocation = playerOffset;
                RotateShape(&currentShape);

                while (!CheckFit(playerOffset, currentShape))
                {
                    collision = DetectCollision(playerOffset, currentShape);

                    if (ContainsFlag(collision, COLLISION_ON_BOTTOM) ||
                        collision == (COLLISION_ON_RIGHT || COLLISION_ON_LEFT))
                    {
                        // This can't be fixed revert rotation
                        currentShape = fallbackShape;
                        playerOffset = fallbackLocation;
                        break;
                    }
                    else if (ContainsFlag(collision, COLLISION_ON_LEFT))
                    {
                        ++playerOffset.X;
                    }
                    else if (ContainsFlag(collision, COLLISION_ON_RIGHT))
                    {
                        --playerOffset.X;
                    }
                }

                updateShape = true;
            }
            break;
            
            case NO_DIRECTION:
                break;
            }
        }

        // Move down if it is time
        if (++downCount > downBeat)
        {
            downCount = 0;

            if (ContainsFlag(collision, COLLISION_ON_BOTTOM))
            {
                // Check for a game over
                Location_t *shape = PlotShape(playerOffset, currentShape);

                for (int i = 0; i < POINTS_PER_SHAPE; ++i)
                {
                    Location_t point = shape[i];

                    if (point.Y < GameOffsetY)
                    {
                        PG_Fill(graphics, BACKGROUND_COLOR);
                        TetrisGameState = END_GAME;
                        GameoverLoop(true);
                    }
                }

                // If the game isn't over, prep a new shape
                if (TetrisGameState == RUNNING_GAME)
                {
                    playerOffset.Y = PlayerStartingY;
                    playerOffset.X = PlayerStartingX;

                    currentShape = nextShape;
                    nextShape = GetRandomShape();
                    DrawPreview(nextShape);
                    lastShape.Name = NO_SHAPE;

                    uint8_t lc = LineErase();

                    const uint8_t scores[] = {0, 1, 4, 9, 16};
                    score += scores[lc];
                    UpdateScoreBoard(score);

                    linesCleared += lc;
                    UpdateBonusBoard(linesCleared);
                    
                    // Increase the game speed every ten rows cleared
                    downBeat = StartingDownBeat - ((linesCleared / 10) * 20);

                    if (downBeat < 50)
                    {
                        downBeat = 50;
                    }
                }
            }
            else
            {
                ++playerOffset.Y;
                updateShape = true;
            }
        }

        // Update the shapes location if needed
        if (updateShape)
        {
            if (lastShape.Name != NO_SHAPE)
            {
                ClearShape(lastOffset, lastShape);
            }

            lastOffset = playerOffset;
            lastShape = currentShape;

            DrawShape(playerOffset, currentShape);
        }
    }
    break;
    case END_GAME:
    {
        result = GameoverLoop(false);

        if (result.MenuReturn == Exit) {
            TetrisGameState = PRE_GAME;
        }
    }
    break;
    }

    return result;
}

Menu_t tetrisMenu[] = {
    DEFINE_MENU_FUNCTION("Start", tetrisSetup, tetrisLoop),
    DEFINE_EMPTY_MENU(),
};
