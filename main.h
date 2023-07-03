#pragma once

#define LED_COMMAND_OFF       0
#define LED_COMMAND_LIGHT_25  1
#define LED_COMMAND_LIGHT_50  2
#define LED_COMMAND_LIGHT_100 3

extern const char *PROCFS_DEVICE;

enum led_mode_t { LED_MODE_AUTO, LED_MODE_MANUAL };

extern int8_t led_r;
extern int8_t led_g;
extern int8_t led_b;
extern enum led_mode_t led_mode;

extern volatile bool state_changed;
