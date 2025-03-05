
#include <usb.h>
#include "usb_process.h"
#include "interrupt_process.h"
#include "run_machine.h"
#include "log.h"
void USB_Process(void)
{
    if (true == State.bits.S_USB_PROCESS)
    {
		if (true == GetFileFromUSB())
		{
#ifdef SEND_LOG
            SendLogStr("PROCESS USB OK!!!\n");
#endif
			if (true == GetDataFromFile(&Charactor, &File))
			{
#ifdef SEND_LOG
                SendLogStr("S_WAIT_START\n");
#endif
				State.bits.S_WAIT_GET_BILLET = true;
                //M_State = S_WAIT;
				//State = S_WAIT;
                //RunInf.u16seqStep = 0;
			}
			else
			{
#ifdef SEND_LOG
                SendLogStr("FILE IS INVALID!!!\n");
#endif
			}
		}
		else
		{
#ifdef SEND_LOG
            SendLogStr("PROCESS USB FAIL!!!\n");
#endif
		}
        memset(&File, 0, sizeof(File));
        State.bits.S_USB_PROCESS = false;
    }
}