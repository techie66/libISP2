#include "../isp2.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main() {
	int fd;
	isp2_t data;

	fd = open("lc-2.bin",O_RDONLY);
	if (fd == -1) {
		printf("Open error:(%d) %s\n",errno,strerror(errno));
	}
	
	while(ISP2::isp2_read(fd,data) != -1) {
		printf("Packet length: %d\n",data.packet_length);
		printf("Lambda: %f\n",data.lambda/1000.0);
		printf("Status: %d\n",data.status);
		printf("afr_m: %f\n",data.afr_multiplier/10.0);
		if(data.is_recording){printf("Recording\n");}
		if(data.is_sensor_data){printf("Sensor Data\n");}
		if(data.sender_can_log){printf("Can Log\n");}
	}
    return 0;
}
