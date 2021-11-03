# Lab 6: Display devices, LCD display


### Learning objectives

The purpose of the laboratory exercise is to understand the serial control of Hitachi HD44780-based LCD character display and how to define custom characters. Another goal is to learn how to read documentation for library functions and use them in your own project


## Preparation tasks (done before the lab at home)

Use schematic of the [LCD keypad shield](../../Docs/arduino_shield.pdf) and find out the connection of LCD display. What data and control signals are used? What is the meaning of these signals?

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;
| **Signal(s)** | **Pin(s)** | **Purpose** |
| :-: | :-: | :-- |
| RS | PB0 | Register selection signal. Selection between *Instruction register* (0) and *Data register* (1) |
| R/W | GND | Selecting reading or writing. GND means only writing is enabled |
| E | PB1 | Enable signal for communication |
| D[3:0] | N/A | Data transfer in 8-bit mode. |
| D[7:4] | PD[7:4] | Data transfer in both 8 and 4-bit modes. |

| **Signal(s)** | **Value(s)** | **Purpose** |
| :-: | :-: | :-- |
| RS | 1 | Selects *Data register* |
| D[7:4] | 0100 | Higher four bits of sent word |
| D[7:4] | 0011 | ASCII code: C|


What is the ASCII table? What are the values for uppercase letters `A` to `Z`, lowercase letters `a` to `z`, and numbers `0` to `9` in this table?

ASCII table is a table of values for character encoding standard for electronic communication. ASCII code is used to represent text in computers and other devices.
&nbsp;

|**CHAR**|**DEC**|**HEX**|**CHAR**|**DEC**|**HEX**|**CHAR**|**DEC**|**HEX**|
| :-: | :-: | :-: | :-: | :-: | :-:| :-: | :-: | :-: |
|0 | 48 | 30 | A | 65 | 41 | a | 97  | 61 |
|1 | 49 | 31 | B | 66 | 42 | b | 98  | 62 |
|2 | 50 | 32 | C | 67 | 43 | c | 99  | 63 |
|3 | 51 | 33 | D | 68 | 44 | d | 100 | 64 |
|4 | 52 | 34 | E | 69 | 45 | e | 101 | 65 |
|5 | 53 | 35 | F | 70 | 46 | f | 102 | 66 | 
|6 | 54 | 36 | G | 71 | 47 | g | 103 | 67 |
|7 | 55 | 37 | H | 72 | 48 | h | 104 | 68 |
|8 | 56 | 38 | I | 73 | 49 | i | 105 | 69 |
|9 | 57 | 39 | J | 74 | 4A | j | 106 | 6A |
|..|....|....| K | 75 | 4B | k | 107 | 6B |
|..|....|....| L | 76 | 4C | l | 108 | 6C |
|..|....|....| M | 77 | 4D | m | 109 | 6D |
|..|....|....| N | 78 | 4E | n | 110 | 6E |
|..|....|....| O | 79 | 4F | o | 111 | 6F |
|..|....|....| P | 80 | 50 | p | 112 | 70 |
|..|....|....| Q | 81 | 51 | q | 113 | 71 |
|..|....|....| R | 82 | 52 | r | 114 | 72 |
|..|....|....| S | 83 | 53 | s | 115 | 73 |
|..|....|....| T | 84 | 54 | t | 116 | 74 | 
|..|....|....| U | 85 | 55 | u | 117 | 75 |
|..|....|....| V | 86 | 56 | v | 118 | 76 |
|..|....|....| W | 87 | 57 | w | 119 | 77 |
|..|....|....| X | 88 | 58 | x | 120 | 78 |
|..|....|....| Y | 89 | 59 | y | 121 | 79 |
|..|....|....| Z | 90 | 5A | z | 122 | 7A |
&nbsp;

In the lab, we are using [LCD library for HD44780 based LCDs](http://www.peterfleury.epizy.com/avr-software.html) developed by Peter Fleury. Use online manual of LCD library and add the input parameters and description of the functions to the following table.

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `lcd_init` | `LCD_DISP_OFF`<br>`LCD_DISP_ON`<br>`LCD_DISP_ON_CURSOR`<br>`LCD_DISP_ON_CURSOR_BLINK` | Display off<br>display on, cursor off<br>display on, cursor on<br>display on, cursor on flashing |`lcd_init(LCD_DISP_OFF);`<br>&nbsp;<br>&nbsp;<br>&nbsp; |
   | `lcd_clrscr` | `void` | Clear display and set cursor to home position. | `lcd_clrscr();` |
   | `lcd_gotoxy` | `uint8_t x`<br>`uint8_t y`|	Set cursor to specified position. | `lcd_gotxy(2,2);` |
   | `lcd_putc` |`char c` | Display character at current cursor position.   | `lcd_putc(c);` |
   | `lcd_puts` | `const char* s` | Display string without auto linefeed.   |`lcd_puts(s);` |
   | `lcd_command` | `uint8_t cmd` | Send LCD controller instruction command, | `lcd_command(cmd);` |
   | `lcd_data` | `uint8_t data` | Send data byte to LCD controller. |`lcd_data(data);` |