# Organelle OS Program

Provides menu interface for operating the Organelle, interfaces with hardware, launches patches, provides settings for midi, wifi, etc...


SUBMODULI:
cd Organelle_OS
git submodule init
git submodule update

AGGIUNGI USER MUSIC:
ATTENZIONE: Su patchbox os sostituire "pi" in  usermod con "patch"

sudo useradd -m -d /home/music music
sudo usermod -a -G pi,adm,dialout,cdrom,sudo,audio,video,plugdev,games,users,input,netdev,spi,i2c,gpio,lpadmin music

come su (sudo su)
sudo echo "music ALL=(ALL) NOPASSWD: ALL">/etc/sudoers.d/010_music-nopasswd
exit

sudo nano /etc/lightdm/lightdm.conf
Scroll down near the bottom of the file and find the line "autologin-user=pi". It will be one of the
few lines that isn't commented out. Change that to the new users name.
4b) If you will be wanting an autologin for the console you'll need to edit the "autologin@.service" 
file, since "pi" is hard coded in that as well :
sudo nano /etc/systemd/system/autologin@.service
Near the top of the file you'll find "ExecStart=-/sbin/agetty --autologin pi --noclear %I $TERM"
Change "pi" to "music".
manca la password: sudo passwd music  ---> mettere come password "music"

AGGIUNGI LE LIBRERIE MANCANTI:
Installare l'ultima versione di pigpio:
 
    sudo apt install python3-distutils python-setuptools python3-setuptools
 
    cd pigpio-master
    make
    sudo make install
    If the Python part of the install fails it may be because you need the setup tools.
    sudo apt install python-setuptools python3-setuptools

e PuredData
sudo apt install pd liblo-tools liblo-dev python-liblo pyliblo-utils

Lanciare PureData e settare come home documenti 
/home/patch/Pd/externals
(ATTENZIONE che non ci sia "Documents")

logout / logon per far leggere i nuovi gruppi


