# ArduinoNixieTubeClock
This is the Arduino code that controls nixie tubes for a clock through shift registers and nixie drivers

    Author: Alejandro Palacios (pamanes@gmail.com)
    Creation Date: Jan. 10, 2015
    Purpose: 
    
    This is a clock program that pushes the time
    to Shift Registers in order to light up Nixie Tubes.
	
	H   H : M   M : S   S  (Nixies)
	|   |   |   |   |   |   (Rusian Driver has 10 output pins, each lighting up a number)
        D   D   D   D   D   D   (Russian Driver K155NA1)
	  \/      \/      \/    (4 pins to each Russian Driver)
	  SF3     SF2     SF1   (each shift register has 8 output pins - 74HC595)
	  
    There are three Shift Registers, each controls two nixie tubes

LICENSE
=======

See COPYING.txt for the MIT LICENSE
