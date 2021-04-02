
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lista.h"
#include "cmdline.h"
#include "BlockChainStatus.h"
#include "BlockChainFileManager.h"
#include "BlockChainManager.h"
#include "Transaction.h"


using namespace std;

static istream* iss = 0;
static ostream* oss = 0;
static fstream ifs;
static fstream ofs;
static string outputFileName;

/*=====================================================================================*/
// 									PROTOTIPOS
/*=====================================================================================*/


static void opt_input(string const &);
static void opt_output(string const &);
static void opt_difficulty(string const &);
static void opt_help(string const &);


/*=====================================================================================*/
// 								ELEMENTOS GLOBALES
/*=====================================================================================*/

static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{1, "d", "difficulty", NULL, opt_difficulty, OPT_MANDATORY},
	{0, "h", "help", NULL, opt_help, OPT_DEFAULT},
	{0, },
};


/*====================================================================================*/
//									MAIN
/*====================================================================================*/


int main(int argc, char * const argv[]){


	std::cout<<"Transaction Test"<<std::endl;

	//------Valido Argumentos ------//
	cmdline cmdl(options);
	cmdl.parse(argc, argv);

	//------Le paso los archivos al Manager ------//
	BlockChainFileManager fileManager;
	raw_t * Raw;

	std::cout<< "Begin Validate ...";
	BlockChainManager::proccesStatus( fileManager.validate(iss) );

	std::cout<< "Begin Parsing ..." ;
	BlockChainManager::proccesStatus( fileManager.parse(iss,Raw));

	Transaction tr(*Raw);


	cout << tr.getNumTransactionIn()<< endl;
	for(int i=0 ; i < tr.getNumTransactionIn() ; i++){
		cout << tr.getTransactionInput(i)->getTxId() << endl;
		cout << tr.getTransactionInput(i)->getIdx() << endl;
		cout << tr.getTransactionInput(i)->getAddr() << endl;
	}
	cout << tr.getNumTransactionOut()<< endl;
	for(int i=0 ; i < tr.getNumTransactionOut() ; i++){
		cout << tr.getTransactionOutput(i)->getValue() << endl;
		cout << tr.getTransactionOutput(i)->getAddr() << endl;
	}





	return 0;
}



/*====================================================================================*/
// 						FUNCIONES INVOCADAS EN EL MAIN
/*====================================================================================*/

//------------------ Callbacks de CMDLINE ------------------------------//





static void opt_input(string const &arg)
{
	// Si el nombre del archivos es "-", usaremos la entrada
	// est?dar. De lo contrario, abrimos un archivo en modo
	// de lectura.
	//
	if (arg == "-") {
		iss = &cin;		// Establezco la entrada estandar cin como flujo de entrada
		cout<<"La direccion del archivo Origen es : Cin (Entrada Standar)" <<endl;

	}
	else {
		ifs.open(arg.c_str(), ios::in); // c_str(): Returns a pointer to an array that contains a null-terminated
										// sequence of characters (i.e., a C-string) representing
										// the current value of the string object.
		iss = &ifs;
		cout<<"La direccion del archivo Origen es :"<< arg.c_str() <<endl;

	}

	// Verificamos que el stream este OK.
	//
	if (!iss->good()) {
		cerr << "cannot open "
		     << arg
		     << "."
		     << endl;
		std::abort();
	}
}

static void opt_output(string const &arg)
{

	// Si el nombre del archivos es "-", usaremos la salida
	// est?dar. De lo contrario, abrimos un archivo en modo
	// de escritura.
	//
	outputFileName = arg.c_str();

	if (arg == "-") {
		oss = &cout;	// Establezco la salida estandar cout como flujo de salida
		cout<< "La direccion del archivo Destino es: Cout (Salida Standar)" << endl;
	} else {
		ofs.open(arg.c_str(), ios::out);
		oss = &ofs;
		cout<< "La direccion del archivo Destino es: "<< arg.c_str() <<endl;
	}

}

static void opt_difficulty(string const &arg)
{
	stringstream iss(arg);
	cout<< "La dificultad elegida es = " <<arg.c_str() <<endl;

	// Intentamos extraer el factor de la l?ea de comandos.
	// Para detectar argumentos que ?nicamente consistan de
	// n?meros enteros, vamos a verificar que EOF llegue justo
	// despu? de la lectura exitosa del escalar.
	//

	if (iss.bad()) {
		cerr << "cannot read integer factor."
			 << endl;
		std::abort();
	}
	int num;
	iss>>num;
	BlockChainManager::setUserDefinedDifficulty(num);
}

static void
opt_help(string const &arg)
{
	cout << "cmdline [-f function] [-i file] [-o file]" << endl;
	cout << "Funciones Admitidas: exp, Re, Im, log, cos, sen " << endl;
	cout << "Las expresiones matematicas no deben tener espacios en blanco " << endl;
	std::abort();
}
//
//
//static bool
//openOutputFile(){
//
//
//	if(outputFileName == "-")
//	{
//		oss = & cout;
//	}
//	else{
//		ofs.open(outputFileName.c_str(), ios::out);
//		oss = &ofs;
//
//	}
//	// Verificamos que el stream este OK.
//	//
//	if (!oss->good()) {
//		cerr << "cannot open "
//		<< outputFileName
//		<< "."
//		<< endl;
//
//		ifs.close();
//		return false;
//
//	}
//
//	return true;
//
//}
