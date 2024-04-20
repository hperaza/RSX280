{$double}

{Free Calc - A spreadsheet program written and donated to public domain
by Martin Burger, all rights reserved, not for sale.}

program sheet;

type
	short = 0..255;
	pcell = ^cell;
	cell = packed record
		row,col:short;
		flag: char;
		ready: short;
		link: pcell;
		value: real;
		format: 0..15;
		prec : 0..15;
		alen: short;
		form: packed array[1..70] of char;
		end;

	pmcell = ^m_cell;
	m_cell = packed record
		row_col:integer;
		flag: char;
		ready: short;
		link: pmcell;
		value: real;
		format: 0..15;
		prec : 0..15;
		alen: short;
		form: packed array[1..70] of char;
		end;

	prcell = ^rcell;
	rcell = packed record
		row,col:short;
		flag: char;
		ready: short;
		link: prcell;
		value: real;
		format: 0..15;
		prec : 0..15;
		case alen: short of
			0,1:(form1: packed array[1..1] of char);
			2:(form2: packed array[1..2] of char);
			3:(form3: packed array[1..3] of char);
			4:(form4: packed array[1..4] of char);
			5:(form5: packed array[1..5] of char);
			6:(form6: packed array[1..6] of char);
			7:(form7: packed array[1..7] of char);
			8:(form8: packed array[1..8] of char);
			9:(form9: packed array[1..9] of char);
			10:(form10: packed array[1..10] of char);
			11:(form11: packed array[1..11] of char);
			12:(form12: packed array[1..12] of char);
			13:(form13: packed array[1..13] of char);
			14:(form14: packed array[1..14] of char);
			15:(form15: packed array[1..15] of char);
			16:(form16: packed array[1..16] of char);
			17:(form17: packed array[1..17] of char);
			18:(form18: packed array[1..18] of char);
			19:(form19: packed array[1..19] of char);
			20:(form20: packed array[1..20] of char);
			21:(form21: packed array[1..21] of char);
			22:(form22: packed array[1..22] of char);
			23:(form23: packed array[1..23] of char);
			24:(form24: packed array[1..24] of char);
			25:(form25: packed array[1..25] of char);
			26:(form26: packed array[1..26] of char);
			27:(form27: packed array[1..27] of char);
			28:(form28: packed array[1..28] of char);
			29:(form29: packed array[1..29] of char);
			30:(form30: packed array[1..30] of char);
			31:(form31: packed array[1..31] of char);
			32:(form32: packed array[1..32] of char);
			33:(form33: packed array[1..33] of char);
			34:(form34: packed array[1..34] of char);
			35:(form35: packed array[1..35] of char);
			36:(form36: packed array[1..36] of char);
			37:(form37: packed array[1..37] of char);
			38:(form38: packed array[1..38] of char);
			39:(form39: packed array[1..39] of char);
			40:(form40: packed array[1..40] of char);
			41:(form41: packed array[1..41] of char);
			42:(form42: packed array[1..42] of char);
			43:(form43: packed array[1..43] of char);
			44:(form44: packed array[1..44] of char);
			45:(form45: packed array[1..45] of char);
			46:(form46: packed array[1..46] of char);
			47:(form47: packed array[1..47] of char);
			48:(form48: packed array[1..48] of char);
			49:(form49: packed array[1..49] of char);
			50:(form50: packed array[1..50] of char);
			51:(form51: packed array[1..51] of char);
			52:(form52: packed array[1..52] of char);
			53:(form53: packed array[1..53] of char);
			54:(form54: packed array[1..54] of char);
			55:(form55: packed array[1..55] of char);
			56:(form56: packed array[1..56] of char);
			57:(form57: packed array[1..57] of char);
			58:(form58: packed array[1..58] of char);
			59:(form59: packed array[1..59] of char);
			60:(form60: packed array[1..60] of char);
			61:(form61: packed array[1..61] of char);
			62:(form62: packed array[1..62] of char);
			63:(form63: packed array[1..63] of char);
			64:(form64: packed array[1..64] of char);
			65:(form65: packed array[1..65] of char);
			66:(form66: packed array[1..66] of char);
			67:(form67: packed array[1..67] of char);
			68:(form68: packed array[1..68] of char);
			69:(form69: packed array[1..69] of char);
			70:(form70: packed array[1..70] of char);
		end;

	keystrokes = set of char;
	mode = (wait,cursor,command,formulas,parameter,execute_command,
		execute_formula,execute_parameter);
	arrow = (up,down,right,left,none);

	unsigned = 0..65535;

var
	chead,ctail : pcell;
	width : packed array[0..255] of short;
	i,j : integer;
	heading : packed array[1..40] of char;
	home_r,home_c : short;
	cur_r,cur_c : short;
	screen : short;
	precision : 0..15;
	justify : integer;
	local_edit : boolean;
	test_r,test_c:short;
	del_row,del_col:integer;
	error : boolean;
	calc_error : boolean;
	buffer : packed array[1..70] of char;
	ip : integer;
	command_type : integer;
	state : mode;
	done : boolean;
	global : boolean;
	direction : arrow;
	inp : file of char;

function memsht:unsigned; external;

function formula(pointer:pcell;var ip:integer):real; forward;

function expression(pointer:pcell;var ip:integer):real; forward;

procedure gotoxy(r,c:integer);
begin
	write(chr(27),'[',r+1:1,';',c+1:1,'H');
end;

procedure setrev;
begin
	write(chr(27),'[7m');
end;

procedure resrev;
begin
	write(chr(27),'[m');
end;

procedure clr_rest;
begin
	write(chr(27),'[0K');
end;

procedure bell;
begin
	write(chr(7));
end;

procedure bwrite(w:integer);
begin
	if w>0 then write(' ':w);
end;

procedure clr_buf;
var
	i:integer;
begin
	for i:=1 to 70 do buffer[i]:=' ';
end;

procedure upper;
var
	i:integer;
begin
	for i:=1 to 70 do
	if (buffer[i]>='a') and (buffer[i]<='z') then
		buffer[i]:=chr(ord(buffer[i])-ord('a')+ord('A'));
end;

procedure rem_blanks(pointer:pcell;var ip:integer);
begin
	while pointer^.form[ip]=' ' do ip:=ip+1;
	if ip>pointer^.alen then error:=true;
end;

function on_screen(r,c:short) : boolean;
var
	i : integer;
	col : integer;
begin
	on_screen:=true;
	if (r<home_r) or (c<home_c) or (r>=(home_r+20)) then on_screen:=false
	else
	  begin
		col:=5;
		for i:=home_c to c do col:=col+width[i];
		if col>=screen then on_screen:=false;
	  end;
end;

procedure dis_alfa(point:pcell ; i:short);
var
	w1,w2 : integer;
begin
	w1:=point^.alen;
	if point^.form[w1]=chr(0) then w1:=w1-1;
	if w1>width[i] then w1:=width[i];
	if w1<0 then w1:=0;
	w2:=width[i]-w1;
	if point^.format=1 then
	begin
		if w2>0 then bwrite(w2);
		if w1>0 then write(point^.form:w1);
	end
	else if point^.format=2 then
	begin
		w2:=w2 div 2;
		if w2>0 then bwrite(w2);
		if w1>0 then write(point^.form:w1);
		w2:=width[i]-w1-w2;
		if w2>0 then bwrite(w2);
	end
	else
	begin
		if w1>0 then write(point^.form:w1);
		if w2>0 then bwrite(w2);
	end
end;

procedure whead(r,c:short);
var
	ch:char;
	i,col : integer;
	v1,v2 : integer;
begin
	bwrite(5);
	col:=5;
	i:=c;
	while (i<=255) and (col+width[i]<screen) do
	begin
	if width[i]>0 then
	 begin
	  v1:=(width[i]-2) div 2;
	  bwrite(v1);
	  v2:=i div 26;
	  if v2=0 then write(' ')
	  else
	  begin
		ch:=chr(ord('A')+v2-1);
		write(ch);
	  end;
	  v2:=i-26*v2;
	  ch:=chr(ord('A')+v2);
	  write(ch);
	  v1:=width[i]-2-v1;
	  bwrite(v1);
	 end;
	  col:=col+width[i];
	  i:=i+1;
	end;
	clr_rest;
end;

procedure dis_form(point:pcell ; i:short);
begin
	  if point<>nil then if point^.alen>0 then
		write(point^.form:point^.alen)
end;

procedure dis_val(point:pcell ; i:short);

procedure cal_form;
var
	j:integer;
begin
with point^ do
	begin
		error:=false;
		calc_error:=false;
		ready:=2;
		j:=1;
		value:=formula(point,j);
		if not calc_error then ready:=1;
		if error then ready:=3;
		dis_val(point,i);
	end;
end;

begin
	  if point=nil then bwrite(width[i])
	  else
	  if width[i]>0 then
	  with point^ do
	  begin
		if flag='A' then dis_alfa(point,i)
		else
		begin
			if ready=3 then write('ERROR':width[i])
			else if ready=2 then write('<VAL/ERR>':width[i])
			else if ready<>0 then write(value:width[i]:prec)
			else
			cal_form;
		end;
	  end;
end;

procedure clr_ready;
var
	head:pcell;
begin
	head:=chead;
	with head^ do
	while (head<>nil) do
	begin
		ready:=0;
		head:=link;
	end;
end;

function get_point(r,c:short):pcell;

{NOTE: will not work if ported to a pascal where integer is not equal to two
bytes. The code should be simpler but since an execution profile of Free Calc
showed that most of the time was spent in this small subroutine I tried to
optimize it for speed.

The correct code for the routine should check if row of cell is equal to r
and column of cell is equal to c. What I have done is put the two together
and check for equality of the row-column combination.

The alternate code should read as follows:

function get_point(r,c:short):pcell;
var
	head,point : pcell;
begin
	head:=chead;
	point:=nil;
	with head^ do
	while (head<>nil) and (point=nil) do
	begin
		if (row=r) and (col=c) then point:=head;
		head:=link;
	end;
	get_point:=point;
end;
}

type
	int_short = packed record
			case boolean of
				true:(i:integer);
				false:(rr,cc:short);
			end;
var
	point,head:pmcell;
	t:int_short;
begin
	t.rr:=r;
	t.cc:=c;
	head:=loophole(pmcell,chead);
	point:=nil;
	with head^ do
	while (head <> nil) and (point = nil) do
	begin
	  if head^.row_col=t.i then point:=head;
	  head:=link;
	end;
	get_point:=loophole(pcell,point);
end;

procedure dis_row(r,c:short);
var
	point : pcell;
	col:integer;
	i:integer;
begin
	setrev;
	write(r+1:3,'  ');
	resrev;
        col:=5;
	i:=c;
	while (i<=255) and (col+width[i]<screen) do
	begin
	  c:=i;
	  point:=get_point(r,c);
	  dis_val(point,c);
	  col:=col+width[i];
	  i:=i+1;
	end;
	clr_rest;
end;

procedure posit(r,c:short);
var
	rr:integer;
	i,col:integer;
begin
	col:=5;
	if c>home_c then for i:=home_c to c-1 do col:=col+width[i];
	rr:=3+r-home_r;
	gotoxy(rr,col);
end;

procedure dis_rc(r,c:short);
var
	v1:integer;
	ch:char;
begin
	v1:=c div 26;
	if v1 <> 0 then
	begin
	  ch:=chr(ord('A')+v1-1);
	  write(ch);
	end;
	v1:=c-v1*26;
	ch:=chr(ord('A')+v1);
	write(ch,r+1:1);
end;

procedure prompt(r,c:short);
var
	point:pcell;
	f:real;
begin
	point:=get_point(r,c);
	posit(r,c);
	setrev;
	dis_val(point,c);
	resrev;
	gotoxy(1,0);
	dis_rc(r,c);
	clr_rest;
	write(':');
	gotoxy(1,10);
	dis_form(point,c);
	gotoxy(0,screen-20);
	f:=memsht;
	if f<0 then f:=f+65536.0;
	write('Memory = ',f:6:0);
	gotoxy(23,0);
	write('Command> ');
	clr_rest;
	gotoxy(23,10);
end;

procedure display(r,c : short);
var
	rs,re : integer;
	i : integer;
begin
	rs:=r;
	re:=rs+19;
	if re>255 then
	begin
		re:=255;
		write(chr(27),'[2J');
	end;
	gotoxy(0,0);
	clr_rest;
	i:=(screen-40) div 2;
	gotoxy(0,i);
	write(heading:40);
	gotoxy(2,0);
	setrev;
	whead(r,c);
	resrev;
	for i:=rs to re do
	  begin
             gotoxy(3+i-rs,0);
             dis_row(i,c);
          end;
	prompt(cur_r,cur_c);
end;

procedure eat_char(pointer:pcell;var ip:integer;c:char);
begin
	rem_blanks(pointer,ip);
	if pointer^.form[ip]=c then ip:=ip+1
	else error:=true;
end;

procedure eat_then(pointer:pcell;var ip:integer);
var
	i:integer;
begin
	rem_blanks(pointer,ip);
	i:=ip;
	with pointer^ do
	if form[i]='@' then
	 if form[i+1]='T' then
	   if form[i+2]='H' then
	     if form[i+3]='E' then
	       if form[i+4]='N' then
		begin
			ip:=ip+5;
		end;
	if ip=i then error:=true;
end;

procedure eat_else(pointer:pcell;var ip:integer);
var
	i:integer;
begin
	rem_blanks(pointer,ip);
	i:=ip;
	with pointer^ do
	if form[i]='@' then
	 if form[i+1]='E' then
	   if form[i+2]='L' then
	     if form[i+3]='S' then
	       if form[i+4]='E' then
		begin
			ip:=ip+5;
		end;
	if ip=i then error:=true;
end;

function get_new(i:short):pcell;
var
	getnew:prcell;
begin
	case i of
		0,1: new(getnew,1);
		2: new(getnew,2);
		3: new(getnew,3);
		4: new(getnew,4);
		5: new(getnew,5);
		6: new(getnew,6);
		7: new(getnew,7);
		8: new(getnew,8);
		9: new(getnew,9);
		10: new(getnew,10);
		11: new(getnew,11);
		12: new(getnew,12);
		13: new(getnew,13);
		14: new(getnew,14);
		15: new(getnew,15);
		16: new(getnew,16);
		17: new(getnew,17);
		18: new(getnew,18);
		19: new(getnew,19);
		20: new(getnew,20);
		21: new(getnew,21);
		22: new(getnew,22);
		23: new(getnew,23);
		24: new(getnew,24);
		25: new(getnew,25);
		26: new(getnew,26);
		27: new(getnew,27);
		28: new(getnew,28);
		29: new(getnew,29);
		30: new(getnew,30);
		31: new(getnew,31);
		32: new(getnew,32);
		33: new(getnew,33);
		34: new(getnew,34);
		35: new(getnew,35);
		36: new(getnew,36);
		37: new(getnew,37);
		38: new(getnew,38);
		39: new(getnew,39);
		40: new(getnew,40);
		41: new(getnew,41);
		42: new(getnew,42);
		43: new(getnew,43);
		44: new(getnew,44);
		45: new(getnew,45);
		46: new(getnew,46);
		47: new(getnew,47);
		48: new(getnew,48);
		49: new(getnew,49);
		50: new(getnew,50);
		51: new(getnew,51);
		52: new(getnew,52);
		53: new(getnew,53);
		54: new(getnew,54);
		55: new(getnew,55);
		56: new(getnew,56);
		57: new(getnew,57);
		58: new(getnew,58);
		59: new(getnew,59);
		60: new(getnew,60);
		61: new(getnew,61);
		62: new(getnew,62);
		63: new(getnew,63);
		64: new(getnew,64);
		65: new(getnew,65);
		66: new(getnew,66);
		67: new(getnew,67);
		68: new(getnew,68);
		69: new(getnew,69);
		70: new(getnew,70);
	end;
	get_new:=loophole(pcell,getnew);
end;

procedure do_dispose(point:pcell);
var
	head,last:pcell;
	rpoint:prcell;
begin
	rpoint:=loophole(prcell,point);
	head:=chead;
	last:=nil;
	while (head<>nil) and (head<>point) do
	begin
		last:=head;
		head:=head^.link;
	end;
	if last<>nil then last^.link:=point^.link
	else chead:=point^.link;
	if point^.link=nil then ctail:=last;
	case point^.alen of
		0,1: dispose(rpoint,1);
		2: dispose(rpoint,2);
		3: dispose(rpoint,3);
		4: dispose(rpoint,4);
		5: dispose(rpoint,5);
		6: dispose(rpoint,6);
		7: dispose(rpoint,7);
		8: dispose(rpoint,8);
		9: dispose(rpoint,9);
		10: dispose(rpoint,10);
		11: dispose(rpoint,11);
		12: dispose(rpoint,12);
		13: dispose(rpoint,13);
		14: dispose(rpoint,14);
		15: dispose(rpoint,15);
		16: dispose(rpoint,16);
		17: dispose(rpoint,17);
		18: dispose(rpoint,18);
		19: dispose(rpoint,19);
		20: dispose(rpoint,20);
		21: dispose(rpoint,21);
		22: dispose(rpoint,22);
		23: dispose(rpoint,23);
		24: dispose(rpoint,24);
		25: dispose(rpoint,25);
		26: dispose(rpoint,26);
		27: dispose(rpoint,27);
		28: dispose(rpoint,28);
		29: dispose(rpoint,29);
		30: dispose(rpoint,30);
		31: dispose(rpoint,31);
		32: dispose(rpoint,32);
		33: dispose(rpoint,33);
		34: dispose(rpoint,34);
		35: dispose(rpoint,35);
		36: dispose(rpoint,36);
		37: dispose(rpoint,37);
		38: dispose(rpoint,38);
		39: dispose(rpoint,39);
		40: dispose(rpoint,40);
		41: dispose(rpoint,41);
		42: dispose(rpoint,42);
		43: dispose(rpoint,43);
		44: dispose(rpoint,44);
		45: dispose(rpoint,45);
		46: dispose(rpoint,46);
		47: dispose(rpoint,47);
		48: dispose(rpoint,48);
		49: dispose(rpoint,49);
		50: dispose(rpoint,50);
		51: dispose(rpoint,51);
		52: dispose(rpoint,52);
		53: dispose(rpoint,53);
		54: dispose(rpoint,54);
		55: dispose(rpoint,55);
		56: dispose(rpoint,56);
		57: dispose(rpoint,57);
		58: dispose(rpoint,58);
		59: dispose(rpoint,59);
		60: dispose(rpoint,60);
		61: dispose(rpoint,61);
		62: dispose(rpoint,62);
		63: dispose(rpoint,63);
		64: dispose(rpoint,64);
		65: dispose(rpoint,65);
		66: dispose(rpoint,66);
		67: dispose(rpoint,67);
		68: dispose(rpoint,68);
		69: dispose(rpoint,69);
		70: dispose(rpoint,70);
	end;
end;

function atom(rrow,ccol:integer):real;
var
	ptr : pcell;
	f : real;
	i : integer;
	c_error,e_error : boolean;
begin
	atom:=0;
    if not local_edit then
    begin
	i:=1;
	 ptr:=get_point(rrow,ccol);
	 with ptr^ do
	 if ptr<>nil then
	 if flag='A' then calc_error:=true
	 else if ready>1 then calc_error:=true
	 else if ready<>0 then atom:=value
	 else
	 begin
		c_error:=calc_error;
		e_error:=error;
		calc_error:=false;
		error:=false;
		f:=formula(ptr,i);
		if calc_error then ready:=2
		else ready:=1;
		if error then ready:=3;
		calc_error:=calc_error or c_error or error;
		error:=e_error;
		if (value<>f) or calc_error then
		begin
			value:=f;
			if on_screen(rrow,ccol) then
			begin
			  posit(rrow,ccol);
			  dis_val(ptr,col);
			end;
		end;
		atom:=value;
	 end
	 else calc_error:=true;
     end;
end;

procedure get_xy(pointer:pcell;var ip:integer;var rrow,ccol:integer);
var
	is:integer;
	absolute_r,absolute_c:integer;

procedure modify;
var
	i,k,l:integer;
	r,c:integer;
begin
	with pointer^ do
	begin
	  if (rrow>=test_r) and (ccol>=test_c) then
	    begin
		r:=rrow+del_row;
		if absolute_r<>0 then r:=rrow;
		c:=ccol+del_col;
		if absolute_c<>0 then c:=ccol;
		if r<9 then l:=1
		  else if r<99 then l:=2
		    else l:=3;
		if c<26 then l:=l+1 else l:=l+2;
		l:=l+absolute_r+absolute_c;
		if ((alen+l-ip+is)<70) and (ip<=alen) and (ip>is)
			and (r>=0) and (r<=255) and (c>=0) and (c<=255) then
		begin
			for i:=ip to alen do
			begin
			  form[i-ip+is]:=form[i];
			end;
			for i:=is to ip-1 do
			begin
				form[alen+is-i]:=chr(0);
			end;
			for i:=alen downto ip do
			begin
			   form[i-ip+is+l]:=form[i-ip+is];
			end;
			i:=is;
			if absolute_c<>0 then
			begin
				form[i]:='$';
				i:=i+1;
			end;
			k:=c div 26;
			if k>0 then
			begin
				form[i]:=chr(k-1+ord('A'));
				i:=i+1;
			end;
			k:=c-26*k;
			form[i]:=chr(k+ord('A'));
			i:=i+1;
			if absolute_r<>0 then
			begin
				form[i]:='$';
				i:=i+1;
			end;
			r:=r+1;
			k:=r div 100;
			if k>0 then
			begin
				form[i]:=chr(k+ord('0'));
				i:=i+1;
			end;
			k:=r-100*k;
			k:=k div 10;
			if r>9 then
			begin
				form[i]:=chr(k+ord('0'));
				i:=i+1;
			end;
			k:=r mod 10;
			form[i]:=chr(k+ord('0'));
			alen:=alen+l-ip+is;
			ip:=is+l;
		end;
	    end;
	end;
end;

begin
	rrow:=0;
	ccol:=0;
	is:=ip;
	absolute_r:=0;
	absolute_c:=0;
	with pointer^ do
       begin
	if form[ip]='$' then
	begin
		absolute_c:=1;
		ip:=ip+1;
	end;
	while (form[ip]>='A') and (form[ip]<='Z') do
	begin
		ccol:=ccol*26+(ord(form[ip])-ord('A')+1);
		ip:=ip+1;
	end;
	ccol:=ccol-1;
	if form[ip]='$' then
	begin
		absolute_r:=1;
		ip:=ip+1;
	end;
	while (form[ip]>='0') and (form[ip]<='9') do
	begin
		rrow:=rrow*10+(ord(form[ip])-ord('0'));
		ip:=ip+1;
	end;
	rrow:=rrow-1;
       end;
	if local_edit then modify;
end;

function elementary(pointer:pcell;var ip:integer):real;
var
	rrow,ccol : integer;
begin
	rem_blanks(pointer,ip);
	elementary:=0;
	get_xy(pointer,ip,rrow,ccol);
	if (rrow<0) or (rrow>255) or (ccol<0) or (ccol>255) then error:=true
	else
	elementary:=atom(rrow,ccol);
end;

function get_range(pointer:pcell;var ip:integer;var r1,c1,r2,c2:integer):boolean;
begin
	get_xy(pointer,ip,r1,c1);
	eat_char(pointer,ip,':');
	get_xy(pointer,ip,r2,c2);
	get_range:=true;
	if (r1<0) or (c1<0) or (r2<0) or (c2<0) or
	   (r1>255) or (c1>255) or (r2>255) or (c2>255) then get_range:=false;
end;

function get_function(pointer:pcell;var ip:integer):real;
type
	tok = packed array[1..8] of char;
var
	token : tok;
	i:integer;
	f:real;
	tok_type:integer;

procedure array_function;
var
	i,j:integer;
	t:real;
	ok,flg:boolean;
	r1,c1,r2,c2:integer;
	items:integer;
begin
with pointer^ do
	begin
		rem_blanks(pointer,ip);
		f:=0;
		ok:=true;
		flg:=true;
		items:=0;
		while (ok) do
		begin
			rem_blanks(pointer,ip);
			i:=ip;
			while form[i] in ['A'..'Z','0'..'9','$',' '] do i:=i+1;
			if form[i]=':' then
			begin
				if get_range(pointer,ip,r1,c1,r2,c2) then
				for i:=r1 to r2 do
				for j:=c1 to c2 do
				begin
					t:=atom(i,j);
					if flg then f:=t;
					items:=items+1;
					case tok_type of
						1,4: if not flg then f:=f+t;
						2: if t>f then f:=t;
						3: if t<f then f:=t;
					end;
					flg:=false;
				end
				else error:=true;
			end
			else 
			begin
				t:=formula(pointer,ip);
				if flg then f:=t;
				items:=items+1;
				case tok_type of
					1,4: if not flg then f:=f+t;
					2: if t>f then f:=t;
					3: if t<f then f:=t;
				end;
				flg:=false;
				items:=items+1;
			end;
			rem_blanks(pointer,ip);
			if form[ip]=',' then ip:=ip+1
			else ok:=false;
		end;
		eat_char(pointer,ip,')');
		if tok_type=4 then if items<>0 then f:=f/items;
		get_function:=f;
	end;
end;

function frac(var f:real):real;
var
	f1,f2:real;
begin
	if f>=0 then f1:=f else f1:=-f;
	while f1>32767 do
	begin
		f2:=32768.0;
		while f2<f1 do f2:=f2*2;
		f2:=f2/2;
		f1:=f1-f2;
	end;
	f1:=f1-trunc(f1);
	if f>=0 then frac:=f1 else frac:=-f1;
end;

procedure do_lookup;
var
	r1,c1,r2,c2 : integer;
	i : integer;
	flg : boolean;
begin
	rem_blanks(pointer,ip);
	f:=formula(pointer,ip);
	eat_char(pointer,ip,',');
	rem_blanks(pointer,ip);
	if get_range(pointer,ip,r1,c1,r2,c2) then
	begin
		if (c2<>(c1+1)) or (c1>c2) then error:=true
		else
		begin
			flg:=false;
			get_function:=0;
			for i:=r1 to r2 do
			begin
				if f>=atom(i,c1) then
				begin
					get_function:=atom(i,c2);
					flg:=true;
				end;
			end;
			if not flg then calc_error:=true;
		end;
	end
	else error:=true;
	eat_char(pointer,ip,')');
end;

begin
	ip:=ip+1;
	token:='        ';
	i:=0;
	with pointer^ do
	while (i<8) and (form[ip]>='A') and (form[ip]<='Z') do
	begin
		i:=i+1;
		token[i]:=form[ip];
		ip:=ip+1;
	end;
	eat_char(pointer,ip,'(');
	tok_type:=0;
	if token='SUM     ' then tok_type:=1
	else if token='MAX     ' then tok_type:=2
	else if token='MIN     ' then tok_type:=3
	else if token='AVG     ' then tok_type:=4
	else if token='LOOKUP  ' then tok_type:=5
	else if token='ABS     ' then tok_type:=6
	else if token='INT     ' then tok_type:=7
	else if token='ROUND   ' then tok_type:=8
	else error:=true;
	with pointer^ do
	case tok_type of
	{SUM,AVG} 1,4:
		begin
			array_function;
		end;
	{MAX} 2:
		begin
			array_function;
		end;

	{MIN} 3:
		begin
			array_function;
		end;

	{LOOKUP} 5:
		begin
			do_lookup;
		end;

	{ABS} 6:
		begin
			f:=formula(pointer,ip);
			eat_char(pointer,ip,')');
			if f<0 then f:=-f;
			get_function:=f;
		end;

	{INT} 7:
		begin
			f:=formula(pointer,ip);
			eat_char(pointer,ip,')');
			if (f>32767) or (f<-32767) then
			begin
				get_function:=f-frac(f);
			end
			else get_function:=trunc(f);
		end;

	{ROUND} 8:
		begin
			f:=formula(pointer,ip);
			eat_char(pointer,ip,')');
			if (f>32767) or (f<-32767) then
			begin
				get_function:=f-frac(f)+round(frac(f));
			end
			else get_function:=round(f);
		end;

	end;
end;

function simple(pointer:pcell;var ip:integer):real;
var
	sign : boolean;
	f : real;
begin
	rem_blanks(pointer,ip);
	sign:=false;
	if pointer^.form[ip]='+' then ip:=ip+1;
	if pointer^.form[ip]='-' then
	begin
		ip:=ip+1;
		sign:=true;
	end;
	if pointer^.form[ip]='@' then f:=get_function(pointer,ip)
	else f:=elementary(pointer,ip);

	if sign then simple:=-f
	else simple:=f;
end;

function is_constant(pointer:pcell;var ip:integer):boolean;
var
	i:integer;
begin
	rem_blanks(pointer,ip);
	i:=ip;
	if (pointer^.form[i]='+') or (pointer^.form[i]='-') then i:=i+1;
	if (pointer^.form[i]='.') or ((pointer^.form[i]>='0') and
		(pointer^.form[i]<='9')) then is_constant:=true
	else is_constant:=false;
end;

function get_constant(pointer:pcell;var ip:integer):real;
var
	sign : boolean;
	f,ff : real;
begin
	rem_blanks(pointer,ip);
	sign:=false;
	if pointer^.form[ip]='+' then ip:=ip+1;
	if pointer^.form[ip]='-' then
	begin
		ip:=ip+1;
		sign:=true;
	end;
	f:=0;
	with pointer^ do
	while (form[ip]>='0') and (form[ip]<='9') do
	begin
		f:=f*10+(ord(form[ip])-ord('0'));
		ip:=ip+1;
	end;
	if pointer^.form[ip]='.' then
	begin
		ff:=0.1;
		ip:=ip+1;
		with pointer^ do
		while (form[ip]>='0') and (form[ip]<='9') do
		begin
			f:=f+(ord(form[ip])-ord('0'))*ff;
			ff:=ff*0.1;
			ip:=ip+1;
		end;
	end;
	if sign then f:=-f;
	get_constant:=f;
end;

function get_log(pointer:pcell;var ip:integer):integer;
var
	i:integer;
	getlog:integer;
begin
	rem_blanks(pointer,ip);
with pointer^ do
begin
	getlog:=0;
	case form[ip] of
	'=': begin
		ip:=ip+1;
		getlog:=1;
	     end;
	'<': begin
		ip:=ip+1;
		getlog:=4;
		case form[ip] of
		'>': begin
			ip:=ip+1;
			getlog:=2;
		     end;
		'=': begin
			ip:=ip+1;
			getlog:=6;
		     end;
		end;
	      end;
	'>': begin
		ip:=ip+1;
		getlog:=3;
		if form[ip]= '=' then
		begin
			ip:=ip+1;
			getlog:=5;
		end;
	      end;
	end;
end;
	if getlog=0 then error:=true;
	get_log:=getlog;
end;

function compare(var f1,f2:real; op:integer):boolean;
begin
	compare:=false;
	case op of
		1: compare:= f1 = f2;
		2: compare:= f1 <> f2;
		3: compare:= f1 > f2;
		4: compare:= f1 < f2;
		5: compare:= f1 >= f2;
		6: compare:= f1 <= f2;
	end;
end;

function variable(pointer:pcell;var ip:integer):real;
begin
	if is_constant(pointer,ip) then variable:=get_constant(pointer,ip)
	else if pointer^.form[ip]='(' then
	   begin
		ip:=ip+1;
		variable:=formula(pointer,ip);
		eat_char(pointer,ip,')');
	   end
	else variable:=simple(pointer,ip);
end;

function pow(pointer:pcell;var ip:integer):real;
var
	f1,f2 : real;
begin
	f1:=variable(pointer,ip);
	rem_blanks(pointer,ip);
	while pointer^.form[ip]='^' do
	begin
		ip:=ip+1;
		f2:=variable(pointer,ip);
		if f1>0 then f1:=exp(f2*ln(f1))
		else calc_error:=true;
		rem_blanks(pointer,ip);
	end;
	pow:=f1;
end;

function mul_div_pow(pointer:pcell;var ip:integer):real;
var
	f1,f2 : real;
begin
	f1:=pow(pointer,ip);
	while pointer^.form[ip] in ['*','/'] do
	begin
		case pointer^.form[ip] of
		'*': begin
			ip:=ip+1;
			f1:=f1*pow(pointer,ip);
		     end;
		'/': begin
			ip:=ip+1;
			f2:=pow(pointer,ip);
			if f2<>0 then f1:=f1/f2
			else calc_error:=true;
		     end;
		end;
	end;
	mul_div_pow:=f1;
end;

function expression;
var
	f1 : real;
begin
	f1:=mul_div_pow(pointer,ip);
	while pointer^.form[ip] in ['+','-'] do
	begin
		case pointer^.form[ip] of
		'+': begin
			ip:=ip+1;
			f1:=f1+mul_div_pow(pointer,ip);
		     end;
		'-': begin
			ip:=ip+1;
			f1:=f1-mul_div_pow(pointer,ip);
		     end;
		end;
	end;
	expression:=f1;
end;

function logical(pointer:pcell;var ip:integer):boolean;
var
	logic : boolean;

function what(pointer:pcell;var ip:integer):boolean;
var
	llog : boolean;
	f1,f2 : real;
	comp : integer;
begin
	rem_blanks(pointer,ip);
	if pointer^.form[ip]='[' then
	begin
		ip:=ip+1;
		llog:=logical(pointer,ip);
		eat_char(pointer,ip,']');
	end
	else
	begin
		f1:=formula(pointer,ip);
		comp:=get_log(pointer,ip);
		f2:=formula(pointer,ip);
		llog:=compare(f1,f2,comp);
	end;
	what:=llog;
end;

begin
	logic:=what(pointer,ip);
	rem_blanks(pointer,ip);
	while (pointer^.form[ip]='|') OR (pointer^.form[ip]='&') do
	begin
		if pointer^.form[ip]='&' then
		begin
			ip:=ip+1;
			logic:=logic AND what(pointer,ip);
		end
		else
		begin
			ip:=ip+1;
			logic:=logic OR what(pointer,ip);
		end;
		rem_blanks(pointer,ip);
	end;
	logical:=logic;
end;

function formula;
var
	logic : boolean;
	f1,f2 : real;
begin
	rem_blanks(pointer,ip);
	if (pointer^.form[ip]='@') and
	   (pointer^.form[ip+1]='I') and
	      (pointer^.form[ip+2]='F') then
		begin
			ip:=ip+3;
			logic:=logical(pointer,ip);
			eat_then(pointer,ip);
			f1:=formula(pointer,ip);
			eat_else(pointer,ip);
			f2:=formula(pointer,ip);
			if logic then formula:=f1
			else formula:=f2;
		end
		else formula:=expression(pointer,ip);
end;

procedure vertical(i:integer);
var
	point : pcell;
begin
	point:=get_point(cur_r,cur_c);
	posit(cur_r,cur_c);
	dis_val(point,cur_c);
	write(chr(27),'[4;23r',chr(27),'[?6h');
	if i<0 then write(chr(27),'M')
	else write(chr(27),'[20;1H',chr(27),'E');
	write(chr(27),'[?6l',chr(27),'[r');
	cur_r:=cur_r+i;
	home_r:=home_r+i;
	if i>0 then direction:=down
	else direction:=up;
	if i<0 then gotoxy(3,0)
	else gotoxy(22,0);
	dis_row(cur_r,home_c);
	prompt(cur_r,cur_c);
end;

procedure horizontal(i:integer);
begin
	cur_c:=cur_c+i;
	home_c:=home_c+i;
	while (cur_c>home_c) and (not on_screen(cur_r,cur_c)) do
	begin
		home_c:=home_c+1;
	end;
	if i>0 then direction:=right
	else direction:=left;
	display(home_r,home_c);
end;

procedure process_cursor;
var
	s_buf : packed array[1..2] of char;
	point : pcell;
begin
	state:=wait;
	get(inp);
	s_buf[1]:=inp^;
	get(inp);
	s_buf[2]:=inp^;
	if s_buf[1]='[' then
	  if (s_buf[2]>='A') and (s_buf[2]<='D') then
	  begin
	   case s_buf[2] of
		'A': if cur_r>0 then if on_screen(cur_r-1,cur_c) then
		     begin
			direction:=up;
			point:=get_point(cur_r,cur_c);
			posit(cur_r,cur_c);
			dis_val(point,cur_c);
			cur_r:=cur_r-1;
			prompt(cur_r,cur_c);
		     end
		     else vertical(-1)
		     else bell;
		'B': if cur_r<255 then if on_screen(cur_r+1,cur_c) then
		     begin
			direction:=down;
			point:=get_point(cur_r,cur_c);
			posit(cur_r,cur_c);
			dis_val(point,cur_c);
			cur_r:=cur_r+1;
			prompt(cur_r,cur_c);
		     end
		     else vertical(1)
		     else bell;
		'C': if cur_c<255 then if on_screen(cur_r,cur_c+1) then
		     begin
			direction:=right;
			point:=get_point(cur_r,cur_c);
			posit(cur_r,cur_c);
			dis_val(point,cur_c);
			cur_c:=cur_c+1;
			prompt(cur_r,cur_c);
		     end
		     else horizontal(1)
		     else bell;
		'D': if cur_c>0 then if on_screen(cur_r,cur_c-1) then
		     begin
			direction:=left;
			point:=get_point(cur_r,cur_c);
			posit(cur_r,cur_c);
			dis_val(point,cur_c);
			cur_c:=cur_c-1;
			prompt(cur_r,cur_c);
		     end
		     else horizontal(-1)
		     else bell;
		end;
	  end
	  else bell
	  else bell;
end;

procedure re_calc;
var
	i:integer;
	head:pcell;
	f:real;
begin
	head:=chead;
	with head^ do
	while (head<>nil) do
		begin
		   if (ready=0) and (flag='F')
		   then
		   if on_screen(row,col) then
		   begin
			i:=1;
			error:=false;
			calc_error:=false;
			ready:=2;
			f:=formula(head,i);
			if calc_error then ready:=2
			else ready:=1;
			if error then ready:=3;
			if (value<>f) or (ready<>1) then
			begin
			  value:=f;
			  posit(row,col);
			  dis_val(head,col);
			end;
		   end;
		   head:=link;
		end;
end;

procedure move(p_from,p_to:pcell);
var
	i:integer;
begin
	with p_to^ do
	begin
		row:=p_from^.row;
		col:=p_from^.col;
		flag:=p_from^.flag;
		ready:=p_from^.ready;
		prec:=p_from^.prec;
		format:=p_from^.format;
		value:=p_from^.value;
		alen:=p_from^.alen;
		for i:=1 to alen do form[i]:=p_from^.form[i];
	end;
end;

procedure help;
var
	out : text;
	buff : packed array [1..80] of char;
	i : integer;
	ok : boolean;
	ch : char;
begin
	reset(out,'sy:calc.hlp',,i);
	if i=-1 then reset(out,'calc.hlp',,i);
	write(chr(27),'[H',chr(27),'[J');
	ok:=true;
	if i<>-1 then
	begin
		while (not eof(out)) and ok do
		begin
			readln(out,buff);
			if buff[1]<>chr(12) then
			begin
				writeln(buff);
			end
			else
			begin
				gotoxy(23,0);
		write('Hit return to continue, anything else to exit ');
				ch:=chr(10);
				while ch=chr(10) do
				begin
					get(inp);
					ch:=inp^;
				end;
				if inp^ <> chr(13) then ok:=false;
				write(chr(27),'[H',chr(27),'[J');
			end;
		end;
		close(out);
	end;
	display(home_r,home_c);
end;

procedure print_out;
var
	out : text;
	i : integer;
	max_r,max_c : integer;
	jstart,jend,len : integer;
	head : pcell;

procedure out_page(var jrow:integer ; jstart,jend:integer);
var
	i,j : integer;
	irow : integer;

procedure out_head;
begin
	writeln(out,chr(12),' ':45,heading);
end;

procedure out_top(jstart,jend:integer);
var
	i : integer;
	v1,v2 : integer;
begin
	write(out,' ':5);
	for i:=jstart to jend do
	begin
		if width[i]>0 then
		begin
			v1:=(width[i]-2) div 2;
			if v1>0 then write(out,' ':v1);
			v2:=i div 26;
			if v2=0 then write(out,' ')
			else
			begin
				write(out,chr(ord('A')+v2-1));
			end;
			v2:=i-26*v2;
			write(out,chr(ord('A')+v2));
			v2:=width[i]-v1-2;
			if v2>0 then write(out,' ':v2);
		end;
	end;
	writeln(out);
end;

procedure print_alfa(point:pcell ; i:short);
var
	w1,w2 : integer;
begin
	w1:=point^.alen;
	if point^.form[w1]=chr(0) then w1:=w1-1;
	if w1>width[i] then w1:=width[i];
	if w1<0 then w1:=0;
	w2:=width[i]-w1;
	if point^.format=1 then
	begin
		if w2>0 then write(out,' ':w2);
		if w1>0 then write(out,point^.form:w1);
	end
	else if point^.format=2 then
	begin
		w2:=w2 div 2;
		if w2>0 then write(out,' ':w2);
		if w1>0 then write(out,point^.form:w1);
		w2:=width[i]-w1-w2;
		if w2>0 then write(out,' ':w2);
	end
	else
	begin
		if w1>0 then write(out,point^.form:w1);
		if w2>0 then write(out,' ':w2);
	end
end;

procedure out_line(jrow,jstart,jend : integer);
var
	i: integer;
	point:pcell;
begin
	write(out,jrow+1:3,': ');
	for i:=jstart to jend do
	if width[i]>0 then
	begin
		point:=get_point(jrow,i);
		if point=nil then write(out,' ':width[i])
		else
		with point^ do
		begin
			gotoxy(1,0);
			clr_rest;
			dis_val(point,i);
			if flag='A' then print_alfa(point,i)
			else
			begin
				if ready>2 then write(out,'ERROR':width[i])
				else if ready=2 then write(out,'<VAL/ERR>':width[i])
				else write(out,value:width[i]:prec);
			end;
		end;
	end;
	writeln(out);
end;

begin
	irow:=jrow;
	out_head;
	out_top(jstart,jend);
	while (jrow<=max_r) do
	begin
		if jrow-irow=50 then
		begin
			out_page(jrow,jstart,jend);
			irow:=jrow;
		end
		else
		begin
			out_line(jrow,jstart,jend);
			jrow:=jrow+1;
		end;
	end;
end;

begin
	i:=0;
	rewrite(out,buffer,'sheet.out',i);
	if i<>-1 then
	begin
		max_r:=0;
		max_c:=0;
		head:=chead;
		with head^ do
		while head<>nil do
		     begin
			if row>max_r then max_r:=row;
			if col>max_c then max_c:=col;
			head:=link;
		     end;
		jstart:=0;
		while (jstart<=max_c) do
		begin
			len:=5;
			i:=jstart;
			jend:=jstart;
			while (i<=max_c) and (len+width[i]<132) do
			begin
				len:=len+width[i];
				jend:=i;
				i:=i+1;
			end;
			i:=0;
			out_page(i,jstart,jend);
			jstart:=jend+1;
		end;
		close(out);
	end;
end;

procedure do_command;
var
	i:integer;
	r,c:integer;
	head:pcell;
	work_cell:cell;

procedure delete;
var
	i:integer;
	head,work,this:pcell;
begin
	head:=chead;
	with head^ do
	while (head<>nil) do
	begin
		this:=link;
		if ((del_row<>0) and (row=cur_r)) or
		   ((del_col<>0) and (col=cur_c)) then
		begin
			do_dispose(head);
		end;
		head:=this;
	end;
	work:=ref(work_cell);
	clr_ready;
	head:=chead;
	while (head<>nil) do
	begin
		this:=head^.link;
		if (head^.ready=0) and (head^.flag='F') then
		begin
			move(head,work);
			i:=1;
			work^.value:=formula(work,i);
			if work^.alen>head^.alen then
			begin
				do_dispose(head);
				head:=get_new(work^.alen);
				if chead=nil then chead:=head;
				if ctail<>nil then ctail^.link:=head;
				ctail:=head;
				head^.link:=nil;
			end;
			move(work,head);
			head^.ready:=1;
		end;
		head:=this;
	end;
	head:=chead;
	with head^ do
	while head<>nil do
	begin
		if (row>=test_r) and (col>=test_c) then
		begin
			row:=row+del_row;
			col:=col+del_col;
		end;
		ready:=0;
		head:=link;
	end;
end;

procedure insert;
var
	i:integer;
	head,work,this:pcell;
begin
	clr_ready;
	work:=ref(work_cell);
	head:=chead;
	while (head<>nil) do
	begin
		this:=head^.link;
		if (head^.ready=0) and (head^.flag='F') then
		begin
			move(head,work);
			i:=1;
			work^.value:=formula(work,i);
			if work^.alen>head^.alen then
			begin
				do_dispose(head);
				head:=get_new(work^.alen);
				if chead=nil then chead:=head;
				if ctail<>nil then ctail^.link:=head;
				ctail:=head;
				head^.link:=nil;
			end;
			move(work,head);
			head^.ready:=1;
		end;
		head:=this;
	end;
	head:=chead;
	with head^ do
	while head<>nil do
	begin
		if (row>=test_r) and (col>=test_c) then
		begin
			row:=row+del_row;
			col:=col+del_col;
		end;
		ready:=0;
		head:=link;
	end;
end;

procedure common;
begin
	state:=parameter;
	gotoxy(23,10);
	clr_rest;
end;

begin
	error:=false;
	calc_error:=false;
	state:=wait;
	global:=false;
	case buffer[2] of

'W': begin
	common;
	command_type:=1;
	write('Enter column width: ');
     end;

'L': begin
	common;
	command_type:=2;
	write('Load what spreadsheet: ');
     end;

'S': begin
	common;
	command_type:=3;
	write('Save as what: ');
     end;

'P': begin
	common;
	command_type:=4;
	if buffer[3]='G' then global:=true;
	write('Digits of precision: ');
     end;

'J': begin
	common;
	command_type:=5;
	if buffer[3]='G' then global:=true;
	write('Justify text L)eft or R)ight or C)enter: ');
     end;

'T': begin
	command_type:=6;
	state:=execute_parameter;
     end;

'H': begin
	common;
	command_type:=7;
	write('Enter new heading: ');
     end;

'I': begin
 	if (buffer[3]='R') or (buffer[3]='C') then
	begin
		if buffer[3]='R' then
		begin
			del_row:=1;
			del_col:=0;
			test_r:=cur_r;
			test_c:=0;
		end
		else
		begin
			del_row:=0;
			del_col:=1;
			test_r:=0;
			test_c:=cur_c;
			for i:=254 downto cur_c do width[i+1]:=width[i];
		end;
		local_edit:=true;
		insert;
		local_edit:=false;
		del_row:=0;
		del_col:=0;
		test_r:=0;
		test_c:=0;
		re_calc;
		display(home_r,home_c);
	end;
     end;

'D': begin
 	if (buffer[3]='R') or (buffer[3]='C') then
	begin
		if buffer[3]='R' then
		begin
			del_row:=-1;
			del_col:=0;
			test_r:=cur_r;
			test_c:=0;
		end
		else
		begin
			del_row:=0;
			del_col:=-1;
			test_r:=0;
			test_c:=cur_c;
			for i:=cur_c to 254 do width[i]:=width[i+1];
		end;
		local_edit:=true;
		delete;
		local_edit:=false;
		del_row:=0;
		del_col:=0;
		test_r:=0;
		test_c:=0;
		re_calc;
		display(home_r,home_c);
	end;
     end;

'=': begin
	state:=wait;
	if ip>2 then
	begin
		for i:=3 to ip do work_cell.form[i-2]:=buffer[i];
		ip:=ip-2;
		work_cell.form[ip+1]:=chr(0);
		head:=ref(work_cell);
		ip:=1;
		get_xy(head,ip,r,c);
		if (r>=0) and (r<=255) and (c>=0) and (c<=255) then
		begin
			if on_screen(r,c) then
			begin
				head:=get_point(cur_r,cur_c);
				posit(cur_r,cur_c);
				dis_val(head,cur_c);
				cur_r:=r;
				cur_c:=c;
			end
			else
			begin
				cur_r:=r;
				home_r:=r;
				cur_c:=c;
				home_c:=c;
				display(home_r,home_c);
			end;
		end;
	end;
     end;

'R': begin
	common;
	command_type:=8;
	write('Replicate range: ');
     end;
	
'O': begin
	common;
	command_type:=9;
	write('Print spreadsheet to: ');
     end;

'C': begin
	common;
	command_type:=10;
	write('Copy from: ');
     end;

'?': begin
	help;
     end;

'!': begin
	state:=wait;
	display(home_r,home_c);
     end;

'E': begin
	done:=true;
     end;
 end;
	if state=wait then prompt(cur_r,cur_c);
	clr_buf;
	ip:=0;
end;

procedure do_parameter;
var
	i,j : integer;
	head,this,work : pcell;
	work_cell : cell;
	f:real;
	r,c,r1,c1,r2,c2:integer;
	file_cell : file of cell;

function get_con(ipp:integer):integer;
var
	i,j:integer;
begin
	i:=1;
	j:=0;
	while (i<=ipp) and (buffer[i]>='0') and (buffer[i]<='9') do
	begin
		j:=j*10+(ord(buffer[i])-ord('0'));
		i:=i+1;
	end;
	get_con:=j;
end;

procedure add_span;
begin
	buffer[ip+1]:='/';
	buffer[ip+2]:='s';
	buffer[ip+3]:='p';
	buffer[ip+4]:='a';
	buffer[ip+5]:='n';
end;

begin
	state:=wait;
	case command_type of

1: begin
	j:=get_con(ip);
	if ip>0 then
	begin
		if j=1 then j:=2;
		width[cur_c]:=j;
		if not on_screen(cur_r,cur_c) then
		begin
			home_c:=cur_c;
		end;
		display(home_r,home_c);
	end;
    end;

2: begin
	if ip>0 then
	begin
		add_span;
		reset(file_cell,buffer,'.SHT',i);
		if i<>-1 then
		begin
			head:=chead;
			while (head<>nil) do
			begin
				this:=head^.link;
				do_dispose(head);
				head:=this;
			end;
			chead:=nil;
			ctail:=nil;
			work_cell.link:=ref(work_cell);
			while (not eof(file_cell)) and (work_cell.link<>nil) do
			begin
				read(file_cell,work_cell);
				head:=get_new(work_cell.alen);
				if chead=nil then chead:=head;
				if ctail<>nil then ctail^.link:=head;
				ctail:=head;
				width[work_cell.col]:=work_cell.ready;
				with head^ do
				begin
					row:=work_cell.row;
					col:=work_cell.col;
					flag:=work_cell.flag;
					ready:=0;
					link:=nil;
					format:=work_cell.format;
					prec:=work_cell.prec;
					value:=work_cell.value;
					alen:=work_cell.alen;
					for i:=1 to alen do
					  form[i]:=work_cell.form[i];
				end;
			end;
			cur_r:=0;
			cur_c:=0;
			home_r:=0;
			home_c:=0;
			close(file_cell);
			display(0,0);
		end;
	end;
   end;

3: begin
	i:=0;
	add_span;
	rewrite(file_cell,buffer,'.SHT',i);
	if i<>-1 then
	begin
		head:=chead;
		while head<>nil do
		begin
			i:=head^.ready;
			head^.ready:=width[head^.col];
			write(file_cell,head^);
			head^.ready:=i;
			head:=head^.link;
		end;
		close(file_cell);
	end;
   end;

4: begin
	j:=get_con(ip);
	if j<=15 then
	begin
		if global then precision:=j;
		head:=get_point(cur_r,cur_c);
		if head<>nil then head^.prec:=j;
	end;
   end;

5: begin
	upper;
	i:=-1;
	if buffer[1]='R' then i:=1
	else if buffer[1]='C' then i:=2
	else if buffer[1]='L' then i:=0;
	if i>=0 then
	begin
		head:=get_point(cur_r,cur_c);
		if head<>nil then head^.format:=i;
		if global then justify:=i;
	end;
   end;

6: begin
	if screen=80 then
	begin
		screen:=132;
		write(chr(27),'[?3h');
		display(home_r,home_c);
	end
	else if screen=132 then
	begin
		screen:=80;
		write(chr(27),'[?3l');
		if not on_screen(cur_r,cur_c) then home_c:=cur_c;
		display(home_r,home_c);
	end;
   end;

7: begin
	if ip>0 then
	begin
		for i:=1 to 40 do heading[i]:=' ';
		j:=40-ip;
		j:=j div 2;
		if j<0 then j:=0;
		if ip>40 then ip:=40;
		for i:=1 to ip do heading[i+j]:=buffer[i];
		gotoxy(0,0);
		clr_rest;
		i:=(screen-40) div 2;
		gotoxy(0,i);
		write(heading:40);
	end;
   end;

8: begin
	upper;
	work:=ref(work_cell);
	for i:=1 to 70 do work_cell.form[i]:=buffer[i];
	work_cell.form[ip+1]:=chr(0);
	i:=1;
	if not get_range(work,i,r1,c1,r2,c2) then
	begin
		r2:=r1;
		c2:=c1;
	end;
	test_r:=0;
	test_c:=0;
	this:=get_point(cur_r,cur_c);
	if (this<>nil) and (r1>=0) and (r1<=255) and (c1>=0) and
		(c1<=255) and (r2>=r1) and (c2>=c1) then
	begin
		for r:=r1 to r2 do
		for c:=c1 to c2 do
		begin
			head:=get_point(r,c);
			if head<>this then
			begin
				if (head<>nil) then do_dispose(head);
				move(this,work);
				work_cell.row:=r;
				work_cell.col:=c;
				local_edit:=true;
				del_row:=r-cur_r;
				del_col:=c-cur_c;
				error:=false;
				calc_error:=false;
				i:=1;
				work^.value:=formula(work,i);
				work^.value:=-0.000095623781;
				head:=get_new(work^.alen);
				ctail^.link:=head;
				ctail:=head;
				move(work,head);
				head^.link:=nil;
				head^.ready:=0;
				local_edit:=false;
			end;
		end;
		clr_ready;
		re_calc;
	end;
   end;

9:
	begin
		upper;
		print_out;
	end;

10: begin
	upper;
	work:=ref(work_cell);
	for i:=1 to 70 do work_cell.form[i]:=buffer[i];
	work_cell.form[ip+1]:=chr(0);
	i:=1;
	if not get_range(work,i,r1,c1,r2,c2) then
	begin
		r2:=r1;
		c2:=c1;
	end;
	test_r:=0;
	test_c:=0;
	if (this<>nil) and (r1>=0) and (r1<=255) and (c1>=0) and
		(c1<=255) and (r2>=r1) and (c2>=c1) then
	begin
		for r:=r1 to r2 do
		for c:=c1 to c2 do
		begin
			head:=get_point(r,c);
			this:=get_point(cur_r+r-r1,cur_c+c-c1);
			if this<>head then
			begin
				if (this<>nil) then do_dispose(this);
				if head<>nil then
				begin
					move(head,work);
					work_cell.row:=cur_r+r-r1;
					work_cell.col:=cur_c+c-c1;
					local_edit:=true;
					del_row:=cur_r-r1;
					del_col:=cur_c-c1;
					error:=false;
					calc_error:=false;
					i:=1;
					work^.value:=formula(work,i);
					work^.value:=-0.000095623781;
					this:=get_new(work^.alen);
					ctail^.link:=this;
					ctail:=this;
					move(work,this);
					this^.link:=nil;
					this^.ready:=0;
					local_edit:=false;
				end;
			end;
		end;
		clr_ready;
		re_calc;
	end;
   end;

  end;
	ip:=0;
	clr_buf;
	prompt(cur_r,cur_c);
end;

procedure calculate_formula;
var
	point,head:pcell;
	i,j,k:integer;
	save_format,save_prec : 0..15;
	save_flag : char;
begin
	error:=false;
	calc_error:=false;
	if ip<70 then
	begin
		ip:=ip+1;
		buffer[ip]:=chr(0);
	end;
	j:=0;
	k:=ip;
	if buffer[1]='"' then
	begin
		j:=1;
		k:=k-1;
	end;
	save_format:=justify;
	save_prec:=precision;
	save_flag:=chr(0);
	point:=get_point(cur_r,cur_c);
	if point=nil then point:=get_new(k)
	else
	begin
		save_format:=point^.format;
		save_prec:=point^.prec;
		save_flag:=point^.flag;
		do_dispose(point);
		point:=get_new(k);
	end;
	for i:=1 to k do
		point^.form[i]:=buffer[i+j];
	if chead=nil then chead:=point;
	if ctail<>nil then ctail^.link:=point;
	ctail:=point;
	with point^ do
	begin
		link:=nil;
		row:=cur_r;
		col:=cur_c;
		flag:='A';
		ready:=2;
		value:=0;
		format:=save_format;
		prec:=save_prec;
		alen:=k;
		if buffer[1] in ['+','-','.','@','0'..'9'] then flag:='F';
		if flag='F' then
		begin
			for i:=1 to ip do
			if (form[i]>='a') and (form[i]<='z') then
			  form[i]:=chr(ord(form[i])-ord('a')+ord('A'));
			head:=chead;
			while (head<>nil) do
				begin
				   if head^.ready>1 then
					head^.value:=-0.0000980162302;
				   head^.ready:=0;
				   head:=head^.link;
				end;
			ready:=2;
			i:=1;
			value:=formula(point,i);
			if calc_error then ready:=2
			else ready:=1;
			if error then ready:=3;
			posit(cur_r,cur_c);
			dis_val(point,cur_c);
			re_calc;
		end
		else
		begin
			posit(cur_r,cur_c);
			dis_val(point,cur_c);
			if save_flag='F' then 
			begin
				clr_ready;
				ready:=2;
				re_calc;
			end;
		end;
	end;
	if point^.ready<>3 then
	case direction of
	up:	if cur_r>0 then if cur_r<=home_r then vertical(-1)
		else cur_r:=cur_r-1;
	down:	if cur_r<255 then if cur_r>=home_r+19 then vertical(1)
		else cur_r:=cur_r+1;
	left:	if cur_c>0 then if cur_c<=home_c then horizontal(-1)
		else cur_c:=cur_c-1;
	right:	if cur_c<255 then if on_screen(cur_r,cur_c+1) then
		cur_c:=cur_c+1
		else horizontal(1);
	end;
	prompt(cur_r,cur_c);
	state:=wait;
	ip:=0;
	clr_buf;
end;

procedure p_process_char(ch:char);

begin
	if ch=chr(27) then
	begin
	  if state=wait then process_cursor;
	end
	else
	if ch=chr(13) then
	  begin
		if state=command then state:=execute_command
		else if state=formulas then state:=execute_formula
		else if state=parameter then state:=execute_parameter;
	  end
	else
	if ch=chr(127) then
	  begin
		if state in [command,formulas,parameter] then
		 if ip>0 then
		   begin
		    ip:=ip-1;
		    write(chr(8),' ',chr(8));
		    if (ip=0) and (state<>parameter) then state:=wait;
		   end;
	   end
	else
	  begin
		if state in [wait,command,formulas,parameter] then
		begin
		  if ip<70 then
		    begin
			if (ip=0) and (state<>parameter) then
				if ch='/' then state:=command
					else state:=formulas;
			ip:=ip+1;
			if state=command then
			if (ch >= 'a') and (ch <= 'z') then
			ch:=chr(ord(ch)-ord('a')+ord('A'));
			buffer[ip]:=ch;
			write(ch);
		    end
		    else bell;
		end;
	   end;
end;			

procedure spread;
var
	valid_key : keystrokes;
	i : integer;
	ch : char;

begin
	direction:=down;
	valid_key:=[];
	for i:=32 to 127 do valid_key:=valid_key+[chr(i)];
	valid_key:=valid_key+[chr(13),chr(27)];
	state:=wait;
	done:=false;
	ip:=0;
	clr_buf;
	while not done do
	begin
	  get(inp);
	  ch:=inp^;
          if ch in valid_key then
	  begin
		p_process_char(ch);
		if state=execute_formula then calculate_formula;
		if state=execute_command then do_command;
		if state=execute_parameter then do_parameter;
	  end;
	end;
end;

(* main *)
begin
	reset(inp,'TI:/ODT');
	write(chr(29),'S',chr(29),'D',chr(13),chr(29),'M');
	for i:=0 to 255 do
	begin
	  width[i]:=10;
	end;
	chead:=nil;
	ctail:=nil;
	heading:='             Free Calc V1.0A            ';
	home_r:=0;
	home_c:=0;
	cur_r:=0;
	cur_c:=0;
	test_r:=0;
	test_c:=0;
	del_row:=0;
	del_col:=0;
	precision:=0;
	justify:=0;
	write(chr(27),'[2J');
	screen:=80;
	local_edit := false;
	display(0,0);
	spread;
end.
                                                                     