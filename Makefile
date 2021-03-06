CXX = g++

CXXFLAGS += -std=c++11

objects =  \
	main.o \
	AppData.o \
	Timer.o \
	MainMenu.o \
	OledScreen.o \
	SLIPEncodedSerial.o \
	Serial.o Socket.o \
	UdpSocket.o \
	OSC/OSCData.o \
	OSC/OSCMatch.o \
	OSC/OSCMessage.o \
	OSC/OSCTiming.o \
	OSC/SimpleWriter.o 

# default : CXXFLAGS += -DCM3GPIO_HW -DMICSEL_SWITCH -DPWR_SWITCH -DOLED_30FPS -DBATTERY_METER -DFIX_ABL_LINK
default : CXXFLAGS += -DCM3GPIO_HW  -DOLED_30FPS -DBATTERY_METER -DFIX_ABL_LINK
default : $(objects) hw_interfaces/CM3GPIO.o hw_interfaces/button.o hw_interfaces/encoder.o hw_interfaces/led.o hw_interfaces/dispdrv.o
	g++ -l pigpio -l rt -o fw_dir/mother $(objects) hw_interfaces/CM3GPIO.o hw_interfaces/button.o hw_interfaces/encoder.o hw_interfaces/led.o hw_interfaces/dispdrv.o

.PHONY : clean

clean :
	rm main $(objects)

IMAGE_BUILD_VERSION = $(shell cat fw_dir/version)
IMAGE_BUILD_TAG = $(shell cat fw_dir/buildtag)
IMAGE_VERSION = $(IMAGE_BUILD_VERSION)$(IMAGE_BUILD_TAG)
IMAGE_DIR = UpdateOS-$(IMAGE_VERSION)


deploy : default
	@echo "Updating OS to $(IMAGE_VERSION)"
	@echo "copying common fw files"
	rm -fr /home/patch/fw_dir
	mkdir /home/patch/fw_dir
	cp -fr fw_dir/* /home/patch/fw_dir
	@echo "copying platform fw files"
	cp -fr platforms/organelle_m/fw_dir/* /home/patch/fw_dir
	chown -R music:music /home/patch/fw_dir
	@echo "copying version file to root for backwards compatiblility"
	cp -fr fw_dir/version /root
	@echo "aggiorno folder home/music"
	cp -fr platforms/organelle_m/home/* /home/patch/
	cp -ar platforms/organelle_m/home/. /home/patch/
	@echo "copying systems files"
	@echo "Creazione dei servizi:"
	@echo "  - cherrypy.service"
	@echo "  - createap.service"
	mkdir tmp
	cp -r platforms/organelle_m/rootfs tmp/
	chown -R root:root tmp/rootfs
	cp -fr --preserve=mode,ownership tmp/rootfs/* /
	rm -fr tmp
	rm -rf /usbdrive
	mkdir /usbdrive
	chown music:music /usbdrive
	@echo "  - copying patches"
	mkdir /usbdrive/Patches
	cp -r Organelle_Patches/* /usbdrive/Patches/
	cp -r platforms/organelle_m/Patches/* /usbdrive/Patches/	
	@echo "  - copying pd-extended"
	mkdir /usbdrive/pd-extended
	cp -r Organelle_pd_extra/* /usbdrive/pd-extended/
	@echo "  - cleanup"
	rm -rf /home/patch/Videos
	rm -rf /home/patch/Music
	rm -rf /home/patch/Pictures
	rm -rf /home/patch/Templates
	rm -rf /home/patch/Documents
	rm -rf /home/patch/Downloads
	rm -rf /home/patch/Public
	sync

install : default
	@echo "Updating OS to $(IMAGE_VERSION)"
	@echo "*** SPECIAL PATCHBOX OS INSTALLESCION ****"
	@echo "copying common fw files"
	rm -fr /home/patch/fw_dir
	mkdir /home/patch/fw_dir
	cp -fr fw_dir/* /home/patch/fw_dir
	@echo "copying platform fw files"
	cp -fr platforms/organelle_m/fw_dir/* /home/patch/fw_dir
	chown -R patch:patch /home/patch/fw_dir
	@echo "copying version file to root for backwards compatiblility"
	cp -fr fw_dir/version /root
	@echo "aggiorno folder home/patch"
	cp -fr platforms/organelle_m/home/* /home/patch/
	cp -ar platforms/organelle_m/home/. /home/patch/
	@echo "copying systems files"
	@echo "Creazione dei servizi:"
	@echo "  - cherrypy.service"
	@echo "  - createap.service"
	mkdir tmp
	cp -r platforms/organelle_m/rootfs tmp/
	chown -R root:root tmp/rootfs
	cp -fr --preserve=mode,ownership tmp/rootfs/* /
	rm -fr tmp
	rm -rf /usbdrive
	mkdir /usbdrive
	chown patch:patch /usbdrive
	@echo "  - copying patches"
	mkdir /usbdrive/Patches
	cp -r Organelle_Patches/* /usbdrive/Patches/
	cp -r platforms/organelle_m/Patches/* /usbdrive/Patches/	
	@echo "  - copying pd-extended"
	mkdir /usbdrive/pd-extended
	cp -r Organelle_pd_extra/* /usbdrive/pd-extended/
	sync

# Generate with g++ -MM *.c* OSC/*.* 
AppData.o: AppData.cpp AppData.h OledScreen.h
MainMenu.o: MainMenu.cpp MainMenu.h AppData.h OledScreen.h
OledScreen.o: OledScreen.cpp OledScreen.h fonts.h simple_svg_1.0.0.hpp
SLIPEncodedSerial.o: SLIPEncodedSerial.cpp SLIPEncodedSerial.h Serial.h \
  UdpSocket.h Socket.h
Serial.o: Serial.cpp Serial.h
Socket.o: Socket.cpp Socket.h
Timer.o: Timer.cpp Timer.h
UdpSocket.o: UdpSocket.cpp UdpSocket.h Socket.h
main.o: main.cpp OSC/OSCMessage.h OSC/OSCData.h OSC/OSCTiming.h \
  OSC/SimpleWriter.h Serial.h UdpSocket.h Socket.h SLIPEncodedSerial.h \
  OledScreen.h MainMenu.h AppData.h Timer.h
serialdump.o: serialdump.c
test.o: test.cpp
OSCData.o: OSC/OSCData.cpp OSC/OSCData.h OSC/OSCTiming.h
OSCMatch.o: OSC/OSCMatch.c OSC/OSCMatch.h
OSCMessage.o: OSC/OSCMessage.cpp OSC/OSCMessage.h OSC/OSCData.h \
  OSC/OSCTiming.h OSC/SimpleWriter.h OSC/OSCMatch.h
OSCTiming.o: OSC/OSCTiming.cpp OSC/OSCTiming.h
SimpleWriter.o: OSC/SimpleWriter.cpp OSC/SimpleWriter.h
SerialMCU.o: hw_interfaces/SerialMCU.cpp hw_interfaces/SerialMCU.h \
 hw_interfaces/../OledScreen.h
