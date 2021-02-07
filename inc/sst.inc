; SST vector offsets

SST.IL	equ	0		; Illegal Instruction (Z180)
SST.EI	equ	0		; Extended Instruction (EPU<->reg, Z280)
SST.E2	equ	SST.EI + 2	; Extended Instruction (EPU<->mem, Z280)
SST.PR	equ	SST.E2 + 2	; Privileged Instruction (Z280)
SST.SC	equ	SST.PR + 2	; Unrecognized System Call (Z180, Z280)
SST.AC	equ	SST.SC + 2	; Access Violation (Z280)
SST.DV	equ	SST.AC + 2	; Division exception (Z280)
SST.SS	equ	SST.DV + 2	; Single-Step (Z280)
SST.BP	equ	SST.SS + 2	; Breakpoint-on-halt (Z280)

SSTSZ	equ	SST.BP + 2	; SST vector table size

