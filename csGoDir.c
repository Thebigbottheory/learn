#pragma platform(NXT)

/*==================================
csGoDir.c
Date: June, 2011
Synopsis:
- robot starts in any angle
- progrm the robot to go to a specific direction
*/
#define DIR_EPSILON 5
#define LM          motorA
#define RM          motorB
#define CW          0
#define CCW         1


#define GoForward(pwr)  { motor[RM] = pwr; motor[LM] = pwr;}
#define SharpRight(pwr)  { motor[RM] = -pwr; motor[LM] = pwr;}
#define SharpLeft(pwr)   { motor[RM] = pwr; motor[LM] = -pwr;}
#define WideRight(pwr, percent)  { motor[RM] = -pwr; motor[LM] = pwr*percent/100;}
#define WideLeft(pwr, percent)   { motor[RM] = pwr; motor[LM] = -pwr*percent/100;}
#define goBackward(pwr)  { motor[RM] = -pwr; motor[LM] = -pwr;}
#define StopM()  { motor[RM] = 0; motor[LM] = 0;}

typedef enum
{
    North, // 0
    East,  // 90 deg
    South, // 180 deg
    West   // 270 deg
} DIR;

string sDirs[] = { "North", "East", "South", "West" };
int nDirs[] = {0, 90, 180, 270};
int Origin = 0;
const tSensors kCompass = S1;

void reportLoc(int deg);
int  setTurn(int goal);
void optimizedGo(int goal);
TButtons waitForPressed();
void waitForReleased();

task main()
{
    SensorType[kCompass] = sensorI2CHiTechnicCompass;
    Origin = SensorValue[kCompass];

    nxtDisplayTextLine(0, "1-Place bot down");
    nxtDisplayTextLine(1, "  at stable loc...");
    nxtDisplayTextLine(2, "2-Press Enter.....");
    waitForPressed();
    waitForReleased();
    wait1Msec(1000);
    eraseDisplay();
    reportLoc(Origin);

    // simple Tests:
    // Test one direction at a time to ensure it is correct
    //    goToDirection(North);
    //    goToDirection(South);
    //    goToDirection(East);

    optimizedGo(nDirs[West]);
    eraseDisplay();
    nxtDisplayTextLine(2, "Point to %s", sDirs[West]);
    wait1Msec(5000);
}

TButtons waitForPressed()
{ TButtons btn;
  do {
    btn = nNxtButtonPressed;
    wait1Msec(5);     // just so that it won't overhaul the cpu
  } while(btn == -1);  // wait for button pressed
    return btn;
}

void waitForReleased()
{ TButtons btn;

  do {
    btn = nNxtButtonPressed;
    wait1Msec(5);     // just so that it won't overhaul the cpu
  }while(btn != -1);  // wait for button released
}

//---------------------------------------------
//  procedure for determining which way to turn

int setTurn(int goal)
{
    int cwDeg=0;
    int ccwDeg=0;

    cwDeg = goal - SensorValue[kCompass];

    nxtDisplayString(2, "Goal:%d", goal);
    nxtDisplayString(3, "Diff:%d", cwDeg);
    // -----
    if (cwDeg < 0)
        ccwDeg = 360 + cwDeg;
    else
        ccwDeg = 360 - cwDeg;
    /* or you can use complex boolean ? expression
    ctcl = (ct<0 ? 360+ct : 360-ct);
    */

    if ( abs(cwDeg) < abs(ccwDeg) )
    {
        if (cwDeg>0)
            return(CW);
        else
            return(CCW);
    }
    else
    {
        if (cwDeg>0)
            return(CCW);
        else
            return(CW);
    }

}


//---------------------------------------------
/* 1) get the closewise diff        (cl)
2) get the counterclosewise diff (ctcl)
3) use the smaller abs(diff)
*/
void optimizedGo(int goal)
{
    int diff=100;
    int turn;

    if (goal==-1)
        goal = Origin;

    StopM();
    nxtDisplayString(2, "Goal:%d", goal);

    while (diff>DIR_EPSILON)
    {
        turn = setTurn(goal);
        if (turn==CW)
            nxtDisplayString(4, "CLOCKWISE        !");

        //            SharpRight(50);
        else
            nxtDisplayString(4, "COUNTER CLOCKWISE!");
        //            SharpLeft(50);
        nxtDisplayString(7, "B:%5d; D:%5d", goal, (int)diff);
        diff = abs(goal - SensorValue[kCompass]);

    }
    StopM();

    return;
}


void reportLoc(int deg)
{
    nxtDisplayTextLine(1, "Start from %d", deg);
    /*
    switch (bearing)
    {
    case North:
    nxtDisplayString(0, "Go North");
    break;
    case West:
    nxtDisplayString(0, "Go West");
    break;
    case East:
    nxtDisplayString(0, "Go East");
    break;
    case South:
    nxtDisplayString(0, "Go South");
    break;
    }
    */
}
