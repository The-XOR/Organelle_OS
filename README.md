# Organelle OS Program

Provides menu interface for operating the Organelle, interfaces with hardware, launches patches, provides settings for midi, wifi, etc...

AGGIUNGI USER MUSIC:
sudo usermod newuser_name -a -G pi,adm,dialout,cdrom,sudo,audio,video,plugdev,games,users,input,netdev,spi,i2c,gpio
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
