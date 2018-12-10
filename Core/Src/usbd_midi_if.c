/**
  ******************************************************************************
  * @file           : usbd_midi_if.c
  * @brief          :
  ******************************************************************************

    (CC at)2016 by D.F.Mac. @TripArts Music

  ******************************************************************************

    Modified by keshikan (www.keshikan.net) 2018
    The license is (CC BY 4.0), and takes over from original usbd_midi_if.h/c.

    See also original source code page.
    https://github.com/mimuz/mimuz-tuch/blob/master/STM32/

  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi_if.h"
#include "stm32f0xx_hal.h"


// basic midi rx/tx functions
static uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length);
static uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length);

#define MIDI_BUFFER_SIZ (512) //FIFO buffer byte size for midi message buffer

RingBufferU8 rbuf_usb_rx; //for input from USB


FUNC_STATUS midiInit()
{
	if(BUFFER_SUCCESS != cureRingBufferU8Init(&rbuf_usb_rx, MIDI_BUFFER_SIZ))
	{
		return FUNC_ERROR;
	}

	return FUNC_SUCCESS;
}

FUNC_STATUS midiGetFromUsbRx(uint8_t* dat) {
	if(BUFFER_SUCCESS != cureRingBufferU8Dequeue(&rbuf_usb_rx, dat))
	{
		return FUNC_ERROR;
	}

	return FUNC_SUCCESS;
}


bool isUsbRxBufEmpty()
{
	if( 0 != _cureRingBufferU8GetUsedSize(&rbuf_usb_rx) ){
		return false;
	}

	return true;
}


USBD_MIDI_ItfTypeDef USBD_Interface_fops_FS =
{
  MIDI_DataRx,
  MIDI_DataTx
};


// this callback is invoked, when new midi data arrives
static uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length){
  uint16_t cnt;
  uint16_t msgs = length / 4;
  uint16_t chk = length % 4;
  uint8_t u8b;
  uint8_t midi_size;

  if(0 != chk)
  {
	  return 0;
  }

  for(uint32_t cnt_msgs = 0; cnt_msgs < msgs; cnt_msgs++){
	  uint8_t code_idx_num = msg[0 + 4*cnt_msgs] & 0x0F;

	  switch (code_idx_num) {

	  	  //not defined
		  case 0x0:
		  case 0x1:
			  midi_size = 0;
			  break;

		  //1byte message
		  case 0x5:
		  case 0xF:
			  midi_size = 1;
			  break;

		  //2byte message
		  case 0x2:
		  case 0x6:
		  case 0xC:
		  case 0xD:
			  midi_size = 2;
			  break;

		  //3byte message
		  case 0x3:
		  case 0x4:
		  case 0x7:
		  case 0x8:
		  case 0x9:
		  case 0xA:
		  case 0xB:
		  case 0xE:
			  midi_size = 3;
			  break;

		  default:
			  midi_size = 0;
			  break;
	  }

	  for(cnt = 0;cnt < midi_size;cnt ++){
		  u8b = *(msg + 4*cnt_msgs + cnt + 1);
		  cureRingBufferU8Enqueue(&rbuf_usb_rx, &u8b);
	  }
  }

  return 0;
}

void sendMidiMessage(uint8_t *msg, uint16_t size){
  if(size == 4){
		if (APP_RX_DATA_SIZE < APP_Rx_ptr_in + 4) {
			USBD_MIDI_SendPacket();
			while(USB_Tx_State == 1);
		}

    MIDI_DataTx(msg, size);
  }
}

static uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length){
  uint32_t i = 0;
  while (i < length) {
    APP_Rx_Buffer[APP_Rx_ptr_in] = *(msg + i);
    APP_Rx_ptr_in++;
    i++;
    if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) {
      APP_Rx_ptr_in = 0;
    }
  }
  return USBD_OK;
}
