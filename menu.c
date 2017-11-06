#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "linktable.h"
#include <string.h>
#include<unistd.h>

tLinkTable * head = NULL;

int Help(int argc, char* argv[]);
int Quit(int argc, char* argv[])
    {
      //  getopt();

        exit(0);
    }
int Test(int argc, char* argv[])
{
    int ch;
　　opterr = 0;
　　while((ch = getopt(argc,argv,"abc"))!= -1)
　　{
　　switch(ch)
　　{
　　//case : printf("cmdtest");
　　case 'a': printf("option a:a\n"); break;
    case 'b': printf("option b:b\n"); break;
　//　case 'b': printf("option b :b\n"); break;
　　default: printf("other option :c");
　　}
　　//printf("optopt +%c\n",optopt);
　　}
　　return 0;
}

#define CMD_MAX_LEN 128
#define DESC_LEN    1024
#define CMD_NUM     10
#define CMD_MAX_ARV_NUM 10


/* data struct and its operations */

typedef struct DataNode
{
    tLinkTableNode * pNext;
    char*   cmd;
    char*   desc;
    int     (*handler)(int argc, char* argv[]);
} tDataNode;

int SearchCondition(tLinkTableNode * pLinkTableNode,void * args)
{
    char * cmd=(char *)args;
    tDataNode * pNode = (tDataNode *)pLinkTableNode;
   // printf("CMD:%s\n", cmd);
   // printf("搜索条件时的cmd%s\n", pNode->cmd);//一直为help；why？
    if(strcmp(pNode->cmd, cmd) == 0)
    {
      //  printf("搜索条件比较成功～");
        return  SUCCESS;  
    }
    return FAILURE;	       
}

/* find a cmd in the linklist and return the datanode pointer */
tDataNode* FindCmd(tLinkTable * head, char * cmd)
{
    return  (tDataNode*)SearchLinkTableNode(head,SearchCondition,(void *)cmd);
}

/* show all cmd in listlist */
int ShowAllCmd(tLinkTable * head)
{
    tDataNode * pNode = (tDataNode*)GetLinkTableHead(head);
    while(pNode != NULL)
    {
        printf("%s - %s\n", pNode->cmd, pNode->desc);
        pNode = (tDataNode*)GetNextLinkTableNode(head,(tLinkTableNode *)pNode);
    }
    return 0;
}

int InitMenuData(tLinkTable ** ppLinktable)
{
    *ppLinktable = CreateLinkTable();
    tDataNode* pNode = (tDataNode*)malloc(sizeof(tDataNode));
    pNode->cmd = "help";
    pNode->desc = "Menu List:";
    pNode->handler = Help;
    AddLinkTableNode(*ppLinktable,(tLinkTableNode *)pNode);
    pNode = (tDataNode*)malloc(sizeof(tDataNode));
    pNode->cmd = "version";
    pNode->desc = "Menu Program V1.0";
    pNode->handler = NULL; 
    AddLinkTableNode(*ppLinktable,(tLinkTableNode *)pNode);
    pNode = (tDataNode*)malloc(sizeof(tDataNode));
    pNode->cmd = "quit";
    pNode->desc = "Quit from Menu Program V1.0\n";
    pNode->handler = Quit; //
    AddLinkTableNode(*ppLinktable,(tLinkTableNode *)pNode);
 
    return 0; 
}

/* menu program */

//tLinkTable * head = NULL;

// int main()
// {
//     InitMenuData(&head); 
//    // MenuConfig(&head)
//    /* cmd line begins */
//     while(1)
//     {
//         char cmd[CMD_MAX_LEN];
//         printf("Input a cmd number > ");
//         scanf("%s", cmd);
//         tDataNode *p = FindCmd(head, cmd);
//         if( p == NULL)
//         {
//             printf("This is a wrong cmd!\n ");
//             continue;
//         }
//         printf("%s - %s\n", p->cmd, p->desc); 
//         if(p->handler != NULL) 
//         { 
//             p->handler();
//         }
   
//     }
//}

int main(int argc, char* argv[])
{
   
    MenuConfig("version", "show the version of the menu program.", NULL);
    MenuConfig("quit", "quit the menu system.", Quit);
    MenuConfig("test", "test the option arguments.",Test);
    ExecuteMenu();
    return 0;
}
int MenuConfig(char* cmd, char* desc, int (*handler)(int argc, char* argv[]))
{
    tDataNode* pNode = NULL;
    if(head == NULL){
        head = CreateLinkTable();
        pNode = (tDataNode*)malloc(sizeof(tDataNode));
        pNode->cmd = "help";
        pNode->desc = "Menu List:";
        pNode->handler = Help;
        AddLinkTableNode(head,(tLinkTableNode *)pNode);
    }
    pNode = (tDataNode*)malloc(sizeof(tDataNode));
    pNode->cmd = cmd;
    pNode->desc = desc;
    pNode->handler = handler;
    AddLinkTableNode(head, (tLinkTableNode *)pNode);
    return 0;
}
//ExecuteMenu取代原来的main函数；
int ExecuteMenu()
{
    while(1)
    {
        int argc = 0;
        char cmd[CMD_MAX_LEN];
        printf("input an cmd number>");
        //scanf("%s", cmd);
        char* argv[CMD_MAX_ARV_NUM];
        char* pcmd = NULL;
        pcmd = fgets(cmd,CMD_MAX_LEN,stdin);
        if(pcmd == NULL)
        {
            continue;
        }
        pcmd = strtok(pcmd, " ");
        while(pcmd !=NULL && argc < CMD_MAX_ARV_NUM)
        {
            argv[argc] = pcmd;
            argc++;
            pcmd = strtok(NULL," ");
        }
        if(argc == 1)
        {
            int len =strlen(argv[0]);
            *(argv[0]+len-1)='\0';
        }
        tDataNode* p=(tDataNode*)SearchLinkTableNode(head, SearchCondition, (void*) argv[0]);
        if(p == NULL)
        {
            printf("this is a wrong cmd\n");
            continue;
        }
        printf("%s,%s", p->cmd, p->desc);
        if(p->handler != NULL)
        {
            p->handler(argc, argv);
        }
    }
}
int Help(int argc, char* argv[])
{
    ShowAllCmd(head);
    return 0; 
}

// int Quit()
// {
//     exit(0);
// }
