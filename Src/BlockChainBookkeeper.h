
#ifndef BLOCKCHAINBOOKKEEPER_H_
#define BLOCKCHAINBOOKKEEPER_H_

#include "BlockChainHistoryBook.h"
#include "BlockChainDataTypes.h"
#include "BlockChainStatus.h"
#include "Mempool.h"
#include "Transaction.h"
#include "Block.h"
#include "BlockChainUser.h"
#include "sha256.h"

// #include "lista.h"

class BlockChainBookkeeper {

private:
	Block * ActualBlock;
	Transaction * ActualTransaction;
	lista<Block * > BlockList;
	lista<Transaction *> TransactionList;
	static lista<BlockChainUser *> ListOfUsers;

	bool updateUserList(float value, bool isConfirmed,const std::string &hashUser, std::string username = "No name");
	bool updateUserList(const lista<Transaction *> & ListaTran , bool isConfirmed,Queue<string> & userNameQueue); // Version Const
	bool updateUserList(const lista<Transaction *> & ListaTran , bool isConfirmed); // Version Const
	bool updateUserList(lista<Transaction *> & ListaTran , bool isConfirmed, Queue<string> & userNameQueue); // Version No Const
	bool updateUserList(lista<Transaction *> & ListaTran , bool isConfirmed);
	bool updateUserList(bool isConfirmed);
	bool eraseBalanceFromHashUser(string hashUser);
	float searchBalanceFromHashUser(std::string hashUser);

public:
	BlockChainBookkeeper();
	~BlockChainBookkeeper();

	status_t createTransaction(payload_t payload);
	status_t createOriginTransaction(payload_t & payload);
	status_t saveBlockInHistoryBook(Block * &block, bool isOrigin);
	status_t saveOriginBlockInHistoryBook(Block* &block);
	status_t saveUserBlockChainInHistoryBook(lista<Block*> &listaBlock);
	status_t saveInMempool(Transaction * trans);
	status_t eraseAllBlockChainRegisters(void);
	status_t searchInHistoryBook(HashIdType type, std::string hashId);
	status_t searchInUserList(std::string hash,float &balance);

	std::string getLastBlockHash(void);
	std::string getTransactionHash(void);
	const lista<Block *> & getBlockChain(void);
	Block * &getActualBlock(void);
	Transaction * & getActualTransaction(void);
	lista<Block *> & getBlockList(void);
	lista<Transaction *> & getTransactionList(void);
	lista<Transaction *> &  getMempool(void );

	void printUsers(void);

};

#endif /* BLOCKCHAINBOOKKEEPER_H_ */
