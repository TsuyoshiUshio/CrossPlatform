GCC=C:\Users\hirosho\Downloads\Edison\iss-iot-win_03-14-16\iss-iot-win\devkit-x86\sysroots\x86_64-pokysdk-mingw32\usr\bin\i586-poky-linux\i586-poky-linux-gcc.exe
INCLUDE="-IC:\Users\hirosho\Downloads\Edison\iss-iot-win_03-14-16\iss-iot-win\devkit-x86\sysroots\i586-poky-linux\usr\include\mraa"
SYSROOT="--sysroot=C:\\Users\\hirosho\\Downloads\\Edison\\iss-iot-win_03-14-16\\iss-iot-win\\devkit-x86\\sysroots\\i586-poky-linux"
TARGET=BlinkLEDSWEST
BlinkLEDSWEST: main.o
	$(GCC) $(SYSROOT) -lmraa -o $(TARGET) main.o

main.o: main.cpp
	$(GCC) $(INCLUDE) -O0 -g3 -Wall -c -fmessage-length=0 $(SYSROOT) -m32 -march=i586 -c -ffunction-sections -fdata-sections -o main.o main.cpp

clean:
	del BlinkLEDWEST main.o
