#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
using namespace std;
#include "BinarySearchTree.h"
#include <thread>

//    [20]
//[10]	   [30]
//      [25]  [40]
//				[50]
int main()
{
	BinarySearchTree bst;

	bst.Insert(30);
	bst.Print();
	this_thread::sleep_for(1s);
	
	bst.Insert(10);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(20);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(25);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Delete(20);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Delete(10);
	bst.Print();
	this_thread::sleep_for(1s);
}