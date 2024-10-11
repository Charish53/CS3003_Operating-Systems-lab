/*Write three  different C programs—Sender1, Sender2, and Receiver—to demonstrate inter-process communication (IPC) using a message queue. The program Sender1 should write a message to the queue, while Receiver should read that message and display the result. Additionally, Sender2 should also write a message to the same queue, which the Receiver can read and display it. The receiver will compute and display both the length of the received message and the number of vowel characters contained in that message.

Note: Explore all possible cases of message passing by changing the parameters value of message type, and message flag.
*/
//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>
# define MAX 20
struct msg
{
    long int msg_type;
    char msg_text[MAX];
};
int main()
{
    int msgid;
    char buffer[20];
    msgid=msgget((key_t)1095,0666 | IPC_CREAT);
    if(msgid==-1)
    {
        printf("error\n");
        exit(0);
    }
    struct msg m;
    m.msg_type=1;
    printf("sender 1 is entering the message: \n");
    scanf("%s",buffer);
    strcpy(m.msg_text,buffer);
    if(msgsnd(msgid,&m,MAX,0)==-1)
    printf("message not sent");
    printf("sender 1 : Sending message to queue.\n");
    
    return 0;
}
