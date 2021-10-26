# Name: MASAUSO LUNGU 209533

Link to My `Digital-electronics-2` GitHub repository:

   [Masauso Lungu](https://github.com/Masauso-L/Digital-electronis-2/tree/main/Labs/05-segments)


# Lab 5: Display devices, 7-segment display


### Learning objectives

The purpose of the laboratory exercise is to understand the serial control of four seven-segment displays (SSDs) using a pair of 74595 shift registers. In addition, the goal is to master the use of interrupts in applications with AVR.

&nbsp;
&nbsp;
<a name="preparation"></a>
## Preparation tasks (done before the lab at home)

1. Read the [7-segment display tutorial](https://www.electronics-tutorials.ws/blog/7-segment-display-tutorial.html) and find out what is the difference between:
   * Common Cathode 7-segment display (CC SSD)
   * Common Anode 7-segment display (CA SSD)

2. In the following table, write the binary values of the segments for display 0 to 9 on a common anode 7-segment display.

   | **Digit** | **A** | **B** | **C** | **D** | **E** | **F** | **G** | **DP** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
   | 1 | 1  | 0  |  0 |  1 | 1  | 1  | 1  |  1 |
   | 2 | 0  | 0  | 1  | 0  | 0  | 1  | 0  |  1 |
   | 3 | 0  | 0  | 0  | 0  | 1  | 1  | 0  |  1 |
   | 4 | 1  | 0  | 0  | 1  | 1  | 0  | 0  |  1 |
   | 5 | 0  | 1  | 0  | 0  | 1  | 0  | 0  |  1 |
   | 6 | 0  | 1  | 0  | 0  | 0  | 0  | 0  |  1 |
   | 7 | 0  | 0  | 0  | 1  | 1  | 1  | 1  |  1 |
   | 8 | 0  | 0  | 0  | 0  | 0  | 0  | 0  |  1 |
   | 9 | 0  | 0  | 0  | 0  | 1  | 0  | 0  |  1 |

3. Use schematic of the [Multi-function shield](../../Docs/arduino_shield.pdf) and find out the connection of seven-segment display. What is the purpose of two shift registers 74HC595?

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;