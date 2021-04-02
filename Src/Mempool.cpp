
#include "Mempool.h"


lista <Transaction *> Mempool::transList;

void Mempool::addNewTransaction(Transaction * & new_txn){

	Transaction * newTx = new Transaction(*new_txn);
	transList.insertar( newTx );
}


lista <Transaction *> & Mempool::getTransactionsList(){
	return transList;
	//Se deberia borrar la mempool cuando se trae toda la lista...
}

size_t Mempool::getMempoolLength(){
	return transList.tamano();
}

void Mempool::BorrarMempool(void){
	if ( ! transList.vacia() ) {
		lista  <Transaction *>::iterador it( transList );
		it = transList.primero();
		while ( ! transList.isEmpty()) {
			delete it.dato();
			transList.eliminar_nodo(it);
		}
	}
}


Transaction * Mempool::searchOutputUser(std::string hashUser){
	if ( ! transList.vacia() ) {
		lista <Transaction *>::iterador itTrans( transList );
		// La lista a iterar es la de TransactionInput
		itTrans =transList.primero();

		do {
			// De las dos listas, itero las de TI
			lista <TransactionOutput *> tOutput;
			tOutput = itTrans.dato()->getTransactionOutputList();
			lista <TransactionOutput *>::iterador itTransOutput( tOutput );
			itTransOutput = tOutput.primero();
			if ( ! tOutput.vacia() ) {
				do {
					// Si encuentro al usuario en el output
					// devuelvo el bloque para analizarlo
					// afuera
					if ( hashUser.compare(itTransOutput.dato()->getAddr()) == 0)  {
						return itTrans.dato();
					}
					itTransOutput.avanzar();
				}  while ( ! itTransOutput.extremo() );
			}
			itTrans.avanzar();
		} while ( ! itTrans.extremo() );

	}
	return NULL;
}




Transaction * Mempool::searchTransaction(const std::string txns_hash ){
	Transaction * T = NULL;

	if ( ! transList.vacia() ) {
		lista <Transaction *>::iterador it( transList );
		it = transList.primero();

		do {
			std::string hashTxnCandidate = sha256(sha256( it.dato()->getConcatenatedTransactions() ));

			if ( txns_hash.compare(hashTxnCandidate) == 0 ) {
				T = it.dato();
				break;
			}
			it.avanzar();
		} while ( ! it.extremo() );
	}
	return T;
}

//bool Mempool::getlistamovimientos( cuentas_t & c ) {
//	// Checks
//	if ( c.addr.empty() ) return false;
//
//	if ( ! transList.vacia() ) {
//		bool Cuentaorigen = false;
//		lista <Transaction *>::iterador itTrans( transList );
//		itTrans =transList.primero();
//		do {
//			// Itero TI para sacar la Cuenta Origen
//			lista <TransactionInput *> tIntput;
//			tIntput = itTrans.dato()->getTransactionInputList();
//			lista <TransactionInput *>::iterador itTransInput( tIntput );
//			itTransInput = tIntput.primero();
//			if ( ! tIntput.vacia() ) {
//				do {
//					if ( c.addr == itTransInput.dato()->getAddr() )  {
//						Cuentaorigen = true;
//						break;
//					}
//					itTransInput.avanzar();
//				}  while ( ! itTransInput.extremo() );
//			}
//
//			lista <TransactionOutput *> tOutput;
//			tOutput = itTrans.dato()->getTransactionOutputList();
//			lista <TransactionOutput *>::iterador itTransOutput( tOutput );
//			itTransOutput = tOutput.primero();
//			if ( ! tOutput.vacia() ) {
//				movimientos_t * t;
//				do {
//					// Si encuentro al usuario en el output
//					// devuelvo el bloque para analizarlo
//					// afuera
//					//if ( hashUser.compare(itTransOutput.dato()->getAddr()) == 0 )  {  	// TODO <- ver como es
//					if ( Cuentaorigen ) {
//						// Son todas salidas o pagos
//						if ( c.addr == itTransOutput.dato()->getAddr() )  {
//							// Vuelto, no se anota, porque es un débito/crédito a la misma cuenta, un movimiento de orden
//						}
//						else {
//							// Pagos
//							t->txns_hash = "";	// <- Es memPool
//							t->tx_id = "";		// <- Es memPool
//							t->idx = 0;			// <- Es memPool
//							t->addr = itTransOutput.dato()->getAddr();
//							t->value -= itTransOutput.dato()->getValue();
//							c.pendiente -= itTransOutput.dato()->getValue();
//							c.mempool.insertar( t );
//						}
//					}
//					else {
//						// Son entradas
//						if ( c.addr == itTransOutput.dato()->getAddr() )  {
//							// Créditos
//							t->txns_hash = "";	// <- Es memPool
//							t->tx_id = "";		// <- Es memPool
//							t->idx = 0;			// <- Es memPool
//							t->addr = itTransOutput.dato()->getAddr();
//							t->value -= itTransOutput.dato()->getValue();
//							c.pendiente += itTransOutput.dato()->getValue();
//							c.mempool.insertar( t );
//						}
//					}
//					itTransOutput.avanzar();
//				}  while ( ! itTransOutput.extremo() );
//			}
//			itTrans.avanzar();
//		} while ( ! itTrans.extremo() );
//
//	}
//	return true;
//}
//


/*
void Mempool::set_new_pool_unit( MempoolUnit * p_memp_unit){

	if ( this->n_txns == 0  ){
		std::cout<<"Mempool vacia. Creando 1er nodo" <<std::endl;

		this->first_node = p_memp_unit;
		this->last_node = this->first_node;
		this->n_txns = 1;

	}
	else{
		int aux = this->n_txns + 1;
		std::cout<<"Creando nodo numero "<< aux <<std::endl;

		(*(this->last_node)).set_next_node( p_memp_unit );
		this->last_node = p_memp_unit;

		this->n_txns += 1;

	}


}


int Mempool::get_number_txns(){
	int a = this->n_txns;
	return a;
}



Transaction Mempool::get_transaction_n(int n){
	MempoolUnit * n_node = this->first_node;

	std::cout<< "get_transac_n, todo bien hasta aca" << std::endl;

	for(int i = 1; i < n; i++){
		std::cout<< "get_transac_n, adentro del for, todo bien hasta aca" << std::endl;
		n_node = (*n_node).get_next_unit();
	}

	std::cout<< "get_transac_n, fuera del for, todo bien hasta aca" << std::endl;
	return (*n_node).get_txn();

}
*/

