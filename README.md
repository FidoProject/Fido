# Fido

### Task examples
 * Learn to always keep an LED on
 * Learn to keep an LED on when a red light is shone on the system
 * Learn to display only red with the LED
 * Learn to drive straight
 * Learn to seek a magnetic field
 

### Black Box Model Specifications

#### Inputs and Outputs

| Inputs       | Outputs         |
|:------------:|:---------------:|
| Full IMU     | Piezo Buzzer    |
| Microphone   | RGB LED         |
| Qi Charging  | Pager Vibrator  |
| Infrared     | Wheels          |

#### Negative and Positive Reinforcements

| Positive     | Negative     |
|:------------:|:------------:|
| IR Intensity | Low battery  |
| Charging     | Hit/Knocking |
