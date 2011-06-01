/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 *
 *  Modified and hard-coded for my own camera testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h> 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>


#define V4L2_MT9P031_TEST_PATTERN_CONTROL	(V4L2_CID_PRIVATE_BASE + 20)
#define V4L2_MT9P031_TEST_PATTERN_GREEN		(V4L2_CID_PRIVATE_BASE + 21)
#define V4L2_MT9P031_TEST_PATTERN_RED		(V4L2_CID_PRIVATE_BASE + 22)
#define V4L2_MT9P031_TEST_PATTERN_BLUE		(V4L2_CID_PRIVATE_BASE + 23)
#define V4L2_MT9P031_TEST_PATTERN_BAR_WIDTH	(V4L2_CID_PRIVATE_BASE + 24)

struct vid_ctrl {
	int id;
	char name[32];
	char idstr[64];
};

#define HFLIP 		0
#define VFLIP		1
#define TEST_PATTERN	2
#define GREEN		3
#define RED		4
#define BLUE		5
#define BAR_WIDTH	6
#define NUM_CTRLS	7

struct vid_ctrl ctrls[NUM_CTRLS] = {
	{ V4L2_CID_HFLIP, "Mirror", "V4L2_CID_HFLIP" },
	{ V4L2_CID_VFLIP, "Flip", "V4L2_CID_VFLIP" },
	{ V4L2_MT9P031_TEST_PATTERN_CONTROL, "Test Pattern", "V4L2_MT9P031_TEST_PATTERN_CONTROL" },
	{ V4L2_MT9P031_TEST_PATTERN_GREEN, "Green", "V4L2_MT9P031_TEST_PATTERN_GREEN" },
	{ V4L2_MT9P031_TEST_PATTERN_RED, "Red", "V4L2_MT9P031_TEST_PATTERN_RED" },
	{ V4L2_MT9P031_TEST_PATTERN_BLUE, "Blue", "V4L2_MT9P031_TEST_PATTERN_BLUE" },
	{ V4L2_MT9P031_TEST_PATTERN_BAR_WIDTH, "Bar Width", "V4L2_MT9P031_TEST_PATTERN_BAR_WIDTH" }
};


void set_control(int fd, int id, int val, const char *idstr)
{
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;

	memset(&queryctrl, 0, sizeof (queryctrl));
	queryctrl.id = id;

	if (-1 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
		if (errno != EINVAL) {
		        perror("VIDIOC_QUERYCTRL");
		} 
		else {
		        printf("%s is not supported\n", idstr);
		}
	} 
	else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
		printf("%s is not supported\n", idstr);
	} 
	else {
		memset(&control, 0, sizeof (control));
		control.id = id;
		control.value = val;

		if (-1 == ioctl(fd, VIDIOC_S_CTRL, &control))
		        perror("VIDIOC_S_CTRL");
	}
}

void dump_current_settings(int fd)
{
	int i;
	struct v4l2_control control;

	for (i = 0; i < NUM_CTRLS; i++) {
		control.id = ctrls[i].id;
		control.value = 0;

		if (-1 == ioctl(fd, VIDIOC_G_CTRL, &control))
		        perror("VIDIOC_G_CTRL");
		else
			printf("%s: %u\n", ctrls[i].name, control.value);
	}
}

static void usage(char **argv)
{
	printf("Usage: %s [options]\n\n"
		"Options:\n"
		"-m<n>	Mirror (horizontal flip) (0 or 1)\n"
		"-f<n>	Flip (vertical flip) (0 or 1)\n"
		"-t<n>	Test pattern (0 - 8)\n"
		"-g<n>	Green test pattern (0 - 4095)\n"
		"-r<n>	Red test pattern (0 - 4095)\n"
		"-b<n>	Blue test pattern (0 - 4095)\n"
		"-w<n>	Bar width for test pattern (odd or zero, 0-4095)\n"
		"-d	Dump current settings\n"
		"-h	Print this message\n",
		argv[0]);
}

int main(int argc, char **argv)
{
	int fd, opt;
	int dump_settings;
	int green, red, blue, bar_width;
	int test_pattern, hflip, vflip;

	hflip = -1;
	vflip = -1;
	test_pattern = -1;
	green = -1;
	red = -1;
	blue = -1;
	bar_width = -1;	
	dump_settings = 0;
	
	while ((opt = getopt(argc, argv, "m:f:t:g:r:b:w:dh")) != -1) {
		switch (opt) {
		case 'm':
			hflip = atol(optarg);

			if (hflip)
				hflip = 1;
			else
				hflip = 0;

			break;

		case 'f':
			vflip = atol(optarg);

			if (vflip)
				vflip = 1;
			else
				vflip = 0;

			break;

		case 't':
			test_pattern = atol(optarg);

			if (test_pattern < 0 || test_pattern > 8)
				test_pattern = -1;

			break;

		case 'g':
			green = atol(optarg);

			if (green < 0 || green > 4095)
				green = -1;

			break;

		case 'r':
			red = atol(optarg);

			if (red < 0 || red > 4095)
				red = -1;

			break;

		case 'b':
			blue = atol(optarg);

			if (blue < 0 || blue > 4095)
				blue = -1;

			break;

		case 'w':
			bar_width = atol(optarg);

			if (bar_width < 0 || bar_width > 4095)
				bar_width = -1;
			else if (bar_width > 0)
				bar_width = (bar_width - 1) | 0x0001;

			break;
	
		case 'd':
			dump_settings = 1;
			break;

		case 'h':
			usage(argv);
			exit(EXIT_SUCCESS);

		default:
			usage(argv);
			exit(EXIT_FAILURE);
		}
	}


	fd = open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (hflip >= 0)
		set_control(fd, ctrls[HFLIP].id, !!hflip, ctrls[HFLIP].idstr);

	if (vflip >= 0)
		set_control(fd, ctrls[VFLIP].id, !!vflip, ctrls[VFLIP].idstr);

	if (green >= 0)
		set_control(fd, ctrls[GREEN].id, green, ctrls[GREEN].idstr);

	if (red >= 0)
		set_control(fd, ctrls[RED].id, red, ctrls[RED].idstr);

	if (blue >= 0)
		set_control(fd, ctrls[BLUE].id, blue, ctrls[BLUE].idstr);

	if (bar_width >= 0)
		set_control(fd, ctrls[BAR_WIDTH].id, bar_width, ctrls[BAR_WIDTH].idstr);

	if (test_pattern >= 0)
		set_control(fd, ctrls[TEST_PATTERN].id, test_pattern, ctrls[TEST_PATTERN].idstr);

	if (dump_settings)
		dump_current_settings(fd);


	close(fd);

	return 0;
}

