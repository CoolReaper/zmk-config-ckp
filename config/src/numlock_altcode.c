#define DT_DRV_COMPAT zmk_behavior_altcode_0248

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <drivers/behavior.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <zmk/hid_indicators.h>
#include <dt-bindings/zmk/hid_indicators.h>
#include <dt-bindings/zmk/keys.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static void tap_key(uint32_t usage) {
    zmk_hid_keyboard_press(usage);
    zmk_endpoints_send_report(HID_USAGE_KEY);
    zmk_hid_keyboard_release(usage);
    zmk_endpoints_send_report(HID_USAGE_KEY);
}

static int on_altcode_0248_binding_pressed(struct zmk_behavior_binding *binding,
                                            struct zmk_behavior_binding_event event) {
    bool numlock_was_on = zmk_hid_indicators_get_current_profile() & HID_INDICATOR_NUM_LOCK;

    if (!numlock_was_on) {
        tap_key(KP_NUM);
        k_msleep(20);
    }

    zmk_hid_keyboard_press(LALT);
    zmk_endpoints_send_report(HID_USAGE_KEY);
    tap_key(KP_N0);
    tap_key(KP_N2);
    tap_key(KP_N4);
    tap_key(KP_N8);
    zmk_hid_keyboard_release(LALT);
    zmk_endpoints_send_report(HID_USAGE_KEY);

    if (!numlock_was_on) {
        tap_key(KP_NUM);
    }

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_altcode_0248_binding_released(struct zmk_behavior_binding *binding,
                                             struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api altcode_0248_driver_api = {
    .binding_pressed = on_altcode_0248_binding_pressed,
    .binding_released = on_altcode_0248_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &altcode_0248_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
