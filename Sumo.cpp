#include <Sumo.h>
#include <Arduino.h>


Sumo::Sumo(int PWMA, int AIN1, int AIN2, int PWMB, int BIN1, int BIN2, int STDBY, int TRIG, int ECHO, int COLOR, int BUTTON)
{
    

    //transfer all the pins over
    pwma = PWMA;
    ain1 = AIN1;
    ain2 = AIN2; 
    pwmb = PWMB;
    bin1 = BIN1;
    bin2 = BIN2;
    stdby = STDBY;
    trig = TRIG;
    echo = ECHO;
    color = COLOR;
    button = BUTTON;

    status = PRESTART;

    //set all the pin modes

    pinMode(pwma, OUTPUT);
    pinMode(ain1, OUTPUT);
    pinMode(ain2, OUTPUT); 

    pinMode(pwmb, OUTPUT);
    pinMode(bin1, OUTPUT);
    pinMode(bin2, OUTPUT); 

    pinMode(stdby, OUTPUT);

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(color, INPUT);

    //set initial states for the outputs
    analogWrite(pwma, 0);
    analogWrite(pwmb, 0);

    digitalWrite(ain1, LOW);
    digitalWrite(ain2, LOW);
    digitalWrite(bin1, LOW);
    digitalWrite(bin2, LOW);
    
    digitalWrite(stdby, 0);
    
    digitalWrite(trig, 0);

}

Sumo::~Sumo()
{
    //no need to delete anything, theres nothing on the heap
}

int Sumo::getDistance()
{
    //get the distance, return it
    //Serial.println("reading distance");

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float duration = pulseIn(echo, HIGH);
    float distance = (duration/2) / 29.1;
    //Serial.println(distance);
    if(distance>800)
    {return 0;}
    return distance;
}

int Sumo::getDistAvg()
{
    int dist = 0;
    for(int i =0;i<3;i++)
    {
        dist += getDistance();
        delay(5);
    }
    dist= dist/3;
    Serial.println(dist);
    return dist;
}   

bool Sumo::isWhite()
{
    //read the ir sensor and return true if it reads white
    Serial.println("reading color");
    
    if(digitalRead(color))
    {
        Serial.println("black");
        return false;
    }
    else{
        Serial.println("white");
        return true;
    }
    
}

void Sumo::driveForward(int speed)
{

    if(speed > 255)
    {
        speed = 255-OFFSET;
    }

    Serial.println("driving forward");
    //set left motor CW (H,L)
    digitalWrite(ain1, HIGH);
    digitalWrite(ain2, LOW);

    //set right motor to CW
    digitalWrite(bin1, HIGH);
    digitalWrite(bin2, LOW);

    //set speeds

    //if its above like, 50, first turn to 50 for a little bit, then turn up
  /*   analogWrite(pwmb, 0);
    analogWrite(pwma, speed+OFFSET);
    //finally set up standby
    digitalWrite(stdby, HIGH);
    delay(40);
    analogWrite(pwmb, speed); */
    analogWrite(pwma, speed+OFFSET);
    analogWrite(pwmb, speed);
    digitalWrite(stdby, HIGH);
}

void Sumo::driveBackward(int speed)
{
    if(speed > 255)
    {
        speed = 255-OFFSET;
    }

    Serial.println("driving backwards");
    //set left motor CCW (L,H)
    digitalWrite(ain1, LOW);
    digitalWrite(ain2, HIGH);

    //set right motor to CCW 
    digitalWrite(bin1, LOW);
    digitalWrite(bin2, HIGH);

    //set speeds
    analogWrite(pwma, speed);
    analogWrite(pwmb, speed+2);

    //finally set up standby
    digitalWrite(stdby, HIGH);


}

void Sumo::turnRight(int speed)
{   
    if(speed > 255)
    {
        speed = 255-OFFSET;
    }

    Serial.println("Turning right");
    

    //set both motors to CW, CCW
    digitalWrite(ain1, HIGH);
    digitalWrite(ain2, LOW);
    analogWrite(pwma, speed+OFFSET);

    digitalWrite(bin1, LOW);
    digitalWrite(bin2, HIGH);
    analogWrite(pwmb, speed);

    //set standby
    digitalWrite(stdby, HIGH);

}

void Sumo::turnLeft(int speed)
{

    if(speed > 255)
    {
        speed = 255-OFFSET;
    }

    Serial.println("turning left");
    

    //set to CCW, CW
    digitalWrite(ain1, LOW);
    digitalWrite(ain2, HIGH);
    analogWrite(pwma, speed+OFFSET);

    digitalWrite(bin1, HIGH);
    digitalWrite(bin2, LOW);
    analogWrite(pwmb, speed);

    //set standby
    digitalWrite(stdby, HIGH);



}

void Sumo::brake()
{
    Serial.println("BRAKING");

    //first turn off standby
    digitalWrite(stdby, LOW);

    //then set all the other stuff to off
    digitalWrite(ain1, LOW);
    digitalWrite(ain2, LOW);
    digitalWrite(bin1, LOW);
    digitalWrite(bin2, LOW);

    analogWrite(pwmb, 0);
    delay(50);
    analogWrite(pwma, 0);

}

void Sumo::spin180()
{
    turnRight(100);
    delay(260);
    brake();
}

void Sumo::spin90right()
{
    turnRight(100);
    delay(117);
    brake();
}

void Sumo::spin90left()
{
    turnLeft(100);
    delay(115);
    brake();   
}

void Sumo::spin90rightfast()
{
    turnRight(250);
    delay(43);
    brake();
}

void Sumo::spin90leftfast()
{
    turnLeft(250);
    delay(41);
    brake();
}

void Sumo::spin180fast()
{
    turnRight(250);
    delay(55);
    brake();
}

void Sumo::beep(int n)
{
   for(int i =0; i< n; i++)
   {
        
        digitalWrite(ain1, LOW);
        digitalWrite(ain2, HIGH);
        digitalWrite(bin1, LOW);
        digitalWrite(bin2, HIGH);

        //set speeds
        analogWrite(pwma, 4);
        analogWrite(pwmb, 4);

        //finally set up standby
        digitalWrite(stdby, HIGH);
        delay(100);
        brake();
        delay(250);
   }
}

void Sumo::scan()
{
    //rotate and scan, stop when u get a reading
    //or stop if its been more than 2 seconds
    unsigned long t = millis()+2000;
    turnLeft(40);
    while(status == SCAN)
    {
        int dist = getDistance();
        //do u see something?STOP
        if(dist>2 && dist <50)
        {
            //delay(15);
            brake();
            //driveForward(100);
            if(status == SCAN)
            {status = ATTACK;}  
            break;
        }
        //has it been too long? move somewhere new
        if(millis() > t)
        {   
            brake();
            if(status == SCAN)
            {status = REPOSITION;}
            break;
        }
        delay(25);
    }
    
    

}

