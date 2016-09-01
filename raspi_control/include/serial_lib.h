#ifndef _MYSERIAL_
#define _MYSERIAL_
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

class Serial{
    public:
        char *_serial_port;
        char *_buf;
        speed_t _baudrate;
        int _buf_size;
        int _fd;


        Serial();
        // Serial(char *serial_port, int baudrate);
        ~Serial();
        // initialize
        void init(char *serial_port, int baudrate);
        // read from serial
        void serial_read(char *str);
        // write char to serial
        void serial_write(char *str);
        // set baudrate to _baudrate
        void set_baudrate(int baudrate);
        // change baudrate
        void change_baudrate(int baudrate);

};

#endif
