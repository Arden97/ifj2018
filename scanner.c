#include "error.h"
#include "scanner.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "strlib.h"

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
  } while (isalpha(c = fgetc(stdin)) || isdigit(c) || '_' == c);

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
      break;
    case 4:
      if (0 == strcmp("else", buf)) return save_token(TOKEN_ELSE);
      if (0 == strcmp("then", buf)) return save_token(TOKEN_THEN);
      break;
    case 5:
      if (0 == strcmp("while", buf)) return save_token(TOKEN_WHILE);
      break;
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
  error_msg(SYNTAX_ERROR, "string hex literal \\x contains invalid digits");
  return -1;
}


/*
 * Scan string.
 */

static ifj18_token_t *scan_string() {
  int c, len = 0;
  char buf[128];

  while ('"' != (c = fgetc(stdin))) {
    switch (c) {
      case '\\':
        switch (c = fgetc(stdin)) {
          case 'a': c = '\a'; break;
          case 'b': c = '\b'; break;
          case 'f': c = '\f'; break;
          case 'n': c = '\n'; break;
          case 'r': c = '\r'; break;
          case 't': c = '\t'; break;
          case 'v': c = '\v'; break;
          case 'x':
            if (-1 == hex_literal()) {
              return 0;
            }
        }
        break;
    }
    buf[len++] = c;
  }

  buf[len++] = 0;
  string_copy_literal(token->value->as_string, buf);
  return save_token(TOKEN_STRING);
}

/*
 * Scan number.
 */

static ifj18_token_t *scan_number(int c) {
  int n = 0, type = 0, expo = 0, e;
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
      if (!isxdigit(c = fgetc(stdin))) {
        error_msg(SYNTAX_ERROR, "hex literal expects one or more digits");
        return 0;
      } else {
        do n = n << 4 | hex(c);
        while (isxdigit(c = fgetc(stdin)));
      }
      ungetc(c, stdin);
      token->value->as_int = n;
      return save_token(TOKEN_INT);
    default:
      ungetc(c, stdin);
      c = '0';
      goto scan_int;
  }

  // [0-9]+

  scan_int:
  do {
    if ('.' == c) goto scan_float;
    else if ('e' == c || 'E' == c) goto scan_expo;
    n = n * 10 + c - '0';
  } while (isdigit(c = fgetc(stdin)) || '.' == c || 'e' == c || 'E' == c);
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
    ungetc(c, stdin);
    token->value->as_float = (float) n / e;
    return save_token(TOKEN_FLOAT);;
  }

  // [\+\-]?[0-9]+

  scan_expo: {
    while (isdigit(c = fgetc(stdin)) || '+' == c || '-' == c) {
      if ('-' == c) {
        expo_type = 0;
        continue;
      }
      expo = expo * 10 + c - '0';
    }

    ungetc(c, stdin);
    if (expo_type == 0) expo *= -1;
    if (type == 0) {
      token->value->as_int = n * pow(10, expo);
      return save_token(TOKEN_INT);
    } else {
      token->value->as_float = ((float) n / e) * pow(10, expo);
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
    case '-': return save_token(TOKEN_OP_MINUS);
    case '*': return save_token(TOKEN_OP_MUL);
    case '/': return save_token(TOKEN_OP_DIV);
    case -1: return save_token(TOKEN_END_OF_FILE);
    case '!': return '=' == (c = fgetc(stdin)) ? save_token(TOKEN_OP_NEQ) : (ungetc(c, stdin), save_token(TOKEN_OP_NOT));
    case '=': return '=' == (c = fgetc(stdin)) ? save_token(TOKEN_OP_EQ) : (ungetc(c, stdin), save_token(TOKEN_OP_ASSIGN));
    case '&':
      switch (c = fgetc(stdin)) {
        case '&':
          return save_token(TOKEN_OP_AND);
      }
    case '|':
      switch (c = fgetc(stdin)) {
        case '|':
          return save_token(TOKEN_OP_OR);
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
    case '\r':
      goto scan;
    case '"':
      return scan_string();
    case 0:
      return save_token(TOKEN_EOS);
    default:
      if (isalpha(c) || '_' == c) return scan_ident(c);
      if (isdigit(c) || '.' == c) return scan_number(c);
      error_msg(LEXICAL_ERROR, "illegal character");
      return save_token(TOKEN_ILLEGAL);
  }
}
