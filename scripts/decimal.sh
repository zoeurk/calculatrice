#!/bin/sh
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
if test -z "$3"
then
	printf "USAGE:\n"
	printf "$0 ENCODE|DECODE [VIRGULE] \"value\" base\n";
	exit
fi
if test -n "$4"
then VAL=$3
	TO=$4
	if printf " $2" | grep -e "[-\.]" >/dev/null
	then
		printf "Valeur invalide: $2\n"
		exit
	fi
	if printf " $4" | grep -e "[-\.]" >/dev/null
	then
		printf "Valeur invalide: $4\n"
		exit
	fi
else
	if printf " $3" | grep -e "[-\.]" >/dev/null
	then
		printf "Valeur invalide: $3\n"
		exit
	fi
	VAL=$2
	TO=$3
fi
#if test -n "$3"
#then
#	#$0 help
#	exit
#fi
#test -n "$3" && VAL=$3 || VAL=$2 
VAR=$VAL
VIRGULE=0
if printf " $VAL" | grep "\-" >/dev/null
then
	NEG=1
	VAL=`printf " $VAL" | sed 's/-//g'`
	VAR=$VAL
fi
case $1
in
ENCODE)
	if mcompare "\-N $VAL"
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
				VAL=`calcule -O 0 "mod($VAR,$TO)"`
				VALUE=`calcule "( $VAR-$VAL )"`
				test $TO -gt 10 -a $TO -le 16 && \
					VAL=`hexadecimal $VAL`
				test $TO -gt 16 && VAL=" "$VAL ||VAL=$VAL
				#echo $VAL
				#test $VAL -gt 10 && echo "GREAT"
				VAR=`calcule -O 0 "( $VALUE/$TO )"`
				RESULT=${VAL}${RESULT}
			else
				VIRGULE=$(($VIRGULE+1))
				VALUE=`calcule "$VAR*$TO"`
				ENTIER=`calcule -O 0 "floor($VALUE)"`
				VAR=`calcule "( $VALUE - $ENTIER )"`
				mcompare "$ENTIER > 10 && $ENTIER <= 16" &&\
					RESULT=${RESULT}`hexadecimal $ENTIER` ||\
					RESULT=${RESULT}$ENTIER
				LAST=$RESULT
				DOT=$(($DOT-1))
				#printf "$RESULT\n"
				if test -n "$4"
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
DECODE)
	I=0
	if ! printf "$VAR" | grep -e "[ \t\n]"
	then
		VAR=`printf "$VAL" | sed -e 's/\(.\)/\1 /g' -e 's/ *$//g'`
	fi
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
			#printf "$V;$I\n"
			test $TO -gt 10 -a $TO -le 16 && \
				VAL=`decimal $V` || \
				VAL=$V
			#VAL=$V
			if test $VAL = none
			then
				printf "Caractere invalide: $V\n"
				exit
			fi
			#echo $V
			test -n "$4" && \
				RESULT=`calcule -O $2 "$VAL * pow($TO,$I) + $RESULT"` || \
				RESULT=`calcule -O 6 " $VAL * pow($TO,$I) + $RESULT"`

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
	printf "$0 ENCODE|DECODE [VIRGULE] \"value\" base\n";
esac
