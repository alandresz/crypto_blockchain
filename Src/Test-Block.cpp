#include<string>
#include <iostream>

#include "Block.h"
#include "BlockChainBuilder.h"
#include "lista.h"
#include "TiposHash.h"
#include "sha256.h"

using namespace std;

bool CheckPreBlock( string valor );
int CheckHexa( string value );
bool CheckHash( string valor, TiposHash Tipo );

extern const string::size_type LargoHashEstandar;  // -> Block.h
extern const string::size_type LargoHashFirma;	   // Hash Pública de la Cuenta definida en Block.h

int main() {
	//int Cola[] = { 3, 4, 5, 47, 19, 39, 21, 34, 54, 97};
	//for (size_t i = 0; i < 10; i++ ) {
	//	cout << Cola[i] << '\n';
	//	lista <int> li;
	//	li.encolar( Cola[i] );
	//}

	//return 0;
	string Hash = "e9dc0f0fbcb9021dc39ec104dfa51e813a86c8205a77d3be6c8cd6140b941e0c";
	string Dir = "f680e0021dcaf15d161604378236937225eeecae85cc3f";

	if ( CheckPreBlock( Hash ) ) {
		cout << "Ok 1" << std::endl;
	}
	
	if ( CheckHash( Hash, TiposHash::clavehash256 ) ) {
		cout << "Ok 2" << std::endl;
	}

	if ( CheckHash( Dir, TiposHash::clavefirma ) ) {
		cout << "Ok 3" << std::endl;
	}

	Dir = "f680e0021dcaf15d161604378236937225eeecae";
	if ( CheckHash( Dir, TiposHash::clavefirma ) ) {
		cout << "Ok 4" << std::endl;
	}

	// Parte nueva
	lista <Block *> ListaBlocks;
	Block * BlocklActual;
	BlocklActual = new Block();
	if ( !BlocklActual->setpre_block( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ) ) return -1;
	if ( !BlocklActual->settxns_hash( "e9dc0f0fbcb9021dc39ec104dfa51e813a86c8205a77d3be6c8cd6140b941e0c" ) ) return -1;
	if ( !BlocklActual->setbits( 3 ) ) return -1;
	ListaBlocks.insertar( BlocklActual );
	BlocklActual = new Block();
	if ( !BlocklActual->setpre_block( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ) ) return -1;
	if ( !BlocklActual->settxns_hash( "cd372fb85148700fa88095e3492d3f9f5beb43e555e5ff26d95f5a6adc36f8e6" ) ) return -1;
	if ( !BlocklActual->setbits( 3 ) ) return -1;
	ListaBlocks.insertar( BlocklActual );
	BlocklActual = new Block();
	if ( !BlocklActual->setpre_block( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" ) ) return -1;
	if ( !BlocklActual->settxns_hash( "155dc94b29dce95bb2f940cdd2d7e0bce66dca9370c3ed96d50a30b3d84f8c4c" ) ) return -1;
	if ( !BlocklActual->setbits( 3 ) ) return -1;
	ListaBlocks.insertar( BlocklActual );
	if ( ! ListaBlocks.vacia() ) {
		lista <Block *>::iterador it;
		/* Itero la lista para recuperar todos los strings de la coleccion Transaction
		   que necesito para calcular el Hash.
		*/
		it = ListaBlocks.primero();
		do  {
			BlocklActual = it.dato();
			size_t contador = 0;
			do {
				std::string resultado = "", nonce, hash_resultado = "";
	
				resultado = BlocklActual->getpre_block() + '\n';          // <- getter que extrae la clave doble hash del Block previo.
				resultado += BlocklActual->getcadenaprehash() + '\n'; 	  // <- getter que extrae el string en la Clase Transaction.
				resultado += BlocklActual->getbits() + '\n'; 
				// BlocklActual->setnonce( BlocklActual->Calculononce() );	// Cada llamada genera un nonce <>, se guarda en Block
				// resultado += BlocklActual->getnonce() + '\n';
				nonce = BlocklActual->Calculononce() + '\n';
				// nonce = BlockChainBuilder::Calculononce();	// Cada llamada genera un nonce <>
				if ( resultado.length() > 0 || nonce.length() ) {
					int test;
					hash_resultado = sha256( resultado + nonce );
					test = BlockChainBuilder::CheckDificultadOk( hash_resultado, BlocklActual->getbits() );
					if ( test == 1 ) {
						cout << "Dificultad Ok < " << test << std::endl;
						break;
					}
					else if ( test < 0 ) {
						cout << "Error: " << test << std::endl;
					}
					else {
						cout << "Dificultad < " << test << std::endl;
					};
				}
				contador++;
			} while ( contador < 100 /* Corte de iteraciones */ );
			it.avanzar();
		} while ( ! it.eol() );
	}
	// Liberar la memoria
	if ( ! ListaBlocks.vacia() ) {
		lista <Block *>::iterador it;
		it = ListaBlocks.primero();
		do  {
			delete it.dato();
			it.avanzar();
		} while ( ! it.eol() );
	}
}

bool CheckHash( string valor, TiposHash Tipo = TiposHash::clavehash256 ) {
	if ( valor.empty() ) {
		return false;
	}
	else if ( Tipo  == TiposHash::clavehash256 && valor.length() != (size_t) LargoHash::LargoHashEstandar ) {
		return false;
	}
	else if ( Tipo  == TiposHash::clavefirma && valor.length() != (size_t) LargoHash::LargoHashFirma ) {
		return false;
	}
	else {
		int i = CheckHexa( valor );
		if ( i > 0 ) {
			// Anotar la posición y valor del dígito erróneo
			return false;
		}
		else return true;
	}
}

bool CheckPreBlock( string valor ) {
/* Esta deberia ser más genérica para hacer un check de Hash de distinas longitudes
	Podría meterse en un Enum o en #define o en Const int más estilo C++
*/
	if ( valor.empty() ) {
		return false;
	}
	else if ( valor.length() != (size_t) LargoHash::LargoHashEstandar /* Hay que sacar este Hardcodeo */ ) {
		return false;
	}
	else {
		int i = CheckHexa( valor );
		if ( i > 0 ) {
			// Anotar la posición y valor del dígito erróneo
			return false;
		}
		else return true;
	}
}

int CheckHexa( string valor ) {
	unsigned int i;
	for (i = 0; i != valor.length(); ++i) {
		if ( ! isxdigit ( valor[i] ) ) break;
	} 
	if ( i < valor.length() ) return i;
	return 0;
}
