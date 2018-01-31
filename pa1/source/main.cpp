/* Copyright (c) 2002 U Wisc. ECE 756*/
/* The source code is originally downloaded from ECE 756 website */
// b03208022  李友岐
#define DEBUG
#include<iostream>
#include "parser.h"
using namespace std;
/* --------------------------------------------------------------------------*/
/*				 Cell Library (pattern graph)		     */
/* --------------------------------------------------------------------------*/

/* Entry of library cell table. */
typedef struct {
  Type type;			/* logic gate type, e.g. AOI21 */
  Tree g;			/* pattern: normalized tree (NAND2/INV) */
  Area area;			/* chip area footprint */
} Libcell;

/* The library cells table, 1 normalized pattern per entry.
   Pattern trees (now NULL) still need to be filled in!
*/
static Libcell libcells[] = {

  {   INV, NULL, 2 },
  { NAND2, NULL, 3 },
  { NAND3, NULL, 4 }, /* Has 2 different topologies. */
  { NAND3, NULL, 4 },
  { NAND4, NULL, 5 }, /* Has 5 different topologies. */
  { NAND4, NULL, 5 },
  { NAND4, NULL, 5 },
  { NAND4, NULL, 5 },
  { NAND4, NULL, 5 },
  { AOI21, NULL, 4 }, /* Has 2 different topologies. */
  { AOI21, NULL, 4 },
  { AOI22, NULL, 5 }
};

/* The size of the libcells table (nr. of entries). */
static int nr_libcells = sizeof(libcells) / sizeof(libcells[0]);

/* Fills in the pattern trees into the library cell table. */
static void
tree_libcells_init(void)
{
  int i = 0;

#define x		NULL
#define INV(a)		tree_mk_inv(a)
#define ND2(a,b)	tree_mk_nand2(a,b)

  /* INV: */
  libcells[i++].g = INV(x);
  /* NAND2: */
  libcells[i++].g = ND2(x,x);
  /* NAND3: */
  libcells[i++].g = ND2(INV(ND2(x,x)),x);
  libcells[i++].g = ND2(x,INV(ND2(x,x)));
  /* NAND4: */
  libcells[i++].g = ND2(INV(ND2(INV(ND2(x,x)),x)),x);
  libcells[i++].g = ND2(INV(ND2(x,INV(ND2(x,x)))),x);
  libcells[i++].g = ND2(x,INV(ND2(INV(ND2(x,x)),x)));
  libcells[i++].g = ND2(x,INV(ND2(x,INV(ND2(x,x)))));
  libcells[i++].g = ND2(INV(ND2(x,x)),INV(ND2(x,x)));
  /* AOI21: */
  libcells[i++].g = INV(ND2(ND2(x,x),INV(x)));
  libcells[i++].g = INV(ND2(INV(x), ND2(x,x)));
  /* AOI22: */
  libcells[i++].g = INV(ND2(ND2(x,x),ND2(x,x)));

#undef x
#undef INV
#undef ND2
}

/* --------------------------------------------------------------------------*/
/*				 Subject Graph (normalization)		     */
/* --------------------------------------------------------------------------*/

/* Inverts t but avoids double inverters. */
static Tree
tree_mk_INV(Tree t)
{
  if (t != NULL && T_TYPE(t) == INV) {
    Tree n = t;

    t = T_LEFT(t);
    T_LEFT(n) = NULL;
    tree_free(n);
    return t;
  }
  return tree_mk_inv(t);
}

/* Normalizes tree t by reexpressing its Boolean function in terms of
   NAND2 and INV gates only. Returns the newly constructed tree.
   Does NOT consider permutations of NAND2 inputs; expects library cells
   to be dependent on this to have multiple representations.
*/
static Tree
tree_normalize(Tree t)
{
  Tree L, R;

  if (t == NULL)
    return t;

  switch (T_TYPE(t)) {
  case NOT:			/* => INV(-) */
    L = tree_normalize(T_LEFT(t));
    return tree_mk_INV(L);

  case AND:			/* => INV(NAND2(-,-)) */
    L = tree_normalize(T_LEFT(t));
    R = tree_normalize(T_RIGHT(t));
    return tree_mk_inv(tree_mk_nand2(L, R));

  case NAND:			/* => NAND2(-,-) */
    L = tree_normalize(T_LEFT(t));
    R = tree_normalize(T_RIGHT(t));
    return tree_mk_nand2(L, R);

  case OR:			/* => NAND2(INV(-),INV(-)) */
    L = tree_normalize(T_LEFT(t));
    R = tree_normalize(T_RIGHT(t));
    return tree_mk_nand2(tree_mk_INV(L), tree_mk_INV(R));

  default:
    assert(1);
          return NULL;
  }
}

/* --------------------------------------------------------------------------*/
/*			    Tree Mapper (filled by you)			     */
/* --------------------------------------------------------------------------*/

/* Tries to match library cell tree g with circuit tree t.
   (Expects normalized trees.) If sucessfully finds a match, then
   adds total area of subtrees induced by the library cell match to *area.

   Returns 1 when successful, 0 otherwise.
*/
// R06921048  李友岐
static int
tree_match(Tree t, Tree g, Area *area)
{
    if (g == NULL){               // 先看看library cell tree g是不是在leaf node,如果是的話再看circuit tree t是不是也在leaf node.
        if (t==NULL){                 // 如果circuit tree t也在leaf node的話表示match是剛開始．
        	return 1;                  // 所以先回傳1
        }
        else {                        // 如果circuit tree t不在leaf node的話
            *area+=T_AREA(t);             //  就把subtree的面積加進總面積
            return 1;                    //  然後回傳1
        }
    }
    else {                        // 如果library cell tree g 不在leaf node的話
    
        if (t==NULL){                 // 看看circuit tree g是不是在leaf node,是的話表示match失敗．
           return 0;                   // 所以回傳0
        }
        if (T_TYPE(g)!=T_TYPE(t)){	 // 再看看library cell tree g與circuit tree t的type相不相同，不同的話表示match失敗
           return 0;                   // 所以回傳0
        }
        if ((tree_match(T_LEFT(t), T_LEFT(g), area) != 1) || (tree_match(T_RIGHT(t), T_RIGHT(g), area) != 1)){
            
                                     // 再看看circuit tree t與librry cell tree g的左右子樹配對有無成功,
                                     // 左邊或右邊其中一邊沒有match的話,就表示match失敗.
            return 0;                   // 所以回傳0
        }
        else{						 // 如果左右兩邊都match成功的話
           return 1;                   // 就回傳1
       }
    }
}

/* Finds minimum area mapping of circuit t on to given library.
 Annotates each node of t with best matched library cell (index).
 Returns area for t.
 */
Area tree_mapper(Tree t)
{
    int j_m=0;                       // 宣告變數 j_m =0
    Area area_min=0;                 // 宣告變數 最小面積area_min=0
    if (t==NULL){                     // 看看 circuit tree t是否在leaf node
      return 0;                        // 是的話面積回傳0
    }
    else{                             // circuit tree t不在leaf node的話就利用遞迴
       tree_mapper(T_RIGHT(t));          // 右子樹mapping
       tree_mapper(T_LEFT(t));           // 左子樹mapping
    }
    int j=0;                         // 宣告變數 j=0
    int nr=nr_libcells;              // 宣告變數 nr = (libecells的數量)
    while (j<nr){                     // 當j<nr時迴圈就持續進行,每個libcell都試過後即停止
        Area area_t=libcells[j].area;    // 宣告變數 area_t = (該libcell的面積)
        if((tree_match(t, libcells[j].g, &area_t)==1) && ((area_min>=area_t) || (area_min==0))){
            
                                        // 如果tree t與該libcell配對成功,而且aree_t面積比原本的最小面積還小或最小面積原本為零的話
            j_m=j;                         // 將j的值assign到j_m
            area_min=area_t;                // 將area_t的值assign到area_min,最小面積有了新的值.
            
        }
        j+=1;                           // 執行完一次迴圈就將j加1,看看下個libcell.
    }
    
    
    cout<<"Tree node: "<<gate_names[T_TYPE(t)]<<", area: "<<area_min<<", cell: "<< gate_names[libcells[j_m].type]<<endl;
    
                                        // 將DAGON過程顯示出來
    T_CELL(t) = j_m;                    // 將j_m的值assign到T_CELL(t)
    T_AREA(t) = area_min;               // 將最小面積的值assign到T_AREA(t)
    return T_AREA(t);                   // 回傳最小面積之值
}


/* --------------------------------------------------------------------------*/
/*			    Main					     */
/* --------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
  Tree s, t;

  if (argc > 1 && argv[1])
    freopen(argv[1], "r", stdin);

  tree_libcells_init();

  t = parse();

#ifdef DEBUG
//  printf("Input tree circuit: ");
//  tree_print(t);
//  printf("\n");
#endif

  t = tree_normalize(s = t);
  tree_free(s);

#ifdef DEBUG
  printf("Normalized tree   : ");
  tree_print(t);
  printf("\n");
#endif

  printf("%d\n", tree_mapper(t));

  tree_free(t);

  return EXIT_SUCCESS;
}


