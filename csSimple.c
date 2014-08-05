

typedef enum
{
	North, // 0
	East,  // 90 deg
	South, // 180 deg
	West   // 270 deg
} DIR;

string SDirs[] = { "North", "East", "South", "West" };

const tSensors kCompass = S1;

task main()
{ int dir;
  string str;
    SensorType[kCompass] = sensorI2CHiTechnicCompass;
    wait1Msec(20);
    while (1)
    {
      dir = SensorValue[kCompass];
      nxtDisplayBigTextLine(2, "%d", dir);
      if ( dir==0 )
          nxtDisplayCenteredTextLine(2, "%d:%s", dir, SDirs[North]);
      else if ( dir<90 )
      { sprintf(str,"%s-%s", SDirs[North], SDirs[East]);
        nxtDisplayCenteredTextLine(2, "%d:%s", dir, str);
      }
      else if (dir==90)
          nxtDisplayCenteredTextLine(2, "%d:%s", dir, SDirs[East]);
      else if ( dir<180)
      { sprintf(str,"%s-%s", SDirs[South], SDirs[East]);
        nxtDisplayCenteredTextLine(2, "%d:%s", dir, str);
      }
      //  finish the rest for your exercise
      // else if  ...
   }
}
