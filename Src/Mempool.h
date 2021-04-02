
#ifndef MEMPOOL_H_
#define MEMPOOL_H_


using namespace std;
//#include "MempoolUnit.h"
#include "Transaction.h"
#include "lista.h"
#include "sha256.h"
//#include "Cuentas.h"
// static lista <Transaction *> Mempool::Mempool;


class Mempool{

	private:

		//friend class Transaction;
		friend class BlockChainBookkeeper;
		static lista <Transaction *> transList;
		static void addNewTransaction(Transaction *& new_txn);     // Usa metodos de LISTA para agregar un nuevo nodo con una transaccion
		//static Transaction & get_transaction_n(int n);   		// DEVUELVE LA N-ESIMA TRANSACCION DE LA MEMPOOL (LA MEMPOOL SE ORGANIZA EN ORDEN DESCENDENTE)
		static lista <Transaction *> & getTransactionsList();     // Devuelve un puntero a la lista de transacciones
		static Transaction * searchOutputUser(std::string hashUser);
		static Transaction * searchTransaction(const std::string txns_hash );
		static size_t getMempoolLength();
		//static bool getlistamovimientos( cuentas_t & c );
		static void BorrarMempool(void);

	public:

};



#endif /* MEMPOOL_H_ */
