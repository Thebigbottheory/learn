#define BTPIN  "0000"
const TMailboxIDs MyQid = mailbox1;
const string TargetDevice="Dudeski";


//==================================
// bSearchDevice :
// parm:      const string &name:   The target device name
// return:    bool = true | false
//
bool bSearchDevice(const string &name)
{ bool found=false;
  int index=0;
  TMyContactsEntry contact;

  btSearch();
  ClearTimer(T1);

  while (time1[T1]<=30000 && (!found))
  {
    for (index=0; index<kMaxMyContacts  && (!found);++index)
    {
      memset(contact, 0, sizeof(contact));
      btGetMyContactsEntry(index, contact);
      if (contact.nDeviceStatus==btDeviceStatusSlotEmpty)
        wait1Msec(100); // No device
      else
      {
        nxtDisplayTextLine(5, "%d:See (%s)",index, contact.sFriendlyName);
        nxtDisplayTextLine(6, "   status:%d",contact.nDeviceStatus);
        wait10Msec(100);
        if (strcmp(contact.sFriendlyName, name)==0)
          found=true;
      }
    } // end of "for" loop allowing max times of attempts for seeking available devices
  }
  btStopSearch();
  return (found? true : false);
}
//================end of bSearchDevice =========================

//===================================================
bool btConn(string name)
{
  bBTHasProgressSounds = true;
  bBTSkipPswdPrompt = true;

  nxtDisplayCenteredTextLine(3, "Connecting %s", name);
  // BT must be turned on first
  if (!bBTVisble)
  {
    nxtDisplayCenteredTextLine(3, "BT Not Visible");
    StopAllTasks();
  }

  setSessionPIN(BTPIN);
  btConnect(MyQid, name);

  wait10Msec(10);
  ClearTimer(T1);

  while (time1[T1] < 30000)
  {
    if (nBTCurrentStreamIndex<0)
    {   wait10Msec(10);
      continue;
    }
    break;
  }
  if (nBTCurrentStreamIndex<0)
  {
    PlaySound(soundLowBuzz);
    PlaySound(soundLowBuzz);
    wait1Msec(100);
    return false;
  }
  nxtDisplayTextLine(7, "# of streams=%d", nBTCurrentStreamIndex);

  while (bBTBusy)
    wait1Msec(10);

  return true;  // An existing Bluetooth connection is present.
}

//=====================================================
//
// -  search
// -  connect
// -  maximum retry 3 times
bool getDevice(string name)
{ int nattempts = 0;

  while (++nattempts<3)
  {  if (!bSearchDevice(name))       //search for the device
    {   nxtDisplayCenteredTextLine(3, "No Device: %s", name);
      wait1Msec(1000);
      nxtDisplayCenteredTextLine(3, "Retry: %s", name);
      continue;
    }
  }
  eraseDisplay();
  nxtDisplayCenteredTextLine(3, "Found: %s", name);
  wait1Msec(200);

  if (!btConn(name))
  {   return false;
  }
  //  btDisconnect(1);
  //  cCmdBTPurgeRcvBuffer();
  wait10Msec(100);
  eraseDisplay();
  return true;
}
//======END OF getDevice()===============================================


//=====================================================

task main()
{
  getDevice(TargetDevice);
  while (1)
    wait10Msec(10);
}
