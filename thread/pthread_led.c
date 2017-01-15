#include "data_global.h"

extern unsigned char dev_led_mask;
extern int dev_led_fd;

extern pthread_mutex_t mutex_led;

extern pthread_cond_t cond_led;

void *pthread_led (void *arg)
{
	int led_no;
	char * buff = NULL;
	unsigned char led_set;
#if 0
	if ((dev_led_fd = open (DEV_LED, O_RDWR | O_NONBLOCK)) < 0 )
	{
		printf ("Cann't open file /dev/led\n");
		exit (-1);
	}
#endif
	if ((dev_led_fd = open ("/sys/class/leds/led2/brightness", O_RDWR)) < 0)
	{
		printf ("Cann't open file /sys/class/leds/led2/brightness\n");
		exit (-1);
	}
	printf ("pthread_led is ok\n");
	while (1)
	{
		pthread_mutex_lock (&mutex_led);
		pthread_cond_wait (&cond_led, &mutex_led);
		led_set = dev_led_mask;
		pthread_mutex_unlock (&mutex_led);
		printf ("pthread_led is wake up\n");
		printf ("led_set = %#x\n",led_set);
		if (led_set == 0x11)
		{
			if ((dev_led_fd = open ("/sys/class/leds/led2/brightness", O_RDWR)) < 0)
			{
				printf ("Cann't open file /sys/class/leds/led2/brightness\n");
				exit (-1);
			}
			buff = "1";
			write(dev_led_fd, buff, 1);
			printf("fs4412 led2 on\n");
			close(dev_led_fd);
		}
		if (led_set == 0x10)
		{
			if ((dev_led_fd = open ("/sys/class/leds/led2/brightness", O_RDWR)) < 0)
			{
				printf ("Cann't open file /sys/class/leds/led2/brightness\n");
				exit (-1);
			}
			buff = "0";
			write(dev_led_fd, buff, 1);
			printf("fs4412 led2 off\n");
			close(dev_led_fd);
		}
		if (led_set == 0x22)
		{
			if ((dev_led_fd = open ("/sys/class/leds/led3/brightness", O_RDWR)) < 0)
			{
				printf ("Cann't open file /sys/class/leds/led3/brightness\n");
				exit (-1);
			}
			buff = "1";
			write(dev_led_fd, buff, 1);
			printf("fs4412 led3 on\n");
			close(dev_led_fd);
		}
		if (led_set == 0x20)
		{
			if ((dev_led_fd = open ("/sys/class/leds/led3/brightness", O_RDWR)) < 0)
			{
				printf ("Cann't open file /sys/class/leds/led3/brightness\n");
				exit (-1);
			}
			buff = "0";
			write(dev_led_fd, buff, 1);
			printf("fs4412 led3 off\n");
			close(dev_led_fd);
		}
#if 0
		switch (led_set & 0xf0)
		{
		case 0x10:
			{
				led_no = 1;
				break;
			}
		case 0x20:
			{
				led_no = 2;
				break;
			}
		case 0x40:
			{
				led_no = 3;
				break;
			}
		case 0x80:
			{
				led_no = 4;
				break;
			}
		default :break;
		}
		led_set &= 0x0f;
		if (led_set & (0x1 <<(led_no-1)))
		{
			ioctl (dev_led_fd, LED_ON, led_no);
		}
		else
		{
			ioctl (dev_led_fd, LED_OFF, led_no);
		}
#endif
	}
}
