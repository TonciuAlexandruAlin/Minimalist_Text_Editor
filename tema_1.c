#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    char data;
    struct node* next;
    struct node* prev;
}node_t;

typedef struct {
    int coloana;
    int linie;
}pozitie;

typedef struct cell{
    char *elem;
    struct cell* next;
}Stack;

Stack* initStack(Stack* s)
{
    s = NULL;
    return s;
}

Stack* push(Stack* s, char *g)
{
    Stack* nou;
    nou = (Stack*)malloc(sizeof(Stack));
    nou->elem = (char*)malloc((strlen(g)+1)*sizeof(char));
    if (nou == NULL)
    {
        return NULL;
    }
    strcpy(nou->elem,g);
    if (s == NULL)
    {
        nou->next = NULL;
        s = nou;
    }
    else
    {
        nou->next = s;
        s = nou;
    }
    return nou;
}

Stack* pop(Stack* s)
{
    char *g;
    Stack* aux;
    aux = s;
    if (s == NULL)
    {
        return 0;
    }
    s = aux->next;
    free(aux->elem);
    free(aux);
    return s;
}

int isEmptyStack(Stack* s)
{
    return (s == NULL);
}

void push_back(node_t** head, char c)
{
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    node_t* current = *head;
    new_node->data = c;
    new_node->next = NULL;

    if (*head == NULL) 
    {
        new_node->prev = NULL;
        *head = new_node;
        return;
    }
 
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
    new_node->prev = current;
    current = new_node;
    return;
}

void deleteNode();

void save(node_t* list)
{   
    FILE *fp;
    fp = fopen ("editor.out", "w");
    while (list)
    {
        fprintf(fp,"%c",list->data);
        list = list->next;
    }
    fclose(fp);
}

void freeList(node_t* list)
{   
    node_t* aux;
    while(list)
    {   
        aux = list;
        list = list->next;
        free(aux);
    }
}

node_t* go_to_line (pozitie a, node_t* list)
{
    pozitie b;
    b.coloana = 1;
    b.linie = 1;
    while(list->next && (a.coloana != b.coloana || a.linie != b.linie))
    {   
        if (list->data == '\n')
        {
            b.linie++;
            b.coloana = 1;
        }
        else
        {
            b.coloana++;
        }
        list = list->next;
    }
    return list;
}

void backspace(node_t** list, pozitie a)
{
    node_t* b = go_to_line(a,*list);
    node_t* aux = b;
    deleteNode(list,aux);
}

void delete_char(node_t** list, int cat, pozitie a)
{
    int d = cat;
    while(d != 0)
    {
        backspace(&(*list)->next,a);
        d--;
    }
}

void deleteNode(node_t** head,node_t* del)
{
    node_t* aux = del;
    if (*head == NULL || aux == NULL)
    {
        return;
    }
    if (*head == aux)
    {
        *head = aux->next;
    }
    if (aux->next != NULL)
    {
        aux->next->prev = aux->prev;
    }
    if (aux->prev != NULL)
    {    
        aux->prev->next = aux->next;    
    }
    free(aux);
    return;
}
void delete_line (node_t* list, pozitie a)
{   
    node_t* b =go_to_line(a,list);
    node_t* aux = b;
    while (aux->data != '\n')
    {   
        b = b->next;
        deleteNode(&list,aux);
        aux = b;
    }
    deleteNode(&list,b);
}


int main(){

    pozitie a;
    a.coloana = 0;
    a.linie = -1;
    node_t *head = NULL;
    FILE *fp;
    char c,d,s[100];
    fp = fopen ("editor.in", "r");
    if  (fp == NULL ) {
        printf ("Nu se poate deschide la citire fişierul!\n");
        exit (1);
    }

    while ((c = fgetc(fp)) != EOF )
    {   
        if (c != ':')
        {
            push_back(&head,c);
            a.coloana++;
            if (c == '\n')
            {
                a.linie++;
                a.coloana = 0;
            }
        }
        if (c == ':')
        {
            d = fgetc(fp);
            if (d == ':') //mod insert ::i
            {
                fgetc(fp);
                fgetc(fp);
                fgets(s,100,fp);
                while(strncmp(s,"::i",3) != 0 && strncmp(s,"q",1) != 0)
                {
                    if (strncmp(s,"s",1) == 0)
                        save(head);
                    else if (strncmp(s,"gl",2) == 0)
                    {
                        char* token = strtok(s," ");
                        char* token2 = strtok(NULL, " \n");
                        if (token2 != NULL)
                        {
                            a.linie = atoi(token2);
                            a.coloana = 1;
                        }
                        //go_to_line(a, head);
                    }
                    else if (strncmp(s,"dl",2) == 0)
                    {
                        char* token = strtok(s," ");
                        while (token != NULL)
                        {
                            token = strtok(NULL," \n");
                        }
                        pozitie m;
                        if(token == NULL)
                        {
                            m.linie = a.linie;
                            m.coloana = 1;
                            delete_line(head,m);
                        }
                        else
                        {
                            m.coloana = 1;
                            m.linie = atoi(token);
                            delete_line(head,m);
                        }
                    }
                    else if (strncmp(s,"gc",2) == 0)
                    {
                        char* token = strtok(s," ");
                        char* token1 = strtok(NULL," ");
                        char* token2 = strtok(NULL," \n");             
                        if (token1 != NULL)
                        {
                            a.coloana = atoi(token1);
                        }
                        // if (token2 == NULL)
                        // {
                        //     a.linie = a.linie;
                        // }
                        if (token2 != NULL)
                        {
                            a.linie = atoi(token2);
                        }
                    }
                    else if (strncmp(s,"b",1) == 0)
                    {   pozitie m;
                        m.linie = a.linie ;
                        m.coloana = a.coloana;
                        backspace(&head,m);
                    }
                    else if (strncmp(s,"d",1) == 0)
                    {   
                        int cat;
                        char* token = strtok(s," ");
                        char* token1 = strtok(NULL, " \n");
                        if (token1 == NULL)
                        {
                            cat = 1;
                        }
                        else
                        {
                            cat = atoi(token1);
                        }
                        pozitie m;
                        m.linie = a.linie ;
                        m.coloana = a.coloana;
                        delete_char(&head,cat,m);
                    }
                    // else if (strncmp(s,"u",1) == 0)
                    // {   
                    //  return 0;
                    // }
                    
                    fgets(s,100,fp);
                }
                
            }
        }
    }
    // while (head->next)
    // {printf("%c", head->data);
    // head = head->next;}
    
    fclose (fp);
    freeList(head);
    return 0;
}