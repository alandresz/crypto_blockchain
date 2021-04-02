
#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "BlockChainDataTypes.h"
#include "lista.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstddef> // Para NULL

class Transaction {
private:
	int n_tx_in; 								 // Indica cantidad total de inputs
	lista <TransactionInput *> ListaTranIn;      // Lista de inputs
	int n_tx_out; 								 // Indica cantidad total de outputs
	lista <TransactionOutput *> ListaTranOut;    // Lista de outputs

	std::string float_to_string_w_precision(float value, int p);
public:
	//---Constructores---//
	Transaction();
	Transaction(const Transaction & otherTrans);
	Transaction(int n_tx_in,int n_tx_out);
	Transaction( const raw_t & raw);
	~Transaction();
	//---Getters---//
	int getNumTransactionIn();
	int getNumTransactionOut();
	TransactionInput * getTransactionInput(int index);
	TransactionOutput * getTransactionOutput(int index);
	lista <TransactionInput *> getTransactionInputList();
	lista <TransactionOutput *> getTransactionOutputList();
	//---Setters---//
	//---Otros---//
	Transaction & operator=(Transaction &tr);
	void addTransactionInput();
	void addTransactionOutput();
	std::string getConcatenatedTransactions();
};

#endif /* TRANSACTION_H_ */
