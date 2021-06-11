#!/bin/sh
if test -z "$3"
then
	printf "USAGE:\n"
	printf "$0 ENCODE|DECODE [VIRGULE] value base\n";
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
		printf "Valeur invalide: $2\n"
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
				#VAL=`hexadecimal $VAL`
				VAR=`calcule -O 0 "( $VALUE/$TO )"`
				RESULT=${VAL}${RESULT}
			else
				VIRGULE=$(($VIRGULE+1))
				VALUE=`calcule "$VAR*$TO"`
				ENTIER=`calcule -O 0 "floor($VALUE)"`
				VAR=`calcule "( $VALUE - $ENTIER )"`
				#RESULT=${RESULT}`hexadecimal $ENTIER`
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
	printf "$RESULT" | grep -e "^\." >/dev/null && RESULT=0${RESULT}
	test -n "$NEG" && printf "-"
	printf "$RESULT\n"
;;
DECODE)
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
			#printf "$V;$I\n"
			#VAL=`decimal $V`
			#if test $VAL = none
			#then
			#	printf "Caractere invalide: $V\n"
			#	exit
			#fi
			test -n "$4" && \
				RESULT=`calcule -O $2 "$V * pow($TO,$I) + $RESULT"` ||
				RESULT=`calcule -O 6 "$V * pow($TO,$I) + $RESULT"`

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
	printf "$0 ENCODE|DECODE [VIRGULE] base value\n";
esac
