/* Copyright (c) 2002 U Wisc. ECE 756 */

/* ------------------------------------------------------------------------ */
/* INCLUDES                                                                 */
/* ------------------------------------------------------------------------ */

#include "parser.h"

/* ------------------------------------------------------------------------ */
/* LOCAL DEFINES                                                            */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* LOCAL TYPE DEFINITIONS                                                   */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* LOCAL VARIABLES                                                          */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/* FUNCTION DEFINITIONS                                                     */
/* ------------------------------------------------------------------------ */

/* Considerations:
   - In case of syntax error, give appropriate message and abort; no recovery.
   - When parsing is successful, tree will be valid.
   - Parse routines mirror structure of grammar rules.
   - Reading a character takes care of skipping white-space.
*/

/* Give syntax error message and abort program. */
static void
fatal(char *mes, ...)
{
  va_list ap;

  fprintf(stderr, "Syntax error: ");
  va_start(ap, mes);
  vfprintf(stderr, mes, ap);
  fprintf(stderr, ".\n");
  va_end(ap);
  exit(1);
}

/* --------------------------------------------------------------------- */
/* The Lexer                                                             */
/* --------------------------------------------------------------------- */

/* Reads a character (first skips any white-space). */
static int
readc(void)
{
  int c;

restart:
  /* Check for end-of-file condition: */
  if (feof(stdin)) return EOF;

  /* Skip any whitespace: */
  do c = getchar(); while (isspace(c));

  /* Skip any comment-till-end-of-line: */
  if (c == '#') {
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    goto restart;
  }
  return c;
}

/* --------------------------------------------------------------------- */
/* The Parser                                                            */
/* --------------------------------------------------------------------- */

/* Forward declaration because of mutual recursion. */
static Tree Expression(void);

/* Primary : Variable [ ''' ] | '(' Expression ')' */
static Tree
Primary(void)
{
  int c = readc();

  /* Variable: */
  if (isalpha(c)) {
    int v = c;

    if ((c = getchar()) == '\'')
      return tree_mk_not(tree_mk_var(v));
    ungetc(c, stdin);
    return tree_mk_var(v);
  }

  /* '(' Expression ')': */
  if (c == '(') {
      Tree t = Expression();

      if (readc() != ')')
	fatal("')' expected");
      return t;
  }
  /* Any other character is considered an error. */
  fatal("primary expected");
  return NULL;
}

/* Factor : Primary | '!' Factor */
static Tree
Factor(void)
{
  int c;

  if ((c = readc()) == '!')
    return tree_mk_not(Factor());

  /* Read ahead; back up character c: */
  ungetc(c, stdin);
  return Primary();
}

/* Term : Factor [ ( [ '&' ] | "|" ) Term ] */
static Tree
Term(void)
{
  int c;
  Tree t = Factor();

  c = readc();

  if (c == '&')
    return tree_mk_and(t, Term());

  if (c == '|')
    return tree_mk_nand(t, Term());

  /* '&' is optional (juxtaposition): */
  if (isalpha(c) || strchr("(!", c)) {
    ungetc(c, stdin);
    return tree_mk_and(t, Term());
  }
  ungetc(c, stdin);
  return t;
}

/* Expression : Term [ '+' Expression ] */
static Tree
Expression(void)
{
  int c;
  Tree t = Term();

  if ((c = readc()) == '+')
    return tree_mk_or(t, Expression());
  ungetc(c, stdin);
  return t;
}

/* Input : Expression ";" */
Tree
parse(void)
{
  Tree t = Expression();

  if (readc() != ';')
    fatal("';' expected");

  return t;
}
