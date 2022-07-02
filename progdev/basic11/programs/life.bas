1 REM GAME OF LIFE
2 REM MODIFIED FOR VT52/VT100 BY BOB SUPNIK
3 REM MODIFIED TO PROMPT FOR MAX GENERATIONS AND TOTAL MAX POPULATION BY DON RUBIN
5 E$=CHR$(27%) \ X2%=23% \ Y2%=80% \ P%=0% \ G=1 \ I9%=0%
10 FOR X%=1% TO X2% \ FOR Y%=1% TO Y2% \ A%(X%,Y%)=0% \ NEXT Y% \ NEXT X%
20 DIM A%(23%,80%),B$(23%)
23 PRINT CHR$(27%);"[?2l";
25 PRINT E$;"H";E$;"J";"Enter maximum number of generations: "; \ INPUT Z
30 C%=1%
35 PRINT "Enter your pattern:"
40 LINPUT B$(C%) \ L%=LEN(B$(C%)) \ IF L%>77% THEN L%=77%
50 IF L%=0% THEN  GO TO 80
55 B$(C%)=SEG$(B$(C%),1,L%)
60 C%=C%+1
70 IF C%<X2% THEN  GO TO 40
80 IF C%=1% THEN  GO TO 1
85 L%=0%
90 FOR X%=1% TO C%-1%
100 IF LEN(B$(X%))>L% THEN L%=LEN(B$(X%))
110 NEXT X%
120 X1%=11%-C%/2%
130 Y1%=38%-L%/2%
140 FOR X%=1% TO C%
150 FOR Y%=1% TO LEN(B$(X%))
160 IF SEG$(B$(X%),Y%,Y%)<>" " THEN A%(X%+X1%,Y%+Y1%)=1% \ P%=P%+1%
170 NEXT Y%
180 NEXT X%
190 X2%=X1%+C% \ PRINT E$;"H";E$;"J";
200 IF P%<>0% THEN  GO TO 210
205 PRINT  \ PRINT E$;"H";E$;"J";"Generation:";G;"  Population is extinct";
209 D$="T" \ GO TO 650
210 IF G>Z THEN 650
211 PRINT  \ PRINT E$;"H";"Generation:";G;"  Population:";P%;E$;"K";
213 IF P%>Q% THEN Q%=P%
215 IF I9%<>0% THEN PRINT "  Bounded";
218 X3%=23% \ Y3%=80% \ X4%=1% \ Y4%=1%
220 G=G+1
230 FOR X%=X1% TO X2%
240 PRINT  \ PRINT E$;"Y";CHR$(32%+X%);CHR$(31%+Y1%);
250 FOR Y%=Y1% TO Y2%
253 IF A%(X%,Y%)=2% THEN A%(X%,Y%)=0%
256 IF A%(X%,Y%)=3% THEN A%(X%,Y%)=1%
260 IF A%(X%,Y%)<>1% THEN PRINT " "; \ GO TO 270
261 PRINT "*";
262 IF X%<X3% THEN X3%=X%
264 IF X%>X4% THEN X4%=X%
266 IF Y%<Y3% THEN Y3%=Y%
268 IF Y%>Y4% THEN Y4%=Y%
270 NEXT Y%
290 NEXT X%
295 PRINT E$;"J";E$;"H"
299 X1%=X3% \ X2%=X4% \ Y1%=Y3% \ Y2%=Y4% \ I9%=0%
301 IF X1%<3% THEN X1%=3% \ I9%=-1%
303 IF X2%>21% THEN X2%=21% \ I9%=-1%
305 IF Y1%<3% THEN Y1%=3% \ I9%=-1%
307 IF Y2%>78% THEN Y2%=78% \ I9%=-1%
309 P%=0%
500 FOR X%=X1%-1% TO X2%+1%
510 FOR Y%=Y1%-1% TO Y2%+1%
520 C%=0%
530 FOR I%=X%-1% TO X%+1%
540 FOR J%=Y%-1% TO Y%+1%
550 IF A%(I%,J%)=1% THEN C%=C%+1%
555 IF A%(I%,J%)=2% THEN C%=C%+1%
560 NEXT J%
570 NEXT I%
580 IF A%(X%,Y%)=0% THEN  GO TO 610
582 P%=P%+1%
584 IF C%=3% THEN  GO TO 620
586 IF C%=4% THEN  GO TO 620
588 P%=P%-1%
590 A%(X%,Y%)=2%
600 GO TO 620
610 IF C%=3% THEN A%(X%,Y%)=3% \ P%=P%+1%
620 NEXT Y%
630 NEXT X%
635 X1%=X1%-1% \ Y1%=Y1%-1% \ X2%=X2%+1% \ Y2%=Y2%+1%
640 GO TO 200
650 IF D$="T" GOTO 683
670 PRINT \ PRINT "Do you want to continue";
680 LINPUT A$ \ IF A$="Y" GOTO 690
683 PRINT \ PRINT "Maximum population was ";Q%,CHR$(27%);"<"
685 STOP
690 PRINT E$;"H";E$;"J";"Enter maximum number of generations: "; \ INPUT Z
700 GOTO 200
711 END
