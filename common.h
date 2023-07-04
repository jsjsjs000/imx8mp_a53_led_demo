#pragma once

#define min(a,b) \
	({__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })
#define max(a,b) \
	({__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })

extern char getch_blocking(void);
extern char getch_non_blocking(void);
extern void calculate_pi(void);
