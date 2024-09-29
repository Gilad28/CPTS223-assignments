#ifndef __BST_H
#define __BST_H

#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;


/* ----------------------------------------------------------------------------
---- Below is declaration of BST class, referring to textbook, Figure 4.16 ----
---------------------------------------------------------------------------- */

template <typename Comparable>
class BST
{
public:
	BST();
	~BST();
	void makeEmpty();

	const Comparable & findMin() const;
	const Comparable & findMax() const;

	bool contains(const Comparable & x) const;
	void insert(const Comparable & x);
	void remove(const Comparable & x);
	int treeSize() const;
	int treeHeight() const;
	void printInOrder() const;
	void printLevels() const;
	void printMaxPath() const;

private:
	struct BinaryNode
	{
		Comparable element;
		BinaryNode *left;
		BinaryNode *right;

		BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt): element(theElement), left(lt), right(rt) {}
		BinaryNode( Comparable && theElement, BinaryNode *lt, BinaryNode *rt): element(move(theElement)), left(lt), right(rt) {}
	};

	BinaryNode *root;

	BinaryNode * findMin( BinaryNode * t ) const;
	BinaryNode * findMax( BinaryNode * t ) const;
	void makeEmpty( BinaryNode * & t );
	bool contains( const Comparable & x, BinaryNode *t ) const;
	void insert( Comparable && x, BinaryNode * & t );
	void insert( const Comparable & x, BinaryNode * & t );
	void remove( const Comparable & x, BinaryNode * & t );
	int treeSize(BinaryNode* t) const;
	int treeHeight(BinaryNode* t) const;
	void printInOrder(BinaryNode *t) const;
	void printLevels(BinaryNode* t, int level) const;
	void printMaxPath(BinaryNode* t) const;
};


/* --------------------------------------------------------------
---- Below is implementation of public and private functions ----
-------------------------------------------------------------- */

// constructor
template<typename Comparable>
BST<Comparable>::BST() : root(NULL) {}

// destructor, refer to textbook, Figure 4.27
template<typename Comparable>
BST<Comparable>::~BST() {
	makeEmpty();
}

// public makeEmpty: refer to textbook, Figure 4.27
template <typename Comparable>
void BST<Comparable>::makeEmpty() {
    makeEmpty(root);
}

// private recursive makeEmpty: refer to textbook, Figure 4.27
template <typename Comparable>
void BST<Comparable>::makeEmpty(BinaryNode *& t) {
    if ( t != NULL ) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = NULL;
    }
}

// // public findMin
template <typename Comparable>
const Comparable & BST<Comparable>::findMin() const {
    if (root == NULL) {
    	throw underflow_error("Tree is empty");
	}
    return findMin(root)->element;
}

// private findMin: refer to textbook, Figure 4.20
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMin(BinaryNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->left == NULL) {
        return t;
    } else {
        return findMin(t->left);
    }
}

// public findMax
template <typename Comparable>
const Comparable & BST<Comparable>::findMax() const {
    if (root == NULL) {
    	throw underflow_error("Tree is empty");
	}
    return findMax(root)->element;
}

// private findMax: refer to textbook, Figure 4.21
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMax(BinaryNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->right == NULL) {
        return t;
    } else {
        return findMin(t->right);
    }
}


//private contains
template<typename Comparable>
bool BST<Comparable>::contains( const Comparable & x, BinaryNode *t ) const
{
	if (t == nullptr)
	{
		return false;
	}
	else if(x < t->element)
	{
		return contains(x, t->left);
	}
	else if(x > t->element)
	{
		return contains(x, t->right);
	}
	else
	{
		return true;
	}

}

// public contains: refer to textbook, Figure 4.17, Line 4 - 7
template<typename Comparable>
bool BST<Comparable>::contains( const Comparable & x ) const 
{
return contains( x, root );
	
}

//private inserts:
template<typename Comparable>
void BST<Comparable>::insert( const Comparable & x, BinaryNode *&t)
{
	if( t == nullptr )
	{
		t = new BinaryNode{ x, nullptr, nullptr };
	}
	else if( x < t->element )
	{
		insert( x, t->left );
	}
	else if( t->element < x )
	{
		insert( x, t->right );
	}
}

template<typename Comparable>
void BST<Comparable>::insert( Comparable && x, BinaryNode *&t)
{
	if( t == nullptr )
	{
		t = new BinaryNode{ std::move( x ), nullptr, nullptr };
	}
 
	else if( x < t->element )
	{
		insert( std::move( x ), t->left );
	}

 	else if( t->element < x )
	{
		insert( std::move( x ), t->right );
	}
}



// public insert: refer to textbook, Figure 4.17, Line 12 - 15
template<typename Comparable>
void BST<Comparable>::insert(const Comparable & x) 
{
	insert(x, root);
}


//private remove:
template<typename Comparable>
void BST<Comparable>::remove( const Comparable & x, BinaryNode *&t)
{
	if( t == nullptr )
	{
		return; 
	}
 
 	if( x < t->element )
 	{
 		remove( x, t->left );
	}

 	else if( t->element < x )
	{
		 remove( x, t->right );
	}

 	else if( t->left != nullptr && t->right != nullptr )
 	{
		 t->element = findMin( t->right )->element;
 		remove( t->element, t->right );
	}

 	else
 	{
 		BinaryNode *oldNode = t;
 		t = ( t->left != nullptr ) ? t->left : t->right;
 		delete oldNode;
 	}

}

// public remove: refer to textbook, Figure 4.17, Line 20 - 23
template<typename Comparable>
void BST<Comparable>::remove( const Comparable & x ) {
	remove(x, root);
}

//private treesize:
template<typename Comparable>
int BST<Comparable>::treeSize(BinaryNode* t) const {
    if (t == nullptr) 
	{
        return 0;
    } 
	else 
	{
        return 1 + treeSize(t->left) + treeSize(t->right);
    }
}

// public treeSize
template <typename Comparable>
int BST<Comparable>::treeSize() const {
	return treeSize(root);
}

//private treeHeight
template <typename Comparable>
int BST<Comparable>::treeHeight(BinaryNode* t) const{
	if(t == nullptr)
	{
		return 0;
	}
	else
	{
		return 1 + std::max(treeHeight(t->left), treeHeight(t->right));
	}
}


// public treeHeight
template <typename Comparable>
int BST<Comparable>::treeHeight() const 
{
	return treeHeight(root);
}

//private printInOrder:
template<typename Comparable>
void BST<Comparable>::printInOrder(BinaryNode *t) const
{
	if (t != nullptr)
	{
		printInOrder(t->left);
		cout << t->element << endl;
		printInOrder(t->right);
	}
}

// public printInOrder: refer to textbook, Figure 4.60
template<typename Comparable>
void BST<Comparable>::printInOrder() const {
	if(root == NULL)
	{
		cout << "Empty Tree" << endl;
	}
	else
	{
		printInOrder(root);
	}
}

//private printLevels:
template <typename Comparable>
void BST<Comparable>::printLevels(BinaryNode* t, int level) const
{
	if (t == nullptr)
	{
		return;
	}
	if (level == 0)
	{
		std::cout << t->element << endl;
	}
	else
	{
		printLevels(t->left, level - 1);
		printLevels(t->right, level -1);
	}
}

// public printLevels
template <typename Comparable>
void BST<Comparable>::printLevels() const 
{
	int h = treeHeight(root);
    for (int level = 0; level <= h; level++) 
	{
        printLevels(root, level);
        std::cout << std::endl;  
    }
}


//private printMaxPath:
template <typename Comparable>
void BST<Comparable>::printMaxPath(BinaryNode* t) const
{
	if (t == nullptr)
	{
		return;
	}

	std::cout << t->element << endl;

	if(treeHeight(t->right) > treeHeight(t->left))
	{
		printMaxPath(t->right);
	}
	else
	{
		printMaxPath(t->left);
	}

}

// public printMaxPath
template <typename Comparable>
void BST<Comparable>::printMaxPath() const {
	if(root == nullptr)
	{
		cout << "tree is empty" << endl;
	}
	else
	{
		printMaxPath(root);
	}

}

#endif
