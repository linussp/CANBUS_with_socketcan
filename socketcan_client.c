#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include <dlfcn.h>
#include <execinfo.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

#define StrLen 8

typedef struct{
	char name[StrLen];
	int id;
	char data[StrLen];
}SocketCan;

void AtteckerInsertedFunction(){
    	printf("Congratulations!\n");
    	printf("You have entered in the secret function!\n");
	printf("We change the Execution Flow\n");
	printf("%p\n", __builtin_frame_address(0));
	//printf("%p\n", __builtin_return_address(0));
	exit (0);
}

void Vulnerable(){
    char buffer[StrLen];

    printf("Enter some text:\n");
    scanf("%s", buffer);
    printf("You entered: %s\n", buffer);    
}


int main(){
        int s;
        unsigned long nbytes;
        struct sockaddr_can addr;
        struct ifreq ifr;
        struct can_frame frame;

        s = socket(PF_CAN,SOCK_RAW,CAN_RAW);

        strcpy((char *)(ifr.ifr_name),"vcan0");
        ioctl(s,SIOCGIFINDEX,&ifr);
        printf("vcan0 can_ifindex = %x\n",ifr.ifr_ifindex);

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(s,(struct sockaddr*)&addr,sizeof(addr));

///////////////////////////data attack/////////////////////////////////
		Vulnerable();
		SocketCan socketcan1;
		socketcan1.data;
		socketcan1.id = 0;
		gets(socketcan1.name);
		
		
		
        //frame.can_id = 0x123;
        //strcpy((char *)frame.data,"hello");
        frame.can_id = socketcan1.id;
        strcpy(frame.data, socketcan1.data);
        
        frame.can_dlc = strlen(frame.data);

        printf("Send a CAN frame to interface %s\n",ifr.ifr_name);
        printf("Send a CAN frame data : %s\n",frame.data);

        nbytes = sendto(s,&frame,sizeof(struct can_frame),0,(struct sockaddr*)&addr,sizeof(addr));
		printf("%p\n", __builtin_return_address(0));
        return 0;
}
