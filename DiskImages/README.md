The Z280RC CF partition images in this directory do not have the bytes swapped
so they can be mounted with the vol180 utility. Therefore you'll need to use
a disk copy program that swaps bytes of words when copying the partition to
the physical disk (the Linux dd command has a 'conv=swab' option that does the
job.)

