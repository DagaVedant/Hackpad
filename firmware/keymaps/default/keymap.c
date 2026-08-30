#include QMK_KEYBOARD_H
#include "globe.h"

enum layer_names {
    _BASE,
    _SET,
};

enum custom_keycodes {
    TM_HRU = SAFE_RANGE,
    TM_HRD,
    TM_MNU,
    TM_MND,
};

static uint32_t clock_base = 0;
static uint32_t clock_mark = 0;

static uint32_t clock_now(void) {
    return (clock_base + (timer_read32() - clock_mark) / 1000) % 86400;
}

static void clock_shift(int32_t delta) {
    int32_t now = (int32_t)clock_now();
    clock_base = (uint32_t)(((now + delta) % 86400 + 86400) % 86400);
    clock_mark = timer_read32();
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        LGUI(KC_1), LGUI(KC_2), LGUI(KC_3),
        KC_F11,     LSG(KC_S),  KC_CALC,
        LCTL(KC_C), LCTL(KC_V), LCTL(KC_Z)
    ),
    [_SET] = LAYOUT(
        TM_HRD,     TG(_SET),   TM_HRU,
        KC_NO,      KC_NO,      KC_NO,
        KC_NO,      KC_NO,      KC_NO
    )
};

const uint16_t PROGMEM encoder_map[][1][2] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_SET]  = { ENCODER_CCW_CW(TM_MND, TM_MNU) },
};

enum combo_names {
    CB_SET,
};

const uint16_t PROGMEM set_combo[] = {LGUI(KC_1), LGUI(KC_3), COMBO_END};

combo_t key_combos[] = {
    [CB_SET] = COMBO(set_combo, TG(_SET)),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case TM_HRU:
            clock_shift(3600);
            return false;
        case TM_HRD:
            clock_shift(-3600);
            return false;
        case TM_MNU:
            clock_shift(60);
            return false;
        case TM_MND:
            clock_shift(-60);
            return false;
    }
    return true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    clock_mark = timer_read32();
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    static uint8_t frame = 0;
    static uint32_t last_step = 0;
    static char buf[12];

    if (timer_elapsed32(last_step) > 80) {
        last_step = timer_read32();
        frame = (frame + 1) % GLOBE_FRAMES;
    }

    for (uint8_t y = 0; y < GLOBE_SIZE; y++) {
        for (uint8_t x = 0; x < GLOBE_SIZE; x++) {
            uint8_t packed = pgm_read_byte(&globe[frame][y * 4 + (x >> 3)]);
            oled_write_pixel(x, y, (packed >> (7 - (x & 7))) & 1);
        }
    }

    uint32_t now = clock_now();
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", (unsigned)(now / 3600),
             (unsigned)((now / 60) % 60), (unsigned)(now % 60));
    oled_set_cursor(7, 1);
    oled_write(buf, false);
    oled_set_cursor(7, 2);
    oled_write(layer_state_is(_SET) ? "set time" : "        ", false);

    return false;
}
#endif
