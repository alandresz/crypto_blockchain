
#include "TransactionOutput.h"

	//---Constructores---//

//Descripcion: Construye el objeto TransactionOutput vacio
//Precondicion:
//Postcondicion: Atributos inicializados en cero o vacio
TransactionOutput::TransactionOutput(){
	this->value = 0;
	this->addr = "";
}

//Descripcion: Destruye el objeto TransactionOutput
//Precondicion:
//Postcondicion: Atributos en cero y strings vacios
TransactionOutput::~TransactionOutput(){
	this->value = 0;
	this->addr = "";
}
	//---Getters---//

//Descripcion: Devuelve el valor de Value
//Precondicion:
//Postcondicion:
float TransactionOutput::getValue(void) const{
	return this->value;
}

//Descripcion: Devuelve el arreglo de char del parametro addr
//Precondicion:
//Postcondicion: Debe ser un rvalue lo que devuelve
const std::string TransactionOutput::getAddr(void) const{
	return this->addr;
}
	//---Setters---//

//Descripcion: Carga el atributo value
//Precondicion: Se asume validado previamente
//Postcondicion:
void TransactionOutput::setValue(float value){
	this->value = value;

}

//Descripcion: Carga el atributo addr
//Precondicion: Se asume validado previamente
//Postcondicion:
void TransactionOutput::setAddr(std::string addr){
	this->addr = addr;
}

