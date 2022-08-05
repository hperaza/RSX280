The "cf128m-full-z280rc.img.gz" file is a full CF image intended to be used
in a real Z280RC machine.  In addition to RSX280, the image contains CP/M
2.2 and CP/M 3 from [Tony Nicholson's distribution](https://github.com/agn453/Z280RC),
and UZI280 partitions.

The "cf32m-partition-z280rc.img.gz" file contains only the RSX280 partition.

Note that the above Z280RC images do not have the bytes swapped so they can
be accessed with the vol180 utility.  Thus, when copying the image to a physical
CF card you'll need to use a disk copy program that swaps bytes of words
(the Linux dd command has a 'conv=swab' option that does the job.)

The "cf128m-full-emulator.img.gz" image is intended to be used with [Michal
Tomek's Z280 emulator](https://github.com/mtdev79/z280emu). Just gunzip it,
rename to "cf00.dsk" and copy it the the emulator's directory.

