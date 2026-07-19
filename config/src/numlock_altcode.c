// src/numlock_altcode.c
#include <zephyr/kernel.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <zmk/hid_indicators.h>       // para ler o estado atual
#include <dt-bindings/zmk/hid_indicators.h>
#include <dt-bindings/zmk/keys.h>

static void tap_key(uint32_t usage) {
    zmk_hid_keyboard_press(usage);
    zmk_endpoints_send_report(HID_USAGE_KEY);
    zmk_hid_keyboard_release(usage);
    zmk_endpoints_send_report(HID_USAGE_KEY);
}

static int altcode_0248_pressed(struct zmk_behavior_binding *binding,
                                 struct zmk_behavior_binding_event event) {
    bool numlock_was_on = /* estado atual do indicador tem HID_INDICATOR_NUM_LOCK, confirmar getter */;

    if (!numlock_was_on) {
        tap_key(KP_NUM);
        k_msleep(20); // dar tempo ao host para confirmar o toggle antes de continuarmos
    }

    zmk_hid_keyboard_press(LALT);
    zmk_endpoints_send_report(HID_USAGE_KEY);
    tap_key(KP_N0); tap_key(KP_N2); tap_key(KP_N4); tap_key(KP_N8);
    zmk_hid_keyboard_release(LALT);
    zmk_endpoints_send_report(HID_USAGE_KEY);

    if (!numlock_was_on) {
        tap_key(KP_NUM);
    }

    return ZMK_BEHAVIOR_OPAQUE;
}

// registar como zmk,behavior-* com binding-cells = <0>, ligar binding_pressed a altcode_0248_pressed
