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
void ShowConsoleCursor(bool flag)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	::GetConsoleCursorInfo(output, &cursorInfo);
	cursorInfo.bVisible = flag;
	::SetConsoleCursorInfo(output, &cursorInfo);
}

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
void BinarySearchTree::Print()
{
	::system("cls");
	ShowConsoleCursor(false);
	Print(_root, 10, 0); 
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

// 먼저 BST 삭제 실행
//         [20]
// [10(B)]      [30]
//   [15(R)]  [25][40]
void BinarySearchTree::Delete(Node* node)
{
	if (node == _nil)
		return;

	// child가 하나만 있거나, 아예 없거나 둘다 사용.
	if (node->left == _nil)
	{
		Color color = node->color;
		Node* right = node->right;
		Replace(node, node->right);

		if (color == Color::Black)
			DeleteFixup(right);
	}
	else if (node->right == _nil)
	{
		Color color = node->color;
		Node* left = node->left;
		Replace(node, node->left);

		if (color == Color::Black)
			DeleteFixup(left);
	}
	else // 양 옆에 자식이 있는 경우
	{
		// 다음 데이터 찾기
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// notion case 1~6 참고할 것.
void BinarySearchTree::DeleteFixup(Node* node)
{
	Node* x = node;

	// [Case][Case2]
	while (x != _root && x->color == Color::Black) // Loop에 빠지는 것 자체가 Double Black.
	{
		
		if (x == x->parent->left)
		{
			// [Case3]
			
			//      [p(B)]
			// [x(DB)]   [s(R)]

			//      [p(R)]
			// [x(DB)]   [s(B)]
			//          [1]

			//			 [s(B)]
			//      [p(R)]
			// [x(DB)]   [1]

			Node* s = x->parent->right;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color = Color::Red;

				LeftRotate(x->parent);
				s = x->parent->right; // [1]
			}

			// [Case4]
			if (s->left->color == Color::Black && s->right->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				//          [p]
				// [x(DB)]       [s(B)]
				//         [near(R)][[far(B)]

				//          [p]
				// [x(DB)]       [s(R)]
				//         [near(B)][[far(B)]

				//          [p]
				// [x(DB)]    [near(B)] 
				//	              [s(R)]
				//                  [[far(B)]

				//[Case5]
				if (s->right->color == Color::Black)
				{
					s->left->color = Color::Black;
					s->color = Color::Red;
					RightRotate(s);
					s = x->parent->right;
				}
				
				//          [p]
				//   [x(DB)]   [s(B)]
				//                [[far(R)]
				// [Case6]
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->right->color = Color::Black;
				LeftRotate(x->parent);
				x = _root;
			}
		}
		else
		{
			// [Case3]
			Node* s = x->parent->left;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color = Color::Red;

				RightRotate(x->parent);
				s = x->parent->left; // [1]
			}

			// [Case4]
			if (s->right->color == Color::Black && s->left->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				//          [p]
				// [x(DB)]       [s(B)]
				//         [near(R)][[far(B)]

				//          [p]
				// [x(DB)]       [s(R)]
				//         [near(B)][[far(B)]

				//          [p]
				// [x(DB)]    [near(B)] 
				//	              [s(R)]
				//                  [[far(B)]

				//[Case5]
				if (s->left->color == Color::Black)
				{
					s->right->color = Color::Black;
					s->color = Color::Red;
					LeftRotate(s);
					s = x->parent->left;
				}

				//          [p]
				//   [x(DB)]   [s(B)]
				//                [[far(R)]
				// [Case6]
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->left->color = Color::Black;
				RightRotate(x->parent);
				x = _root;
			}

		}
	}

	x->color = Color::Black;

}

// u 서브트리를 v 서브트리로 교체
// 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
	if (u->parent == _nil)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v; 

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

