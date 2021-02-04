# Organelle OS Program

Provides menu interface for operating the Organelle, interfaces with hardware, launches patches, provides settings for midi, wifi, etc...

PER QUESTA DISTRO UTILIZZIAMO L'UTENTE DI BIFOLC "patch"
***************IMPORTANTERRIMO: per il deployment, sudo make install (E NON deploy!!!!!!!!!)

SUBMODULI:
cd Organelle_OS
git submodule init
git submodule update

AGGIUNGI LE LIBRERIE MANCANTI:
Installare l'ultima versione di pigpio:

    sudo apt install python3-distutils python-setuptools python3-setuptools

    cd pigpio-master
    make
    sudo make install

e PuredData
sudo apt install pd liblo-tools liblo-dev python-liblo pyliblo-utils

Lanciare PureData e settare come home documenti 
/home/patch/Pd/externals
(ATTENZIONE che non ci sia "Documents")

