
#include "BlockChainUser.h"

BlockChainUser::BlockChainUser() {
	this->confirmedBalance = 0;
	this->unconfirmedBalance = 0;
	this->user = "";
	this->hashUser = "";
}

BlockChainUser::~BlockChainUser() {
	this->confirmedBalance = 0;
	this->unconfirmedBalance = 0;
	this->user = "";
	this->hashUser = "";
}

void BlockChainUser::addUser(std::string hashUser){
	this->user = "No Name";
	this->hashUser = hashUser;
}

void BlockChainUser::addUser(std::string hashUser,std::string User){
	this->user = User;
	this->hashUser = hashUser;
}

void BlockChainUser::setConfirmBalance(float confirmedBalance){
	this->confirmedBalance = confirmedBalance;
	this->unconfirmedBalance = confirmedBalance;
}

void BlockChainUser::setUnConfirmBalance(float unconfirmedBalance){
	this->unconfirmedBalance += unconfirmedBalance;
}

void BlockChainUser::transfer(float unconfirmedBalance){
	this->unconfirmedBalance -= unconfirmedBalance;
}

void BlockChainUser::confirmBalance(){
	this->confirmedBalance = this->unconfirmedBalance;
}

std::string BlockChainUser::getHashUser(void){
	return this->hashUser;
}

std::string BlockChainUser::getUsername(void){
	return this->user;
}

float BlockChainUser::getConfirmedBalance(void){
	return this->confirmedBalance;
}
float BlockChainUser::getUnConfirmedBalance(void){
	return this->unconfirmedBalance;
}

bool BlockChainUser::canTransfer(float mount){
	if( mount > this->unconfirmedBalance ) return false;
	else return true;
}
