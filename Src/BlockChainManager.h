
#ifndef BLOCKCHAINMANAGER_H_
#define BLOCKCHAINMANAGER_H_

#include <iostream>
#include "BlockChainStatus.h"
#include "BlockChainFileManager.h"
#include "BlockChainBuilder.h"
#include "BlockChainBookkeeper.h"

class BlockChainManager {
	static status_t state;
	static status_t command;
	static unsigned int userDefinedDifficulty;
	static lista<file_t *> userFiles;
	static void parseCommands(std::string command,payload_t & payload);
public:

	static void	proccesBlockChain();
	static void	proccesStatus( status_t status );

	static void setUserDefinedDifficulty(int d);
	static unsigned int getUserDefinedDifficulty(void);

	static void setUserFilename(ios_base::openmode  mode , std::string filename = "Standard",bool isStandar = true);
};

#endif /* BLOCKCHAINMANAGER_H_ */
