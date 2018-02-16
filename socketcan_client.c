#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif
    
    int main()
    {
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
    
        frame.can_id = 0x123;
        strcpy((char *)frame.data,"hello");
        frame.can_dlc = strlen(frame.data);
    
        printf("Send a CAN frame from interface %s\n",ifr.ifr_name);
    
        nbytes = sendto(s,&frame,sizeof(struct can_frame),0,(struct sockaddr*)&addr,sizeof(addr));
        
        return 0;
}
