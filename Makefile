CFLAGS = -Wall -Wextra -std=gnu11 -Og -g
LDFLAGS = -L/opt/phytec-yogurt-vendor-xwayland/BSP-Yocto-FSL-i.MX8MP-PD21.1.3/sysroots/aarch64-phytec-linux/usr/lib
LDLIBS = -li2c -lpthread
BUILD_PATH=build

all: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) main.c thread.c common.c parse_commands.c -o ${BUILD_PATH}/imx8_a53_js_led_demo.bin
clean:
	rm -f ${BUILD_PATH}/imx8_a53_js_led_demo.bin
