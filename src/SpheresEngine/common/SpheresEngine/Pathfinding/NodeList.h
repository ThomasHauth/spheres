#pragma once

#include <list>
#include <vector>

class Node;

/**
 * Typdef when referring to a node with a non-owning pointer
 */
typedef Node * NodePtr;

/**
 * List of non-owning pointers to nodes
 */
typedef std::list<NodePtr> NodeList;

/**
 * Vector of Node objects
 */
typedef std::vector<Node> NodeVector;

/**
 * Vector of non-owning pointers to Nodes
 */
typedef std::vector<NodePtr> NodePtrVector;
