/*
     .k8GOGGNqkSFS5XkqXPSkSkkqXXFS5kSkSS15U22F2515U2uujuu1U1u2U1U2uUuFS.
   :0qE     JS5uuJuuFFX51jU2SSk12jU2SSXF5uuu15SFS5k12ujj21S5kFS5S12jJYu11
  5XS:        1UYYLu.   vUUX    U22r     SUF         SUF           ;YYLuU5
 1F5i  NNSkS7  2uLJui   51u     S5.      .PX         .XX           LJvLLu1.
 kUk  0iLk5FFu vuYY2:   5F    Xkk7        78    E0    i0    GEXPXk2uLLvLLuk
X25, 8O   2kX0  5YJUi   M    555    PkXk   i    q1FU   7    ONNkP12YLvLvLYS
S25  8888  888  5uY5         FuS    PS50   .    FuUU   7          uJvLvLLJ2i
kUF             SJjU.      P02UF    P25k   .    Su2Y   v          2LLvLvLL17
S21  XJj88  0u  1uY2.        X2k           .    k11E   v    7;ii:JuJvLvLvJ2:
2257 jqv   Pqq  1LJur         PP.          7    EX:    q    OqqXP51JYvLvYYS.
 X2F  kXkXSXk  kJYLU:   O     ,Z    0PXZ   i    ii    q0    i:::,,.jLLvLLuF'
 ik1k  ;qkPj  .uJvYu:   UN      :   XU2F   :         S5S           iJLLvjUF8
  :PSq       72uLLLui   uSi    .;   2uY1   r.       72j1           LYYLYJSU88
    XqE2   rP12juJuu1FX55U5FqXXSXkXF1juUkkPSXSPXPXPF1Jju5FkFSFXFSF5uujUu5j28V
      .uGOZESS5S5SFkkPkPkXkPXqXPXqXXFkSkkPXPXPkqSkSS1521252121U2u2u12Suv7

*
* Arduino Micro (Leonardo) PS3 USB Pad emulator firmware
*
* Copyright (c) 2016
* Bruno Freitas - bruno@brunofreitas.com
* Jon Wilson    - degenatrons@gmail.com
* Kevin Mackett - kevin@sharpfork.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "PS3Pad.h"
#include "util.h"

void setup_pins(void);

static const uint8_t digital_dir_lookup[16] = {8, 2, 6, 8, 4, 3, 5, 8, 0, 1, 7, 8, 8, 8, 8, 8};

int main(void) {
	uint8_t pad_up, pad_down, pad_left, pad_right, pad_cross, pad_triangle, pad_circle, pad_square, pad_l2,
	pad_r2, pad_start, pad_select, pad_l3, pad_r3, pad_l1, pad_r1, pad_left_analog_x,
	pad_left_analog_y, pad_right_analog_x, pad_right_analog_y;

	// Set clock @ 16Mhz
	CPU_PRESCALE(0);

	// Disable JTAG
	bit_set(MCUCR, 1 << JTD);
	bit_set(MCUCR, 1 << JTD);

	// Setup pins
	setup_pins();

	// Init PS3 pad emulation
	vs_init(true);

	// Pins polling and gamepad status updates
	for (;;) {
		vs_reset_watchdog();

		pad_up = !bit_check(PINC, 7);
		pad_down = !bit_check(PINB, 2);
		pad_left = !bit_check(PINB, 0);
		pad_right = !bit_check(PIND, 3);
		pad_triangle = !bit_check(PIND, 2);
		pad_circle = !bit_check(PIND, 1);
		pad_square = !bit_check(PIND, 0);
		pad_cross = !bit_check(PIND, 4);
		pad_l2 =  !bit_check(PINC, 6);
		pad_r2 =  !bit_check(PIND, 7);
		pad_start =  !bit_check(PINE, 6);
		pad_select =  !bit_check(PINB, 4);
		pad_l3 =  !bit_check(PINB, 5);
		pad_r3 =  !bit_check(PINB, 6);
		pad_l1 = !bit_check(PINB, 7);
		pad_r1 = !bit_check(PIND, 6);

		pad_left_analog_x = pad_left_analog_y = pad_right_analog_x = pad_right_analog_y = 0x7F;

		if(!bit_check(PINB, 1)) {
			pad_left_analog_x = 0x00;
		} else if(!bit_check(PINB, 3)) {
			pad_left_analog_x = 0xFF;
		}

		if(!bit_check(PINF, 0)) {
			pad_left_analog_y = 0x00;
		} else if(!bit_check(PINF, 1)) {
			pad_left_analog_y = 0xFF;
		}

		if(!bit_check(PINF, 4)) {
			pad_right_analog_x = 0x00;
		} else if(!bit_check(PINF, 5)) {
			pad_right_analog_x = 0xFF;
		}

		if(!bit_check(PINF, 6)) {
			pad_right_analog_y = 0x00;
		} else if(!bit_check(PINF, 7)) {
			pad_right_analog_y = 0xFF;
		}

		gamepad_state.cross_btn = pad_cross;
		gamepad_state.cross_axis = gamepad_state.cross_btn * 0xFF;
		gamepad_state.circle_btn = pad_circle;
		gamepad_state.circle_axis = gamepad_state.circle_btn * 0xFF;
		gamepad_state.square_btn = pad_square;
		gamepad_state.square_axis = gamepad_state.square_btn * 0xFF;
		gamepad_state.triangle_btn = pad_triangle;
		gamepad_state.triangle_axis = gamepad_state.triangle_btn * 0xFF;

		gamepad_state.l1_btn = pad_l1;
		gamepad_state.l1_axis = gamepad_state.l1_btn * 0xFF;
		gamepad_state.r1_btn = pad_r1;
		gamepad_state.r1_axis = gamepad_state.r1_btn * 0xFF;
		gamepad_state.l2_btn = pad_l2;
		gamepad_state.l2_axis = gamepad_state.l2_btn * 0xFF;
		gamepad_state.r2_btn = pad_r2;
		gamepad_state.r2_axis = gamepad_state.r2_btn * 0xFF;

		gamepad_state.direction = digital_dir_lookup[pad_up << 3 | pad_down << 2 |
				pad_left << 1 | pad_right];

		gamepad_state.start_btn = pad_start;
		gamepad_state.select_btn = pad_select;

		gamepad_state.ps_btn = pad_start && pad_select;

		gamepad_state.l3_btn = pad_l3;
		gamepad_state.r3_btn = pad_r3;

		gamepad_state.l_x_axis = pad_left_analog_x;
		gamepad_state.l_y_axis = pad_left_analog_y;
		gamepad_state.r_x_axis = pad_right_analog_x;
		gamepad_state.r_y_axis = pad_right_analog_y;

		vs_send_pad_state();
	}
}

void setup_pins(void) {

	// Setup pins
	bit_clear(DDRF, 1 << 7);
	bit_set(PORTF, 1 << 7);

	bit_clear(DDRF, 1 << 6);
	bit_set(PORTF, 1 << 6);

	bit_clear(DDRF, 1 << 5);
	bit_set(PORTF, 1 << 5);

	bit_clear(DDRF, 1 << 4);
	bit_set(PORTF, 1 << 4);

	bit_clear(DDRF, 1 << 1);
	bit_set(PORTF, 1 << 1);

	bit_clear(DDRF, 1 << 0);
	bit_set(PORTF, 1 << 0);

	bit_clear(DDRB, 1 << 3);
	bit_set(PORTB, 1 << 3);

	bit_clear(DDRB, 1 << 1);
	bit_set(PORTB, 1 << 1);

	bit_clear(DDRD, 1 << 6);
	bit_set(PORTD, 1 << 6);

	bit_clear(DDRB, 1 << 7);
	bit_set(PORTB, 1 << 7);

	bit_clear(DDRB, 1 << 6);
	bit_set(PORTB, 1 << 6);

	bit_clear(DDRB, 1 << 5);
	bit_set(PORTB, 1 << 5);

	bit_clear(DDRB, 1 << 4);
	bit_set(PORTB, 1 << 4);

	bit_clear(DDRE, 1 << 6);
	bit_set(PORTE, 1 << 6);

	bit_clear(DDRD, 1 << 7);
	bit_set(PORTD, 1 << 7);

	bit_clear(DDRC, 1 << 6);
	bit_set(PORTC, 1 << 6);

	bit_clear(DDRD, 1 << 4);
	bit_set(PORTD, 1 << 4);

	bit_clear(DDRD, 1 << 0);
	bit_set(PORTD, 1 << 0);

	bit_clear(DDRD, 1 << 1);
	bit_set(PORTD, 1 << 1);

	bit_clear(DDRD, 1 << 2);
	bit_set(PORTD, 1 << 2);

	bit_clear(DDRD, 1 << 3);
	bit_set(PORTD, 1 << 3);

	bit_clear(DDRB, 1 << 0);
	bit_set(PORTB, 1 << 0);

	bit_clear(DDRB, 1 << 2);
	bit_set(PORTB, 1 << 2);

	bit_clear(DDRC, 1 << 7);
	bit_set(PORTC, 1 << 7);
}
