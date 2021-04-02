
#include "TransactionInput.h"

//---Constructores---//

//Descripcion: Construye el objeto TransactionInput vacio
//Precondicion:
//Postcondicion: Todos los parametros iniciados en 0 o vacio
TransactionInput::TransactionInput(){
	this->outpoint.idx = 0;
	this->outpoint.tx_id = "";
	this->addr = "";
}

//Descripcion: Destruye el objeto TransactionInput
//Precondicion:
//Postcondicion: Todos los parametros iniciados en 0 o vacio
//Los hashes no deben quedar en ninguna zona
TransactionInput::~TransactionInput(){
	this->outpoint.idx = 0;
	this->outpoint.tx_id = "";
	this->addr = "";
}

	//---Getters---//

//Descripcion: Devuelve el parametro tx_id del outpoint
//Precondicion:
//Postcondicion:
const std::string TransactionInput::getTxId(void) const{
	return this->outpoint.tx_id;
}

//Descripcion: Devuelve el parametro idx del outpoint
//Precondicion:
//Postcondicion:
int TransactionInput::getIdx(void) const{
	return this->outpoint.idx;
}

//Descripcion: Devuelve el parametro addr
//Precondicion:
//Postcondicion:
const std::string TransactionInput::getAddr(void) const{
	return this->addr;
}

	//---Setters---//

//Descripcion: Carga el atributo tx_id
//Precondicion: Se asume validado previamente
//Postcondicion:
void TransactionInput::setTxId(std::string tx_id){
       this->outpoint.tx_id = tx_id;
}


//Descripcion: Carga el atributo idx
//Precondicion: Se asume validado previamente
//Postcondicion:
void TransactionInput::setIdx(int idx){
	this->outpoint.idx = idx;
}

//Descripcion: Carga el atributo addr
//Precondicion:  Se asume validado previamente
//Postcondicion:
void TransactionInput::setAddr(std::string addr){
	this->addr = addr;
}
