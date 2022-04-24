#!/bin/sh
if test -n "$3"
then VAL=$3 
	if printf " $2" | grep -e "[-\.]" >/dev/null
	then
		printf "Valeur invalide: $2\n"
		exit
	fi
else
	VAL=$2 
fi

#test -n "$3" && VAL=$3 || VAL=$2 
VAR=$VAL
VIRGULE=0
if printf " $VAL" | grep "\-" >/dev/null
then
	NEG=1
	VAL=`printf " $VAL" | sed 's/-//g'`
	VAR=$VAL
fi

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
2HEX)
	if ! mcompare "\-n $VAL"
	then printf "Bad value:'$VAL'\n"
		exit
	fi
	ENTIER=`calcule -O 0 "floor($VAL)"`
	SUB=`calcule -O 6 "( $VAR - $ENTIER )"`
	dot(){
		I=0
		printf " $VAL" | sed -e 's/\(.\)/\1\n/g' -e 's/\n$//' | \
		while read entry
		do
			if test "$entry" = "." -o $I -ne 0
			then
				I=$(($I+1))
			fi
		done
	}
	DOT=`dot | tail -n 1`
	for V in $ENTIER $SUB
	do
		VAR=$V
		while mcompare "( $VAR != 0)"
		do
			VALUE=$V
			if mcompare "\-N $VAR"
			then
				printf "Caractere invalid dans: $VAR\n"
				exit
			fi
			if test $VIRGULE -eq 0
			then
				VAL=`calcule -O 0 "mod($VAR,16)"`
				VALUE=`calcule "( $VAR-$VAL )"`
				VAL=`hexadecimal $VAL`
				VAR=`calcule -O 0 "( $VALUE/16 )"`
				RESULT=${VAL}${RESULT}
			else
				VIRGULE=$(($VIRGULE+1))
				VALUE=`calcule "$VAR*16"`
				ENTIER=`calcule -O 0 "floor($VALUE)"`
				VAR=`calcule "( $VALUE - $ENTIER )"`
				RESULT=${RESULT}`hexadecimal $ENTIER`
				LAST=$RESULT
				DOT=$(($DOT-1))
				#printf "$RESULT\n"
				if test -n "$3"
				then
					test $DOT -lt -$(($2-1)) && break
				else 	test $DOT -lt -6 && break
				fi
			fi
		done
		if mcompare "$VIRGULE == 0 && 0 < $SUB"
		then
			VIRGULE=1
			RESULT="${RESULT}."
		fi
	done
	printf "$RESULT" | grep -e "^\." >/dev/null && RESULT=0${RESULT}
	test -n "$NEG" && printf "-"
	printf "$RESULT\n"
;;
2DEC)
	I=0
	VAR=`printf "$VAL" | sed -e 's/\(.\)/\1 /g' -e 's/ *$//g'`
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
		fi
	done
	RESULT=0
	test $VIRGULE -eq 1 && I=$MAX
	for V in $VAR
	do	if test "$V" != "."
		then
			I=$(($I-1))
			VAL=`decimal $V`
			if test $VAL = none
			then
				printf "Caractere invalide: $V\n"
				exit
			fi
			test -n "$3" && \
				RESULT=`calcule -O $2 "$VAL * pow(16,$I) + $RESULT"` ||
				RESULT=`calcule -O 6 "$VAL * pow(16,$I) + $RESULT"`

		fi
	done
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
	printf "$0 2HEX|2DEC [VIRGULE] value\n";
esac
