# Project Context

## Purpose
In this project, we are building a remote-controlled door lock.

Parts:
- Hardware (off-the-shelf like Amazon Basics Traditional Electronic Keypad Deadbolt)
- Board for lock (we will build this)
- Remote with board (we will build this)

## Tech Stack
- Boards will use ATmega328P or AtmelSAM CPUs with the Arduino library
- Boards will have RFM69HCW radios
- Lock board will use a TB6612FNG board to drive the motor
- Lock board will use a INA169 board to sense current going to the motor
- We use KiCAD to design the boards and PCBWay to make them

## Project organization

- `kicad`: KiCAD files for the lock and remote boards
- `include`: Shared code
- `lib`: Shared code
- `src`
  - `lock`: Code for the lock
  - `remote`: Code for the remote

## Important Constraints
- Lock board max dimensions are 24mm X 63mm X 9mm
- Remote is powered by battery
- Lock board is powered by 4 AA batteries
- We are not electrical engineers. We really need to avoid risk of fire from our custom boards.

## AI Use
We will use AI to help with designing the hardware and keeping the specs up-to-date.
We will NOT use it for writing code. AI assistants must not generate any code!