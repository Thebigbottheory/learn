

#define ENGINE     "NXTCAR"
#define MASTER     "CONTROL"
const tSensors TOUCH1=S1;
const tSensors TOUCH2=S2;
const tSensors TOUCH3=S3;

const TMailboxIDs Queue1 = 1;
const int kMaxMsgSize=10;

//
// checkLink()
// -- check if there is at least one connected
// -- if not, try 3times to reconnnect
// -- if it still fails after 3 attempts, stop all tasks
// -- return to caller
// --      if it is successful or already have one queue connected to a remote unit
// --
void checkLink()
{
  int nTries=0;
  setFriendlyName(MASTER);

  while (nTries<3 && nBTCurrentStreamIndex < 1)
  {
    PlaySound(soundLowBuzz);
    PlaySound(soundLowBuzz);
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "BT Not Connected!");
    nxtDisplayCenteredTextLine(4, "Connecting!");
    btConnect(1, ENGINE);
    wait1Msec(1000);
    ++nTries;
  }

  if (nBTCurrentStreamIndex < 1)// -- check if there is at least one connected
    StopAllTasks();   // exit the entire process

  while (bBTBusy)
  {
    wait1Msec(100);
  }

  nxtDisplayCenteredTextLine(3, "StreamIndex=%d", nBTCurrentStreamIndex);
  nxtDisplayCenteredTextLine(4, "Connect to Q %d", Queue1);
  wait1Msec(1000);
  return;
}

ubyte encodeByBits(){
  ubyte ret=(SensorValue[TOUCH1]<<2 | SensorValue[TOUCH2] <<1 | SensorValue[TOUCH3]);
  return ret;
}

ubyte encodeByAdd(){
  ubyte ret = 0;
  if(SensorValue[TOUCH1])
    ret+=100;
  if(SensorValue[TOUCH2])
    ret+=10;
  if(SensorValue[TOUCH3])
    ret+=1;
  return ret;
}

void sndOver(ubyte x)
{
  ubyte xmitBuffer[kMaxMsgSize];
  TFileIOResult nErrStatus;

  memset(xmitBuffer,0, kMaxMsgSize);
  nxtDisplayTextLine(3, "Attempt to send %d!", x);

  xmitBuffer[0] = x;
  // cCmdMessageWriteToBluetooth(x, 1, Queue1); <--- this works too if you really just want to send the byte x
  // the following prepare you the structure in case you want to send more than 1 byte
  nErrStatus = cCmdMessageWriteToBluetooth(xmitBuffer, 1, Queue1);

  switch (nErrStatus){
  case ioRsltSuccess:
  case ioRsltCommPending:
    nxtDisplayTextLine(3, "Send %d Success !", xmitBuffer);
    break;
  case ioRsltCommChannelBad:
    nxtDisplayString(3, "Bad Channel!....");
    break;
  default:
    nxtDisplayString(3, "Send Failed!....");
    break;
  }


}


task main()
{
  ubyte msg=0;
  SensorType[TOUCH1]=sensorTouch;
  SensorType[TOUCH2]=sensorTouch;
  SensorType[TOUCH3]=sensorTouch;

  checkLink();

  PlayTone(400,100);
  eraseDisplay();
  while(true){
    //do not attempt to do encoding until you know your bot
    //   is able to communicate and send the simple incrementing number over
    //   to the worker bot
    //msg=encodeByAdd();
    //msg=encodeByBits();
    msg = (msg<255 ?  ++msg : 0); //simply just send 1, 2, ... up to 255, then recycle
    nxtDisplayCenteredTextLine(3,"%d", msg);
    sndOver(msg);
    wait1Msec(20);
  }
}
