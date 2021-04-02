
#include "BlockChainFileManager.h"

// BlockChainFileManager::fileList es una lista estatica
// visible para cualquier instancia de filemanager.
// En esta se encuentran todos los archivos abiertos y operados
// por filemanager. BlockChainFileManager::fileList solo puede contener
// un archivo de cada type.
lista<file_t *> BlockChainFileManager::fileList;


// Descripcion: Constructor de un objeto filemanager
// Precondicion:
// PostCondicion: Atributos inicializado a valores adecuados.
BlockChainFileManager::BlockChainFileManager() {
	this->pRawData = NULL;
}

// Descripcion: Destructor de un objeto filemanager
// Precondicion: Si el objeto pidio memoria dinamica, debe liberarse y
// cargarse a valores adecuados.
// PostCondicion:
BlockChainFileManager::~BlockChainFileManager() {
		if(this->pRawData != NULL){
		pRawData->inTx = 0;
		delete [] pRawData->IN_tableOfTxId; 		pRawData->IN_tableOfTxId    = NULL;
		delete [] pRawData->IN_tableOfIndex;		pRawData->IN_tableOfIndex   = NULL;
		delete [] pRawData->IN_tableOfAddr;			pRawData->IN_tableOfAddr    = NULL;
		pRawData->outTx = 0;
		delete [] pRawData->OUT_tableOfValues;		pRawData->OUT_tableOfValues = NULL;
		delete [] pRawData->OUT_tableOfAddr;		pRawData->OUT_tableOfAddr   = NULL;
		delete pRawData;
		pRawData = NULL;
	}
	if ( ! this->userBlockChain.vacia() ) {
			lista  <Block *>::iterador it( this->userBlockChain );
			it = this->userBlockChain.primero();
			while ( ! this->userBlockChain.isEmpty()) {
				delete it.dato();
				this->userBlockChain.eliminar_nodo(it);
			}
		}
}

// Descripcion: Devuelve en un puntero a fstream pasado como argumento el valor de
// fs correspondiente al file_t de la lista de archivos.
// Precondicion: type debe ser pasado correctamente.
// PostCondicion: Devuelve true/false si encuentra o no dicho file_t en la lista.
bool BlockChainFileManager::getFilefromList(FileTypes type,std::fstream ** fs){
	lista<file_t *>::iterador it(BlockChainFileManager::fileList);
	while(! it.extremo())
	{
		if(it.dato()->type == type){
			(*fs) = & (it.dato()->fs);
			return true;
		}
		it.avanzar();
	}
	return false;
}

// Descripcion: Devuelve en un puntero a istream pasado como argumento el valor de
// iss correspondiente al file_t de la lista de archivos.
// Precondicion: type debe ser pasado correctamente.
// PostCondicion: Devuelve true/false si encuentra o no dicho file_t en la lista.
bool BlockChainFileManager::getIssfromList(FileTypes type,std::istream ** iss){
	lista<file_t *>::iterador it(BlockChainFileManager::fileList);
	while(! it.extremo())
	{
		if(it.dato()->type == type){
			(*iss) = it.dato()->iss;
			return true;
		}
		it.avanzar();
	}
	return false;
}

// Descripcion: Devuelve en un puntero a ostream pasado como argumento el valor de
// oss correspondiente al file_t de la lista de archivos.
// Precondicion: type debe ser pasado correctamente.
// PostCondicion: Devuelve true/false si encuentra o no dicho file_t en la lista.
bool BlockChainFileManager::getOssfromList(FileTypes type,std::ostream ** oss){
	lista<file_t *>::iterador it(BlockChainFileManager::fileList);
	while(! it.extremo())
	{
		if(it.dato()->type == type){
			(*oss) = it.dato()->oss;
			return true;
		}
		it.avanzar();
	}
	return false;
}



// Descripcion: Devuelve en un puntero a ostream pasado como argumento el valor de
// oss correspondiente al file_t de la lista de archivos.
// Precondicion: type debe ser pasado correctamente.
// PostCondicion: Devuelve true/false si encuentra o no dicho file_t en la lista.
bool BlockChainFileManager::getIsStandarfromList(FileTypes type){
	lista<file_t *>::iterador it(BlockChainFileManager::fileList);
	while(! it.extremo())
	{
		if(it.dato()->type == type){
			return it.dato()->isStandard;
		}
		it.avanzar();
	}
	return false;
}

// Descripcion: Es funcion hace una traduccion del lenguaje de archivo a un tipo de dato conocido
// por el sistema (payload_t). Basicamente sera llamada siempre que haya comandos por ingresar o si hay un txt con comandos
// y los parseara en un tipo de dato payload_t. Este tipo de dato permite limitar el alcance del archivo solo
// al scope de FileManager puesto que pasada esta funcion no existe el concepto de archivo y se utiliza el de payload_t.
// Precondicion: Se le da un payload que debe ser inicializado a valores seguros previo a la conversion.
// PostCondicion: Si los comandos son validos debe rellenar el payload.
status_t BlockChainFileManager::translateCommands( payload_t & payload){
	std::istream * iss;
	//Obtengo el archivo de comandos de la lista de archivos
	if( ! this->getIssfromList(FileTypes::userCommandInputFiles,&iss)) return STATUS_BAD_READ_INPUT_FILE;

	std::string command,line;
	int subStringNum = 1;
	bool state;
	bool eof = false;
	Commands commandType = Commands::commandNotDefined;

	//Preinicializo el pyaload en valores seguros
	this->safeValuePayload(payload);

	//Detecto de la primer linea, solo el comando
	if (std::getline(*iss,line).eof() )	eof = true;

	//Compatibilidad de archivos de Windows
	if(line.back() == '\r')	line.pop_back();

	//Encuentro el primer delimitador de comando
	command = getSubString(line, ' ', subStringNum++, &state);

	//Valido y obtengo el comando
	if( !state )										return STATUS_ERROR_COMMAND_PAYLOAD;
	if( !isOnValidCommandTable(command,commandType) ) 	return STATUS_ERROR_COMMAND_NOT_FOUND;

	//Completo el payload en base al comando
	switch(commandType)
	{
		case Commands::init:
			{
				//std::cout<< "init" << std::endl;
				std::string usr,value,bits;

				usr =  getSubString(line, ' ', subStringNum++, &state);		if( !state ) return STATUS_ERROR_COMMAND_PAYLOAD;
				value = getSubString(line, ' ',subStringNum++, &state);		if( !state ) return STATUS_ERROR_COMMAND_PAYLOAD;
				bits = getSubString(line, ' ', subStringNum++, &state);		if( !state ) return STATUS_ERROR_COMMAND_PAYLOAD;

				//------------Debug -------------//
				//std::cout<< usr << std::endl;
				//std::cout<< value << std::endl;
				//std::cout<< bits << std::endl;
				if (! isPositiveIntNumberFromString(value))  return STATUS_ERROR_COMMAND_PAYLOAD;
				if (! isPositiveIntNumberFromString(bits))   return STATUS_ERROR_COMMAND_PAYLOAD;

				payload.command = commandType;
				payload.user = usr;
				payload.value = std::stof(value);
				payload.bits = std::stoi(bits);	;
			}
			break;
		case Commands::transfer:
			{
				//std::cout<< "transfer" << std::endl;
				std::string src,dest,value;
				//size_t inTx = 1 ;
				//size_t outTx = 0;

				payload.ArgTranfer = new Queue<string>;
				this->argBuffer = payload.ArgTranfer;

				bool MoreParameters;

				src = getSubString(line, ' ', subStringNum++, &state);	if( !state ) return STATUS_ERROR_COMMAND_PAYLOAD;
				payload.ArgTranfer->enqueue(src);
				// Esta pasada valida, aucumula los datos y cuenta la cantidad de destinos
				// Para luego pedir memoria dinamica con datos veraces.
				do{
					dest = getSubString(line, ' ', subStringNum++, &MoreParameters);	if (! MoreParameters) break;
					value = getSubString(line, ' ', subStringNum++, &state);

					if( !state ) 									return STATUS_ERROR_COMMAND_PAYLOAD;
					if (! isPositiveFloatNumberFromString(value))  	return STATUS_ERROR_COMMAND_PAYLOAD;

					payload.ArgTranfer->enqueue(dest);
					payload.ArgTranfer->enqueue(value);
				}while( MoreParameters );

				// Comienzo a rellenar el payload

				//Creo el archivo raw_t en el entorno del filemanager
//				this->pRawData = new raw_t{0};
//				if(pRawData == NULL) return STATUS_BAD_ALLOC;
//				pRawData->inTx = inTx;
//
//				pRawData->IN_tableOfTxId = new std::string[pRawData->inTx];
//				pRawData->IN_tableOfIndex = new int[pRawData->inTx];
//				pRawData->IN_tableOfAddr = new std::string[pRawData->inTx];
//
//				if(		pRawData->IN_tableOfTxId == NULL  ||
//						pRawData->IN_tableOfIndex == NULL ||
//						pRawData->IN_tableOfAddr == NULL ) 	return STATUS_BAD_ALLOC;
//
//				for(int i = 0; i < pRawData->inTx; i++)
//				{
//					// El outpoint en esta instancia no puede parsearse puesto que como comando
//					// no se tiene esa informacion. Tampoco se tiene un hash de la direccion por
//					// se envia en el addr el string del nombre.
//					pRawData->IN_tableOfTxId[i]  = "";
//					pRawData->IN_tableOfIndex[i] = -1;
//					pRawData->IN_tableOfAddr[i]  = argQueue.dequeue();
//				}
//
//				pRawData->outTx = outTx;
//				pRawData->OUT_tableOfValues = new float[pRawData->outTx];
//				pRawData->OUT_tableOfAddr = new std::string[pRawData->outTx];
//
//				if(		pRawData->OUT_tableOfValues == NULL  ||
//						pRawData->OUT_tableOfAddr   == NULL  ) 	return STATUS_BAD_ALLOC;
//
//				for(int i = 0; i < pRawData->outTx; i++)
//				{
//
//					pRawData->OUT_tableOfAddr[i] = argQueue.dequeue();
//					pRawData->OUT_tableOfValues[i] = std::stof( argQueue.dequeue() );
//				}
//
//				// Como el Raw data no esta completamente relleno puesto
//				// que no se tiene toda la data necesaria para el minado
//				// se levanta el completeFlag en falso
//				pRawData->completeFlag = false;
				payload.command = commandType;
				//payload.pRaw = pRawData;
			}
		break;
		case Commands::mine:
			{
				//std::cout<< "mine" << std::endl;
				std::string bits;

				bits = getSubString(line, ' ', subStringNum++, &state);

				if( !state ) 								return STATUS_ERROR_COMMAND_PAYLOAD;
				if (!isPositiveIntNumberFromString(bits))  	return STATUS_ERROR_COMMAND_PAYLOAD;

				//------------Debug -------------//
				//std::cout<< bits << std::endl;
				payload.command = commandType;
				payload.bits = std::stoi(bits);

			}
			break;
		case Commands::block:
			{
				//std::cout<< "block" << std::endl;
				std::string id;
				id =  getSubString(line, ' ', subStringNum++, &state);

				//------------Debug -------------//
				//std::cout<< id << std::endl;
				if( !state ) 						return STATUS_ERROR_COMMAND_PAYLOAD;
				if( ! this->isHashFromString(id) )	return STATUS_ERROR_COMMAND_PAYLOAD;

				payload.command = commandType;
				payload.id = id;
			}
			break;
		case Commands::balance:
			{
				//std::cout<< "balance" << std::endl;
				std::string usr;
				usr = getSubString(line, ' ', subStringNum++, &state);

				if( !state ) 						return STATUS_ERROR_COMMAND_PAYLOAD;

				//------------Debug -------------//
				//std::cout<< usr << std::endl;

				payload.command = commandType;
				payload.user = usr;
			}
			break;
		case Commands::txn:
			{
				//std::cout<< "txn" << std::endl;
				std::string id;
				id = getSubString(line, ' ', subStringNum++, &state);

				//------------Debug -------------//
				//std::cout<< id << std::endl;
				if( !state ) 					return STATUS_ERROR_COMMAND_PAYLOAD;
				if( !isHashFromString(id) )		return STATUS_ERROR_COMMAND_PAYLOAD;

				payload.command = commandType;
				payload.id = id;
			}
			break;
		case Commands::load:
			{
				//std::cout<< "load" << std::endl;
				std::string filename;
				filename = getSubString(line, ' ', subStringNum++, &state);

				//------------Debug -------------//
				//std::cout<< filename << std::endl;

				payload.command = commandType;
				payload.filename = filename;
			}
			break;
		case Commands::save:
			{
				//std::cout<< "save" << std::endl;
				std::string filename;
				filename = getSubString(line, ' ', subStringNum++, &state);

				//------------Debug -------------//
				//std::cout<< filename << std::endl;

				payload.command = commandType;
				payload.filename = filename;
			}
			break;
		default:
			break;
	}

	// Evitar problema de ultima linea con \n y getline en archivos
	if (!this->getIsStandarfromList(FileTypes::userCommandInputFiles)){
		static unsigned int lastCharpos;
		static bool hasEndOfLine = this->hasNewLineAtTheEnd(iss,&lastCharpos);
		if( hasEndOfLine){
		//	cout<<lastCharpos<<endl;
		//	cout<<iss->tellg()<<endl;
			if(abs(lastCharpos - iss->tellg() ) == 1){
				return STATUS_NO_MORE_COMMANDS;
			}
		}
	}
	if( eof == true)
		return STATUS_NO_MORE_COMMANDS;
	return STATUS_READING_COMMANDS;
}

bool BlockChainFileManager::hasNewLineAtTheEnd(std::istream *iss,unsigned int * pos){
	unsigned int prevPos = iss->tellg();
	iss->seekg(-1,ios_base::end);    // go to one position before the EOF
	if(pos) *pos = iss->tellg();
	char c;
	iss->get(c);                     // Read current character
	iss->seekg(prevPos,ios_base::beg);
	if(c =='\n'){
		return true;
	}else
		return false;
}



// Descripcion: Verifica si el comando esta en la tabla especificada de comandos y si lo encuentra
// devuelve en commandType el tipo enumerativo asociado a ese comando.
// Precondicion: CommandType se debe precargar con algo no definido antes de buscar.
// PostCondicion: Devuelve verdadero o falso
bool BlockChainFileManager::isOnValidCommandTable(std::string command, Commands & commandType)
{
	size_t i;
	size_t size = BlockChainFileManager::getNumberOfValidFunctions();
	commandType = Commands::commandNotDefined;
	bool functionIsOnTable = false;
	for(i = 0 ; i < size ; ++i)
	{
		if( command.compare(_commands_[i]) == 0 )
		{
			functionIsOnTable = true;
			commandType = static_cast<Commands>(i);
			break;
		}
	}

	return functionIsOnTable;
}

// Descripcion:
// Precondicion:
// PostCondicion:
unsigned int BlockChainFileManager::getNumberOfValidFunctions()
{
	return sizeof(_commands_)/sizeof(std::string);
}

// Descripcion: Metodo para cargar los valores por defecto del tipo de dato Payload_t
// Precondicion:
// PostCondicion: Todos los campos de Payload_t inicializados a valores correctos.
void BlockChainFileManager::safeValuePayload(payload_t & payload){
	static bool firstTime = true;
	if (!firstTime){
		if(payload.ArgTranfer!= NULL){
			delete payload.ArgTranfer;
		}
	}
	else
		firstTime = false;
	payload.ArgTranfer = NULL;
	payload.command = Commands::commandNotDefined;
	//payload.pRaw = NULL;
	payload.filename = "";
	payload.id = "";
	payload.user = "";
	payload.value = 0;
	payload.bits = 0;

}

// Descripcion: Este metodo recibe un tipo de dato file_t, abre el archivo asociado y lo agrega a la lista
// de file_t de FileManager. Debe ser usado en conjunto con removeFile o removeAllFiles en el momento de
// terminar el trabajo de fileManager.
// Precondicion: Tiene que recibir un file_t incompleto pero con los campos filename, type y mode completos
// PostCondicion: Archivo abierto con los campos del file_t correctamente inicializados iss, oss, fs
status_t BlockChainFileManager::addFile( file_t & newFile ){

	file_t * pFile = new file_t;
	pFile->fileID = newFile.fileID;
	pFile->mode = newFile.mode;
	pFile->type = newFile.type;
	pFile->isStandard = newFile.isStandard;



	if( pFile->mode == ios::in){
		if(pFile->isStandard ){
			pFile->iss = & cin;
		}
		else{
			pFile->fs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
			try {
				pFile->fs.open(pFile->fileID,pFile->mode);
			}catch (std::system_error& e) {
				std::cerr << "Exception opening/reading/closing file error: " << e.code().message() << "\n";
				delete pFile;
				return STATUS_BAD_READ_INPUT_FILE;
			}
			pFile->iss = & pFile->fs;

		}
		pFile->oss = NULL;
	}
	else if( pFile->mode == ios::out){
		if(pFile->isStandard){
			pFile->oss = & cout;
		}
		else{
			pFile->fs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
			try {
				pFile->fs.open(pFile->fileID,pFile->mode);
			}catch (std::system_error& e) {
				std::cerr << "Exception opening/reading/closing file error: " << e.code().message() << "\n";
				delete pFile;
				return STATUS_BAD_READ_INPUT_FILE;
			}
			pFile->oss = & pFile->fs;
		}
		pFile->iss = NULL;
	}


	fileList.insertar(pFile);
	return STATUS_OPEN_FILE_SUCCESSFULY;
}

// Descripcion: Destruye el elemento type de la lista de archivos, liberando memoria
// y cerrando el archivo asociado con el tipo type.
// Este metodo debe invocarse siempre que se uso el metodo AddNewFile.
// Precondicion: Debe usarse al final de utilizar AddNewFile.
// PostCondicion: Elemento de la lista liberado, archivo del del elemento cerrado
// y variables post inicializadas.

status_t BlockChainFileManager::removeFile(FileTypes type){
	lista<file_t *>::iterador it(BlockChainFileManager::fileList);
	while(!it.extremo()){
		if(it.dato()->type == type){
			it.dato()->fileID = "";
			it.dato()->iss = NULL;
			it.dato()->oss = NULL;
			it.dato()->isStandard = NULL;
			it.dato()->type = FileTypes::indefinedFile;
			if (! it.dato()->isStandard ){
				it.dato()->fs.close();
			}
			delete it.dato();
			fileList.eliminar_nodo(it);
			break;
		}
		it.avanzar();
	}
	// TODO IMPLEMENTAR UN METODO MEJOR DE BORRADO PUESTO QUE ESTE NO
	// OBEDECE LA DIRECCION DEL DATO

	return STATUS_CLOSE_FILE_SUCCESSFULY;
}

// Descripcion: Destruye la lista de file_t del FileManager, liberando toda la memoria pedida
// cerrando los archivos abiertos y postinicializando los valores del file_t a valores nulos.
// Este metodo debe invocarse siempre que se uso el metodo AddNewFile.
// Precondicion: Debe usarse al final de utilizar AddNewFile.
// PostCondicion: Lista completamente liberada, todos los archivos que fueron abiertos cerrados
// y variables post inicializadas.
status_t BlockChainFileManager::removeAllFiles(){
	lista<file_t *>::iterador it(BlockChainFileManager::fileList);
	it = fileList.primero();
	while(!fileList.isEmpty()){
		it.dato()->fileID = "";
		it.dato()->iss = NULL;
		it.dato()->oss = NULL;
		it.dato()->type = FileTypes::indefinedFile;
		if (! it.dato()->isStandard ){
			it.dato()->fs.close();
		}
		delete it.dato();
		fileList.eliminar_nodo(it);
	}
	return STATUS_CLOSE_FILE_SUCCESSFULY;
}

static FileTypes GlobalType = FileTypes::userCommandResponseFiles;


// Descripcion: Sobrecarga del operador << para tipos FileTypes. Este metodo
// carga la variable global GlobalType con el tipo de archivo donde se desea
// imprimir el mensaje y luego junto la sobrecarga de << para strings imprime
// el mensaje.
// Precondicion:
// PostCondicion:Variable global cargada con el valor type pasada como argumentos
BlockChainFileManager& BlockChainFileManager::operator<<(FileTypes type){
	GlobalType = type;
	return *this;
}


// Descripcion: Sobrecarga del operador << para strings. Este metodo imprime el mensaje
// en el archivo determinado por la variable global GlobalType. Por defecto los mensajes
// siempre se imprimen en el archivo con asociado al tipo FileTypes::userCommandResponseFiles.
// Sin embargo, puede cambiarse usando el metodo de sobrecarga << de Filetypes para cambiar
// el archivo de salida.
// Precondicion: Si no encuentra el archivo especificado no imprime nada.
// PostCondicion: Mensaje impreso en el archivo asociado al tipo definido por GlobalType
BlockChainFileManager& BlockChainFileManager::operator<<(std::string message){
	std::ostream * oss;
	if(! this->getOssfromList(GlobalType, &oss)) return *this;
	*oss << message;
	return *this;
}

// Descripcion:
// Precondicion:
// PostCondicion:
status_t BlockChainFileManager::convert(FileTypes type, const lista <Block *> & BlockChain){
	ostream * oss;
	switch(type){
	case FileTypes::saveBlockChainFile:
		if( !this->getOssfromList(type,&oss) ) return STATUS_BAD_READ_OUTPUT_FILE;
		return this->convert(oss, BlockChain);
		break;
	case FileTypes::userCommandResponseFiles:
		if( !this->getOssfromList(type,&oss) ) return STATUS_BAD_READ_OUTPUT_FILE;
		return this->convert(oss, BlockChain);
		break;
	case FileTypes::userCommandInputFiles:
	case FileTypes::loadBlockChainFile:
	default:
		return STATUS_NO_BLOCKS_TO_CONVERT;
		break;
	}
	return STATUS_BAD_READ_OUTPUT_FILE;
}

status_t BlockChainFileManager::convert(FileTypes type, const lista <Transaction *> & listaTran){
	switch(type){
	case FileTypes::saveBlockChainFile:
		break;
	case FileTypes::userCommandResponseFiles:
		ostream * oss;
		if( !this->getOssfromList(type,&oss) ) return STATUS_BAD_READ_OUTPUT_FILE;
		return this->convert(oss, listaTran);
		break;
	case FileTypes::userCommandInputFiles:
	case FileTypes::loadBlockChainFile:
	default:
		return STATUS_NO_BLOCKS_TO_CONVERT;
		break;
	}
	return STATUS_BAD_READ_OUTPUT_FILE;
}


// Descripcion:
// Precondicion:
// PostCondicion:
status_t BlockChainFileManager::convert(std::ostream * oss, const lista <Block *> & BlockChain){
	lista <Block *> ::iterador it(BlockChain);
	it = BlockChain.ultimo();

	if(!oss->good())						return STATUS_BAD_READ_OUTPUT_FILE;
	if( BlockChain.vacia() )				return STATUS_NO_BLOCKS_TO_CONVERT;
	while(!it.extremo()){
		*oss << it.dato()->getpre_block() << '\n';
		*oss << it.dato()->gettxns_hash() << '\n';
		*oss << it.dato()->getbits( )	  << '\n';
		*oss << it.dato()->getnonce()	  << '\n';
		*oss << it.dato()->gettxn_count() << '\n';
		*oss << it.dato()->getcadenaprehash();
		it.retroceder();
	}
	return STATUS_FINISH_CONVERT_SUCCESSFULY;
}

status_t BlockChainFileManager::convert(std::ostream * oss, const lista <Transaction *> & listaTran){
	lista <Transaction *> ::iterador it(listaTran);
	it = listaTran.ultimo();

	if(!oss->good())						return STATUS_BAD_READ_OUTPUT_FILE;
	if( listaTran.vacia() )					return STATUS_NO_BLOCKS_TO_CONVERT;
	while(!it.extremo()){
		*oss << it.dato()->getConcatenatedTransactions();
		it.retroceder();
	}
	return STATUS_FINISH_CONVERT_SUCCESSFULY;
}
#define BLOCK_1_LINE_PREVIOUS_HASH 1
#define BLOCK_2_LINE_BLOCK_HASH 2
#define BLOCK_3_LINE_BITS 3
#define BLOCK_4_LINE_NONCE 4
#define BLOCK_5_LINE_CANT_TX 5
#define BLOCK_TRANSAC_TX_INPUT_INDEX 6
#define BLOCK_TRANSAC_INPUTS 7
#define BLOCK_TRANSAC_TX_OUTPUT_INDEX 8
#define BLOCK_TRANSAC_OUTPUTS 9

status_t BlockChainFileManager::validateBlockChain(void){
	std::istream * iss;
	if(! this->getIssfromList(FileTypes::loadBlockChainFile, &iss)) return STATUS_BAD_READ_INPUT_FILE;

	std::string line,aux;
	unsigned int blockLineCounter = 0, trasnferCounter = 0;
	unsigned int inTx,outTx,cantTx;
	bool isCompleteBlock = false;
	bool isEof = false;


	while( std::getline(*iss, line)){
		// Verifico si llegue al fin de archivo
		if ((*iss).eof()) {
			isEof = true;
		}
		//Compatibilidad de archivos de Windows
		if(line.back() == '\r')
			line.pop_back();

		blockLineCounter++;
		switch(blockLineCounter){
		case BLOCK_1_LINE_PREVIOUS_HASH:
			isCompleteBlock = false;
			if( this->isHashFromString(line) == false){			  	return STATUS_CORRUPT_FORMAT_BAD_HASH;} break;
		case BLOCK_2_LINE_BLOCK_HASH:
			if( this->isHashFromString(line) == false){			  	return STATUS_CORRUPT_FORMAT_BAD_HASH;} break;
		case BLOCK_3_LINE_BITS:
			if( this->isPositiveIntNumberFromString(line) == false){return STATUS_CORRUPT_FORMAT_BAD_BITS;} break;
		case BLOCK_4_LINE_NONCE:
			if( this->isPositiveIntNumberFromString(line) == false){return STATUS_CORRUPT_FORMAT_BAD_NONCE;}break;
		case BLOCK_5_LINE_CANT_TX:
			if( this->isPositiveIntNumberFromString(line) == false){return STATUS_CORRUPT_FORMAT_BAD_NONCE;}
			cantTx = getPositiveNumberFromString(line);
			break;
		case BLOCK_TRANSAC_TX_INPUT_INDEX:
			if( this->isPositiveIntNumberFromString(line) == false){return STATUS_CORRUPT_FORMAT_BAD_TXIN;}
			inTx = getPositiveNumberFromString(line);
			if(inTx)
				break;
			blockLineCounter++;
		case BLOCK_TRANSAC_INPUTS:
			if(inTx){
				trasnferCounter ++;
				if( this->isHashFromString(this->getSubString(line,' ', 1)) == false){
					return STATUS_CORRUPT_FORMAT_BAD_HASH;}
				if( this->isPositiveIntNumberFromString(this->getSubString(line,' ', 2)) == false){
					return STATUS_CORRUPT_FORMAT_BAD_TXINDEX;}
				if( this->isHashFromString(this->getSubString(line,' ', 3)) == false){
					return STATUS_CORRUPT_FORMAT_BAD_HASH;}
			}
			if (trasnferCounter != inTx) blockLineCounter--;
			else trasnferCounter = 0;
			break;
		case BLOCK_TRANSAC_TX_OUTPUT_INDEX:
			if( this->isPositiveIntNumberFromString(line) == false){ return STATUS_CORRUPT_FORMAT_BAD_TXOUT;}
			outTx = getPositiveNumberFromString(line);
			if (outTx) break;
		case BLOCK_TRANSAC_OUTPUTS:
			if (outTx){
				trasnferCounter ++;
				if( this->isPositiveFloatNumberFromString(this->getSubString(line,' ', 1)) == false){
					return STATUS_CORRUPT_FORMAT_BAD_BTCVALUE;}
				if( this->isHashFromString(this->getSubString(line,' ', 2)) == false){
					return STATUS_CORRUPT_FORMAT_BAD_HASH;}
			}
			if (trasnferCounter != outTx) blockLineCounter--;
			else{
				if (cantTx ==1){
					trasnferCounter = 0;
					blockLineCounter = 0;
					isCompleteBlock = true;
				}else{
					trasnferCounter = 0;
					blockLineCounter = BLOCK_5_LINE_CANT_TX;
					cantTx--;
					return STATUS_ERROR_LIST_OF_TRANSACT_NOT_SUPPORTED;
				}
			}
			break;
		}


		// Evitar problema de ultima linea con \n y getline en archivos
		if (!this->getIsStandarfromList(FileTypes::loadBlockChainFile)){
			static unsigned int lastCharpos;
			static bool hasEndOfLine = this->hasNewLineAtTheEnd(iss,&lastCharpos);
			if( hasEndOfLine){
			//	cout<<lastCharpos<<endl;
			//	cout<<iss->tellg()<<endl;
				if(abs(lastCharpos - iss->tellg() ) == 1){
					if (isCompleteBlock)
						return STATUS_OK;
					else
						return STATUS_BAD_READ_INPUT_FILE;
				}
			}
		}

		// Si se leyo un bloque completo y es el fin de archivo
		if (isEof){
			if (isCompleteBlock)
				return STATUS_OK;
			else
				return STATUS_BAD_READ_INPUT_FILE;
		}
	}
	return STATUS_OK;
}

status_t BlockChainFileManager::loadBlockChain(void){
	std::istream * iss;
	if(! this->getIssfromList(FileTypes::loadBlockChainFile, &iss)) return STATUS_BAD_READ_INPUT_FILE;
	iss->clear();
	iss->seekg(0, iss->beg);

	std::string line,aux;
	unsigned int blockLineCounter = 0, trasnferCounter = 0;
	unsigned int inTx,outTx,cantTx;
	bool isCompleteBlock = false;
	bool isEof = false;
	bool finishLoading = false;

	while (!finishLoading){
		Block * newBlock = new Block;
		Transaction * newTrans = new Transaction;

		while( std::getline(*iss, line)){
			// Verifico si llegue al fin de archivo
			if ((*iss).eof()) {
				isEof = true;
			}
			//Compatibilidad de archivos de Windows
			if(line.back() == '\r')
				line.pop_back();

			blockLineCounter++;

			switch(blockLineCounter){
			case BLOCK_1_LINE_PREVIOUS_HASH:
				isCompleteBlock = false;
				newBlock->setpre_block(line);
				break;
			case BLOCK_2_LINE_BLOCK_HASH:
				newBlock->settxns_hash(line);
				break;
			case BLOCK_3_LINE_BITS:
				newBlock->setbits(std::stoi(line));
				break;
			case BLOCK_4_LINE_NONCE:
				newBlock->setnonce(std::stoi(line));
				break;
			case BLOCK_5_LINE_CANT_TX:
				//newBlock->settxn_count(std::stoi(line));
				cantTx = getPositiveNumberFromString(line);
				break;
			case BLOCK_TRANSAC_TX_INPUT_INDEX:
				inTx = getPositiveNumberFromString(line);
				for(unsigned int i = 0; i < inTx ;i++){
					newTrans->addTransactionInput();
				}
				if(inTx)
					break;
				blockLineCounter++;
			case BLOCK_TRANSAC_INPUTS:
				if(inTx){
					trasnferCounter ++;
					newTrans->getTransactionInput(trasnferCounter)->setTxId(this->getSubString(line,' ', 1));
					newTrans->getTransactionInput(trasnferCounter)->setIdx(std::stoi(this->getSubString(line,' ', 2)));
					newTrans->getTransactionInput(trasnferCounter)->setAddr(this->getSubString(line,' ', 3));
				}
				if (trasnferCounter != inTx) blockLineCounter--;
				else trasnferCounter = 0;
				break;
			case BLOCK_TRANSAC_TX_OUTPUT_INDEX:
				outTx = getPositiveNumberFromString(line);
				for(unsigned int i = 0; i < outTx ;i++){
					newTrans->addTransactionOutput();
				}
				if (outTx) break;
			case BLOCK_TRANSAC_OUTPUTS:
				if(outTx){
					trasnferCounter ++;
					newTrans->getTransactionOutput(trasnferCounter)->setValue(std::stof(this->getSubString(line,' ', 1)));
					newTrans->getTransactionOutput(trasnferCounter)->setAddr(this->getSubString(line,' ', 2));
				}
				if (trasnferCounter != outTx) blockLineCounter--;
				else{
					trasnferCounter = 0;
					if (cantTx ==1){
						blockLineCounter = 0;
						isCompleteBlock = true;
						newBlock->settransaction(newTrans);
						this->userBlockChain.insertar(newBlock);
					}else{
						cantTx--;
						newBlock->settransaction(newTrans);
					}

				}
				break;
			}

			// Evitar problema de ultima linea con \n y getline en archivos
			if (!this->getIsStandarfromList(FileTypes::loadBlockChainFile)){
				static unsigned int lastCharpos;
				static bool hasEndOfLine = this->hasNewLineAtTheEnd(iss,&lastCharpos);
				if( hasEndOfLine){
				//	cout<<lastCharpos<<endl;
				//	cout<<iss->tellg()<<endl;
					if(abs(lastCharpos - iss->tellg() ) == 1){
						if (isCompleteBlock)
							return STATUS_OK;
						else
							return STATUS_BAD_READ_INPUT_FILE;
					}
				}
			}

			// Si se leyo un bloque completo y es el fin de archivo
			if (isEof){
				if (isCompleteBlock){
					finishLoading = true;
					break;
				}
				else
					return STATUS_BAD_READ_INPUT_FILE;
			}else{
				if (isCompleteBlock){
					finishLoading = false;
					break;
				}
			}


		}

	}
	return STATUS_OK;
}


std::string BlockChainFileManager::getSubString(std::string line,size_t delim,unsigned int substringNum,bool *pState){
	//Encuentro el primer delimitador de comando
	const unsigned int MAX_ALLOWED_SUBSTRING = 50;
	if (substringNum > MAX_ALLOWED_SUBSTRING){
		if( pState ) *pState = false;
		return "";
	}
	size_t delimPos = line.find(delim);
	if ( delimPos == std::string::npos){
		if( pState ) *pState = false;
		return "";
	}
	if (substringNum == 1){
		if( pState ) *pState = true;
		string aux = line.substr(0, delimPos);
		return aux;
	}
	else{
		for(unsigned int posCounter = 2; posCounter != MAX_ALLOWED_SUBSTRING ; posCounter++){
			size_t delimPosNext = line.find(delim,delimPos+1);

			if( posCounter == substringNum)
			{
				if ( delimPosNext == std::string::npos){
					string aux = line.substr(delimPos+1);
					if( pState ) *pState = true;
					return aux;
				}else{
					string aux = line.substr(delimPos+1,delimPosNext - (delimPos+1));
					if( pState ) *pState = true;
					return aux;
				}
			}else{
				if ( delimPosNext == std::string::npos){
					if( pState ) *pState = false;
					return "";
				}
				delimPos = delimPosNext;
			}
		}
	}
	if( pState ) *pState = false;
	return "";
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isHashFromString(std::string line){
if ( line.size() != (size_t) LargoHash::LargoHashEstandar ) 	return false;
	for (unsigned int i = 0; i < line.length(); ++i) {
		if ( ! isxdigit ( line[i] ) ) 							return false;
	}
	return true;
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isPositiveIntNumberFromString(std::string line){
	int IndexValue;
	std::string::size_type sz;
	std::stringstream ss;
	ss.str(line);
	if ((ss >> IndexValue).fail())	  	return false;
	if (IndexValue < 0) 			  	return false;
	IndexValue = std::stoi(line,&sz);
	if( sz != line.size() )				return false;
	return true;
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isPositiveFloatNumberFromString(std::string line){
	float btcValue;
	std::string::size_type sz;
	std::stringstream ss;
	ss.str(line);
	if ((ss >> btcValue).fail())	  	return false;
	if (btcValue < 0) 			  		return false;
	btcValue = std::stof(line,&sz);
	if( sz != line.size() )				return false;
	return true;
}
// Descripcion:
// Precondicion:
// PostCondicion:
unsigned int BlockChainFileManager::getPositiveNumberFromString(std::string line){
	unsigned int IndexValue;
	std::stringstream ss;
	ss.str(line);
	ss >> IndexValue;
	return IndexValue;
}


//---------------------------------------------------------------------------------//
// OBSOLETO PARA LA VERSION 2.1.1 (TP1)

// Descripcion:
// Precondicion:
// PostCondicion:
status_t BlockChainFileManager::validate(std::istream * iss){
	if( ! this->isEmpty(iss)){
			int inTxTotal,outTxTotal;
		if( this->isTxIndexFromStream(iss,'\n',&inTxTotal) == false ) 	return STATUS_CORRUPT_FORMAT_BAD_TXIN;
		for(int inTx = 0 ; inTx < inTxTotal ; inTx++){
			if( this->isHashFromStream(iss,' ') == false ) 				return STATUS_CORRUPT_FORMAT_BAD_HASH;
			if( this->isTxIndexFromStream(iss,' ') == false ) 			return STATUS_CORRUPT_FORMAT_BAD_TXINDEX;
			if( this->isHashFromStream(iss) == false ) 					return STATUS_CORRUPT_FORMAT_BAD_HASH;
		}
		if( this->isTxIndexFromStream(iss,'\n',&outTxTotal) == false ) 	return STATUS_CORRUPT_FORMAT_BAD_TXOUT;
		for(int outTx = 0 ; outTx < outTxTotal ; outTx++){
			if( this->isBTCValueFromStream(iss,' ') == false ) 			return STATUS_CORRUPT_FORMAT_BAD_BTCVALUE;
			if( this->isHashFromStream(iss) == false ) 				    return STATUS_CORRUPT_FORMAT_BAD_HASH;
		}
		if( this->isEofFromStream(iss) == false ) 						return STATUS_CORRUPT_FORMAT;
	}
	return STATUS_OK;
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isEmpty(std::istream  * iss)
{
	// PRECONDICION: ESTA FUNCION SOLO DEBE USARSE ANTES DE HACER
	// EL TRABAJO DEL ARCHIVO PUESTO QUE AL TERMINAR DEJA APUNTANDO
	// AL PRINCIPIO

	bool empty;
	//Voy al final de File
	iss->seekg(0, ios::end);
	empty = (iss->tellg() == 0)? true: false;
	//Vuelvo al principio del File
	iss->clear();
	iss->seekg(0, iss->beg);
	return empty;

}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isTxIndexFromStream(std::istream *iss,char delim , int * pValue)
{
	int IndexValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);

	bool state = this->isPositiveIntNumberFromString(line);
	//Debug
	//std::cout << line << std::endl;
	ss.str(line);
	ss >> IndexValue;
	if(pValue != NULL) *pValue = IndexValue;
	return state;
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isHashFromStream(std::istream *iss,char delim, std::string * pString)
{
	std::string line;
	std::stringstream ss;
	std::getline(*iss, line,delim);
	if( line.back() == '\r'){  				//Para portabilidad Linux - Window
		line.substr(0, line.size()-1);
	}
	bool state = this->isHashFromString(line);
	//Debug
	//std::cout << line << std::endl;
	if(pString != NULL) *pString = line;
	return state;
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isBTCValueFromStream(std::istream *iss,char delim,float * pFloat)
{
	float floatValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	if ((ss >> floatValue).fail())	  return false;
	if (floatValue < 0) 			  return false;
	//Debug
	//std::cout << line << std::endl;
	if(pFloat != NULL) *pFloat = floatValue;
	return true;
}

// Descripcion:
// Precondicion:
// PostCondicion:
bool BlockChainFileManager::isEofFromStream(std::istream *iss){
	std::string line;
	if (iss->eof()) return true;
	try{
		if (std::getline(*iss, line,'\r').fail() ) return true;
	}catch(const std::ios_base::failure& ex) {
		//std::cerr << "Caught: std::ios_base::failure" << std::endl;
		iss->clear();
		return true;
	}
	if (line.size() != 0 )
		return false;
	return true;
}

// Descripcion:
// Precondicion:
// PostCondicion:
status_t BlockChainFileManager::parse(std::istream * iss, raw_t * &pBuilderRawData){

	//Creo el archivo raw_t en el entorno del filemanager
	this->pRawData = new raw_t{0};
	if ( ! this->isEmpty(iss)){
		if(pRawData == NULL) return STATUS_BAD_ALLOC;
		pRawData->inTx = this->getTxIndexFromStream(iss,'\n');

		pRawData->IN_tableOfTxId = new std::string[pRawData->inTx];
		pRawData->IN_tableOfIndex = new int[pRawData->inTx];
		pRawData->IN_tableOfAddr = new std::string[pRawData->inTx];
		if(		pRawData->IN_tableOfTxId == NULL  ||
				pRawData->IN_tableOfIndex == NULL ||
				pRawData->IN_tableOfAddr == NULL ) 	return STATUS_BAD_ALLOC;

		for(int i = 0; i < pRawData->inTx; i++)
		{
			pRawData->IN_tableOfTxId[i]  = this->getHashFromStream(iss,' ');
			pRawData->IN_tableOfIndex[i] = this->getTxIndexFromStream(iss,' ');
			pRawData->IN_tableOfAddr[i]  = this->getHashFromStream(iss);
		}

		pRawData->outTx = this->getTxIndexFromStream(iss,'\n');
		pRawData->OUT_tableOfValues = new float[pRawData->outTx];
		pRawData->OUT_tableOfAddr = new std::string[pRawData->outTx];
		if(		pRawData->OUT_tableOfValues == NULL  ||
				pRawData->OUT_tableOfAddr   == NULL  ) 	return STATUS_BAD_ALLOC;

		for(int i = 0; i < pRawData->outTx; i++)
		{
			pRawData->OUT_tableOfValues[i]  = this->getBTCValueFromStream(iss,' ');
			pRawData->OUT_tableOfAddr[i] = this->getHashFromStream(iss);
		}
	}
	pBuilderRawData = this->pRawData;

	return STATUS_OK;
}

// Descripcion:
// Precondicion:
// PostCondicion:
int BlockChainFileManager::getTxIndexFromStream(std::istream *iss,char delim)
{
	int IndexValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	ss >> IndexValue;
	return IndexValue;
}

// Descripcion:
// Precondicion:
// PostCondicion:
std::string BlockChainFileManager::getHashFromStream(std::istream *iss,char delim)
{
	std::string line;
	std::stringstream ss;
	std::getline(*iss, line,delim);
	return line;
}

// Descripcion:
// Precondicion:
// PostCondicion:
float BlockChainFileManager::getBTCValueFromStream(std::istream *iss,char delim)
{
	float floatValue;
	std::string line;
	std::stringstream ss;

	std::getline(*iss, line,delim);
	ss.str(line);
	ss >> floatValue;
	return floatValue;
}
