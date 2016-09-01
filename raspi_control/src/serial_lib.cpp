#include "serial_lib.h"

Serial::Serial(){
    _buf_size = 255;
    _serial_port = "/dev/ttyUSB0";
    _buf = new char[_buf_size];
    set_baudrate(9600);
}

Serial::~Serial(){
    close(_fd);
    delete _buf;
}

void Serial::init(char *serial_port, int baudrate){
    _serial_port = serial_port;
    set_baudrate(baudrate);
    if(_fd){close(_fd);}
    _fd = open(_serial_port, O_RDWR);
    if(_fd == -1) {
        printf( "failed to open port\n" );
    }
    struct termios oldtio, newtio;
    ioctl(_fd, TCGETS, &oldtio); 
    newtio = oldtio;
    newtio.c_lflag = ICANON; 
    cfsetispeed(&newtio, _baudrate);
    cfsetospeed(&newtio, _baudrate);
    ioctl(_fd, TCSETS, &newtio);
}

void Serial::serial_read(char *str){
    memset(_buf, 0, _buf_size);
    int count = read(_fd, _buf, _buf_size);  
    if (count < 0) {  
        printf("Could not read from serial port\n");
    } 
    strcpy(str, _buf);
}

void Serial::serial_write(char *str){
    int inputcount = 0;
    for (int i = 0; i < 255; i++){  
        if (str[i] == '\n') {  
            inputcount = i;  
            str[i] = 13;  
            // str[i+1] = 10;  
            break;  
        }  
    }  
    write(_fd, str, inputcount + 1);
}


void Serial::set_baudrate(int baudrate){
    switch(baudrate){
        case 4800:
            _baudrate = B4800;
            break;
        case 9600:
            _baudrate = B9600;
            break;
        case 19200:
            _baudrate = B19200;
            break;
        case 38400:
            _baudrate = B38400;
            break;
        case 57600:
            _baudrate = B57600;
            break;
        case 115200:
            _baudrate = B115200;
            break;
        case 230400:
            _baudrate = B230400;
            break;
        default:
            printf("wrong baudrate!\n");
            break;
    }
}

void Serial::change_baudrate(int baudrate){
    init(_serial_port, baudrate);
    printf("change baudrate to %d\n", baudrate);
}

