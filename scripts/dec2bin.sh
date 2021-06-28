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

#VAL=$2;
VAR=$VAL
VIRGULE=0
LAST=0
if printf " $VAL" | grep "\-" >/dev/null
then
	NEG=1
	VAL=`printf " $VAL" | sed 's/-//g'`
	VAR=$VAL
fi
case $1
in
2BIN)
	if ntest "\-N $VAL"
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
				#printf "$I"
				I=$(($I+1))
			fi
		done
	}
	DOT=`dot | tail -n 1`
	for V in $ENTIER $SUB
	do
		VAR=$V
		while ntest "( $VAR != 0 )"
		do
			VALUE=$V
			if ntest "\-N $VAR"
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
				VIRGULE=$(($VIRGULE+1))
				if test -n "$3"
				then
					test $(($VIRGULE-1)) -eq $2 && break
				else	test $(($VIRGULE-1)) -eq 6 && break
				fi
				test $LAST = $R && break;
			fi
		done
		if ntest "( $SUB != 0 && $VIRGULE == 0 )"
		then
			test -z "$RESULT" && RESULT=0
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
2DEC)
	I=0
	VIRGULE=0
	VAR=`printf "$VAL" | sed -e 's/\(.\)/\1 /g' -e 's/\( \+\)/ /g' -e 's/ *$//g'`
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
		if test "$V" != "1" -a "$V" != "0" -a "$V" != "." -a "$V" != "-"
		then
			printf "Caractere invalid: $V\n"
			exit
		fi
		if test "$V" != "." -a "$V" != "-"
		then
			I=$(($I-1))
			test -n "$3" &&\
				RESULT=`calcule -O $2 "$V * pow(2,$I) + $RESULT"` || \
				RESULT=`calcule -O 6 "$V * pow(2,$I) + $RESULT"`
				
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
	printf "$0 2BIN|2DEC [VIRGULE] value\n";
esac
