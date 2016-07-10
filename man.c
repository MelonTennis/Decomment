#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define word_num 42
#define word_length 32
#define sym_table_length 256
#define num_length 32
#define MAX 1024
#define float_table_length 256
//enum type{INT,SHORT,LONG INT,FLOAT,DOUBLE,CHAR,POINTER,STRING};
char word_table[word_num][word_length]={"$","#include <stdio.h>","main","for","while","if",
                                        "else","printf","scanf","return","int",
                                        "float","char","+","-","*","/","++","--","(",")",
                                        "[","]","{","}",";","\"","<",">","<=",">=",
                                        "=","==","!=","!","%","%d","&",","};
struct hnode
{
    int code;
    struct hnode *ptr;
};
struct hnode hash_table[word_num];

struct snode
{
    char name[word_length];
    int id;
    int type;
    int addr;
};

struct ssnode
{
    struct snode table[sym_table_length];
    int pr;
};
struct ssnode sym_table,symtmp;

struct tnode
{
    int id;
    int position;
    char ss[MAX];
};
struct tnode token;

struct sstrnode
{
    char str[MAX];
    int pr;
};
struct sstrnode sstr;

struct fnode{
    float table[float_table_length];
    int pr;
};
struct fnode float_table;

FILE *fd1,*fd2,*fd3,*fd4,*fd5;
int line,column;

unsigned int RShash(char *str);
void init();
char ffgetc(void);
int test(char ch);
void sort(void);
void recogid(char ch);
void recoglonid(char ch);//识别include
void recogano(char ch);
int handlecom(char ch);
void recogdel(char ch);
int recogdig(char ch);
void recogch(char ch);
void recogstr(char ch);
int iskeyword(char word[]);
int lookup(char word[]);
int insert(char word[],int id);
void disp(void);
void num_error(void);
int flookup(float p);

int main(void)
{
    init();
    fd1=fopen("hello1.c","r");
    fd2=fopen("token","w+");
    fd3=fopen("token.txt","w+");
    fd4=fopen("sym_table","w+");
    fd5=fopen("error.txt","w+");

    sort();
    disp();
    fclose(fd1);
    fclose(fd2);
    fclose(fd3);
    fclose(fd4);
    fclose(fd5);

    printf("词法分析完成!\n");

    return 0;
}

unsigned int RShash(char *str)
{
    unsigned  int  b  =   378551 ;
    unsigned  int  a  =   63689 ;
    unsigned  int  hash  =   0 ;

    while (*str)
    {
        hash=hash*a+(*str++);
        a*=b;
    }
    return(hash&0x7FFFFFFF)%word_num;
}

void init()
{
    int i,j;
    struct hnode *tmp;

    line=column=1;
    sym_table.pr=0;
    sstr.pr=0;
    float_table.pr=0;
    for (i=0;i<word_num;i++)
    {
        hash_table[i].code=-1;
        hash_table[i].ptr=NULL;
    }
    for (i=0;i<word_num;i++)
    {
        j=RShash(word_table[i]);
        if (hash_table[j].code==-1)
        {
            hash_table[j].code=i;
        }
        else
        {
            tmp=&hash_table[j];
            while (tmp->ptr!=NULL)
            {
                tmp=tmp->ptr;
            }
            tmp->ptr=(struct hnode*)malloc(sizeof(struct hnode));
            tmp=tmp->ptr;
            tmp->code=i;
            tmp->ptr=NULL;
        }
    }
}

char ffgetc(void)
{
    column++;
    return fgetc(fd1);
}

int iskeyword(char word[])
{
    int p;
    struct hnode *tmp;
    p=RShash(word);
    tmp=&hash_table[p];
    while (tmp!=NULL)
    {
        if (strcmp(word_table[tmp->code],word)==0)
            return tmp->code;
        else tmp=tmp->ptr;
    }
    return -1;
}

int test(char ch)
{
    if ((ch>='A'&&ch<='Z')||ch=='_'||(ch>='a'&&ch<='z'))
        return 0;
    else if (ch>='0'&&ch<='9')
        return 1;
    else if (ch=='/')
        return 2;
    else if (ch=='\'')
        return 3;
    else if (ch=='"')
        return 4;
    else if (ch=='#')
        return 5;
    else if (ch=='&')
        return 6;
    else return 7;
}

void sort(void)
{
    char ch;
    int tmp;

    ch=ffgetc();
    while (ch!=EOF)
    {
        tmp=1;
        if (ch!=' '&&ch!='\t'&&ch>0)
        {
            switch (test(ch))
            {
            case 0:
                recogid(ch);
                break;
            case 1:
                tmp=recogdig(ch);
                break;
            case 2:
                tmp=handlecom(ch);
                break;
            case 3:
                recogch(ch);
                break;
            case 4:
                recogstr(ch);
                break;
            case 5:
                recoglonid(ch);
                break;
            case 6:
                recogano(ch);
                break;
            default :
                recogdel(ch);
                break;
            }
            if (tmp==1)
            {
                if(token.id<39)
                    strcpy(token.ss,word_table[token.id]);
                    //printf("%s\n",word_table[token.id]);
                else if(token.id==39)
                    strcpy(token.ss,"id");
                    //printf("%s\n","id");
                else if(token.id==40)
                    strcpy(token.ss,"digit");
                    //printf("%s\n","digit");
                else if(token.id==41)
                    strcpy(token.ss,"fudian");
                    //printf("%s\n","fudian");
                else if(token.id==42)
                    strcpy(token.ss,"charu");
                    //printf("%s\n","charu");
                else if(token.id==43)
                    strcpy(token.ss,"string");
                    //printf("%s\n","string");
                else if(token.id==44)
                    strcpy(token.ss,"array");
                    //printf("%s\n","array");
                fwrite(&token,8,1,fd2);
                fprintf(fd3,"%d\t%d\t%s\n",token.id,token.position,token.ss);
                printf("%d\t%d\t%s\n",token.id,token.position,token.ss);
            }
        }
        ch=ffgetc();
        while (ch=='\n')
        {
            line++;
            column=1;
            ch=ffgetc();
        }
    }
}

void recogano(char ch)
{
    token.id=37;
    token.position=-1;
}

void recogid(char ch)
{
    char word[word_length];
    int i=0,tmp;
    word[i++]=ch;

    do
    {
        ch=ffgetc();
        tmp=test(ch);
        if (tmp==1||tmp==0)
            word[i++]=ch;
    }
    while ((tmp==0||tmp==1));
    word[i]='\0';
    ungetc(ch,fd1);
    column--;
    if ((tmp=iskeyword(word))!=-1)
    {
        token.id=tmp;
        token.position=-1;
    }
    else
    {
        tmp=lookup(word);
        fpos_t pos;
        fgetpos (fd1,&pos);
        char cur=fgetc(fd1);
        if(cur!='[')
            token.id=39;
        else
            token.id=44;
        fsetpos (fd1,&pos);
        if (tmp==-1)
            token.position=insert(word,39);
        else
            token.position=tmp;
    }
}


void recoglonid(char ch)
{
    char word[word_length];
    int i=0,tmp;
    word[i++]=ch;

    do
    {
        ch=ffgetc();
        //tmp=test(ch);
        //if (tmp==1||tmp==0)
            word[i++]=ch;
    }
    while ((ch!='\n'&&ch!='{'));
    word[i]='\0';
    ungetc(ch,fd1);
    column--;
    //if ((tmp=iskeyword(word))!=-1)
    //{
        token.id=1;
        token.position=-1;
    //}
    //else
    //{
        //error
    //}
}

int handlecom(char ch)
{
    ch=ffgetc();
    if (ch=='*')
    {

        while ((ch=ffgetc())!='*');
        ch=ffgetc();
        if (ch!='/')
        {
            fprintf(fd5,"注解未完!line=%d,column=%d\n",line,column);
            return -1;
        }
        return 0;
    }
    else if (ch=='/')
    {
        while ((ch=ffgetc())!='\n');
        ungetc(ch,fd1);
        column--;
        return 0;
    }
    else
    {
        token.id=16;
        token.position=-1;
        ungetc(ch,fd1);
        column--;
        return 1;
    }
}

void recogdel(char ch)
{
    int p,q;
    char word[3];

    word[0]=ch;
    word[1]='\0';
    ch=ffgetc();
    p=iskeyword(word);
    if (p!=-1)
    {
        word[1]=ch;
        word[2]='\0';
        q=iskeyword(word);
        if (q!=-1)
        {
            token.id=q;
            token.position=-1;
        }
        else
        {
            token.id=p;
            token.position=-1;
            ungetc(ch,fd1);
            column--;
        }
    }
}

int recogdig(char ch)
{
    char word[num_length];
    int i,state,j,tmp1;
    float tmp2;

    i=0;
    state=1;
    word[i++]=ch;
    while (state!=7)
    {
        ch=ffgetc();
        switch (state)
        {
        case 1:
            if (test(ch)==1) state=1;
            else if (ch=='.') state=2;
            else if (ch=='e') state=4;
            else if (test(ch)==0)
            {
                num_error();
                return 0;
            }
            else state=7;
            break;
        case 2:
            if (test(ch)==1) state=3;
            else
            {
                num_error();
                return 0;
            }
            break;
        case 3:
            if (test(ch)==1) state=3;
            else if (ch=='e') state=4;
            else if (test(ch)==0||ch=='.')
            {
                num_error();
                return 0;
            }
            else state=7;
            break;
        case 4:
            if (ch=='+'||ch=='-')    state=5;
            else
            {
                num_error();
                return 0;
            }
            break;
        case 5:
            if (test(ch)==1) state=6;
            else
            {
                num_error();
                return 0;
            }
            break;
        case 6:
            if (test(ch)==1) state=6;
            else if (test(ch)==0||ch=='.')
            {
                num_error();
                return 0;
            }
            else state=7;
            break;
        }
        word[i++]=ch;
    }
    word[i-1]='\0';
    ungetc(ch,fd1);
    column--;
    for (j=0;j<i-1;j++)
    {
        if (word[j]=='.'||word[j]=='e')
        {
            sscanf(word,"%f",&tmp2);
            token.id=41;
            token.position=flookup(tmp2);
            return 1;
        }
    }
    sscanf(word,"%d\n",&tmp1);
    token.id=40;
    token.position=tmp1;

    return 1;
}

void num_error(void)
{
    char ch;

    ch=ffgetc();
    while (test(ch)==0||test(ch)==1||ch=='.')
        ch=ffgetc();
    ungetc(ch,fd1);
    column--;
    fprintf(fd5,"wrong number!line=%d,column=%d\n",line,column);
}

void recogch(char ch)
{
    char ch1;

    ch=ffgetc();
    if ((ch1=ffgetc())!='\'')
        fprintf(fd5,"char error!line=%d,column=%d\n",line,column);
    else
    {
        token.id=42;
        token.position=ch;
    }
}

void recogstr(char ch)
{
    token.id=43;
    token.position=sstr.pr;
    ch=ffgetc();
    //if(ch=='%')
    //{
      //  ch=ffgetc();
        //if(ch=='d')
        //{
          //  token.id=36;
            //token.position=-1;
            //ch=ffgetc();
            //return;
        //}
        //else
          //  ungetc(ch,fd1);
   // }

    while (ch!='"'&&ch!='\n')
    {
        sstr.str[(sstr.pr)++]=ch;
        ch=ffgetc();
    }
    sstr.str[(sstr.pr)++]='\0';
    if(ch=='\n')
    {
        fprintf(fd5,"string error!line=%d,column=%d\n",line,column);
        ungetc(ch,fd1);
        column--;
    }
}


int lookup(char word[])
{
    int i;

    for (i=sym_table.pr-1;i>=0;i--)
    {
        if (strcmp(sym_table.table[i].name,word)==0)
            return i;
    }
    return -1;
}

int insert(char word[],int id)
{
    int len;

    len=strlen(word);
    if (sym_table.pr>sym_table_length)
        fprintf(fd5,"symbol table full!\n");
    if (len>word_length)
        fprintf(fd5,"name'length excel %d!\n",word_length);
    sym_table.table[sym_table.pr].id=id;
    strcpy(sym_table.table[sym_table.pr].name,word);
    sym_table.pr++;
    return sym_table.pr-1;
}

int flookup(float p)
{
    int i;

    for (i=float_table.pr-1;i>=0;i--)
    {
        if (float_table.table[i]==p)
            return i;
    }
    if(float_table.pr>float_table_length)
        fprintf(fd5,"float table full!\n");
    else
    {
        float_table.table[float_table.pr++]=p;
    }
    return float_table.pr-1;
}

void disp(void)
{
    int i;
    printf("\n\nsym_table:\n");
    for (i=0;i<sym_table.pr;i++)
        printf("%s\t\t%d\n",sym_table.table[i].name,sym_table.table[i].id);
    fwrite(&sym_table,4+sym_table_length*(12+word_length),1,fd4);

}

