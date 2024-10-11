//CS22B1095 REDDIPALLI SAI CHARISH
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/ipc.h>
#include<stdlib.h>
# define MAX 20
struct msg
{
    long int msg_type;
    char msg_text[MAX];
};

int count_vowels(char *msg)
{
    int count=0;
    char vowels[]="AEIOUaeiou";
    for(int i=0;msg[i]!='\0';i++)
    {
        if (strchr(vowels, msg[i]) != NULL) {
            count++;
        }
    }
    return count;
}

int length(char *msg)
{
    int count=0;
    for(int i=0;msg[i]!='\0';i++)
    count++;
    return count;
    
}

int main()
{
    int msgid;
    char buffer[20];
    msgid=msgget((key_t)1095,0666 | IPC_CREAT);
    if(msgid==-1)
    {
        printf("error in creating");
        exit(0);
    }
    struct msg m;
    msgrcv(msgid, &m, MAX, 1, 0);
    printf("Receiver: Received message from Sender1: %s\n", m.msg_text);
    printf("Message Length: %d\n", length(m.msg_text));  
    printf("Number of vowels: %d\n\n", count_vowels(m.msg_text));
    msgrcv(msgid, &m, MAX, 1, 0);
    printf("Receiver: Received message from Sender2: %s\n", m.msg_text);
    printf("Message Length: %d\n", length(m.msg_text));  
    printf("Number of vowels: %d\n\n", count_vowels(m.msg_text));
    msgctl(msgid, IPC_RMID, NULL);
    
    return 0;
}
