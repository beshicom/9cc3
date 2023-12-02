


// 構文木からアセンブラコードを生成



#include	<stdio.h>
#include	<malloc.h>

#include	"object.h"
#include	"token.h"
#include	"parse.h"



#ifndef	B__GEN_H__B
#define	B__GEN_H__B



// 構文木からアセンブラコードを生成する gen()					//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int gen ( Node * node, int nDepth );



// 構文木からアセンブラコードを生成する gen()					//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int GenAsm ( Node * node );



#endif



// end of this file : gen.h
