#pragma once

#include <deque>

#include <boost/noncopyable.hpp>
#include <SpheresEngine/Log.h>
#include <SpheresEngine/Pathfinding/NodeList.h>

/**
 * Astar wayfinding algorithm within a node graph
 *
 * Possible speed improvements:
 *  - grid-based fast lookup for nodes according to their position in space
 *  - fixed number of possible Neigbours ( array / std::vec ) of 6
 */
class AStar {
public:

	/** reset the internal state of this astar algorithm.
	 * must be done before using it
	 */
	void reset();

	/**
	 * Reset the status of one node, must be done before using it
	 */
	void resetNodes(NodeVector & nodes);

	/**
	 * Search for the best path from startNode to targetNode. Will
	 * return a list of nodes which must be visited in order to arrive
	 * at targetNode
	 */
	std::deque<Node*> pathToNode(Node * startNode, Node * targetNode);

private:

	/**
	 * Find the way back along the shortest path and compile the step
	 * into the "path" deque
	 */
	void backtrack(Node * child, std::deque<Node*> & path);

	/**
	 *  Evaluate all neighbors and compute their rating in the
	 *  current search run
	 */
	void expandNode(Node * parent, Node * targetNode);

	/**
	 * Contains a list of all nodes which have not been visited yet
	 */
	NodeList m_openList;

	/**
	 * Nodes which have already been visited during this search run
	 */
	NodeList m_closedList;
};

/**
 * Use the Astar as default pathfinding class
 */
using Pathfinding = AStar;
