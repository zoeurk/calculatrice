#!/bin/bash
VAL=$2;
VAR=$VAL

decimal(){
	case $1
	in
	0)
		printf "0"
	;;
	1)
		printf "1"
	;;
	2)
		printf "2"
	;;
	3)
		printf "3"
	;;
	4)
		printf "4"
	;;
	5)
		printf "5"
	;;
	6)
		printf "6"
	;;
	7)
		printf "7"
	;;
	8)
		printf "8"
	;;
	9)
		printf "9"
	;;
	A|a)
		printf "10"
	;;
	B|b)
		printf "11"
	;;
	C|c)
		printf "12"
	;;
	D|d)
		printf "13"
	;;
	E|e)
		printf "14"
	;;
	F|f)
		printf "15"
	;;
	*)
		printf "none"
		exit
	esac
}
hexadecimal(){
	case $1
	in
	0)
		printf "0"
	;;
	1)
		printf "1"
	;;
	2)
		printf "2"
	;;
	3)
		printf "3"
	;;
	4)
		printf "4"
	;;
	5)
		printf "5"
	;;
	6)
		printf "6"
	;;
	7)
		printf "7"
	;;
	8)
		printf "8"
	;;
	9)
		printf "9"
	;;
	10)
		printf "A"
	;;
	11)
		printf "B"
	;;
	12)
		printf "C"
	;;
	13)
		printf "D"
	;;
	14)
		printf "E"
	;;
	15)
		printf "F"
	;;
	*)
		printf "none"
	esac
}

case $1
in
TO_HEX)
	while [ "$VAR" != "0" ]
	do
		if printf $VAR | grep '\(\.\|,\)'>/dev/null
		then
			printf "virgule detectee\n"
			exit
		fi
		VALUE=$VAR
		if compare "\-N $VAR"
		then
			printf "Caractere invalid dans: $VAR\n"
			exit
		fi
		VAL=`calcule -O 0 "mod($VAR,16)"`
		VALUE=`calcule "( $VAR-$VAL )"`
		VAL=`hexadecimal $VAL`
		VAR=`calcule -O 0 "( $VALUE/16 )"`
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
		I=$(($I-1))
		VAL=`decimal $V`
		if test $VAL = none
		then
			printf "Caractere invalide: $V\n"
			exit
		fi
		RESULT=`calcule -O 0 "$VAL * pow(16,$I) + $RESULT"`
	done
	echo $RESULT
;;
*)
	printf "USAGE:\n"
	printf "$0 TO_HEX|TO_DEC value\n";
esac
