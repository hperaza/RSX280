# Target platform
#
# Possible values are cpu280 and z280rc (note lowercase!)
# Make sure to select the correct device driver set in the inc/sysconf.inc
# file before building the system image.
#
#platform = cpu280
platform = z280rc

# Disk image characteristics (used by 'make disk-image')
#
# 'image' is the image file name
# 'size' is image size in blocks
# 'files' is max number of files (index file entries)
# 'offset' is the offset to the start of the partition, in blocks
# 'bootloader' is the file name of the appropriate binary boot loader
#              for the image
#
# 3.5" 1.44M floppy image (e.g. for CPU280):
#
#image = floppy.img
#size = 2880
#files = 512
#offset = 0
#bootloader = fdboot.bin
#
# 4M CF partition (e.g. for Z280RC):
#
#image = cf-partition.img
#size = 8192
#files = 1024
#offset = 65536
#bootloader = cfboot.bin
#
# 32M CF or GIDE partition (for Z280RC or CPU280):
#
image = cf-partition.img
size = 65536
files = 8192
offset = 65536
bootloader = cfboot.bin

# Physical disk device to update with the 'make dev-copy' command.
# !!!Always make sure you're writing to the right one!!! (check with e.g.
# dmesg) As a safety measure, previous device contents will be saved into
# a file named 'device.backup' unless you explicitly set the 'backup' option
# to 'no'.
#
# Set 'conv = swab' to swap word bytes during copy (Z280RC).
#
#outdev = /dev/fd0
outdev = /dev/sdc
#backup = no
conv = swab
