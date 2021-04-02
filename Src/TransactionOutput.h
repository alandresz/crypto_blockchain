
#ifndef TRANSACTIONOUTPUT_H_
#define TRANSACTIONOUTPUT_H_

#include <string>

class TransactionOutput {
private:
	float value;
	std::string addr;
public:
	//---Constructores---//
	TransactionOutput();
	~TransactionOutput();
	//---Getters---//
	float getValue(void) const;
	const std::string getAddr(void) const;
	//---Setters---//
	void setValue(float value);
	void setAddr(std::string addr);
	//---Otros---//
};

#endif /* TRANSACTIONOUTPUT_H_ */
