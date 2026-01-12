#include <Arduino.h>

#include <Sumo.h>


//slot in the pin assignments- A is left motor, B is right motor. 1 is front, 2 is back 
#define PWMA 10
#define AIN1 5 
#define AIN2 6
#define PWMB 11
#define BIN1 4
#define BIN2 A1
#define STDBY A0
#define TRIG 12
#define ECHO 13
#define COLOR 2
#define BUTTON 3
int go =1;



//create bot object, class does the rest of the initialization
Sumo bot = Sumo(PWMA, AIN1, AIN2, PWMB, BIN1, BIN2, STDBY, TRIG, ECHO, COLOR, BUTTON);



//function prototypes here
void ifWhite()
{ 
  //if the scanner reads white, immediately stop and tell the program to reverse
 
  Serial.println("IR interrupt");
  //detachInterrupt(digitalPinToInterrupt(COLOR));
  bot.status = RESET;
  return;
}

void ifbutton()
{
  bot.brake();
  Serial.println("BUTTON interrupt");
  bot.status = REAREND;
  return;
  
}



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  //interrupt set up

  attachInterrupt(digitalPinToInterrupt(COLOR), ifWhite, FALLING);
  
  //attachInterrupt(digitalPinToInterrupt(BUTTON), ifbutton, HIGH);

  bot.status = PRESTART;

}

void loop() {
  // put your main code here, to run repeatedly:

  
  switch(bot.status)
  {


  //isolated state to test stuff without messing with the rest of the program 
 
  case CALIBRATE:
    //Serial.println("calibrate");
    { 
      bot.driveForward(135);
      delay(1000);
      bot.brake();
      delay(1000);
      
      

    break;
    }
  
  case PRESTART:
    {
    Serial.println("prestart");
    delay(5000);
    //bot.beep(3);
    if(bot.status == PRESTART)
      {bot.status = SCAN;}
    break;
    }


  case SCAN:
    {
    Serial.println("scan");
    bot.scan();
    break;
    }

  case REPOSITION:
    {
    Serial.println("reposition");
    bot.driveForward(100);
    //drive forward for a second, but interruptable
    unsigned int t = millis()+500;
    while(millis()<t)
    {
    }

    bot.brake();
    if(bot.status == REPOSITION)
      {bot.status = SCAN;}
    break;
    }

  case ATTACK:
    {
    Serial.println("attack");
    /* int distprev = 100;
    int distnext = 99; */
    bot.driveForward(200);
    break;
    //if the loop broke, and status is still attack(no intterupt), then scan again
  

    }


  case REAREND:
    {
    //if ur being rear ended, back up until the button is unpressed
      while(digitalRead(BUTTON))
      {
        bot.driveBackward(100);
      }
      delay(500);
      bot.brake();
      if(bot.status == REAREND)
      {
        bot.status = SCAN;
      }
      break;
    }
    

  case RESET:
    {
    Serial.println("reset");
    bot.driveBackward(150);
    delay(500);
    bot.brake();
    bot.spin180();
    bot.driveForward(100);
    delay(250);
    
    if(bot.status == RESET)
    {bot.status = SCAN;}
    
      break;
    }

  default:
    {
    break;
    }
  }
  
  


}

