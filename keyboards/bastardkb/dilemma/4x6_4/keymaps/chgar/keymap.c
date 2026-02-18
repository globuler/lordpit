/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_BASEW,
    LAYER_NAV,
    LAYER_NUM,
    LAYER_SYM,
    LAYER_MOUSE,
    LAYER_POINTER,
};

enum {
    TD_C,
    TD_X,
    TD_V,
};

typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_DOUBLE_SINGLE_TAP,
    TD_DOUBLE_TAP,
} td_state_t;

typedef struct {
    uint16_t keycode;
} td_copy_data_t;

static td_copy_data_t td_c_data = {KC_C};
static td_copy_data_t td_x_data = {KC_X};
static td_copy_data_t td_v_data = {KC_V};

td_state_t current_dance(tap_dance_state_t *state) {
    if (state->count == 1) return TD_SINGLE_TAP;
    if (state->count == 2) {
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        return TD_DOUBLE_TAP;
    }
    return TD_NONE;
}

bool is_mac(void) {
    return get_highest_layer(default_layer_state) == LAYER_BASE;
}

void td_copypaste_finished(tap_dance_state_t *state, void *user_data) {
    td_copy_data_t *data = (td_copy_data_t *)user_data;
    switch (current_dance(state)) {
        case TD_SINGLE_TAP:
            register_code(data->keycode);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            // fast typing: sends two single taps instead of the double tap action
            tap_code(data->keycode);
            register_code(data->keycode);
            break;
        case TD_DOUBLE_TAP:
            if (is_mac()) tap_code16(LGUI(data->keycode));
            else          tap_code16(LCTL(data->keycode));
            break;
        default:
            break;
    }
}


void td_copypaste_reset(tap_dance_state_t *state, void *user_data) {
    td_copy_data_t *data = (td_copy_data_t *)user_data;
    switch (current_dance(state)) {
        case TD_SINGLE_TAP:
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code(data->keycode);
            break;
        default:
            break;
    }
}


tap_dance_action_t tap_dance_actions[] = {
    [TD_C] = { .fn = {NULL, td_copypaste_finished, td_copypaste_reset}, .user_data = &td_c_data },
    [TD_X] = { .fn = {NULL, td_copypaste_finished, td_copypaste_reset}, .user_data = &td_x_data },
    [TD_V] = { .fn = {NULL, td_copypaste_finished, td_copypaste_reset}, .user_data = &td_v_data },
};

typedef enum {
    D_F_ESC,
    D_F_ESCM,
    J_K_ENT,
    J_K_ENTM,
    F_J_CPL
} combos;

const uint16_t PROGMEM comboesc[] = {SFT_T(KC_F), CTL_T(KC_D), COMBO_END};
const uint16_t PROGMEM comboescm[] = {SFT_T(KC_F), GUI_T(KC_D), COMBO_END};

const uint16_t PROGMEM comboent[] = {SFT_T(KC_J), CTL_T(KC_K), COMBO_END};
const uint16_t PROGMEM comboentm[] = {SFT_T(KC_J), GUI_T(KC_K), COMBO_END};

const uint16_t PROGMEM combocaps[] = {SFT_T(KC_F), SFT_T(KC_J), COMBO_END};

combo_t key_combos[] = {
    [D_F_ESC] = COMBO(comboesc, KC_ESC),
    [D_F_ESCM] = COMBO(comboescm, KC_ESC),
    [J_K_ENT] = COMBO(comboent, KC_ENT),
    [J_K_ENTM] = COMBO(comboentm, KC_ENT),
    [F_J_CPL] = COMBO(combocaps, KC_CAPS),
};

#define WIN DF(LAYER_BASEW)
#define MAC DF(LAYER_BASE)
#define NAV LT(LAYER_NAV, KC_BSPC)
#define MOUSE LT(LAYER_MOUSE, KC_TAB)
#define NUM LT(LAYER_NUM, KC_SPC)
#define SYM LT(LAYER_SYM, KC_ENT)
#define PT LT(LAYER_POINTER, KC_NO)

// Automatically enable sniping-mode on the pointer layer.
// #define DILEMMA_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────╮
        KC_ESC,      XXXXXXX,     KC_MPRV,       KC_MPLY,      KC_MNXT, KC_MUTE,    KC_VOLD,      KC_VOLU,      XXXXXXX,      XXXXXXX,         RGB_VAD, RGB_VAI,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
        KC_EQL,         KC_Q,        KC_W,          KC_E,         KC_R,    KC_T,       KC_Y,         KC_U,         KC_I,         KC_O,            KC_P, KC_MINS,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       KC_LSFT, LCTL_T(KC_A), LALT_T(KC_S), LGUI_T(KC_D), LSFT_T(KC_F),    KC_G,       KC_H, LSFT_T(KC_J), LGUI_T(KC_K), LALT_T(KC_L), LCTL_T(KC_SCLN), KC_QUOT,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       KC_LCTL,         KC_Z,     TD(TD_X),     TD(TD_C),     TD(TD_V),    KC_B,       KC_N,         KC_M,      KC_COMM,       KC_DOT,         KC_SLSH, KC_BSLS,
  // ╰──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────╯
                             HYPR_T(KC_NO),          NAV,        MOUSE,      PT,    XXXXXXX,          SYM,          NUM,          WIN
  //                        ╰───────────────────────────────────────────────────╯ ╰───────────────────────────────────────────────────╯
  ),


  [LAYER_BASEW] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────╮
        KC_ESC,      XXXXXXX,     KC_MPRV,       KC_MPLY,      KC_MNXT, KC_MUTE,    KC_VOLD,      KC_VOLU,      XXXXXXX,      XXXXXXX,         RGB_VAD, RGB_VAI,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
        KC_EQL,         KC_Q,        KC_W,          KC_E,         KC_R,    KC_T,       KC_Y,         KC_U,         KC_I,         KC_O,            KC_P, KC_MINS,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       KC_LSFT, LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F),    KC_G,       KC_H, LSFT_T(KC_J), LCTL_T(KC_K), LALT_T(KC_L), LGUI_T(KC_SCLN), KC_QUOT,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       KC_LCTL,         KC_Z,     TD(TD_X),     TD(TD_C),     TD(TD_V),    KC_B,       KC_N,         KC_M,      KC_COMM,       KC_DOT,         KC_SLSH, KC_BSLS,
  // ╰──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────╯
                             HYPR_T(KC_NO),          NAV,        MOUSE,      PT,    XXXXXXX,          SYM,          NUM,          MAC
  //                        ╰───────────────────────────────────────────────────╯ ╰───────────────────────────────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────╮
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,        KC_7,          KC_8,        KC_9,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      KC_LBRC,        KC_4,          KC_5,        KC_6,  KC_RBRC,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,        KC_1,          KC_2,        KC_3,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────╯
                                  XXXXXXX,          KC_0,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,          NUM,      XXXXXXX
  //                        ╰───────────────────────────────────────────────────╯ ╰───────────────────────────────────────────────────╯
  ),

  [LAYER_MOUSE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────╮
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      KC_LBRC,     MS_BTN3,       MS_BTN1,     MS_BTN2,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────╯
                                  XXXXXXX,       XXXXXXX,       MOUSE,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX
  //                        ╰───────────────────────────────────────────────────╯ ╰───────────────────────────────────────────────────╯
  ),

  [LAYER_SYM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────╮
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,     KC_AMPR,       KC_ASTR,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      KC_LCBR,      KC_DLR,       KC_PERC,     KC_CIRC,  KC_RCBR,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      KC_TILD,      KC_EXLM,        KC_AT,     KC_HASH,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────╯
                                  XXXXXXX,       KC_LPRN,     KC_RPRN,  XXXXXXX,    XXXXXXX,          SYM,      XXXXXXX,      XXXXXXX
  //                        ╰───────────────────────────────────────────────────╯ ╰───────────────────────────────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────╮
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,     XXXXXXX,       XXXXXXX,     XXXXXXX,  XXXXXXX,    KC_LEFT,      KC_DOWN,        KC_UP,     KC_RIGHT,         XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────┤
       XXXXXXX,      XXXXXXX,     XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,    KC_HOME,      KC_PGDN,      KC_PGUP,       KC_END,         XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────╯
                                  XXXXXXX,            NAV,    XXXXXXX,  XXXXXXX,    XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX
  //                        ╰───────────────────────────────────────────────────╯ ╰───────────────────────────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, EE_CLR,  QK_BOOT,    QK_BOOT, EE_CLR,  SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         XXXXXXX, KC_BTN2, KC_BTN1, KC_BTN3,    KC_BTN3, KC_BTN1, KC_BTN2, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLEE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif // RGB_MATRIX_ENABLE

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    if (!rgb_matrix_is_enabled()) {
        return false;
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        bool is_underglow = (g_led_config.flags[i] & LED_FLAG_INDICATOR);
        uint8_t row = 0xFF;
        uint8_t col = 0xFF;

        for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                if (g_led_config.matrix_co[r][c] == 1) {
                    row = r;
                    col = c;
                    break;
                }
            }
            if (row != 0xFF) break;
        }

        if (is_underglow) {
            switch(layer) {
                case LAYER_BASE:
                case LAYER_BASEW:
                    rgb_matrix_set_color(i, RGB_WHITE);
                    break;
                case LAYER_NAV:
                    rgb_matrix_set_color(i, RGB_CYAN);
                    break;
                case LAYER_NUM:
                    rgb_matrix_set_color(i, RGB_BLUE);
                    break;
                case LAYER_SYM:
                    rgb_matrix_set_color(i, RGB_GREEN);
                    break;
                case LAYER_MOUSE:
                    rgb_matrix_set_color(i, RGB_YELLOW);
                    break;
                case LAYER_POINTER:
                    rgb_matrix_set_color(i, RGB_RED);
                    break;
            }
            continue;
        }

        if (row == 0xFF || col == 0xFF) {
            continue;
        }

        uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col, row});
        if (keycode == KC_TRNS || keycode == KC_NO || keycode == XXXXXXX) {
            continue;
        }

        switch (layer) {
            case LAYER_BASE:
            case LAYER_BASEW: {
                bool is_homerow_mod = false;
                if (keycode == LSFT_T(KC_F) || keycode == LSFT_T(KC_J)) {
                    rgb_matrix_set_color(i, RGB_BLUE);
                    is_homerow_mod = true;
                }
                else if (keycode == LGUI_T(KC_D) || keycode == LGUI_T(KC_K) ||
                        keycode == LCTL_T(KC_D) || keycode == LCTL_T(KC_K)) {
                    rgb_matrix_set_color(i, RGB_MAGENTA);
                    is_homerow_mod = true;
                }
                else if (keycode == LALT_T(KC_S) || keycode == LALT_T(KC_L)) {
                    rgb_matrix_set_color(i, RGB_GREEN);
                    is_homerow_mod = true;
                }
                else if (keycode == LCTL_T(KC_A) || keycode == LCTL_T(KC_SCLN) ||
                        keycode == LGUI_T(KC_A) || keycode == LGUI_T(KC_SCLN)) {
                    rgb_matrix_set_color(i, RGB_RED);
                    is_homerow_mod = true;
                }

                if (!is_homerow_mod) {
                    rgb_matrix_set_color(i, RGB_WHITE);
                }
                break;
            }
            case LAYER_NAV:
                rgb_matrix_set_color(i, RGB_CYAN);
                break;
            case LAYER_NUM:
                rgb_matrix_set_color(i, RGB_BLUE);
                break;
            case LAYER_SYM:
                rgb_matrix_set_color(i, RGB_GREEN);
                break;
            case LAYER_MOUSE:
                rgb_matrix_set_color(i, RGB_YELLOW);
                break;
            case LAYER_POINTER:
                rgb_matrix_set_color(i, RGB_RED);
                break;
        }
    }

    return false;
}
#endif
