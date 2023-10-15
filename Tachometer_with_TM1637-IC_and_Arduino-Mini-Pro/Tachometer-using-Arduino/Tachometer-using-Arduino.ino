/*
Arduino-based Tachometer using an Arduino-Mini-Pro with 4x7-LED-Display (incl. TM1637 control-IC)
------------------------
!!! Best +working example with Timer-1: https://quartzcomponents.com/blogs/electronics-projects/arduino-based-tachometer-rpm-meter-using-ir-sensor
Instructions to set Timers ⇒ https://wolles-elektronikkiste.de/timer-und-pwm-teil-2-16-bit-timer1
Calculate Timer Prescaler-values ⇒ http://evolutec.publicmsg.de/index.php?menu=software&content=prescalertools
Adapted on 06.09.2023 (nl):

Tachometer displays 4 states:
-----------------------------
1. Displays rpm-value (1-9999 rpm)
2. Displays "----" if machine is idle
3. Displays "ErrC" if Temperature-Sensor-Signal on Pin-D2 (input) is HIGH
4. Displays "HHHH" if rpm overflows the capability of the 4x7-digit LED-Display (>9999)

Arduino-Mini-Pro Pin-connections:
---------------------------------
D2 ... Frequency   Digital-In  from Hall-Sensor (! Important: with external 4,7k Pull-Up Resistance for Hall-S. with Open-Collector - to avoid "floating" signal!)
D3 ... CLK         Digital-Out (to TM1637-IC - I²C-communication, Pin-18)
D4 ... DIO         Digital-Out (to TM1637-IC - I²C-communication, Pin-17)
D5 ... "ErrC"      Digital-In  ("1" = Signal for Overtemperature, computed from external uContoller, there Pin-D2)
D6 ... "On"        Digital-Out ("1" = Signal from Hall-Sensor = "machine-active", feedback to external uContoller, there Pin-D3)
*/

//4x 7-Segment LED-Display with TM1637-IC:
#include <SevenSegmentTM1637.h>
const byte CLK = 3;     //define CLK pin (any digital pin) //Connection from Arduino-D3 to Display-Driver-IC TM1637
const byte DIO = 4;     //define DIO pin (any digital pin) //Connection from Arduino-D4 to Display-Driver-IC TM1637

SevenSegmentTM1637  display(CLK, DIO); //initialize the 4x 7-segm-LED-Display (0,56" height) //CLK = 4; DIO = 5;

unsigned long preset;	//timing-base for 16bit timer-1
float rpmflt;					//total time counting-up
unsigned int rpm;			//revolutions per minute
boolean tooslow = 1;	//if no signal is incoming, timer doesn't count-up

void setup() {
	Serial.begin(9600);

//Set Input-Pin register (Timer1 "INT0" on Port-Pin PD2 = Pin-D2):
	TCCR1A = 0;		//(re)set the 16bit Timer/Counter1 Control Register "TCCR1A" to 0 (Waveform = WGR10 to R/W ; WGR10 = Waveform Generation Mode, set bit identifier to 0)
	TCCR1B = 0;		//reset TCCR1B (Timer/Counter1 Control Register 1B )
	TCCR1B |= (1 << CS12);		//set Timer-1 mode CS12 (bit-2) as 256 prescaler (nl: set variable "preset" = 16MHz / 256 = 62500, see "rpmflt"-value below)
	TIMSK1 |= (1 << TOIE1);		//Enable timer1 overflow *interrupt* (TOIE1) by setting global interrupt *enable* bit in SREG
	pinMode(2, INPUT_PULLUP);	//Pin-D2 as digital Hall-Signal input. Grounded per hardware, so input isn't floating if no signal is incoming!
	pinMode(6, OUTPUT); digitalWrite(6, LOW); //set Pin-D6 as output, LOW on begin (supposing that on begin machine is in idle-mode, standing still)
	//Input-Pins don't need initialization

attachInterrupt(0, RevPM, FALLING); //attachs the function "RevPM", below

//LED-Display:
	display.begin();						//initializes the LED-Display
	display.setBacklight(100);	//sets the brightness to 100%
}

void RevPM() { //
	preset = TCNT1;	//timer1 Clock Select Bits / Prescaler (= prescaler, set to 65200 ms, see calculation below)
	TCNT1 = 0;			//"TCNT1" = Timer1 Counter Register counting-up from "0" (see https://forum.arduino.cc/t/ch-9-timer-counter-module-of-atmega328p-mcu/662590)	
	tooslow = 0;		//reset flag for normal use
}

ISR(TIMER1_OVF_vect) {	//set Timer1 ISR (InterruptServiceRoutine) to "Timer Overflow" (normal mode)
	tooslow = 1;					//if no-signal is incoming on Pin-D2, Timer1 doesn't count-up...
}

void loop() {

	if (tooslow == 1) {
		Serial.println("---");
		display.clear(); display.setCursor(0,0); //clear the Display and set the cursor to the 4th position (left)
		display.print("----");
		digitalWrite(6, LOW);  //Signal to external uController to indicate that machine is standing still (no Halls-Sensor-Signal coming-in)
	} else {
//	rpmflt = 120 / (rpmtime / 31250.00);	//nl: why? ⇒ 31250 = 62500/2 (in float units), using the preset-*base* of 16MHz/256 = 62500 Hz
		rpmflt =  (30 * 31250.00) / preset;		//15 instead of 60 seconds/minute - resp. 30 of 120, bec. of 4 Magnets on machine-axle! (15=60/4) - for rpm
		rpm = (round(rpmflt)); 								//rounding rpmflt = rpm without decimals (to use it as int)
		if (rpm >=9999) { rpm = 9999; }				//limit rpm to 9999 (don't overflow)
		digitalWrite(6, HIGH); //Signal to external uController to indicate that machine is running (= Hall-Sensor-Siganl coming-in)
		Serial.println(rpm);

		//Forward RpM to LED-Display (once per ½-second):
		display.clear(); //clear the display and show the actual RpM on LED-Display:
		if (rpm <10)									{ display.setCursor(0,3); display.print(rpm); } //right side
		if ((rpm <100) && (rpm >10))	{ display.setCursor(0,2); display.print(rpm); } //m-r. ""
		if ((rpm <1000) && (rpm >100)){ display.setCursor(0,1); display.print(rpm); } //m-l. ""
		if	 (rpm >=1000)							{ display.setCursor(0,0); display.print(rpm); } //left side
		if (rpm >=9999)								{ display.setCursor(0,0); display.print("HHHH"); } //if it overflows, don't go rambling, display "rpm too high"
		if (digitalRead(5) == HIGH)		{ display.setCursor(0,0); display.print("ErrC"); } //display overheat-warning if Input-Pin D5 is HIGH
	}

	delay(500); //renew display each ½-second

} //end of loop

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////
Flashing the chip (atmega328) directly = without bootloader (from "cmd" command-line):
avrdude.exe -v -V -p m328p -c usbasp -e -D -U flash:w:Tachometer-using-Arduino.ino.hex.hex:i

Fuses:                                                                             (0xfd: Brown-out-level @VCC = 2,7V)
avrdude.exe -v -V -p m328p -c usbasp -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xfd:m      orig: E:FD, H:DA, L:FF

//////////////////////////////////////////////////////////////////////////////////////
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/