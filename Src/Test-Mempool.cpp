
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Mempool.h"


using namespace std;



static fstream ifs;
static fstream ofs;
static string outputFileName;


/*====================================================================================*/
//									MAIN
/*====================================================================================*/

int main(void){


	std::cout<<"AlgoChain v1.1.1 - Grupo 5 : Galvan - Vera - Dreszman"<<std::endl;
	std::cout<<"MEMPOOL TEST"<<std::endl;


	// ----- INICIALIZO MEMPOOL ----- //
	Mempool mem_pool;
	std::cout<<"Mempool creada" <<std::endl;


	// ---- CREO TRANSACCIONES ----- //
	Transaction transaction_1;
	std::cout<<"transaccion 1 creada" <<std::endl;

	Transaction transaction_2;
	std::cout<<"transaccion 2 creada" <<std::endl;


	Transaction transaction_3;
	std::cout<<"transaccion 3 creada" <<std::endl;

	Transaction transaction_4;
	std::cout<<"transaccion 4 creada" <<std::endl;

	Transaction transaction_5;
	std::cout<<"transaccion 5 creada" <<std::endl;



	MempoolUnit memp_unit_1 = MempoolUnit( &transaction_1) ;
	MempoolUnit memp_unit_2 = MempoolUnit( &transaction_2) ;
	MempoolUnit memp_unit_3 = MempoolUnit( &transaction_3) ;
	MempoolUnit memp_unit_4 = MempoolUnit( &transaction_4) ;
	MempoolUnit memp_unit_5 = MempoolUnit( &transaction_5) ;



	// ---- CARGO TRANSACCION 1----- //
	mem_pool.set_new_pool_unit( &memp_unit_1 );
	std::cout<<"transaccion 1 cargada a la mempool" <<std::endl;

	// ---- CARGO TRANSACCION 2----- //
	mem_pool.set_new_pool_unit( &memp_unit_2 );
	std::cout<<"transaccion 2 cargada a la mempool" <<std::endl;

	// ---- CARGO TRANSACCION 3----- //
	mem_pool.set_new_pool_unit( &memp_unit_3 );
	std::cout<<"transaccion 3 cargada a la mempool" <<std::endl;

	// ---- CARGO TRANSACCION 4----- //
	mem_pool.set_new_pool_unit( &memp_unit_4  );
	std::cout<<"transaccion 4 cargada a la mempool" <<std::endl;

	// ---- CARGO TRANSACCION 5----- //
	mem_pool.set_new_pool_unit( &memp_unit_5  );
	std::cout<<"transaccion 5 cargada a la mempool" <<std::endl;

	// ----- Cantidad de elementos en la mempool ----- //
	std::cout<< "Cantidad de transacciones: " << mem_pool.get_number_txns() << std::endl;

	// ----- Consulto transacciones ------ //
	int N = 3;

	std::cout<< "Declaro un int N" << std::endl;

	Transaction a = mem_pool.get_transaction_n(N);



	std::cout<< "Tengo en a guardada la transaccion numero "<< N << std::endl;

	int b = a.getNumTransactionIn() ;
	int c = a.getNumTransactionOut() ;
	std::cout<< "Num txn in en a: " << b << std::endl;
	std::cout<< "Num txn out en a: " << c << std::endl;





	std::cout<< "Se borrara la mempool" <<std::endl;
	// ----- Se elimina la Mempool ----- //
	mem_pool.~Mempool();

	std::cout<< "Mempool borrada!" <<std::endl;



	return 0;
}


