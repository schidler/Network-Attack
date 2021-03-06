#include <stdio.h>
#include <libnet.h>
#include <string.h>
int main()
{
	libnet_t *l;
	libnet_ptag_t t;
	char *payLoad = NULL;
	char *device = "wlan0";
	unsigned short payLoadS = 0;
	unsigned int srcIp = 0, dstIp = 0;
	char *srcIpStr = "192.168.0.116", *dstIpStr = "211.69.143.10";
	unsigned short srcPrt = 2323, dstPrt = 80;
	char errBuf[LIBNET_ERRBUF_SIZE];
	l = libnet_init(LIBNET_RAW4, device, errBuf);	
	if(NULL == l){
		fprintf(stderr, "Fail: %s\n", errBuf);
		exit(1);
	}
	srcIp = libnet_name2addr4(l, srcIpStr, LIBNET_DONT_RESOLVE);
	if(-1 == srcIp){
		fprintf(stderr, "Fail %s\n", srcIpStr);
		exit(1);
	}
	dstIp = libnet_name2addr4(l, dstIpStr, LIBNET_DONT_RESOLVE);
	if(-1 == dstIp){
		fprintf(stderr, "Fail %s\n", dstIpStr);
	}
	t = libnet_build_tcp(srcPrt, dstPrt, 0x01010101, 0x02020202, TH_SYN, 32767,\
			 0, 10, LIBNET_TCP_H + payLoadS, payLoad, payLoadS, l, 0);
	if(-1 == t){
		fprintf(stderr, "%s\n", libnet_geterror(l));
		exit(1);
	}
	t = libnet_build_ipv4(LIBNET_IPV4_H + LIBNET_TCP_H + payLoadS, 0, 242, 0, 64,\
			 IPPROTO_TCP, 0, srcIp, dstIp, NULL, 0, l, 0);
	if(-1 == t){
		fprintf(stderr, "%s\n", libnet_geterror(l));
		exit(-1);
	}
	int c = libnet_write(l);
	if(-1 == c){
		fprintf(stderr, "%s\n", libnet_geterror(l));
		exit(1);
	}else{
		printf("%d data send to internet\n", c);
	}
	libnet_destroy(l);
	return 0;
}
