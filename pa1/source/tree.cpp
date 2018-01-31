/* Copyright (c) 2002 U Wisc. ECE 756 */

/* ------------------------------------------------------------------------ */
/* INCLUDES                                                                 */
/* ------------------------------------------------------------------------ */

#include "tree.h"

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
/* EXPORTED VARIABLES                                                       */
/* ------------------------------------------------------------------------ */

const char *gate_names[] = {
  "NOT",
  "AND",
  "NAND",
  "OR",
  "INV",
  "NAND2",
  "NAND3",
  "NAND4",
  "AOI21",
  "AOI22"
};

/* ------------------------------------------------------------------------ */
/* FUNCTION DEFINITIONS                                                     */
/* ------------------------------------------------------------------------ */

void
tree_free(Tree t)
{
  if (t) {
    tree_free(T_LEFT(t));
    tree_free(T_RIGHT(t));
    free(t);
  }
}

Tree
tree_mk(Type type, Tree left, Tree right)
{
  Tree t = (Tree)malloc(sizeof(*t));

  if (!t) {
    printf("[tree_mk]: memory allocation failed.\n");
    exit(1);
  }
  T_TYPE (t) = type;
  T_LEFT (t) = left;
  T_RIGHT(t) = right;
  return t;
}

/* Prints tree `t' in infix. */
void
tree_print(Tree t)
{
  Type type;

  if (t == NULL) {
    printf("-");
    return;
  }

  switch (type = T_TYPE(t)) {
  case INV:
  case NOT:
    printf("%s(", gate_names[type]);
    tree_print(T_LEFT(t));
    printf(")");
    break;

  case AND:
  case OR:
  case NAND:
  case NAND2:
    printf("%s(", gate_names[type]);
    tree_print(T_LEFT(t));
    printf(",");
    tree_print(T_RIGHT(t));
    printf(")");
    break;

  default:
    assert(1);
  }
}
