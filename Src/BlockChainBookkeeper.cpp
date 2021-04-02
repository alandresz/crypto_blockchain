
#include "BlockChainBookkeeper.h"

lista<BlockChainUser *> BlockChainBookkeeper::ListOfUsers;


BlockChainBookkeeper::BlockChainBookkeeper() {
	this->ActualTransaction = NULL;
	this->ActualBlock = NULL;
}

BlockChainBookkeeper::~BlockChainBookkeeper() {
	if(this->ActualTransaction !=NULL)
		delete ActualTransaction;

	if ( ! this->TransactionList.vacia() ) {
			lista  <Transaction *>::iterador it( this->TransactionList );
			it = this->TransactionList.primero();
			while ( ! this->TransactionList.isEmpty()) {
				delete it.dato();
				this->TransactionList.eliminar_nodo(it);
			}
	}
	if ( ! this->BlockList.vacia() ) {
			lista  <Block *>::iterador it( this->BlockList );
			it = this->BlockList.primero();
			while ( ! this->BlockList.isEmpty()) {
				delete it.dato();
				this->BlockList.eliminar_nodo(it);
			}
	}
}
bool BlockChainBookkeeper::updateUserList(const lista<Transaction *> &listaTran, bool isConfirmed,Queue<string> & userNameQueue){
	lista<Transaction *>::iterador it(listaTran);
	while (!it.extremo()){
		int OutputCant = it.dato()->getNumTransactionOut();
		for(int i = 1; i <= OutputCant; i++){
			std::string hashUser = it.dato()->getTransactionOutput(i)->getAddr();
			float 		value	 = it.dato()->getTransactionOutput(i)->getValue();
			if (this->updateUserList(value, isConfirmed, hashUser,userNameQueue.dequeue()) == false) return false;
		}
		it.avanzar();
	}
	return true;
}

bool BlockChainBookkeeper::updateUserList(const lista<Transaction *> &listaTran, bool isConfirmed){

	lista<Transaction *>::iterador it(listaTran);
	while (!it.extremo()){
		int inputCant = it.dato()->getNumTransactionIn();
		if( inputCant == 1){
			// Borro la informacion que tenia si encuentra al usuario
			// dado que al ser un bloque su otput esta en el vuelto
			string senderHash = it.dato()->getTransactionInput(1)->getAddr();
			this->eraseBalanceFromHashUser(senderHash);
		}
		int OutputCant = it.dato()->getNumTransactionOut();
		for(int i = 1; i <= OutputCant; i++){
			std::string hashUser = it.dato()->getTransactionOutput(i)->getAddr();
			float 		value	 = it.dato()->getTransactionOutput(i)->getValue();
			if (this->updateUserList(value, isConfirmed, hashUser) == false) return false;
		}
		it.avanzar();
	}
	return true;
}

bool BlockChainBookkeeper::updateUserList( bool isConfirmed){

	lista<BlockChainUser *>::iterador it(ListOfUsers);
	while (!it.extremo()){
		if (isConfirmed)
			// Si se pasa verdadero confirma todo
			it.dato()->confirmBalance();
		else
			// Si se pasa falso borra todo
			it.dato()->setConfirmBalance(0);
		it.avanzar();
	}
	return true;
}



bool BlockChainBookkeeper::updateUserList( lista<Transaction *> &listaTran, bool isConfirmed,Queue<string> & userNameQueue){
	lista<Transaction *>::iterador it(listaTran);
	while (!it.extremo()){
		int OutputCant = it.dato()->getNumTransactionOut();
		for(int i = 1; i <= OutputCant; i++){
			std::string hashUser = it.dato()->getTransactionOutput(i)->getAddr();
			float 		value	 = it.dato()->getTransactionOutput(i)->getValue();
			if (this->updateUserList(value, isConfirmed, hashUser,userNameQueue.dequeue()) == false) return false;

		}
		it.avanzar();
	}
	return true;
}

bool BlockChainBookkeeper::updateUserList( lista<Transaction *> &listaTran, bool isConfirmed){
	lista<Transaction *>::iterador it(listaTran);
	while (!it.extremo()){
		int OutputCant = it.dato()->getNumTransactionOut();
		for(int i = 1; i <= OutputCant; i++){
			std::string hashUser = it.dato()->getTransactionOutput(i)->getAddr();
			float 		value	 = it.dato()->getTransactionOutput(i)->getValue();
			if (this->updateUserList(value, isConfirmed, hashUser) == false) return false;

		}
		it.avanzar();
	}
	return true;
}
bool BlockChainBookkeeper::updateUserList(float value, bool isConfirmed,const std::string&hashUser,std::string username){
	bool userNotFound = true;
	if (! ListOfUsers.vacia()){
		lista<BlockChainUser *>::iterador it(ListOfUsers);
		while(!it.extremo()){
			if(hashUser.compare(it.dato()->getHashUser()) == 0)
			{
				userNotFound = false;
				//if ( it.dato()->canTransfer(value) )
				//{
					 if(isConfirmed){
						// if(value == it.dato()->getUnConfirmedBalance()){
						 	 //std::cout << "P1"<< std::endl;
							 it.dato()->confirmBalance();
						 //}else{
						//	 it.dato()->setConfirmBalance(value);
						 //}
					 }
					 else{
						 //std::cout << "P2"<< std::endl;
						 it.dato()->setUnConfirmBalance(value);
					 }
					//	 else{
					// Si el usuario no puede transferir se avisa que no se puede
					//return false;
				//}
			}
			it.avanzar();
		}
	}

	if( userNotFound || ListOfUsers.vacia() ) {
		BlockChainUser * newUser = new BlockChainUser;
		newUser->addUser(hashUser, username);
		newUser->setUnConfirmBalance(value);
		if(isConfirmed) newUser->confirmBalance();
		this->ListOfUsers.insertar(newUser);
		return true;
	}
	return true;

}

float BlockChainBookkeeper::searchBalanceFromHashUser(std::string hashUser){
	if (! ListOfUsers.vacia()){
		lista<BlockChainUser *>::iterador it(ListOfUsers);
		while(!it.extremo()){
			if(hashUser.compare(it.dato()->getHashUser()) == 0)
			{
				return it.dato()->getUnConfirmedBalance();
			}
			it.avanzar();
		}
	}
	return -1;
}

bool BlockChainBookkeeper::eraseBalanceFromHashUser(string hashUser){
	if (! ListOfUsers.vacia()){
			lista<BlockChainUser *>::iterador it(ListOfUsers);
			while(!it.extremo()){
				if(hashUser.compare(it.dato()->getHashUser()) == 0)
				{
					it.dato()->setConfirmBalance(0);
					return true;
				}
				it.avanzar();
			}
		}
		return false;
}

status_t BlockChainBookkeeper::createOriginTransaction(payload_t & payload){
	this->ActualTransaction = new Transaction(0,1);
	std::string hashUser = sha256(sha256(payload.user));
	this->ActualTransaction->getTransactionOutput(1)->setAddr(hashUser);
	this->ActualTransaction->getTransactionOutput(1)->setValue(payload.value);

	if( this->updateUserList(payload.value,false,hashUser,payload.user) == false){
		return STATUS_CORRUPT_FORMAT_BAD_BTCVALUE;
	}
	return STATUS_FINISH_CONVERT_SUCCESSFULY;
}

Transaction * & BlockChainBookkeeper::getActualTransaction(void){
	return this->ActualTransaction;
}

Block * & BlockChainBookkeeper::getActualBlock(void){
	return this->ActualBlock;
}

status_t BlockChainBookkeeper::saveOriginBlockInHistoryBook(Block *& block){

	if (! BlockChainHistoryBook::AlgoChain.isEmpty() ) BlockChainHistoryBook::BorrarHistoria();
	return saveBlockInHistoryBook(block,true);
}

status_t BlockChainBookkeeper::saveBlockInHistoryBook(Block* &block,bool isOrigin){

	if (! isOrigin){
		if (BlockChainHistoryBook::AlgoChain.isEmpty() ) return STATUS_NO_ORIGIN_BLOCK_MINED;
	}

	if (BlockChainHistoryBook::AddBlock(block) ) {
		//lista<Transaction *>::iterador iter(block->getListaTran());
		//while(!iter.extremo()){
			this->updateUserList(block->getListaTran(), true);
		//	iter.avanzar();
		//}
		return STATUS_OK;
	}

	else return STATUS_BAD_ALLOC;
}

status_t BlockChainBookkeeper::saveUserBlockChainInHistoryBook(lista<Block*> &listaBlock){
	if (! BlockChainHistoryBook::AlgoChain.isEmpty() ) BlockChainHistoryBook::BorrarHistoria();
	this->updateUserList(false);


	lista<Block*>::iterador it(listaBlock);
	it = listaBlock.ultimo();
	while(!it.extremo()){
		if (! BlockChainHistoryBook::AddBlock(it.dato() ) ) return STATUS_BAD_ALLOC;
		this->updateUserList(it.dato()->getListaTran(), false);
		this->updateUserList(true);
		this->printUsers();
	it.retroceder();
	}
	return STATUS_OK;
}

status_t BlockChainBookkeeper::createTransaction(payload_t payload){
	Queue<string> namesQueue;
	Queue<string> auxQueue;
	std::string _user_ = payload.ArgTranfer->dequeue();
	auxQueue.copyQueue(*payload.ArgTranfer);
	// Por diseno, el primer elemento de la cola sera el usuario que
	// realiza la transferencia.
	namesQueue.enqueue(_user_);
	const string hashUser= sha256(sha256(_user_));

	//Buscar en la lista de usuario a ver si tiene saldo
	float totalTransfer = 0;
	float userBalance = BlockChainBookkeeper::searchBalanceFromHashUser(hashUser);

	// Verifico si la suma de las transferencias no es mayor que el saldo del
	// Usuario
	float auxUserBalance = userBalance;
	while(!auxQueue.isEmpty()){
		auxQueue.dequeue(); // desacolo el nombre
		float value = std::stof(auxQueue.dequeue()); // me quedo con el valor
		auxUserBalance -= value;
		if( auxUserBalance < 0 ) return STATUS_USER_WITHOUT_BTC;
	}
	//Si pasa este while el usuario puede hacer dicha transferencia.


	// Busco en la historia la transaccion asociado al usuario pasado por hash
	Transaction * tr = BlockChainHistoryBook::getTransactionByTransactionOutputUser(hashUser);
	if(tr == NULL){
		 tr = Mempool::searchOutputUser(hashUser);
		 if(tr == NULL) 	 return STATUS_ERROR_HASH_NOT_FOUND;
	}


	// Mirando como es la estructura de la transaccion completo el outpoint

	// Contando con txIn encuentro el valor de indice del outpoint
	unsigned int txIn = 0;
	lista <TransactionOutput *> tOutput;
	tOutput =tr->getTransactionOutputList();
	lista <TransactionOutput  *>::iterador itTransOutput( tOutput);
	itTransOutput = tOutput.primero();
	do {
		if ( hashUser.compare(itTransOutput.dato()->getAddr()) == 0 )  {
		//	UserValue = itTransOutput.dato()->getValue();
		break;
		}
		txIn++;
		itTransOutput.avanzar();
	}  while ( ! itTransOutput.extremo() );

	// Con el doble hash de la transaccion obtengo el valor de Txid
	std::string TxId = sha256(sha256(tr->getConcatenatedTransactions()));

	this->ActualTransaction = new Transaction();
	this->ActualTransaction->addTransactionInput();
	this->ActualTransaction->getTransactionInput(1)->setTxId(TxId);
	this->ActualTransaction->getTransactionInput(1)->setIdx(txIn);
	this->ActualTransaction->getTransactionInput(1)->setAddr(hashUser);

	unsigned int OutputNumber = 1 ;

	while( ! payload.ArgTranfer->isEmpty()){
		this->ActualTransaction->addTransactionOutput(); // El insertar interno me hace aumentar automaticamente
		string addr = payload.ArgTranfer->dequeue();
		namesQueue.enqueue(addr);
		string hashAddr = sha256(sha256(addr));
		this->ActualTransaction->getTransactionOutput(OutputNumber)->setAddr(hashAddr);
		float value = std::stof(payload.ArgTranfer->dequeue());
		userBalance -= value;
		totalTransfer +=value;
		this->ActualTransaction->getTransactionOutput(OutputNumber)->setValue(value);
		//OutputNumber++;
		this->updateUserList(value,false,hashAddr,addr);
	}

	// Dado que le cargo los vueltos como transaccion antes de actualizar la lista de usuario
	// pongo en cero el balance del que transfiere, de modo que al trasnferirle el vuelto
	// su saldo queda correctamente definido.

	this->ActualTransaction->addTransactionOutput();
	this->ActualTransaction->getTransactionOutput(OutputNumber)->setAddr(hashUser);
	this->ActualTransaction->getTransactionOutput(OutputNumber)->setValue(userBalance);
	this->updateUserList(- totalTransfer,false,hashUser,_user_);
	//this->printUsers();
	//lista<Transaction * > listaTran;
	//listaTran.insertar(this->ActualTransaction);
	//this->updateUserList(listaTran, false,namesQueue);

	return STATUS_OK;
}

lista<Transaction *> & BlockChainBookkeeper::getMempool(){
	if (Mempool::getMempoolLength()){
		lista<Transaction *> mempool = Mempool::getTransactionsList();
		lista<Transaction *> ::iterador itMempool(mempool);
		while(! itMempool.extremo()){
			Transaction actualTran;
			actualTran = *(itMempool.dato());
			Transaction * copyTrans = new Transaction(actualTran);
			this->TransactionList.insertar(copyTrans) ;
			itMempool.avanzar();
		}
		Mempool::BorrarMempool();
	}else{
		this->ActualTransaction = new Transaction(0,0);
		this->TransactionList.insertar(this->ActualTransaction );
	}
	return this->TransactionList;
}


status_t BlockChainBookkeeper::saveInMempool(Transaction * trans){
	Mempool::addNewTransaction(trans);
	return STATUS_OK;
}

status_t BlockChainBookkeeper::searchInHistoryBook(HashIdType type, std::string hashId){

	switch(type){
	case HashIdType::blockId:{
		Block * B = BlockChainHistoryBook::searchBlock(hashId);
		if(B == NULL) return STATUS_ERROR_HASH_NOT_FOUND;
		Block * newBlock = new Block(*B);
		this->BlockList.insertar(newBlock);
		return STATUS_OK;
		break;
	}

	case HashIdType::txnId:{
		Transaction * TxFromAlgochain = BlockChainHistoryBook::searchTransaction(hashId);
		Transaction * TxFromMempool   = Mempool::searchTransaction(hashId);

		if( TxFromAlgochain == NULL && TxFromMempool == NULL) return STATUS_ERROR_HASH_NOT_FOUND;

		lista <Transaction *>  T;
		if( TxFromAlgochain != NULL )
			T.insertar(TxFromAlgochain);
		else
			T.insertar(TxFromMempool);

		lista<Transaction *> ::iterador it(T);

		while(!it.extremo()){
			Transaction * newTrans = new Transaction(*(it.dato()));
			this->TransactionList.insertar(newTrans) ;
			it.avanzar();
		}
		return STATUS_OK;
		break;
		}
	}
	return STATUS_ERROR_HASH_NOT_FOUND;
}

status_t BlockChainBookkeeper::searchInUserList(std::string user,float &balance){

	balance = this->searchBalanceFromHashUser(sha256(sha256(user)));
	if (balance == -1) return STATUS_ERROR_HASH_NOT_FOUND;
	return STATUS_OK;
}


const lista<Block *> & BlockChainBookkeeper::getBlockChain(void){
	return BlockChainHistoryBook::getListaBlocks();
}

lista<Block *> & BlockChainBookkeeper::getBlockList(void){
	return this->BlockList;
}

lista<Transaction *> & BlockChainBookkeeper::getTransactionList(void){
	return this->TransactionList;
}

status_t BlockChainBookkeeper::eraseAllBlockChainRegisters(void){
	BlockChainHistoryBook::BorrarHistoria();
	Mempool::BorrarMempool();
	if ( ! ListOfUsers.vacia() ) {
			lista  <BlockChainUser*>::iterador it( ListOfUsers );
			it = this->ListOfUsers.primero();
			while ( ! ListOfUsers.isEmpty()) {
				delete it.dato();
				ListOfUsers.eliminar_nodo(it);
		}
	}
	return STATUS_OK;
}

std::string BlockChainBookkeeper::getLastBlockHash(void){
	lista<Block *>::iterador AlgoChain( BlockChainHistoryBook::AlgoChain);
	AlgoChain = BlockChainHistoryBook::AlgoChain.ultimo();
	return AlgoChain.dato()->getBlockHash();
}

std::string BlockChainBookkeeper::getTransactionHash(){
	string debugString = this->ActualTransaction->getConcatenatedTransactions();
	return sha256(sha256(debugString));
}



void BlockChainBookkeeper::printUsers(void){
		lista<BlockChainUser *>::iterador it(ListOfUsers);
		it = ListOfUsers.primero();
		while(!it.extremo()){
			std::cout << "-----------------------"			<< std::endl;
			std::cout << it.dato()->getUsername() 			<< std::endl;
			std::cout << it.dato()->getHashUser() 			<< std::endl;
			std::cout << it.dato()->getConfirmedBalance() 	<< std::endl;
			std::cout << it.dato()->getUnConfirmedBalance() << std::endl;
			std::cout << "-----------------------"			<< std::endl;
			it.avanzar();
		}
}

