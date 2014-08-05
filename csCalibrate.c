/*
Calibrate HiTechnic Compass Sensor
Operation:
    - Left Arrow Button - send command to put the compass in Calibration Mode
    - Right Arrow Button - send command to put the compass in Read Mode

Special NoteA:
- sensor hardware address : 0x02
- Should consult the memory model from the HiTechnic
- for calibration mode:
-    set command register = 0x41
-    set compass mode = 0x43;
- for read mode:
-    set command register = 0x42
-    set compass mode = 0x0;

*/

const tSensors CS=S3;
typedef struct{
    byte nMsgSize;
    byte nDeviceAddress;
    byte nLocationPtr;
    byte nCompassMode;
} TI2C_Output;

TButtons waitForPressed();
void waitForReleased();

task main()
{
  int hUpper, hLower, heading;
  TButtons nBtn;

  SensorType[CS] = sensorI2CCustom;
  wait10Msec(5);
  nI2CBytesReady[CS]=0;
  while(nI2CStatus[CS]== STAT_COMM_PENDING)
  wait1Msec(2);

  byte replyMessage[6];

  TI2C_Output sOutput;

  while(true)
  {
    nxtDisplayTextLine(1, "Mode = Read");
    nI2CBytesReady[CS] = 0;
    sOutput.nMsgSize = 2;                  //read the value from the sensor
    sOutput.nDeviceAddress = 0x02;
    sOutput.nLocationPtr = 0x42;
    sendI2CMsg(CS, sOutput.nMsgSize, 6);

    while (nI2CStatus[CS] == STAT_COMM_PENDING)
      wait1Msec(2);

    if (nI2CStatus[CS] == NO_ERR)
    {
      readI2CReply(CS, replyMessage[0], 6);            //get the compass heading
      hUpper = (replyMessage[0] & 0x00FF);
      hLower = (replyMessage[1] & 0x00FF);
      heading = (hUpper << 1) | hLower ;                //convert contents of 0x44 0x45

      nxtDisplayTextLine(4, "U-val %d", hUpper);
      nxtDisplayTextLine(5, "L-val %d", hLower);
      nxtDisplayTextLine(6, "Heading = %d", heading);

    }
    else
    {
      nxtDisplayTextLine(1, "i2c err %d", nI2CStatus[CS]);
    }

  //check if a button has been pressed.

  nBtn = nNxtButtonPressed; // check for button press

     switch (nBtn)
     {
        case kLeftButton:
           {
              PlayTone(440,1000);
              sOutput.nMsgSize = 3;
              sOutput.nDeviceAddress = 0x02;
              sOutput.nLocationPtr = 0x41;               //Calibration mode command
              sOutput.nCompassMode = 0x43;

              nI2CBytesReady[CS] = 0;
               sendI2CMsg(CS, sOutput.nMsgSize, 0);
               wait1Msec(40);
               while ((nBtn = nNxtButtonPressed) != kRightButton)
                 {                        //wait for the right button to be pressed before leaving cal mode;
                    nxtDisplayTextLine(1, "Mode = Cal ");
                    wait1Msec(1000);
                    PlaySound(soundBlip);
                 }
              break;
           }
         case kRightButton:
            {
               PlaySound(soundBeepBeep);

               sOutput.nMsgSize = 3;
              sOutput.nDeviceAddress = 0x02;
              sOutput.nLocationPtr = 0x41;
               sOutput.nCompassMode   = 0x00;                  //Read mode command
              nI2CBytesReady[CS] = 0;
               sendI2CMsg(CS, sOutput.nMsgSize, 0);
               wait1Msec(1250);                                    //wait for the cal info to be saved
               break;
           }
      }

  }
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
