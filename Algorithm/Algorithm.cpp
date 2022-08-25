#include <iostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
using namespace std;
#include "BinarySearchTree.h"

//    [20]
//[10]	   [30]
//      [25]  [40]
//				[50]
int main()
{
	BinarySearchTree bst;

	bst.Insert(10);
	bst.Insert(20);
	bst.Insert(30);
	bst.Insert(40);
	bst.Insert(50);
	//bst.Insert(25);
	//bst.Insert(26);

	//bst.Delete(20);
	//bst.Delete(26);

	bst.Print();
}