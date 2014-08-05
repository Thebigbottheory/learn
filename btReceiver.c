
const TMailboxIDs Queue1 = 1;

//
// checkLink()
// -- check if there is at least one connected
// -- if not, stop all tasks
// -- do not attempt to reconnect as this is supposed to be the worker bot, not master
// -- if already have one queue connected to a remote unit, return to caller
// --
void checkLink()
{
  while (nBTCurrentStreamIndex < 0)
  {
    PlaySound(soundLowBuzz);
    PlaySound(soundLowBuzz);

    nxtDisplayCenteredTextLine(3, "BT Not Connected!");
    nxtDisplayCenteredTextLine(4, "Connecting!");
    wait1Msec(2000);
    StopAllTasks();
  }

  while (bBTBusy)
    wait1Msec(100);

  if (nBTCurrentStreamIndex>0)
  {	nxtDisplayCenteredTextLine(3, "StreamIndex=%d", nBTCurrentStreamIndex);
    nxtDisplayCenteredTextLine(4, "Will talk to Q %d", Queue1);
    wait1Msec(100);
    return;
  }
}

//===================================================
// getMsg()
//     to read data from BT stream
//     if # of byte read <=0
//         warn the user
//     return the read in byte
//
ubyte getMsg()
{ byte msgSize=0;
  ubyte xmitByte=0;
  TFileIOResult nErrStatus;

   while (bBTBusy) //make sure the queue is not curently busy
  {
    wait1Msec(100);
  }


  while (msgSize<=0)
  {
    wait10Msec(1);
    msgSize = cCmdMessageGetSize(Queue1); // if msg size <=0, it means no message
    cCmdMessageRead(xmitByte, msgSize, Queue1);
  }

  nErrStatus = cCmdMessageWriteToBluetooth(xmitByte, msgSize, Queue1);

  switch (nErrStatus){
  case ioRsltSuccess:
  case ioRsltCommPending:
    nxtDisplayString(3, "Get %d...      ", xmitByte);
    break;
  case ioRsltCommChannelBad:
    nxtDisplayString(3, "Bad Channel!...");
    break;
  default:
    nxtDisplayString(3, "Send Failed!...");
    break;
  }
  return xmitByte;
}

void decodeMsg(ubyte x)
{
  // thisis where you will decode your message and decide how to move
  // the following is an incomplete sample  based on the encoding method used in the counter
  //     partner btSender.c :
  // bool touch1=false; ... create touch2 and touch3 as well
  // if x>1  touch2 was pushed, then set touch2=true
  // if x>10 touch1 was pushed, then set touch1=true
  // if x%100== 1  touch 3 was pushed
  // if  touch2
  //     if touch1
  //         left motor 80%
  //     else
  //         lett motor 0
  //     if touch3
  //         right motor 80%
  //     else
  //         right motor 0
  // else
  //      if

}

task main()
{
  ubyte msg=0;

  checkLink();
  PlayTone(400,100);
  eraseDisplay();
  while(true){
    msg=getMsg();
    // make sure you message comes across correctly before you attemtp
    // to decode and move.
    decodeMsg(msg);
    wait1Msec(20);
  }
}
