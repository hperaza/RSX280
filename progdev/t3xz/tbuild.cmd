.enable substitution
.sets fname p1
.if fname = "" .asks fname Enter file name (no extension)
.ifnins ...t3x ins t3x/task=...t3x/inc=25000
t3x 'fname'
pip 'fname'.tsk/co/nv='fname'.tsk
pip 'fname'.tsk/pu
