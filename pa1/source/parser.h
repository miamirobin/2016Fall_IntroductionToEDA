/* Copyright (c) 2002 ACM/SIGDA */

#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

#if defined __cplusplus
extern "C" {
#endif

/* Syntax for specifying a logic expression.

   White-space (blank, tab, newline) has no semantic meaning and may be used
   in any quantity to enhance the readability. The same holds for comments
   that start with the hash character '#' and extend to the end of the line.

   The Scheffer stroke "|" is the NAND operator. It binds just as strong
   as the AND-operator. The AND-operator '&' binds stronger than the OR-
   operator '+'. All are commutative and associative. Note that the
   AND-operator is optional and may thus also be denoted by juxtaposition
   of Factors. The NOT-operator '!' has the highest precedence.

	Input       : Expression ";" .

	Expression  : Term [ "+" Expression ] .

	Term        : Factor [ ( [ "&" ] | "|" ) Term ] .

	Factor      : Primary | "!" Factor .

	Primary     : Literal | "(" Expression ")" .

	Literal     : Variable [ "'" ] .

	Variable    : "A" | "B" | "C" | ... | 'Z'
	            | "a" | "b" | "c" | ... | 'z' .
*/

/* Reads a logic expression from stdin and constructs a tree
   using the functions and macros defined in the file `tree.h'.
   The accepted input format is defined above. Note that variables merely
   act as place-holders for primary inputs. They do not appear in the
   parse tree: at the position of a variable there will be an empty tree,
   i.e., the NULL pointer value.

   Aborts (with exit(1)) whenever a syntax error occurs.
*/
extern Tree parse(void);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
