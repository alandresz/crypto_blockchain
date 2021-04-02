
#ifndef BLOCKCHAINUSER_H_
#define BLOCKCHAINUSER_H_

#include <string>

class BlockChainUser {
private:
	float confirmedBalance;
	float unconfirmedBalance;
	std::string user;
	std::string hashUser;

public:
	BlockChainUser();
	~BlockChainUser();

	void addUser(std::string hashUser);
	void addUser(std::string hashUser,std::string User);
	void confirmBalance(void);
	void setConfirmBalance(float confirmedBalance);
	void setUnConfirmBalance(float unconfirmedBalance);
	void transfer(float unconfirmedBalance);

	std::string getHashUser(void);
	std::string getUsername(void);
	float getConfirmedBalance( void );
	float getUnConfirmedBalance(void );

	bool canTransfer(float mount);
};

#endif /* BLOCKCHAINUSER_H_ */
