#ifndef MYLIST_H_
#define MYLIST_H_

#include <iostream>
using namespace std;

template<class T>
class myList
{
public:

	myList();
	~myList();
	int isempty(); /* */
	void print();
	int length();
	//int searchData(int);
	int searchNo(T);
	T get();		/*  */
	int insert(T); /*  */
	//void sort();
	//void deleteData(T);
	//void deleteNo(T);
	//void clear();

private:
	typedef struct __node
	{
		T data;
		struct __node *next;
	}Node;
	Node *head, *tail;
	int size;
};



template<class T>
myList<T>::myList()
{
	this->head = NULL;
	this->tail = NULL;
	size = 0;
}

template<class T>
myList<T>::~myList()
{
	Node *temp = NULL;
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

template<class T>
int myList<T>::isempty()
{
	if(this->head == NULL)
	{
		return 0;
	}
	else return 1;
}

template<class T>
int myList<T>::length()
{
	return this->size;
}

template<class T>
int myList<T>::insert(T n)
{
	Node *newNode = NULL;
	newNode	= (Node *)malloc(sizeof(Node));
	if(newNode == NULL) return -1;
	this->size++;
	newNode->data = n;
	newNode->next = this->head;
	this->head = newNode;
	return 0;
}

template<class T>
void myList<T>::print()
{
	Node *tmp = head;
	while(tmp!=NULL)
	{
		cout<<tmp->data<<"\t";
		tmp = tmp->next;
	}
	cout<<endl;
}

template<class T>
int myList<T>::searchNo(T n)
{
	Node *tmp = head;
	while(tmp!=NULL)
	{
		if (tmp->data == n)
		{
			return 0;
		}
		tmp = tmp->next;
	}
	return -1;
}

template<class T>
T myList<T>::get()
{
	Node *tmp = head;
	if(tmp!=NULL)
	{
		head = head->next;
		return tmp->data;
	}else
	{
		cout<<"List is empty"<<endl;
	}
	return -1;
}


#endif /* MYLIST_H_ */

