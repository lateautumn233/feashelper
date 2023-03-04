#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>

#include "include/fpsgo.h"

using namespace std;

struct fpsgo_config {
	uint64_t ioctl;
	uint32_t tid;
	uint32_t start;
	uint64_t connectedAPI;
	uint32_t identifier;
} fc;

int fpsgo_ioctl(unsigned long fpsgo_ioctl, uint32_t tid, uint32_t start, uint64_t connectedAPI, uint32_t identifier) {
	int fd = 0, ret = 0;
	char perf_dev[] = "/proc/perfmgr/perf_ioctl";

	uint64_t ioctl_cmd = 0;
	bool ioctl_cmd_ret = false;
	struct _FPSGO_PACKAGE fpsgoPkg = {};
	// fc.ioctl = ioctl;
	// fc.tid =
	// fc.start =
	// fc.connectedAPI =
	// fc.identifier =
	
	memset(&fpsgoPkg, 0, sizeof(_FPSGO_PACKAGE));

	switch (fpsgo_ioctl){
		case 1:  //connect
			fpsgoPkg.tid = tid;
			fpsgoPkg.connectedAPI = connectedAPI;
			fpsgoPkg.identifier = identifier;
			ioctl_cmd = FPSGO_QUEUE_CONNECT;
			printf("skkk: fpsgo connect arg, cmd=%lu tid=%u api=%u id=%llu\n",
				ioctl_cmd, fpsgoPkg.tid, fpsgoPkg.connectedAPI, fpsgoPkg.identifier);
			break;
		case 2: // add queue
			fpsgoPkg.tid = tid;
			fpsgoPkg.start = start;
			fpsgoPkg.identifier = identifier;
			ioctl_cmd  = FPSGO_QUEUE;
			printf("skkk: fpsgo add queue arg, cmd=%lu start=%u tid=%u id=%llu\n",
				ioctl_cmd, fpsgoPkg.start, fpsgoPkg.tid, fpsgoPkg.identifier);
			break;
		case 3: // delete queue
			fpsgoPkg.tid = tid;
			fpsgoPkg.start = start;
			fpsgoPkg.identifier = identifier;
			ioctl_cmd = FPSGO_DEQUEUE;
			printf("skkk: fpsgo delete queue arg, cmd=%lu start=%u tid=%u id=%llu\n",
				ioctl_cmd, fpsgoPkg.start, fpsgoPkg.tid, fpsgoPkg.identifier);
			break;
		case 4:
			ioctl_cmd_ret = true;
			ioctl_cmd = FPSGO_GET_FPS;
			printf("skkk: FPSGO_GET_FPS, cmd=%lu", ioctl_cmd);
			break;
		default:
			printf("skkk: arg error!\n");
			ret = 1;
			goto exit;
	}

	fd = open(perf_dev, O_WRONLY | O_NONBLOCK);
	//printf("skkk: open '%s'\n", perf_dev);
	if (fd < 0) {
		//printf("skkk: Cannot open %s!\n",  perf_dev);
		ret = 2;
		goto exit;
	}
	printf("skkk: exec ioctl...\n");
	ret = ioctl(fd, ioctl_cmd, &fpsgoPkg);
	if (ret < 0) {
		//printf("skkk: ioctl failed.\n");
		ret = 3;
		goto exit;
	}
	
	if (ioctl_cmd_ret) {
		switch(fpsgo_ioctl) {
			case 4:
				printf("skkk: fpsgo_ret tid=%u fps=%u", fpsgoPkg.tid, fpsgoPkg.value1);
				break;
		}
	}
	
exit:
	if (fd > 0) close(fd);
	//printf("skkk: ioctl ret=%d\n", ret);
	return ret;
}

/*
int main(int argc, char *argv[]) {
	int ret = 0;
	memset(&fc, 0, sizeof(fpsgo_config));
	for (int i = 0; i < argc; i++) {
		char *arg = argv[i];
		if (strncmp(arg, "cmd=", 4) == 0) {
			string _arg = arg;
			if (!_arg.empty()) fc.ioctl = strtoul(_arg.substr(4, _arg.length()).c_str(), nullptr, 10);
		} else if (strncmp(arg, "start=", 6) == 0) {
			string _arg = arg;
			if (!_arg.empty()) fc.start = strtoul(_arg.substr(6, _arg.length()).c_str(), nullptr, 10);
		} else if (strncmp(arg, "tid=", 4) == 0) {
			string _arg = arg;
			if (!_arg.empty()) fc.tid = strtoul(_arg.substr(4, _arg.length()).c_str(), nullptr, 10);
		} else if (strncmp(arg, "api=", 4) == 0) {
			string _arg = arg;
			if (!_arg.empty()) fc.connectedAPI = strtoul(_arg.substr(4, _arg.length()).c_str(), nullptr, 10);
		} else if (strncmp(arg, "id=", 3) == 0) {
			string _arg = arg;
			if (!_arg.empty()) fc.identifier = strtoul(_arg.substr(3, _arg.length()).c_str(), nullptr, 10);
		}
	}
	ret = fpsgo_ioctl(1, 0, 0, 0, 0);
	return ret;
}
*/