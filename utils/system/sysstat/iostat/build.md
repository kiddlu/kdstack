change Mkafile CC to arm-linux-gnueabi-gcc

arm-linux-gnueabi-gcc -o iostat -g -O2 -Wall -Wstrict-prototypes -pipe -O2 iostat.o librdstats_light.a libsyscom.a -s -static
