#calculatrice  
petite caclulatrice en ligne de commande.  
Libre d'usage, de modification et de distribution  
Installation (si vous etes sous bsd il y a un patch:  
patch scripts/hex2bin.sh scripts/hex2bin_sh.bsd.patch  
aclocal  
autoreconf --install  
./configure --prefix=/usr --exec-prefix=/usr  
make  
make install
