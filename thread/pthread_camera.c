#include "data_global.h"

extern char dev_camera_mask;
extern int dev_camera_fd;

extern pthread_mutex_t mutex_camera;

extern pthread_cond_t cond_camera;

void *pthread_camera (void *arg)
{
	unsigned char picture = 0;
//	char *buf = NULL;

	if ((dev_camera_fd = open (DEV_CAMERA, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	{
		printf ("Cann't open file /tmp/webcam\n");
		exit (-1);
	}
	printf ("pthread_camera is ok\n");
#if 0
	if ((buf = (char *)malloc(10)) == NULL)
	{
		printf("In pthread_camera malloc failed!\n");
		return NULL;
	}
#endif
	while (1)	
	{
		pthread_mutex_lock (&mutex_camera);
		pthread_cond_wait (&cond_camera, &mutex_camera);
		picture = dev_camera_mask;
		printf("dev_camera_mask = %d\n", dev_camera_mask);
		pthread_mutex_unlock (&mutex_camera);

		switch (picture){
		case 1:{
				   //		buf = "one";
				   write (dev_camera_fd, "one", 3);		
				   break;	
			   }	
		case 3:{
				   //		buf = "three";
				   write (dev_camera_fd, "three", 5);		
				   break;
			   }
		case 5:{
				   //		buf = "five";
				   write (dev_camera_fd, "five", 4);
				   break;
			   }
		case 7:{
				   //		buf = "seven";
				   write (dev_camera_fd, "seven", 5);
				   break;
			   }
		case 9:{
				   //		buf = "nine";
				   write (dev_camera_fd, "nine", 4);
				   break;
			   }
		default:{
					break;
				}
		}
	}
}
