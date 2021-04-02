
#include "BlockChainHistoryBook.h"

lista<Block*> BlockChainHistoryBook::AlgoChain;
//Cuentas BlockChainHistoryBook::ListadoCuentas;

// Para usar x línea de comandos block <id>
 Block * BlockChainHistoryBook::searchBlock( const std::string HashBlock ) {
	Block * B = NULL;
	// EL HASH YA ES VALIDADO EN FILEMANAGER
	// Checks
	//if ( txns_hash.empty()  ) {
	//	return B;
	//}
	//else if ( ! Block::CheckHash( txns_hash, TiposHash::clavehash256 ) ) {
	//	return B;
	//}
	// End Checks
	if ( ! AlgoChain.vacia() ) {
		lista <Block *>::iterador it( AlgoChain );
		it = AlgoChain.primero();
//		if ( ! ( it.dato()->getpre_block() == b_prev ) ) {
//			// Mal definido el Block Zero
//			return B;
//		}
		do {
			//std::cout << "Debug " << std::endl;
			//std::cout << it.dato()->getBlockHash() << std::endl;
			//std::cout << HashBlock << std::endl;
			if ( HashBlock.compare(it.dato()->getBlockHash()) == 0 ){
				B = it.dato();
				break;
			}
			it.avanzar();
		} while ( ! it.extremo() );
	}
	return B;
}


Transaction * BlockChainHistoryBook::searchTransaction(const std::string txns_hash ){

	Transaction * Trans = NULL;

	if ( ! AlgoChain.vacia() ) {
		lista <Block *>::iterador itBlock( AlgoChain );
		itBlock = AlgoChain.primero();
		do {
			// Debo abrir un segundo bloque de iteraciones sobre la lista de TI
			lista <Transaction *> trns;
			trns = itBlock.dato()->getListaTran();
			lista <Transaction *>::iterador itTrans( trns );
			// La lista a iterar es la de TransactionInput
			itTrans = trns.primero();
			if ( ! trns.vacia() ) {
				do{
					std::string hashTxnCandidate = sha256(sha256( itTrans.dato()->getConcatenatedTransactions() ));

					if ( txns_hash.compare(hashTxnCandidate) == 0 ) {
						Trans = itTrans.dato();
						break;
					}


					itTrans.avanzar();
				} while ( ! itTrans.extremo() );
			}
			itBlock.avanzar();
		} while ( ! itBlock.extremo() );
	}
	return Trans;
}

 TransactionInput * BlockChainHistoryBook::obtenerTransactionInput( const std::string tx_id ) {
	std::string b_prev;
	TransactionInput * TI = NULL;

	// Checks
//	if ( tx_id.empty()  ) {
//		return TI;
//	}
//	else if ( ! Block::CheckHash( tx_id, TiposHash::clavehash256 ) ) {
//		return TI;
//	}
	// End Checks

	//for ( size_t i = 0; i < (size_t) LargoHash::LargoHashEstandar; i++) { b_prev += '0'; }  // Block Zero
	if ( ! AlgoChain.vacia() ) {
		lista <Block *>::iterador itBlock( AlgoChain );
		itBlock = AlgoChain.primero();
		do {


			// Si el primer elemento no es el bloque origen hubo corrupcion de Algochain
			// Esl problema es que la implementacion de listas dejan el primer elemento
			// al final por eso genera error al usar AlgoChain.primero()
			//if ( ! ( it.dato()->getpre_block() == b_prev ) ) { return TI; }

			//b_prev = it.dato()->getpre_block();
			// Debo abrir un segundo bloque de iteraciones sobre la lista de TI
			lista <Transaction *> trns;
			trns = itBlock.dato()->getListaTran();
			lista <Transaction *>::iterador itTrans( trns );
			// La lista a iterar es la de TransactionInput
			itTrans = trns.primero();
			if ( ! trns.vacia() ) {
				do {
					// De las dos listas, itero las de TI
					lista <TransactionInput *> tInput;
					tInput = itTrans.dato()->getTransactionInputList();
					lista <TransactionInput *>::iterador itTransInput( tInput );
					itTransInput = tInput.primero();
					if ( ! tInput.vacia() ) {
						do {
							if ( tx_id.compare(itTransInput.dato()->getTxId()) )  {
								return itTransInput.dato();
							}
							itTransInput.avanzar();
						}  while ( ! itTransInput.extremo() );
					}
					itTrans.avanzar();
				} while ( ! itTrans.extremo() );
			}
			itBlock.avanzar();
		} while ( ! itBlock.extremo() );
	}
	return TI;
}

Transaction * BlockChainHistoryBook::getTransactionByTransactionOutputUser( const std::string user ) {
	//std::string b_prev;
	//TransactionOutput * TI = NULL;
	Transaction * Trans = NULL;

	// Checks
//	if ( tx_id.empty()  ) {
//		return TI;
//	}
//	else if ( ! Block::CheckHash( tx_id, TiposHash::clavehash256 ) ) {
//		return TI;
//	}
	// End Checks

	//for ( size_t i = 0; i < (size_t) LargoHash::LargoHashEstandar; i++) { b_prev += '0'; }  // Block Zero
	if ( ! AlgoChain.vacia() ) {
		lista <Block *>::iterador itBlock( AlgoChain );
		itBlock = AlgoChain.primero();
		do {


			// Si el primer elemento no es el bloque origen hubo corrupcion de Algochain
			// Esl problema es que la implementacion de listas dejan el primer elemento
			// al final por eso genera error al usar AlgoChain.primero()
			//if ( ! ( it.dato()->getpre_block() == b_prev ) ) { return TI; }

			//b_prev = it.dato()->getpre_block();
			// Debo abrir un segundo bloque de iteraciones sobre la lista de TI
			lista <Transaction *> trns;
			trns = itBlock.dato()->getListaTran();
			lista <Transaction *>::iterador itTrans( trns );
			// La lista a iterar es la de TransactionInput
			itTrans = trns.primero();
			if ( ! trns.vacia() ) {
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
							if ( user.compare(itTransOutput.dato()->getAddr()) == 0)  {
								return itTrans.dato();
							}
							itTransOutput.avanzar();
						}  while ( ! itTransOutput.extremo() );
					}
					itTrans.avanzar();
				} while ( ! itTrans.extremo() );
			}
			itBlock.avanzar();
		} while ( ! itBlock.extremo() );
	}
	return Trans;
}
void BlockChainHistoryBook::BorrarHistoria(void){
	// AlgoChain se autodestruye, antes debo liberar la memoria asignada en cada elemento * AlgoChain de la lista
	// El compilador ejecuta antes los destructores de las clases hijas que liberan su memoria dinámica.
	if ( ! AlgoChain.vacia() ) {
		lista  <Block *>::iterador it( AlgoChain );
		it = AlgoChain.primero();
		while ( ! AlgoChain.isEmpty()) {
			delete it.dato();
			AlgoChain.eliminar_nodo(it);
		}
		//BlockChainHistoryBook::ListadoCuentas.vaciarcuentas();
	}
}

bool BlockChainHistoryBook::AddBlock( Block * & B ) {
	Block * newBlock = new Block( *B );
	AlgoChain.insertar( newBlock );
	// Aca debo en Cuentas actualizar datos
	//BlockChainHistoryBook::ListadoCuentas.updatedatos( B );


	return true;
}

