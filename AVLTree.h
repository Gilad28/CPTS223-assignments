#include <iostream>
#include <limits>
#include <vector>
using namespace std;

template <typename Comparable>
class AVLTree
{
public:
    AVLTree();
    ~AVLTree();

    void makeEmpty();
    const Comparable & findMin() const;
    const Comparable & findMax() const;

    bool contains(const Comparable & x) const;

    void insert(const Comparable &x);
    
    void remove(const Comparable &x);
    
    int treeSize() const;
    
    int computeHeight() const;
    
    int readRootHeight() const;
    bool isBalanced() const;
    
    bool isBST() const;

    double averageDepth() const;

    void removeByRank(int rank);

    static const int ALLOWED_IMBALANCE = 1;
private:
    struct AVLNode
    {
        Comparable element;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode( const Comparable & theElement, AVLNode *lt, AVLNode *rt ): element(theElement), left(lt), right(rt) {}
        AVLNode( Comparable && theElement, AVLNode *lt, AVLNode *rt ): element(move(theElement)), left(lt), right(rt) {}
    };

    AVLNode *root;

    AVLNode * findMin( AVLNode * t ) const;
    AVLNode * findMax( AVLNode * t ) const;
    void makeEmpty( AVLNode * & t );
    int height(AVLNode *t) const;
    bool contains(const Comparable &x, AVLNode *t) const;
    void insert(const Comparable &x, AVLNode *&t);
    void calculateDepthNodes(AVLNode * t, int currentDepth, int & totalDepth, int & totalNodes) const;
    void insert(Comparable &&x, AVLNode *&t);
    void remove(const Comparable &x, AVLNode *&t);
    bool isBalanced(AVLNode *t) const;
    bool isBST(AVLNode *t, const Comparable *minValue, const Comparable *maxValue) const;
    int treeSize(AVLNode *t) const;
    void removeByRank(AVLNode *&t, int rank, int &currentRank);
    void balance(AVLNode * & t);
    void rotateWithLeftChild( AVLNode * & t );
    void rotateWithRightChild( AVLNode * & t );
    void doubleWithLeftChild( AVLNode * & t);
    void doubleWithRightChild( AVLNode * & t);
};

template <class Comparable>
AVLTree<Comparable>::AVLTree() : root(NULL) {}

template <class Comparable>
AVLTree<Comparable>::~AVLTree()
{
    makeEmpty();
}

template <typename Comparable>
void AVLTree<Comparable>::makeEmpty() {
    makeEmpty(root);
}

template <typename Comparable>
void AVLTree<Comparable>::makeEmpty(AVLNode * & t) {
    if ( t != NULL ) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = NULL;
    }
}

template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMin() const {
    if (root == NULL) {
        throw underflow_error("Tree is empty");
    }
    return findMin(root)->element;
}

template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMin(AVLNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->left == NULL) {
        return t;
    } else {
        return findMin(t->left);
    }
}

template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMax() const {
    if (root == NULL) {
        throw underflow_error("Tree is empty");
    }
    return findMax(root)->element;
}

template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMax(AVLNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->right == NULL) {
        return t;
    } else {
        return findMax(t->right);
    }
}

template<typename Comparable>
bool AVLTree<Comparable>::contains( const Comparable & x ) const {
    return contains( x, root );
}

template<typename Comparable>
bool AVLTree<Comparable>::contains( const Comparable & x, AVLNode *t ) const
{
    if( t == nullptr )
        return false;
    else if( x < t->element )
        return contains( x, t->left );
    else if( t->element < x )
        return contains( x, t->right );
    else
        return true;
}

template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x) {
    insert( x, root );
    balance(root);
}

template<typename Comparable>
void AVLTree<Comparable>::insert( const Comparable & x, AVLNode *&t)
{
    if( t == nullptr )
        t = new AVLNode{ x, nullptr, nullptr };
    else if( x < t->element )
        insert( x, t->left );
    else if( t->element < x )
        insert( x, t->right );
    balance(t);
}

template<typename Comparable>
void AVLTree<Comparable>::insert( Comparable && x, AVLNode *&t)
{
    if( t == nullptr )
        t = new AVLNode{ std::move( x ), nullptr, nullptr };
    else if( x < t->element )
        insert( std::move( x ), t->left );
    else if( t->element < x )
        insert( std::move( x ), t->right );
    balance(t);
}

template<typename Comparable>
void AVLTree<Comparable>::remove( const Comparable & x ) {
    remove( x, root );
    balance(root);
}

template<typename Comparable>
void AVLTree<Comparable>::remove( const Comparable & x, AVLNode *&t)
{
    if( t == nullptr )
        return;
    if( x < t->element )
        remove( x, t->left );
    else if( t->element < x )
        remove( x, t->right );
    else if( t->left != nullptr && t->right != nullptr )
    {
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
    }
    else
    {
        AVLNode *oldNode = t;
        t = ( t->left != nullptr ) ? t->left : t->right;
        delete oldNode;
    }
    balance(t);
}

template<typename Comparable>
void AVLTree<Comparable>::balance(AVLNode *&t)
{
    if (t == nullptr)
        return;

    if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
    {
        if (height(t->left->left) >= height(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
    }

    else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
    {
        if (height(t->right->right) >= height(t->right->left))
            rotateWithRightChild(t);
        else
            doubleWithRightChild(t);
    }

    t->height = max(height(t->left), height(t->right)) + 1;
}

template<typename Comparable>
void AVLTree<Comparable>::rotateWithLeftChild(AVLNode * & k2)
{
    AVLNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    
    k2 = k1;
}

template<typename Comparable>
void AVLTree<Comparable>::rotateWithRightChild(AVLNode *&k2)
{
    AVLNode *k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->right), k2->height) + 1;

    k2 = k1;
}

template<typename Comparable>
void AVLTree<Comparable>::doubleWithLeftChild(AVLNode * & k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template<typename Comparable>
void AVLTree<Comparable>::doubleWithRightChild(AVLNode *&k3)
{
    rotateWithLeftChild(k3->right);
    rotateWithRightChild(k3);
}

template <class Comparable>
bool AVLTree<Comparable>::isBalanced() const {
    return isBalanced(root);
}

template <typename Comparable>
bool AVLTree<Comparable>::isBalanced(AVLNode *t) const
{
    if (t == nullptr)
        return true;

    int leftHeight = height(t->left);
    int rightHeight = height(t->right);

    if (abs(leftHeight - rightHeight) <= ALLOWED_IMBALANCE &&
        isBalanced(t->left) && isBalanced(t->right))
    {
        return true;
    }
    return false;
}

template <class Comparable>
bool AVLTree<Comparable>::isBST() const {
    return isBST(root, nullptr, nullptr);
}

template <typename Comparable>
bool AVLTree<Comparable>::isBST(AVLNode *t, const Comparable *minValue, const Comparable *maxValue) const
{
    if (t == nullptr)
        return true;

    if ((minValue != nullptr && t->element <= *minValue) || 
        (maxValue != nullptr && t->element >= *maxValue))
    {
        return false;
    }

    return isBST(t->left, minValue, &t->element) && 
           isBST(t->right, &t->element, maxValue);
}

template <typename Comparable>
int AVLTree<Comparable>::treeSize() const {
    return treeSize(root);
}

template <typename Comparable>
int AVLTree<Comparable>::treeSize(AVLNode *t) const {
    if (t == nullptr)
        return 0;
    return 1 + treeSize(t->left) + treeSize(t->right);
}

template <typename Comparable>
int AVLTree<Comparable>::computeHeight() const {
    return height(root);
}

template <typename Comparable>
int AVLTree<Comparable>::height(AVLNode *t) const {
    if (t == nullptr)
        return -1;
    else
        return 1 + max(height(t->left), height(t->right));
}

template <typename Comparable>
int AVLTree<Comparable>::readRootHeight() const {
    return (root == nullptr) ? -1 : root->height;
}

template <typename Comparable>
double AVLTree<Comparable>::averageDepth() const {
    int totalDepth = 0;
    int totalNodes = 0;
    calculateDepthNodes(root, 0, totalDepth, totalNodes);
    return (totalNodes == 0) ? 0.0 : static_cast<double>(totalDepth) / totalNodes;
}

template <typename Comparable>
void AVLTree<Comparable>::calculateDepthNodes(AVLNode *t, int currentDepth, int &totalDepth, int &totalNodes) const {
    if (t == nullptr)
        return;
    
    totalDepth += currentDepth;
    totalNodes++;

    calculateDepthNodes(t->left, currentDepth + 1, totalDepth, totalNodes);
    calculateDepthNodes(t->right, currentDepth + 1, totalDepth, totalNodes);
}

template <typename Comparable>
void AVLTree<Comparable>::removeByRank(int rank) {
    int currentRank = 0;
    removeByRank(root, rank, currentRank);
}

template <typename Comparable>
void AVLTree<Comparable>::removeByRank(AVLNode *&t, int rank, int &currentRank) {
    if (t == nullptr)
        return;

    removeByRank(t->left, rank, currentRank);

    if (currentRank == rank) {
        remove(t->element);
        return;
    }
    currentRank++;

    removeByRank(t->right, rank, currentRank);
}
