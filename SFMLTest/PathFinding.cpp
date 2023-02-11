#include "pch.h"
#include "PathFinding.h"


ai::Node::Node()
{
	this->m_index = 0;
}

//Node
ai::Node::Node(unsigned int index)
{
	this->m_index = index;
}


ai::Node::Node(const Node& node)
{
	this->m_index = node.m_index;
	this->m_connections = node.m_connections;
}

void ai::Node::addEdge(Node* to, unsigned int cost)
{
	if (to == nullptr)
		return;

	this->m_connections.push_back(Edge(to, cost));
}

ai::Node::EdgeList& ai::Node::getConnections()
{
	return this->m_connections;
}

const unsigned int ai::Node::getIndex() const
{
	return this->m_index;
}

bool ai::Node::operator==(Node& node)
{
	return this->m_index == node.m_index;
}


//Edge
ai::Edge::Edge(Node* n, unsigned int c)
{
	this->to = n;
	this->cost = c;
}

ai::Edge::Edge(const Edge& edge)
{
	this->to = edge.to;
	this->cost = edge.cost;
}

ai::Node* ai::Edge::getTo()
{
	return this->to;
}

unsigned int ai::Edge::getCost() const
{
	return this->cost;
}

ai::Edge::~Edge()
{}


//Graph
ai::Graph::Graph(std::vector<std::vector<TD::Cell*>>& map)
{
	unsigned int row = unsigned int(map.size());
	unsigned int column = unsigned int(map[0].size());

	for (unsigned int y = 0; y < row; y++)
		for (unsigned int x = 0; x < column; x++)
		{
			unsigned int idx = (y * column) + x;

			Node* node = this->getNode(idx);
			if (node == nullptr)
				node = &this->addNode(idx);

			if (y > 0)
			{
				unsigned int cost = (unsigned int)(map[y - __int64(1)][x]->get_terrain());
				unsigned int toIdx = ((y - 1) * column) + x;
				Node* to = this->getNode(toIdx);
				if (to == nullptr)
					to = &this->addNode(toIdx);
				node->addEdge(to, cost);

				if (cost == 1)
					this->start = to;
				else if (cost == 0)
					this->goal = to;
			}

			if (y < map.size() - 1)
			{
				unsigned int cost = (unsigned int)(map[y + __int64(1)][x]->get_terrain());
				unsigned int toIdx = ((y + 1) * column) + x;
				Node* to = this->getNode(toIdx);
				if (to == nullptr)
					to = &this->addNode(toIdx);
				node->addEdge(to, cost);

				if (cost == 1)
					this->start = to;
				else if (cost == 0)
					this->goal = to;
			}

			if (x > 0)
			{
				unsigned int cost = (unsigned int)(map[y][x - __int64(1)]->get_terrain());
				unsigned int toIdx = (y * column) + (x - 1);
				Node* to = this->getNode(toIdx);
				if (to == nullptr)
					to = &this->addNode(toIdx);
				node->addEdge(to, cost);

				if (cost == 1)
					this->start = to;
				else if (cost == 0)
					this->goal = to;
			}

			if (x < map[y].size() - 1)
			{
				unsigned int cost = (unsigned int)(map[y][x + __int64(1)]->get_terrain());
				unsigned int toIdx = (y * column) + (x + 1);
				Node* to = this->getNode(toIdx);
				if (to == nullptr)
					to = &this->addNode(toIdx);
				node->addEdge(to, cost);

				if (cost == 1)
					this->start = to;
				else if (cost == 0)
					this->goal = to;
			}
		}


}

ai::Node& ai::Graph::addNode(unsigned int idx)
{
	this->nodes.push_back(Node(idx));
	return this->nodes.back();
}

ai::Node* ai::Graph::getNode(unsigned int idx)
{
	for (std::list<ai::Node>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it)
		if (it->getIndex() == idx)
			return &*it;
	return nullptr;
}


//struct sort
inline bool ai::SortByCostSoFar::operator()(const PathRecord& a, const PathRecord& b)
{
	return a.costSoFar < b.costSoFar;
}


//pathRecord
ai::PathRecord::PathRecord()
{
	this->node = nullptr;
	this->costSoFar = 0;
	this->prev = nullptr;
}


ai::PathRecord::PathRecord(const PathRecord& pathRecord)
{
	this->node = pathRecord.node;
	this->costSoFar = pathRecord.costSoFar;
	this->connection = pathRecord.connection;
	this->prev = pathRecord.prev;
}


ai::PathRecord::PathRecord(ai::Node* node, Edge connection, PathRecord* prev)
{
	this->node = node;
	this->connection = connection;
	this->costSoFar = 0;
	this->prev = new PathRecord;
	*this->prev = *prev;
}

ai::PathRecord::~PathRecord()
{
}


//OpenNodeList
void ai::OpenNodeList::operator+=(PathRecord& node)
{
	this->m_recordsQueue.push_back(node);
	this->SortByCostSoFar();
}

void ai::OpenNodeList::operator-=(PathRecord& node)
{
	for (std::vector<PathRecord>::iterator it = this->m_recordsQueue.begin(); it != this->m_recordsQueue.end(); it++)
		if (*it == node) 
		{
			this->m_recordsQueue.erase(it);
			return;
		}
}

const bool ai::OpenNodeList::isEmpty() const
{
	return this->m_recordsQueue.empty();
}

ai::PathRecord& ai::OpenNodeList::popSmallestCostSoFarNode()
{
	PathRecord* pathSoFar = nullptr;
	for (unsigned int i = 0; i < this->m_recordsQueue.size(); i++)
		if (pathSoFar == nullptr)
			pathSoFar = &this->m_recordsQueue[i];
		else if (this->m_recordsQueue[i].costSoFar < pathSoFar->costSoFar)
			pathSoFar = &this->m_recordsQueue[i];

	if (pathSoFar != nullptr)
		this->operator-=(*pathSoFar);

	return *pathSoFar;
}

ai::PathRecord& ai::OpenNodeList::getSmallestCostSoFarNode()
{
	PathRecord* pathSoFar = nullptr;
	for (unsigned int i = 0; i < this->m_recordsQueue.size(); i++)
		if (pathSoFar == nullptr)
			pathSoFar = &this->m_recordsQueue[i];
		else if (this->m_recordsQueue[i].costSoFar < pathSoFar->costSoFar)
			pathSoFar = &this->m_recordsQueue[i];

	return *pathSoFar;
}

ai::PathRecord* ai::OpenNodeList::find(const ai::Node& node)
{
	for (unsigned int i = 0; i < this->m_recordsQueue.size(); i++)
		if (this->m_recordsQueue[i].node == &node)
			return &this->m_recordsQueue[i];
	return nullptr;
}

void ai::OpenNodeList::SortByCostSoFar()
{
	std::sort(this->m_recordsQueue.begin(), this->m_recordsQueue.end(), ai::SortByCostSoFar());
}


//CloseNodeList
void ai::CloseNodeList::operator+=(const PathRecord& node)
{
	this->m_recordsQueue.push_back(node);
}

const bool ai::CloseNodeList::contains(const Node* node) const
{
	for (unsigned int i = 0; i < this->m_recordsQueue.size(); i++)
		if (this->m_recordsQueue[i].node == node)
			return true;
	return false;
}

const ai::PathRecord* ai::CloseNodeList::find(const Node& node) const
{
	for (unsigned int i = 0; i < this->m_recordsQueue.size(); i++)
		if (this->m_recordsQueue[i].node == &node)
			return &this->m_recordsQueue[i];
	return nullptr;
}

//Dijkstra
bool ai::Dijkstra::pathFinder(Graph& graph)
{
	OpenNodeList openList;
	CloseNodeList closeList;

	PathRecord recStart;
	recStart.node = graph.getStart();

	PathRecord recGoal;
	recGoal.node = graph.getGoal();

	openList += recStart;

	PathRecord current = recStart;
	current.costSoFar = 0;
	while (!openList.isEmpty())
	{
		current = openList.getSmallestCostSoFarNode();
		if (current.node == recGoal.node)
			break;

		ai::Node::EdgeList& connection = current.node->getConnections();
		for (unsigned int i = 0; i < connection.size(); i++)
		{
			ai::Edge* con = &connection[i];
			if (closeList.find(*con->getTo()) != nullptr)
				continue;

			unsigned int endCost = current.costSoFar + con->getCost();

			PathRecord* connectionRecord = openList.find(*con->getTo());
			if (connectionRecord != nullptr)
			{
				if (connectionRecord->costSoFar <= endCost)
					continue;

				connectionRecord->connection = Edge(current.node, endCost);
				connectionRecord->costSoFar = endCost;
				openList.SortByCostSoFar();
			}
			else
			{
				PathRecord* rec = new PathRecord(con->getTo(), Edge(current.node, endCost), &current);
				rec->costSoFar = endCost;
				openList += *rec;
			}
		}
		
		openList -= current;
		closeList += current;
	}

	if (current.node != recGoal.node)
		return false;

	while (current.node != recStart.node)
	{
		this->path.push_back(current.connection);
		current = *current.prev;
	}
	std::reverse(this->path.begin(), this->path.end());
	
	//Add goal node to vector
	recGoal.connection = Edge(recGoal.node, 0);
	this->path.push_back(recGoal.connection);

	return true;
}

sf::Vector2i ai::Dijkstra::getMapIdx(unsigned int pathIndx, unsigned int row)
{
	return sf::Vector2i(pathIndx / row, pathIndx % row);
}