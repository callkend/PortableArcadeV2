#include <stdbool.h>

void UpdateScoreBoard(unsigned int newScore, char* scoreBoard )
    {
        const char BCDToSeg[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
        const unsigned int Decades[] = {1000,100,10,1};
        
        for(char i=0;i<4;i++)
        {
          char d = 0;
          while (newScore >= Decades[i])
          {
            ++d;
            newScore -= Decades[i];
          }
          *scoreBoard++ = BCDToSeg[d];
        }
    }

typedef union UserInputs
{
  struct
  {
    bool joyLeft:1;
    bool joyRight:1;
    bool joyUp:1;
    bool joyDown:1;
    bool button1:1;
    bool button2:1;
    bool button3:1;
    bool button4:1;
  };
  struct 
  {
    unsigned char allBits;
  };
};

void ReadUserInputs()
{
  
}