
#include "orb55.h"
#include "pointing_device.h"
extern const pointing_device_driver_t pointing_device_driver;

static bool scroll_pressed;
static int8_t scroll_h;
static int8_t scroll_v;

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    if (!is_keyboard_master()) return mouse_report;

    int8_t clamped_x = mouse_report.x, clamped_y = mouse_report.y;
    mouse_report.x = 0;
    mouse_report.y = 0;

    if (scroll_pressed) {
        // accumulate scroll
        scroll_h += clamped_x;
        scroll_v += clamped_y;

        int8_t scaled_scroll_h = scroll_h / SCROLL_DIVIDER;
        int8_t scaled_scroll_v = scroll_v / SCROLL_DIVIDER;

        // clear accumulated scroll on assignment

        if (scaled_scroll_h != 0) {
            mouse_report.h = -scaled_scroll_h;
            scroll_h       = 0;
        }

        if (scaled_scroll_v != 0) {
            mouse_report.v = -scaled_scroll_v;
            scroll_v       = 0;
        }
    } else {
        mouse_report.x = -clamped_x;
        mouse_report.y = clamped_y;
    }

    return mouse_report;
}

static void on_cpi_button(uint16_t cpi, keyrecord_t *record) {

    if(!record->event.pressed)
        return;

    pointing_device_set_cpi(cpi);
}

static void on_mouse_button(uint8_t mouse_button, keyrecord_t *record) {

    report_mouse_t report = pointing_device_get_report();

    if(record->event.pressed)
        report.buttons |= mouse_button;
    else
        report.buttons &= ~mouse_button;

    pointing_device_set_report(report);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {

    if(!process_record_user(keycode, record))
        return false;

    // handle mouse drag and scroll

    switch (keycode) {
        case KC_BTN1:
            on_mouse_button(MOUSE_BTN1, record);
            return false;

        case KC_BTN2:
            on_mouse_button(MOUSE_BTN2, record);
            return false;

        case KC_BTN3:
            on_mouse_button(MOUSE_BTN3, record);
            return false;

        case KC_BTN4:
            on_mouse_button(MOUSE_BTN4, record);
            return false;

        case KC_BTN5:
            on_mouse_button(MOUSE_BTN5, record);
            return false;

        case KC_SCROLL:
            scroll_pressed = record->event.pressed;
            return false;

        case KC_CPI1:
            on_cpi_button(CPI_1, record);
            return false;

        case KC_CPI2:
            on_cpi_button(CPI_2, record);
            return false;

        case KC_CPI3:
            on_cpi_button(CPI_3, record);
            return false;

        default:
            return true;
    }
}