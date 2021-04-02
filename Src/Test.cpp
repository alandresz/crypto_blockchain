#include<string>
#include <iostream>

#include "Cuentas.h"
#include "BlockChainBuilder.h"

#include "TiposHash.h"

using namespace std;

int main() {
	Cuentas Usuarios;
	if ( Usuarios.openlista( "Cuentas.txt" ) ) {
		cout << "Cuentas: " << Usuarios.getcantidad() << endl;
	}
	else return -1;

	if ( Usuarios.addcuenta( "5e2149d75798816533665d9f856f670a30da813da690b0bf3def42f7953c84e4", "Cuenta Leandro" , 100 ) ) {
		cout << "Cuentas: " << Usuarios.getcantidad() << endl;
	}

	if ( Usuarios.addcuenta( "679621a42ec38b8ee381a25bf34fb9899359a5b26bd054e10b2ad96354db6eeb", "Cuenta Rama" , 10 ) ) {
		cout << "Cuentas: " << Usuarios.getcantidad() << endl;
	}

	if ( Usuarios.addcuenta( "6b48422401687cb409d9caf69a0bd769b47ca6352020bc66f13d2ec8dee2bc70", "Cuenta sin alias" , 50 ) ) {
		cout << "Cuentas: " << Usuarios.getcantidad() << endl;
	}

	if ( Usuarios.savelista( "Cuentas2.txt" ) ) {
		cout << "grabado Ok: " << Usuarios.getcantidad() << endl;
	}
	else return -1;

}
