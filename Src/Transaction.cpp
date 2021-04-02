
#include "Transaction.h"


	//---Constructores---//

//Descripcion: Instancia el objeto Transaction vacio
//Precondicion: -
//Postcondicion: La lista de transacciones de entrada y salida apuntan a NULL
Transaction::Transaction(){
	this->n_tx_in = 0;
	this->n_tx_out = 0;
}


Transaction::Transaction(const Transaction & otherTrans){
		lista <TransactionInput *>::iterador itInput( otherTrans.ListaTranIn);
		lista <TransactionOutput*>::iterador itOutput( otherTrans.ListaTranOut);
		this->n_tx_in = otherTrans.n_tx_in;
		while(!itInput.extremo() )
		{
			try {
				TransactionInput * pTxInput = new TransactionInput;
				pTxInput->setTxId(itInput.dato()->getTxId() );
				pTxInput->setIdx(itInput.dato()->getIdx()   );
				pTxInput->setAddr(itInput.dato()->getAddr() );
				this->ListaTranIn.insertar(pTxInput);
			}
			catch (std::bad_alloc& ba)	{std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
			itInput.avanzar();
		}
		this->n_tx_out = otherTrans.n_tx_out;
		while(!itOutput.extremo())
		{
			try {
				TransactionOutput * pTxOutput = new TransactionOutput;
				pTxOutput->setValue(itOutput.dato()->getValue());
				pTxOutput->setAddr(itOutput.dato()->getAddr());
				this->ListaTranOut.insertar(pTxOutput);
			}
			catch (std::bad_alloc& ba)	{std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
			itOutput.avanzar();
		}
}


//Descripcion: Instancia el objeto Transaction a partir de un archivo raw_t
//Precondicion:
//Postcondicion: Dos punteros a memoria de tama�o definido creados y
// precargados con los datos de raw_t
Transaction::Transaction( const raw_t & Raw ){
	//TODO preparar Transaction para una cadena de Raw
	this->n_tx_in = Raw.inTx;
	for(int i = 0; i < this->n_tx_in ;i++ )
	{
		try {
			TransactionInput * pTxInput = new TransactionInput;
			pTxInput->setTxId(Raw.IN_tableOfTxId[i]);
			pTxInput->setIdx(Raw.IN_tableOfIndex[i]);
			pTxInput->setAddr(Raw.IN_tableOfAddr[i]);
			this->ListaTranIn.insertar(pTxInput);
		}
		catch (std::bad_alloc& ba)
		{
			std::cerr << "bad_alloc caught: " << ba.what() << '\n';
		}
	}
	this->n_tx_out = Raw.outTx;
	for(int i = 0; i < this->n_tx_out ;i++ )
	{
		try {
			TransactionOutput * pTxOutput = new TransactionOutput;
			pTxOutput->setValue(Raw.OUT_tableOfValues[i]);
			pTxOutput->setAddr(Raw.OUT_tableOfAddr[i]);
			this->ListaTranOut.insertar(pTxOutput);
		}
		catch (std::bad_alloc& ba)
		{
			std::cerr << "bad_alloc caught: " << ba.what() << '\n';
		}
	}
}

Transaction::Transaction(int n_tx_in,int n_tx_out){
	this->n_tx_in = n_tx_in;
	if(this->n_tx_in ){
		for(int i = 0; i < this->n_tx_in ;i++ )
		{
			try {
				TransactionInput * pTxInput = new TransactionInput;
				this->ListaTranIn.insertar(pTxInput);
			}
			catch (std::bad_alloc& ba) { std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
		}
	}
	this->n_tx_out = n_tx_out;
	if(this->n_tx_out){
		for(int i = 0; i < this->n_tx_out ;i++ )
		{
			try {
				TransactionOutput * pTxOutput = new TransactionOutput;
				this->ListaTranOut.insertar(pTxOutput);
			}
			catch (std::bad_alloc& ba)	{std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
		}
	}
}


//Descripcion: Destruye elemento de Transaction
//Precondicion: Si se envia una transaccion nula no es necesario que se realice accion
//Postcondicion: Objeto destruido, memoria liberada, punteros a null y parametros a cero.
Transaction::~Transaction(){
	if ( ! this->ListaTranIn.vacia() ) {
		lista <TransactionInput *>::iterador it(ListaTranIn);
		it = this->ListaTranIn.primero();
		do{
			if(it.dato() != NULL){
				delete it.dato();
				it.dato() = NULL;
			}
			it.avanzar();
		}while ( ! it.extremo() );
	}
	if ( ! this->ListaTranOut.vacia() ) {
		lista <TransactionOutput *>::iterador it(ListaTranOut);
		it = this->ListaTranOut.primero();
		do {
			if(it.dato() != NULL){
				delete it.dato();
				it.dato() = NULL;
			}
			it.avanzar();
		}while ( ! it.extremo() );
	}
}

//---Getters---//

//Descripcion: Devuelve cantidad de transacciones de input
//Precondicion:
//Postcondicion:
int Transaction::getNumTransactionIn(){
	return this->n_tx_in;
}

//Descripcion: Devuelve cantidad de transacciones de output
//Precondicion:
//Postcondicion:
int Transaction::getNumTransactionOut(){
	return this->n_tx_out;
}

//Descripcion: Obtiene la transaccion de la lista de entradas
//Precondicion: Si el indice esta fuera de rango debe devolver null
//Postcondicion:
TransactionInput * Transaction::getTransactionInput(int index){
	size_t index_ = (size_t)index;
	if( index < 0 || index_ > this->ListaTranIn.tamano()){
		std::cerr<<"Fuera de Rango"<<std::endl;
		std::abort();
	}
	else{
		lista <TransactionInput *>::iterador it(this->ListaTranIn);
		int counter = 1;
		while(counter != index){
			it.avanzar();
			counter++;
		}
		return it.dato();
	}
}

//Descripcion: Obtiene la transaccion de la lista de salidas
//Precondicion:  Si el indice esta fuera de rango debe devolver null
//Postcondicion:
TransactionOutput * Transaction::getTransactionOutput(int index){
	size_t index_ = (size_t)index;
	if( index < 0 || index_ > this->ListaTranOut.tamano()){
		std::cerr<<"Fuera de Rango"<<std::endl;
		std::abort();
	}
	else{
		lista <TransactionOutput *>::iterador it(this->ListaTranOut);
		int counter = 1;
		while(counter != index){
			it.avanzar();
			counter++;
		}
	return it.dato();
	}
}

lista <TransactionInput *> Transaction::getTransactionInputList() {
	return this->ListaTranIn;
}

lista <TransactionOutput *> Transaction::getTransactionOutputList() {
	return this->ListaTranOut;
}

//Descripcion: Devuelve un string de los valores concatenados de la listas
//para ser aplicado el hash correspondiente por fuera
//Precondicion: Se considera todo precargado antes
//Postcondicion:
std::string Transaction::getConcatenatedTransactions( void ){
       lista <TransactionInput *>::iterador itIn(this->ListaTranIn);
       lista <TransactionOutput *>::iterador itOut(this->ListaTranOut);
       //std::ostringstream concatenation;
       std::string concatenation;
// 	   concatenation << this->n_tx_in << '\n';
       concatenation += std::to_string( this->n_tx_in );
	   concatenation += '\n';
       for(itIn = ListaTranIn.primero(); !itIn.extremo() ; itIn.avanzar()){
//               concatenation<< itIn.dato()->getTxId() <<' ';
//               concatenation<< itIn.dato()->getIdx()  <<' ';
//               concatenation<< itIn.dato()->getAddr() <<'\n';
    	   	   	 concatenation += itIn.dato()->getTxId();
    	  		 concatenation += ' ';
    	  		 concatenation += std::to_string( itIn.dato()->getIdx() );
    	  		 concatenation += ' ';
    	  		 concatenation += itIn.dato()->getAddr();
    	  	     concatenation += '\n';

      }
      concatenation += std::to_string( this->n_tx_out );
      concatenation += '\n';
// 	  concatenation << this->n_tx_out << '\n';
      for(itOut = ListaTranOut.primero(); !itOut.extremo() ; itOut.avanzar()){
//               concatenation<< itOut.dato()->getValue() <<' ';
//               concatenation<< itOut.dato()->getAddr()  <<'\n';
    	  	  	 concatenation += float_to_string_w_precision( itOut.dato()->getValue() , 1 );
    	  	  	 //concatenation += std::to_string(itOut.dato()->getValue());
    	  	  	 concatenation += ' ';
    	  	  	 concatenation += itOut.dato()->getAddr();
    	  	  	 concatenation += '\n';
       }
      //std::cout <<concatenation.str()<<std::endl; //DEBUG
      //return concatenation.str();
      return concatenation;
}


std::string Transaction::float_to_string_w_precision(float val, int p)
{
	if( p < 0 ) return "";
	p = (unsigned int) p;
	std::stringstream stream;
	stream << std::fixed << std::setprecision(p) << val;
	return stream.str();
}

Transaction & Transaction::operator=(Transaction &tr){
	lista <TransactionInput *>::iterador itInput( tr.ListaTranIn);
	lista <TransactionOutput*>::iterador itOutput( tr.ListaTranOut);
	this->n_tx_in = tr.n_tx_in;
	if(this->n_tx_in ){
		do{
			try {
				TransactionInput * pTxInput = new TransactionInput;
				pTxInput->setTxId(itInput.dato()->getTxId() );
				pTxInput->setIdx(itInput.dato()->getIdx()   );
				pTxInput->setAddr(itInput.dato()->getAddr() );
				this->ListaTranIn.insertar(pTxInput);
			}
			catch (std::bad_alloc& ba)	{std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
			itInput.avanzar();
		}while(!itInput.extremo() );
	}
	this->n_tx_out = tr.n_tx_out;
	if(this->n_tx_out ){
		do{
			try {
				TransactionOutput * pTxOutput = new TransactionOutput;
				pTxOutput->setValue(itOutput.dato()->getValue());
				pTxOutput->setAddr(itOutput.dato()->getAddr());
				this->ListaTranOut.insertar(pTxOutput);
			}
			catch (std::bad_alloc& ba)	{std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
			itOutput.avanzar();
		}while(!itOutput.extremo());
	}
	return *this;
}

void Transaction::addTransactionInput(void){
	this->n_tx_in++;
	try {
		TransactionInput * pTxInput = new TransactionInput;
		this->ListaTranIn.insertar(pTxInput);
	} catch (std::bad_alloc& ba) { std::cerr << "bad_alloc caught: " << ba.what() << '\n';}
}
void Transaction::addTransactionOutput(void){
	this->n_tx_out++;
	try {
		TransactionOutput * pTxOutput = new TransactionOutput;
		this->ListaTranOut.insertar(pTxOutput);
	} catch (std::bad_alloc& ba) { std::cerr << "bad_alloc caught: " << ba.what() << '\n';}

}
