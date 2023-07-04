#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "common.h"

char getch_blocking(void)
{
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("getch() tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("getch() tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
 		perror("getch() read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("getch() tcsetattr ~ICANON");

	return buf;
}

char getch_non_blocking(void)
{
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("getch() tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("getch() tcsetattr ICANON");

	int count;
	if (ioctl(0, FIONREAD, &count) < 0)
		perror("getch() ioctl");
	if (count > 0)
	{
		if (read(0, &buf, 1) < 0)
	 		perror("getch() read()");
	}

	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("getch() tcsetattr ~ICANON");

	return buf;
}

void calculate_pi(void)
{
	struct timeval start_tv;
	struct timeval end_tv;
	gettimeofday(&start_tv, NULL);

	double p16 = 1, pi = 0, precision = 100000000;	// pi calculate
	for(int k = 0; k <= precision; k++)
	{
		pi += 1.0 / p16 * (4.0 / (8 * k + 1) - 2.0 / (8 * k + 4) - 1.0 / (8 * k + 5) - 1.0 / (8 * k + 6));
		p16 *= 16;
	}
	printf("pi = %.9f\n", pi);

	usleep(100 * 1000);										// 100 ms

	gettimeofday(&end_tv, NULL);
}
