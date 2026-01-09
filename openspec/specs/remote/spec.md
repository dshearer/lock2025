# Remote Control

## Overview
The remote control is a battery-powered handheld device that sends wireless commands to the lock.

## Power
### Requirement: Battery Operation
The remote SHALL operate on battery power.

#### Scenario: Battery life
- **WHEN** the remote is used normally
- **THEN** the battery lasts a while

## Physical Interface
### Requirement: Two-Button Control
The remote SHALL provide two physical buttons for lock control: "lock" and "unlock".

### Requirement: Button Commands
The remote SHALL send commands to the lock board when buttons are pressed:
- The "lock" button SHALL tell the lock board to engage the lock
- The "unlock" button SHALL tell the lock board to disengage the lock

#### Scenario: Lock button pressed
- **WHEN** the user presses the "lock" button
- **THEN** the remote sends a command to engage the lock

#### Scenario: Unlock button pressed
- **WHEN** the user presses the "unlock" button
- **THEN** the remote sends a command to disengage the lock

## Visual Feedback
### Requirement: RGB LED Indicator
The remote SHALL provide an RGB LED for visual status feedback.

### Requirement: Command Progress Indication
The remote SHALL display yellow on the LED while waiting for a response after a button is pressed.

#### Scenario: Waiting for response
- **WHEN** a button is pressed and a command is sent
- **THEN** the LED shows yellow
- **AND** remains yellow until a response is received or timeout occurs

### Requirement: Success Feedback
The remote SHALL display solid green momentarily and then turn off the LED when the lock operation is successful.

#### Scenario: Successful operation
- **WHEN** a response indicates the operation was successful
- **THEN** the LED shows solid green momentarily
- **AND** turns off

### Requirement: Failure Feedback
The remote SHALL flash red several times and then turn off the LED when the lock operation fails or no response is received.

#### Scenario: Failed operation
- **WHEN** a response indicates the operation failed
- **THEN** the LED flashes red several times
- **AND** turns off

#### Scenario: No response timeout
- **WHEN** no response is received after some time
- **THEN** the LED flashes red several times
- **AND** turns off
