### Readme-Contents:
- [Basic Schematic](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#phase-control-for-230v-single-phase-motors-v2)
- [Module-Alignments](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#possible-alignment-of-used-modules-on-a-5mm-acrylic-base-board)
- [Main-Schematic](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#main-schematic--wiring-connections)
- [Safety-features](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#safety-features)
- [Disclaimer](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/blob/main/README.md#readme-contents)
- [Principle-description](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#principle-description)
- [The Wiring Board](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#the-wiring-board)
- [Speedometer](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#speedometer)
- [Schematic and Layout of the DIY-Speedometer](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#diy-speedometer)
- [DIY-Boards + PCB-Layout](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/#diy-boards)

# Phase-Control-for-230V-Single-Phase-Motors v2
Control for direction-switchable 230V *direct current* (DC)-motors, whose speed (rpm) can be controlled using a Potentiometer *or*  
using a PWM (Pulse Width Modulation) signal in the low voltage range, generated here by a microController.
![325V-DC-Motor-on-Phase-Control_Principle-Diagram](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/1215f584-f1fe-4574-99d1-6883fae35f82)
&nbsp; *Phase-Control priciple for 230V **DC**-Motors with reversing-switch.*

The picture above shows a basic diagram for the electronic control of a 230V **DC** motor, e.g. a hobby lathe, connected to a **single-phase** (230V) **AC**-current network.

Phase control circuits are often used for simple speed controls, here with an additional Bridge-Rectifier for 230V DC electric motors,
which are often used in medium power hobby machines (up to 10 amps ≙ approx. 2300 watts), such as lathes, milling benches, etc.

### Possible alignment of used modules on a 5mm acrylic base-board:
![3D-Steuerung-für-Bürsten-Gleichstrom-Motor-325V_Nano](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/25a48b36-aa1e-4cca-b5c9-efbb3eb60a19)
### Another module-alignment purpose:
![3D-Steuerung-für-Bürsten-Gleichstrom-Motor-325V_Nano_v2](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/blob/main/3D-Steuerung-f%C3%BCr-B%C3%BCrsten-Gleichstrom-Motor-325V_Nano_v2.png)  
Since electronic controls of 230V *DC* motors in today's hobby machines (such as lathes, milling benches, etc.) often are built with
inexpensive components that are not really protected against surges or current peaks – some electronic-failures are inevitable.
So I decided to use a circuit that works a little more reliable, with electronic components and *modules* that are readily available
on the market *and* at the same time are better protected against voltage-glitches, overloads, incorrect operations and engine over-temperature.
All 230VDC (Direct Current!) brushed motors with up-to 2300 Watts can be controlled with this phase control board. It allows to control
the speed of the motor (0-100%) as well as the motor direction of rotation.

### Main Schematic / Wiring Connections:
⇒ *(the DIY-Wiring-Board with the Ardujino-nano here is yellow-framed)*
![Phasenanschnittsteuerung-f-325V-Gleichstrom-Motor_Übersicht_DE](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/blob/main/Phasenanschnittsteuerung-f-325V-Gleichstrom-Motor_%C3%9Cbersicht.png)

## Safety features
This circuit has some essential safety-functions:
- The main module "Kemo-M240" used here contains an integrated soft start, needed to start up strong motors (>800 - 2300 Watts),
   to avoid the tripping of standard 16A household fuses
- This main-module also contains an automatic overload or anti-lock shutdown with electronic reset
- If the used Motor contains a built-in NTC-thermistor (with 100kOhm), the µController can provide a protection against
   engine overheatings: if the motor-temperature exceeds 80°C, the µC goes into an endless loop, and reactivating PWM
   is only possible by switching off (main-power) or by pushing the reset-button of the µC.

!!!: Protection against direction of rotation switching ***while driving*** is **NOT** provided: if so an "EMF short circuit" occurs,
the "Kemo-M240" controller shots the power-control off in a fractions of a second and thus at least the mechanics of the machine is protected.
Electronics then can only be reactivated after a complete restart - either per µContoller-Reset or after a complete Main-Power Shutdown.

## Disclaimer:  
!!! It goes without saying that you reproduce this circuit at your own risk and responsibility !!!
Anyone who does not have sufficient knowledge of how to handle *dangerous* 230VAC currents or 325VDC(ss) voltages(!):
If you are not familiar with electrical or electronic components, you should either hire a specialist to do it for you
or stay away from it!
“Experiments” without special knowledge and appropriate protection(s) on high voltages can quickly end in death
following an electric shock! And sudden cardiac-arrest can occur even days after an electric shock! - if then
no immediate help is nearby...
In any case, a replica of this project is done expressly and explicitly *at your own risk*!

**Further notes:**
- Three-phase motor control is excluded due to the principle! This circuit only controls single phase Engines!
- The circuit also works with (simpler) *AC* single-phase asynchronous motors - omitting the bridge rectifier used here.
   However, a connected ***AC*** motor then is no longer direction-reversible.
- It is NOT suitable for capacitor motors! - "Capacitor starting motors" are often only used for simple purposes and
   built+used for on/off operation only (such as for smaller hobby drill press machines) - and are not speed-regulated.
   But it depends specifically on the type of engine, some capacitor motors can still be regulated (speed-limited).
   However, any weak to poor efficiency that so occurs should be taken into account (including heat losses).

Factual suggestions for improvements and changes are always welcome!

## Principle description:
The control works according to the 230VAC phase control principle (similar to a lamp dimmer), but in contrast to this
it can neutralize EMF (electromotive forces) of greater motors and is determined by special selection of used electronic components.

The Kemo-240 control-module is controlled via PWM (Pulse Width Modulation). This (low-voltage: 5VDC) PWM-signal is generated
with a “Arduino-Nano” micro-controller and (if present), this µC also evaluates an NTC-Sensor installed within the Motor, to check
a possible motor-overtemperature.
The Speedometer (RPM display on a LED display) does not run - as usually *independently* of the main phase-control, 
but sends back a "busy" signal to the µController, which therefore "knows" when the motor is running or not
(=when it is standing still), in order to ensure active discharge of the triac snubber and the EMI or EMC-filters charged
through supplied voltages + motor back EMF, to avoid uncontrolled jerky startings of the DC-motor after changing the direction of rotation.

The control consists of several modules chained(wired) one behind the other:

1. the 10AT fuse (F1) and the on/off switch with the self-holding latching contactor,
2. the (mandatory) safety switches plus other 230V safety microswitches on the specific machine,
3. the upstream EMC filter (= ElectroMagnetic Compatibility), here called EMI filter (serves to avoid incoming E.M. interferences),
4. the actual (triac) phase control, using *either* a potentiometer *or* a µController with PWM-output (Pulse-Width Modulation),
5. in parallel the 5V low-voltage circuit with the "Arduino-Nano" module, to generate the PWM for the phase control (2nd. version), 
   with additional, software-side monitoring of eventual motor-over-temperature,
6. the 15A/1500V bridge rectifier,
7. the reversing switch, also called “pole inverter” or direction of rotation switch,
8. a second EMI (carbon-brush-"fire"-) suppression filter + a second 10AT motor fuse (F2), and
9. at last the 325V DC (=direct current) motor.

230v-Wiring between modules and the outside-parts (rear 230V inlet-panel, front machine-control-panel, motor-connections) have to be connected safely
through PVC-isolated 1mm² wires (about AWG 17) and in-between through isolated threaded-connectors or clamping-connectors with 16A/230V.
I used lever clamps ("WAGO Hebelklemme 221-2411") for most of the "flying"-connections.  
For all 5Volt (=low-voltage) connections I used smaller wires (about 0.25-0.75mm² = AWG-23 upto AWG-20) with JST-Connectors (2-, 3- and 4-wired),
as connectors for the wiring-board. Of course also it's possible to use above mentionned WAGO-connectors for smaller wires.  
**Att: Reliability of connections** is VERY IMPORTANT, so dont choose cheap (=mostly bad) connectors, due to 99% of all electical problems
are resulting from loose contacts/connections!

## The Wiring Board:
The two DIY-parts in this chain that still needs to be soldered together + programmed are the “wiring board” and the DIY-Speedometer.  
The Wiring-board serves to wire/connect the Low-Voltage PWM-Signal to the main phase control module "Kemo-M240" and to other low-voltage connections
as the Panel-Signal-"On"-LED, the NTC-Sensor and the 10k&Omega;-Poti.  
This wiring board also contains the Arduino Nano µController as the heart of the circuit, some passive components (resistors + capacitors) and above mentionned
(JST-)Connectors plus a 4-wire JST-Connector for the Speedometer (rpm display) and the 230VAC Discharge Circuit (Power-Resistor), this to avoid uncontrolled,
jerk startings of the motor while switching it into reverse-direction.
![Wiring-Board-Nano_sch](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/5f90a135-2036-4bc7-9c50-fc07217dd82b)
*Circuit diagram of the low-voltage circuit "wiring board" with the Arduino Nano*  
![Wiring-Board-Nano_3D](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/70447996-00d2-4e98-be32-b3b7b477d32b)

This wiring board with the Arduino Nano is provided as schematic and as PCB-layout in Eagle format - which can easily be converted
into other formats. Included are also .pdf-files and layouts, prepared for the DIY "toner-direct-method" and for external PCB-Services.

## Speedometer:
![Speedometer](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/33459c03-00fe-4def-9850-420df5149345)

## DIY-Speedometer:
![4x7-Segment-LED-Display-Module](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/4ce9e9f0-d56a-4a53-8fab-c4b05e97ae68)

**Schematic and Layout of the DIY-Speedometer**:  
![Tachometer-with-4x7-Segm-LED-Display_sch](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/7436ab00-27f8-456e-ae47-7618ab607492)
![Tachometer-with-4x7-Segm-LED-Display_pcb](https://github.com/nlohr1/Phase-Control-for-230V-Single-Phase-Motors/assets/49346586/b8c57f8a-a1c3-4273-8336-95b31fcea549)

The DIY-Speedometer consists of a 4-digit LED-display-module and a "Arduino Mini Pro"-module (inside the case) with 2 pieces of 10k resistors to GND.  
This tachometer can either be “manually wired”:
- by wire-connecting a 4-digit LED-Display module to a (Arduino-)Mini-Pro board + 2x 10k resistors to GND,
   or
- soldering SMD-parts on a DIY-Board with above provided layout. For this purpose I included Schematic + Layout, as well as BOM (Bill of Materials).

A speedometer to display the speed of the motor can also be purchased as a (ready-made) module - but this modules have no "running" monitor Signal and
so the discharge-unit (on the wiring-board) is no longer switching on/off (as needed for the 15k-Resistor R9).

## DIY-Boards:
Anyone who is familiar with DIY-etching of circuit boards can either use the 2-sided Toner Direct Method (see .pdf plans for 2-sided copy in 1:1 scale) or
give-it-out to an external PCB layout-service.  
In a further -separate- article I will describe a new and simpler method to make exact 2-sided DIY-PCB-layouts through laser-burning "resting"
ink-protect areas on the PCB-board (resting "black" areas: substracted isolated wiring-lines from the total surface).

All Documents are available as Eagle layouts, png-Image-Files and pdf-Files and also as Gerber+Excellon files (attached CAM .zip archives).
So also both Arduino Sketches (for the Arduino-Nano "heart"-Module: "Firmware-Nano.ino" and the Sketch for the **DIY**-Speedometer-*Board* including a "naked"
ATmega328-µController + 4x7-LED-Display ***or*** for the wired version with an Ardino-Mini-Pro-*Module* + 4x7-LED-Display(-module).  
Both variants are using the "Tachometer-using-Arduino.ino" sketch.  

Thereto I've enclosed also a 3D-printable case + frame-parts for the Tachometer, adapted to insert+glue the 4x7-LED-Display module.
