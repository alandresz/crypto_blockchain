#include "Queue.h"
#include <iostream>
using namespace std;

template<class T>
Queue<T>::Queue(): frontPtr(NULL), backPtr(NULL), count(0)
{
}

template<class T>
Queue<T>::~Queue()
{
	if (frontPtr != NULL)
	{
		while(! isEmpty()){
			dequeue();
		}
	}
}

template<class T>
void Queue<T>::copyQueue(Queue<T> &other){
 if(other.isEmpty()){
	 frontPtr = NULL;
	 backPtr = NULL;
	 count = 0;
 }else{
	Node * aux = other.frontPtr;
	do{
		Node *newOne = new Node;
		newOne->data = aux->data;
		newOne->next = NULL;
		if(this->isEmpty()){
			frontPtr = newOne;
		}
		else{
			backPtr->next = newOne;
		}
		backPtr = newOne;
		count++;
		aux = aux->next;
	}while(aux != NULL);
 }
}



template<class T>
bool Queue<T>::isEmpty(){
    return(count == 0);
}

template<class T>
bool Queue<T>::isEmpty()const{
    return(count == 0);
}


template<class T>
void Queue<T>::enqueue(T data){
    Node *newOne = new Node;
    newOne->data = data;
    newOne->next = NULL;
    if(isEmpty()){
        frontPtr = newOne;
    }
	else{
		backPtr->next = newOne;
	}
	backPtr = newOne;
	count++;
}

template<class T>
T Queue<T>::dequeue(){
    if(isEmpty()){
    	T emptyData{};
        cout << "Nothing inside" << endl;
        return emptyData;
    }
	else{
		T data;
		Node *temp = frontPtr;
		if(frontPtr == backPtr){
			frontPtr = NULL;
			backPtr = NULL;
		}
		else{
			frontPtr = frontPtr->next;
		}
		data = temp->data;
		delete temp;
		count--;
		return data;
	}
}

#include "Transaction.h"

//template class Queue<int>;
template class Queue<size_t>;
//template class Queue<char>;
//template class Queue<float>;
template class Queue<string>;
template class Queue<Transaction>;


// QUEUE_H
