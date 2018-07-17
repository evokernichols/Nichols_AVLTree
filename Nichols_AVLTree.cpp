/*
* Ryan Nichols
* CS-3100-01
* Project 3
* 3/29/16
*/

#include "stdafx.h" 
#include "AVLTree.h"
#include <iostream>
#include <random>

using namespace std;

int main()
{
	AVLTree* tree = new AVLTree;

	
	tree->insert(30, 1);
	tree->insert(20, 3);
	tree->insert(10, 3);	
	tree->insert(10, 3);
	tree->insert(20, 2);
	tree->insert(30, 1);
	tree->insert(31, 1);
	tree->insert(32, 1);
	tree->insert(33, 1);
	tree->insert(9, 1);
	tree->insert(8, 1);	
	tree->insert(70, 3);
	tree->insert(60, 3);
	tree->insert(80, 3);
	tree->insert(90, 3);
	tree->insert(65, 3);
	tree->insert(66, 3);	
	tree->insert(10, 3);
	tree->insert(20, 3);
	tree->insert(15, 3);

	tree->print();


	
	int val = 5;
	bool found = tree->find(0, val);
	cout << "Found: " << found << " Value: " << val << endl;

	cout << "Size = " << tree->getSize() << endl;
	cout << "Height = " << tree->getHeight() << endl << endl;

	vector<int> pairs = tree->findRange(25, 40);
	for (int i = 0; i < pairs.size(); i++) {
		cout << pairs[i] << ", ";
	}
	cout << endl;

	return 0;
}
