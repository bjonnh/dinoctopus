#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include <Adafruit_TinyUSB.h>
#include <midi_Namespace.h>
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "uart_rx.pio.h"
#include "uart_tx.pio.h"
#include "midi_Defs.h"
#include "MIDI.h"
#include "piomidi.hpp"
#include "usbcablemidi.hpp"

#define LED 0
#define NEOPIXEL_PIN 0

#define MIDI_RX_PIN_1 14
#define MIDI_TX_PIN_1 16
#define MIDI_RX_PIN_2 12
#define MIDI_TX_PIN_2 13
#define MIDI_RX_PIN_3 10
#define MIDI_TX_PIN_3 11
#define MIDI_RX_PIN_4 8
#define MIDI_TX_PIN_4 9

#define LCD_CLOCK 1  // Clock (Common), sometimes called SCK or SCL
#define LCD_MOSI 2   // MOSI (common), sometimes called SDA or DATA
#define LCD_RESET 3   // LCD reset, sometimes called RST or RSTB
#define LCD_CS 4      // LCD CS, sometimes called EN or SS
#define LCD_RS 5      // LCD RS, sometimes called A0 or DC


int r0=0,r1=0,r2=0;
int g0=0,g1=0,g2=0;
int b0=0,b1=0,b2=0;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NEOPIXEL_PIN, NEO_GRB+ NEO_KHZ800);
U8G2_ST7567_JLX12864_1_4W_SW_SPI u8g2_lcd(U8G2_R0,
                                          LCD_CLOCK,
                                          LCD_MOSI,
                                          LCD_CS,
                                          LCD_RS,
                                          LCD_RESET); // clock, data, cs, dc, reset


void setColors() {
    r0+=5;
    if (r0>55) { r1+=5; r0=25; }
    if (r1>55) { g0+=5 ; r1=25; }
    if (g0>55) { g1+=5; g0=25; }
    if (g1>55) { b0+=5; g1=25; }
    if (b0>55) { b1+=5; b0=25; }
    if (b1>55) { b1=25; }
    strip.clear();
    strip.setPixelColor(0, strip.Color(g0, r0, b0));
    strip.setPixelColor(1, strip.Color(g1, r1, b1));
    strip.setPixelColor(2, strip.Color(155, 255, 150));
    strip.show();
}

void initStrip() {
    strip.begin();
    strip.setBrightness(255);
    strip.setPixelColor(0, strip.Color(15, 25, 0*25));
    strip.setPixelColor(1, strip.Color(15, 25, 0*25));
    strip.setPixelColor(2, strip.Color(155, 255, 0*255));
    strip.show();
}

char buffer[32];
int cycle = 0;


// Midi stuff

PIO pio_rx = pio0;
PIO pio_tx = pio1;
uint offset_rx = pio_add_program(pio_rx, &uart_rx_program);
uint offset_tx = pio_add_program(pio_tx, &uart_tx_program);

// USB MIDI object
Adafruit_USBD_MIDI usb_midi(4);
MIDI_NAMESPACE::UsbJackMIDI jack_0(usb_midi, 0);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::UsbJackMIDI> MIDI_USB_1((MIDI_NAMESPACE::UsbJackMIDI&)jack_0);

MIDI_NAMESPACE::UsbJackMIDI jack_1(usb_midi, 1);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::UsbJackMIDI> MIDI_USB_2((MIDI_NAMESPACE::UsbJackMIDI&)jack_1);

MIDI_NAMESPACE::UsbJackMIDI jack_2(usb_midi, 2);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::UsbJackMIDI> MIDI_USB_3((MIDI_NAMESPACE::UsbJackMIDI&)jack_2);

MIDI_NAMESPACE::UsbJackMIDI jack_3(usb_midi, 3);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::UsbJackMIDI> MIDI_USB_4((MIDI_NAMESPACE::UsbJackMIDI&)jack_3);

MIDI_NAMESPACE::PioMIDI pio_serial(pio_rx, pio_tx, offset_rx, offset_tx, 0, 0, MIDI_RX_PIN_1, MIDI_TX_PIN_1);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::PioMIDI> MIDI_IF_1((MIDI_NAMESPACE::PioMIDI&)pio_serial);

MIDI_NAMESPACE::PioMIDI pio_serial2(pio_rx, pio_tx, offset_rx, offset_tx, 1, 1, MIDI_RX_PIN_2, MIDI_TX_PIN_2);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::PioMIDI> MIDI_IF_2((MIDI_NAMESPACE::PioMIDI&)pio_serial2);

MIDI_NAMESPACE::PioMIDI pio_serial3(pio_rx, pio_tx, offset_rx, offset_tx, 2, 2, MIDI_RX_PIN_3, MIDI_TX_PIN_3);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::PioMIDI> MIDI_IF_3((MIDI_NAMESPACE::PioMIDI&)pio_serial3);

MIDI_NAMESPACE::PioMIDI pio_serial4(pio_rx, pio_tx, offset_rx, offset_tx, 3, 3, MIDI_RX_PIN_4, MIDI_TX_PIN_4);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::PioMIDI> MIDI_IF_4((MIDI_NAMESPACE::PioMIDI&)pio_serial4);

void setup() {
    //pinMode(LED, OUTPUT);
    /*initStrip();
    u8g2_lcd.begin();
    u8g2_lcd.setContrast(180);  // This is extremely important
    u8g2_lcd.clearBuffer();
    u8g2_lcd.firstPage();
*/
    // Set up the state machine we're going to use to receive them.
    /*pio_rx = pio0;
    pio_tx = pio1;
    uint offset_rx = pio_add_program(pio_rx, &uart_rx_program);
    uint offset_tx = pio_add_program(pio_tx, &uart_tx_program);
    uart_rx_program_init(pio_rx, sm_midi_port_0_rx, offset_rx, MIDI_RX_PIN_1, SERIAL_BAUD);
    uart_tx_program_init(pio_tx, sm_midi_port_0_tx, offset_tx, MIDI_TX_PIN_1, SERIAL_BAUD);*/

    TinyUSB_Device_Init(0);

    // Initialize MIDI, and listen to all MIDI channels
    // This will also call usb_midi's begin()
    MIDI_USB_1.begin(MIDI_CHANNEL_OMNI);
    MIDI_USB_2.begin(MIDI_CHANNEL_OMNI);
    MIDI_USB_3.begin(MIDI_CHANNEL_OMNI);
    MIDI_USB_4.begin(MIDI_CHANNEL_OMNI);

    // wait until device mounted
    while (!TinyUSBDevice.mounted())
        delay(1);

    MIDI_IF_1.begin();
    MIDI_IF_2.begin();
    MIDI_IF_3.begin();
    MIDI_IF_4.begin();
    //uart_tx_program_init(pio_tx, sm_midi_port_1_tx, offset_tx, MIDI_TX_PIN_2, SERIAL_BAUD);
    //uart_rx_program_init(pio_rx, sm_midi_port_1_rx, offset_rx, MIDI_RX_PIN_2, SERIAL_BAUD);
    /*u8g2_lcd.setFont(u8g2_font_6x12_tr);
    u8g2_lcd.setFontMode(0);
    u8g2_lcd.firstPage();
    do {
        //u8g2_lcd.drawHLine(0, 58, 128);
        //u8g2_lcd.drawStr(10, 10, "HELLO");
        //char c=42;
        //snprintf_P(buffer, sizeof(buffer), PSTR("%02x %04x"), c, cycle);
        //u8g2_lcd.drawStr(10, 10, buffer);
        u8g2_lcd.drawStr(0, 0, "Started");
    } while (u8g2_lcd.nextPage());*/
    //MIDI_IF_1.turnThruOff();
}


void loop() {
    if (MIDI_IF_2.read())
    {
        MIDI_IF_1.send(MIDI_IF_2.getType(),
                       MIDI_IF_2.getData1(),
                       MIDI_IF_2.getData2(),
                       MIDI_IF_2.getChannel());

        MIDI_USB_2.send(MIDI_IF_2.getType(),
                       MIDI_IF_2.getData1(),
                       MIDI_IF_2.getData2(),
                       MIDI_IF_2.getChannel());
    }

    if (MIDI_IF_1.read())
    {
        MIDI_IF_1.send(MIDI_IF_1.getType(),
                       MIDI_IF_1.getData1(),
                       MIDI_IF_1.getData2(),
                       MIDI_IF_1.getChannel());

        MIDI_USB_1.send(MIDI_IF_1.getType(),
                        MIDI_IF_1.getData1(),
                        MIDI_IF_1.getData2(),
                        MIDI_IF_1.getChannel());
    }

    if (MIDI_IF_3.read())
    {
        MIDI_IF_1.send(MIDI_IF_3.getType(),
                       MIDI_IF_3.getData1(),
                       MIDI_IF_3.getData2(),
                       MIDI_IF_3.getChannel());

        MIDI_USB_3.send(MIDI_IF_3.getType(),
                        MIDI_IF_3.getData1(),
                        MIDI_IF_3.getData2(),
                        MIDI_IF_3.getChannel());
    }

    if (MIDI_IF_4.read())
    {
        MIDI_IF_1.send(MIDI_IF_4.getType(),
                       MIDI_IF_4.getData1(),
                       MIDI_IF_4.getData2(),
                       MIDI_IF_4.getChannel());

        MIDI_USB_4.send(MIDI_IF_4.getType(),
                        MIDI_IF_4.getData1(),
                        MIDI_IF_4.getData2(),
                        MIDI_IF_4.getChannel());
    }

    if (MIDI_USB_1.read()) {
        uint16_t cable = jack_0.cable;
        midi::MidiInterface<midi::PioMIDI> *midiInterface;

        switch(cable){
            case 2: midiInterface = &MIDI_IF_2;break;
            case 3: midiInterface = &MIDI_IF_3;break;
            case 4: midiInterface = &MIDI_IF_4;break;
            default: midiInterface = &MIDI_IF_1;break;
        }


        midiInterface->send(MIDI_USB_1.getType(),
                           MIDI_USB_1.getData1(),
                           MIDI_USB_1.getData2(),
                           MIDI_USB_1.getChannel());
    }
    /*digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(100);               // wait for a second*/
    /*strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(255, 255, 255));
    strip.show();
    delay(1000);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.show();
    delay(1000);
    */

    //char c = uart_rx_program_getc(pio, sm_midi_port_0_rx);
    //char c2 = uart_rx_program_getc(pio_rx, sm_midi_port_0_rx);
    //uart_tx_program_putc(pio_tx, sm_midi_port_0_tx, c2);

    /*u8g2_lcd.firstPage();
    do {
        //u8g2_lcd.drawHLine(0, 58, 128);
        //u8g2_lcd.drawStr(10, 10, "HELLO");
        //char c=42;
        //snprintf_P(buffer, sizeof(buffer), PSTR("%02x %04x"), c, cycle);
        //u8g2_lcd.drawStr(10, 10, buffer);
        snprintf_P(buffer, sizeof(buffer), PSTR("%02x %04x"), c2, cycle);
        u8g2_lcd.drawStr(10, 20, buffer);
    } while (u8g2_lcd.nextPage());*/
    //cycle++;
}
