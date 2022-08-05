! T3X -> Z80 compiler (CP/M, RSX180/280)
! Nils M Holm, 2017,2019,2020
! Public Domain / CC0 license
! REL version by H Peraza, 2022

module  t3xz(t3x);

object	t[t3x];

const	BPW = 2;

const	STACK_SIZE = 128;	! not used?

const	BUFLEN = 128;		! disk I/O buffer size

const	SYMTBL_SIZE = 512;
const	LABEL_SIZE = 1536;
const	NLIST_SIZE = 3072;
const	FWDCL_SIZE = 64;
const	RTLIB_SIZE = 10;

var	Outname::40;
var	Modname::10;

var	Line;			! input line number

var	Verbose;

var	Pass;

const	ENDFILE = %1;
const	EOFCHAR = 0x1a;

const	FALSE = 0;
const	TRUE = %1;

! convert unsigned value to decimal ASCII string

var ntoa_buf::7;

ntoa(x) do var i, k;
	if (x = 0) return "0";
	if (x = 0x8000) return "32768"; ! special case
	i := 6;
	ntoa_buf::i := 0;	! null terminator
	k := x<0-> -x: x;
	while (k > 0) do var j;
		i := i-1;
		j := k mod 10;
		k := k/10;
		if ((x < 0) /\ (i = 5)) do
			k := 6553-k;
			j := 6-j;
			if (j < 0) do
				j := j+10;
				k := k-1;
			end
		end
		ntoa_buf::i := '0' + j;
	end
	return @ntoa_buf::i;
end

str_length(s) return t.memscan(s, 0, 32767);

str_copy(sd, ss) t.memcopy(sd, ss, str_length(ss)+1);

str_append(sd, ss) t.memcopy(@sd::str_length(sd), ss, str_length(ss)+1);

str_equal(s1, s2) return t.memcomp(s1, s2, str_length(s1)+1) = 0;

writes(s) t.write(1, s, str_length(s));	! write to stdout

! display error message

aw(m, s) do
	writes("\nT3X -- Error: ");
	writes(ntoa(Line));     		! display line number
	writes(": ");
	writes(m);				! display message
	if (s \= 0) do
		writes(": ");
		writes(s);			! display symbol name, if any
	end
	writes("\r");
	if (Outname::0) t.remove(Outname);	! delete output
	halt 4;					! and exit
end

oops(m, s) do
	writes("\nT3X -- Internal error\r");
	aw(m, s);
end

numeric(c) return '0' <= c /\ c <= '9';

alphabetic(c) return 'a' <= c /\ c <= 'z' \/
		     'A' <= c /\ c <= 'Z';

!
! Symbol tables
!

struct	SYM = SNAME, SFLAGS, SVALUE, SSEG;	! symbol table fields

! symbol flags (bits)

const	GLOB = 1;	! global scope (i.e. not inside a function or block)
const	CNST = 2;	! constant
const	VECT = 4;	! vector
const	FORW = 8;	! forward ref
const	FUNC = 16;	! function
const	IFUNC = 32;	! interface function
const	CLSS = 64;	! class
const	OBJCT = 128;	! object

! REL segments

const	ASEG = 0x00;	! absolute (constant)
const	CSEG = 0x40;	! code segment
const	DSEG = 0x80;	! data segment
const	SEGBITS = 0xc0;	! segment bits mask
const	EXTRN = 0x02;	! external reference
const	PUBLC = 0x01;	! public symbol

var	Syms[SYM*SYMTBL_SIZE];	! symbol table
var	Labels[LABEL_SIZE];
Var	Lab;
Var     Lskip;
var	Nlist::NLIST_SIZE;

var	Yp, Np;			! pointer to top of Syms, Nlist

var	Fwlab[FWDCL_SIZE],	! forward reference table: label
	Fwaddr[FWDCL_SIZE];	! forward reference table: address (always in code segment)
var	Fwp;			! number of entries in above table

var	RTlib[RTLIB_SIZE];
var	Rp;

! search symbol in table (search done backwards since chances of referencing
! local symbols entered last are higher)

find(s) do var i;
	i := Yp-SYM;
	while (i >= 0) do
		if (str_equal(Syms[i+SNAME], s))
			return @Syms[i];
		if (Syms[i+SFLAGS] & CLSS)
			i := i - SYM * Syms[i+SVALUE];	! skip over class
		i := i - SYM;
	end
	return 0;
end

findmessage(y, s) do var i, match;
	i := y-Syms-SYM;
	while (i >= 0) do
		match := str_equal(Syms[i+SNAME], s);
		if (Syms[i+SFLAGS] & IFUNC)
			i := i - SYM;	! point to func
		if (match)
			return @Syms[i];
		i := i - SYM;
	end
	return 0;
end

! enter new name (string) in name pool
! symbol table contains pointers to names in this table

newname(s) do var k, new;
	k := str_length(s)+1;
	if (Np+k >= NLIST_SIZE)
		aw("name pool overflow", s);
	new := @Nlist::Np;
	t.memcopy(new, s, k);
	Np := Np+k;
	return new;
end

! enter new symbol in symbol table

add(s, f, v, vs) do var y;
	y := find(s);
	if (y \= 0) do
		ie (y[SFLAGS] & FORW /\ f & FUNC)
			return y;
		else
			aw("redefined", s);
	end
	if (Yp+SYM >= SYMTBL_SIZE*SYM)
		aw("too many symbols", 0);
	y := @Syms[Yp];
	Yp := Yp+SYM;
	y[SNAME] := newname(s);
	y[SFLAGS] := f;
	y[SVALUE] := v;
	y[SSEG] := vs;
	return y;
end

! add a forward label reference (mark)

addfwd(l, a) do
	if (Fwp >= FWDCL_SIZE)
		aw("too many forward declarations", 0);
	Fwlab[Fwp] := l;
	Fwaddr[Fwp] := a;
	Fwp := Fwp+1;
end

!
! REL file output functions
!

var	Outfile;
var	Outbuf::BUFLEN;
var	Outp;

var bitcnt, byte, curseg;

initrel() do
	bitcnt := 0;
	byte := 0;
	curseg := ASEG;
end

flush() do			! flush output buffer
	if (\Outp) return;
	if (t.write(Outfile, Outbuf, Outp) \= Outp)
		aw("file write error", 0);
	Outp := 0;
end

wrbyte() do			! write REL byte
	if (Outp >= BUFLEN) flush();
	Outbuf::Outp := byte;
	Outp := Outp + 1;
end

wrbit(b) do			! write single bit
	if (bitcnt = 8) do
		wrbyte();
		bitcnt := 0;
	end
	bitcnt := bitcnt+1;
	byte := byte << 1;
	if (b & 0x80) byte := byte | 1;
end

wrbits(b, n) do			! write n bits
	while (n > 0) do
		wrbit(b);
		b := b << 1;
		n := n-1;
	end
end

wr8b(b) do			! write 8 bits
	wrbits(b, 8);
end

wr16b(b) do			! write 16 bits
	wr8b(b);
	wr8b(b >> 8);
end

wrpad() do			! pad to byte boundary
	while (bitcnt < 8) wrbit(0);
end

wreof() do			! write end of file record
	wrbits(0x9e, 7);
	wrpad();		! pad to byte boundary
	wrbyte();		! write last byte
	flush();		! and flush output file
end

wreom(addr, seg) do		! write end of module record
	wrbits(0x9c, 7);
	wrbits(seg, 2);		! write start address
	wr16b(addr);
	wrpad();		! pad to byte boundary
end

wrname(s, n) do var i, c;	! write name
	if (n > 8) n := 8;
	wrbits(n << 5, 3);
	i := 0;
	while (n > 0) do
		c := s::i;
		if ('a' <= c /\ c <= 'z') c := c & 0x5f;
		wr8b(c);
		i := i + 1;
		n := n-1;
	end
end

wrmodname(s, n) do		! write module name
	wrbits(0x84, 7);
	wrname(s, n);
end

wr8a(b) do			! write absolute 8-bit value
	wrbit(0);
	wr8b(b);
end

wr16a(w) do			! write absolute 16-bit value
	wr8a(w);
	wr8a(w >> 8);
end

wr16r(w, seg) do		! write relative 16-bit value
	ie (seg = 0) do
		wr16a(w);
	end else do
		wrbit(0x80);
		wrbits(seg, 2);
		wr16b(w);
	end
end

wrextchain(a, seg, name, n) do	! write External chain head
	wrbits(0x8c, 7);
	wrbits(seg, 2);
	wr16b(a);
	wrname(name, n);
end

wrchain(addr, seg) do		! write chain address entry
	wrbits(0x98, 7);
	wrbits(seg, 2);
	wr16b(addr);
end

wrdfentry(a, seg, name, n) do	! write entry point record
	wrbits(0x8e, 7);
	wrbits(seg, 2);
	wr16b(a);
	wrname(name, n);
end

wrentry(name, n) do		! write entry symbol record
	wrbits(0x80, 7);
	wrname(name, n);
end

wrloc(addr, seg) do		! write loc counter
	if (seg \= ASEG) do
		wrbits(0x96, 7);
		wrbits(seg, 2);
		wr16b(addr);
	end
end

wrcsize(size) do		! write code segment size
	wrbits(0x9a, 7);
	wrbits(CSEG, 2);
	wr16b(size);
end

wrdsize(size) do		! write data segment size
	wrbits(0x94, 7);
	wrbits(ASEG, 2);	! L80 quirk
	wr16b(size);
end

selseg(addr, seg, force) do
	if ((seg \= curseg) \/ force) do
		if (Pass) wrloc(addr, seg);
		curseg := seg;
	end
end

!
! Emitter
!

var	Gp;

var	Tp, Dp, Lp, Ls, Lp0;	! Tp = next address in text segment
				! Dp = next address in data segment
				! Lp = next address in local stack frame

var	Tsize, Dsize;		! text and data segment sizes

var	Tstart;			! main entry point

var	Acc;

var	Codetbl;

struct	OPT = OINST1, OARG, OINST2, OREPL;

var	Opttbl;

struct	CG =	CG_NULL,
		CG_PUSH, CG_CLEAR, CG_DROP,
		CG_LDVAL, CG_LDADDR, CG_LDLREF, CG_LDGLOB,
		CG_LDLOCL,
		CG_STGLOB, CG_STLOCL, CG_STINDR, CG_STINDB,
		CG_INCGLOB, CG_INCLOCL, CG_INCR,
		CG_STACK, CG_UNSTACK, CG_LOCLVEC, CG_GLOBVEC,
		CG_INDEX, CG_DEREF, CG_INDXB, CG_DREFB,
		CG_CALL, CG_CALR, CG_JUMP, CG_JMPFALSE,
		CG_JMPTRUE, CG_FOR, CG_FORDOWN, CG_MKFRAME,
		CG_DELFRAME, CG_RET, CG_START, CG_HALT,
		CG_NEG, CG_INV, CG_LOGNOT, CG_ADD, CG_SUB,
		CG_MUL, CG_DIV, CG_MOD, CG_AND, CG_OR, CG_XOR,
		CG_SHL, CG_SHR, CG_EQ, CG_NEQ, CG_LT, CG_GT,
		CG_LE, CG_GE, CG_JMPEQ, CG_JMPNE, CG_JMPLT,
		CG_JMPGT, CG_JMPLE, CG_JMPGE;

! search for label in table

findlab(id) return Labels[id-1];

! enter new label, labels are identified by their seq number

newlab() do
	if (Lab >= LABEL_SIZE) aw("too many labels", 0);
	Lab := Lab+1;
	return Lab;
end

decl	commit(0);

resolve(id) do
	commit();
	Labels[id-1] := Tp;
end

resolve_gbl(id) do
	commit();
	Labels[id-1] := Gp;
end

resolve_fwd(a) do var i;
	i := 0;
	while (i < Fwp) do
		if (Fwaddr[i] = a) do
			resolve(Fwlab[i]);
			return;
		end
		i := i+1;
	end
	oops("unknown forward reference", 0);
end

emit(x) do
	Tp := Tp+1;
	if (Pass = 0) return;
	wr8a(x);
end

emit_gbl(x) do
	Gp := Gp+1;
	if (Pass = 0) return;
	wr8a(x);
end

emitw(x) do
	Tp := Tp+2;
	if (Pass = 0) return;
	wr16r(x, 0);
end

emitwr(x, seg) do
	if (curseg = CSEG) Tp := Tp+2;
	if (curseg = DSEG) Gp := Gp+2;
	if (Pass = 0) return;
	wr16r(x, seg);
end

hex(c)	ie (numeric(c))
		return c-'0';
	else
		return c-'a'+10;

! emit code block from Codetbl

rgen(s, v, vs, y) do var cv, cs;
	selseg(Tp, CSEG, 0);			! select code segment
	while (s::0) do
		ie (s::0 = ',') do		! special arg follows?
			ie (s::1 = 'w') do	! w = emit word
				cv := v;
				cs := vs;
				if (y /\ vs & EXTRN) do
					cv := y[SVALUE]; ! chain external
					cs := y[SSEG] & SEGBITS;
					y[SVALUE] := Tp;
					y[SSEG] := (y[SSEG] & ~SEGBITS) | CSEG;!vs;
				end
				emitwr(cv, cs);
			end
			else ie (s::1 = 'l')	! l = emit lo byte
				emit(v);
			else ie (s::1 = 'h')	! h = emit hi byte
				emit(v+1);
			else ie (s::1 = 'x') do	! x = external RT lib reference
				s := s+2;
				y := RTlib[16*hex(s::0) + hex(s::1)];
				cv := y[SVALUE];
				cs := y[SSEG] & SEGBITS;
				y[SVALUE] := Tp;
				y[SSEG] := (y[SSEG] & ~SEGBITS) | CSEG;
				emitwr(cv, cs);
			end
			else
				oops("bad code", 0);
		end
		else do
			emit(16*hex(s::0) + hex(s::1));	! emit literal byte
		end
		s := s+2;
	end
end

var	Qi, Qa, Qs, Qy;	! Code table index and optional argument (addr, seg)

commit() do
	rgen(Codetbl[Qi][1], Qa, Qs, Qy);
	Qi := CG_NULL;
end

gen(id, a, seg, y) do var i, skiparg;
	skiparg := %1;
	i := 0;
	while (Opttbl[i] \= %1) do
		ie (Opttbl[i][OINST1] = %1)
			skiparg := 0;
		else if (Qi = Opttbl[i][OINST1] /\
			 id = Opttbl[i][OINST2] /\
			 (skiparg \/ Qa = Opttbl[i][OARG]))
		do
			Qi := Opttbl[i][OREPL];
			Qa := a;
			Qs := seg;
			Qy := y;
			return;
		end
		i := i+1;
	end
	if (Qi \= CG_NULL) commit();
	Qi := id;
	Qa := a;
	Qs := seg;
	Qy := y;
end

gen0(id) gen(id, 0, 0, 0);

spill() ie (Acc)
		gen0(CG_PUSH);
	else
		Acc := 1;

active() return Acc;

clear() Acc := 0;

activate() Acc := 1;

rt_lib(name) do
	RTlib[Rp] := add(name, 0, 0, EXTRN);
	Rp := Rp+1;
end

class_func(name, arity, iname) do
	add(iname, GLOB|FUNC  | (arity << 8), 0, EXTRN);
	add(name,  GLOB|IFUNC | (arity << 8), 0, 0);
end

globaddr() do var l, i, g;
	g := Gp;
	selseg(Gp, DSEG, 0);
	emitwr(0, 0);
	Gp := Gp+2;
	return g;
end

align(x, a) return (x+a) & ~(a-1);

!
! Scanner
!

const	META	 = 256;

const	TOKEN_LEN = 64;

var	Infile;
var	Inbuf::BUFLEN;
var	Ip, Ep;
var	Rejected;
var	Tk;
var	Str::TOKEN_LEN;
var	Val;
var	Oid;

var	Equal_op, Minus_op, Mul_op, Add_op;

struct	OPER = OPREC, OLEN, ONAME, OTOK, OCODE;

var	Ops;

!
! Token definitions
!

struct	TOKENS =
	SYMBOL, INTEGER, STRING,
	ADDROF, ASSIGN, BINOP, BYTEOP, COLON, COMMA, COND,
	CONJ, DISJ, DOT, LBRACK, LPAREN, RBRACK, RPAREN, SEMI, UNOP,
	KCALL, KCONST, KDECL, KEXTERNAL, KDO, KELSE, KEND, KFOR, KHALT, KIE,
	KIF, KLEAVE, KLOOP, KMODULE, KOBJECT, KPACKED, KRETURN,
	KSTRUCT, KVAR, KWHILE;

! symbol lookup, output error if undefined or if type not expected

decl	expect(2), xsymbol(0), scan(0);

lookup(s, f) do var y;
	y := find(s);
	if (y /\ y[SFLAGS] & OBJCT) y := y[SVALUE];
	if (y /\ y[SFLAGS] & CLSS) do
		Tk := scan();
		expect(DOT, ".");
		Tk := scan();
		xsymbol();
		y := findmessage(y, Str);
	end
	if (y = 0) aw("undefined", s);
	if (y[SFLAGS] & f \= f)
		aw("unexpected type", s);
	return y;
end

readrc() do var c;
	if (Rejected) do
		c := Rejected;
		Rejected := 0;
		return c;
	end
	if (Ip >= Ep) do
		Ep := t.read(Infile, Inbuf, BUFLEN);
		Ip := 0;
	end
	if (Ip >= Ep) return ENDFILE;
	c := Inbuf::Ip;
	Ip := Ip+1;
	return c;
end

readc() do var c;
	c := readrc();
	return 'A' <= c /\ c <= 'Z'-> c-'A'+'a': c;
end

readec() do var c;
	c := readrc();
	if (c \= '\\') return c;
	c := readrc();
	if (c = 'a') return '\a';
	if (c = 'b') return '\b';
	if (c = 'e') return '\e';
	if (c = 'f') return '\f';
	if (c = 'n') return '\n';
	if (c = 'q') return '"' | META;
	if (c = 'r') return '\r';
	if (c = 's') return '\s';
	if (c = 't') return '\t';
	if (c = 'v') return '\v';
	return c;
end

reject(c) Rejected := c;

skip() do var c;
	c := readc();
	while (1) do
		while (c = ' ' \/ c = '\t' \/ c = '\n' \/ c = '\r') do
			if (c = '\n') Line := Line+1;
			c := readc();
		end
		if (c \= '!')
			return c;
		while (c \= '\n' /\ c \= ENDFILE)
			c := readc();
	end
end

findkw(s) do
	if (s::0 = 'c') do
		if (str_equal(s, "call")) return KCALL;
		if (str_equal(s, "const")) return KCONST;
		return 0;
	end
	if (s::0 = 'd') do
		if (str_equal(s, "do")) return KDO;
		if (str_equal(s, "decl")) return KDECL;
		return 0;
	end
	if (s::0 = 'e') do
		if (str_equal(s, "else")) return KELSE;
		if (str_equal(s, "end")) return KEND;
		if (str_equal(s, "external")) return KEXTERNAL;
		return 0;
	end
	if (s::0 = 'f') do
		if (str_equal(s, "for")) return KFOR;
		return 0;
	end
	if (s::0 = 'h') do
		if (str_equal(s, "halt")) return KHALT;
		return 0;
	end
	if (s::0 = 'i') do
		if (str_equal(s, "if")) return KIF;
		if (str_equal(s, "ie")) return KIE;
		return 0;
	end
	if (s::0 = 'l') do
		if (str_equal(s, "leave")) return KLEAVE;
		if (str_equal(s, "loop")) return KLOOP;
		return 0;
	end
	if (s::0 = 'm') do
		if (str_equal(s, "mod")) return BINOP;
		if (str_equal(s, "module")) return KMODULE;
		return 0;
	end
	if (s::0 = 'o') do
		if (str_equal(s, "object")) return KOBJECT;
		return 0;
	end
	if (s::0 = 'p') do
		if (str_equal(s, "packed")) return KPACKED;
		return 0;
	end
	if (s::0 = 'r') do
		if (str_equal(s, "return")) return KRETURN;
		return 0;
	end
	if (s::0 = 's') do
		if (str_equal(s, "struct")) return KSTRUCT;
		return 0;
	end
	if (s::0 = 'v') do
		if (str_equal(s, "var")) return KVAR;
		return 0;
	end
	if (s::0 = 'w') do
		if (str_equal(s, "while")) return KWHILE;
		return 0;
	end
	return 0;
end

scanop(c) do var i, j;
	i := 0;
	j := 0;
	Oid := %1;
	while (Ops[i][OLEN] > 0) do
		ie (Ops[i][OLEN] > j) do
			if (Ops[i][ONAME]::j = c) do
				Oid := i;
				Str::j := c;
				c := readc();
				j := j+1;
			end
		end
		else do
			leave;
		end
		i := i+1;
	end
	if (Oid = %1) do
		Str::j := c;
		j := j+1;
		Str::j := 0;
		aw("unknown operator", Str);
	end
	Str::j := 0;
	reject(c);
	return Ops[Oid][OTOK];
end

findop(s) do var i;
	i := 0;
	while (Ops[i][OLEN] > 0) do
		if (str_equal(s, Ops[i][ONAME])) do
			Oid := i;
			return Oid;
		end
		i := i+1;
	end
	oops("operator not found", s);
end

symbolic(c) return alphabetic(c) \/ c = '_';

scan() do var c, i, k, sgn, base;
	c := skip();
	if (c = ENDFILE \/ c = EOFCHAR) do
		str_copy(Str, "end of file");
		return ENDFILE;
	end
	if (symbolic(c)) do
		i := 0;
		while (symbolic(c) \/ numeric(c)) do
			if (i >= TOKEN_LEN-1) do
				Str::i := 0;
				aw("symbol too long", Str);
			end
			Str::i := c;
			i := i+1;
			c := readc();
		end
		Str::i := 0;
		reject(c);
		k := findkw(Str);
		if (k \= 0) do
			if (k = BINOP) findop(Str);
			return k;
		end
		return SYMBOL;
	end
	if (numeric(c) \/ c = '%') do
		sgn := 1;
		i := 0;
		if (c = '%') do
			sgn := %1;
			c := readc();
			Str::i := c;
			i := i+1;
			if (\numeric(c))
				aw("missing digits after '%'", 0);
		end
		base := 10;
		if (c = '0') do
			c := readc();
			if (c = 'x') do
				base := 16;
				c := readc();
				if (\numeric(c) /\ (c < 'a' \/ c > 'f'))
					aw("missing digits after '0x'", 0);
			end
		end
		Val := 0;
		while (	numeric(c) \/
			base = 16 /\ 'a' <= c /\ c <= 'f'
		) do
			if (i >= TOKEN_LEN-1) do
				Str::i := 0;
				aw("integer too long", Str);
			end
			Str::i := c;
			i := i+1;
			c := c >= 'a'-> c-'a'+10: c-'0';
			Val := Val * base + c;
			c := readc();
		end
		Str::i := 0;
		reject(c);
		Val := Val * sgn;
		return INTEGER;
	end
	if (c = '\'') do
		Val := readec();
		if (readc() \= '\'')
			aw("missing ''' in character", 0);
		return INTEGER;
	end
	if (c = '"') do
		i := 0;
		c := readec();
		while (c \= '"' /\ c \= ENDFILE) do
			if (i >= TOKEN_LEN-1) do
				Str::i := 0;
				aw("string too long", Str);
			end
			Str::i := c & (META-1);
			i := i+1;
			c := readec();
		end
		Str::i := 0;
		return STRING;
	end
	return scanop(c);
end

!
! Parser
!

const	MAXTBL	 = 128;
const	MAXLOOP	 = 100;

var	Retlab;
var	Frame;
var	Loop0;
var	Leaves[MAXLOOP], Lvp;
var	Loops[MAXLOOP], Llp;

expect(tok, s) do var b::100;
	if (tok = Tk) return;
	str_copy(b, s);
	str_append(b, " expected");
	aw(b, Str);
end

xeqsign() do
	if (Tk \= BINOP \/ Oid \= Equal_op)
		expect(BINOP, "'='");
	Tk := scan();
end

xsemi() do
	expect(SEMI, "';'");
	Tk := scan();
end

xlparen() do
	expect(LPAREN, "'('");
	Tk := scan();
end

xrparen() do
	expect(RPAREN, "')'");
	Tk := scan();
end

xsymbol() expect(SYMBOL, "symbol");

constfac() do var v, y;
	if (Tk = INTEGER) do
		v := Val;
		Tk := scan();
		return v;
	end
	if (Tk = SYMBOL) do
		y := lookup(Str, CNST);
		Tk := scan();
		return y[SVALUE];
	end
	aw("constant value expected", Str);
end

constval() do var v;
	v := constfac();
	ie (Tk = BINOP /\ Oid = Mul_op) do
		Tk := scan();
		v := v * constfac();
	end
	else if (Tk = BINOP /\ Oid = Add_op) do
		Tk := scan();
		v := v + constfac();
	end
	return v;
end

checklocal(y)
	if (y[SVALUE] > 126 \/ y[SVALUE] < -126)
		aw("local storage exceeded", y[SNAME]);

vardecl(glb) do var y, size, a;
	Tk := scan();
	while (1) do
		xsymbol();
		ie (glb & GLOB) do
			a := globaddr();
			y := add(Str, glb, a, DSEG);
		end
		else do
			y := add(Str, 0, Lp, 0);
		end
		Tk := scan();
		size := 1;
		ie (Tk = LBRACK) do
			Tk := scan();
			size := constval();
			if (size < 1)
				aw("invalid size", 0);
			y[SFLAGS] := y[SFLAGS] | VECT;
			expect(RBRACK, "']'");
			Tk := scan();
		end
		else if (Tk = BYTEOP) do
			Tk := scan();
			size := constval();
			if (size < 1)
				aw("invalid size", 0);
			size := (size + BPW-1) / BPW;
			y[SFLAGS] := y[SFLAGS] | VECT;
		end
		ie (glb & GLOB) do
			if (y[SFLAGS] & VECT) do
				if (Lskip) resolve(Lskip);
				Lskip := 0;
				gen(CG_STACK, -(size*BPW), 0, 0);
				Dp := Dp + size*BPW;
				!gen(CG_LDVAL, Gp, DSEG, 0);
				!Gp := Gp + size*BPW;
				gen(CG_GLOBVEC, a, DSEG, 0);
				!if (Lskip) do
				!	Lskip := newlab();
				!	gen(CG_JUMP, findlab(Lskip), CSEG, 0);
				!end
			end
		end
		else do
			ie (y[SFLAGS] & VECT) do
				gen(CG_STACK, -((Ls+size)*BPW), 0, 0);
				Lp := Lp - size*BPW;
				Ls := 0;
				gen0(CG_LOCLVEC);
			end
			else do
				Ls := Ls + 1;
			end
			Lp := Lp - BPW;
			y[SVALUE] := Lp;
			checklocal(y);
		end
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	xsemi();
end

constdecl(glb) do var y;
	Tk := scan();
	while (1) do
		xsymbol();
		y := add(Str, glb|CNST, 0, 0);
		Tk := scan();
		xeqsign();
		y[SVALUE] := constval();
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	xsemi();
end

stcdecl(glb) do var y, i;
	Tk := scan();
	xsymbol();
	y := add(Str, glb|CNST, 0, 0);
	Tk := scan();
	xeqsign();
	i := 0;
	while (1) do
		xsymbol();
		add(Str, glb|CNST, i, 0);
		i := i+1;
		Tk := scan();
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	y[SVALUE] := i;
	xsemi();
end

fwddecl() do var y, n, l;
	Tk := scan();
	if (Lskip = 0) do
		Lskip := newlab();
		gen(CG_JUMP, findlab(Lskip), CSEG, 0);
	end
	while (1) do
		xsymbol();
		l := newlab();
		commit();
		addfwd(l, Tp);
		y := add(Str, GLOB|FORW, Tp, CSEG);
		gen(CG_JUMP, findlab(l), CSEG, 0);
		Tk := scan();
		xlparen();
		n := constval();
		if (n < 0) aw("invalid arity", 0);
		y[SFLAGS] := y[SFLAGS] | (n << 8);
		xrparen();
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	xsemi();
end

extdecl() do var y, n, l;
	Tk := scan();
	while (1) do
		xsymbol();
		y := add(Str, GLOB|FUNC, 0, EXTRN);
		Tk := scan();
		xlparen();
		n := constval();
		if (n < 0) aw("invalid arity", 0);
		y[SFLAGS] := y[SFLAGS] | (n << 8);
		xrparen();
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	xsemi();
end

decl	stmt(1);

fundecl() do
	var	l_base, l_addr;
	var	i, na, oyp, onp;
	var	y;

	if (Verbose) do
		writes("\n");
		writes(Str);
		writes("\r");
	end
	l_addr := 2*BPW;
	na := 0;
	if (Lskip = 0) do
		Lskip := newlab();
		gen(CG_JUMP, findlab(Lskip), CSEG, 0);
	end
	commit();
	y := add(Str, GLOB|FUNC, Tp, CSEG);
	Tk := scan();
	oyp := Yp;		! mark top of symbol table
	onp := Np;		! mark top of name pool
	l_base := Yp;		! start of func formal params
	xlparen();
	while (Tk = SYMBOL) do
		add(Str, 0, l_addr, 0);	! add formal param
		l_addr := l_addr + BPW;
		na := na+1;
		Tk := scan();
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	xrparen();
	for (i = l_base, Yp, SYM) do
		Syms[i+SVALUE] := 6+na*BPW - Syms[i+SVALUE];	! fix formal param addr
	end
	if (y[SFLAGS] & FORW) do
		if (na \= y[SFLAGS] >> 8)
			aw("function does not match DECL", y[SNAME]);
		y[SFLAGS] := y[SFLAGS] & ~FORW | FUNC;
		resolve_fwd(y[SVALUE]);
		y[SVALUE] := Tp;
	end
	y[SFLAGS] := y[SFLAGS] | (na << 8);	! arity is in 8 upper bits of SFLAGS
	if (na) gen0(CG_MKFRAME);
	Frame := na;
	Retlab := newlab();
	stmt(1);		! process function body
	if (Retlab) resolve(Retlab);
	Retlab := 0;
	if (Frame) gen0(CG_DELFRAME);
	gen0(CG_RET);
	Yp := oyp;		! restore top of symbol table (purge locals)
	Np := onp;		! restore top of name pool
	Lp := 0;
end

declaration(glb)
	ie (Tk = KVAR)
		vardecl(glb);
	else ie (Tk = KCONST)
		constdecl(glb);
	else ie (Tk = KSTRUCT)
		stcdecl(glb);
	else ie (Tk = KDECL)
		fwddecl();
	else ie (Tk = KEXTERNAL)
		extdecl();
	else
		fundecl();

decl	expr(1);

load(y) ie (y[SFLAGS] & GLOB)
		gen(CG_LDGLOB, y[SVALUE], y[SSEG], y);
	else
		gen(CG_LDLOCL, y[SVALUE], 0, 0);

store(y)
	ie (y[SFLAGS] & GLOB)
		gen(CG_STGLOB, y[SVALUE], y[SSEG], y);
	else
		gen(CG_STLOCL, y[SVALUE], 0, 0);

fncall(fn, ind) do var i, msg;
	msg := "call of non-function";
	Tk := scan();
	if (fn = 0) aw(msg, 0);
	if (\ind /\ fn[SFLAGS] & (FUNC|FORW) = 0) aw(msg, fn[SNAME]);
	i := 0;
	while (Tk \= RPAREN) do
		expr(0);
		i := i+1;
		if (Tk \= COMMA) leave;
		Tk := scan();
		if (Tk = RPAREN)
			aw("syntax error", Str);
	end
	if (\ind /\ i \= fn[SFLAGS] >> 8)
		aw("wrong number of arguments", fn[SNAME]);
	expect(RPAREN, "')'");
	Tk := scan();
	if (active()) spill();
	ie (fn[SFLAGS] & (FUNC|FORW))
		gen(CG_CALL, fn[SVALUE], fn[SSEG], fn);
	else do
		load(fn);
		gen0(CG_CALR);
	end
	if (i \= 0) gen(CG_UNSTACK, i*BPW, 0, 0);
	activate();
end

mkstring(s) do var i, a, k, l;
	k := str_length(s);
	commit();
	a := Gp;
	selseg(Gp, DSEG, 0);
	for (i=0, k+1) emit_gbl(s::i);
	return a;
end

mkbytevec() do var a, l, k;
	Tk := scan();
	expect(LBRACK, "'['");
	Tk := scan();
	commit();
	a := Gp;
	selseg(Gp, DSEG, 0);
	while (1) do
		k := constval();
		if (k > 255 \/ k < 0)
			aw("byte vector member out of range", Str);
		emit_gbl(k);
		if (Tk \= COMMA) leave;
		Tk := scan();
	end
	expect(RBRACK, "']'");
	Tk := scan();
	return a;
end

var	gtbl[MAXTBL*3], gaf[MAXTBL*3], gseg[MAXTBL*3];

mktable2(depth) do
	var	n, i, a, l, y;
	var	tbl, af, seg;
	var	dynamic;

	if (depth > 2) aw("table nesting too deep", 0);
	tbl := @gtbl[depth*128];
	seg := @gseg[depth*128];
	af := @gaf[depth*128];
	Tk := scan();
	dynamic := 0;
	n := 0;
	while (Tk \= RBRACK) do
		if (n >= MAXTBL)
			aw("table too big", 0);
		ie (Tk = LPAREN /\ \dynamic) do
			Tk := scan();
			dynamic := 1;
			loop;
		end
		else ie (dynamic) do
			expr(1);
			l := newlab();
			gen(CG_STGLOB, findlab(l), DSEG, 0);
			tbl[n] := 0;
			seg[n] := 0;
			af[n] := l;
			if (Tk = RPAREN) do
				Tk := scan();
				dynamic := 0;
			end
		end
		else ie (Tk = INTEGER \/ Tk = SYMBOL) do
			tbl[n] := constval();
			seg[n] := 0;
			af[n] := 0;
		end
		else ie (Tk = STRING) do
			tbl[n] := mkstring(Str);
			seg[n] := DSEG;
			af[n] := 0;
			Tk := scan();
		end
		else ie (Tk = LBRACK) do
			tbl[n] := mktable2(depth+1);
			seg[n] := DSEG;
			af[n] := 0;
		end
		else ie (Tk = KPACKED) do
			tbl[n] := mkbytevec();
			seg[n] := DSEG;
			af[n] := 0;
		end
		else ie (Tk = ADDROF) do
			Tk := scan();
			xsymbol();
			y := lookup(Str, FUNC);
			tbl[n] := y[SVALUE];
			seg[n] := y[SSEG];
			af[n] := 0;
			Tk := scan();
		end
		else do
			aw("invalid table element", Str);
		end
		n := n+1;
		if (Tk \= COMMA) leave;
		Tk := scan();
		if (Tk = RBRACK)
			aw("syntax error", Str);
	end
	if (dynamic)
		aw("missing ')' in dynamic table", 0);
	expect(RBRACK, "']'");
	if (n = 0) aw("empty table", 0);
	Tk := scan();
	commit();
	a := Gp;
	selseg(Gp, DSEG, 0);
	for (i=0, n) do
		if (af[i]) resolve_gbl(af[i]);
		emitwr(tbl[i], seg[i]);
	end
	return a;
end

mktable() return mktable2(0);

decl	factor(0);

address(lv, bp) do var y;
	y := lookup(Str, 0);
	Tk := scan();
	ie (y[SFLAGS] & CNST) do
		if (lv > 0) aw("invalid location", y[SNAME]);
		spill();
		gen(CG_LDVAL, y[SVALUE], y[SSEG], 0);
	end
	else ie (y[SFLAGS] & (FUNC|FORW)) do
		! Don't load
	end
	else if (lv = 0 \/ Tk = LBRACK \/ Tk = BYTEOP) do
		spill();
		load(y);
	end
	if (Tk = LBRACK \/ Tk = BYTEOP)
		if (y[SFLAGS] & (FUNC|FORW|CNST))
			aw("bad subscript", y[SNAME]);
	while (Tk = LBRACK) do
		Tk := scan();
		bp[0] := 0;
		expr(0);
		expect(RBRACK, "']'");
		Tk := scan();
		y := 0;
		gen0(CG_INDEX);
		if (lv = 0 \/ Tk = LBRACK  \/ Tk = BYTEOP)
			gen0(CG_DEREF);
	end
	if (Tk = BYTEOP) do
		Tk := scan();
		bp[0] := 1;
		factor();
		y := 0;
		gen0(CG_INDXB);
		if (lv = 0) gen0(CG_DREFB);
	end
	return y;
end

factor() do var y, op, b;
	ie (Tk = INTEGER) do
		spill();
		gen(CG_LDVAL, Val, 0, 0);
		Tk := scan();
	end
	else ie (Tk = SYMBOL) do
		y := address(0, @b);
		if (Tk = LPAREN) fncall(y, 0);
	end
	else ie (Tk = STRING) do
		spill();
		gen(CG_LDADDR, mkstring(Str), DSEG, 0);
		Tk := scan();
	end
	else ie (Tk = LBRACK) do
		spill();
		gen(CG_LDADDR, mktable(), DSEG, 0);
	end
	else ie (Tk = KPACKED) do
		spill();
		gen(CG_LDADDR, mkbytevec(), DSEG, 0);
	end
	else ie (Tk = ADDROF) do
		Tk := scan();
		y := address(2, @b);
		ie (y = 0) do
			;
		end
		else ie (y[SFLAGS] & GLOB) do
			spill();
			gen(CG_LDADDR, y[SVALUE], y[SSEG], y);
		end
		else do
			spill();
			gen(CG_LDLREF, y[SVALUE], y[SSEG], y);
		end
	end
	else ie (Tk = BINOP) do
		if (Oid \= Minus_op)
			aw("syntax error", Str);
		Tk := scan();
		factor();
		gen0(CG_NEG);
	end
	else ie (Tk = UNOP) do
		op := Oid;
		Tk := scan();
		factor();
		gen0(Ops[op][OCODE]);
	end
	else ie (Tk = LPAREN) do
		Tk := scan();
		expr(0);
		xrparen();
	end
	else ie (Tk = KCALL) do
		Tk := scan();
		xsymbol();
		y := lookup(Str, 0);
		Tk := scan();
		if (Tk \= LPAREN) aw("incomplete CALL", 0);
		fncall(y, 1);
	end
	else do
		aw("syntax error", Str);
	end
end

emitop(stk, p) do
	gen0(Ops[stk[p-1]][OCODE]);
	return p-1;
end

arith() do var stk[10], p;
	factor();
	p := 0;
	while (Tk = BINOP) do
		while (p /\ Ops[Oid][OPREC] <= Ops[stk[p-1]][OPREC])
			p := emitop(stk, p);
		stk[p] := Oid;
		p := p+1;
		Tk := scan();
		factor();
	end
	while (p > 0)
		p := emitop(stk, p);
end

logop(conop) do var l;
	ie (conop)
		arith();
	else
		logop(%1);
	l := 0;
	while (Tk = (conop-> CONJ: DISJ)) do
		Tk := scan();
		if (\l) l := newlab();
		commit();
		gen(conop-> CG_JMPFALSE: CG_JMPTRUE, findlab(l), CSEG, 0);
		clear();
		ie (conop)
			arith();
		else
			logop(%1);
	end
	if (l) resolve(l);
end

expr(clr) do var l1, l2;
	if (clr) clear();
	logop(0);
	if (Tk = COND) do
		Tk := scan();
		l1 := newlab();
		l2 := newlab();
		gen(CG_JMPFALSE, findlab(l1), CSEG, 0);
		expr(1);
		expect(COLON, "':'");
		Tk := scan();
		gen(CG_JUMP, findlab(l2), CSEG, 0);
		resolve(l1);
		expr(1);
		resolve(l2);
	end
end

halt_stmt() do var r;
	Tk := scan();
	r := Tk = SEMI-> 0: constval();
	gen(CG_HALT, r, 0, 0);
	xsemi();
end

return_stmt() do
	Tk := scan();
	if (Retlab = 0)
		aw("cannot return from main body", 0);
	ie (Tk = SEMI)
		gen0(CG_CLEAR);
	else
		expr(1);
	ie (Frame /\ Lp /\ Lp0 = Lp) do
		gen(CG_JUMP, findlab(Retlab), CSEG, 0);
	end
	else do
		if (Lp \= 0) gen(CG_UNSTACK, -Lp, 0, 0);
		if (Frame) gen0(CG_DELFRAME);
		gen0(CG_RET);
	end
	xsemi();
end

if_stmt(alt) do var l1, l2;
	Tk := scan();
	xlparen();
	expr(1);
	l1 := newlab();
	gen(CG_JMPFALSE, findlab(l1), CSEG, 0);
	xrparen();
	stmt(0);
	if (alt) do
		l2 := newlab();
		gen(CG_JUMP, findlab(l2), CSEG, 0);
		resolve(l1);
		l1 := l2;
		expect(KELSE, "ELSE");
		Tk := scan();
		stmt(0);
	end
	resolve(l1);
end

while_stmt() do var olp, olv, l, a0;
	Tk := scan();
	commit();
	olp := Loop0;
	olv := Lvp;
	a0 := Tp;
	Loop0 := Tp;
	xlparen();
	expr(1);
	xrparen();
	l := newlab();
	gen(CG_JMPFALSE, findlab(l), CSEG, 0);
	stmt(0);
	gen(CG_JUMP, a0, CSEG, 0);
	resolve(l);
	while (Lvp > olv) do
		resolve(Leaves[Lvp-1]);
		Lvp := Lvp-1;
	end
	Loop0 := olp;
end

for_stmt() do
	var	y, l, a0;
	var	step;
	var	oll, olp, olv;
	var	test;

	Tk := scan();
	oll := Llp;
	olv := Lvp;
	olp := Loop0;
	Loop0 := 0;
	xlparen();
	xsymbol();
	y := lookup(Str, 0);
	if (y[SFLAGS] & (CNST|FUNC|FORW))
		aw("unexpected type", y[SNAME]);
	Tk := scan();
	xeqsign();
	expr(1);
	store(y);
	expect(COMMA, "','");
	Tk := scan();
	commit();
	a0 := Tp;
	test := Tp;
	load(y);
	expr(0);
	ie (Tk = COMMA) do
		Tk := scan();
		step := constval();
	end
	else do
		step := 1;
	end
	l := newlab();
	gen(step<0-> CG_FORDOWN: CG_FOR, findlab(l), CSEG, 0);
	xrparen();
	stmt(0);
	while (Llp > oll) do
		resolve(Loops[Llp-1]);
		Llp := Llp-1;
	end
	ie (y[SFLAGS] & GLOB) do
		ie (step = 1) do
			gen(CG_INCGLOB, y[SVALUE], y[SSEG], y);
		end
		else do
			gen(CG_LDGLOB, y[SVALUE], y[SSEG], y);
			gen(CG_INCR, step, 0, 0);
			gen(CG_STGLOB, y[svalue], y[SSEG], y);
		end
	end
	else do
		ie (step = 1) do
			gen(CG_INCLOCL, y[SVALUE], 0, 0);
		end
		else do
			gen(CG_LDLOCL, y[SVALUE], 0, 0);
			gen(CG_INCR, step, 0, 0);
			gen(CG_STLOCL, y[svalue], 0, 0);
		end
	end
	gen(CG_JUMP, a0, CSEG, 0);
	resolve(l);
	while (Lvp > olv) do
		resolve(Leaves[Lvp-1]);
		Lvp := Lvp-1;
	end
	Loop0 := olp;
end

leave_stmt() do var l;
	Tk := scan();
	if (Loop0 < 0)
		aw("LEAVE not in loop context", 0);
	xsemi();
	if (Lvp >= MAXLOOP)
		aw("too many LEAVEs", 0);
	l := newlab();
	Leaves[Lvp] := l;
	gen(CG_JUMP, findlab(l), CSEG, 0);
	Lvp := Lvp+1;
end

loop_stmt() do var l;
	Tk := scan();
	if (Loop0 < 0)
		aw("LOOP not in loop context", 0);
	xsemi();
	ie (Loop0 > 0) do
		gen(CG_JUMP, Loop0, CSEG, 0);
	end
	else do
		if (Llp >= MAXLOOP)
			aw("too many LOOPs", 0);
		l := newlab();
		Loops[Llp] := l;
		gen(CG_JUMP, findlab(l), CSEG, 0);
		Llp := Llp+1;
	end
end

asg_or_call() do var y, b;
	clear();
	y := address(1, @b);
	ie (Tk = LPAREN) do
		fncall(y, 0);
	end
	else ie (Tk = ASSIGN) do
		Tk := scan();
		expr(0);
		ie (y = 0)
			gen0(b-> CG_STINDB: CG_STINDR);
		else ie (y[SFLAGS] & (FUNC|FORW|CNST|VECT))
			aw("bad location", y[SNAME]);
		else
			store(y);
	end
	else do
		aw("syntax error", Str);
	end
	xsemi();
end

decl	compound(1);

stmt(body) ie (Tk = KFOR)
		for_stmt();
	else ie (Tk = KHALT)
		halt_stmt();
	else ie (Tk = KIE)
		if_stmt(1);
	else ie (Tk = KIF)
		if_stmt(0);
	else ie (Tk = KELSE)
		aw("ELSE without IE", 0);
	else ie (Tk = KLEAVE)
		leave_stmt();
	else ie (Tk = KLOOP)
		loop_stmt();
	else ie (Tk = KRETURN)
		return_stmt();
	else ie (Tk = KWHILE)
		while_stmt();
	else ie (Tk = KDO)
		compound(body);
	else ie (Tk = SYMBOL)
		asg_or_call();
	else ie (Tk = KCALL) do
		clear();
		factor();
	end
	else ie (Tk = SEMI)
		Tk := scan();
	else
		expect(%1, "statement");

compound(body) do var oyp, olp, onp, ols, msg;
	msg := "unexpected end of compound statement";
	Tk := scan();
	oyp := Yp;
	onp := Np;
	olp := Lp;
	ols := Ls;
	Ls := 0;
	while (Tk = KVAR \/ Tk = KCONST \/ Tk = KSTRUCT) do
		if (Tk = KVAR /\ \Frame) do
			gen0(CG_MKFRAME);
			Frame := 1;
		end
		declaration(0);
	end
	if (Ls) gen(CG_STACK, -(Ls*BPW), 0, 0);
	if (body) Lp0 := Lp;
	while (Tk \= KEND) do
		if (Tk = ENDFILE) aw(msg, 0);
		stmt(0);
	end
	Tk := scan();
	if (body) do
		gen0(CG_CLEAR);
		resolve(Retlab);
		Retlab := 0;
	end
	if (olp \= Lp) gen(CG_UNSTACK, olp-Lp, 0, 0);
	if (body /\ Frame) do
		gen0(CG_DELFRAME);
		Frame := 0;
	end
	Yp := oyp;
	Np := onp;
	Lp := olp;
	Ls := ols;
end

checkclass() do var y;
	y := find(Str);
	if (y /\ y[SFLAGS] & CLSS) return y;
	aw("class not found", Str);
end

module_decl() do var i;
	Tk := scan();
	xsymbol();
	i := str_length(Str);
	if (i > 8) i := 8;
	t.memcopy(Modname, Str, i);
	Modname::i := 0;
	Tk := scan();
	xlparen();
	xsymbol();
	checkclass();
	Tk := scan();
	xrparen();
	xsemi();
end

object_decl() do var y, s::TOKEN_LEN;
	Tk := scan();
	xsymbol();
	str_copy(s, Str);
	Tk := scan();
	expect(LBRACK, "'['");
	Tk := scan();
	expect(SYMBOL, "symbol");
	y := checkclass();
	add(s, GLOB|OBJCT, y, 0);
	Tk := scan();
	expect(RBRACK, "']'");
	Tk := scan();
	xsemi();
end

program() do var i;
	Tstart := Tp;
	gen(CG_START, Tstart+6, CSEG, 0);
	Tk := scan();
	if (Tk = KMODULE) module_decl();
	if (Tk = KOBJECT) object_decl();
	while (	Tk = KVAR \/ Tk = KCONST \/ Tk = SYMBOL \/
		Tk = KDECL \/ Tk = KSTRUCT \/ Tk = KEXTERNAL
	)
		declaration(GLOB);
	if (Tk \= KDO)
		aw("DO or declaration expected", 0);
	if (Lskip) resolve(Lskip);
	Lskip := 0;
!	Tstart := Tp;
!	gen(CG_START, 0, CSEG, 0);!Tstart+6, CSEG, 0);
	compound(0);
	if (Tk \= ENDFILE)
		aw("trailing characters", Str);
	gen0(CG_HALT);
!	commit();
!	Tstart := Tp;
!	gen(CG_START, 0, CSEG, 0);
	for (i=0, Yp, SYM)
		if (Syms[i+SFLAGS] & FORW /\ Syms[i+SVALUE])
			aw("undefined function", Syms[i+SNAME]);
end

!
! Main
!

init(p) do var i, b::10;
	initrel();
	Tsize := Tp;	! from previous pass
	Dsize := Gp;
	Tstart := 0;
	Pass := p;
	Rejected := 0;
	Ip := 0;
	Ep := 0;
	Gp := 0;	! start of global variable pool
	Outp := 0;
	Tp := 0;	! start of code
	Dp := 0;	! start of data
	Lp := 0;
	Yp := 0;	! clear symbol table
	Np := 0;	! clear symbol name pool
	Fwp := 0;	! clear forward ref table
	Lab := 0;
	Line := 1;
	Acc := 0;
	Retlab := 0;
	Frame := 0;
	Loop0 := %1;
	Lvp := 0;
	Llp := 0;
	Qi := CG_NULL;
	Codetbl := [
		[ CG_NULL,	""			],
		[ CG_PUSH,	"e5"			],
		[ CG_CLEAR,	"210000"		],
		[ CG_DROP,	"d1"			],
		[ CG_LDVAL,	"21,w"			],
		[ CG_LDADDR,	"21,w"			],
		[ CG_LDLREF,	"21,wdde5d119"		],
		[ CG_LDGLOB,	"2a,w"			],
		[ CG_LDLOCL,	"dd66,hdd6e,l"		],
		[ CG_STGLOB,	"22,w"			],
		[ CG_STLOCL,	"dd74,hdd75,l"		],
		[ CG_STINDR,	"ebe1732372"		],
		[ CG_STINDB,	"ebe173"		],
		[ CG_INCGLOB,	"21,w3423200134"	],
		[ CG_INCLOCL,	"dd34,l2003dd34,h"	],
		[ CG_INCR,	"11,w19"		],
		[ CG_STACK,	"21,w39f9"		],
		[ CG_UNSTACK,	"eb21,w39f9eb"		],
		[ CG_LOCLVEC,	"e5" 			],
		[ CG_GLOBVEC,	"22,w"			],
		[ CG_INDEX,	"29d119"		],
		[ CG_DEREF,	"7e23666f"		],
		[ CG_INDXB,	"d119"			],
		[ CG_DREFB,	"6e2600"		],
		[ CG_CALL,	"cd,w"			],
		[ CG_CALR,	"cd,x04"		],
		[ CG_JUMP,	"c3,w"			],
		[ CG_JMPFALSE,	"7cb5ca,w"		],
		[ CG_JMPTRUE,	"7cb5c2,w"		],
		[ CG_FOR,	"d1ebcd,x00d2,w"	],
		[ CG_FORDOWN,	"d1cd,x00d2,w"		],
		[ CG_MKFRAME,	"dde5dd210000dd39"	],
		[ CG_DELFRAME,	"dde1"			],
		[ CG_RET,	"c9"			],
		[ CG_START,	"01,wc3,x05"		],
		[ CG_HALT,	"21,wc3,x06"		],
		[ CG_NEG,	"110000ebafed52"	],
		[ CG_INV,	"11ffffebafed52"	],
		[ CG_LOGNOT,	"eb21ffff7ab3280123"	],
		[ CG_ADD,	"d119"			],
		[ CG_SUB,	"d1ebafed52"		],
		[ CG_MUL,	"d1cd,x01"		],
		[ CG_DIV,	"d1cd,x02"		],
		[ CG_MOD,	"d1cd,x03"		],
		[ CG_AND,	"d17ca2677da36f"	],
		[ CG_OR,	"d17cb2677db36f"	],
		[ CG_XOR,	"d17caa677dab6f"	],
		[ CG_SHL,	"d1eb432910fd"		],
		[ CG_SHR,	"d1eb43cb3ccb1d10fa"	],
		[ CG_EQ,	"d1afed5221ffff280123"	],
		[ CG_NEQ,	"d1afed5221ffff200123"	],
		[ CG_LT,	"d1ebcd,x0021ffff380123"],
		[ CG_GT,	"d1cd,x0021ffff380123"	],
		[ CG_LE,	"d1cd,x0021000038012b"	],
		[ CG_GE,	"d1ebcd,x0021000038012b"],
		[ CG_JMPEQ,	"d1afed52ca,w"		],
		[ CG_JMPNE,	"d1afed52c2,w"		],
		[ CG_JMPLT,	"d1ebcd,x00da,w"	],
		[ CG_JMPGT,	"d1cd,x00da,w"		],
		[ CG_JMPLE,	"d1cd,x00d2,w"		],
		[ CG_JMPGE,	"d1ebcd,x00d2,w"	],
		[ %1,		""			] ];
	Opttbl := [
		[ CG_EQ,	0,	CG_JMPFALSE,	CG_JMPNE	],
		[ CG_NEQ,	0,	CG_JMPFALSE,	CG_JMPEQ	],
		[ CG_LT,	0,	CG_JMPFALSE,	CG_JMPGE	],
		[ CG_GT,	0,	CG_JMPFALSE,	CG_JMPLE	],
		[ CG_LE,	0,	CG_JMPFALSE,	CG_JMPGT	],
		[ CG_GE,	0,	CG_JMPFALSE,	CG_JMPLT	],
		[ CG_LOGNOT,	0,	CG_JMPFALSE,	CG_JMPTRUE	],
		[ %1,		%1,	%1,		%1		],
		[ CG_LDVAL,	0,	CG_ADD,		CG_DROP		],
		%1 ];
	Ops := [[ 7, 3, "mod",	BINOP,  CG_MOD		],
		[ 6, 1, "+",	BINOP,  CG_ADD		],
		[ 7, 1, "*",	BINOP,  CG_MUL		],
		[ 0, 1, ";",	SEMI,   0		],
		[ 0, 1, ",",	COMMA,  0		],
		[ 0, 1, "(",	LPAREN, 0		],
		[ 0, 1, ")",	RPAREN, 0		],
		[ 0, 1, "[",	LBRACK, 0		],
		[ 0, 1, "]",	RBRACK, 0		],
		[ 3, 1, "=",	BINOP,  CG_EQ		],
		[ 5, 1, "&",	BINOP,  CG_AND		],
		[ 5, 1, "|",	BINOP,  CG_OR		],
		[ 5, 1, "^",	BINOP,  CG_XOR		],
		[ 0, 1, "@",	ADDROF, 0		],
		[ 0, 1, "~",	UNOP,   CG_INV		],
		[ 0, 1, ":",	COLON,  0		],
		[ 0, 2, "::",	BYTEOP, 0		],
		[ 0, 2, ":=",	ASSIGN, 0		],
		[ 0, 1, "\\",	UNOP,   CG_LOGNOT	],
		[ 1, 2, "\\/",	DISJ,   0		],
		[ 3, 2, "\\=",	BINOP,  CG_NEQ		],
		[ 4, 1, "<",	BINOP,  CG_LT		],
		[ 4, 2, "<=",	BINOP,  CG_LE		],
		[ 5, 2, "<<",	BINOP,  CG_SHL		],
		[ 4, 1, ">",	BINOP,  CG_GT		],
		[ 4, 2, ">=",   BINOP,  CG_GE		],
		[ 5, 2, ">>",	BINOP,  CG_SHR		],
		[ 6, 1, "-",	BINOP,  CG_SUB		],
		[ 0, 2, "->",	COND,   0		],
		[ 7, 1, "/",	BINOP,  CG_DIV		],
		[ 2, 2, "/\\",	CONJ,   0		],
		[ 0, 1, ".",	DOT,	0		],
		[ 0, 0, 0,	0,      0		] ];
	Equal_op := findop("=");
	Minus_op := findop("-");
	Mul_op := findop("*");
	Add_op := findop("+");
	i := 0;
	while (Codetbl[i][0] \= %1) do
		if (Codetbl[i][0] \= i) do
			str_copy(b, ntoa(i));
			oops("bad code table entry", b);
		end
		i := i+1;
	end
	Rp := 0;
	! pre-defined symbols:
	rt_lib("?cmp16s");                      ! run-time library
	rt_lib("?mul15s");
	rt_lib("?div15s");
	rt_lib("?mod15s");
	rt_lib("?jphl");
	rt_lib("?init");
	rt_lib("?halt");
	class_func("bpw",     0, "?bpw");       ! the T3X class
	class_func("newline", 1, "?nl");
	class_func("memcomp", 3, "?mcomp");
	class_func("memcopy", 3, "?mcopy");
	class_func("memfill", 3, "?mfill");
	class_func("memscan", 3, "?mscan");
	class_func("getarg",  3, "?gtarg");
	class_func("open",    2, "?open");
	class_func("close",   1, "?close");
	class_func("read",    3, "?read");
	class_func("write",   3, "?write");
	class_func("rename",  2, "?ren");
	class_func("remove",  1, "?del");
	class_func("system",  2, "?sys");
	add("sysin",  GLOB|CNST, 0, 0);
	add("sysout", GLOB|CNST, 1, 0);
	add("syserr", GLOB|CNST, 2, 0);
	add("oread",  GLOB|CNST, 0, 0);
	add("owrite", GLOB|CNST, 1, 0);
	add("t3x", GLOB|CLSS, 14*2+5, 0);
end

info() do
	writes("\nText = ");
	writes(ntoa(Tp));
	writes(", Data = ");
	writes(ntoa(Dp+Gp));
	writes(", Symbols = ");
	writes(ntoa(Yp/SYM));
	writes(", Nlist = ");
	writes(ntoa(Np));
	writes(", Labels = ");
	writes(ntoa(Lab));
	writes("\r");
end

phase(in, n) do
	if (Verbose) do
		writes(n-> "\nPass 2:\r": "\nPass 1:\r");
	end
	Infile := t.open(in, T3X.OREAD);
	if (Infile < 0) aw("no such file", in);
	if (n) do
		Outfile := t.open(Outname, T3X.OWRITE);
		if (Outfile < 0) aw("cannot create", Outname);
	end
	init(n);
	if (n) do var i, j, k;
		if (Modname::0 = 0) do
			i := str_length(Outname);
			k := t.memscan(Outname, ']', i);
			if (k < 0) k := t.memscan(Outname, ':', i);
			k := (k < 0)-> 0: k+1;
			for (i=0, 8) do
				j := k+i;
				if ((Outname::j = 0) \/
				    (Outname::j = '.') \/
				    (Outname::j = ';')) leave;
				Modname::i := Outname::j;
			end
			Modname::i := 0;
		end
		wrmodname(Modname, str_length(Modname));
		wrcsize(Tsize);
		wrdsize(Dsize);
	end
	program();
	commit();
	t.close(Infile);
	if (n) do var i, y, name, seg, addr;
		i := Yp-SYM;
		while (i >= 0) do
			y := @Syms[i];
			if (y[SSEG] & EXTRN) do
				name := y[SNAME];
				seg := y[SSEG] & SEGBITS;
				addr := y[SVALUE];
				if (seg) wrextchain(addr, seg, name, str_length(name));
			end
			i := i - SYM;
		end
		wreom(Tstart, CSEG);
		wreof();
		flush();
		t.close(Outfile);
	end
end

! main routine

do var in::40, k;
	Outname::0 := 0;
	Modname::0 := 0;
	Verbose := 0;
	if (t.getarg(2, in, 4) /\ (str_equal(in, "/V") \/ str_equal(in, "/v")))
		Verbose := 1;
	k := t.getarg(1, in, 30);
	if (k < 0) aw("missing file name", 0);
	t.memcopy(@in::k, ".t3x", 5);
	str_copy(Outname, in);
	t.memcopy(@Outname::k, ".obj", 5);
	phase(in, 0);
	phase(in, 1);
	if (Verbose) info();
	halt 1;
end
