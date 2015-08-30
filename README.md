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

### Papers used for References
 * Q Learning for Continous State Action Spaces - http://users.cecs.anu.edu.au/~rsl/rsl_papers/99ai.kambara.pdf
 * First Description of Backpropagation - Beyond Regression: New Tools for Prediction and Analysis in the Behavioral Sciences by Paul Werbos
 * Backpropagation Tutorial - https://www4.rgu.ac.uk/files/chapter3%20-%20bp.pdf
 * Backpropagation Tutorial - http://www.cse.unsw.edu.au/~cs9417ml/MLP2/
 * Q Learning Implementation - http://web.cs.swarthmore.edu/~meeden/cs81/s12/papers/MarkStevePaper.pdf
