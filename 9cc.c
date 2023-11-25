


#include	<stdio.h>
#include	<stdlib.h>



int main ( int argc, char ** argv )
{

	if( argc != 2 ){
		fprintf( stderr, "引数の個数が正しくありません。\n" );
		return 100;
	}

	printf( "\n\n\n" );

	printf( ".intel_syntax noprefix\n" );

	printf( "\n" );

	printf( ".global main\n" );

	printf( "\n\n\n" );

	printf( "main:\n" );

	printf( "\n" );

	char *	pCmd = argv[1];

	printf( "    mov     rax, %ld\n", strtol( pCmd, &pCmd, 10 ) );

	while( *pCmd ){
	
		switch( *pCmd ){
		case '+':
			++pCmd;
			printf( "    add     rax, %ld\n", strtol( pCmd, &pCmd, 10 ) );
			break;
		case '-':
			++pCmd;
			printf( "    sub     rax, %ld\n", strtol( pCmd, &pCmd, 10 ) );
			break;
		default:
			fprintf( stderr, "予期しない文字です。: '%c'\n", *pCmd );
			return 1100;
		}// switch *pCmd

	}// while *pCmd

	printf( "\n" );

	printf( "    ret\n" );

	printf( "\n\n\n" );
	
	return 0;

}




