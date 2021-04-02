//Archivo fuente clase Block / AlgoBlock

#include "Block.h"


// Constructores
Block::Block()
	: pre_block(""), txns_hash(""), bits(3  /* El valor por default establecido en el TP0 */), nonce(0), eBlock(StatusBlock::BlockSinDatos), txn_count(0), CurTran(NULL)
	// ver el #define DIFFICULTY_DEFAULT_VALUE 3
{
	//this->ListaTran = NULL;
	// this->CurTran = NULL;
	// this->txn_count = 0;
	// this->eBlock = StatusBlock::BlockSinDatos;
}

Block::Block( const raw_t & raw )
	: pre_block(""), txns_hash(""), bits( 3  /* El valor por default establecido en el TP0 */), nonce(0), eBlock(StatusBlock::BlockSinDatos)
{
	try {
		this->CurTran = new Transaction( raw );  	// <- Ojo, nuevo constructor
		this->ListaTran.insertar( this->CurTran );	// Para el Constructor con un contenedor de raw_t habrá que iterar pasando el mismo tipo de parámetros al constructor de Transaction
		this->txn_count = 1;						// Para el Constructor que recibe un Contenedor, se incrementa en cada instancia nueva de Transaction
		this->eBlock = StatusBlock::BlockPendienteCadena_prehash;
		RecalculoHash();
	}
	catch (std::bad_alloc& ba)
	{
		this->eBlock = StatusBlock::BlockBadAlloc;
		std::cerr << "bad_alloc caught: " << ba.what() << '\n';
	}
}

Block::Block( const  Transaction & tr)
	: pre_block(""), txns_hash(""), bits(), nonce(0), eBlock(StatusBlock::BlockSinDatos)
{
	try {
		this->CurTran = new Transaction( tr );
		this->ListaTran.insertar( this->CurTran );	// Para el Constructor con un contenedor de raw_t habrá que iterar pasando el mismo tipo de parámetros al constructor de Transaction
		this->txn_count = 1;						// Para el Constructor que recibe un Contenedor, se incrementa en cada instancia nueva de Transaction
		this->eBlock = StatusBlock::BlockPendienteCadena_prehash;
		RecalculoHash();
	}
	catch (std::bad_alloc& ba)
	{
		this->eBlock = StatusBlock::BlockBadAlloc;
		std::cerr << "bad_alloc caught: " << ba.what() << '\n';
	}
}

Block::Block( const  lista<Transaction*> & trList)
	: pre_block(""), txns_hash(""), bits(), nonce(0), eBlock(StatusBlock::BlockSinDatos)
{
	lista<Transaction*> ::iterador it(trList);
	try {
		while(!it.extremo()){
			Transaction actualTran;
			actualTran = *(it.dato());
			Transaction * copyTrans = new Transaction(actualTran);
			this->ListaTran.insertar(copyTrans) ;
			it.avanzar();
		}
		this->txn_count = trList.tamano();
		this->eBlock = StatusBlock::BlockPendienteCadena_prehash;
		RecalculoHash();
	}
	catch (std::bad_alloc& ba)
	{
		this->eBlock = StatusBlock::BlockBadAlloc;
		std::cerr << "bad_alloc caught: " << ba.what() << '\n';
	}
}

Block::Block(Block & otherBlock){
	pre_block = otherBlock.getpre_block();
	txns_hash = otherBlock.gettxns_hash();
	bits = otherBlock.getbits();
	nonce = otherBlock.getnonce();
	eBlock = otherBlock.eBlock;
	txn_count = otherBlock.gettxn_count();
	cadena_prehash = otherBlock.getcadenaprehash();
	lista<Transaction *>::iterador itTran(otherBlock.getListaTran());
	itTran = otherBlock.getListaTran().ultimo();
	CurTran = itTran.dato();
	while(! itTran.extremo())
	{
		Transaction * copyTran = new Transaction(*itTran.dato());
		this->ListaTran.insertar(copyTran);
		itTran.avanzar();
	}

}

// Destructor
Block::~Block() {
	// ListaTran se autodestruye, antes debo liberar la memoria asignada en cada elemento * ListaTran de la lista
	if ( ! this->ListaTran.vacia() ) {

		lista <Transaction *>::iterador it(ListaTran);
		/* Itero la lista para recuperar todos los strings de la coleccion Transaction
		   que necesito para calcular el Hash.
		*/
		it = this->ListaTran.primero();
		while ( ! it.extremo() ) {
			if(it.dato() != NULL){
				delete it.dato();
				it.dato() = NULL;
			}
			it.avanzar();
		}
	}
}

// Getters
size_t Block::gettxn_count() {
	return this->txn_count;
}

std::string Block::getpre_block() {
	return this->pre_block;
}

std::string Block::gettxns_hash() {
	return this->txns_hash;
}

unsigned int Block::getbits() {
	return this->bits;
}

size_t Block::getnonce() {
	return this->nonce;
}

std::string Block::getStrNonce() {
	return std::to_string(this->nonce);
}

std::string Block::getcadenaprehash() {
	return this->cadena_prehash;
}

double Block::tiempominado() {
	return this->seconds;
}

Transaction * Block::getTran( size_t Index ) {
	Transaction * T = NULL;
	size_t i = 0;
	// Checks
	if ( Index <= 0 ) { return T; }
	else if ( Index > this->txn_count ) { return T; }
	if ( ! this->ListaTran.vacia() ) {
		lista <Transaction *>::iterador it(ListaTran);
		it = this->ListaTran.primero();
		do  {
			if ( Index == i++ ) {
				T = it.dato();
				break;
			}
			it.avanzar();
		} while ( ! it.extremo() );
	}
	return T;
}

const lista <Transaction *>& Block::getListaTran() {
	return this->ListaTran;
}

// Setters
bool Block::setpre_block( std::string valor ) {
	if ( valor.empty() ) {
		this->pre_block = "";
		// Hay que anotar, en un status ?, el error o disparar un throw
	}
	else {
		/* 1) Debo validar que sea una cadena de 32 bytes o 64 dígitos Hexa
		   2) Chequear que cada byte sea un caracter hexa válido.
		   2) Chequear que cada byte sea un caracter hexa válido. Se elimina se supone que vien externamente validado.
		   		if ( BlockChainBuilder::CheckHash( valor, TiposHash::clavehash256 ) ) {
					this->pre_block = valor;
		*/
		this->pre_block = valor;
	}
	return true;
}

bool Block::settxns_hash( std::string valor ) {
	if ( valor.empty() ) {
		this->txns_hash = "";
		// Hay que anotar, en un status ?, el error o disparar un throw
	}
	else {
		/* 1) Debo validar que sea una cadena de 32 bytes o 64 dígitos Hexa
		   2) Chequear que cada byte sea un caracter hexa válido. Se elimina se supone que viene externamente validado.
		   		if ( BlockChainBuilder::CheckHash( valor, TiposHash::clavehash256 ) ) {
			this->txns_hash = valor;
		}
		*/
		this->txns_hash = valor;
	}
	return true;
}

bool Block::setbits( size_t valor ) {
	if ( !valor ) {
		this->bits = 0;
		// Hay que anotar, en un status ?, el error o disparar un throw
	}
	else {
		this->bits = valor;
	}
	return true;
}

bool Block::setnonce( size_t valor ) {
	this->nonce = valor;
	return true;
}

bool Block::settxn_count(size_t valor){
	this->txn_count = valor;
	return true;
}

bool Block::settransaction( const raw_t & raw ) {
	try {
		this->CurTran = new Transaction( raw );  	// <- Ojo, nuevo constructor
		this->ListaTran.insertar( this->CurTran );	// Para el Constructor con un contenedor de raw_t habrá que iterar pasando el mismo tipo de parámetros al constructor de Transaction
		this->txn_count ++;						// Para el Constructor que recibe un Contenedor, se incrementa en cada instancia nueva de Transaction
		this->eBlock = StatusBlock::BlockPendienteCadena_prehash;
		RecalculoHash();
		return true;
	}
	catch (std::bad_alloc& ba)
	{
		this->eBlock = StatusBlock::BlockBadAlloc;
		std::cerr << "bad_alloc caught: " << ba.what() << '\n';
		return false;
	}
}

bool Block::settransaction(Transaction * pTr){
	try {
			this->CurTran = pTr;  	// <- Ojo, nuevo constructor
			this->ListaTran.insertar( this->CurTran );	// Para el Constructor con un contenedor de raw_t habrá que iterar pasando el mismo tipo de parámetros al constructor de Transaction
			this->txn_count ++;						// Para el Constructor que recibe un Contenedor, se incrementa en cada instancia nueva de Transaction
			this->eBlock = StatusBlock::BlockPendienteCadena_prehash;
			RecalculoHash();
			return true;
		}
		catch (std::bad_alloc& ba)
		{
			this->eBlock = StatusBlock::BlockBadAlloc;
			std::cerr << "bad_alloc caught: " << ba.what() << '\n';
			return false;
		}
}

bool Block::setseconds( double segundos ) {
	this->seconds = segundos;
	return true;
}

std::string Block::RecalculoHash( void ) {
	std::string cadena = "";
	if ( ! this->ListaTran.vacia() ) {
		lista <Transaction *>::iterador it(ListaTran);
		/* Itero la lista para recuperar todos los strings de la coleccion Transaction
		   que necesito para calcular el Hash.
		*/
		it = this->ListaTran.primero();
		while ( ! it.extremo() ) {
			cadena += it.dato()->getConcatenatedTransactions();
			it.avanzar();
		}
	}
	if ( ! cadena.empty() ) {
		this->cadena_prehash = cadena;
		this->eBlock = StatusBlock::BlockCalculadoCadena_prehash;
		this->hash_Merkle = ArbolMerkle();
	}
	else this->eBlock = StatusBlock::BlockPendienteCadena_prehash;
	return cadena;
}

std::string Block::Calculononce() {
	static size_t contador = 0;
	this->nonce = contador++;
	return std::to_string( this->nonce );
}

std::string Block::ArbolMerkle( void ) {
	/*
	   Dudas
       Utilizar recursividad plantea un aumento de espacio con una misma complejidad log(n).
	   Al utilizar iteraciones combinado con el uso de un vector inplace() elimina el aumento de espacio y posee la misma complejidad
	   Con la última transaction si es impar se asume que la de al lado es el mismo y se hace un sha256.
	*/
	std::string cadena = "";
	if ( ! this->ListaTran.vacia() ) {
		// rellenado del vector.
		// Esto se podria implementar para optimizar en la iteracion de RecalculoHash.
		// A costa de extender el espacio de memoria consumido por strMerkle[] a lo largo de toda la instancia de Block.
		lista <Transaction *>::iterador it(ListaTran);
		size_t largo = this->ListaTran.tamano();  // Lo almaceno para no recalcularlo en cada iteración
		vector < std::string > strMerkle( largo );
		it = this->ListaTran.primero();
		size_t i = 0, tam = largo;
		while ( ! it.extremo() ) {
			strMerkle[i++] =  sha256( sha256( it.dato()->getConcatenatedTransactions() ) );
			it.avanzar();
		};
		// TODO. Analizar ventajas en este tramo de llevarlo a recursivo e inplace.
		for ( size_t j = 0; ( j < tam ) && ( tam > 1 ); j++  ) {
			for ( i = 0; i < largo; i += 2 ) {
				if ( i == largo - 1) {
					// Lucio me recordó que es hash doble !!
					strMerkle[ i ] = sha256( sha256( strMerkle[ i ] + strMerkle[ i ] ) );
				}
				else {
					strMerkle[ i ] = sha256( sha256( strMerkle[ i ] + strMerkle[ i + 1 ] ) );
				}
			}
			// ( tam % 2 ) ? tam <<= 1 : tam = ( tam + 1 ) << 1;   <- Opcion rápida con operadores de bits.
			if ( tam % 2 ) {
				tam /= 2;
			}
			else {
				tam = ( tam + 1) / 2;
			}
			
		}
		cadena = strMerkle[ 0 ];
	}
	return cadena;
}

std::string Block::gethash_Merkle() {
	return this->hash_Merkle;
}
std::string Block::getBlockHash(){
	//El campo prev block del header de un bloque b indica el hash del bloque antecesor b 0 en la
	//Algochain. De este modo, prev block = SHA256 ( SHA256 ( b 0 )) . Dicho hash lo calcularemos
	//sobre una concatenación secuencial de todos los campos de b 0 respetando exactamente el
	//formato de bloque que describiremos en la Sección
	std::string blockConcantenation;
	blockConcantenation += this->pre_block					+ '\n';
	blockConcantenation += this->txns_hash 			 		+ '\n';
	blockConcantenation += std::to_string(this->bits) 		+ '\n';
	blockConcantenation += std::to_string(this->nonce)		+ '\n';
	blockConcantenation += std::to_string(this->txn_count)	+ '\n';
	blockConcantenation += this->cadena_prehash; //El '\n' viene incluido
	return sha256(sha256(blockConcantenation));
}


