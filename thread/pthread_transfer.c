#include "link_list.h"
#include "data_global.h"

#define LEN_ENV 20
#define LEN_RFID 4 

extern int dev_uart_fd;

extern linklist linkHead;

extern pthread_cond_t cond_analysis;

extern pthread_mutex_t mutex_linklist;

void serial_init(int fd)
{
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag |= ( CLOCAL | CREAD );
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB; 
	options.c_iflag |= IGNPAR;
	options.c_iflag &= ~(ICRNL | IXON);
	options.c_oflag = 0;
	options.c_lflag = 0;

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	tcsetattr(fd,TCSANOW,&options);
}

void *pthread_transfer (void *arg)
{
	int i = 0, len;
	char flag = 0, check = 0,zigbee_insert_flag = 0;
	link_datatype buf;

	linkHead = CreateEmptyLinklist ();
#if 1
	sleep(2);
	if((dev_uart_fd = open(DEV_ZIGBEE, O_RDWR)) < 0)
	{
		perror ("open ttyUSB");
		puts("!!!Please insert your Zigbee device!");
	}
	serial_init (dev_uart_fd);

	printf ("pthread_transfer is ok\n");

#endif
    sendMsgQueue(MSG_M0,MSG_CONNECT_SUCCESS);
	while (1)
	{
		memset (&buf, 0, sizeof (link_datatype));
		read (dev_uart_fd, &check, 1);
        if (check == 'c')
        {
            sendMsgQueue(MSG_M0,MSG_CONNECT_SUCCESS);
        }
		if (check == 's')
		{
			check = 0;
			read (dev_uart_fd, &check, 1);
			if (check == 't')
			{
				check = 0;
				read (dev_uart_fd, &check, 1);
				if (check == ':')
				{
					check = 0;
					read (dev_uart_fd, &check, 1);
					if (check == 'e')
					{
						buf.msg_type = 'e';
						usleep(1);
						if ((len = read (dev_uart_fd, buf.text, LEN_ENV)) != LEN_ENV)
						{
							for (i = len; i < LEN_ENV; i++)
							{
								read (dev_uart_fd, buf.text+i, 1);
							}
						}
						flag = 1;
						//buf.text中的数据是结构体强转为字符串中的,对应的字符串的ascii字符可能已经超出了可打印字符的范围.
						//这样打印不能打印出来效果
					//	printf("\npthread_transfer buf from uart= %s\n",buf.text);
					}
					else if (check == 'r')
					{			
						buf.msg_type = 'r';
						usleep(1);
						if ((len = read (dev_uart_fd, buf.text, LEN_RFID)) != LEN_RFID)
						{
							for (i = len; i < LEN_RFID; i++)
							{
								read (dev_uart_fd, buf.text+i, 1);
							}
						}
						flag = 1;
					}
				}
			}
		}
		if (1 == flag)
		{
			pthread_mutex_lock (&mutex_linklist);
			if ((InsertLinknode(buf)) == -1)
			{
				pthread_mutex_unlock(&mutex_linklist);
				printf ("NONMEM\n");
			}
			pthread_mutex_unlock (&mutex_linklist);
			flag = 0;
			pthread_cond_signal (&cond_analysis);
		}
		
	}
	return 0;
}
