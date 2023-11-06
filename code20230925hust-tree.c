#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>


/*链式存储的树的节点*/
typedef struct treenode{
    char ch;
    struct treenode* leftChild,*rightChild;
}treenode;

/*一个顺序存储的栈*/
#define MAX 10
treenode* str[MAX] = {0};
uint8_t i = 0; //str也是栈底，&str[i]是栈顶，初始化时栈空i=0

/**
 * @brief 建立树
 * 通过类似于先序的终端输入序列创建树
 * @return LinkedTree 树的根节点地址
 */
treenode* buildTree(){
    char c = getchar();
    if(c == '0'){return 0;}
    treenode* p_node = malloc(sizeof(treenode)*1);
    p_node->ch = c;
    p_node->leftChild = buildTree();
    p_node->rightChild = buildTree();
    return p_node;
}

/**
 * @brief 先序非递归遍历树
 * 入栈str[i++] = p_node;
 * 出栈p_node = str[--i];
 * @param p_node 要遍历的树的根节点地址
 */
void inOrderTraverse(treenode* p_node){
    while(p_node || i!=0){
        while(p_node){
            printf("%c",p_node->ch);
            str[i++] = p_node;
            p_node = p_node->leftChild;
        }
        p_node = str[--i];
        p_node = p_node->rightChild;
    }
}

/**
 * @brief 先序递归遍历树
 * 
 * @param p_node 要遍历的树的根节点地址
 */
void inOrderTraverse_(treenode* p_node){
        if(p_node == NULL){return;}
        printf("%c",p_node->ch);
        inOrderTraverse_(p_node->leftChild);
        inOrderTraverse_(p_node->rightChild);
} 



int main(){
    treenode* p_node = buildTree();
    inOrderTraverse_(p_node);
   

    /*操作一个顺序存储的栈*/
    /*treenode node;
    treenode* p_node = &node;
    str[i++]=p_node;
    printf("stack_init\nstr:%p,str[i]:%p;\n",str,&str[i]);
    printf("stack_push\nstr:%p,str[i]:%p;\n",str,&str[i]);
    treenode* p_node_temp = str[--i];
    printf("stack_pop:\nstr:%p,str[i]:%p;\n",str,&str[i]);
    if(i==0){
        printf("stack_empty:\nstr:%p,str[i]:%p;\n",str,&str[i]);
    }
    */

    return 0;
}