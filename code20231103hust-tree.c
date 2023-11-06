/**
 * @file code20231103hust-tree.c
 * @author kjq (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-03
 * 
 * @copyright Copyright (c) 2023
 * bug总结：访问数组时要清楚有几个指针；循环访问数组时要注意指针的恢复；涉及算法要画流程图；int数组要先初始化为0再更新值；char数组要注意留出'\0'的位置
 * 
 * bug1: search_min_two()函数找最小的两个值时，算法错误
 * bug2: 更新huffmantree的后n-1个行时没有初始化为0，导致与.parent有关的判断失效
 * bug3: 每次循环完成code_buff_start"指针"位置的位置没有恢复，导致访问数组越界
 * bug4: 每次循环完成huffmantree的指针i没有恢复，导致不能进行下一次循环
 * bug5: huffmantree 的循环访问指针k,i,father中k(外层按序1-8访问）和i（内层按.parent访问）混用，导致不能进行下一次循环
 * bug6: 将编码复制到code_print时使用了"指针"i（内层按.parent访问），导致数组不能按顺序读出。（应该使用指针k(外层按序1-8访问））
 * bug7: 将编码复制到code_print时使用了"指针"i（内层按.parent访问），导致数组不能按顺序读出。（应该使用指针k(外层按序1-8访问））
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_PRINT 0
#define TREE_DEBUG_PRINT 0
#define MIN_DEBUG_PRINT 0
#define char_num 8


typedef struct 
{
    int weight;
    int parent,lchild,rchild;
}HTNode, *HuffmanTree;

void search_min_two(HuffmanTree huffmantree_ptr,int max_pos,int *code1,int *code2){
    *code1 = 0;
    *code2 = 0;
    for(int i = 1;i<= max_pos;i++){
        if(MIN_DEBUG_PRINT){
            printf("%d\n",max_pos);
        }
        if(huffmantree_ptr[i].parent != 0) continue;
        if(!*code1){*code1 = i;continue;}
        if(!*code2){*code2 = i;continue;}
        if(huffmantree_ptr[i].weight < huffmantree_ptr[*code1].weight){
            if(huffmantree_ptr[*code1].weight < huffmantree_ptr[*code2].weight){
                *code2 = i; //code2最大
            }else{
                *code1 = i;  //code1最大
            }
        }else if(huffmantree_ptr[i].weight < huffmantree_ptr[*code2].weight){
            *code2 = i; //code2最大
        }

        if(MIN_DEBUG_PRINT){
            printf("%d and %d\n",huffmantree_ptr[*code1].weight,huffmantree_ptr[*code2].weight);
        }
    }
}



void code_huffmantree(HuffmanTree huffmantree_ptr,int * weight,char code_print[char_num+1][char_num]){//函数输出的数组要么是全局数组的地址，要么就传参时传主函数数组的地址
    if(char_num <=1) return NULL;

    int node_num = 2*char_num - 1;
    huffmantree_ptr = (HuffmanTree)malloc((node_num+1) * sizeof(HTNode)); //0号单元未使用
    int i = 1;
    HuffmanTree p = huffmantree_ptr;

    for(p+=1,i = 1;i<=char_num;p++,i++,weight++){//0号单元未使用应该首先给p+1
        memset(p,0,sizeof(HTNode));
        memcpy(p,weight,sizeof(int));
    }

    if(TREE_DEBUG_PRINT){
        int j;
        for(j = 0;j<=node_num;j++){
            printf("%d:weight:parent:lchild:rchild=%4d:%4d:%4d:%4d\n",j,huffmantree_ptr[j].weight,huffmantree_ptr[j].parent,huffmantree_ptr[j].lchild,huffmantree_ptr[j].rchild);
        }
    }

    if(DEBUG_PRINT){
        printf("success init!\n");
    }

    int code1,code2;
    for(;i<=node_num;i++,p++){
        memset(p,0,sizeof(HTNode)); //置零防止判断失误
        search_min_two(huffmantree_ptr,i-1,&code1,&code2);
        p->weight = huffmantree_ptr[code1].weight + huffmantree_ptr[code2].weight;
        p->lchild = code1;
        p->rchild = code2;
        huffmantree_ptr[code1].parent = i;
        huffmantree_ptr[code2].parent = i;

        if(TREE_DEBUG_PRINT){
        int j;
        for(j = 0;j<=node_num;j++){
            printf("%d:weight:parent:lchild:rchild=%4d:%4d:%4d:%4d\n",j,huffmantree_ptr[j].weight,huffmantree_ptr[j].parent,huffmantree_ptr[j].lchild,huffmantree_ptr[j].rchild);
        }
    }
    }


    if(DEBUG_PRINT){
        printf("success erect tree!\n");
    }

    if(DEBUG_PRINT){
        int j;
        for(j = 0;j<=node_num;j++){
            printf("%d:weight:parent:lchild:rchild=%4d:%4d:%4d:%4d\n",j,huffmantree_ptr[j].weight,huffmantree_ptr[j].parent,huffmantree_ptr[j].lchild,huffmantree_ptr[j].rchild);
        }
    }

    char* code_buff = (char*)malloc(char_num*sizeof(char));
    int code_buff_start = char_num - 1;
    code_buff[code_buff_start] = '\0';
    int father,k;

    for(k = 1;k<=char_num;k++){
        code_buff_start = char_num - 1; //每次循环完成后需要恢复start的位置
        if(DEBUG_PRINT){
            printf("success encode step %d!\n",k);
        }
        i = k; 
        for(father = huffmantree_ptr[i].parent;father!=0;i = father,father = huffmantree_ptr[i].parent){
            if(huffmantree_ptr[father].lchild == i) code_buff[--code_buff_start] = '0';//极端情况下code_buff_start最终会到0
            if(huffmantree_ptr[father].rchild == i) code_buff[--code_buff_start] = '1';
            if(DEBUG_PRINT){
                printf("father = %d!\n",father);
            }
        }
        if(DEBUG_PRINT){
                printf("code length = %d!\n",char_num-code_buff_start);
            }

        strcpy((char*)code_print[k],&code_buff[code_buff_start]);
        //符号i和符号k混用，导致数组访问顺序未知
        if(DEBUG_PRINT){
            int j;
            for(j = 0;code_print[k][j]!='\0';j++){
                printf("%c",code_print[k][j]);
            }
            printf("\n");
        }
        if(DEBUG_PRINT){
            int j;
            for(j = code_buff_start;j<char_num;j++){
                printf("%c",code_buff[j]);
            }
            printf("\n");
        }
    }

    if(DEBUG_PRINT){
        printf("success encode!\n");
    }

}

int main(){
    HuffmanTree huffmantree_ptr;
    int weight[char_num] = {7,19,2,6,32,3,21,10};
    char code[char_num+1][char_num] = {"you and","faraway","police","youtube"};
    int i,j;

    for(i = 1;i<=char_num;i++){
        printf("%d:",i);
        for(j = 0;code[i][j] !='\0';j++)
        {printf("%c",code[i][j]);}
        printf("\n");
    }
    code_huffmantree(huffmantree_ptr,weight,code); 
    
    for(i = 1;i<=char_num;i++){
        printf("%d:",i);
        for(j = 0;code[i][j] !='\0';j++)
        {printf("%c",code[i][j]);}
        printf("\n");
    }
}