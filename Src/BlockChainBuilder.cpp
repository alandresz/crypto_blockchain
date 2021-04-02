
#include <time.h> 

#include "BlockChainBuilder.h"


BlockChainBuilder::BlockChainBuilder() :
										BlocklActual(),
										ListaBlocks(),
										hash_resultado( "" ),
										bits( ),
										pRawData(NULL),
										seconds()
{}

BlockChainBuilder::BlockChainBuilder(size_t d) :
										BlocklActual(),
										ListaBlocks(),
										hash_resultado( "" ),
										bits( d ),
										pRawData(NULL),
										seconds()
{}

BlockChainBuilder::~BlockChainBuilder() {
	if ( ! this->ListaBlocks.isEmpty() ){
	// lista <Transaction>::iterador it();
		lista <Block *>::iterador it(ListaBlocks);
		/* Itero la lista para recuperar todos los strings de la coleccion Transaction
		   que necesito para calcular el Hash.
		*/
		it = this->ListaBlocks.primero();
		while ( ! it.extremo() ) {
			if(it.dato() != NULL){
				delete it.dato();
				it.dato() = NULL;
			}
			it.avanzar();
		}
	}

}

//int BlockChainBuilder::CheckHexa( string value ) {
//	unsigned int i;
//	for (i = 0; i != value.length(); ++i) {
//		if ( ! isxdigit ( value[i] ) ) break;
//	}
//	if ( i < value.length() ) return i;
//	return 0;
//}
//
//bool BlockChainBuilder::CheckHash( std::string valor, TiposHash Tipo ) {
//	if ( valor.empty() ) {
//		return false;
//	}
//	//else if ( Tipo  == TiposHash::clavehash256 && valor.length() != LargoHashEstandar ) {
//	else if ( Tipo  == TiposHash::clavehash256 && valor.length() != (size_t) LargoHash::LargoHashEstandar ) {
//		return false;
//	}
//	//else if ( Tipo  == TiposHash::clavefirma && valor.length() != LargoHashFirma ) {
//	else if ( Tipo  == TiposHash::clavefirma && valor.length() != (size_t) LargoHash::LargoHashFirma ) {
//		return false;
//	}
//	else {
//		int i = CheckHexa( valor );
//		if ( i > 0 ) {
//			// Anotar la posici�n y valor del d�gito err�neo
//			return false;
//		}
//		else return true;
//	}
//}

//unsigned int BlockChainBuilder::Calculononce() {
//	static unsigned int contador = 0;
//	contador++;
//	return contador;

//}

bool BlockChainBuilder::CalculoBits( std::string hash, size_t bits ) {
	int resultado;
	if ( hash.length() > 0  ) {
		std::string hash_hex = "";
		hash_hex = BlockChainBuilder::hex_str_to_bin_str( hash );  // No lleva this-> porque es static
		resultado = BlockChainBuilder::CheckDificultadOk( hash_hex, bits );
		if ( resultado == 1 ) {
			return true;
		}
		else {
			// Incluye cadena hash vacia y bits == 0
			// Lo bueno de los booleanos es que siempre estas como m�ximo a un bit de acertar.
			return false;
		}
	}
	else {
		return false;
	}
}

bool BlockChainBuilder::Minando() {
	std::string resultado;
	
	if ( ! this->ListaBlocks.vacia() ) {
		lista <Block *>::iterador it;
		/* Itero la lista para recuperar todos los strings de la coleccion Transaction
		   que necesito para calcular el Hash.
		*/
		it = this->ListaBlocks.primero();
		do  {
			time_t timer1, timer2;
			time(&timer1);  	/* get current time; same as: timer = time(NULL)  */
			this->BlocklActual = it.dato();
			do{		
				resultado = "";

				resultado += this->BlocklActual->getpre_block();
				resultado += '\n';
				resultado += this->BlocklActual->gettxns_hash(); 
				resultado += '\n';
				resultado += std::to_string(this->BlocklActual->getbits());
				resultado += '\n';
				resultado += this->BlocklActual->Calculononce();
				resultado += '\n';
				//std::cout << resultado << std::endl;//DEBUG
				//if ( resultado.length() > 0  ) {
				this->hash_resultado = sha256 ( sha256( resultado ) );
				//std::cout << this->hash_resultado << std::endl; //DEBUG
				//}
			}while(! CalculoBits( this->hash_resultado, this->bits ) );
			time(&timer2);
			this->BlocklActual->setseconds( difftime( timer2, timer1 ) );


			it.avanzar();
		} while ( ! it.extremo() );
		return true;
	}
	return false;
}

double BlockChainBuilder::tiempominado() {
	return this->seconds;
}

const char* BlockChainBuilder::hex_char_to_bin( char c )
{
    // TODO handle default / error
	// https://stackoverflow.com/questions/18310952/convert-strings-between-hex-format-and-binary-format
    switch( toupper(c) )
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
		default: return "";
    }
}

std::string BlockChainBuilder::hex_str_to_bin_str( const std::string & hex )
{
    // TODO use a loop from <algorithm> or smth
    std::string bin;
	std::string hexbin;
    for( size_t i = 0; i != hex.length(); ++i ) {
		hexbin = hex_char_to_bin( hex[i] );
		if ( hexbin.empty() ) return "";
		bin += hexbin;
	}
    return bin;
}

int BlockChainBuilder::dificultad( const std::string & value, const size_t dif ) {
	// Se corta el recorrido de la cadena una vez alcanzado el valor dif
	size_t j = 0;

	if ( value.empty() ) return -1;
	else if ( dif == 0 ) return -1;

	for ( size_t i = 0;  value[ i ]; i++ ) {
		if ( value[ i ] == '0' ) j++;
		else if ( value[ i ] == '1' ) break;
		else return -1;
		if ( j >= dif ) break; 
	}
	return j;
}

int BlockChainBuilder::CheckDificultadOk( const std::string & cadenaHexa, const size_t dif ) {
	int d;
	if ( cadenaHexa.empty() ) return -3;
	if ( dif == 0 ) return -2;
	d = dificultad( cadenaHexa, dif);
	if ( d < 0 ) return -1;
	return (size_t) d >= dif ? 1 : 0;
}


//status_t BlockChainBuilder::createBlockChain( void ){
//	Block * newBlock = new Block(*pRawData);
//	newBlock->setpre_block( "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
//	newBlock->settxns_hash(sha256(sha256(newBlock->getcadenaprehash())));
//	newBlock->setbits(this->bits);
//	this->ListaBlocks.insertar(newBlock);
//	this->Minando();
//	return STATUS_OK;
//}

status_t BlockChainBuilder::createOriginBlock(Transaction& tr){
	Block * newBlock = new Block(tr);
	newBlock->setpre_block( "0000000000000000000000000000000000000000000000000000000000000000" );
	newBlock->settxns_hash( newBlock->gethash_Merkle());
	newBlock->setbits(this->bits);
	ListaBlocks.insertar(newBlock);
	this->Minando();
	return STATUS_OK;
}

status_t BlockChainBuilder::createBlock(lista<Transaction*> & mempoolList,std::string previousHashBlock){
	Block * newBlock = new Block(mempoolList);
	newBlock->setpre_block( previousHashBlock);
	newBlock->settxns_hash( newBlock->gethash_Merkle());
	newBlock->setbits(this->bits);
	ListaBlocks.insertar(newBlock);
	this->Minando();
	return STATUS_OK;
}
