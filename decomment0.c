
/***********************************************************************
 *This program is used to remove comments in the rogram and add space  *
 *With the number of lines unchanged                                   *
 **********************************************************************/
#include <stdio.h>
#include <string.h>
#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0
int unterminated(int c);
int state_decide(int c);
int line=0;
int comment_line=0;
enum Statetype{
   normal,slash,star,starback,charstr,charslashstr,charcon,charslashcon
 };
enum Statetype state=normal;
int main()
{
  int c;
  while ((c=getchar())!=EOF){
  state_decide(c);
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
  else if (state== slash){
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
  else if(state==star){/*comment situation*/
    comment_line=line;
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
  else if(state==starback){
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
  else if (state== charstr){
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
  else if(state== charslashstr){
    state=charstr;
    putchar(c);
  }
  else if(state== charcon){
    putchar(c);
    if (c=='\\'){
      state=charslashcon;/*When there is a \ in character. */
    }else if (c=='\''){
      state=normal;
    }else {
      state=charcon;
    }
  }
  else if(state== charslashcon){
    putchar(c);
    state=charcon;
  }
  return line;

}

int unterminated(int c)
/**************************************************************************
 *This function is used to decide whether there is an untrminated comment.*
 *************************************************************************/
{
  while(c!='*');
    c=getchar();
  if(c!='/'){
      printf("Error: line %d: unterminated comment\n",comment_line);
      return(EXIT_FAILURE);
    }
  else {
  }return EXIT_SUCCESS;
}




