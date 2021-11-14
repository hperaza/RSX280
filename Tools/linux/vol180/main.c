/***********************************************************************

   This file is part of vol180, an utility to handle RSX180 volumes.
   Copyright (C) 2008-2020, Hector Peraza.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#ifdef __MINGW32__
#include <sys/utime.h>
#endif
#include <sys/stat.h>
#include <unistd.h>

#include "fileio.h"
#include "dirio.h"
#include "indexf.h"
#include "mkdisk.h"
#include "mount.h"
#include "vmr.h"
#include "misc.h"
#include "bitmap.h"

extern struct FCB *mdfcb, *cdfcb;
extern FILE *imgf;
extern unsigned long img_offset;

extern unsigned long nblocks;
extern unsigned char clfactor;

/*-----------------------------------------------------------------------*/

void show_help(char *topic) {
  if (!*topic) {
    printf("Available commands:\n\n");
    printf("mount <imgname> [<offset>]        - mount disk image\n");
    printf("new <imgname> <nblocks> <nfiles>  - create empty disk image\n");
    printf("dir [<dirname>] [/f]              - display directory\n");
    printf("cd <dirname>                      - change directory\n");
    printf("mkdir <dirname> <gid>,<uid>       - create directory\n");
    printf("type <filename>                   - type file contents\n");
    printf("dump <filename> [/h]              - dump file contents\n");
    printf("copy <srcfile> <dstfile> [/[-]c]  - copy a file\n");
    printf("delete <filename>                 - delete file\n");
    printf("import <unixfile> <file> [/c]     - import file\n");
    printf("export <file> <unixfile>          - export file\n");
    printf("updboot [<bootloader>]            - update boot record\n");
    printf("vmr <arguments...>                - enter a VMR command\n");
    printf("quit                              - exit program\n\n");
    printf("Type \"help <command>\" for detailed information about a command.\n\n");
  } else {
    if (strcasecmp(topic, "mount") == 0) {
      printf("Mounts a disk image so files can be accessed. Any previously mounted image is\n");
      printf("dismounted.\n\n");
      printf("Syntax:\n\n");
      printf("  mount <imgname> [<offset>]\n\n");
      printf("where <imgname> is the name of a device or file containing the disk image, and\n");
      printf("<offset> is an optional offset to the start of the partition if e.g. the image\n");
      printf("is a hard disk image or if it has a header like the one used by emulators. The\n");
      printf("<offset> argument is specified as a decimal value representing the number of\n");
      printf("blocks to skip (or number of bytes if preceded by a '+') and defaults to zero.\n\n");
      printf("Examples:\n\n");
      printf("  mount /dev/fd0\n");
      printf("  mount floppy.img\n");
      printf("  mount hd.img 16000\n\n");
    } else if (strcasecmp(topic, "dir") == 0) {
      printf("Lists the contents of a directory.\n\n");
      printf("Syntax:\n\n");
      printf("  dir [<dirname>] [/f]\n\n");
      printf("where the optional <dirname> is the name of the directory to list.\n");
      printf("If the /f (full) option is specified, additional information is\n");
      printf("displayed such as file ownership and protection bits.\n\n");
      printf("Examples:\n\n");
      printf("  dir\n");
      printf("  dir [master] /f\n\n");
    } else if (strcasecmp(topic, "cd") == 0) {
      printf("Displays or sets the current directory for file operations.\n\n");
      printf("Syntax:\n\n");
      printf("  cd [<dirname>]\n\n");
      printf("where the optional <dirname> is the name of the directory to switch to.\n");
      printf("If not specified, the current directory is displayed.\n\n");
      printf("Examples:\n\n");
      printf("  cd\n");
      printf("  cd system\n\n");
    } else if (strcasecmp(topic, "mkdir") == 0) {
      printf("Creates a directory.\n\n");
      printf("Syntax:\n\n");
      printf("  mkdir <dirname> <gid>,<uid>\n\n");
      printf("where <dirname> is the name of the directory to create, <gid> and <uid>\n");
      printf("set the directory ownership: <gid> is the numeric group ID and <uid> the\n");
      printf("numeric user ID.\n\n");
      printf("Example:\n\n");
      printf("  mkdir user 20,2\n\n");
    } else if (strcasecmp(topic, "type") == 0) {
      printf("Displays the contents of a text file.\n\n");
      printf("Syntax:\n\n");
      printf("  type <filename>\n\n");
      printf("where <filename> is the name of the file to display.\n\n");
      printf("Example:\n\n");
      printf("  type startup.cmd;1\n\n");
    } else if (strcasecmp(topic, "dump") == 0) {
      printf("Displays information about the file such as index file entry number, location\n");
      printf("on disk, user ownership, access permissions, etc. optionally followed by the\n");
      printf("file contents in hexadecimal format.\n\n");
      printf("Syntax:\n\n");
      printf("  dump <filename> [/h]\n\n");
      printf("where <filename> is the name of the file to dump. If the /h option is specified,\n");
      printf("then only the information from the file header is displayed.\n\n");
      printf("Example:\n\n");
      printf("  dump system.sys\n\n");
    } else if (strcasecmp(topic, "copy") == 0) {
      printf("Copies a file on the local volume.\n\n");
      printf("Syntax:\n\n");
      printf("  copy <srcfile> <dstfile> [/[-]c]\n\n");
      printf("where <srcfile> is the name of the source file and <dstfile> the name\n");
      printf("of the destination file or directory. If the /c option is specified, the\n");
      printf("output file will be forced contiguous. Similarly, the /-c option forces\n");
      printf("the output file non-contiguous.\n\n");
      printf("Examples:\n\n");
      printf("  copy system.sys system.old /c\n");
      printf("  copy [user]example.bas;1 [basic]\n\n");
    } else if (strcasecmp(topic, "delete") == 0) {
      printf("Deletes a file, no questions asked.\n\n");
      printf("Syntax:\n\n");
      printf("  delete <filename>\n\n");
      printf("where <filename> is the name of the file to delete. Wildcards\n");
      printf("are not allowed.\n\n");
      printf("Example:\n\n");
      printf("  delete icp.tsk;2\n\n");
    } else if (strcasecmp(topic, "import") == 0) {
      printf("Copies a file from the host machine to the current directory of the mounted\n");
      printf("disk image.\n\n");
      printf("Syntax:\n\n");
      printf("  import <unixfile> <file> [/c]\n\n");
      printf("where <unixfile> is the path name of the file to import, and <file> the name\n");
      printf("of the destination file on the mounted volume. If the /c option is specified\n");
      printf("the destination file will be contiguous.\n\n");
      printf("Examples:\n\n");
      printf("  import ../startup.cmd startup.cmd\n");
      printf("  import ~/rsx180/mcr/sys/sys.tsk sys.tsk /c\n\n");
    } else if (strcasecmp(topic, "export") == 0) {
      printf("Copies a file from the current directory of the mounted disk image to the\n");
      printf("host machine.\n\n");
      printf("Syntax:\n\n");
      printf("  export <file> <unixfile>\n\n");
      printf("where <file> is the name of the file to export, and <unixfile> the path name\n");
      printf("of the file destination file on the host machine.\n\n");
      printf("Example:\n\n");
      printf("  export startup.cmd ./backup/startup.cmd.old\n\n");
    } else if (strcasecmp(topic, "updboot") == 0) {
      printf("Makes the volume bootable by writing the appropriate information to the\n");
      printf("BOOT.SYS file. A valid SYSTEM.SYS system image file must be present on the\n");
      printf("MASTER directory.\n\n");
      printf("Syntax:\n\n");
      printf("  updboot [<bootloader>]\n\n");
      printf("where <bootloader> is the name of a file on the host machine containing\n");
      printf("a bootstrap loader binary image appropriate for the mounted volume type.\n");
      printf("If not specified, the existing loader is kept and only the boot information\n");
      printf("is updated. Please note that the \"new\" command creates a volume without\n");
      printf("a bootstrap loader image, so one must be specified at least once in the\n");
      printf("\"updboot\" command for a volume to be bootable.\n\n");
      printf("Examples:\n\n");
      printf("  updboot\n");
      printf("  updboot ../boot/fdboot.bin\n\n");
    } else if (strcasecmp(topic, "new") == 0) {
      printf("Creates a new disk image.\n\n");
      printf("Syntax:\n\n");
      printf("  new <imgname> <nblocks> <nfiles>\n\n");
      printf("where <imgname> is the name of the file or device to write the new image to,\n");
      printf("<nblocks> is the image size in 512-byte blocks, and <nfiles> sets the maximum\n");
      printf("number of files that the image can contain.\n\n");
      printf("Examples:\n\n");
      printf("  new /dev/fd0 2880 512\n");
      printf("  new hd.img 16000 4096\n\n");
    } else if (strcasecmp(topic, "vmr") == 0) {
      printf("Allows entering a VMR command in order to configure a system image.\n\n");
      printf("Syntax:\n\n");
      printf("  vmr <sysimgfile> <command> [<arguments...>]\n");
      printf("  vmr @<commandfile>\n\n");
      printf("The first form opens a system image file and executes the specified command.\n");
      printf("The second form executes commands from a local command file.\n\n");
      printf("Currently supported VMR commands are: ASN SET INS REM FIX UNF\n\n");
      printf("Examples:\n\n");
      printf("  vmr system ins ted/inc=5000\n");
      printf("  vmr @sysvmr\n\n");
    } else if ((strcasecmp(topic, "quit") == 0) ||
               (strcasecmp(topic, "exit") == 0) ||
               (strcasecmp(topic, "bye") == 0)) {
      printf("Saves changes made to the disk image and exits the program.\n\n");
      printf("Syntax:\n\n");
      printf("  quit\n\n");
      printf("or\n\n");
      printf("  exit\n\n");
      printf("or\n\n");
      printf("  bye\n\n");
    } else {
      printf("Unknown command: %s\n", topic);
    }
  }
}

/* Display user directory */
int cmd_dir(char *dirname, int full) {
  char *dname;
  unsigned char dirent[16], inode[64];
  struct FCB *fcb, *bmfcb;
  int i, nfiles, freefcb;
  unsigned short ino, vers, lbcount;
  unsigned long nused;
#if 0
  unsigned long nalloc;
#endif
  unsigned long fsize, nfree;
  
  if (!imgf) return 1;

  dname = dirname;
  if (!dname || !*dname) {
    fcb = cdfcb ? cdfcb : mdfcb;
    freefcb = 0;
  } else {
    fcb = open_md_file(dname);
    freefcb = 1;
  }
  
  if (!fcb) {
    fprintf(stderr, "Directory not found\n");
    return 1;
  }
  
  printf("\nDirectory SY0:[%s]\n\n", get_dir_name(fcb));

  file_seek(fcb, 0L);
  for (nfiles = 0; ;) {
    if (file_read(fcb, dirent, 16) != 16) break;
    ino = GET_INT16(dirent, 0);
    if (ino != 0) {
#if 1
      /* SRD-like */
      for (i = 0; i < 9; ++i) fputc(dirent[i+2], stdout);
      fputc('.', stdout);
      for (i = 0; i < 3; ++i) fputc(dirent[i+11], stdout);
      vers = GET_INT16(dirent, 14);
      printf(";%-5d", vers);
#else
      /* PIP-like */
      vers = 2;
      for (i = 0; i < 9; ++i) if (dirent[i+2] != ' ') fputc(dirent[i+2], stdout), ++vers;
      fputc('.', stdout);
      for (i = 0; i < 3; ++i) if (dirent[i+11] != ' ') fputc(dirent[i+11], stdout), ++vers;
      i = vers;
      vers = GET_INT16(dirent, 14);
      printf(";%-5d", vers);
      for (; i < 12; ++i) fputc(' ', stdout);
#endif
      if (read_inode(ino, inode) == 0) {
        printf(" - Failed to read attributes, index file error\n");
        continue;
      }
      if (GET_INT16(inode, 0) == 0) {
        printf(" - Failed to read attributes, index file error\n");
        continue;
      }
#if 0
      nalloc = GET_INT24(inode, 8);
#endif
      nused = GET_INT24(inode, 11);
      lbcount = GET_INT16(inode, 14);
      if (nused == 0)
        fsize = 0;
      else
        fsize = (nused - 1) * 512L + lbcount;
      printf("%9lu ", fsize);  /* 65,536 * 512 = 33,554,432 (8 digits max) */
      printf("%s  ", (inode[2] & _FA_CTG) ? "C" : " ");
      /*printf("%s", timestamp_str(&inode[23]));*/ /* modified timestamp */
      printf("%s", timestamp_str(&inode[16])); /* created timestamp */
      if (full) {
        char tmp[32];
        unsigned short perm;
        sprintf(tmp, "[%d,%d]", inode[7], inode[6]);
        printf("  %-9s", tmp);
        perm = GET_INT16(inode, 30);
        printf(" [%s]", perm_str(perm));
      }
      printf("\n");
      ++nfiles;
    }
  }
  
  bmfcb = open_md_file("BITMAP.SYS");
  nfree = 0;
  if (!bmfcb) {
    printf("Error opening BITMAP.SYS\n");
  } else {
    /* nblocks was obtained from volume id (second block of BOOT.SYS) */
    unsigned char bmp, mask;
    unsigned long nclusters;
    mask = 0;
    file_seek(bmfcb, (long) BMHDRSZ); /* skip header */
    nclusters = nblocks >> clfactor;
    for (i = 0; i < nclusters; ++i) {
      if (mask == 0) {
        if (file_read(bmfcb, &bmp, 1) != 1) {
          printf("Error reading BITMAP.SYS\n");
          break;
        }
        mask = 0x80;
      }
      if ((bmp & mask) == 0) ++nfree;
      mask >>= 1;
    }
    nfree *= 512L;
    nfree <<= clfactor;
    close_file(bmfcb);
    free_fcb(bmfcb);
  }
  
  printf("\n%d file(s), %lu byte(s) free.\n\n", nfiles, nfree);
  
  if (freefcb) free_fcb(fcb);
  
  return 0;
}

/* Display file contents (type) on console */
int cmd_type(char *filename) {
  struct FCB *fcb;
  
  fcb = open_file(filename);
  if (fcb) {
    int i, len;
    unsigned char buf[256];
          
    while ((len = file_read(fcb, buf, 256)) > 0) {
      for (i = 0; i < len; ++i) fputc(buf[i], stdout);
    }
    close_file(fcb);
    free_fcb(fcb);
    return 1;
  } else {
    printf("File not found\n");
    return 0;
  }
}

/* Dump file contents in hexadecimal */
int cmd_dump(char *filename, int option) {
  struct FCB *fcb;

  fcb = open_file(filename);
  if (fcb) {
    int i, len;
    unsigned char buf[16];
    unsigned addr;
    
    dump_inode(fcb->header->inode);
    
    if (option == 2) {
      dump_alloc_map(fcb);
    } else {
      if (!option) {
        addr = 0;
        while ((len = file_read(fcb, buf, 16)) > 0) {
          printf("%08X: ", addr);
          for (i = 0; i < len; ++i)
            printf("%02X ", buf[i]);
          for ( ; i < 16; ++i)
            printf("   ");
          for (i = 0; i < len; ++i)
            fputc(((buf[i] >= 32) && (buf[i] < 127)) ? buf[i] : '.', stdout);
          printf("\n");
          addr += 16;
        }
      }
    }
    close_file(fcb);
    free_fcb(fcb);
    return 1;
  } else {
    printf("File not found\n");
    return 0;
  }
}

/* Copy a file */
int cmd_copy(char *srcfile, char *dstfile, int mode, int alloc) {
  struct FCB *srcfcb, *dstfcb;
  int len, retc;
  unsigned char buf[512];
  char dname[10], fname[10], ext[4], dstnam[256];
  short vers;

  if (!parse_name(srcfile, dname, fname, ext, &vers)) {
    printf("Invalid source file name\n");
    return 0;
  }
  if (!*fname && !*ext && !vers) {
    printf("Invalid source file name\n");
    return 0;
  }

  if (!parse_name(dstfile, dname, fname, ext, &vers)) {
    printf("Invalid source file name\n");
    return 0;
  }
  if (!*dname && !*fname && !*ext && !vers) {
    printf("Invalid destination file name\n");
    return 0;
  }
  if (!*fname && !*ext && !vers) {
    /* if the dest name contains only a directory specification,
       use the same name as the source file */
    snprintf(dstnam, 20, "[%s]", dname);
    parse_name(srcfile, dname, fname, ext, &vers);
    snprintf(dstnam + strlen(dstnam), 200, "%s.%s;%d", fname, ext, vers);
  } else {
    strcpy(dstnam, dstfile);
  }
  
  srcfcb = open_file(srcfile);
  if (!srcfcb) {
    printf("Source file not found\n");
    return 0;
  }
  
  if (mode == 0) {
    /* output of the same type as source file */
    mode = srcfcb->header->attrib & _FA_CTG;
  } else if (mode > 0) {
    /* force output contiguous */
    mode = 1;
  } else {
    /* force output non-contiguous */
    mode = 0;
  }
  
  if (alloc < srcfcb->header->nused) alloc = srcfcb->header->nused;

  dstfcb = create_file(dstnam, srcfcb->header->group, srcfcb->header->user,
                       mode, alloc);
  if (!dstfcb) {
    printf("Could not create file\n");
    close_file(srcfcb);
    free_fcb(srcfcb);
    return 0;
  }

  retc = 1;
  while ((len = file_read(srcfcb, buf, 512)) > 0) {
    if (file_write(dstfcb, buf, len) != len) {
      retc = 0;
      printf("File write error\n");
      break;
    }
  }

  close_file(srcfcb);
  free_fcb(srcfcb);
  close_file(dstfcb);
  free_fcb(dstfcb);

  return retc;
}

/* Import file */
int cmd_import(char *srcfile, char *dstfile, int contiguous, int alloc) {
  struct FCB *fcb;
  FILE *f;
  long pos, size, blks;
  char user, group;
  int len, wlen;
  unsigned char buf[256];
  struct stat sbuf;
        
  f = fopen(srcfile, "rb");
  if (!f) {
    printf("File not found\n");
    return 0;
  }
  
  user = 1;
  group = 1;
  fseek(f, 0L, SEEK_END);
  size = ftell(f);
  fseek(f, 0L, SEEK_SET);
  blks = (size + 511) / 512;
  
  if (alloc < blks) alloc = blks;

  strupr(dstfile);
  if (cdfcb) {
    user = cdfcb->header->user;
    group = cdfcb->header->group;
  }
  fcb = create_file(dstfile, group, user, contiguous, alloc);
  if (!fcb) {
    printf("Could not create file\n");
    fclose(f);
    return 0;
  }
  
  pos = 0;
  while ((len = fread(buf, 1, 256, f)) > 0) {
    wlen = file_write(fcb, buf, len);
    pos += wlen;
    if (wlen != len) {
      printf("Error writing file: offset %ld, %d of %d bytes written\n",
             pos, wlen, len);
    }
  }
  close_file(fcb);
  if (fstat(fileno(f), &sbuf) == 0) {
    /* done here, since close_file() sets mtime,
       OK since the FCB has not been free'd yet */
    set_file_dates(fcb, sbuf.st_mtime, sbuf.st_mtime);
  }
  free_fcb(fcb);
  fclose(f);
  return 1;
}

/* Export file */
int cmd_export(char *srcfile, char *dstfile) {
  struct FCB *fcb;
  FILE *f;
  int len;
  unsigned char buf[256], inode[32];

  f = fopen(dstfile, "wb");
  if (!f) {
    printf("Could not create file\n");
    return 0;
  }

  strupr(srcfile);
  fcb = open_file(srcfile);
  if (!fcb) {
    printf("File not found\n");
    fclose(f);
    return 0;
  }

  while ((len = file_read(fcb, buf, 256)) > 0) {
    fwrite(buf, 1, len, f);
  }
  close_file(fcb);
  fclose(f);

  if (read_inode(fcb->header->inode, inode)) {
    if (GET_INT16(inode, 0) != 0) {
#ifdef __MINGW32_
      struct utimbuf utim;
      utim.actime  = timestamp_to_secs(&inode[16]);
      utim.modtime = timestamp_to_secs(&inode[16]);
      utime(dstfile, &utim);
#else
      struct timeval times[2];
      times[0].tv_sec = timestamp_to_secs(&inode[16]);
      times[0].tv_usec = 0;
      times[1].tv_sec = timestamp_to_secs(&inode[16]);
      times[1].tv_usec = 0;
      utimes(dstfile, times);
#endif
    }
  }
  free_fcb(fcb);
  
  return 0;
}

/* Update boot record with the specified boot loader */
int update_boot(char *filename) {
  struct FCB *fcb;
  unsigned long stablk, size;
  
  fcb = open_md_file("SYSTEM.SYS");
  if (fcb) {
    stablk = fcb->header->bmap[0];  /* assumes contiguous file! */
    size = fcb->header->nused;
    close_file(fcb);
    free_fcb(fcb);
  } else {
    printf("Could not open SYSTEM.SYS\n");
    return 0;
  }

  fcb = open_md_file("BOOT.SYS");
  if (fcb) {
    if (filename && *filename) {
      FILE *f;
      int  n;
      char buf[512];
      
      f = fopen(filename, "rb");
      if (!f) {
        printf("Could not open %s\n", filename);
        return 0;
      }
      n = fread(buf, 1, 512, f);
      if (n == 0) {
        printf("Could not read %s\n", filename);
        fclose(f);
        return 0;
      }
      fclose(f);
      file_seek(fcb, 0L);
      file_write(fcb, (unsigned char *) buf, n);
    }
    file_seek(fcb, 512L + 72L);
    file_write(fcb, (unsigned char *) &stablk, 3);
    file_seek(fcb, 512L + 76L);
    file_write(fcb, (unsigned char *) &size, 2);
    close_file(fcb);
    free_fcb(fcb);
  } else {
    printf("Could not open BOOT.SYS\n");
    return 0;
  }
  
  return 1;
}

/*-----------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
  char filename[256], str[256], cmd[256], arg1[256], arg2[256], arg3[256];

  filename[0] = '\0';

  if (argc > 1) {
    img_offset = 0;
    strncpy(filename, argv[1], 255);
    filename[255] = '\0';
    if (argc > 2) {
      char *p = argv[2];
      if (*p == '+') ++p;
      img_offset = atol(p);
      if (*argv[2] != '+') img_offset *= 512L;
    }
    if (mount_disk(filename) != 0) filename[0] = '\0';
  }
  
  for (;;) {
    printf(">");
    fflush(stdout);
    fgets(str, 255, stdin);
    str[strlen(str)-1] = '\0';  // strip newline
    if (feof(stdin)) break;

    cmd[0] = arg1[0] = arg2[0] = arg3[0] = '\0';
    sscanf(str, "%s %s %s %s", cmd, arg1, arg2, arg3);
        
    if (!*cmd) {
      continue;
    } else if (*cmd == ';') {
#if 0
      char dname[10], fname[10], ext[4];
      short vers;
      parse_name(arg1, dname, fname, ext, &vers);
      printf("\"%s\" -> \"%s\", \"%s\", \"%s\", %d\n",
             arg1, dname, fname, ext, vers);
#endif
      continue;
    } else if (strcmp(cmd, "echo") == 0) {
      int i;
      sscanf(str, "%s %n", cmd, &i);
      printf("%s\n", str+i);
    } else if (strcmp(cmd, "help") == 0) {
      show_help(arg1);
    } else if (strcmp(cmd, "mount") == 0) {
      if (*arg1) {
        strcpy(filename, arg1);
        img_offset = 0;
        if (*arg2) {
          char *p = arg2;
          if (*p == '+') ++p;
          img_offset = atol(p);
          if (*arg2 != '+') img_offset *= 512L;
        }
        if (mount_disk(filename) != 0) filename[0] = '\0';
      } else {
        if (*filename) printf("Mounted: %s\n", filename);
      }
    } else if (strcmp(cmd, "new") == 0) {
      if (*arg1 && *arg2 && *arg3) {
        unsigned long nblocks;
        unsigned nfiles;
        nblocks = atol(arg2);
        nfiles  = atoi(arg3);
        create_disk(arg1, nblocks, nfiles);
      } else {
        printf("Missing filename, size and/or number of files\n");
      }
    } else if ((strcmp(cmd, "quit") == 0) ||
               (strcmp(cmd, "exit") == 0) ||
               (strcmp(cmd, "bye")  == 0)) {
      dismount_disk();
      printf("\n");
      break;
    } else {
      if (!imgf) {
        printf("Volume not mounted\n");
      } else {
        if (strcmp(cmd, "dir") == 0) {
          char *p, *dirname;
          int full = 0;
          dirname = arg1;
          if (strcmp(dirname, "/f") == 0) {
            full = 1;
            *dirname = '\0';
          } else {
            if (*dirname == '[') {
              ++dirname;
              p = strchr(dirname, ']');
              if (p) *p = '\0';
            }        
            if (*dirname && !strchr(dirname, '.')) {
              strupr(dirname);      
              strcat(dirname, ".DIR");
              if (strcmp(arg2, "/f") == 0) full = 1;
            }
          }
          cmd_dir(dirname, full);
        } else if (strcmp(cmd, "cd") == 0) {
          if (*arg1) {
            strupr(arg1);
            if (!strchr(arg1, '.')) strcat(arg1, ".DIR");
            if (change_dir(arg1) == 0) printf("Failed to change directory\n");
          } else {
            printf("Current directory is %s\n", get_dir_name(cdfcb));
          }
        } else if ((strcmp(cmd, "mkdir") == 0) ||
                   (strcmp(cmd, "ufd") == 0)) {
          if (*arg1) {
            strupr(arg1);
            if (!strchr(arg1, '.')) strcat(arg1, ".DIR");
            if (*arg2) {
              int user, group;
              sscanf(arg2, "%d,%d", &group, &user);
              if (create_dir(arg1, group & 0xff, user & 0xff) == 0)
                printf("Failed to create directory\n");
            } else {
              printf("Missing user,group\n");
            }
          } else {
            printf("Missing directory name\n");
          }
        } else if (strcmp(cmd, "type") == 0) {
          if (*arg1) {
            strupr(arg1);
            cmd_type(arg1);
          } else {
            printf("Missing filename\n");
          }
        } else if (strcmp(cmd, "dump") == 0) {
          int opt = 0;
          
          if (*arg1) {
            strupr(arg1);
            if (strcmp(arg2, "/h") == 0) opt = 1;
            if (strcmp(arg2, "/m") == 0) opt = 2;
            cmd_dump(arg1, opt);
          } else {
            printf("Missing filename\n");
          }
        } else if (strcmp(cmd, "copy") == 0) {
          if (*arg1 && *arg2) {
            int mode = 0, alloc = 0;
            char *p;
            
            strupr(arg1);
            strupr(arg2);
            p = strchr(arg3, ':');
            if (p) *p++ = '\0';
            if (strcmp(arg3, "/c") == 0) mode = 1;
            else if (strcmp(arg3, "/-c") == 0) mode = -1;
            if (mode && p) alloc = atoi(p);
            cmd_copy(arg1, arg2, mode, alloc);
          } else {
            printf("Missing argument\n");
          }
        } else if (strcmp(cmd, "delete") == 0) {
          if (*arg1) {
            strupr(arg1);
            if (delete_file(arg1) == 0) printf("File not found\n");
          } else {
            printf("Missing filename\n");
          }
        } else if (strcmp(cmd, "import") == 0) {
          if (*arg1 && *arg2) {
            int contiguous = 0, alloc = 0;
            char *p;
            
            p = strchr(arg3, ':');
            if (p) *p++ = '\0';
            if (strcmp(arg3, "/c") == 0) contiguous = 1;
            if (contiguous && p) alloc = atoi(p);
            cmd_import(arg1, arg2, contiguous, alloc);
          } else {
            printf("Missing argument\n");
          }
        } else if (strcmp(cmd, "export") == 0) {
          if (*arg1 && *arg2) {
            cmd_export(arg1, arg2);
          } else {
            printf("Missing argument\n");
          }
        } else if (strcmp(cmd, "updboot") == 0) {
          update_boot(arg1);
        } else if (strcmp(cmd, "vmr") == 0) {
          int n;
          sscanf(str, "%s %n", cmd, &n);
          strcpy(cmd, str + n);
          vmr(cmd);
        } else {
          printf("Unknown command: %s\n", cmd);
        }
      }
    }
  }

  return 0;
}
