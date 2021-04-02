
#ifndef BLOCKCHAINHISTORYBOOK_H_
#define BLOCKCHAINHISTORYBOOK_H_

#include <string>
#include "lista.h"
#include "Block.h"
#include "sha256.h"
//#include "Cuentas.h"

enum class HashIdType{
	blockId,
	txnId,
};

class BlockChainHistoryBook {
private:
	friend class BlockChainBookkeeper;
	static lista<Block*> AlgoChain;
	//static Cuentas ListadoCuentas;
	//---Getters---//
	static const lista <Block *> & getListaBlocks(){return AlgoChain;};
	// Para usar x línea de comandos block <id>
	static Block * searchBlock( const std::string txns_hash );							// Ante cualquier error devuelve NULL
	// Para usar x línea de comandos txn <id>
	static Transaction * searchTransaction(const std::string txns_hash );
	static Transaction * getTransactionByTransactionOutputUser( const std::string user );
	static TransactionInput * obtenerTransactionInput( const std::string tx_id );	// Ante cualquier error devuelve NULL


	//---Setters---//
	//---Otros---//
	static void BorrarHistoria( void );
	static bool AddBlock( Block *& B );
	static bool AddListaBlocks( lista <Block *> & lista );
public:
	//static lista <TransactionOutput *> obtenerOutputs( const std::string tx_id, const int idx );
	//static bool updatedatosdatos();
	
	//static void imprimirdetalle( const std::string addr );
	//static lista <movimientos_t *> obtenerdetalle( std::string addr );
};


#endif /* BLOCKCHAINHISTORYBOOK_H_ */
