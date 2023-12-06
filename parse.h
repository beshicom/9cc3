


// 再帰下降構文解析



#include	<stdio.h>
#include	<malloc.h>

#include	"object.h"
#include	"token.h"



#ifndef	B__PARSE_H__B
#define	B__PARSE_H__B



// 抽象構文木のノードの種類 NodeKind			//TAG_JUMP_MARK
typedef enum
{

	ND_NUM,		// 整数

	ND_ADD,		// +
	ND_SUB,		// -

	ND_MUL,		// *
	ND_DIV,		// /

	ND_SML,		// <
	ND_SME,		// <=
	ND_BIG,		// >
	ND_BGE,		// >=

	ND_EQU,		// ==
	ND_NEQ,		// !=
	ND_END		// (終端子)

} NodeKind ;



typedef	struct Node	Node;

// 抽象構文木のノードの型 Node					//TAG_JUMP_MARK
struct Node
{

	NodeKind	kind;		// ノードの型
	Node *		lhs;		// 左辺 left hand side
	Node *		rhs;		// 右辺 roight hand side
	int			val;		// kind==ND_NUMの時のみ使う

	int			nErr;		// エラーが発生したらnot0
	int			nBlockID;	// メモリーブロックの通し番号

};

extern Node *	NodeTop;	// 構文木の先頭

extern int		nNumBlock;	// メモリーブロックの総数



// ト－クンのリストをもとに、構文木を作成 Parse()	//TAG_JUMP_MARK
//	グローバル変数 token : トークンリスト
int Parse ();



// 構文木をプリントする Print()					//TAG_JUMP_MARK
//	グローバル変数 NodeTop : 構文木
int Print ();



int Calc ( Node * node );                       //TAG_JUMP_MARK
void delete_tree ( Node * pNode );				//TAG_JU<P_MARK



#endif



// end of this file : parse.h

