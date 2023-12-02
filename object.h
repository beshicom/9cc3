


// ユーティリティ



#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>



#ifndef	B__OBJECT_H__B
#define	B__OBJECT_H__B



extern int		nLastError;	// 直近のエラーコード



// n個のスペースを表示 Indent()					//TAG_JUMP_MARK
void Indent ( int n );



// n個のスペースを表示 Indent0()				//TAG_JUMP_MARK
void Indent0 ( int n );



// キーボードから１行を入力 getline()			//TAG_JUMP_MARK
//int getline ( char * pBuf, int nChar );



#endif



// end of this file : object.h
