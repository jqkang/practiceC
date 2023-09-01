#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BASE 100
#define INCREMENT 10
typedef struct {
    char *base;
    int num;
}String;

int find_index(String str,String mode){
    int i = 0;
    int j = -1;
    //求回溯函数next[j]
    int next[mode.num];
    next[0] = -1;
    while(i<mode.num-1){
        if(j == -1 || mode.base[i] == mode.base[j]){++i;++j;next[i] = j;}
        else j = next[j];
    }
    //KMP 模式匹配
    i = -1,j = 0;
    while(i < str.num && j< mode.num){
        if(j == -1 || str.base[i] == mode.base[j]) {++i;++j;}
        else j = next[j];
    }
    if(j>=mode.num) return (i-mode.num);
    else return 0;
}


int main(){
    int i,pos_sub;
    char str_src_1[] = "Hello string!";
    char str_src_2[] = "Find the sub string, please!";
    String str,mode;
    char str_sub[] = "please";
    char *p;
//定义:用堆实现字符串
    str.base = (char*)malloc(sizeof(char)*BASE);
    str.num = BASE;

//字符串链接
    strcat(str_src_1,str_src_2);

//字符串赋值
    strcpy(str.base,str_src_1);
    str.num = strlen(str.base);             //注意：strlen只查找到'\0'

//查找子串 strstr()函数
    p = strstr(str.base,str_sub); 
    if(p){
        printf("strstr find content:\n");
        for(i = 0;i<strlen(str_sub);i++)
        printf("%c",*(p+i)); 
        printf("\n");
    }else{
        printf("not found!\n");
    }
//查找子串 KMP 
    strcpy(mode.base,str_sub);
    mode.num = strlen(mode.base);
    printf("mode string: %s\n",mode.base);
    printf("mode string length: %d\n",mode.num);
    pos_sub = find_index(str,mode);
    if(pos_sub){
        printf("mode string position:%d\nfind_index find content:\n",pos_sub);
        for(i=0;i<mode.num;i++)
        printf("%c",*(str.base+pos_sub+i));
    } else {
        printf("not found!\n");
    }
    return 0;
}