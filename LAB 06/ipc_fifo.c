/*
Write two different programs (Sender and Receiver) in C to demonstrate IPC using FIFO Named Pipe. Process "Sender.c" will write one integer value (Say A) as a message in FIFO. "Receiver.c" will read that message. After reading the message, it will find the one’s complement of A and name it as B1. Then, it will write B1 on to the pipe. "Sender.c" will read the integer value B1 from the pipe and display it.

Write the program on your Lab UNIX system. After successful execution and demonstration to the TAs, copy your sender code into the sender.c file and your receiver code into the receiver.c file before the deadline.
*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int main() {
    int fd;
    char *myfifo = "/home/charish/os/p.txt";
    mkfifo(myfifo, 0666);
    unsigned int a, b1;
    fd = open(myfifo, O_WRONLY);
    printf("Enter a number: ");
    scanf("%d", &a);
    write(fd, &a, sizeof(a));
    printf("Sender: Sent number: %d\n", a);
    close(fd);
    fd = open(myfifo, O_RDONLY);
    read(fd, &b1, sizeof(b1));
    printf("Sender: Received one's complement: %d\n", b1);
    close(fd);
    return 0;
}
///=======================================================================================receiver code =====================================================================================///
dont copy both codes and try to exceute 
  //CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int main() {
    int fd;
    char *myfifo = "/home/charish/os/p.txt";
    mkfifo(myfifo, 0666);
    unsigned int a, b1;
    fd = open(myfifo, O_RDONLY);
    read(fd, &a, sizeof(a));
    printf("Receiver: Received number: %d\n", a);
    close(fd);
    b1 = (unsigned int)~a & 0xF;
    fd = open(myfifo, O_WRONLY);
    write(fd, &b1, sizeof(b1));
    printf("Receiver: Sent one's complement: %d\n", b1);
    close(fd);
    return 0;
}


// int main() {
//     int fd;
//     char *myfifo = "/home/charish/os/s.txt";
//     mkfifo(myfifo, 0666);
//     int a, b1;
//     fd = open(myfifo, O_RDONLY);
//     read(fd, &a, sizeof(a));
//     printf("Receiver: Received number: %d\n", a);
//     close(fd);
//     b1 = ~a;
//     fd = open(myfifo, O_WRONLY);
//     write(fd, &b1, sizeof(b1));
//     printf("Receiver: Sent one's complement: %d\n", b1);
//     close(fd);
//    
