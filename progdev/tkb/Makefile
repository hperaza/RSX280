# Path to Linux utilities
ZXCC   = zxcc
VOL180 = ../../Tools/linux/vol180/vol180

# Path to CP/M utilities
ZSM4   = ../../Tools/cpm/zsm4.com
DRLINK = ../../Tools/cpm/drlink.com
TKB    = ../../Tools/cpm/tkb.com

.PREFIX:
.PREFIX: .mac .rel

SRCS = link.mac \
	linkcpm.mac \
	linkrsx.mac

OBJS = $(SRCS:.mac=.rel)

all: tkb.com tkb.tsk

$(OBJS): %.rel: %.mac *.inc
	$(ZXCC) $(ZSM4) -"=$</l/s8"

tkb.com: $(OBJS)
	$(ZXCC) $(DRLINK) -"tkb=link,linkcpm"
	cp tkb.sym tkbcpm.sym

tkb.tsk: $(OBJS) syslib.lib
	$(ZXCC) $(TKB) -"tkb,tkbrsx,tkb=link,linkrsx,syslib/lb/ofmt=tsk/task=...TKB/ext=20000/asg=TI:5,SY:1-4:6-14"

test: tkb.com
	@cp tkb.com tk.com
	$(ZXCC) tk -"tkb1=link,linkcpm"
	@rm tk.com
	comp tkb.com tkb1.com > comp_tkb_tkb1
	ls -l comp_tkb_tkb1

check: tkb.com
	@cp tkb.com tk.com
	$(ZXCC) $(DRLINK) -"a0=link,linkcpm"
	$(ZXCC) tk -"a1,a1,a1=link,linkcpm"
	$(ZXCC) tk -"a2,a2,a2=link,linkcpm/xm"
	comp a0.com a1.com > comp_a0_a1
	comp a0.com a2.com > comp_a0_a2
	comp a0.sym a1.sym
	$(ZXCC) $(DRLINK) -"ab0[b]=link,linkcpm"
	$(ZXCC) tk -"ab1/ofmt=spr:b,ab1,ab1=link,linkcpm"
	$(ZXCC) tk -"ab2/ofmt=spr:b,ab2,ab2=link,linkcpm/xm"
	comp ab0.spr ab1.spr > comp_ab0_ab1
	comp ab0.spr ab2.spr > comp_ab0_ab2
	comp ab0.sym ab1.sym
	$(ZXCC) $(DRLINK) -"ap0[op]=link,linkcpm"
	$(ZXCC) tk -"ap1/ofmt=prl,ap1,ap1=link,linkcpm"
	$(ZXCC) tk -"ap2/ofmt=prl,ap2,ap2=link,linkcpm/xm"
	comp ap0.prl ap1.prl > comp_ap0_ap1
	comp ap0.prl ap2.prl > comp_ap0_ap2
	$(ZXCC) $(DRLINK) -"ar0[or]=link,linkcpm"
	$(ZXCC) tk -"ar1/ofmt=rsp,ar1,ar1=link,linkcpm"
	$(ZXCC) tk -"ar2/ofmt=rsp,ar2,ar2=link,linkcpm/xm"
	comp ar0.rsp ar1.rsp > comp_ar0_ar1
	comp ar0.rsp ar2.rsp > comp_ar0_ar2
	$(ZXCC) $(DRLINK) -"as0[os]=link,linkcpm"
	$(ZXCC) tk -"as1/ofmt=spr,as1,as1=link,linkcpm"
	$(ZXCC) tk -"as2/ofmt=spr,as2,as2=link,linkcpm/xm"
	comp as0.spr as1.spr > comp_as0_as1
	comp as0.spr as2.spr > comp_as0_as2
	$(ZXCC) $(DRLINK) -"root0=link(ov0=linkcpm)"
	$(ZXCC) tk -"root1,root1,root1=link(ov1=linkcpm)"
	$(ZXCC) tk -"root2/xm,root2,root2=link(ov2=linkcpm)"
	comp root0.com root1.com > comp_root0_root1
	comp root0.com root2.com > comp_root0_root2
	comp root0.sym root1.sym
	comp ov0.ovl ov1.ovl > comp_ov0_ov1
	comp ov0.ovl ov2.ovl > comp_ov0_ov2
	comp ov0.sym ov1.sym
	$(ZXCC) $(DRLINK) -"t0=link,linkrsx,syslib.lib[s]"
	$(ZXCC) tk -"t1,t1,t1=link,linkrsx,syslib.lib/lb"
	$(ZXCC) tk -"t2,t2,t2=link,linkrsx,syslib.lib/lb/xm"
	comp t0.com t1.com > comp_t0_t1
	comp t0.com t2.com > comp_t0_t2
	$(ZXCC) $(DRLINK) -"ti0=link,linkrsx,syslib.irl[s]"
	$(ZXCC) tk -"ti1,ti1,ti1=link,linkrsx,syslib.irl/lb"
	$(ZXCC) tk -"ti2,ti2,ti2=link,linkrsx,syslib.irl/lb/xm"
	comp ti0.com ti1.com > comp_ti0_ti1
	comp ti0.com ti2.com > comp_ti0_ti2
	comp t0.com ti0.com > comp_t0_ti0
	$(ZXCC) m80 =comntest
	$(ZXCC) $(DRLINK) -"cc0=comntest"
	$(ZXCC) tk -"cc1,cc1,cc1=comntest"
	$(ZXCC) tk -"cc2,cc2,cc2=comntest/xm"
	comp cc0.com cc1.com > comp_cc0_cc1
	comp cc0.com cc2.com > comp_cc0_cc2
	ls -l comp*
#	$(ZXCC) $(DRLINK) -"ax0[ox]=link,linkcpm"
#	$(ZXCC) tk -"ax1/ofmt=rsx,ax1,ax1=link,linkcpm"
#	$(ZXCC) tk -"ax2/ofmt=rsx,ax2,ax2=link,linkcpm/xm"
#	comp ax0.rsx ax1.rsx > comp ax0_ax1
#	comp ax0.rsx ax2.rsx > comp ax0_ax2

clchk: clean_check
clean_check:
	rm -f a0.com a1.com a2.com a0.sym a1.sym a2.sym a0.map a1.map a2.map comp_a0_a1 comp_a0_a2
	rm -f ab0.spr ab1.spr ab2.spr ab0.sym ab1.sym ab2.sym ab0.map ab1.map ab2.map comp_ab0_ab1 comp_ab0_ab2
	rm -f ap0.prl ap1.prl ap2.prl ap0.sym ap1.sym ap2.sym ap0.map ap1.map ap2.map comp_ap0_ap1 comp_ap0_ap2
	rm -f ar0.rsp ar1.rsp ar2.rsp ar0.sym ar1.sym ar2.sym ar0.map ar1.map ar2.map comp_ar0_ar1 comp_ar0_ar2
	rm -f as0.spr as1.spr as2.spr as0.sym as1.sym as2.sym as0.map as1.map as2.map comp_as0_as1 comp_as0_as2
	rm -f root0.com root1.com root2.com root0.sym root1.sym root2.sym root0.map root1.map root2.map comp_root0_root1 comp_root0_root2
	rm -f ov0.ovl ov1.ovl ov2.ovl ov0.sym ov1.sym ov2.sym ov0.map ov1.map ov2.map comp_ov0_ov1 comp_ov0_ov2
	rm -f t0.com t1.com t2.com t0.sym t1.sym t2.sym t0.map t1.map t2.map comp_t0_t1 comp_t0_t2
	rm -f ti0.com ti1.com ti2.com ti0.sym ti1.sym ti2.sym ti0.map ti1.map ti2.map comp_ti0_ti1 comp_ti0_ti2 comp_t0_ti0 
	rm -f cc0.com cc1.com cc2.com cc0.sym cc1.sym cc2.sym cc0.map cc1.map cc2.map comp_cc0_cc1 comp_cc0_cc2
#	rm -f ax0.rsx ax1.rsx ax2.rsx ax0.sym ax1.sym ax2.sym ax0.map ax1.map ax2.map comp_ax0_ax1 comp_ax0_ax2
	rm -f tk.com

clean:
	rm -f tkb.com tkb.tsk *.rel *.sym *.prn *.map *.crf core *~ *.\$$\$$\$$

copy: tkb.tsk
	@echo "cd system" > cptkb.cmd
	@echo "delete tkb.tsk" >> cptkb.cmd
	@echo "import tkb.tsk tkb.tsk /c" >> cptkb.cmd
	@echo "bye" >> cptkb.cmd
	$(VOL180) /dev/fd0 < cptkb.cmd
	@rm cptkb.cmd
