
#ifndef TRANSACTIONINPUT_H_
#define TRANSACTIONINPUT_H_

#include <string>

class TransactionInput {
private:
	struct outpoint{
	   std::string tx_id;
	   int	idx;
	}outpoint;
	std::string addr;
public:
	//---Constructores---//
	TransactionInput();
	~TransactionInput();
	//---Getters---//
	const std::string getTxId(void) const;
	int getIdx(void) const;
	const std::string getAddr(void) const;
	//---Setters---//
	void setTxId(std::string tx_id);
	void setIdx(int idx);
	void setAddr(std::string addr);
	//---Otros---//
};

#endif /* TRANSACTIONINPUT_H_ */
