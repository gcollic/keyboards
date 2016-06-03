/*
Copyright 2016 Guillaume Collic <gcollic@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"

extern const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern const uint16_t fn_actions[];

#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06,         K07, K08, K09, K0A, K0B, K0C, K0D, \
    K10, K11, K12, K13, K14, K15, K16,         K17, K18, K19, K1A, K1B, K1C, K1D, \
    K20, K21, K22, K23, K24, K25, K26,         K27, K28, K29, K2A, K2B, K2C, K2D, \
    K30, K31, K32, K33, K34, K35, K36,         K37, K38, K39, K3A, K3B, K3C, K3D, \
     K40,  K41,  K43,  K44,  K45, K46,         K47, K48,  K49,  K4A,  K4C,  K4D   \
) { \
/* Left hand                                                                   */ \
/*        A(Red)    B(orange) C(Yellow) D(Green)  E(Blue)   F(Purple) G(Pink)  */ \
/* L0 */{ KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06 }, \
/* L1 */{ KC_##K10, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15, KC_##K16 }, \
/* L2 */{ KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26 }, \
/* L3 */{ KC_##K30, KC_##K31, KC_##K32, KC_##K33, KC_##K34, KC_##K35, KC_##K36 }, \
/* L4 */{ KC_##K40, KC_##K41, KC_NO   , KC_##K43, KC_##K44, KC_##K45, KC_##K46 }, \
/*                                                                             */ \
/* Right hand                                                                  */ \
/*        A(Red)    B(orange) C(Yellow) D(Green)  E(Blue)   F(Purple) G(Pink)  */ \
/* R0 */{ KC_##K0D, KC_##K0C, KC_##K0B, KC_##K0A, KC_##K09, KC_##K08, KC_##K07 }, \
/* R1 */{ KC_##K1D, KC_##K1C, KC_##K1B, KC_##K1A, KC_##K19, KC_##K18, KC_##K17 }, \
/* R2 */{ KC_##K2D, KC_##K2C, KC_##K2B, KC_##K2A, KC_##K29, KC_##K28, KC_##K27 }, \
/* R3 */{ KC_##K3D, KC_##K3C, KC_##K3B, KC_##K3A, KC_##K39, KC_##K38, KC_##K37 }, \
/* R4 */{ KC_##K4D, KC_##K4C, KC_NO   , KC_##K4A, KC_##K49, KC_##K48, KC_##K47 }  \
}

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode)
{
    return (action_t){ .code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]) };
}

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Keymap 0: Default Layer
    KEYMAP(
         ESC, 1,   2,   3,   4,   5, GRV,      EQL,  6,   7,   8,   9,   0,MINS, \
         TAB, Q,   W,   E,   R,   T,BSPC,      DEL,  Y,   U,   I,   O,   P,LBRC, \
         FN2, A,   S,   D,   F,   G,  NO,     RBRC,  H,   J,   K,   L,SCLN,QUOT, \
        LSFT, Z,   X,   C,   V,   B,LGUI,     NUBS,  N,   M,COMM, DOT,SLSH,BSLS, \
         LCTL,LGUI,  NO, LALT,  SPC, ENT,      FN1,FN0, RALT,  INS,  APP, RCTL   ),
    // Keymap 1: Main function layer
    KEYMAP(
        TRNS,         F1,  F2,  F3,  F4,      F5,  F6,    F7,  F8,  F9, F10, F11, F12,PSCR, \
        TRNS,   KP_SLASH,KP_7,KP_8,KP_9,KP_MINUS,  NO,  VOLU,MNXT,HOME,  UP, END,PGUP,SLCK, \
        TRNS,KP_ASTERISK,KP_4,KP_5,KP_6, KP_PLUS,  NO,  VOLD,MPLY,LEFT,DOWN,RGHT,PGDN, BRK, \
        TRNS,       KP_0,KP_1,KP_2,KP_3,  KP_DOT,  NO,  MUTE,MPRV,  NO,  NO,  NO,  NO,TRNS, \
         TRNS,        TRNS, TRNS, TRNS,     TRNS, FN0,   FN1,TRNS, TRNS, TRNS, TRNS, TRNS   ),
    // Keymap 2: Mouse layer
    KEYMAP(
        TRNS,TRNS,TRNS,TRNS, TRNS,TRNS,TRNS,  TRNS,TRNS,WH_L,BTN3,WH_R,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS, TRNS,TRNS,TRNS,  TRNS,TRNS,BTN1,MS_U,BTN2,WH_U,TRNS, \
        TRNS,TRNS, FN3, FN4,  FN5,TRNS,TRNS,  TRNS,TRNS,MS_L,MS_D,MS_R,WH_D,TRNS, \
        TRNS,TRNS,TRNS,TRNS, TRNS,TRNS,TRNS,  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, \
          TRNS, TRNS, TRNS, TRNS, TRNS,TRNS,  TRNS,TRNS, TRNS, TRNS, TRNS, TRNS   ),
};

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),
    [1] = ACTION_LAYER_TOGGLE(1),
    [2] = ACTION_LAYER_MOMENTARY(2),
    [3] = ACTION_MODS_KEY(MOD_LSFT, KC_DEL),
    [4] = ACTION_MODS_KEY(MOD_LCTL, KC_INS),
    [5] = ACTION_MODS_KEY(MOD_LSFT, KC_INS),
};