.enable substitution
.sets fname p1
.if fname = "" .asks fname Enter file name (no extension)
.ifnins ...t3x ins t3x/task=...t3x/inc=25000
t3x 'fname'
tkb 'fname'='fname'/of:t,$t3xz/lb/task='fname'/ext=2048/asg=ti:1,sy:2-6
