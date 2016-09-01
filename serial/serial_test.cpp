#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "serial_lib.h"
#include <iostream>


int main(int argc, char *argv[])
{
    // initialize serial
    Serial ser;
    ser.init("/dev/ttyUSB0", 115200);
    usleep(1000 * 1000);
    while(1){
        // read 
        char read_buf[255];
        ser.serial_read(read_buf);
        std::cout << "read_buf = " << read_buf << std::endl;

        // write
        char writestr[255] = "1,2,3,4,5,6*\n";
        ser.serial_write(writestr);

        // sleep
        usleep(1000);
    }
    return 0;
}
