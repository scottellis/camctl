  camctl
=======

Driver testing prog for excersing the VFLIP/HFLIP and TEST PATTERN options
for an MT9P031 sensor Linux driver.

The Makefile assumes an OE cross-build environment.

Run with -h to get usage help.

	root@overo# ./camctl -h
	Usage: camctl [options]

	Options:
	-m<n>	Mirror (horizontal flip) n = 0 or 1
	-f<n>	Flip (vertical flip) n = 0 or 1
	-t	Test pattern (0 - 8)
	-g	Green test pattern
	-r	Red test pattern
	-b	Blue test pattern
	-w	Bar width for test pattern (odd)
	-d	Dump current settings
	-h	Print this message


Refer to the mt9p031 datasheet for what the test pattern controls do.

