# Phase-Control-for-230V-Single-Phase-Motors
Control for direction-switchable 230V *direct current* (DC)-motors, whose speed (rpm) can be controlled using a Potentiometer *or*  
using a PWM (Pulse Width Modulation) in the low voltage range, generated here by a microController.
![325V-DC-Motor-on-Phase-Control_Principle-Diagram](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/1215f584-f1fe-4574-99d1-6883fae35f82)
Priciple of a Poti-Controlled Phase-Control for 230VAC (325VDC) Motors.  
The picture above shows a basic diagram for the electronic control of a 230V **DC** motor, e.g. a hobby lathe, connected to a **single-phase** (230V) **AC**-current network.

Phase control circuits are often used for simple speed controls, here a P.C. with Bridge-Rectifier for reversible 230V DC electric motors,
which are often used in medium power hobby machines (up to 10 amps ≙ approx. 2300 watts), such as lathes and milling benches.
![3D-Steuerung-für-Bürsten-Gleichstrom-Motor-325V_Nano](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/25a48b36-aa1e-4cca-b5c9-efbb3eb60a19)
Since electronic controls of 230V *DC* motors in today's hobby machines (such as lathes, milling benches, etc.) often are built with
inexpensive components that are not protected against surges or current peaks – some electronic-failures are inevitable.
So I decided to use a circuit that works a little more reliable, with electronic components and *modules* that are readily available
on the market *and* at the same time are better protected against overloads, incorrect operations and engine over-temperatures.
All 230VDC (direct current!) brushed motors with up-to 2300 Watts can be controlled with this phase control board. It allows to control
the speed of the motor (0-100%) as well as the motor direction of rotation.

## Safety features
This circuit has also some essential safety-functions:
- The main module "Kemo-M240" used here contains an integrated soft start, needed to start up larger motors,
   to avoid the tripping of standard 16A household fuses
- This module also contains an automatic overload or anti-lock shutdown with electronic reset
- If the used Motor contains a built-in NTC-thermistor (with 100kOhm), the µController can provide a protection against
   engine overheatings. If the motor-temperature exceeds 80°C, the µC goes into an endless loop, and reactivating PWM
   is only possible by switching off (main-power) or by pushing the reset-button of the µC.

!!!: Protection against direction of rotation switching ***while driving*** is NOT provided here: Due to this, and if so an "EMF short circuit" occurs,
the "Kemo-M240" controller shots the power-control off in a fractions of a second and thus at least protects the mechanics of the machine.
The electronics then can only be reactivated after a complete restart - either per µContoller-Reset or after Main-Power Shutdown.

Disclaimer:
!!! It goes without saying that you reproduce this circuit at your own risk and responsibility!!!
Anyone who does not have sufficient knowledge of how to handle *dangerous* 230VAC currents or 325VDC(ss) voltages(!):
If you are not familiar with electrical or electronic components, you should either hire a specialist to do it for you
or stay away from it!
“Experiments” without special knowledge and appropriate protection(s) on high voltages can quickly end in death
following an electric shock! And sudden cardiac-arrest can occur even days after an electric shock! - if then
no immediate help is nearby...
In any case, a replica of this project is done expressly and explicitly *at your own risk*!

**Further notes:**
- Three-phase motor activations are hereby excluded due to the principle! This circuit only controls single phase Engines!
- The circuit also works with (simpler) *AC* single-phase asynchronous motors - omitting the bridge rectifier used here.
   However, a connected *AC* motor then is no longer direction-reversible.
- It is NOT suitable for capacitor motors! - "Capacitor starting motors" are often only used for simple purposes and
   built + used for on/off operation only (such as for smaller hobby drill presses) - and are not speed-regulated. But
   it depends specifically on the type of engine, some capacitor motors can still be regulated (speed-limited).
   However, any weak to poor efficiency that occurs should be taken into account (including heat losses).

Factual suggestions for improvements and changes are always welcome!

Main Schematic / Wiring Connections:
![Phasenanschnittsteuerung-f-325V-Gleichstrom-Motor_Übersicht_DE](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/blob/main/Phasenanschnittsteuerung-f-325V-Gleichstrom-Motor_%C3%9Cbersicht.png)

## Principle description:
The control works according to the 230VAC phase control principle (similar to a lamp dimmer), but in contrast to this
it can neutralize EMF (electromotive forces) of a greater motor and is determined by special-selections of electronic components.

A reliable control-module is used to control the motor, controlled via PWM (Pulse Width Modulation).
The micro-controller “Arduino-Nano” used here generates needed PWM and (if present) this µC also evaluates an NTC-Sensor
installed within the Motor, to check a possible overtemperature.
The Speedometer (RPM display on a LED display) does not run - as usually *independently* of the main phase-control, 
but sends back a "busy" signal to the µController, which therefore "knows" when the motor is running or not
(=when it is standing still), in order to ensure active discharge of the triac snubber and the EMI or EMC-filters charged
through supplied voltages + motor back EMF, to avoid uncontrolled an jerky starting of the DC-motor after changing the direction of rotation!

The control consists of several modules chained(wired) one behind the other:

1. the 10AT fuse and the on/off switch with latching contactor,
2. the (mandatory) safety switches or other 230V safety microswitches on a specific machine,
3. the upstream EMC filter (= electromagnetic compatibility), here called EMI filter (to avoid E.M. interferences),
4. the actual (triac) phase control, using *either* a potentiometer *or* a µController with PWM-output (Pulse-Width Modulation),
5. in parallel the 5V low-voltage circuit to generate the PWM for the phase control, using an "Arduino-Nano" module with
   additional software-side monitoring of eventual motor-over-temperature per NTC-sensor,
6. the 15A/1500V bridge rectifier,
7. the reversing switch, also called “pole inverter” or direction of rotation switch,
8. a second EMI (carbon brush fire) suppression filter + a second 10AT motor fuse, and
9. at last the 325V DC (=direct current) motor.

## The Wiring Board:
The only part that still needs to be soldered together + programmed is the “wiring board”. It serves to wire/connect the Low-Voltage
PWM-Signal to the main phase control module "Kemo-M240" and other low-voltage connections as LEDs and Sensors.  
This wiring board also contains the Arduino Nano µController as the heart of the circuit, some passive components (resistors + capacitors) and
the JST-Connectors for the NTC-Sensor, the Signal-LED and the Speedometer (rpm display), plus the Discharge Circuit for both Discharge Resistors,
to avoid uncontrolled starting impulses and a jerk starting of the motor, while switching it to reverse-direction.
![Wiring-Board-Nano_sch](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/5f90a135-2036-4bc7-9c50-fc07217dd82b)
*Circuit diagram of the low-voltage circuit "wiring board" with the Arduino Nano*  
![Wiring-Board-Nano_3D](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/70447996-00d2-4e98-be32-b3b7b477d32b)

The “wiring board” with the Arduino Nano is provided as schematic and as PCB-layout in Eagle format - which can
easily be converted into other formats. Included are also .pdf-files and layouts, prepared for the DIY "toner-direct-method"
and for external PCB-Services.

## Speedometer:
![Speedometer](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/33459c03-00fe-4def-9850-420df5149345)

## DIY-Speedometer:
![4x7-Segment-LED-Display-Module](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/4ce9e9f0-d56a-4a53-8fab-c4b05e97ae68)
![Tachometer-with-4x7-Segm-LED-Display_sch](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/7436ab00-27f8-456e-ae47-7618ab607492)
![Tachometer-with-4x7-Segm-LED-Display_pcb](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/b8c57f8a-a1c3-4273-8336-95b31fcea549)

The DIY-Speedometer consists of a 4-digit LED display and an "Arduino Mini Pro" with 2 pieces of 10k resistors to GND. This tachometer can either be “manually wired”:
- by wire-connecting the 4-digit LED-Display to the Mini-Pro-Board + 2x 10k resistors to GND,
   or
- soldering SMD-parts on a SMD-Board with the provided Board-layout. Both Schematic + Layout as well as the BOM (Bill of Materials) are included.

The speedometer for speed display/control can also be purchased as a (ready-made) module - but then the "running" monitor + also the discharge-unit
is no longer availane / thereby (and can be ignored).

Anyone who is familiar with DIY-etching of circuit boards can either use the 2-sided Toner Direct Method (.pdf plans for 2-sided copy in 1:1 scale) or
give-it-out to an external PCB layout-service.  
In a separate article I will describe a new and simple method to make exact 2-sided DIY-PCB-layouts.

Documents are available as Eagle layouts, pdf-Files and as Gerber/Excellon files (in attached CAM .zip archives).
So also both Arduino Sketches (for the Arduino-Nano "heart"-Module: "Firmware-Nano.ino" and the Sketch for the DIY-Speedometer-*Board* with an ATmega328-uController + 4x7-LED-Display ***or***  
for the wired version of this DIY-Speedometer with an Ardino-Mini-Pro-*Module* + 4x7-LED-Display, wired together, using both the sketch "Tachometer-using-Arduino.ino".  
Thereto enclosed also a 3D-printable case + frame-parts, adapted to insert+glue the 4x7-LED-Display.
