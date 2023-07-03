#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "common.h"
#include "thread.h"
#include "main.h"

const char *PROCFS_DEVICE = "/proc/led_demo";

int8_t led_r = LED_COMMAND_OFF;
int8_t led_g = LED_COMMAND_OFF;
int8_t led_b = LED_COMMAND_OFF;
enum led_mode_t led_mode = LED_MODE_AUTO;
volatile bool state_changed = true;

void write_tty(const char *buffer, int count);
bool receive_from_device(const char *device);
bool send_to_device(const char *device, char *send);
static void print_help(void);
static char* i2c_get_led_status(uint8_t value);

int main(void)
{
	printf("Welcome to the World of PHYTEC!\n\n");

	// const char buf[] = { "\nWelcome from console\n" };
	// write_tty(buf, strlen(buf));

	if (!thread_initialize())
		return 1;

	printf("\033[s");

	while (true)
	{
		if (state_changed)
		{
				/// https://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html
			printf("\033[u");
			print_help();
			fflush(stdout);
			state_changed = false;
		}

		int c = getch_non_blocking();
		if (c)
		{
			printf("\b      \r");
			state_changed = true;

			// if (c == 'r')
			// {
			// 	receive_from_device(device);
			// }
			// if (c == 'w')
			// {
			// 	srand(time(NULL));
			// 	int r = rand();
			// 	char buffer[128] = { 0 };
			// 	sprintf(buffer, "From user space %d.\n", r);
			// 	printf(buffer);
			// 	send_to_device(device, buffer);
			// }
			if (c == '1')
			{
				enqueue_command_send_leds_states(max(led_r - 1, 0), led_g, led_b);
			}
			else if (c == '2')
			{
				enqueue_command_send_leds_states(min(led_r + 1, LED_COMMAND_LIGHT_100), led_g, led_b);
			}
			else if (c == '3')
			{
				enqueue_command_send_leds_states(led_r, max(led_g - 1, 0), led_b);
			}
			else if (c == '4')
			{
				enqueue_command_send_leds_states(led_r, min(led_g + 1, LED_COMMAND_LIGHT_100), led_b);
			}
			else if (c == '5')
			{
				enqueue_command_send_leds_states(led_r, led_g, max(led_b - 1, 0));
			}
			else if (c == '6')
			{
				enqueue_command_send_leds_states(led_r, led_g, min(led_b + 1, LED_COMMAND_LIGHT_100));
			}
			else if (c == 'a')
			{
				enqueue_command_send_leds_mode((led_mode == LED_MODE_AUTO) ? LED_MODE_MANUAL : LED_MODE_AUTO);
			}
			else if (c == 'q')
			{
				printf("\nBye.\n");
				thread_destroy();
				return 0;
			}
		}

		usleep(10 * 1000); /// sleep 10 ms
	}

	return 0;
}

void print_help(void)
{
	printf("[1/2] LED Red:   %s  \n", i2c_get_led_status(led_r));
	printf("[3/4] LED Green: %s  \n", i2c_get_led_status(led_g));
	printf("[5/6] LED Blue:  %s  \n", i2c_get_led_status(led_b));
	printf("[a]   Auto mode: %s  \n", (led_mode == LED_MODE_AUTO) ? "on" : "off");
	printf("[q]   Quit program.\n");
	printf("Press suitable key: ");
}

static char* i2c_get_led_status(uint8_t value)
{
	switch (value)
	{
		case LED_COMMAND_OFF:       return "  0%";
		case LED_COMMAND_LIGHT_25:  return " 25%";
		case LED_COMMAND_LIGHT_50:  return " 50%";
		case LED_COMMAND_LIGHT_100: return "100%";
	}
	return "undef";
}

void write_tty(const char *buffer, int count)
{
	int out;
	out = open("/dev/console", O_RDWR);
	write(out, buffer, count);
	close(out);
}
