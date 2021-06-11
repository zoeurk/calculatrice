#!/bin/sh
tobin(){
	case $1
	in
	0)
		printf "0000"
	;;
	1)
		printf "0001"
	;;
	2)
		printf "0010"
	;;
	3)
		printf "0011"
	;;
	4)
		printf "0100"
	;;
	5)
		printf "0101"
	;;
	6)
		printf "0110"
	;;
	7)
		printf "0111"
	;;
	8)
		printf "1000"
	;;
	9)
		printf "1001"
	;;
	A|a)
		printf "1010"
	;;
	B|b)
		printf "1011"
	;;
	C|c)
		printf "1100"
	;;
	D|d)
		printf "1101"
	;;
	E|e)
		printf "1110"
	;;
	F|f)
		printf "1111"
	;;
	*)
		printf "none"
	esac
}
tohex(){
	case $1
	in
	0000)
		printf "0"
	;;
	0001)
		printf "1"
	;;
	0010)
		printf "2"
	;;
	0011)
		printf "3"
	;;
	0100)
		printf "4"
	;;
	0101)
		printf "5"
	;;
	0110)
		printf "6"
	;;
	0111)
		printf "7"
	;;
	1000)
		printf "8"
	;;
	1001)
		printf "9"
	;;
	1010)
		printf "A"
	;;
	1011)
		printf "B"
	;;
	1100)
		printf "C"
	;;
	1101)
		printf "D"
	;;
	1110)
		printf "E"
	;;
	1111)
		printf "F"
	;;
	*)
		printf "none"
	esac
}
VAL="$2"
if printf " $VAL" | grep "\-" >/dev/null
then
	NEG=1
	VAL=`printf " $VAL" | sed 's/-//g'`
	VAR=$VAL
fi
case $1
in
HEX2BIN)
	TEST=$VAL
	VAL=$VAL
	VAL=`printf "$VAL" | sed -e 's/\(.\)/\1 /g' -e 's/ *$//g'`
	for T in $VAL
	do
		if test `tobin $T` = "none" -a $T != "."
		then
			printf "Caractere invalid dans: $TEST\n"
			exit
		fi
	done
	for I in $VAL
	do
		test $I != "." && RESULT=${RESULT}`tobin $I` || RESULT="${RESULT}."
	done
	RESULT=`printf "$RESULT" | sed s/^0*//`
	while test 1 -eq 1;
	do	printf "$RESULT" | grep "\." >/dev/null || break
		RESULT=`printf "$RESULT" | sed '/\./ s/[\.,0]$//'`
		R=`printf "$RESULT" | sed -n "/[0\.]$/p"`
		test -z "$R" && break
	done
	printf "$RESULT" | grep -e "^\." >/dev/null && RESULT=0${RESULT}
	test -n "$NEG" && printf "-"
	printf "$RESULT\n"

;;
BIN2HEX)
	VAL1="$VAL"
	VAL1=${VAL1%.*}
	LEN1=${#VAL1}
	LEN1=`calcule -O 0 "mod($LEN1,4)"`
	LEN1=$((4-$LEN1))
	VAL2="$VAL"
	if printf "$VAL2" | grep "\." >/dev/null;
	then
		VAL2=${VAL2#$VAL1.}
		LEN2=${#VAL2}
		LEN2=`calcule -O 0 "mod($LEN2,4)"`
		LEN2=$((4-LEN2))
	else
		VAL2=""
		LEN2=0
	fi
	TEST=`printf "$VAL" | sed 's/\(.\)/\1 /g'`
	for T in $TEST
	do
		if test $T != 1 -a $T != 0 -a $T != "."
		then
			printf "Caractere invalid dans: $VAL\n"
			exit
		fi
	done
	for I in $(seq 1 1 $LEN1)
	do	VAL1="0${VAL1}"
	done
	VAL1=`printf "$VAL1" | sed -e 's/\(\(.\)\{4\}\)/\1 /g'`
	for I in $VAL1
	do
		RESULT=${RESULT}`tohex $I`
	done
	if test -n "$VAL2"
	then
		RESULT="${RESULT}."
		for I in $(seq 1 1 $LEN2)
		do
			VAL2=${VAL2}0
		done
		VAL2=`printf "$VAL2" | sed -e 's/\(\(.\)\{4\}\)/\1 /g'`
		for I in $VAL2
		do
			RESULT=${RESULT}`tohex $I`
		done
	fi
	RESULT=`printf "$RESULT" | sed s/^0*//`
	while test 1 -eq 1;
	do	printf "$RESULT" | grep "\." >/dev/null || break
		RESULT=`printf "$RESULT" | sed '/\./ s/[\.,0]$//'`
		R=`printf "$RESULT" | sed -n "/[0\.]$/p"`
		test -z "$R" && break
	done
	printf "$RESULT" | grep -e "^\." >/dev/null && RESULT=0${RESULT}
	test -n "$NEG" && printf "-"
	printf "$RESULT\n"
;;
*)
	printf "USAGE:\n"
	printf "$0 HEX2BIN|BIN2HEX value\n";
;;
esac
