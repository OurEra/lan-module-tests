#ifndef TEST_H_
#define TEST_H_

class List
{
public:
	
	List();
	~List();
	int isempty(); /* */
	void print();
	int length();
	//int searchData(int);
	int searchNo(int);
	int get();		/*  */
	int insert(int); /*  */
	//void sort();
	//void deleteData(T);
	//void deleteNo(T);
	//void clear();

private:
	typedef struct __node
	{
		int data;
		struct __node *next;
	}Node;
	Node *head, *tail;
	int size;
};

#endif /* TEST_H_ */

