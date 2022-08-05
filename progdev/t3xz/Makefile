all: t.com t3x.com t3x.tsk

t.com: t.t
	zxcc ./bin/t3xz16 t

t.obj: t.com
	cp t.t t.t3x
	zxcc t t

t3x.tsk: t.obj
	(cd lib; make)
	cp lib/t3xz.lib .
	zxcc tkb -"t3x,trsx,trsx=t.obj/of:t,t3xz/lb/q/ext=25000/task=...t3x/ident=t3xz16/asg=ti:1,sy:2-6"

t3x.com: t.obj
	(cd lib; make)
	cp lib/t3xzcpm.lib .
	zxcc tkb -"t3x,tcpm,tcpm=t.obj/of:c,t3xzcpm/lb/q"

clean:
	(cd lib; make clean)
	rm -f t.com t3x.com t3x.tsk *.rel *.obj *.lib *.sym *.map *~
