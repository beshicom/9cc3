


//	再帰下降構文解析法 recursive descent parsing
//	64ビット用



#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>

#include	"token.h"
#include	"parse.h"
#include	"gen.h"



// main()									//TAG_JUMP_MARK
int main ( int argc, char **argv )
{

	if(  ( argc != 2 )&&( argc != 3 )  ){
		fprintf( stderr, "引数の個数が正しくありません。\n" );
		fprintf( stderr, "Usage : CCompiler "
									"ソース文字列 [1|2|3]\n" );
		fprintf( stderr, "\t1 (規定値) : "
									"アセンブラソース表示\n" );
		fprintf( stderr, "\t2          : 構文木表示\n" );
		fprintf( stderr, "\t3          : "
						"アセンブラソース表示、構文木表示\n" );
		return 100;
	}

	int		fAsm = 1;
	int		fPrint = 0;
	if( argc == 3 ){
		switch( atoi(argv[2]) ){
		case 1:  fAsm = 1;  fPrint = 0;  break;
		case 2:  fAsm = 0;  fPrint = 1;  break;
		case 3:  fAsm = 1;  fPrint = 1;  break;
		}
	}

	// 引数からトークンのリストを作る
	user_input = argv[1];
	Token *	mToken = token = tokenize( argv[1], 1000 );

	// トークンのリストから構文木を作る
	Parse();

	if( fPrint ){
		printf( "%s = %d\n", argv[1], Calc( NodeTop ) );
		Print();
		printf( "push return -------------" );
		getchar();
		printf( "\n" );
	}

	if( ! fAsm ){
		delete_list( mToken );
		delete_tree( NodeTop );
		return 0;
	}

	// x86_64 アセンブラコードの生成
	GenAsm( NodeTop );

	delete_list( mToken );
	delete_tree( NodeTop );

	return 0;

}



