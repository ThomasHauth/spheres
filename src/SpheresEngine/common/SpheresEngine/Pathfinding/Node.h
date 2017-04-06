#pragma once

#include <SpheresEngine/VectorTypes.h>
#include <vector>
#include <list>

#include "NodeList.h"

/**
 * One node is the search graph. Stores its neighbours and parent
 */
class Node {
public:
	/**
	 * Creates a node with a 2-dimensional position
	 */
	Node(Vector2 const& loc) :
			G(0.0f), F(0.0f), Previous(nullptr), Location(loc) {
	}

	/**
	 * Set this node to blocking, which means it will be not be considered for any path finding
	 */
	void setBlocked() {
		F = std::numeric_limits<float>::max();
	}

	/**
	 * Add neighboring node
	 */
	void addNeighbour(Node * n) {
		Neighbours.push_back(n);
	}

	/**
	 * rating of this Node, in this implementation the distance to the target node
	 */
	float G = 0.0f;

	/**
	 * overall rating of this Node, including the heuristic contribution
	 */
	float F = 0.0f;

	/**
	 * List of neighbors of this node
	 */
	NodePtrVector Neighbours;

	/**
	 * The node previously visited, useful for backtracking to the origin
	 */
	NodePtr Previous = nullptr;

	/**
	 * The location of this node in 2d space. Useful to compute the distance for the
	 */
	Vector2 Location = Vector2::Zero();

	/**
	 * Reset for new path search, neighbors will stay
	 */
	void reset() {
		Previous = nullptr;
		G = 0.0f;
		F = 0.0f;
	}

	/**
	 * Compute he distance to another node
	 */
	float distanceTo(Node const* n) const {
		return distanceTo(n->Location);
	}

	/**
	 * Compute the distance to another location in 2d space
	 */
	float distanceTo(Vector2 const& v) const {
		const Vector2 distVec = v - this->Location;
		// don't use magSquared because we may use distances smaller than 1
		return distVec.mag();
	}
};

