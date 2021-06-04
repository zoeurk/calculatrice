#!/bin/bash
VAL=$2;
VAR=$VAL
case $1
in
TO_BIN)
	while [ "$VAR" != "0" ]
	do
		if compare "-N $VAR"
		then
			printf "Caractere invalid dans: $VAR\n"
			exit
		fi
		VAL=`calcule -O 0 "mod($VAR,2)"`
		VAR=$(($VAR-$VAL))
		VAR=$(($VAR/2))
		RESULT=${VAL}${RESULT}
	done
	printf "$RESULT\n"
;;
TO_DEC)
	I=0
	VAR=`echo $2 | sed -e 's/\(.\)/\1 /g' -e 's/ *$//g'`
	for V in $VAR;
	do
		I=$(($I+1))
	done
	RESULT=0
	for V in $VAR
	do
		if ! test $V = 1 -a $V = 0
		then
			printf "Caractere invalid: $V\n"
			exit
		fi
		I=$(($I-1))
		RESULT=`calcule -O 0 "$V * pow(2,$I) + $RESULT"`
	done
	echo $RESULT
;;
*)
	printf "USAGE:\n"
	printf "$0 TO_BIN|TO_DEC value\n";
esac
