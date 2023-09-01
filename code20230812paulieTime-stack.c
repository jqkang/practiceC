/*
功能：
用栈实现简单的算术表达式求解，栈用链表实现
函数功能：
stack_init：栈的初始化
stack_push：入栈
stack_pop：出栈
stack_show：打印栈的内容
op_cmp：比较两个运算符号的优先级
stack_cmp：将存放在运算符号的栈stackOp中优先级更高的运算符号弹出栈，并做运算存入存放数字和中间结果的栈stackNum
不足：表达式有很多限制，比如运算数需要为10以内整数，中间结果不能超过79
改进思路：增加一个函数，负责将字符串识别为整数；将栈的链表元素直接以int形式存放
stack_cmp的是用递归实现的，或许可以用循环实现:(一个伪代码)
while(stack_pop){
    while(stack_e>=str_e){
        switch(stack_e){};
        stack_push();
    }
    stack_push(stack_e);
} 
stack_push(str_e);
*/
#include<stdio.h>
#include<stdlib.h>
typedef struct SNode{
    char data;
    struct SNode* next;
}SNode,*LinkedStack;

typedef struct Stack{
    LinkedStack top;
    LinkedStack base;
}Stack;

void stack_init(Stack* pStack){
    (*pStack).top = (LinkedStack)malloc(sizeof(SNode));
    if(!(*pStack).top) exit(1);
    (*pStack).base = (*pStack).top; //空栈
}

int stack_push(Stack* pStack, char e){
    LinkedStack p;
    p = (LinkedStack)malloc(sizeof(SNode));
    p->data = e;
    if( (*pStack).base == (*pStack).top){
    (*pStack).top->next = p;
    p->next = NULL;
    (*pStack).base = p;
    }else{
        p->next = (*pStack).top->next;
        (*pStack).top->next = p;
    }
    return 1;
}

int stack_pop(Stack* pStack, char* pE){
    LinkedStack p = pStack->top->next;
    if(pStack->top == pStack->base){return 0;}
    else{
        *pE = p->data ;
        if(p->next == NULL){ //栈中只剩一个元素
            pStack->top->next == NULL;
            free(p);
            pStack->base = pStack->top;
        }else{
            pStack->top->next = p->next;
            free(p);
        }
    }
    return 1;
}

void stack_show(Stack stack){
    LinkedStack p = stack.top;
    if(stack.top == stack.base){
        printf("stack:NULL\n");
    }else{
        printf("stack:");
        while(p->next){
            p = p->next;
            printf("%4c",p->data);
        }
        printf("\n");
    }
}

int op_cmp(char str_e,char stack_e){
    switch(str_e){
        case '+':return 0;break;
        case '-':return 0;break;
        case '*':if(stack_e == '+' || stack_e == '-')return 1;return 0;break;
        case '/':if(stack_e == '+' || stack_e == '-')return 1;return 0;break;
        case '=':return 0;break;
        default: exit(2);break;
    }
}

void stack_cmp(Stack* stackNum,Stack* stackOp,char str_e){
    float num_rear,num_front;
    char char_rear,char_front;
    char stack_e;
    if(stack_pop(stackOp,&stack_e)){
                if(op_cmp(str_e,stack_e)){
                    stack_push(stackOp,stack_e);
                    stack_push(stackOp,str_e);
                }else{
                        stack_pop(stackNum,&char_rear);
                        stack_pop(stackNum,&char_front);
                        num_rear = char_rear - 48; //使用的是ascii码，因此能存储最大为127-48 = 79的数
                        num_front = char_front - 48;
                        //printf("num_rear:%lf\nnum_front:%lf\n",num_rear,num_front);
                    switch(stack_e){
                        case '+':stack_push(stackNum,num_front + num_rear+48);break; //float类型转换为char的ascii码
                        case '-':stack_push(stackNum,num_front - num_rear+48);break;
                        case '*':stack_push(stackNum,num_front * num_rear+48);break;
                        case '/':stack_push(stackNum,num_front / num_rear+48);break;
                        default : exit(1);break;
                    }
                        stack_cmp(stackNum,stackOp,str_e);
                }
            }else{
                stack_push(stackOp,str_e);
            }
}

int main(){
    Stack stackNum,stackOp;
    stack_init(&stackNum);
    stack_init(&stackOp);
    printf("\'\"\':%d\n\'9\':%d\n =:\n",'"','9','"'-'9');
    char str[] = "4+5*2*1+3/2-9="; //条件：运算数为10以内的整数，中间结果不能超过79，表达式不能带'('和')'，表达式的计算只包括加减乘除，所有中间值取整数后进行运算；
    char e;
    int i;
    for(i=0;str[i]!= '\0';i++){ //要求str最后一个符号是'='
        if(str[i]>=48 && str[i]<=57){
            stack_push(&stackNum,str[i]);
        }
        else{
            stack_cmp(&stackNum,&stackOp,str[i]);
        }
        printf("%d:\n",i);
        stack_show(stackNum);
        stack_show(stackOp);
    }

    printf("%s",str);
    if(stack_pop(&stackNum,&e)){
        printf("(%c:)",e);
        printf("%d",e-48); //任何中间结果和最终结果需要在79以内
    }else{exit(3);}
    
    return 0 ;

}

