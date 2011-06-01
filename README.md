  camctl
=======

Driver testing prog for excersing the VFLIP/HFLIP and TEST PATTERN options
for an MT9P031 sensor Linux driver.

The Makefile assumes an OE cross-build environment.

Run with -h to get usage help.

	root@overo# ./camctl -h
	Usage: camctl [options]

	Options:
	-m<n>	Mirror (horizontal flip) (0 or 1)
	-f<n>	Flip (vertical flip) (0 or 1)
	-t<n>	Test pattern (0 - 8)
	-g<n>	Green test pattern (0 - 4095)
	-r<n>	Red test pattern (0 - 4095)
	-b<n>	Blue test pattern (0 - 4095)
	-w<n>	Bar width for test pattern (odd or zero, 0 - 4095)
	-d	Dump current settings
	-h	Print this message


Refer to the mt9p031 datasheet for what the test pattern controls do.

