#!/bin/sh
VAL=$2;
VAR=$VAL
VIRGULE=0
LAST=0
case $1
in
TO_BIN)
	ENTIER=`calcule -O 0 "floor($VAL)"`
	SUB=`calcule -O 6 "( $VAR - $ENTIER )"`
	dot(){
		I=0
		printf "$VAL" | sed -e 's/\(.\)/\1\n/g' -e 's/\n$//' | \
		while read entry
		do
			if test $entry = "." -o $I -ne 0
			then
				printf "$I"
				I=$(($I+1))
			fi
		done
	}
	DOT=`dot | tail -n 1`
	for V in $ENTIER $SUB
	do
		VAR=$V
		while mcompare "( $VAR != 0 )"
		do
			VALUE=$V
			if mcompare "\-N $VAR"
			then
				printf "Caractere invalid dans: $VAR\n"
				exit
			fi
			if test $VIRGULE -eq 0
			then
				VAL=`calcule -O 0 "mod($VAR,2)"`
				VAR=`calcule -O 0 "($VAR - $VAL)"`
				VAR=`calcule -O 0 "( $VAR/2 )"`
				RESULT=${VAL}${RESULT}
			else
				VALUE=`calcule "$VAR*2"`
				ENTIER=`calcule -O 0 "floor($VALUE)"`
				VAR=`calcule "( $VALUE - $ENTIER )"`
				RESULT=${RESULT}${ENTIER}
				LAST=$RESULT
				R=`calcule "( $RESULT-0 )"`
				test $LAST = $R && break;
			fi
		done
		if mcompare "( $SUB != 0 )" && test $VIRGULE -eq 0
		then
			test -z "$RESULT" && RESULT=0
			VIRGULE=-1
			RESULT="${RESULT}."
		fi
	done
	printf "$RESULT\n"
;;
TO_DEC)
	I=0
	VIRGULE=0
	VAR=`printf "$2" | sed -e 's/\(.\)/\1 /g' -e 's/\( \+\)/ /g' -e 's/ *$//g'`
	RESULT=0
	for V in $VAR;
	do
		if test "$V" = "."
		then	VIRGULE=1
			MAX=$I
			I=0
		fi
		if test $VIRGULE -eq 0
		then
			I=$(($I+1))
			MAX=$I
		fi
	done
	I=$MAX
	for V in $VAR
	do
		if test "$V" != "1" -a "$V" != "0" -a "$V" != "."
		then
			printf "Caractere invalid: $V\n"
			exit
		fi
		if test "$V" != "."
		then
			I=$(($I-1))
			RESULT=`calcule "$V * pow(2,$I) + $RESULT"`
		fi
	done
	printf "$RESULT\n"
;;
*)
	printf "USAGE:\n"
	printf "$0 TO_BIN|TO_DEC value\n";
esac
