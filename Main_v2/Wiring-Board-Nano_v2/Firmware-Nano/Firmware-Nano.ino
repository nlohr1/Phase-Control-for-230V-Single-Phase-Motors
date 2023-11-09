//Firmware for Arduino-Mini (ATMega328) - PWM-Control for Brushed *DC*-Motor 325V
/*
Main features:
- PWM-generation with a 10k-Poti + Arduino-"analogRead() from Pin-A0 to PWM-Out on Pin-D13 (with about 125Hz),
- Integrated Overcurrent-Shutdown *within* the "Kemo-240" Phase-Control-Module, if motor is overloaded or shocked,
- Integrated also a Soft-Start for motors (up to ~3600 Watt), to avoid abrupt Motor + Load-Stress (16A max. fuses),
- Overtemperature shutdown with NTC (100kOhm-Sensor/Resistor, inbuilt/inside the motor) - if temperature exceeds 80°C,
- Direction of rotation switching: only possible if incoming Tachometer-Signal on Pin-D3 is "0" (=if machine stands still)

//Arduino-Nano Pin-connections: Input or Output?
//-----------------------------
⇒ All Pins refer to the common GND (+5V Low-Voltage-Supply: "1" = 5V, "0" = 0V)
A0 ... 10k-Poti    Analog-In coming from 10k-Poti (middle-pin), grounded with 100k
A1 ... Discharge   Digital-Out (= digital Pin-D15), grounded with 10k
A2 ... NTC         Analog-In   (= digital Pin-D16), attached to GND through 4,7k resistance
A3 ... Signal-LED  Digital-Out (= digital Pin-D17), grounded with 10k
D13 .. PWM         Digital-Out to phase-control module (Kemo-240), grounded with 10k (security)
D2     "ErrC"      Digital-Out "1"=5V, to indicate Overtemperature-Signal on Tachometer (= 4x 7bit-LED-Display)
D3....."On"        Digital-In from Tachometer (Hall-Sensor), if D3="1", then A1="1". To disconnect the 15k-Discharge-Resistor R9, deactivate the Optocoupler (with D3=1).
*/

//GLOBAL VARIABLES:
int val, vals = 0;  //Input from A0: value-portion of 2^10 = 1023, correlating to the analog Poti-position (vals = val*8)
unsigned long prevPulse; //timebase for PWM (=)
int ampere = 0;     //variable for analogRead(A1) ⇐ Hall-Sensor, measuring the Motor-Current (in Volt)
int amps = 0;       //variable to average = ampere per 0.1s
float corr = 1.0;   //val-multiplier for Power-readjustment
//byte count = 0;   //counter-variable (?)
int tempC = 0;      //Input from A2: variable for analogRead(A2) (with NTC-3950) on Pin-A2 / Input of Motor-Temperature
boolean errorC = 0; //variable to indicate that motor-temperature is too high
boolean error = 0;  //variable for blinking the Signal-LED on Pin-D17 (=A3)
unsigned long previous; //timebase for blinking Signal-LED (400ms)


//////////////////////////////////////////////////////////////////////////
void setup() {
  //Serial.begin(115200); //initializing the serial-COM - not needed here.

//Input-Pins: A0, A2 and D3 don't need initialization - this Input-Pins (and also some Output-Pins) here are grounded externally (10k and 100k), and additionally...
//...Arduino sets all "A"- and "D"-Pins on start or reset to floating-INPUTS. Only Output-A1 is inactivated to "+5V", because its goal is to discharge when m. in idle-mode.
//Input-D3, coming from Tacho: if Hall-Detector sends impulses (=machine running), D3=1 , else if m. stopped, D3=0). Initially machine stands still, so on begin D3 = 0.

//Output-Pins: A1, A3, D13 and D2 (same order as in the schematic):
  pinMode(A1, OUTPUT);  digitalWrite(A1, LOW);  //set Pin-A1 as Output for the discharge-control, to discharge charged capacitors while machine is idle, so initially LOW=connected.
  pinMode(A3, OUTPUT);  digitalWrite(A3, HIGH); //set Pin-A3 (D17) as Output for Signal-LED ⇒ Signal-LED shining if "machine ready" (if "On/Off"-Switch is "On") at setup() time.
  pinMode(13, OUTPUT);  digitalWrite(13, LOW);  //set Pin-D13 as Output for PWM-Control - with initial PWM-state "off" (for security).
  pinMode(2, OUTPUT);   digitalWrite(2, LOW);   //set Pin-D2 as Output for "ErrC"-Signal to Tachometer/LED-Display => if D2=1, Tach. displays "ErrC" and machine goes into stand-by.
}
//////////////////////////////////////////////////////////////////////////

//Switch-On/Off A1 = 0 (⇒ 230V main-line-Switch from 30A-Relay)

///////////////////////////////////////////////////////////////////////////////////////////////
//***************************************** Main-Loop *****************************************
void loop() {
//PWM-width: Input from 10k-Poti (analog-voltage on Pin-A0) ⇒ translated as PWM output on Pin-D13 (=PB5):
  val = analogRead(A0); //Voltage on Input-pin A0 (from 10k Poti middle-pin), "val" variable stores values betw. 0-1023 corresponding to actual poti-position.
  //Skip "mapping" analog-values (usually from 0 to 255), using the output-values 0-1023 directly from analogRead(0-1023 bits to µs) as PWM-timing-values:
  //Desired PWM-width: Input from voltage on Pin-A0 (10k-Poti), translated as PWM output on Pin-D13 in microseconds (µs):
  vals = map (val, 0,1023, 0,8160);       //"vals" = spreaded val*8 ⇒ greater accuracy for the PWM-timing ⇒ no need to use high PWM-frequencies (Kemo-240 PWM-Module: 10...10kHz),
  //add upper and lower "dead-band":      //adding security "dead-bands", to avoid glitches on both endpoints of the 10k-Poti, caused by evtl. not so stable +5V Power-Input, etc.
  if (vals < 96) { vals = 0; }            //⇒ Resulting bandwidth = "0" to 8160 µs, where "0" = 104 (= 8256 - (1023 *8)) = "deadband"-difference for "0"...
  if (vals > 8000) { vals = 8000; }       //...to be absolutely sure that also upper values (peaks + glitches over 5V?) don't not overflow the upper value (=8256).
  if (!errorC) {                          //If motor-temperature isn't too heigh [see "analogRead(A2)" + "tempC" below] - then go PWM:
    //PWM-Output on Pin-D13 using "Port-manipulation with BitMath": see https://wolles-elektronikkiste.de/en/logical-operations-and-port-manipulation
    if (micros() - prevPulse >= 8256 - (vals*0.7)) { //0.7 = 70% bec. Kemo-240 module uses only ~70% of PWM-bandwidth, so "max."-value is (vals*0.7) ⇒ upper deadband,
      PORTB |= (1 << PB5); }              //this sets output on D13 = HIGH, (Pin-D13 = PB5), "OR"ing PORTB with PB5 (=Pin-D13) sets D13 HIGH, but keeps all others unchanged,
    if (micros() - prevPulse >= 8160) {   //8256 = time in µs for duration of a complete PWM-cycle + 96µs (upper-)deadband,
      PORTB &= ~(1 << PB5);               //this sets output on D13 = LOW, (Pin-D13 = PB5), "AND"ing + Inverting PORTB with PB5 sets D13 LOW, keeping all others unchanged.
      prevPulse = micros();               //actualize "prevPulse"-variable each time "micros()" surpasses pwm-frequency (max. pulse-length-time = 8256 us ≙ ~120Hz).
    }
  }

//Discharge-Resistance: Pin-A1 (D15): if LOW, disconnects 15k/5W-Resistor if machine is in action, while connecting it if machine is in idle-state (discharging).
//"machine running/stopped" is dependent from incoming Tachometer-Signal on Input-D3 !! (see ⇒ modified Arduino-Mini-Tachometer ⇒ "Tachometer-with-4x7-Segm-LED-Display.sch")
//---------------------
  if (digitalRead(3) == LOW) { digitalWrite(A1, LOW); }  //if incoming  LOW on Pin-D3 (=machine STOPPED), set A1 to LOW, to activate Discharge-circuit ⇒ R9 connected
  else { digitalWrite(A1, HIGH); }                       //if incoming HIGH on Pin-D3 (=machine RUNNING), set A1 to HIGH, disconnecting R9 to protect it against overtemperature.


//Pin-A2 Analog-Input from NTC-Sensor:
//Shutdown if Motor-Temperature gets higher than 80°C: (NTC-3950 on Pin-A2 - "NTC" = NegativeTemperatureCoefficient Temp.-Sensor)
  tempC = analogRead(A2);         //Read the analog-voltage on Pin-A2 Input (NTC 100kOhm, see values from Thermistor-Table)
//Calculation for NTC min.Value @ 80°C:  (NTC = Negative Temperature Coefficient!, see "NTC-Widerstands-Schaltung.png" !)
//-------------------------------------
//25°C ⇒ 100k ⇒ OK     ⇒  x1 ≈ 0,22 V ... @ 25°C (from 5V total)   ⇒ calculation: "Widerstands-Teiler bei gegebenen Voltzahlen !.htm"
//80°C ⇒  12k ⇒ NOK    ⇒  x2 ≈ 3,59 V ... @ 80°C (min. value!)     ⇒  " " " "
//5,0 V ... 1023 bits   ; Y1 = 1023 * 0,22 / 5 = ≈ 45 bits @ 25°C
//3,59V ...    x bits   ; y2 = 1023 * 3,59 / 5 = ≈735 bits @ 80°C (=max. value)
//Input Pin-A2:
  if (tempC >= 735) {             //100k-NTC at 80°C ⇒ ~12kOhm, so if 4,7k (=lower-R) + 12k NTC (upper-R = NTC !), then tempC goes up over >3,59V and "errorC" becomes valid,
    errorC = 1;                   //if above 80°C, set variable to "1" and
    while(errorC) {               //if this condition is true ⇒ then HALT! and rest here within while-loop, until reset...
      digitalWrite(13, LOW);      //disable PWM-Out (set duty-cycle on Pin-D13 to "0" = PWM-output on Pin-D13 "LOW" = machine halt)
      digitalWrite(2, HIGH);      //Pin-D2 LOW-signal on output of Arduino-Nano (to show "ErrC" on LED-Display, forwarded to input on L.D.-Arduino-Mini-Pro, input Pin-D5)
      if (millis() - previous >= 400) { //Signal-LED:
        previous = millis();      //actualize "previous" variable each 400ms,
        if (error == 0) { error = 1; } //invert "error" (helper variable to blink Signal-LED)
        else error = 0;                // " " " "
        digitalWrite(A3, error);  //blinking Signal-LED on A3 = Pin-D17 - if temperature is too high (NTC lower than 12k if >80°C)
      }
    }
  } else { errorC = 0; }          //if temperature is normal (< 80°C)

}
//************************************* End of Main-Loop **************************************
///////////////////////////////////////////////////////////////////////////////////////////////


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////////////////////
Flashing the chip (atmega328) directly = without bootloader from "cmd" command-line: starting faster!
avrdude.exe -v -V -p m328p -c usbasp -e -D -U flash:w:Firmware-Nano.ino.hex:i

Fuses:                                                                             (0xfd: Brown-out-level @VCC = 2,7V)
avrdude.exe -v -V -p m328p -c usbasp -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xfd:m      orig: E:FD, H:DA, L:FF
/////////////////////////////////////////////////////////////////////////////////////////////////////

⇒ Result: "avrdude: writing flash (2462 bytes)"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
