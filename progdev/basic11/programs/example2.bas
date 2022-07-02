50 REM Program to round off decimal numbers
100 PRINT "What number do you wish to round off";
110 INPUT N
120 PRINT "To how many places";
130 INPUT Y
140 PRINT
150 LET A=INT(N*10^Y+0.5)/(10^Y)
160 PRINT N "=" A "to" Y "decimal places."
170 PRINT
180 GO TO 100
190 END
