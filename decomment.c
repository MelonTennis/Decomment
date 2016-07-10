/***********************************************************************
 *This program is used to remove comments in the rogram and add space  *
 *With the number of lines unchanged                                   *
 **********************************************************************/
#include <stdio.h>
#include <string.h>
#include <error.h>
#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0
int state_decide(int c);
int line=0;
int comment_line=0;
enum Statetype{
   normal,slash,star,starback,charstr,charslashstr,charcon,charslashcon
 };
enum Statetype state;
int main()
{
  int c;
  state=normal;
  while ((c=getchar())!=EOF){
  state_decide(c);
  }
  if(state==star || state==starback){
  fprintf(stderr, "Error: line %d: unterminated comment\n",comment_line);
  printf("\n");
 } return 0;
}
int state_decide(int c)
/**************************************************************
 *This function can chage and decide the state the DFA is in .*
 *Additionally, it will print the message with comments moved.*
 *************************************************************/
{ 
  if(c=='\n'){
    line++;
  }
  if(state== normal){ /*Means a normal state.*/
    if(c=='/'){ 
      state=slash;/*Get a '/'. */
    }else if (c=='"'){
     state=charstr; /*Get a '\"'.*/
      putchar(c);
	}else if(c=='\''){
      state=charcon; /*Get a '\''.*/
      putchar(c);
    }else{
      state=normal;
      putchar(c);
    }
  }      
  else if (state== slash){
    if(c=='*'){
      state=star; /* When meeting a star behind'/' */
      putchar(' ');
      comment_line=line;
    }else if(c=='/'){
      state= slash;
      putchar('/');
    }else{
      state=normal;
      putchar('/');
      putchar(c);
    }
  }
  else if(state==star){/*comment situation*/
    if(c=='\n'){
      line++;
      putchar(c);
    }
    do{
      c=getchar();
      if(c=='\n'){
	line++;
	putchar(c);
      }
      else  if(c==EOF){
       break;
     }
    }while(c!='*');
    state=starback;
  }
    else if(state==starback){
      if(c=='/'){
	state=normal;
      }else if(c=='*'){
	state=starback;
      }else{
	state=star;
      }
    }
  else if (state==charstr){
        putchar(c);
	if(c=='\n'){
	  line++;
	}
	do{
	  c=getchar();
	  putchar(c);
	  if(c=='\n'){
	    line++;
	    return 0;
	  }
	}while(c!='"');
	state=normal;
  }
   else if(state== charcon){
	  putchar(c);
	  do{
	    c=getchar();
	    putchar(c);
	  if(c=='\n'){
	    line++;
	    return 0;
	  }
	}while(c!='\'');
	state=normal;
  }
  return line;
}



  
   
