# Organelle OS Program

Provides menu interface for operating the Organelle, interfaces with hardware, launches patches, provides settings for midi, wifi, etc...

AGGIUNGI USER MUSIC:
sudo usermod music -a -G pi,adm,dialout,cdrom,sudo,audio,video,plugdev,games,users,input,netdev,spi,i2c,gpio
sudo echo "music ALL=(ALL) NOPASSWD: ALL">/etc/sudoers.d/010_music-nopasswd

sudo nano /etc/lightdm/lightdm.conf
Scroll down near the bottom of the file and find the line "autologin-user=pi". It will be one of the
few lines that isn't commented out. Change that to the new users name.
4b) If you will be wanting an autologin for the console you'll need to edit the "autologin@.service" 
file, since "pi" is hard coded in that as well.

sudo nano /etc/systemd/system/autologin@.service
Near the top of the file you'll find "ExecStart=-/sbin/agetty --autologin pi --noclear %I $TERM"
Change "pi" to the new user name.

AGGIUNGI LE LIBRERIE MANCANTI:
sudo apt install pd liblo-tools
creare user music assegnando i gruppi

sudo usermod -a -G adm,dialout,cdrom,audio,video,plugdev,games,users,input,netdev,spi,gpio,lpadmin music

logout / logon per far leggere i nuovi gruppi

C'è bisogno della libreria gpio:
    http://abyz.me.uk/rpi/pigpio/download.html
    Download and install latest version
    wget https://github.com/joan2937/pigpio/archive/master.zip
    unzip master.zip
    cd pigpio-master
    make
    sudo make install

    If the Python part of the install fails it may be because you need the setup tools.
    sudo apt install python-setuptools python3-setuptools

