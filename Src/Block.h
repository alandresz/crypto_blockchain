//Archivo fuente header clase Block / AlgoBlock

#ifndef BLOCK_H_
#define BLOCK_H_

#include <cstdlib>
#include <string>
#include <vector>
#include "sha256.h"
#include "lista.h"
#include "TiposHash.h"
#include "Transaction.h"

#include "BlockChainDataTypes.h"

// const size_t LargoHashEstandar = 64;
// const size_t LargoHashFirma    = 40;	// Hash Pública de la Cuenta
// https://stackoverflow.com/questions/2268749/defining-global-constant-in-c
// Análisis de Pro vs Contras contra #define y otras formas

using namespace std;

class Block {
	private:
		// Atributos Seccion Header
		std::string pre_block;
		std::string txns_hash;	// <- retiene el hash256(hash256(cadena_prehash))
		size_t bits;			// La dificultad de bits
		size_t nonce;
		std::string hash_Merkle;
		StatusBlock eBlock;
		// Atributos Seccion Body;
		size_t txn_count;
		lista <Transaction *> ListaTran;
		Transaction * CurTran;
		std::string cadena_prehash;
		// Para medir tiempos de minado x Block.
		double seconds;
		// Métodos privados
		std::string RecalculoHash( void );
		std::string ArbolMerkle( void );

	public:
    // Métodos
        // Constructores
        Block();
		Block( const raw_t & raw );
		Block( const Transaction & tr);
		Block(  Block & otherBlock);
		Block( const lista<Transaction*> & tr);
		//Block( const & std::string previo_block, size_t bits, const & raw_t );
		// size_t bits sale de BlockChainManager::getUserDefinedDifficulty(void), pero referenciar a esta clase implica un encastramiento indeseado.
        // Destructor
        ~Block();
		// Getters
		std::string getpre_block();
		std::string gettxns_hash();
		unsigned int getbits();
		size_t getnonce();
		std::string getStrNonce();
		size_t gettxn_count();
		std::string getcadenaprehash();
		std::string gethash_Merkle();
		std::string getBlockHash();
		const lista <Transaction *>& getListaTran();
		Transaction * getTran( size_t Index );
		// Setters
		bool setpre_block( std::string valor );
		bool settxns_hash( std::string valor );		// Debo dejar el método de asignación. El cálculo Hash es externo al objeto block, no está encapsulado.
		bool setbits( size_t valor );
		bool setnonce( size_t valor );				// Debo dejar el método de asignación. El cálculo del Nonce es externo al objeto block, no está encapsulado.
		bool setseconds( double segundos );
		bool settxn_count(size_t valor);
		bool settransaction( const raw_t & raw );
		bool settransaction( Transaction * pTr);
		StatusBlock EstatusBlock();
		// Métodos públicos
		double tiempominado();
		std::string Calculononce();

};

#endif /* BLOCK_H_ */
