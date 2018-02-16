#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  

#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
    
#ifndef PF_CAN
#define PF_CAN 29
#endif
    
#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif
    
int main(){
        int s;
        unsigned int nbytes,len;
        struct sockaddr_can addr;
        struct ifreq ifr;
        struct can_frame frame;
    
        s = socket(PF_CAN,SOCK_RAW,CAN_RAW);
    
        strcpy(ifr.ifr_name,"vcan0");
        ioctl(s,SIOCGIFINDEX,&ifr);
        printf("vcan0 can_ifindex = %x\n",ifr.ifr_ifindex);
    
        addr.can_family = AF_CAN;
        addr.can_ifindex =0;
        bind(s,(struct sockaddr*)&addr,sizeof(addr));
    
        nbytes = recvfrom(s,&frame,sizeof(struct can_frame),0,(struct sockaddr *)&addr,&len);
        
        ifr.ifr_ifindex = addr.can_ifindex;
        ioctl(s,SIOCGIFNAME,&ifr);
        printf("Received a CAN frame from interface %s\n",ifr.ifr_name);
        printf("frame message\n"
            "--can_id = %x\n"
            "--can_dlc = %x\n"
            "--data = %s\n",frame.can_id,frame.can_dlc,frame.data);
    
        return 0;
}
