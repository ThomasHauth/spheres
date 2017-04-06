#include <SpheresEngine/Pathfinding/Node.h>
#include <SpheresEngine/Pathfinding/Pathfinding.h>
#include <algorithm>

void AStar::resetNodes(NodeVector& nodes) {
	for (auto & n : nodes) {
		n.reset();
	}
}

void AStar::reset() {
	m_openList.clear();
	m_closedList.clear();
}

std::deque<Node*> AStar::pathToNode(Node * startNode, Node * targetNode) {

	assert(startNode != nullptr);
	assert(targetNode != nullptr);

	m_openList.clear();
	m_openList.push_back(startNode);

	int maxIterations = 10000;
	int iter = 0;

	while (m_openList.size() != 0) {
		// expand the best node on the open list

		// find smallest F
		float smallest = std::numeric_limits<float>::max();
		Node * smallestN = nullptr;
		for (Node * n : m_openList) {
			if (n->F < smallest) {
				smallest = n->F;
				smallestN = n;
			}
		}

		assert(smallestN);

		m_openList.remove(smallestN);
		m_closedList.push_back(smallestN);

		if (smallestN == targetNode) { // found
			std::deque<Node*> path;

			backtrack(targetNode, path);
			return path;
		}
		expandNode(smallestN, targetNode);
		iter++;
		if (iter > maxIterations) {
			logging::Fatal() << "Pathfinding from " << startNode->Location
					<< " to " << targetNode->Location
					<< " stopped after max iterations reached";
		}
	}

	// no path found
	return std::deque<Node*>();
}

void AStar::backtrack(Node * child, std::deque<Node*> & path) {
	path.push_front(child);
	if (child->Previous != nullptr) {
		backtrack(child->Previous, path);
	}
}

void AStar::expandNode(Node * parent, Node * targetNode) {
	for (Node* child : parent->Neighbours) {

		if (std::find(m_closedList.begin(), m_closedList.end(), child)
				!= m_closedList.end()) {
			continue;
		}

		const float thisG = 1.0f + parent->G;
		child->G = thisG;

		// add heuristic, explicit weighting for different use cases may be needed here
		const float thisF = thisG + targetNode->distanceTo(child);

		child->F = thisF;
		child->Previous = parent;

		// add to the open list?
		if (std::find(m_openList.begin(), m_openList.end(), child)
				== m_openList.end()) {
			m_openList.push_back(child);
		}
	}
}
