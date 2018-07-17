/*
* Ryan Nichols
* CS-3100-01
* Project 3
* 3/29/16
*/

#include "stdafx.h"       
#include "AVLTree.h"
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

/*Helper function declarations*/
AVLTree::TreeNode* insertHelper(AVLTree::TreeNode*, AVLTree::TreeNode*, string&, AVLTree::TreeNode*);
void printHelper(AVLTree::TreeNode*, int);
int getHeightHelper(AVLTree::TreeNode*);
bool findHelper(int, int&, AVLTree::TreeNode*);
void findRangeHelper(int, int, vector<int>&, AVLTree::TreeNode*);
AVLTree::TreeNode* findDeepestUnbalanced(AVLTree::TreeNode*, string, int, int&);
int checkNodeBalance(AVLTree::TreeNode*);
bool checkForBend(string, int);


/** Default Constructor
* This constructor creates a new AVLTree with no nodes
* Worst case efficiency: Theta(1)
* Precondition: None
* PostCondition: A new, empty AVLTree has been created
*/
AVLTree::AVLTree()
{
	size = 0;
	root = NULL;
}

/** Destructor
*
* Worst case efficiency: Theta(1)
* Precondition: None
* PostCondition: The AVLTree object has been deleted
*/
AVLTree::~AVLTree()
{
}

/** Insert Function
* This function inserts a new node with the provided key and value int the tree
* and balances the tree if necessary, duplicate keys are not allowed and will not
* no be added. True is returned if successful, false is returned otherwise.
* Worst case efficiency: Theta(log(n))
* Precondition: Two ints have been provided
* PostCondition: A new TreeNode has been created with the provided values,
*				 has been inserted into the tree, and true has been returned;
*				 or false has been returned
*/
bool AVLTree::insert(int key, int value) {
	int placeHolder;
	if (find(key, placeHolder)) {
		return false;   //If the key is already in the tree, return false
	}

	TreeNode* newNode = new TreeNode();
	newNode->key = key;
	newNode->value = value;
	string insertionPath = "";

	if (root == NULL) {
		root = newNode; //Set as root if the root is empty
		newNode->parent = NULL;
	}
	else if (key >= root->key) {
		insertionPath += "r";
		root->rightChild = insertHelper(newNode, root->rightChild, insertionPath, root); //Insert into right subtree		
	}
	else if (key < root->key) {
		insertionPath += "l";
		root->leftChild = insertHelper(newNode, root->leftChild, insertionPath, root); //Insert into left subtree
	}
	size++;
	//cout << "INSERTING " << key << ", " << value << " ";	//DEBUG	
	balance(insertionPath, root);
	//cout << endl;											//DEBUG

	return true;
}

/** Insert Helper Function
* This function inserts the provided node into the subtree of the provided position
* and balances the tree if necessary. The position pointer is returned.
* Worst case efficiency: Theta(log(n))
* Precondition: Two TreeNode pointers have been provided
* PostCondition: The TreeNode pointed to by newNode has been inserted into the
*                subtree of insertPoint. The subtree root has been returned.
*/
AVLTree::TreeNode* insertHelper(AVLTree::TreeNode* newNode, AVLTree::TreeNode* insertPoint, string& path, AVLTree::TreeNode* parent) {
	if (insertPoint == NULL) {
		newNode->parent = parent;
		return newNode; //If the subtree root is null, return newNode as the subtree root
	}
	else if (newNode->key >= insertPoint->key) {
		path += "r";
		insertPoint->rightChild = insertHelper(newNode, insertPoint->rightChild, path, insertPoint);
	}
	else if (newNode->key < insertPoint->key) {
		path += "l";
		insertPoint->leftChild = insertHelper(newNode, insertPoint->leftChild, path, insertPoint);
	}
	return insertPoint;
}

/** Get Height Function
* This function returns the height of the tree.
* Worst case efficiency: Theta(n)
* Precondition: None
* PostCondition: The height of the tree has been returned
*/
int AVLTree::getHeight() {
	if (root == NULL) {
		return 0;      //Return 0 if root is null
	}
	int leftDepth = getHeightHelper(root->leftChild);
	int rightDepth = getHeightHelper(root->rightChild);
	if (leftDepth >= rightDepth) {
		return leftDepth + 1;
	}
	else { //rightDepth > leftDepth
		return rightDepth + 1;
	}
}

/** Get Height Helper Function
* This function returns the height of the given subtree
* Worst case efficiency: Theta(n)
* Precondition: A TreeNode pointer has been provided
* PostCondition: The height of the subtree has been returned
*/
int getHeightHelper(AVLTree::TreeNode* subTreeRoot) {
	if (subTreeRoot == NULL) {
		return 0;      //Return 0 if subTreeRoot is null
	}
	int leftDepth = getHeightHelper(subTreeRoot->leftChild);
	int rightDepth = getHeightHelper(subTreeRoot->rightChild);
	if (leftDepth >= rightDepth) {
		return leftDepth + 1;
	}
	else { //rightDepth > leftDepth
		return rightDepth + 1;
	}
}

/** Get Size Function
* This function returns the number of entries in the tree.
* Worst case efficiency: Theta(1)
* Precondition: None
* PostCondition: The height of the tree has been returned
*/
int AVLTree::getSize() {
	return size;
}

/** Print Function
* The function prints visual representation of the tree, rotated 90 degress
* counter clockwise.
* Worst case efficiency: Theta(n)
* Precondition: None
* PostCondition: The contents of the tree has been printed
*/
void AVLTree::print() {
	if (root != NULL)
		printHelper(root, 0);
}

/** Print Helper Function
* The function is used by the print function to recursively traverse and
* print the tree.
* Worst case efficiency: Theta(n)
* Precondition: A pointer to a TreeNode and an integer has been provided
* PostCondition: The contents of the tree has been printed
*/
void printHelper(AVLTree::TreeNode* node, int indent) {
	if (node->rightChild != NULL)
		printHelper(node->rightChild, indent + 7);

	cout << setw(indent) << ' ' << node->key << ", " << node->value;
	/*if (node->parent != NULL) {																//
		cout << ", " << node->parent;															//DEBUG
	}	*/																						//
	cout << endl;

	if (node->leftChild != NULL)
		printHelper(node->leftChild, indent + 7);
}

/** Find Function
* This function finds a TreeNode of the provided key, if present, assigns its value
* to the value argument, and returns true; or returns false, if it is not present,
* and leaves the value argument unchanged.
* Worst case efficiency: Theta(log(n))
* Precondition: An int and an int reference has been provided
* PostCondition: If a TreeNode of the provided key is found, its value is stored
*				 into the value address and true is returned; otherwise false is
*				 returned and the contents of the value address are left unchanged
*/
bool AVLTree::find(int key, int& value) {
	bool found = false;
	if (root == NULL) {
		return false;
	}
	if (root->key == key) {
		value = root->value;
		return true;
	}

	if (key >= root->key) {
		found = findHelper(key, value, root->rightChild);
	}
	else { //key < root->key
		found = findHelper(key, value, root->leftChild);
	}
	return found;
}

/** Find Helper Function
* This function finds a TreeNode of the provided key in the subtree, if present, assigns its value
* to the value argument, and returns true; or returns false, if it is not present,
* and leaves the value argument unchanged.
* Worst case efficiency: Theta(log(n))
* Precondition: An int, an int reference, and a TreeNode* has been provided
* PostCondition: If a TreeNode of the provided key is found, its value is stored
*				 into the value address and true is returned; otherwise false is
*				 returned and the contents of the value address are left unchanged
*/
bool findHelper(int key, int& value, AVLTree::TreeNode* subTreeRoot) {
	bool found = false;
	if (subTreeRoot == NULL) {
		return false;
	}
	if (subTreeRoot->key == key) {
		value = subTreeRoot->value;
		return true;
	}

	if (key >= subTreeRoot->key) {
		found = findHelper(key, value, subTreeRoot->rightChild);
	}
	else { //key < subTreeRoot->key
		found = findHelper(key, value, subTreeRoot->leftChild);
	}
	return found;
}

/** Find Range Function
* This function returns a vector of ints containing all key and value pairs in the tree between
* the provided lowkey and highkey values. An empty vector is returned if no values
* between this range are found.
* Worst case efficiency: Theta(n)
* Precondition: Two ints have been provided
* PostCondition: A vector containing all key value pairs in the tree between the provided
*				 lowkey and highkey (inclusive) has been returned
*/
vector<int> AVLTree::findRange(int lowkey, int highkey) {
	vector<int> pairs;
	if (root == NULL) {
		return pairs;   //Return empty vector
	}
	if (root->key <= highkey && root->key >= lowkey) {
		pairs.push_back(root->key);
		pairs.push_back(root->value); //Add root key and value to vector if in range (inclusive)

		findRangeHelper(lowkey, highkey, pairs, root->leftChild);   //Check left subtree
		findRangeHelper(lowkey, highkey, pairs, root->rightChild);  //Check right subtree
	}
	else if (root->key > highkey) { //Everything to right must also be greater than highkey
		findRangeHelper(lowkey, highkey, pairs, root->leftChild);   //Check left subtree
	}
	else if (root->key < lowkey) { //Everything to left must also be less than lowkey
		findRangeHelper(lowkey, highkey, pairs, root->rightChild);  //Check right subtree
	}
	return pairs;
}

/** Find Range Helper Function
* This function adds to the provided vector of ints all key and value pairs in
* the subtree between the provided lowkey and highkey values. The vector is left
* unchanged if no keys in the subtree are in range.
* Worst case efficiency: Theta(n)
* Precondition: Two ints, a vector<int> reference, and a TreeNode pointer have
*               been provided
* PostCondition: All key and value pairs in range in the subtree have been added
*                to the provided vector.
*/
void findRangeHelper(int lowkey, int highkey, vector<int>& pairs, AVLTree::TreeNode* subTreeRoot) {
	if (subTreeRoot == NULL) {
		return;   //Return unchanged vector
	}
	if (subTreeRoot->key <= highkey && subTreeRoot->key >= lowkey) {
		pairs.push_back(subTreeRoot->key);
		pairs.push_back(subTreeRoot->value); //Add subTreeRoot key and value to vector if in range (inclusive)

		findRangeHelper(lowkey, highkey, pairs, subTreeRoot->leftChild);   //Check left subtree
		findRangeHelper(lowkey, highkey, pairs, subTreeRoot->rightChild);  //Check right subtree
	}
	else if (subTreeRoot->key > highkey) { //Everything to right must also be greater than highkey
		findRangeHelper(lowkey, highkey, pairs, subTreeRoot->leftChild);   //Check left subtree
	}
	else if (subTreeRoot->key < lowkey) { //Everything to left must also be less than lowkey
		findRangeHelper(lowkey, highkey, pairs, subTreeRoot->rightChild);  //Check right subtree
	}
	return;
}

/** Balance Function
* This function balances the tree after an insert.
* Worst case efficiency: Theta(log(n))
* Precondition: An insertion path string and TreeNode* (containing root) has been provided
* PostCondition: The tree has been balanced if need
*/
void AVLTree::balance(string insertionPath, AVLTree::TreeNode* rootNode) {
	if (insertionPath.length() == 0) {
		return;	//e.g. root node inserted
	}
	int deepestPos = 0;
	TreeNode* deepest = findDeepestUnbalanced(rootNode, insertionPath, 0, deepestPos); //Get the deepest unbalanced node

	if (deepest != NULL) {

		bool isBent = checkForBend(insertionPath, deepestPos);
		//cout << deepestPos << endl << insertionPath << endl; //Debug

		if (isBent == true) {	//Bent Path (double rotate)			
			if (insertionPath.at(deepestPos) == 'l') {
				//cout << "Rotating " << deepestPos + 1 << " to right" << endl;
				singleRightRotate(deepest->leftChild);
				//cout << "Rotating " << deepestPos << " to left" << endl;
				singleLeftRotate(deepest);
			}
			else {
				//cout << "Rotating " << deepestPos + 1 << " to left" << endl;
				singleLeftRotate(deepest->rightChild);
				//cout << "Rotating " << deepestPos << " to right" << endl;
				singleRightRotate(deepest);
			}			
		} //END OF DOUBLES
		
		else { //Straight Path (single rotate)
			if (insertionPath.at(deepestPos) == 'l') {
				//cout << "Rotating " << deepestPos << " to left" << endl;
				singleLeftRotate(deepest);
			}
			else {
				//cout << "Rotating " << deepestPos << " to right" << endl;
				singleRightRotate(deepest);
			}
		} //END OF SINGLES
	}
}

/** Find Deepest Unblanced (Node) Function
* This function finds the deepest unbalanced node in the tree.
* Worst case efficiency: Theta(log(n))
* Precondition: A TreeNode*, string, int, and int reference has been provided
* PostCondition: A pointer to the deepest unbalanced node in the tree has been 
*				 returned and the deepestPos argument has been set to the depth 
*/
AVLTree::TreeNode* findDeepestUnbalanced(AVLTree::TreeNode* node, string insertionPath, int current, int& deepestPos) {
	AVLTree::TreeNode* deepest = NULL;
	AVLTree::TreeNode* next;
	int balance = checkNodeBalance(node);
	if (balance > 1 || balance < -1) {
		deepest = node;			//If this node is unbalanced, set deepest to it
		deepestPos = current;
	}

	if (current < insertionPath.length()) {
		if (insertionPath.at(current) == 'l') {	//If next node is to left
			next = findDeepestUnbalanced(node->leftChild, insertionPath, current + 1, deepestPos);
			if (next != NULL) {
				deepest = next;
			}
		}
		if (insertionPath.at(current) == 'r') {	//If next node is to right
			next = findDeepestUnbalanced(node->rightChild, insertionPath, current + 1, deepestPos);
			if (next != NULL) {
				deepest = next;
			}
		}
	}
	return deepest;
}

/** Check Node Balance Function
* This function check the balance of a node.
* Worst case efficiency: Theta(log(n))
* Precondition: A TreeNode* has been provided
* PostCondition: The balance of the node has been returned 
*				 Negative for right heavy
*				 Positive for left heavy
*				 Zero for balanced
*/
int checkNodeBalance(AVLTree::TreeNode* node) {
	int leftDepth = 0;
	int rightDepth = 0;
	int balance;

	if (node->leftChild != NULL) {
		leftDepth = getHeightHelper(node->leftChild);
	}
	if (node->rightChild != NULL) {
		rightDepth = getHeightHelper(node->rightChild);
	}
	balance = leftDepth - rightDepth;
	return balance;
}

/** Check For Bend Function
* This function determines if there is a bend between the deepest unbalanced node and the
* newly added node.
* Worst case efficiency: Theta(1)
* Precondition: A string and an int has been provided
* PostCondition: True has been returned if there is a bend
				 False has been returned otherwise
*/
bool checkForBend(string insertionPath, int deepestUnbal) {
	if (insertionPath.length() == 0) {
		return false;
	}
	char curr = insertionPath.at(deepestUnbal);
	for (int i = deepestUnbal+ 1; i < insertionPath.length(); i++) {
		if (insertionPath.at(i) != curr) {
			return true;
		}
	}
	return false;
}


/** Single Left Rotate
* This function rotates the provided node (down and to the) left
* Worst case efficiency: Theta(1)
* Precondition: A TreeNode* has been provided
* PostCondition: The provided node has been rotated to the left
*/
void AVLTree::singleLeftRotate(TreeNode* unbal) {
	if (unbal == NULL) {
		return;
	}
	if (unbal->parent != NULL) {
		if (unbal->parent->leftChild == unbal) {
			unbal->parent->leftChild = unbal->leftChild;
		}
		else {
			unbal->parent->rightChild = unbal->leftChild;
		}
		unbal->leftChild->parent = unbal->parent;
	}

	else {		
		root = unbal->leftChild;
		root->parent = NULL;
	}
	
	unbal->parent = unbal->leftChild;

	if (unbal->leftChild->rightChild != NULL) {
		TreeNode* temp = unbal->leftChild->rightChild;
		unbal->leftChild->rightChild = unbal;
		unbal->leftChild = temp;
		temp->parent = unbal;
	}
	else {
		unbal->leftChild->rightChild = unbal;
		unbal->leftChild = NULL;
	}	
}

/** Single Right Rotate
* This function rotates the provided node (down and to the) right
* Worst case efficiency: Theta(1)
* Precondition: A TreeNode* has been provided
* PostCondition: The provided node has been rotated to the right
*/
void AVLTree::singleRightRotate(TreeNode* unbal) {
	if (unbal == NULL) {
		return;
	}
	if (unbal->parent != NULL) {
		if (unbal->parent->rightChild == unbal) {
			unbal->parent->rightChild = unbal->rightChild;
		}
		else {
			unbal->parent->leftChild = unbal->rightChild;
		}
		unbal->rightChild->parent = unbal->parent;
	}
	else {
		root = unbal->rightChild;
		root->parent = NULL;
	}

	unbal->parent = unbal->rightChild;

	if (unbal->rightChild->leftChild != NULL) {
		TreeNode* temp = unbal->rightChild->leftChild;
		unbal->rightChild->leftChild = unbal;
		unbal->rightChild = temp;
		temp->parent = unbal;
	}
	else {
		unbal->rightChild->leftChild = unbal;
		unbal->rightChild = NULL;
	}	
}