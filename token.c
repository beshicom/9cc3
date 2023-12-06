


// 文字列をトークンに分ける



#include	"token.h"



char *	user_input;		// 処理するプログラムの文字列全体の先頭



// 現在着目しているトークン
Token *	token;



// エラーを報告するための関数 error()	    	//TAG_JUMP_MARK
//	printf()と同じ引数。
int error ( int nErrCode, int nSubCode, char * fmt, ... )
{

	va_list	ap;
	va_start( ap, fmt );

	fprintf( stderr, "error %d (%d) : ", nErrCode, nSubCode );
	vfprintf( stderr, fmt, ap );
	fprintf( stderr, "\n" );
	exit( nErrCode );

	nLastError = nErrCode;

	return nErrCode;

}
//int error ( char * fmt, ... )



// エラーを報告するための関数 error_at()			//TAG_JUMP_MARK
//	printf()と同じ引数。
int error_at( int nErrCode, int nSubCode,
                            char * loc, char * fmt, ... )
{

	va_list		ap;
	va_start( ap, fmt );

	int	pos = loc - user_input;

	// 1+2-a
	//     ^ 文法が違います

	fprintf( stderr, "%s\n", user_input );
	fprintf( stderr, "%*s", pos, " " );	// pos個のスペース
	fprintf( stderr, "^ " );

	char	buf[ 100 ];
	vsprintf( buf, fmt, ap );
	strcat( buf, "\n" );

	fprintf( stderr, buf );

	error ( nErrCode, nSubCode, buf );

	nLastError = nErrCode;

	return nErrCode;

}
//int error_at( int nErrCode, int nSubCode,
//						char *p loc, char * fmt, ... )



// consume()								//TAG_JUMP_MARK
//	次のトークンtokenが期待している記号の時は真を返す。
//										でなければ偽を返す。
//	トークンを読み進める。
bool consume ( char * op )
{

	nLastError = 0;

	if( op == NULL ){
		nLastError = 3;
		return false;
	}

	if(
		( token->kind != TK_RESERVED )||
		( strlen(op) != token->len )||
		memcmp( token->str, op, token->len )
	){
		nLastError = 1;
		return false;
	}

	token = token->next;	// 次のトークンへ

	return true;

}
//bool consume ( char * op )



// expect()										//TAG_JUMP_MARK
//	次のトークンtokenが期待している記号の時は
//								トークンを１つ読み進める。
//	でなければエラーを報告する。
int expect ( char * op, int nErrCode )
{

	nLastError = 0;

	if(
		( token->kind != TK_RESERVED )||
		( strlen( op ) != token->len )||
		memcmp( token->str, op, token->len )
	){
		error_at( nErrCode, 100, token->str,
								"'%s'ではありません。", op );
		nLastError = nErrCode;
		return nErrCode;
	}

	token = token->next;	// 次のトークンへ

	return 0;


}
//int expect ( char * op, int nErrCode )



// expect_number()						//TAG_JUMP_MARK
//	次のトークンtokenが数値の場合、
//				トークンを１つ読み進めて、その数値を返す。
//	でなければエラーを報告する。
int expect_number ( int nErrCode )
{

	nLastError = 0;

	if( token->kind != TK_NUM ){
		error_at( nErrCode, 100, token->str, "数値ではありません。" );
		nLastError = nErrCode;
	}

	int	val = token->val;
	token = token->next;	// 次のトークンへ

	return val;

}
//int expect_number ( int nErrCode )



// at_eof()									//TAG_JUMP_MARK
//	次のトークンtokenが入力トークンの終わりなら真を返す。
bool at_eof ()
{

	nLastError = 0;

	bool	f = ( token->kind == TK_EOF ) ;
	if( f )	nLastError = 1;

	return f;

}
//bool at_eof ()



// new_token()								//TAG_JUMP_MARK
//	新しいトークンを作成して、していたトークンcurの後に繋げる。
//	作成したトークンのアドレスを返す。
//	呼び出し元がメモリを開放する。
Token * new_token ( TokenKind kind, Token * cur, char * str )
{

	nLastError = 0;

	Token * tok = 
        (Token*)calloc( 1, sizeof(Token) );	// ゼロクリアする
	if( tok == NULL ){
		nLastError = 1;
		return NULL;
	}

	tok->kind = kind;  tok->len = 1;
	if( kind == TK_RESERVED2 )
			{  tok->kind = TK_RESERVED;  tok->len = 2;  }

	tok->str = str;

	// curの次に作成したトークンを繋げる。
	cur->next = tok;

	return tok;

}
//Token * new_token ( TokenKind kind, Token * cur, char * str )



// delete_list()							//TAG_JUMP_MARK
//	トークンのリストを全て開放する。
void delete_list ( Token * pList )
{

	nLastError = 0;

	Token *	p = pList;
	while( p != NULL ){
		Token *	mp = p->next;
		free( p );
		p = mp;
	}

}
//void delete_list ( Token * pList )



// tokenize()								//TAG_JUMP_MARK
//	指定した文字列をトークンに分割してリストを作成し、
//	リストの先頭のアドレスを返す。
Token * tokenize ( char * pStr, int nErrCode )
{

	nLastError = 0;

	char *	p = pStr;

	Token	head;		// リストの先頭アドレス確保用
	head.next = NULL;

	Token *	cur = &head;	// 現在の最後尾の要素

	if( pStr == NULL ){
		nLastError = 1;
		return NULL;
	}

	while ( *p ) {

		// 空白文字をスキップ
		if( isspace( *p ) ) {  ++p;  continue;  }

		// ２文字の記号
		{
		char *	key[] = { "==", "!=", "<=", ">=", NULL };
		int		i = 0;
		for( ; key[i] != NULL; ++i ){
			if( memcmp( key[i], p, 2 ) == 0 ){
				cur = new_token( TK_RESERVED2, cur, p );
				p += 2;
				if( cur == NULL ){
					delete_list( head.next );
					error( nErrCode, 300, "メモリ不足です。" );
					nLastError = 300;
					return head.next;
				}// if
				continue;
			}// if
		}// for
		}// end

		// １文字の記号
		{
		char	key[] = "+-*/()<>";
		if( strchr( key, *p ) != NULL ){
			cur = new_token( TK_RESERVED, cur, p++ );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 100, "メモリ不足です。" );
				nLastError = 100;
				return head.next;
			}
			continue;
		}
		}// end

		/*
		// + or -
		if(  ( *p == '+' )||( *p == '-' )  ){
			cur = new_token( TK_RESERVED, cur, p++ );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 100, "メモリ不足です。" );
				nLastError = 100;
				return head.next;
			}
			continue;
		}
		*/

		// 数値
		if( isdigit( *p ) ) {
			cur = new_token( TK_NUM, cur, p );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 200, "メモリ不足です。" );
				nLastError = 200;
				return head.next;
			}
			cur->val = strtol( p, &p, 10 );
									// pには次のアドレスが入る
			continue;
		}

		error_at( nErrCode, 100, p,
					"文法が違います。(%dバイト目)", p-pStr );

	}// while *p

	// 終端
	cur = new_token( TK_EOF, cur, p );
	if( cur == NULL ){
		delete_list( head.next );
		error( nErrCode, 900, "メモリ不足です。" );
		nLastError = 900;
		return head.next;
	}

	return head.next;

}
//Token * tokenize ( char * pStr, int nErrCode )



// end of this file : token.c

