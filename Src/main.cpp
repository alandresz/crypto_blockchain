
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "cmdline.h"
#include "BlockChainManager.h"


using namespace std;


/*=====================================================================================*/
// 									PROTOTIPOS
/*=====================================================================================*/


static void opt_input(string const &);
static void opt_output(string const &);



/*=====================================================================================*/
// 								ELEMENTOS GLOBALES
/*=====================================================================================*/

static option_t options[] = {
	{1, "i", "input", "-", opt_input, OPT_DEFAULT},
	{1, "o", "output", "-", opt_output, OPT_DEFAULT},
	{0, },
};


/*====================================================================================*/
//									MAIN
/*====================================================================================*/


int main(int argc, char * const argv[]){

	std::cout<<"AlgoChain v2.1.2 - Grupo 5 : Galvan - Vera - Dreszman"<<std::endl;

	//------Valido Argumentos ------//
	cmdline cmdl(options);
	cmdl.parse(argc, argv);

	//------Le paso los archivos al Manager ------//

	BlockChainManager::proccesBlockChain();

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
	if (arg == "-")  BlockChainManager::setUserFilename(ios::in);
	else 			 BlockChainManager::setUserFilename(ios::in,arg.c_str(),false);
}

static void opt_output(string const &arg)
{

	// Si el nombre del archivos es "-", usaremos la salida
	// est?dar. De lo contrario, abrimos un archivo en modo
	// de escritura.
	//
	//outputFileName = arg.c_str();
	if (arg == "-")  BlockChainManager::setUserFilename(ios::out);
	else 			 BlockChainManager::setUserFilename(ios::out,arg.c_str(),false);
}

