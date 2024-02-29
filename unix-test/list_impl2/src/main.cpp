//============================================================================
// Name        : List.cpp
// Author      : root
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "list.h"
using namespace std;

int main()
{
	List B;
	List *A=new List(); /*  */

	cout<<"hello\n";
	A->insert(3);
	A->insert(5);
	A->insert(13);
	if (A->isempty())
	{
		A->print();
	}	
	if(!A->searchNo(5))
	{
		cout<<"found\n";
	}

	cout<<A->get()<<endl;

	//A->clear();
	A->print();
	delete A;

	return 0;
}
