//============================================================================
// Name        : List.cpp
// Author      : root
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include "myList.h"
using namespace std;

int main()
{
	myList<char> *A = new myList<char> ; /*  */
	myList<char*> B;
	A->insert('3');
	A->insert('c');
	A->insert('#');
	if (A->isempty())
	{
		A->print();
	}	
	if(!A->searchNo('c'))
	{
		cout<<"data 'c' found\n";
	}

	cout<<A->get()<<endl;

	//A->clear();
	A->print();

	/*  ----------------- */
	cout<<"--------\n";

	B.insert("hello");
	B.insert("abc");
	B.print();

	if(!B.searchNo("abc"))
	{
		cout<<"data \"abc\"found\n";
	}
	if(!B.searchNo("bc"))
	{
		cout<<"data \"bc\" found\n";
	}

	/*  ----------------- */
	cout<<"--------\n";
	/* STL向量 */
	vector<int> a ;
	a.push_back(3);
	cout<<a[0];

	return 0;
}
