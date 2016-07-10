/***********************************************************************
 *This program is used to remove comments in the rogram and add space  *
 *With the number of lines unchanged                                   *
 **********************************************************************/
#include <stdio.h>
#define EXIT_FALIURE -1
#define TRUE 1
int  unterminated(int c);
int line=0;
int state_decide(int c)
/**************************************************************
 *This function can chage and decide the state the DFA is in .*
 *Additionally, it will print the message with comments moved.*
 *************************************************************/
{
 enum Statetype{
   normal,slash,star,starbake,charstr,charslashstr,charcon,charslashcon
  }; 
 enum Statetype state;
 state = normal;
  if(c=='\n'){
    line++;
  }
  switch(state){
  case normal:{ /*Means a normal state.*/
    if(c=='/'){ 
      state=slash;/*Get a '/'. */
    }else if (c=='\"'){
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
  case slash:{
    if(c=='*'){
      state=star; /* When meeting a star behind'/' */
      putchar(' ');
    }else if(c=='/'){
      state= slash;
      putchar('/');
    }else{
      state=normal;
      putchar('/');
      putchar(c);
    }
  }
  case star:{/*comment situation*/
    return line;
    unterminated(c);
    if(c=='\n'){
      putchar(c);
    }
    if(c=='*'){
      state=starback;/*Get the * behind.*/
    }else{
      state=star;
    }
  }
  else if (state==starback){
    if(c=='\n'){
      putchar('c');
    }
    if(c=='*'){
      state=starback;
    }else if(c=='/'){
      state=normal;
    }else {
      state=star;
    }
  }
  case charstr:{
    if(c=='\\'){
      state=charslashstr; /*When there is a \ in string.*/
      putchar(c);
    }else if (c=='"'){
      state=normal;
      putchar(c);
    }else {
      state=charstr;
      putchar(c);
    }
  }
  case charslashstr:{
    state=charstr;
    putchar(c);
  }
  case charcon:{
    putchar(c);
    if (c=='\\'){
      state=charslashcon;/*When there is a \ in character. */
    }else if (c=='\''){
      state=normal;
    }else {
      state=charcon;
    }
  }
  case charslashcon:{
    putchar(c);
    state=charcon;
  }    
}
void unterminated(int c)
/**************************************************************************
 *This function is used to decide whether there is an untrminated comment.*
 *************************************************************************/
{
  while(c!='*');
    c=getchar();
    if(c!='/'){
      printf("Error: line %d: unterminated comment\n",line);
      return(EXIT_FAILURE);
    }
    return TRUE;
}
int main(void)
{
  int c=getchar();
  line=state_decide(c);
  return 0;
}

  
   
