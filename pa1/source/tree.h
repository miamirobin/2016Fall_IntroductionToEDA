/* Copyright (c) 2002 ACM/SIGDA */

#ifndef TREE_H
#define TREE_H

/* ------------------------------------------------------------------------ */
/* INCLUDES								    */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#if defined __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* DEFINES								    */
/* ------------------------------------------------------------------------ */

/* Tree node field access macros: */
#define T_TYPE(t)		((t)->type)
#define T_AREA(t)		((t)->area)
#define T_CELL(t)		((t)->cell)
#define T_LEFT(t)		((t)->left)
#define T_RIGHT(t)		((t)->right)

/* Tree constructor macros: */
#define tree_mk_var(v)		NULL
#define tree_mk_inv(t)		tree_mk(INV,(t),NULL)
#define tree_mk_nand2(t1,t2)	tree_mk(NAND2,(t1),(t2))
#define tree_mk_not(t)		tree_mk(NOT,(t),NULL)
#define tree_mk_and(t1,t2)	tree_mk(AND,(t1),(t2))
#define tree_mk_nand(t1,t2)	tree_mk(NAND,(t1),(t2))
#define tree_mk_or(t1,t2)	tree_mk(OR,(t1),(t2))

/* ------------------------------------------------------------------------ */
/* TYPE DEFINITIONS							    */
/* ------------------------------------------------------------------------ */

/* Type to express chip area units; for now discrete. */
typedef unsigned int Area;

/* Type of tree node: */
typedef enum {
  /* Technology independent. */
  NOT,
  AND,
  NAND, /* Included for test purposes to allow to skip normalizing phase. */
  OR,

  /* Normalized (canonical) technology independent. */
  INV,				/* INV(-) */
  NAND2,			/* NAND(-,-) */

  /* Technology dependent; library cells */
  /*INV, NAND2 belong to this category as well */
  NAND3,			/* NAND(-,-,-) */
  NAND4,			/* NAND(-,-,-,-) */
  AOI21,			/* INV(OR(-, AND(-, -)) */
  AOI22				/* INV(OR(AND(-,-), AND(-,-) */
} Type;

/* Data structure for combinational logic trees:
   A binary tree with internal nodes that represent logic gates.
   There are no special nodes for primary inputs; these represented
   by the empty tree, NULL.
*/
typedef struct node* Tree;
struct node {
  Type type;		/* logic gate type */
  Area area;		/* total area of this subtree when mapped */
  int cell;		/* mapped to cell at this index in libcells table */
  Tree left;		/* left subtree */
  Tree right;		/* right subtree, NULL for INV/NOT */
};

/* ------------------------------------------------------------------------ */
/* EXPORTED VARIABLES							    */
/* ------------------------------------------------------------------------ */

/* Translates types to character string descriptions. */
extern const char *gate_names[];

/* ------------------------------------------------------------------------ */
/* FUNCTION PROTOTYPES							    */
/* ------------------------------------------------------------------------ */

/* Returns the tree constructed by creating a fresh root node of type
   `type' and giving it the children `left' and `right'.
   (You might prefer to use the specialized constructor macros instead.)
*/
Tree tree_mk(Type type, Tree left, Tree right);

/* Prints tree `t' to stdout in infix notation. */
void tree_print(Tree t);

/* Frees the tree `t'. No action for NULL argument. */
void tree_free(Tree t);

#ifdef __cplusplus
}
#endif

#endif /* TREE_H */
