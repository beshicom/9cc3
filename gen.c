




// 構文木からアセンブラコードを生成
//	機械的にスタックマシンとしてのコードを生成。


#include	"gen.h"



// 構文木からアセンブラコードを生成する gen()		//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int gen ( Node * node, int nDepth )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	if( node->kind == ND_NUM ){
		Indent0( nDepth );
		printf( "    push    %d\n", node->val );
		return 0;
	}

	// 左辺値の生成
	//Indent0( nDepth+1 );
	//printf( "    ; left hand side\n" );
	nLastError = gen( node->lhs, nDepth+1 );
	if( nLastError != 0 )	return nLastError;

	// 右辺値の生成
	//Indent0( nDepth+1 );
	//printf( "    ; right hand side\n" );
	nLastError = gen( node->rhs, nDepth+1 );
	if( nLastError != 0 )	return nLastError;

	// 計算コードの生成
	Indent0( nDepth );
	printf( "    pop     rdi\n" );
	Indent0( nDepth );
	printf( "    pop     rax\n" );

	switch( node->kind ){
	case ND_ADD:
		Indent0( nDepth );
		printf( "    add     rax, rdi\n" );
		break;
	case ND_SUB:
		Indent0( nDepth );
		printf( "    sub     rax, rdi\n" );
		break;
	case ND_MUL:
		Indent0( nDepth );
		printf( "    imul    rax, rdi\n" );
		break;
	case ND_DIV:
		Indent0( nDepth );
		printf( "    cqo\n" );
			// raxを128ビット値に拡張し、上位64ビットをrdxへ入れる。
		Indent0( nDepth );
		printf( "    idiv    rdi\n" );
			//rdx rax を128ビット整数とみなしてrdiで符号付き除算をして
			//	商をrax、余りをrdxに入れる。
		break;
	case ND_SML:	// <
		Indent0( nDepth );
		printf( "    cmp     rax, rdi\n" );
		printf( "    setl    al\n" );
							// 直前のcmpが < の時にalに1をセット
		printf( "    movzb   rax, al\n" );
							// 上位ビットをゼロに
		break;
	case ND_BIG:	// >  ->  ! <=
		Indent0( nDepth );
		printf( "    cmp     rax, rdi\n" );
		printf( "    setle   al\n" );
							// 直前のcmpが <= の時にalに1をセット
		printf( "    movzb   rax, al\n" );
							// 上位ビットをゼロに
		printf( "    xor     rax, 1\n" );
		break;
	case ND_SME:	// <=
		Indent0( nDepth );
		printf( "    cmp     rax, rdi\n" );
		printf( "    setle   al\n" );
							// 直前のcmpが <= の時にalに1をセット
		printf( "    movzb   rax, al\n" );
							// 上位ビットをゼロに
		break;
	case ND_BGE:	// >=  ->  ! <
		Indent0( nDepth );
		printf( "    cmp     rax, rdi\n" );
		printf( "    setl    al\n" );
							// 直前のcmpが < の時にalに1をセット
		printf( "    movzb   rax, al\n" );
							// 上位ビットをゼロに
		printf( "    xor     rax, 1\n" );
		break;
	case ND_EQU:
		Indent0( nDepth );
		printf( "    cmp     rax, rdi\n" );
		printf( "    sete    al\n" );
							// 直前のcmpが等しい時にalに1をセット
		printf( "    movzb   rax, al\n" );
							// 上位ビットをゼロに
		break;
	case ND_NEQ:
		Indent0( nDepth );
		printf( "    cmp     rax, rdi\n" );
		printf( "    setne    al\n" );
							// 直前のcmpが等しくない時にalに1
		printf( "    movzb   rax, al\n" );
							// 上位ビットをゼロに
		break;
	}// switch

	// 計算結果をスタックへ積む。
	Indent0( nDepth );
	printf( "    push    rax\n" );
	printf( "\n\n" );

	return 0;

}
//int gen ( Node * node )



// 構文木からアセンブラコードを生成する gen()					//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int GenAsm ( Node * node )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	printf( "\n\n\n" );
	printf( ".intel_syntax noprefix\n" );
	printf( "\n" );
	printf( ".global main\n" );
	printf( "\n\n\n" );
	printf( "main:\n" );
	printf( "\n\n\n" );

	nLastError = gen( node, 0 );

	printf( "    pop     rax\n" );
	printf( "    ret\n" );
	printf( "\n\n\n" );

	return nLastError;

}



// end of this file : gen.c
