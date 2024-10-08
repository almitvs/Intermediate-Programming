#include <cstdlib>
#include <string>
#include <iostream>
#include "CTree.h"

/*
 * Public overloaded << operator for printing a CTree object
 */
std::ostream& operator<<(std::ostream& os, CTree& rt) {
  os << rt.toString();
  return os;
}

/*
 * Public CTree constructor
 */
CTree::CTree(char ch) {
  this->data = ch;
  this->kids = NULL;
  this->sibs = NULL;
  this->prev = NULL;
}

/*
 * Public CTree destructor
 */
CTree::~CTree() {
  // Delete kids and sibs if they exist
  if (kids != NULL) {
    delete kids;
  }
  if (sibs != NULL) {
    delete sibs;
  }
}

/*
 * Public overloaded + operator for adding a CTree object
 */
CTree& CTree::operator+(CTree& rt) {
  // Call the addChild function
  this->addChild(&rt);
  return *this;
}

/*
 * Public overloaded == operator for comparing a CTree object
 */
bool CTree::operator==(const CTree& root) {
  // Call the treeCompare function
  return treeCompare(this, &root);
}

/*
 * Public function to add a character to a CTree object
 */
bool CTree::addChild(char ch) {
  CTree * node = new CTree(ch);
  // Call the addChild function
  bool success = this->addChild(node);
  if (success) {
    return true;
  } else {
    // Free the memory for the new node if it failed to add
    delete node;
    return false;
  }
}

/*
 * Public function to add a CTree object to a CTree object
 */
bool CTree::addChild(CTree* root) {
  // Return false if a node is attempting to add itself to itself
  if (this == root) {
    return false;
  }
  if (root == NULL || root->prev != NULL || root->sibs != NULL) {
    return false;
  }
  CTree * cur = this->kids;
  // Add the new node as the child if there are no children
  if (cur == NULL) {
    this->kids = root;
    root->prev = this;
    return true;
  }
  // Add the new node as the child if it comes first among the siblings and it is not already present
  if (root->data < cur->data) {
    root->sibs = cur;
    cur->prev = root;
    root->prev = this;
    this->kids = root;
    return true;
  }
  // Call the addSibling function to determine if the node belongs as a sibling of the kids
  return cur->addSibling(root);
}

/*
 * Public function to return a string representation of a CTree object
 */
std::string CTree::toString() {
  // Add the data of the current node to the final string
  std::string tree = "";
  tree += this->data;
  tree += '\n';
  // Recursively call toString if the node has a kid
  if (this->kids != NULL) {
    tree += this->kids->toString();
  }
  // Recursively call toString if the node has a sib
  if (this->sibs != NULL) {
    tree += this->sibs->toString();
  }
  return tree;
}

/*
 * Private function to add a character to a CTree object, only called by addChild
 */
bool CTree::addSibling(char ch) {
  CTree * node = new CTree(ch);
  // Call the addSibiling function
  bool success = this->addSibling(node);
  if (success) {
    return true;
  } else {
    // Free the space for the new node if it failed to add
    delete node;
    return false;
  }
}

/*
 * Private function to add a CTree object to a CTree object, only called by addChild   
 */
bool CTree::addSibling(CTree* root) {
  // Return false if the node is the very top root and thus cannot have siblings
  if (this->prev == NULL) {
    return false;
  }
  if (root == NULL || root->prev != NULL || root->sibs != NULL) {
    return false;
  }
  CTree * cur = this;
  CTree * prev = this->prev;
  // Find the node to which the new node should be added before
  while (cur != NULL && (cur->data < root->data)) {
    prev = cur;
    cur = cur->sibs;
  }
  // Add the new node as a sib if if is the tail
  if (cur == NULL) {
    prev->sibs = root;
    root->prev = prev;
  } else if (cur->data == root->data) {
      return false;
  } else if (cur->prev->kids == cur) {
    cur->prev->kids = root;
    root->prev = cur->prev;
    root->sibs = cur;
    cur->prev = root;
    return true;
  } else {
    // Add the new node in the middle of the linked list of siblings
    root->prev = cur->prev;
    cur->prev->sibs = root;
    cur->prev = root;
    root->sibs = cur;
  }
  return true;
}

/*
 * Private function to compare a CTree object, only called by the overload == operator
 */
bool CTree::treeCompare(const CTree* a, const CTree* b) const {
  if (a->data != b->data) {
    // Return false if the data do not match
    return false;
  } else if (a->kids != NULL && b->kids != NULL && a->sibs != NULL && b->sibs != NULL) {
    // If both have kids and sibs, check that those match
    return treeCompare(a->kids, b->kids) && treeCompare(a->sibs, b->sibs);
  } else if (a->kids != NULL && b->kids != NULL && a->sibs == NULL && b->sibs == NULL) {
    // If both have kids but no sibs, check that the kids match
    return treeCompare(a->kids, b->kids);
  } else if (a->kids == NULL && b->kids == NULL && a->sibs != NULL && b->sibs != NULL) {
    // If both have sibs but no kids, check that the sibs match
    return treeCompare(a->sibs, b->sibs);
  } else if (a->kids == NULL && b->kids == NULL && a->sibs == NULL && b->sibs == NULL) {
    // If both have no sibs and no kids, the leaf has been reached with no mismatches
    return true;
  } else {
    // The sizes do not match
    return false;
  }
}
