#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () {
	// Alocação Inicial: Controle (16) + Rascunho (512) = 528 bytes
	void *pBuffer = malloc ( ( sizeof ( int ) * 4 ) + 512 );

	if ( pBuffer == NULL ) {
		printf ( "Erro ao alocar pBuffer.\n" );
		return 1;
	}

	// PONTEIROS DE CONTROLE
	int *quantidade = ( int * ) pBuffer;
	int *menu = ( int * ) ( pBuffer + sizeof ( int ) );
	int *indice = ( int * ) ( pBuffer + sizeof ( int ) * 2 );
	int *posicaoLivre = ( int * ) ( pBuffer + sizeof ( int ) * 3 );

	// INICIALIZAÇÃO
	*quantidade = 0;
	*posicaoLivre = ( sizeof ( int ) * 4 ) + 512;

	do {
		printf ( "\n========== AGENDA DINAMICA ==========\n" );
		printf ( "1 - Adicionar Pessoa\n" );
		printf ( "2 - Remover Pessoa\n" );
		printf ( "3 - Buscar Pessoa\n" );
		printf ( "4 - Listar Todos\n" );
		printf ( "5 - Sair\n" );
		printf ( "Escolha: " );

		if ( scanf ( "%d", menu ) != 1 ) {
			printf ( "Entrada invalida.\n" );
			break;
		}
		getchar ( );

		switch ( *menu ) {
			case 1: {
				char *bufferTemp = ( char * ) pBuffer + ( sizeof ( int ) * 4 );
				char *nome = bufferTemp;
				int *idade = ( int * ) ( bufferTemp + 256 );
				char *email = bufferTemp + 260;
				int *tamanhoNome = ( int * ) ( bufferTemp + 500 );
				int *tamanhoEmail = ( int * ) ( bufferTemp + 504 );
				int *tamanhoRegistro = ( int * ) ( bufferTemp + 508 );

				printf ( "Nome: " );
				scanf ( " %[^\n]", nome );

				printf ( "Idade: " );
				scanf ( "%d", idade );
				getchar ( );

				printf ( "Email: " );
				scanf ( " %[^\n]", email );

				*tamanhoNome = strlen ( nome ) + 1;
				*tamanhoEmail = strlen ( email ) + 1;
				*tamanhoRegistro = sizeof ( int ) + *tamanhoNome + sizeof ( int ) + sizeof ( int ) + *tamanhoEmail;

				pBuffer = realloc ( pBuffer, *posicaoLivre + *tamanhoRegistro );

				if ( pBuffer == NULL ) {
					printf ( "Erro de memoria.\n" );
					return 1;
				}

				quantidade = ( int * ) pBuffer;
				menu = ( int * ) ( ( char * ) pBuffer + sizeof ( int ) );
				indice = ( int * ) ( ( char * ) pBuffer + sizeof ( int ) * 2 );
				posicaoLivre = ( int * ) ( ( char * ) pBuffer + sizeof ( int ) * 3 );
				bufferTemp = ( char * ) pBuffer + ( sizeof ( int ) * 4 );
				nome = bufferTemp;
				idade = ( int * ) ( bufferTemp + 256 );
				email = bufferTemp + 260;
				tamanhoNome = ( int * ) ( bufferTemp + 500 );
				tamanhoEmail = ( int * ) ( bufferTemp + 504 );
				tamanhoRegistro = ( int * ) ( bufferTemp + 508 );

				char *pNav = ( char * ) pBuffer + *posicaoLivre;
				*( ( int * ) pNav ) = *tamanhoNome;
				pNav += sizeof ( int );
				memcpy ( pNav, nome, *tamanhoNome );
				pNav += *tamanhoNome;
				*( ( int * ) pNav ) = *idade;
				pNav += sizeof ( int );
				*( ( int * ) pNav ) = *tamanhoEmail;
				pNav += sizeof ( int );
				memcpy ( pNav, email, *tamanhoEmail );
				pNav += *tamanhoEmail;

				( *quantidade )++;
				*posicaoLivre += *tamanhoRegistro;

				printf ( "\nPessoa adicionada com sucesso!\n" );
				break;
			} case 2: {
				if ( *quantidade == 0 ) {
					printf ( "\nAgenda vazia.\n" );
					break;
				}

				char *temp = ( char * ) pBuffer + 16;
				char *nomeBusca = temp;
				char *emailBusca = temp + 250;
				int *tamNome = ( int * ) ( temp + 500 );
				int *tamEmail = ( int * ) ( temp + 504 );
				int *tamRegistro = ( int * ) ( temp + 508 );
				int *bytesRestantes = ( int * ) ( temp + 496 ); //guarda quantos bytes existem depois do registro
				int *encontrado = ( int * ) ( temp + 492 ); // especie de flag pro if

				*encontrado = 0;

				printf ( "Nome para remover: " );
				scanf ( " %[^\n]", nomeBusca );

				printf ( "Email para remover: " );
				scanf ( " %[^\n]", emailBusca );

				char *pNav = ( char * ) pBuffer + ( ( sizeof ( int ) * 4 ) + 512 ); // nesse vai pro comeco dos registros

				for ( *indice = 0; *indice < *quantidade; ( *indice )++ ) {
					char *inicio = pNav;
					*tamNome = *( ( int * ) pNav );
					pNav += sizeof ( int );
					char *nome = pNav;
					pNav += *tamNome;
					pNav += sizeof ( int );
					*tamEmail = *( ( int * ) pNav );
					pNav += sizeof ( int );
					char *email = pNav;
					pNav += *tamEmail;
					*tamRegistro = ( int ) ( pNav - inicio );

					if ( strcmp ( nome, nomeBusca ) == 0 && strcmp ( email, emailBusca ) == 0 ) {
						*encontrado = 1;
						*bytesRestantes = ( int ) ( ( ( char * ) pBuffer + *posicaoLivre ) - pNav );
						memmove ( inicio, pNav, *bytesRestantes ); //Remove o registro deslocando memória
						( *quantidade )--;
						*posicaoLivre -= *tamRegistro;

						pBuffer = realloc ( pBuffer, *posicaoLivre );

						if ( pBuffer == NULL ) {
							printf ( "Erro de memoria.\n" );
							return 1;
						}

						quantidade = ( int * ) pBuffer;
						menu = ( int * ) ( ( char * ) pBuffer + 4 );
						indice = ( int * ) ( ( char * ) pBuffer + 8 );
						posicaoLivre = ( int * ) ( ( char * ) pBuffer + 12 );

						printf ( "Pessoa removida!\n" );
						break;
					}
				}

				if ( *encontrado == 0 ) {
					printf ( "Pessoa nao encontrada.\n" );
				}

				break;
			} case 3: {
				if ( *quantidade == 0 ) {
					printf ( "\nAgenda vazia.\n" );
					break;
				}

				char *bufferTemp = ( char * ) pBuffer + ( sizeof ( int ) * 4 );
				char *pNav = ( char * ) pBuffer + ( ( sizeof ( int ) * 4 ) + 512 );//comeca no inicio dos registros
				int *encontrado = ( int * ) ( ( char * ) pBuffer + 492 );
				int *tamNome = ( int * ) ( bufferTemp + 500 );
				int *tamEmail = ( int * ) ( bufferTemp + 504 );

				*encontrado = 0;

				printf ( "Digite o nome para buscar: " );
				scanf ( " %[^\n]", bufferTemp );

				for ( *indice = 0; *indice < *quantidade; ( *indice )++ ) {
					*tamNome = *( ( int * ) pNav );
					pNav += sizeof ( int );
					char *nome = pNav;
					pNav += *tamNome;
					int *idade = ( int * ) pNav;
					pNav += sizeof ( int );
					*tamEmail = *( ( int * ) pNav );
					pNav += sizeof ( int );
					char *email = pNav;
					pNav += *tamEmail;

					if ( strcmp ( nome, bufferTemp ) == 0 ) {
						printf ( "\n===== PESSOA ENCONTRADA =====\n" );
						printf ( "Nome: %s\n", nome );
						printf ( "Idade: %d\n", *idade );
						printf ( "Email: %s\n", email );
						printf ( "=============================\n" );
						*encontrado = 1;
					}
				}

				if ( *encontrado == 0 ) {
					printf ( "\nPessoa nao encontrada.\n" );
				}

				break;
			} case 4: {
				if ( *quantidade == 0 ) {
					printf ( "\nAgenda vazia.\n" );
					break;
				}

				char *pNav = ( char * ) pBuffer + ( ( sizeof ( int ) * 4 ) + 512 ); // incio do registrador

				printf ( "\n======= LISTA DE PESSOAS =======\n" );

				for ( *indice = 0; *indice < *quantidade; ( *indice )++ ) {
					int *tamNome = ( int * ) pNav;
					pNav += sizeof ( int );
					char *nome = pNav;
					pNav += *tamNome;

					int *idade = ( int * ) pNav;
					pNav += sizeof ( int );

					int *tamEmail = ( int * ) pNav;
					pNav += sizeof ( int );
					char *email = pNav;
					pNav += *tamEmail;

					printf ( "\nPessoa %d\n", *indice + 1 );
					printf ( "Nome: %s\n", nome );
					printf ( "Idade: %d\n", *idade );
					printf ( "Email: %s\n", email );
				}

				printf ( "\n===============================\n" );
				break;
			} case 5: {
				printf ( "Encerrando programa...\n" );
				break;
			} default: {
				printf ( "Opcao invalida. Tente novamente.\n" );
				break;
			}
		}
	} while ( *menu != 5 );

	free ( pBuffer );
	return 0;
}