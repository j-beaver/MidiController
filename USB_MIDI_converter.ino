/*
 *******************************************************************************
 * USB-MIDI to Legacy Serial MIDI converter
 * Copyright (C) 2012-2017 Yuuichi Akagawa
 *
 * Idea from LPK25 USB-MIDI to Serial MIDI converter
 *   by Collin Cunningham - makezine.com, narbotic.com
 *
 * This is sample program. Do not expect perfect behavior.
 *******************************************************************************
 */

#include <usbh_midi.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#else
#define _MIDI_SERIAL_PORT Serial
#endif
//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USB Usb;
USBH_MIDI  Midi(&Usb);

void MIDI_poll();
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime);

void setup()
{
  _MIDI_SERIAL_PORT.begin(31250);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  Usb.Task();
  uint32_t t1 = (uint32_t)micros();
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();
  }
  //delay(1ms)
  doDelay(t1, (uint32_t)micros(), 1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  uint8_t outBuf[ 3 ];
  uint8_t size;
  uint8_t send_flag = 0;

  do {
    if ( (size = Midi.RecvData(outBuf)) > 0 && outBuf[ 0 ] == 176) {
      //MIDI Output
      switch (outBuf[ 1 ]){
        case 2:
          outBuf[ 1 ] = 8;
          send_flag = 1;
          break;
        case 3:
          outBuf[ 1 ] = 17;
          send_flag = 1;
          break;
        case 4:
          outBuf[ 1 ] = 18;
          send_flag = 1;
          break;
        case 5:
          outBuf[ 1 ] = 15;
          send_flag = 1;
          break;
        case 6:
          outBuf[ 1 ] = 16;
          send_flag = 1;
          break;
        case 7:
          outBuf[ 1 ] = 31;
          send_flag = 1;
          break;

        case 8:
          outBuf[ 1 ] = 0;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 9:
          outBuf[ 1 ] = 21;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 10:
          outBuf[ 1 ] = 8;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 11:
          outBuf[ 1 ] = 17;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 12:
          outBuf[ 1 ] = 18;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 13:
          outBuf[ 1 ] = 15;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 14:
          outBuf[ 1 ] = 16;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;
        case 15:
          outBuf[ 1 ] = 31;
          outBuf[ 0 ] += 2;
          send_flag = 1;
          break;

        case 24:
          outBuf[ 1 ] = 0;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 25:
          outBuf[ 1 ] = 21;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 26:
          outBuf[ 1 ] = 8;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 27:
          outBuf[ 1 ] = 17;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 28:
          outBuf[ 1 ] = 18;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 29:
          outBuf[ 1 ] = 15;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 30:
          outBuf[ 1 ] = 16;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
        case 31:
          outBuf[ 1 ] = 31;
          outBuf[ 0 ] += 1;
          send_flag = 1;
          break;
      }
      if  (send_flag == 1) {    
        _MIDI_SERIAL_PORT.write(outBuf, size);
        send_flag = 0;      
      }
    }
  } while (size > 0);
}

// Delay time (max 16383 us)
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime)
{
  uint32_t t3;

  if ( t1 > t2 ) {
    t3 = (0xFFFFFFFF - t1 + t2);
  } else {
    t3 = t2 - t1;
  }

  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}
