#include "list.h"
#include <iostream>
using namespace std;

List::List()
{
	this->head = NULL;
	this->tail = NULL;
	size = 0;

}
List::~List()
{
	Node *temp = NULL;
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

int List::isempty()
{
	if(this->head == NULL)
	{
		return 0;
	}
	else return 1;
}
int List::length()
{
	return this->size;
}

int List::insert(int n)
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

void List::print()
{
	Node *tmp = head;
	while(tmp!=NULL)
	{
		cout<<tmp->data<<"\t";
		tmp = tmp->next;
	}
	cout<<endl;
}

int List::searchNo(int n)
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

int List::get()
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
