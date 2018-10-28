#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "scanner.h"

token_t * token;
bool unget;

void initToken(){
    token = (token_t*) malloc(sizeof(struct token_t));
    if(!token) exit(99);

    token->str = strInit(STR_INIT); // allocation memmory for string
}

void ungetToken(){
    unget=true;
}

//function to save read token to token structure
token_t *saveToken(int type, bool string){
    if(!token){
        exit(99);
        return NULL;
    }
    if(!string) token->str->string[0]='\0';

    token->type = type; //assign the type
    token->str->length = strlen(token->str->string);
    printf("token : %s %d\n",token->str->string,token->type);
    return token;
}

token_t *getToken(){
  if(unget){
      unget=false;
      return token;
  }

  int position = 0;           //index of actual char
  char hexa[4];
  hexa[3] = '\0';
  int escape = 0;
  int number_cnt = 0;
  bool exponent = false;
  bool dot = false;
  int i = 0;
  states_t state = S_START;   //state of automaton

  initToken();

  while(1){
    char c = fgetc(stdin);
    token->str->length=strlen(token->str->string);

    if(token->str->length+1 > token->str->capacity) // if we don't have enough space for token's string
        extendStr(token->str, 2*token->str->capacity);

    switch (state) {
      case S_START:

        if(c == '\n')
          return saveToken(EOL, false); //EOL

        else if(isspace(c))
          state = S_START; //white sign == continue

        else if(isalpha(c) || c == '_'){
          state = S_IDENTIFIER; //identifier, key word
          token->str->string[position++] = c;
        }

        else if(isdigit(c)){
          state = S_NUMBER; //number
          token->str->string[position++] = c;
        }

        else if(c == '(')
          return saveToken(LEFT_R_BRACKET, false); //left rounded bracket

        else if(c == ')')
          return saveToken(RIGHT_R_BRACKET, false); //right rounded bracket

        else if(c == ',')
          return saveToken(COMMA, false);  //comma

        else if(c == ';')
          return saveToken(SEMICOLON, false);  //semicolon

        else if(c == '*')
          return saveToken(MUL, false);    //mul

        else if(c == '-')
          return saveToken(SUB, false);    //subtract

        else if(c == '+')
          return saveToken(ADD, false);    //add

        else if(c == '='){
          state = S_EQ;
        }

        else if(c == '\\')
          return saveToken(INT_DIV, false);   //integer dividing

        else if(c == '#'){ // line comment
          while ( ((c = fgetc(stdin)) != '\n') && (c != -1) );
            if(c == -1)
              return saveToken(END_OF_FILE, false);

            else if (c=='\n')
              return saveToken(EOL, false);    //EOL

            else state = 0;
        }

        else if(c == '!')
          state = S_NOT_EQ;

        else if(c == '>')
          state = S_GR_EQ;    //greater/ greater or equal

        else if(c == '<')
          state = S_LESS_EQ;  //less/ less or equal

        else if(c == '"')
          state = S_STR;

        else if(c == -1)
          return  saveToken(END_OF_FILE, false);   //end of file

        else{
          exit(1);  //lexical error
          return NULL;
        }

      break;

      case S_STR:
        //character is not backslah, newline or quotation mark
        if(((c!='"') && (escape == 0) && (c!='\x0A') && (c!='\x5C'))){
          token->str->string[position++] = c;
          state = S_STR;
        }

        //backslash means, that escape sequence is on
        else if(((c=='\x5C') && (escape == 0))){
          escape = 1;
          state = S_STR;
        }

        //if escape sequence is on, write backslash to string
				else if (((c == '\x5C') && (escape == 1))){
					token->str->string[position++] = c;
					escape = 0;
					state = S_STR;
			  }

        //if escape sequence is on, write quotation mark to string
        else if(((c == '\x22') && (escape == 1))){
          token->str->string[position++] = c;
          escape = 0;
          state = S_STR;
        }

        //if escape sequence is on, write new line to string
        else if (c == 'n' && escape == 1){
          c = '\x0A';
          token->str->string[position++] = c;
          escape = 0;
          state = S_STR;
        }

        //if escape sequence is on, write tab to string
        else if(((c == 't') && (escape == 1))){
          c = '\x09';
          token->str->string[position++] = c;
          escape = 0;
          state = S_STR;
        }

        //escape sequence
        else if (((isdigit(c)) && (escape == 1))){
          hexa[0] = c;
          number_cnt = 1;
          escape = 0;
          state = S_STRING_NUMBERS;
        }

        //string must be in one row
        else if (((c == '\n') && (escape == 0))){
          exit(1);
          return NULL;
        }

        //quotation mark means end of string
        else if (((c == '"') && (escape == 0))){ // end of string
          token->str->string[position] = '\0';
          return saveToken(TEXT, true);
        }

        //everything else we consider as lexical error
        else{
          exit(1);
          return NULL;
        }

      break;

      //decimal escape sequence
			case S_STRING_NUMBERS:

				//2nd number is written
        if(((isdigit(c)) && (number_cnt == 1))){
					hexa[1] = c;
				 	number_cnt = 2;
					state = S_STRING_NUMBERS;
			  }
			  //3rd number is written
			  else if(((isdigit(c)) && (number_cnt == 2))){
					hexa[2] = c;

					int number = atoi(hexa);
          //printf("ASCII: %d\n", number);

          //number must be in interval <0,255>, otherwise lexical error
				  if(((number < 001) || (number > 255))){
				    exit(1);
						return NULL;
				  }

				  else{
            token->str->string[position++] = number;
						number_cnt = 0;
						state = S_STR;
					}
				}

				else{
					exit(1);
					return NULL;
				}

			break;

      //not equal or lex error
      case S_NOT_EQ:
        if(c == '=')
          return  saveToken(NEQ, false);   // !=

        else{
          exit(1);  //lexical error
          return NULL;
        }
      break;

      //greater, equal
      case S_GR_EQ:
        if(c == '=')
          return  saveToken(GREATER_EQ, false);    // >=

        else{
          ungetc(c, stdin);
          return  saveToken(GREATER, false);   //>
        }
      break;

      //less, equal
      case S_LESS_EQ:
          if(c == '=')
              return  saveToken(LESS_EQ, false);   //<=

          else if (c == '>')
              return saveToken(NEQ, false);     //<>

          else{
              ungetc(c, stdin);
              return  saveToken(LESS, false);  //<
          }
      break;

      case S_EQ:

        c=fgetc(stdin);
        if(c=='b'){
          i++;
          c=fgetc(stdin);
          if(c=='e'){
            i++;
            c=fgetc(stdin);
            if(c=='g'){
              i++;
              c=fgetc(stdin);
              if(c=='i'){
                i++;
                c=fgetc(stdin);
                if(c=='n') i = 0;
                else{
                  // if it's not block comment
                  for(; i>=0; i--) ungetc(c, stdin);
                  return saveToken(ASSIGNMENT_EQ, false);   // assignment
                }
              }
              else{
                // if it's not block comment
                for(; i>=0; i--) ungetc(c, stdin);
                return saveToken(ASSIGNMENT_EQ, false);   // assignment
              }
            }
            else{
              // if it's not block comment
              for(; i>=0; i--) ungetc(c, stdin);
              return saveToken(ASSIGNMENT_EQ, false);   // assignment
            }
          }
          else{
            // if it's not block comment
            for(; i>=0; i--) ungetc(c, stdin);
            return saveToken(ASSIGNMENT_EQ, false);   // assignment
          }
        }
        else{
          // if it's not block comment
          for(; i>=0; i--) ungetc(c, stdin);
          return saveToken(ASSIGNMENT_EQ, false);   // assignment
        }

        while((c  = fgetc(stdin)) != -1){ //read till end of file
          if(c == '='){
            c  = fgetc(stdin);
            if(c=='e'){
              i++;
              c  = fgetc(stdin);
              if(c=='n'){
                i++;
                c  = fgetc(stdin);
                if(c=='d') {
                  i = 0;
                  break; // exit from comment
                }
                else for(; i>=0; i--) ungetc(c, stdin);
              }
              else for(; i>=0; i--) ungetc(c, stdin);
            }
            else ungetc(c, stdin);
          }
        }
        //comment till end of file
        //if(c == -1) exit(1);

        state = S_START;
      break;

      case S_NUMBER:

        if(isdigit(c))
          token->str->string[position++] = c;

        //exponent
        else if(c == 'e'){
          if( exponent == true){
            exit(1);  //2 exponents == lexical error
            return NULL;
          }

          exponent = true;
          token->str->string[position++] = c;
          c = fgetc(stdin);

          if(isdigit(c))
            token->str->string[position++] = c;

          // + alebo -
          else if((c == '+') || (c == '-')){
            token->str->string[position++] = c;
            c = fgetc(stdin);
            //after + or - must follow the number, else lexical error
            if(isdigit(c))
              token->str->string[position++] = c;

            else{
              exit(1);
              return NULL;
            }
          }
          else{
            exit(1);
            return NULL;
          }
        }

        //double
        else if(c == '.'){
            if( dot == true){
                exit(1);  //number can not content 2 dots
                return NULL;
            }

            dot = true;
            token->str->string[position++] = c;
            c = fgetc(stdin);

            if(isdigit(c))
                token->str->string[position++] = c;

            else{
                exit(1);
                return NULL;
            }
        }

        //characters, that number can not content
        else if ((c == '_') || isalpha(c)){
            exit(1);
            return NULL;
        }

        else{
            ungetc(c, stdin);
            token->str->string[position] = '\0';
            bool integer = true;
            for(int i = 0; i < position; i++){
                if(!(isdigit(token->str->string[i])))
                    integer = false;
            }

            if(integer)
              return saveToken(INT_NUMBER, true);   //integer

            else
              return saveToken(DOUBLE_NUMBER, true);  //double
        }
      break;

      case S_IDENTIFIER:

        if(isalpha(c) || (c == '_') || isdigit(c))
            token->str->string[position++] = c;

        else{
            ungetc(c, stdin);
            token->str->string[position] = '\0';

            //identifier or keyword
            if ((strcmp(token->str->string, "do")) == 0)
              return saveToken(DO, false);

            else if ((strcmp(token->str->string, "def")) == 0)
              return saveToken(DEF, false);

            else if ((strcmp(token->str->string, "else")) == 0)
              return saveToken(ELSE, false);

            else if ((strcmp(token->str->string, "end")) == 0)
              return saveToken(END, false);

            else if ((strcmp(token->str->string, "if")) == 0)
              return saveToken(IF, false);

            else if ((strcmp(token->str->string, "then")) == 0)
              return saveToken(THEN, false);

            else if ((strcmp(token->str->string, "while")) == 0)
              return saveToken(WHILE, false);

            else if ((strcmp(token->str->string, "inputs")) == 0)
              return saveToken(INPUTS, false);

            else if ((strcmp(token->str->string, "inputi")) == 0)
              return saveToken(INPUTI, false);

            else if ((strcmp(token->str->string, "inputf")) == 0)
              return saveToken(INPUTF, false);

            else if ((strcmp(token->str->string, "print")) == 0)
              return saveToken(PRINT, false);

            else if ((strcmp(token->str->string, "length")) == 0)
              return saveToken(LENGTH, false);

            else if ((strcmp(token->str->string, "substr")) == 0)
              return saveToken(SUBSTR, false);

            else if ((strcmp(token->str->string, "chr")) == 0)
              return saveToken(CHR, false);

            else if ((strcmp(token->str->string, "ord")) == 0)
              return saveToken(CHR, false);

            else return saveToken(ID, true);
        }
    break;
    }

    if(c == -1) exit(1);
  }
}
