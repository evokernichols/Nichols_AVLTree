/*
* Ryan Nichols
* CS-3100-01
* Project 3
* 3/29/16
*/
#pragma once
#include <vector>
#include <cstddef>
#include <iostream>

using namespace std;
/** This header defines the functions of the AVLTree class. This is a self
* balancing binary tree, which ensures insertion and find efficiency of order
* O(log(n)). This tree's nodes will act as a map between keys and values (ints).
* This class includes functions to insert values, obtain the height of the tree
* structure, obtain the number of entries in the tree, print the tree, and find \
* nodes within the tree by key or key range.
*/
class AVLTree
{
public:
	AVLTree();
	~AVLTree();
	bool insert(int, int);
	int getHeight();
	int getSize();
	void print();
	bool find(int, int&);
	vector<int> findRange(int, int);

	class TreeNode {
	public:
		TreeNode* leftChild;
		TreeNode* rightChild;
		TreeNode* parent;
		int key;
		int value;
		/*
		TreeNode() {
		key = 0;
		value = 0;
		leftChild = NULL;
		rightChild = NULL;
		parent = NULL;
		}

		TreeNode(int k, int v) {
		key = k;
		value = v;
		leftChild = NULL;
		rightChild = NULL;
		parent = NULL;
		}
		*/
	};
	TreeNode* root;
	int size;

	void balance(string, TreeNode*);
	void singleLeftRotate(TreeNode*);
	void singleRightRotate(TreeNode*);
};

