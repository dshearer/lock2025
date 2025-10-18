# Hardware

- Amazon Basics Traditional Electronic Keypad Deadbolt
- Motor has "PL-48001" written on it.
- CPU is by Sonix

# Voltage
- ATmega328P:
  - 0 to 8MHz at 2.7 to 5.5V 
  - 0 to 16MHz at 4.5 to 5.5V
- RFM69HCW
  - 2.4 to 3.6V

# Power
- ATmega328P:
    - Active: 8mA
    - Idle: 2mA
    - Sleep: 0.1uA
- RFM69HCW:
    - Transmit: 45-130mA
    - Sleep: 0.1µA
- Motor
  - Takes 150mA to spin
  - Batteries provide 6V

# Position detection
- Hall-effect sensor
- quadrature encoder

# Motor current/voltage

- Startup: No back EMF yet, so current is very high (called inrush current)
- Running freely: High back EMF reduces current to just what's needed to overcome friction
- Under heavy load: Motor slows, back EMF decreases, current increases to provide more torque
- Stalled motor: No rotation = no back EMF = maximum current flows (can damage the motor!)

# Lock board max dimens

24mm X 63mm X 9mm