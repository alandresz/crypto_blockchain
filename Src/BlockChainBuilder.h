
#ifndef BLOCKCHAINBUILDER_H_
#define BLOCKCHAINBUILDER_H_

#include "TiposHash.h"
#include "BlockChainDataTypes.h"
#include "BlockChainStatus.h"
#include "Block.h"
#include "lista.h"
#include "sha256.h"


class BlockChainBuilder {
private:	// Redundante pero más legible
	//  Anterior
	//static int CheckHexa( std::string value );	// <- esta le sería más util a BlockChainFileManager
	// Datos privados
	Block * BlocklActual;
	lista <Block *> ListaBlocks;
	std::string hash_resultado;
	size_t bits;	/* La dificultad de bits */
	//  Nuevo
	raw_t * pRawData;  // raw_t es el dato raw que devuelve filemanager. De aca builder saca los datos
	static bool CalculoBits( std::string hash, size_t bits );
	bool Minando();
	static std::string hex_str_to_bin_str( const std::string & hex );
	static const char* hex_char_to_bin( char c );
	static int dificultad( const std::string & value, const size_t dif );  					// -1 -> Error
	// Para medir tiempos de minado x Block.
	double seconds;
public:
	BlockChainBuilder();
	BlockChainBuilder(size_t d);
	~BlockChainBuilder();
		// Getters
	// size_t getbits();
	std::string getObtainedHash(){return hash_resultado;};
	raw_t *& getRawPointer(){return pRawData;}
	lista <Block *>& getBlockChainPointer(){return ListaBlocks;};
	Block *& getBlocklActual(){return BlocklActual;}
	double tiempominado();
	// Setters
	bool setbits( size_t valor );
	// Métodos
	size_t cantidadBlocks(); // VS me canta que no se usa
	static int CheckDificultadOk( const std::string & cadenaHexa, const size_t dif );  // Error -> < 0, No -> 0, 0k -> 1
	static std::string Calculononce();
	//status_t createBlockChain(void);
	status_t createOriginBlock(Transaction &tr);
	status_t createBlock(lista<Transaction*> & tr,std::string previousHashBlock);
	// removidas?
	//static bool CheckHash( const std::string valor, TiposHash Tipo = TiposHash::clavehash256 );
	//static size_t CheckHexa( const std::string value );
};

#endif /* BLOCKCHAINBUILDER_H_ */
