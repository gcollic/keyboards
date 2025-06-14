#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

   [0] = LAYOUT(
          KC_ESC,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5          ,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,  KC_MINS
      ,   KC_TAB,     KC_Q,     KC_W,     KC_E,     KC_R,     KC_T          ,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,  KC_LBRC
      ,    MO(1),     KC_A,     KC_S,     KC_D,     KC_F,     KC_G          ,     KC_H,     KC_J,     KC_K,     KC_L,  KC_SCLN,  KC_QUOT
      ,  KC_LSFT,     KC_Z,     KC_X,     KC_C,     KC_V,     KC_B          ,     KC_N,     KC_M,  KC_COMM,   KC_DOT,  KC_SLSH,  KC_BSLS
      ,  KC_LCTL,  KC_LGUI          ,  KC_LALT,   KC_SPC,   KC_ENT          ,  KC_RALT,    MO(1)
   ),

   [1] = LAYOUT(
         _______,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5          ,    KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,   KC_F11
      ,  _______,  _______,  _______,  _______,  _______,   KC_INS          ,   KC_GRV,  KC_HOME,    KC_UP,   KC_END,  KC_PGUP,   KC_F12
      ,  _______,  _______,S(KC_DEL),C(KC_INS),S(KC_INS),  KC_BSPC          ,   KC_DEL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_PGDN,  KC_PSCR
      ,  _______,  _______,  _______,  _______,  _______,  _______          ,  KC_MUTE,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_VOLD,  KC_VOLU
      ,  _______,  _______          ,  _______,  _______,  _______          ,  _______,  _______
   ),

   [2] = LAYOUT(
         _______,  _______,  _______,  _______,  _______,  _______          ,  _______,  _______,  _______,  _______,  _______,  _______
      ,  _______,  _______,  _______,KC_SCROLL,  _______,  _______          ,  _______,  _______,  _______,  _______,  _______,  _______
      ,  _______,  _______,  MS_BTN3,  MS_BTN2,  MS_BTN1,  _______          ,  _______,  _______,  _______,  _______,  _______,  _______
      ,  _______,  _______,  KC_CPI1,  KC_CPI2,  KC_CPI3,  _______          ,  _______,  _______,  _______,  _______,  _______,  _______
      ,  _______,  _______          ,  _______,  _______,  _______          ,  _______,  _______
   )

};

// https://docs.splitkb.com/product-guides/liatris/power-led
// https://docs.splitkb.com/product-guides/liatris/onboard-rgb
void keyboard_post_init_user(void) {
    // Initialize RGB to static black
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_BLACK);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}

void housekeeping_task_user(void) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case 0:
            // Default layer
            rgblight_setrgb_at(RGB_BLACK, 1);
            break;
        case 1:
            rgblight_setrgb_at(RGB_RED, 1);
            break;
        case 2:
            rgblight_setrgb_at(RGB_GREEN, 1);
            break;
        case 3:
            rgblight_setrgb_at(RGB_BLUE, 1);
            break;
    }

    led_t led_state = host_keyboard_led_state();
    uint8_t red = led_state.caps_lock ? 255 : 0;
    uint8_t green = led_state.num_lock ? 255 : 0;
    uint8_t blue = led_state.scroll_lock ? 255 : 0;
    rgblight_setrgb_at(red, green, blue, 0);
}

void pointing_device_init_user(void) {
    set_auto_mouse_enable(true);
}
