#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        LGUI(KC_1), LGUI(KC_2), LGUI(KC_3),
        KC_F11,     LSG(KC_S),  KC_CALC,
        LCTL(KC_C), LCTL(KC_V), LCTL(KC_Z)
    )
};

const uint16_t PROGMEM encoder_map[][1][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_P(PSTR("shortcut board\n"), false);
    return false;
}
#endif
