#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>
#include <zmk/rgb_underglow.h>
#include <dt-bindings/zmk/hid_indicators.h>

static int caps_indicator_listener(const zmk_event_t *eh) {
    const struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    bool caps_on = ev->indicators & HID_INDICATOR_CAPS_LOCK;

    if (caps_on) {
        zmk_rgb_underglow_set_hsb((struct zmk_led_hsb){.h = 0, .s = 100, .b = 50});
    } else {
        zmk_rgb_underglow_set_hsb((struct zmk_led_hsb){.h = 0, .s = 0, .b = 0});
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(caps_indicator, caps_indicator_listener);
ZMK_SUBSCRIPTION(caps_indicator, zmk_hid_indicators_changed);
