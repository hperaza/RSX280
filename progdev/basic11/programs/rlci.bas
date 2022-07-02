10 REM Linear circuit analysys program, based on the one published
20 REM by A. Dolgij in the Radio Journal, No. 2 and 3 of 1989.
30 REM Y1 contains the conductance
40 REM Y2 contains the reluctance
50 REM Y3 contains the elastance
60 REM T1 contains the controlled current sources
70 REM Y4 and Y5 contains the complex admittance, real and imaginary parts
80 REM A1 and A2 contains the complex node voltages
90 DIM Y1(10,10),Y2(10,10),Y3(10,10),T1(10,10)
100 DIM Y4(10,11),Y5(10,11),A1(10),A2(10)
110 PRINT 
120 PRINT " *** AC circuit analisys program ***"
130 PRINT 
140 PRINT "Max number of nodes is 10"
150 PRINT "Input file"; \ INPUT F1$
160 OPEN F1$ FOR INPUT AS FILE #1 \ L1=0
170 PRINT 
180 PRINT "**************************************************"
190 PRINT 
200 LINPUT #1,L$ \ L1=L1+1
210 PRINT USING "###.";L1; \ PRINT "  ";L$
220 P1=POS(L$,"N=",1)
230 IF P1=0 THEN M$="Bad start" \ GOSUB 1930 \ U=0
240 U=VAL(SEG$(L$,P1+2,LEN(L$)))
250 IF U<2 THEN 270 \ IF U>10 THEN 270
260 GO TO 280
270 M$="Too many nodes" \ GOSUB 1930 \ U=10
280 LINPUT #1,L$ \ L1=L1+1
290 PRINT USING "###.";L1; \ PRINT "  ";L$
300 GOSUB 1480 \ E$=SEG$(I$,1,1)
310 IF E$="R" THEN  GOSUB 1580 \ GOSUB 1700 \ GO TO 280
320 IF E$="L" THEN  GOSUB 1580 \ GOSUB 1750 \ GO TO 280
330 IF E$="C" THEN  GOSUB 1580 \ GOSUB 1800 \ GO TO 280
340 IF E$="I" THEN  GOSUB 1850 \ GO TO 280
350 IF E$="K" GO TO 380
360 IF E$="Z" GO TO 380
370 M$="Unrecognized element" \ GOSUB 1930 \ GO TO 280
380 PRINT 
390 PRINT "**************************************************"
400 PRINT 
410 IF E9=0 THEN PRINT " No "; \ GO TO 430
420 PRINT E9;
430 PRINT "Errors detected." \ PRINT  \ PRINT 
440 GOSUB 1480 \ F0=VAL(I$) \ REM starting frequency
450 GOSUB 1480 \ D1=VAL(I$) \ REM frequency step
460 GOSUB 1480 \ P2=VAL(I$) \ REM number of points
470 GOSUB 1660 \ I0=P \ I1=M \ REM input voltage nodes
480 IF E$="Z" GO TO 540
490 GOSUB 1660 \ O0=P \ O1=M \ REM output voltage nodes
500 PRINT "   Freq.","Amplification factor","  Phase"
510 PRINT "   (Hz)","  (times)      (dB)","(degrees)"
520 PRINT 
530 GO TO 580
540 PRINT ,"   Input impedance"
550 PRINT "   Freq."," Modulus","  Phase"
560 PRINT "   (Hz)"," (Ohms)","(degrees)"
570 PRINT 
580 IF P2<1 THEN P2=1
590 FOR I2=1 TO P2
600 F=F0+D1*(I2-1)
610 IF F=0 THEN F=1.00000E-30
620 GOSUB 740
630 IF E$="K" THEN  GOSUB 1230
640 IF E$="Z" THEN  GOSUB 1400
650 NEXT I2
660 PRINT  \ PRINT "End." \ PRINT 
670 STOP
680 REM --- Compute angle. C = cosine, S = sine
690 IF C=0 THEN A=90*SGN(S) \ RETURN
700 A=180/PI*ATN(S/C)
710 IF C<0 THEN A=A+180
720 IF A>180 THEN A=A-360
730 RETURN
740 REM --- Perform analysis for frequency F
750 W=2*PI*F
760 FOR I=1 TO U
770 Y4(I,U+1)=0 \ Y5(I,U+1)=0
780 IF I=I0 THEN Y4(I,U+1)=1
790 IF I=I1 THEN Y4(I,U+1)=-1
800 FOR J=1 TO U
810 Y4(I,J)=Y1(I,J)+T1(I,J)
820 Y5(I,J)=Y3(I,J)*W-Y2(I,J)/W
830 NEXT J
840 NEXT I
850 N=0
860 N=N+1 \ K=N
870 IF Y4(K,N)=0 THEN IF Y5(K,N)=0 THEN K=K+1 \ GO TO 870
880 IF K=N GO TO 940
890 J=U+1
900 FOR M=N TO J
910 T2=Y4(N,M) \ Y4(N,M)=Y4(K,M) \ Y4(K,M)=T2
920 T3=Y5(N,M) \ Y5(N,M)=Y5(K,M) \ Y5(K,M)=T3
930 NEXT M
940 FOR J=U+1 TO N STEP -1
950 IF ABS(Y4(N,N))>ABS(Y5(N,N)) GO TO 990
960 R=Y4(N,N)/Y5(N,N) \ D2=Y5(N,N)+R*Y4(N,N)
970 T2=(Y4(N,J)*R+Y5(N,J))/D2 \ T3=(Y5(N,J)*R-Y4(N,J))/D2
980 GO TO 1010
990 R=Y5(N,N)/Y4(N,N) \ D2=Y4(N,N)+R*Y5(N,N)
1000 T2=(Y4(N,J)+Y5(N,J)*R)/D2 \ T3=(Y5(N,J)-Y4(N,J)*R)/D2
1010 Y4(N,J)=T2 \ Y5(N,J)=T3
1020 NEXT J
1030 M=U+1
1040 FOR I=K+1 TO U
1050 IF N+1=M GO TO 1110
1060 FOR J=N+1 TO M
1070 T2=Y4(I,J)-Y4(I,N)*Y4(N,J)+Y5(I,N)*Y5(N,J)
1080 Y5(I,J)=Y5(I,J)-Y5(I,N)*Y4(N,J)-Y4(I,N)*Y5(N,J)
1090 Y4(I,J)=T2
1100 NEXT J
1110 NEXT I
1120 IF N<>U GO TO 860
1130 FOR I=U TO 1 STEP -1
1140 A1(I)=Y4(I,M)
1150 A2(I)=Y5(I,M)
1160 FOR K=I-1 TO 1 STEP -1
1170 T2=Y4(K,M)-Y4(K,I)*A1(I)+Y5(K,I)*A2(I)
1180 Y5(K,M)=Y5(K,M)-Y4(K,I)*A2(I)-Y5(K,I)*A1(I)
1190 Y4(K,M)=T2
1200 NEXT K
1210 NEXT I
1220 RETURN
1230 REM --- Compute amplification factor
1240 U0=A1(O0)-A1(O1) \ U1=A2(O0)-A2(O1)
1250 V0=A1(I0)-A1(I1) \ V1=A2(I0)-A2(I1)
1260 IF ABS(V0)>ABS(V1) GO TO 1310
1270 R=V0/V1 \ D2=V1+R*V0
1280 W0=(U0*R+U1)/D2
1290 W1=(U1*R-U0)/D2
1300 GO TO 1340
1310 R=V1/V0 \ D2=V0+R*V1
1320 W0=(U0+U1*R)/D2
1330 W1=(U1-U0*R)/D2
1340 K=SQR(W0*W0+W1*W1)
1350 K0=INT(2000*LOG10(K+1.00000E-30)+.5)/100
1360 C=W0 \ S=W1 \ GOSUB 680
1370 P0=INT(A*10+.5)/10
1380 PRINT F,K,K0,P0
1390 RETURN
1400 REM --- Compute impedance
1410 W0=A1(I0)-A1(I1)
1420 W1=A2(I0)-A2(I1)
1430 Z=SQR(W0*W0+W1*W1)
1440 C=W0 \ S=W1 \ GOSUB 680
1450 P0=INT(A*10+.5)/10
1460 PRINT F,Z,P0
1470 RETURN
1480 REM --- Get next token
1490 P1=POS(L$," ",1) \ IF P1=0 THEN P1=254
1500 I$=SEG$(L$,1,P1) \ L$=SEG$(L$,P1+1,LEN(L$))
1510 RETURN
1520 REM --- Get node number
1530 GOSUB 1480
1540 I=VAL(I$)
1550 IF I>=0 THEN IF I<=U THEN RETURN
1560 M$="Out of bounds" \ GOSUB 1930 \ I=0
1570 RETURN
1580 REM --- Get passive element
1590 GOSUB 1520 \ E1=I \ REM first node
1600 GOSUB 1520 \ E2=I \ REM second node
1610 GOSUB 1480 \ Z1=VAL(I$) \ REM impedance
1620 IF Z1<>0 THEN RETURN
1630 IF E$<>"R" THEN IF E$<>"L" THEN RETURN
1640 M$=E$+"=0 not allowed" \ GOSUB 1930 \ Z1=1
1650 RETURN
1660 REM --- Get + and - nodes
1670 GOSUB 1480 \ P=VAL(I$)
1680 GOSUB 1480 \ M=VAL(I$)
1690 RETURN
1700 REM --- Enter conductance
1710 Y=1/Z1 \ REM Resistance is in ohms
1720 Y1(E1,E2)=Y1(E1,E2)-Y \ Y1(E2,E1)=Y1(E2,E1)-Y
1730 Y1(E1,E1)=Y1(E1,E1)+Y \ Y1(E2,E2)=Y1(E2,E2)+Y
1740 RETURN
1750 REM --- Enter reluctance
1760 Y=1/Z1*1.00000E+06 \ REM Inductance is in uH
1770 Y2(E1,E2)=Y2(E1,E2)-Y \ Y2(E2,E1)=Y2(E2,E1)-Y
1780 Y2(E1,E1)=Y2(E1,E1)+Y \ Y2(E2,E2)=Y2(E2,E2)+Y
1790 RETURN
1800 REM --- Enter elastance
1810 Y=Z1*1.00000E-12 \ REM Capacitance is in pF
1820 Y3(E1,E2)=Y3(E1,E2)-Y \ Y3(E2,E1)=Y3(E2,E1)-Y
1830 Y3(E1,E1)=Y3(E1,E1)+Y \ Y3(E2,E2)=Y3(E2,E2)+Y
1840 RETURN
1850 REM --- Enter controlled current source
1860 GOSUB 1480 \ S2=VAL(I$) \ REM Source node
1870 GOSUB 1480 \ D2=VAL(I$) \ REM Drain node
1880 GOSUB 1660 \ C0=P \ C1=M \ REM Controlling nodes
1890 GOSUB 1480 \ Y=VAL(I$) \ REM Slope
1900 T1(D2,C0)=T1(D2,C0)-Y \ T1(S2,C1)=T1(S2,C1)-Y
1910 T1(D2,C1)=T1(D2,C1)+Y \ T1(S2,C0)=T1(S2,C0)+Y
1920 RETURN
1930 REM --- Display error message
1940 PRINT "*** ";M$
1950 E9=E9+1
1960 RETURN
1970 END
