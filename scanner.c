///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Lexical analysis 	                                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////


#include "scanner.h"

void init_token() {
  token = malloc(sizeof(ifj18_token_t));
  token->value = malloc(sizeof(t_token_value));
  token->value->as_string = malloc(sizeof(string));
  string_init(token->value->as_string);
}

// function to save read token to token structure
ifj18_token_t *save_token(ifj18_token type) {
  token->type = type; // assign the type
  token->len = token->value->as_string->length;

  return token;
}

/*
 * Convert hex digit `c` to a base 10 int,
 * returning -1 on failure.
 */

static int hex(int c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return -1;
}


/*
 * Scan identifier.
 */

static ifj18_token_t *scan_ident(int c) {
  int len = 0;
  char buf[128]; // TODO: ditch these buffers
  do {
    buf[len++] = c;
  } while (isalpha(c = fgetc(stdin)) || isdigit(c) || '_' == c || '?' == c || '!' == c);

  ungetc(c, stdin);

  buf[len++] = 0;
  switch (len-1) {
    case 2:
      if (0 == strcmp("if", buf)) return save_token(TOKEN_IF);
      if (0 == strcmp("do", buf)) return save_token(TOKEN_DO);
      break;
    case 3:
      if (0 == strcmp("def", buf)) return save_token(TOKEN_DEF);
      if (0 == strcmp("end", buf)) return save_token(TOKEN_END);
      if (0 == strcmp("chr", buf)) return save_token(TOKEN_CHR);
      if (0 == strcmp("ord", buf)) return save_token(TOKEN_ORD);
      if (0 == strcmp("nil", buf)) return save_token(TOKEN_NIL);
      break;
    case 4:
      if (0 == strcmp("else", buf)) return save_token(TOKEN_ELSE);
      if (0 == strcmp("then", buf)) return save_token(TOKEN_THEN);
      break;
    case 5:
      if (0 == strcmp("while", buf)) return save_token(TOKEN_WHILE);
      if (0 == strcmp("print", buf)) return save_token(TOKEN_PRINT);
      break;
    case 6:
      if (0 == strcmp("inputs", buf)) return save_token(TOKEN_INPUTS);
      if (0 == strcmp("inputf", buf)) return save_token(TOKEN_INPUTF);
      if (0 == strcmp("inputi", buf)) return save_token(TOKEN_INPUTI);
      if (0 == strcmp("length", buf)) return save_token(TOKEN_LENGTH);
      if (0 == strcmp("substr", buf)) return save_token(TOKEN_SUBSTR);
  }

  string_copy_literal(token->value->as_string, buf);
  return save_token(TOKEN_ID);
}

/*
 * Scan string hex literal, returning -1 on invalid digits.
 */

static int hex_literal() {
  int a = hex(fgetc(stdin));
  int b = hex(fgetc(stdin));
  if (a > -1 && b > -1) return a << 4 | b;
  error(LEXICAL_ERROR, "string hex literal \\x contains invalid digits");
}


/*
 * Scan string.
 */

static ifj18_token_t *scan_string() {
  int c, len = 0;
  char lit[5] = "";
  char buf[128];

  while ('"' != (c = fgetc(stdin))) {
    switch (c) {
      case '\\': // escape sequence start
        switch (c = fgetc(stdin)) {
          case 'n':
            c = '\\';
            buf[len++] = c;
            c = '0';
            buf[len++] = c;
            c = '1';
            buf[len++] = c;
            c = '0';
            buf[len++] = c;
          break;

          case 't':
            c = '\\';
            buf[len++] = c;
            c = '0';
            buf[len++] = c;
            c = '9';
            buf[len++] = c;
          break;

          case 's':
            c = '\\';
            buf[len++] = c;
            c = '0';
            buf[len++] = c;
            c = '3';
            buf[len++] = c;
            c = '2';
            buf[len++] = c;
          break;

          case '"':
            c = '\\';
            buf[len++] = c;
            c = '0';
            buf[len++] = c;
            c = '3';
            buf[len++] = c;
            c = '4';
            buf[len++] = c;
          break;

          case '\\':
            c = '\\';
            buf[len++] = c;
            c = '0';
            buf[len++] = c;
            c = '9';
            buf[len++] = c;
            c = '2';
            buf[len++] = c;
          break;

          case 'x':
            c = '\\';
            buf[len++] = c;
            c = hex_literal();

            sprintf(lit, "%d", c);
            if(strlen(lit)==2){
              c = '0';
              buf[len++] = c;
              buf[len++] = lit[0];
              buf[len++] = lit[1];
            }
            else if(strlen(lit)==3){
              buf[len++] = lit[0];
              buf[len++] = lit[1];
              buf[len++] = lit[2];
            }
          break;
        }
      break;

      default: buf[len++] = c;
    }
  }

  buf[len++] = 0;
  string_copy_literal(token->value->as_string, buf);
  return save_token(TOKEN_STRING);
}

/*
 * Scan number.
 */

static ifj18_token_t *scan_number(int c) {
  int n = 0, type = 0, expo = 0, e = 0;
  int expo_type = 1;
  /* expo_type:
   * 1 -> '+'(default)
   * 0 -> '-'
   */

  switch (c) {
    case '0': goto scan_hex;
    default: goto scan_int;
  }

  scan_hex:
  switch (c = fgetc(stdin)) {
    case 'x':
      error(LEXICAL_ERROR, "invalid digit");
      if (!isxdigit(c = fgetc(stdin))) {
        error(LEXICAL_ERROR, "hex literal expects one or more digits");
        return 0;
      } else {
        do n = n << 4 | hex(c);
        while (isxdigit(c = fgetc(stdin)));
      }
      ungetc(c, stdin);
      token->value->as_int = n;
      return save_token(TOKEN_INT);
    case '.':
      ungetc(c, stdin);
      c = '0';
      goto scan_int;
    case 'e':
    case 'E':
      ungetc(c, stdin);
      c = '0';
      goto scan_expo;
    default:
      if (isdigit(c))
        error(LEXICAL_ERROR, "invalid digit");
      else{
        ungetc(c, stdin);
        c = '0';
        token->value->as_int = 0;
        return save_token(TOKEN_INT);
      }
  }

  // [0-9]+

  scan_int:
  do {
    if ('.' == c) goto scan_float;
    else if ('e' == c || 'E' == c) goto scan_expo;
    n = n * 10 + c - '0';
  } while (isdigit(c = fgetc(stdin)) || '.' == c || 'e' == c || 'E' == c);

  if(isalpha(c) || (c >= 33 && c <= 34) || (c >= 36 && c<= 37) || c == 39  || (c >= 58 && c<= 59) || (c >= 63 && c<= 64) ||
                   (c >= 91 && c<= 96) || c == 123 || (c >= 125 && c <= 126))
    error(LEXICAL_ERROR, "number contains invalid symbols");

  ungetc(c, stdin);
  token->value->as_int = n;
  return save_token(TOKEN_INT);

  // [0-9]+

  scan_float: {
    e = 1;
    type = 1;
    while (isdigit(c = fgetc(stdin)) || 'e' == c || 'E' == c) {
      if ('e' == c || 'E' == c) goto scan_expo;
      n = n * 10 + c - '0';
      e *= 10;
    }

    if(isalpha(c) || (c >= 33 && c <= 34) || (c >= 36 && c<= 37) || c == 39  || (c >= 58 && c<= 59) || (c >= 63 && c<= 64) ||
                     (c >= 91 && c<= 96) || c == 123 || (c >= 125 && c <= 126))
      error(LEXICAL_ERROR, "number contains invalid symbols");

    ungetc(c, stdin);
    token->value->as_float = (float) n / e;
    return save_token(TOKEN_FLOAT);;
  }

  // [\+\-]?[0-9]+

  scan_expo: {
    int op_was = 0;
    while (isdigit(c = fgetc(stdin)) || '+' == c || '-' == c) {
      if ('-' == c && !op_was) {
        op_was = 1;
        expo_type = 0;
        continue;
      }
      else if('-' == c && op_was)
        error(LEXICAL_ERROR, "number contains invalid symbols");

      if ('+' == c && !op_was){
        op_was = 1;
        continue;
      }
      else if('+' == c && op_was)
        error(LEXICAL_ERROR, "number contains invalid symbols");
      expo = expo * 10 + c - '0';
    }

    if(isalpha(c) || (c >= 33 && c <= 34) || (c >= 36 && c<= 37) || c == 39  || (c >= 58 && c<= 59) || (c >= 63 && c<= 64) ||
                     (c >= 91 && c<= 96) || c == 123 || (c >= 125 && c <= 126))
      error(LEXICAL_ERROR, "number contains invalid symbols");

    ungetc(c, stdin);
    if (expo_type == 0) expo *= -1;
    if (type == 0) {
      token->value->as_int = (int) (n * pow(10, expo));
      return save_token(TOKEN_INT);
    } else {
      token->value->as_float = (float) (((float) n / e) * pow(10, expo));
      return save_token(TOKEN_FLOAT);
    }
  }
}

ifj18_token_t *get_token() {
  init_token();
  int c;

  // scan
  scan:
  switch (c = fgetc(stdin)) {
    case ' ':
    case '\t': goto scan;
    case '(': return save_token(TOKEN_LPAREN);
    case ')': return save_token(TOKEN_RPAREN);
    case ',': return save_token(TOKEN_COMMA);
    case '.': return save_token(TOKEN_OP_DOT);
    case '+': return save_token(TOKEN_OP_PLUS);
    case '-':
      // c = fgetc(stdin);
      // if(isdigit(c)) error(LEXICAL_ERROR, "only positive numbers");
      // else{
        // ungetc(c, stdin);
        return save_token(TOKEN_OP_MINUS);
      //}
    case '*': return save_token(TOKEN_OP_MUL);
    case '/': return save_token(TOKEN_OP_DIV);
    case -1: return save_token(TOKEN_END_OF_FILE);
    case '!':
      if('=' == (c = fgetc(stdin))){
        return save_token(TOKEN_OP_NEQ);
      }
      else{
        error(LEXICAL_ERROR, "Unknown token");
      }
    case '=':
      c = fgetc(stdin);
      if ('=' == c) return save_token(TOKEN_OP_EQ);
      else if(c == 'b'){
        if((c = fgetc(stdin)) == 'e');
        else{
          for(int i = 0; i<2; i++) ungetc(c, stdin);
          return save_token(TOKEN_OP_ASSIGN);
        }

        if((c = fgetc(stdin)) == 'g');
        else{
          for(int i = 0; i<3; i++) ungetc(c, stdin);
          return save_token(TOKEN_OP_ASSIGN);
        }
        if((c = fgetc(stdin)) == 'i');
        else{
          for(int i = 0; i<4; i++) ungetc(c, stdin);
          return save_token(TOKEN_OP_ASSIGN);
        }
        if((c = fgetc(stdin)) == 'n'){
          while (c = fgetc(stdin)){
            if(c == '=');
            else continue;
            if((c = fgetc(stdin)) == 'e');
            else continue;
            if((c = fgetc(stdin)) == 'n');
            else continue;
            if((c = fgetc(stdin)) == 'd') break;
          }
          //ungetc(c, stdin);
          goto scan;
        }
        else{
          for(int i = 0; i<5; i++) ungetc(c, stdin);
          return save_token(TOKEN_OP_ASSIGN);
        }
      }
      else {
        ungetc(c, stdin);
        return save_token(TOKEN_OP_ASSIGN);
      }
    case '&':
      switch (c = fgetc(stdin)) {
        case '&':
          return save_token(TOKEN_OP_AND);
        default:
          error(LEXICAL_ERROR, "Unknown token");
      }
    case '|':
      switch (c = fgetc(stdin)) {
        case '|':
          return save_token(TOKEN_OP_OR);
        default:
          error(LEXICAL_ERROR, "Unknown token");
      }
    case '<':
      switch (c = fgetc(stdin)) {
        case '=': return save_token(TOKEN_OP_LTE);
        default: return ungetc(c, stdin), save_token(TOKEN_OP_LT);

      }

    case '>':
      switch (c = fgetc(stdin)) {
        case '=': return save_token(TOKEN_OP_GTE);
        default: return ungetc(c, stdin), save_token(TOKEN_OP_GT);
      }
    case '#':
      while ((c = fgetc(stdin)) != '\n' && c) ; ungetc(c, stdin);
      goto scan;
    case '\n':
      return save_token(TOKEN_END_OF_LINE);
    case '\r':
      goto scan;
    case '"':
      return scan_string();
    case 0:
      return save_token(TOKEN_EOS);
    default:
      if ((c >= 'a' && c <= 'z') || '_' == c) return scan_ident(c);
      if (isdigit(c)) return scan_number(c);
      error(LEXICAL_ERROR, "illegal character");
      return save_token(TOKEN_ILLEGAL);
  }
}


void check_arg(int required_type, char id_allowed) {
  if ((token->type != required_type) || (id_allowed && token->type != TOKEN_ID)) {
    error(TYPE_ERROR, "Incorrect type");
  }
}

void check_token_type(int required_type, int error_type, int inv) {
  if (inv) {
    if (token->type != required_type) {
      error(error_type, "Unknown type, required:%s. Given: %s", ifj18_token_type_string(required_type), ifj18_token_type_string(token->type));
    }
  } else {
    if (token->type == required_type) {
      error(error_type, "");
    }
  }
}

void check_token_type_msg(int required_type, int error_type, int inv, char *message) {
  if (inv) {
    if (token->type != required_type) {
      error(error_type, message);
    }
  } else {
    if (token->type == required_type) {
      error(error_type, message);
    }
  }
//  printf("end of check_token\n");
}
