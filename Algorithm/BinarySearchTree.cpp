#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

void SetCursorColor(ConsoleColor color)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}


BinarySearchTree::BinarySearchTree()
{
	_nil = new Node(); // Black
	_root = _nil;
}
BinarySearchTree::~BinarySearchTree()
{
	delete _nil;
}
void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	SetCursorPosition(x, y);

	if (node->color == Color::Black)
		SetCursorColor(ConsoleColor::BLUE);
	else
		SetCursorColor(ConsoleColor::RED);

	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);

	SetCursorColor(ConsoleColor::WHITE);
}


Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == _nil || key == node->key)
		return node;
	
	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
}

Node* BinarySearchTree::Min(Node* node)
{
	while (node->left != _nil)
		node = node->left;

	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	while (node->right != _nil)
		node = node->right;
	return node;
}

Node* BinarySearchTree::Next(Node* node)
{
	if (node->right != _nil)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent  != _nil && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}
	return parent;
}

void BinarySearchTree::Insert(int key)
{
	Node* newNode = new Node();
	newNode->key = key;

	Node* node = _root;
	Node* parent = _nil;

	while (node != _nil)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;

	if (parent == _nil)
		_root = newNode;
	else if (key < parent->key)
		parent->left = newNode;	
	else
		parent->right = newNode;

	// 검사
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red;

	InsertFixup(newNode);
}

void BinarySearchTree::InsertFixup(Node* node)
{
	// 1) p = red, uncle = red
	// -> p = black, uncle = black, pp = red로 바꿈
	// 2) p = red, uncle = black(triangle)
	// -> 회전을 통해 case 3으로 바꿈
	// 3) p = red, uncle = black(list)
	// -> 색상 변경 + 회전

	//      [pp(R)]
	//   [p(B)]   [u(? -> R)] // parent의 parent의 자식 = uncle.
	//      [n(R)]
	while (node->parent->color == Color::Red)
	{
		if (node->parent == node->parent->parent->left)
		{

			Node* uncle = node->parent->parent->right;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp
				node = node->parent->parent;
			}
			else
			{
				// Triangle
				//      [pp(B)]
				//   [p(R)]   [u(B)]
				//      [n(R)]
				
				//       [pp(B)] // left rotate
				//    [p(R)]   [u(B)]
				//  [n(R)]

				if (node == node->parent->right) // Triangle 타입에서 list 타입으로 바꿔줌.
				{
					node = node->parent;
					LeftRotate(node);
				}

				// List 타입
				//      [pp(R)]
				//   [p(B)]   [u(B)]
				// [n(R)]

				//      [p(B)]
				// [n(R)]    [pp(R)]
				//                 [u(B)]

				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				RightRotate(node->parent->parent);

			}
		}
		else
		{
				//      [pp(B)]
				//   [u(R)]   [p(R)] // uncle node가 red인 상황.
				//				  [n(R)]

				//      [pp(R)]
				//   [u(B)]   [p(B)]
				//				  [n(R)]
			Node* uncle = node->parent->parent->left;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp
				node = node->parent->parent;
			}
			else // uncle node가 black인 상황
			{
				// Triangle
				//      [pp(B)]
				//   [u(B)]   [p(R)]
				//		   [n(R)]

				//       [pp(B)]
				//    [u(B)]   [p(R)]		n -> p
				//				    [n(R)]  p -> n

				if (node == node->parent->left) // Triangle 타입에서 list 타입으로 바꿔줌.
				{
					node = node->parent;
					RightRotate(node);
				}

				// List 타입
				//       [pp(B)]
				//    [u(B)]   [p(R)]
				//				    [n(R)]

				//       [pp(R)]
				//    [u(B)]   [p(B)]
				//				    [n(R)]

				//		     [p(B)]
				//     [pp(R)]    [n(R)]
				// [u(B)]   
				//				    

				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				LeftRotate(node->parent->parent);

			}
		}
	}
	_root->color = Color::Black;
}

void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void BinarySearchTree::Delete(Node* node)
{
	if (node == nullptr)
		return;

	// child가 하나만 있거나, 아예 없거나 둘다 사용.
	if (node->left == nullptr) 
		Replace(node, node->right);
	else if (node->right == nullptr)
		Replace(node, node->left);
	else
	{
		// 다음 데이터 찾기
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// u 서브트리를 v 서브트리로 교체
// 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
	if (u->parent == nullptr)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v; 

	if (v)
		v->parent = u->parent;

	delete u;
}
/*RightRotate*/
//     [y]
//  [x]   [3]
// [1][2]

//     [x]
//  [1]   [y]
//       [2][3]

void BinarySearchTree::RightRotate(Node* y)
{
	Node* x = y->left;

	y->left = x->right; // [2];
	if (y->right != _nil)
		y->right->parent = y;

	x->parent = y->parent;

	if (y->parent == _nil)
		_root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	x->right = y;
	y->parent = x;
}

/*LeftRotate*/
//     [x]
//  [1]   [y]
//       [2][3]

//       [p] <- else if 검사
//     [y]
//  [x]   [3]
// [1][2]

void BinarySearchTree::LeftRotate(Node* x)
{
	Node* y = x->right;

	x->right = y->left; // [2];
	if(y->left != _nil)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == _nil)
		_root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

