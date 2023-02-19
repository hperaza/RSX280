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

#ifndef __RSX180_H
#define __RSX180_H

/* TCB offsets */

#define T_LNK	0		// utility link pointer (2 bytes)
#define T_TCBL	(T_LNK + 2)	// link to next TCB in directory list (2 bytes)
#define T_ACTL	(T_TCBL + 2)	// link to next TCB in active list (2 bytes)
#define T_ATTR	(T_ACTL + 2)	// attributes (1 byte)
#define T_ST	(T_ATTR + 1)	// status (2 bytes)
#define T_DPRI	(T_ST + 2)	// default priority (1 byte)
#define T_PRI	(T_DPRI + 1)	// current priority (1 byte)
#define T_SPRI	(T_PRI + 1)	// current swap priority (1 byte)
#define T_NAME	(T_SPRI + 1)	// task name (6 characters)
#define T_VID	(T_NAME + 6)	// task version identification (6 characters)
#define T_CMD	(T_VID + 6)	// pointer to command line block (2 bytes)
#define T_IOC	(T_CMD + 2)	// outstanding I/O count (1 byte)
#define T_RCVL	(T_IOC + 1)	// pointer to receive queue (2 bytes)
#define T_OCBL	(T_RCVL + 2)	// pointer to list of OCBs (2 bytes)
#define T_SAST	(T_OCBL + 2)	// pointer to list of specified ASTs (2 bytes)
#define T_ASTL	(T_SAST + 2)	// pointer to list of AST events (2 bytes)
#define T_SVST  (T_ASTL + 2)	// saved task status during AST (2 bytes)
#define T_FLGS	(T_SVST + 2)	// task event flags (4 bytes = 32 flags)
#define T_WAIT	(T_FLGS + 4)	// flag wait mask (4 bytes)
#define T_CTX	(T_WAIT + 4)	// pointer to context block (2 bytes)
#define T_TI	(T_CTX + 2)	// UCB of terminal device (2 bytes) 
#define T_LDEV	(T_TI + 2)	// UCB of load device (2 bytes) 
#define T_SBLK	(T_LDEV + 2)	// task starting disk block number (4 bytes)
#define T_NBLK	(T_SBLK + 4)	// task size in disk blocks (2 bytes)
#define T_PCB	(T_NBLK + 2)	// pointer to PCB (2 bytes)
#define T_CPCB	(T_PCB + 2)	// pointer to checkpoint PCB (2 bytes)
#define T_STRT	(T_CPCB + 2)	// start address (2 bytes)
#define T_DEND	(T_STRT + 2)	// default end address (2 bytes)
#define T_END	(T_DEND + 2)	// current end address (2 bytes)
#define T_EPT	(T_END + 2)	// entry point (2 bytes)
#define T_SP	(T_EPT + 2)	// task SP (2 bytes)

/* TCB size */

#define TCBSZ	(T_SP + 2)

/* Attribute bit numbers */

#define TA_PRV	0		// task is privileged
#define TA_REM	1		// remove on exit
#define TA_AST	2		// AST recognition enabled
#define TA_FIX	3		// task fixed in memory
#define TA_MCR	4		// task is external MCR function
#define TA_CLI	5		// task is a CLI
#define TA_ACP  6		// task is an ACP
#define TA_CKD  7		// checkpointing disabled

/* Status bit numbers */

#define TS_ACT	0		// task active
#define TS_AST	1		// task is executing an AST
#define TS_SUP	2		// task is in supervisor mode
#define TS_CKR	3		// checkpoint requested

#define T2_STP	0		// stopped
#define T2_EFW	1		// event flag waiting
#define T2_WTD	2		// waiting for data
#define T2_OUT	3		// task is out of memory
#define T2_CKP	4		// task checkpointed
#define T2_ABO	5		// task being aborted

/* Task Context block offsets */

#define	TX_UID	0		// protection user ID (1 byte)
#define	TX_GID	(TX_UID + 1)	// protection group ID (1 byte)
#define	TX_DIR	(TX_GID + 1)	// task's current directory (9 bytes)
#define	TX_SWM	(TX_DIR + 9)	// saved flag wait mask during AST (4 bytes)
#define	TX_LUT	(TX_SWM + 4)	// LUN translation table (64 bytes)
#define	TX_SST	(TX_LUT + 64)	// user SST vector table (2 bytes)
#define	TX_XDT	(TX_SST + 2)	// external debugger context data (2 bytes)
#define TX_REGS (TX_XDT + 2)	// saved register bank (RSX280) (28 bytes)

/* Context Block size */

#define	CTX180SZ	(TX_XDT + 2)
#define	CTX280SZ	(TX_REGS + 28)

/* Task File Header offsets */

#define TH_HDR	0x00		// task header magic string (6 bytes)
#define TH_VER	0x08		// task image file version (2 bytes)
#define TH_CPU	0x0A		// CPU type (1 byte)
#define TH_NAME	0x10		// default task name (6 bytes)
#define TH_VID	0x18		// task version identification (6 bytes)
#define TH_PAR	0x20		// partition name (6 bytes)
#define TH_STRT	0x28		// start address (2 bytes)
#define TH_END	0x2A		// end address (2 bytes)
#define TH_EPT	0x2C		// entry point (2 bytes)
#define TH_INC	0x2E		// extension/increment size (2 bytes)
#define TH_PRI	0x30		// task priority (1 byte)
#define TH_ATTR	0x31		// task attributes (1 byte)
#define TH_LUNT	0x40		// LUN table (16*4 = 64 bytes)

/* Task File Header size */

#define THSZ	256

/* Partition Control Block structure */

#define P_LNK	0		// link to next PCB in list (2 bytes)
#define P_MAIN	(P_LNK + 2)	// pointer to main partition (2 bytes)
#define P_SUB	(P_MAIN + 2)	// pointer to subpartition (2 bytes)
#define P_SIZE	(P_SUB + 2)	// size in pages (1=Z180 or 2=Z280 bytes)
#define P_BASE	(P_SIZE + 2)	// start page address (1=Z180 or 2=Z280 bytes)
#define P_ATTR	(P_BASE + 2)	// attributes (1 byte)
#define P_STAT	(P_ATTR + 1)	// status flags (1 byte)
#define P_NAME	(P_STAT + 1)	// partition name (6 chars)
#define P_WAIT	(P_NAME + 6)	// pointer to wait queue list head (2 bytes)
#define P_TCB	(P_WAIT + 2)	// ptr to TCB of owner (2 bytes)
#define P_PROT	(P_TCB + 2)	// protection word for common partition (2 bytes)

#define PCBSZ	(P_PROT + 2)

/* Checkpoint PCB */

#if 0
#define P_LNK	0		// link to next PCB in list (2 bytes)
#define P_MAIN	(P_LNK + 2)	// pointer to main PCB (2 bytes)
#define P_SUB	(P_MAIN + 2)	// pointer to next checkpoint PCB (2 bytes)
#define P_SIZE	(P_SUB + 2)	// size of checkpointed task in disk blks (2 bytes)
#endif
#define P_UCB	(P_SIZE + 2)	// UCB of checkpoint file device (2 bytes)
#define P_LBN	(P_UCB + 2)	// starting LBN (4 bytes)
#define P_REL	(P_LBN + 4)	// relative block# within chkpnt file (2 bytes)

/* Partition attributes */

#define PA_SUB	0		// subpartition
#define PA_SYS	1		// system-controlled
#define PA_CHK	2		// not checkpointable
#define PA_FXD	3		// fixed
#define PA_NST	4		// not shuffable
#define PA_COM	5		// lib or common block
#define PA_DEL	6		// delete when not attached

/* Partition status bits */

#define PS_OUT	0		// partition is out of memory
#define PS_CKP	1		// checkpoint in progress
#define PS_CKR	2		// checkpoint requested
#define PS_BSY	3		// partition busy
#define PS_DRV	4		// driver loaded in partition

/* DCB offsets */

#define D_LNK	0		// link to next DCB in device list (2 bytes)
#define D_ST	(D_LNK + 2)	// status (1 byte)
#define D_TCNT	(D_ST + 1)	// timeout counter (2 bytes)
#define D_NAME	(D_TCNT + 2)	// device name (2 bytes)
#define D_UNITS	(D_NAME + 2)	// number of units (1 byte)
#define D_UCBL	(D_UNITS + 1)	// link to list of UCBs (2 bytes)
#define D_BANK	(D_UCBL + 2)	// device driver page (BBR) (1 byte)
#define D_START	(D_BANK + 1)	// device driver start address (2 bytes)
#define D_END	(D_START + 2)	// device driver end address (2 bytes)
#define D_EPT	(D_END + 2)	// device driver jump table entry point (2 bytes)

/* DCB size */

#define DCBSZ	(D_EPT + 2)

/* Status bit numbers */

#define DS_RES	0		// resident (unloadable)

/* Unit Control Block structure, one per device unit (statically allocated) */

#define U_LNK	0		// link to next UCB in list (2 bytes)
#define U_UNIT	(U_LNK + 2)	// physical unit number (1 byte)
#define U_ST	(U_UNIT + 1)	// status bits (1 byte)
#define U_DCB	(U_ST + 1)	// pointer to the corresponding DCB (2 bytes)
#define U_SCB	(U_DCB + 2)	// pointer to the related SCB (2 bytes)
#define U_RED	(U_SCB + 2)	// redirect pointer (2 bytes)
#define U_CTL	(U_RED + 2)	// control bits (1 byte)
#define U_CW	(U_CTL + 1)	// characteristics word (2 bytes)
#define U_ATT	(U_CW + 2)	// TCB of attached task (0=detached) (2 bytes)
#define U_ACP	(U_ATT + 2)	// TCB of ACP task (0=no ACP) (2 bytes)
#define U_LCB	(U_ACP + 2)	// LCB of owner (2 bytes)

/* UCB min size */

#define UCBSZ	(U_LCB + 2)

/* Unit control bit numbers */

#define UC_ATT	0		// send attach/detach notifications

/* UCB terminal extension fields */

#define UX_BDR	(UCBSZ + 0)	// initial baud rate

/* UCB status bit numbers */

#define US_OFL	0		// unit is offline
#define US_BSY	1		// unit busy
#define US_MNT	2		// unit mounted
#define US_PUB	3		// unit is public
#define US_DMO	4		// unit is being dismounted (file access denied)
#define US_RED	5		// unit can be redirected
#define US_AST	6		// generate AST on I/O completion for the attached task

/* Device/Unit Characteristics Word bit numbers: Lo-byte */

#define DV_REC	0		// record-oriented (block) device
#define DV_TTY	1		// terminal (character) device
#define DV_DIR	2		// directory device
#define DV_MNT	3		// device mountable
#define DV_PSE	4		// pseudo-device

/* Device/Unit Characteristics Word bit numbers: Hi-byte (device-dependent)
   Terminal characteristics bits */

#define TC_TTS	0		// misc terminal operation sync bit
#define TC_BIN	1		// binary mode (pass-all)
#define TC_NEC	2		// no-echo mode
#define TC_SCP	3		// scope mode
#define TC_SLV	4		// slave mode
#define TC_SMR	5		// uppercase conversion on input disabled
#define TC_NBR	6		// not receiving broadcast messages
#define	TC_ANS	7		// ANSI terminal

/* Additional terminal characteristics codes, not present un U_CW */

#define TC_SPD	8		// serial speed, if supported
#define TC_PAR	9		// parity, if supported (0=none, 1=odd, 3=even)
#define TC_BPC	10		// bits per character, if supported
#define TC_STP	11		// stop bits, if supported
#define TC_FLC	12		// flow control type, if supported

/* Status Control Block, one per controller (statically allocated) */

#define S_ST	0		// controller status (1 byte)
#define S_PKTL	(S_ST + 1)	// link to I/O packet queue (2 bytes)
#define S_CPKT	(S_PKTL + 2)	// address of current I/O packet (2 bytes)

/* SCB min size */

#define SCBSZ	(S_CPKT + 2)

/* Logical device assignment structure, one per logical device */

#define N_LNK	0		// link to next item in list (2 bytes)
#define N_TYPE	(N_LNK + 2)	// assignment type (1 byte)
#define N_TI	(N_TYPE + 1)	// UCB of terminal (2 bytes)
#define N_NAME	(N_TI + 2)	// logical device name (2 bytes)
#define N_UNIT	(N_NAME + 2)	// logical device unit (1 byte)
#define N_UCB	(N_UNIT + 1)	// UCB of physical device (2 bytes)

/* Logical Assignment structure size */

#define LASZ	(N_UCB + 2)

/* Assignment types */

#define N_LCL	0		// local
#define N_LGN	1		// login
#define N_GBL	2		// global

#define SYSRST	0x20
#define DBGRST	0x30

/* Terminal baud rate codes */

#define S_0	0		// means baud rates not supported
#define S_50	1
#define S_75	2
#define S_110	3
#define S_134	4		// IBM Selectric typewriter
#define S_150	5
#define S_200	6
#define S_300	7
#define S_600	8
#define S_1200	9
#define S_1800	10
#define S_2000	11
#define S_2400	12
#define S_3600	13
#define S_4800	14
#define S_7200	15
#define S_9600	16
#define S_14K4	17
#define S_19K2	18
#define S_28K8	19
#define S_38K4	20
#define S_57K6	21
#define S_76K8	22
#define S_115K2	23
#define S_UNK	-1		// Unknown


#endif  // __RSX180_H
