/*
程序功能：模拟银行业务处理
函数功能：
    需求功能：
OpenForDay:银行系统初始化，将第一个顾客到达事件和离开事件插入事件表
minimum：找到四个队列中元素最少的队列，元素个数相同时，标号越小优先级更高
clientArrived：处理事件表中的到达事件，生成新的达到事件，若新到达事件所插入的队伍本来为空，同时生成一个离开事件
clientDeparture:处理事件表中的离开事件，并生成队伍中新的离开事件
    基本功能：
example：有序链表和4个队列的示例程序
eList_init：事件表初始化
que_init:队列初始化 
    bank_init()：银行系统初始化，包括事件表的初始化和队列的初始化
bank_show:将事件表和队列内容打印出来
que_enter:将客户从尾部插入队列
que_leave:将客户从队列头部取出，如果队列为空则返回0，反之返回1
event_enter:将事件按事件顺序插入事件表
event_leave:从事件表中取出头部的事件，如果事件表为空，则返回0，反之返回1
que_empty:查看队列是否为空，队列空则返回1，反之返回0
que_getHead:查看队列的第一个顾客
不足：
生成到达事件的条件是有一个顾客到达，且一次只会到达一个顾客，因此队列几乎不会有人排队；
若一次生成多个顾客到达事件，顾客数量将随关门时间的变大呈指数增长，因此程序有时无法正常结束，正常结束时得出的数据也不合理（银行结束时间33，顾客平均等待时间200左右）。
改进
...git 别人的库 cpp工具
结构体可以直接赋值，*q = event;


*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define closeTime 33
#define QFULL 3
#define EFULL 4
typedef struct ENode{  //事件表由顺序链实现
    int occurTime;
    int eType;
    int duration; 
    struct ENode* next;
}ENode,*LinkedEvent;

typedef struct QNode{
    int arrivalTime;
    int duration;
    struct QNode* next;
}QNode,*LinkedQue;

typedef struct Que{
    LinkedQue front;
    LinkedQue rear;
}Que;

ENode eList; //定义结构体（头节点），而不要定义结构体指针（头指针）
Que que[5]; //定义结构体（队列
QNode client; //定义节点
ENode event; 
int totalTime,clientNum;

void eList_init(LinkedEvent peList){
    peList = (LinkedEvent)malloc(sizeof(ENode));
    if(!peList) exit(1);
}

void que_init(Que* pque){
    pque->front = (LinkedQue)malloc(sizeof(QNode));
    if(!pque->front) exit(2);
    pque->front->next = NULL;
    pque->rear = pque->front; //空栈
}

void bank_init(){
    int i;
    eList_init(&eList);
    for(i=1;i<5;i++)que_init(&que[i]);
}

void que_enter(Que* pque,QNode client){
    LinkedQue p;
    p = (LinkedQue)malloc(sizeof(QNode));
    if(!p) exit(QFULL);
    p->arrivalTime = client.arrivalTime;
    p->duration = client.duration;
    p->next = NULL;
    pque->rear->next = p;
    pque->rear = p;
}

int que_leave(Que* pque,LinkedQue pclient){
    LinkedQue p;
    p = pque->front->next;
    if(!p) return 0; //队列空
    pque->front->next = p->next;
    if(p == pque->rear)pque->rear = pque->front; //队列只剩1个元素
    pclient->arrivalTime = p->arrivalTime;
    pclient->duration = p->duration;
    free(p);
    return 1;
}

void event_enter(LinkedEvent peList,ENode event){
    LinkedEvent p,q;
    q = (LinkedEvent)malloc(sizeof(ENode));
    if(!q) exit(EFULL);
    q->eType = event.eType; //结构体可以直接赋值，*q = event;
    q->occurTime = event.occurTime;
    q->duration = event.duration;
    p = peList;
    while(p->next){
        if(p->occurTime <= q->occurTime && p->next->occurTime > q->occurTime){
            break;
        }
        p = p->next;
    }
        q->next = p->next;
        p->next = q;
}

int event_leave(LinkedEvent peList,LinkedEvent pevent){
    LinkedEvent p;

    if(!peList->next) return 0; //顺序表空
    p = peList->next;
    peList->next = p->next;
    pevent->eType = p->eType;
    pevent->occurTime = p->occurTime;
    pevent->duration = p->duration;
    return 1;
}

int bank_show(){
    LinkedEvent p;
    LinkedQue q;
    int i;
    p = &eList;
    printf("**************EventList:***********\n");
    if(!p->next) printf("NULL\n"); 
    else{
        do{
        p = p->next;
        printf("eType:%4d|occurTime:%4d\n",p->eType,p->occurTime);
        }while(p->next);
    }
    


    for(i = 1;i<5;i++){
        q = que[i].front;
        printf("********Que%d:\n",i);
        if(que[i].front == que[i].rear)printf("NULL\n");
        else{
            do{
            q = q->next;
            printf("arrivalTime:%4d|duration:%4d\n",q->arrivalTime,q->duration);
            }while(q->next);
        }
    }
}

void example(){  //有序链表和4个队列的示例程序
    bank_init();
    // QNode client; //定义节点
    // ENode event; 
    client.arrivalTime = 1;
    client.duration = 10;
    event.eType = 1;
    event.occurTime =1;
    que_enter(&que[1],client);
    que_leave(&que[1],&client);
    event_enter(&eList,event);
    event_leave(&eList,&event);
    bank_show();
}

void OpenForDay(){
    totalTime = 0;
    clientNum = 0;
    event.occurTime = 0;
    event.eType = 0;
    event.duration =10;
    bank_init();
    event_enter(&eList,event);
}

int minimum(Que que_cmp[5]){
    int i,count[5] = {0};
    int flag = 0,min = 99;
    LinkedQue q;
    for(i = 1;i<5;i++){
        q = que[i].front;
        if(que[i].front == que[i].rear) count[i] = 0;
        else{
            do{
            q = q->next;
            count[i]++;
            }while(q->next);
        } 
    }

    for(i = 1;i<5;i++){
        if(min > count[i])min = count[i],flag = i;
    }

    return flag;
}

int que_empty(Que q){
    if(q.front == q.rear) return 1;
    return 0;
}



void clientArrived(){
    clientNum ++; 
    int i = minimum(que);
    int flag = que_empty(que[i]);
    client.arrivalTime = event.occurTime;
    client.duration = event.duration;
    que_enter(&que[i],client);

    if(flag){ //如果该队列没有客户，产生新加入客户的离开事件
    event.occurTime = event.occurTime+event.duration;
    event.eType = i;
    event.duration = 0;
    event_enter(&eList,event);
    }
    srand((unsigned)time(NULL));
    int client_arrive_num = (int)5*rand()/(RAND_MAX+1.0);
    for(i = 0;i<client_arrive_num;i++){
        int intertime = rand()%5; //产生一个新的到达时间
        //srand((unsigned)time(NULL)); //程序进行很快，重新生成的种子都一样，因此得到的随机数页一样
        int durtime = rand()%30;

        event.occurTime = event.occurTime+intertime;
        event.eType = 0;
        event.duration = durtime;
        if(event.occurTime<closeTime){ //小于银行关门时间则插入事件列表
            event_enter(&eList,event);
        }
    }

}

void que_getHead(Que que,LinkedQue pclient){
    LinkedQue p;
    p = que.front->next;
    if(!p) exit(0); //队列空
    pclient->arrivalTime = p->arrivalTime;
    pclient->duration = p->duration;
}

void clientDeparture(){
    int i;
    i = event.eType;
    que_leave(&que[i],&client);
    totalTime += event.occurTime - client.arrivalTime;
    if(!que_empty(que[i])){ //条件：队列非空
        que_getHead(que[i],&client);
        event.eType = i;
        event.occurTime = event.occurTime+client.duration;
        event_enter(&eList,event);
    }
}

int main(){
    //example();
    OpenForDay();
    bank_show();
    while(event_leave(&eList,&event)){
        if(event.eType == 0)
            clientArrived();
        else clientDeparture();
        bank_show();
    }
    printf("the Average Time is %f\n",(float)totalTime/clientNum);
    return 0 ;
}

