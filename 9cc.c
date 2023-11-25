


//	再帰下降構文解析法 recursive descent parsing
//	32ビット用



#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>



// トークンの種類 TokenKind										//TAG?JUMP_MARK
typedef enum {

	TK_RESERVED,	// 記号
	TK_NUM,			// 整数
	TK_EOF			// 入力の終わり

} TokenKind;

typedef	struct Token	Token;

// トークン型 Token												//TAG_JUMP_MARK
struct Token {

	TokenKind	kind;	// トークンの型
	Token *		next;	// 次の入力トークン
	long		val;	// kindがTK_NUMの時
	char *		str;	// トークン文字列

};

// 現在着目しているトークン
Token *	token;



// エラーを報告するための関数 error()							//TAG_JUMP_MARK
//	printf()と同じ引数。
void error ( int nErrCode, int nSubCode, char * fmt, ... )
{

	va_list	ap;
	va_start( ap, fmt );

	fprintf( stderr, "error %d (%d) : ", nErrCode, nSubCode );
	vfprintf( stderr, fmt, ap );
	fprintf( stderr, "\n" );
	exit( nErrCode );

}
//void error ( char * fmt, ... )



// consume()													//TAG_JUMP_MARK
//	次のトークンtokenが期待している記号の時は真を返す。でなければ偽を返す。
//	トークンを読み進める。
bool consume ( char op )
{

	if(  ( token->kind != TK_RESERVED )||( token->str[0] != op )  )
		return false;

	token = token->next;	// 次のトークンへ

	return true;

}
//bool consume ( char op )



// expect()														//TAG_JUMP_MARK
//	次のトークンtokenが期待している記号の時はトークンを１つ読み進める。
//	でなければエラーを報告する。
void expect ( char op, int nErrCode )
{

	if(  ( token->kind != TK_RESERVED )||( token->str[0] != op )  )
		error( nErrCode, 100, "'%c'ではありません。", op );

	token = token->next;	// 次のトークンへ

}
//void expect ( char op, int nErrCode )



// expect_number()												//TAG_JUMP_MARK
//	次のトークンtokenが数値の場合、トークンを１つ読み進めて、その数値を返す。
//	でなければエラーを報告する。
long expect_number ( int nErrCode )
{

	if( token->kind != TK_NUM )
		error( nErrCode, 100, "数値ではありません。" );

	long	val = token->val;
	token = token->next;	// 次のトークンへ

	return val;

}
//int expect_number ( int nErrCode )



// at_eof()														//TAG_JUMP_MARK
//	次のトークンtokenが入力トークンの終わりなら真を返す。
bool at_eof ()
{

	return token->kind == TK_EOF ;

}
//bool at_eof ()



// new_token()													//TAG_JUMP_MARK
//	新しいトークンを作成して、していたトークンcurの後に繋げる。
//	作成したトークンのアドレスを返す。
//	呼び出し元がメモリを開放する。
Token * new_token ( TokenKind kind, Token * cur, char * str )
{

	Token * tok = (Token*)calloc( 1, sizeof(Token) );	// ゼロクリアする
	if( tok == NULL )	return NULL;

	tok->kind = kind;
	tok->str = str;

	// curの次に作成したトークンを繋げる。
	cur->next = tok;

	return tok;

}
//Token * new_token ( TokenKind kind, Token * cur, char * str )



// delete_list()												//TAG_JUMP_MARK
void delete_list ( Token * pList )
{

	Token *	p = pList;
	while( p != NULL ){
		Token *	mp = p->next;
		free( p );
		p = mp;
	}

}
//void delete_list ( Token * pList )



// tokenize()													//TAG_JUMP_MARK
//	指定した文字列をトークンに分割してリストを作成し、
//	リストの先頭のアドレスを返す。
Token * tokenize ( char * pStr, int nErrCode )
{

	char *	p = pStr;

	Token	head;		// リストの先頭アドレス確保用
	head.next = NULL;

	Token *	cur = &head;	// 現在の最後尾の要素

	if( pStr == NULL )		return NULL;

	while ( *p ) {

		// 空白文字をスキップ
		if( isspace( *p ) ) {  ++p;  continue;  }

		// + or -
		if(  ( *p == '+' )||( *p == '-' )  ){
			cur = new_token( TK_RESERVED, cur, p++ );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 100, "メモリ不足です。" );
			}
			continue;
		}

		// 数値
		if( isdigit( *p ) ) {
			cur = new_token( TK_NUM, cur, p );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 200, "メモリ不足です。" );
			}
			cur->val = strtol( p, &p, 10 );	// pには次のアドレスが入る
			continue;
		}

		error( nErrCode, 100, "文法が違います。(%dバイト目)", p-pStr );

	}// while *p

	// 終端
	cur = new_token( TK_EOF, cur, p );
	if( cur == NULL ){
		delete_list( head.next );
		error( nErrCode, 900, "メモリ不足です。" );
	}

	return head.next;

}
//Token * tokenize ( char * pStr, int nErrCode )



int main ( int argc, char **argv )
{

	if( argc !=2 ){
		fprintf( stderr, "引数の個数が正しくありません。\n" );
		return 100;
	}

	// 引数からトークンのリストを作る
	Token *	mToken = token = tokenize( argv[1], 1000 );

	printf( "\n\n\n" );

	printf( ".intel_syntax noprefix\n" );

	printf( "\n" );

	printf( ".global main\n" );

	printf( "\n\n\n" );

	printf( "main:\n" );

	printf( "\n" );

	// 士気の最初は数値でなければならないので、それをチェックしつつ出力
	printf(  "    mov     rax, %ld\n",  expect_number( 2100 )  );

	while( ! at_eof() ){

		if( consume( '+' ) ){
			printf(  "    add     rax, %ld\n",  expect_number( 2200 )  );
		}

		expect( '-', 2300 );
		printf(  "    sub     rax, %ld\n",  expect_number( 2400 )  );

	} //while *pCmd

	printf( "\n" );

	printf( "    ret\n" );

	printf( "\n\n\n" );

	delete_list( mToken );

	return 0;

}



