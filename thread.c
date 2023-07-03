#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "thread.h"
#include "parse_commands.h"
#include "main.h"

static volatile enum command_t command = COMMAND_NONE;
static int8_t send_led_r;
static int8_t send_led_g;
static int8_t send_led_b;
static enum led_mode_t send_led_mode;

static volatile bool thread_exit = false;
static pthread_t thread;

static void* thread_loop(void *data __attribute__((unused)));
static bool receive_from_device(const char *device);
static bool send_to_device(const char *device, char *send);
static void command_send_leds_states(int8_t r, int8_t g, int8_t b);
static void command_send_leds_mode(enum led_mode_t led_mode);

bool thread_initialize(void)
{
	if (pthread_create(&thread, NULL, thread_loop, "thread"))
	{
		printf("Error: Can't create uart thread.\n");
		return false;
	}

  return true;
}

void thread_destroy(void)
{
	thread_exit = true;
	if (pthread_cancel(thread))
	{
		printf("Error: Can't destroy uart thread.\n");
	}
}

void* thread_loop(void *data __attribute__((unused)))
{
	while (!thread_exit)
	{
		if (command == COMMAND_SEND_LEDS_STATES)
		{
			command_send_leds_states(send_led_r, send_led_g, send_led_b);
			command = COMMAND_NONE;
		}
		else if (command == COMMAND_SEND_LEDS_MODE)
		{
			command_send_leds_mode(send_led_mode);
			command = COMMAND_NONE;
		}
		else
		{
  		receive_from_device(PROCFS_DEVICE);
    }

		usleep(30 * 1000); /// sleep 30 ms
	}

	return NULL;
}

void command_send_leds_states(int8_t r, int8_t g, int8_t b)
{
	char s[64];
	sprintf(s, "leds %d %d %d", r, g, b);
	send_to_device(PROCFS_DEVICE, s);
}

void command_send_leds_mode(enum led_mode_t led_mode)
{
	char s[64];
	sprintf(s, "leds_mode %d", (int)led_mode);
	send_to_device(PROCFS_DEVICE, s);
}

void enqueue_command_send_leds_states(int8_t r, int8_t g, int8_t b)
{
	send_led_r = r;
	send_led_g = g;
	send_led_b = b;
	command = COMMAND_SEND_LEDS_STATES;
}

void enqueue_command_send_leds_mode(enum led_mode_t led_mode)
{
	send_led_mode = led_mode;
	command = COMMAND_SEND_LEDS_MODE;
}

bool receive_from_device(const char *device)
{
	char data[256] = { 0 };
	FILE *f = fopen(device, "r");
	if (f == NULL)
	{
		printf("Can't open device %s.\n", device);
		return false;
	}

	int count = 0;
	int received;
	while ((received = fgetc(f)) != EOF)
		data[count++] = (char)received;
	fclose(f);

	char result[256] = { 0 };
	// printf("Received %d bytes: %s\n", count, data);
	parse_line(data, result);
	return true;
}

bool send_to_device(const char *device, char *send)
{
	FILE *f = fopen(device, "w");
	if (f == NULL)
	{
		printf("Can't open device %s.\n", device);
		return false;
	}

	fprintf(f, send);
	fclose(f);
	return true;
}
