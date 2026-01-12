
#include <time.h>

//sumobot class stuff here

//Assume A motor is the one on the left, B is the one on the right

//offset default
#define OFFSET 0


//enum for bot status

enum state{

    PRESTART,
    CALIBRATE,
    SCAN,
    ATTACK,
    REPOSITION,
    RESET,
    REAREND,
    


};


//this class should
//-initalize with all of the relevant pins
//is internally aware of all the stuff the bot is currently doing
//have member functions to do all the stuff, no pin calling in main ever


class Sumo{

private:
    
//motor driver pins
    int pwma; //must be analogue pin
    int ain1;
    int ain2;
    int pwmb; //must be analogue pin
    int bin1;
    int bin2;
    int stdby;
//ultrasonic pins
    int trig;
    int echo;
// IR sensor pins
    int color;
//impact button
    int button;



public:
    //status enumeration
    enum state status;

    //constructor
    Sumo(int PWMA, int AIN1, int AIN2, int PWMB, int BIN1, int BIN2, int STDBY, int TRIG, int ECHO, int COLOR, int BUTTON);

    //do i even need a destructor?
    ~Sumo();


    //simple get sensor functions- distance and color, are we moving/ are we spinning    
    int getDistance(); //returns distance in cm

    int getDistAvg(); //returns avg of 3 measurements
    
    bool isWhite(); //returns true if the sensor is reading white (Back up!)

    
    //motion functions

    void driveForward(int speed); //goes forward with a speed (0-255)

    //void driveForward(int speed, int distance); //goes forward at speed for a cm distance (use d/v for time), overloaded

    void driveBackward(int speed); //goes back at speed

    //void driveBackward(int speed, int distance); //speed and dist back up

    //maybe try to implement a distance version? needs calibration

    void turnLeft(int speed); //maybe fine a way to implement this and its sister function as degrees rather than speed?

    void turnRight(int speed);

    void spin180();
    
    void spin180fast();

    void spin90left();

    void spin90right();

    void spin90leftfast();

    void spin90rightfast();

    void brake(); //stops

    void scan(); //turns and scans distance till a guy is found

    void beep(int n); //makes beeps to indicate stuff


};









