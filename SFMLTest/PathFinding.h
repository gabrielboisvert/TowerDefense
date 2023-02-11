#pragma once
#include <vector>
#include <list>
#include "Cell.h"
#include "SFML/System/Vector2.hpp"

namespace ai
{
	class Edge;

	class Node
	{
	public:
		typedef std::vector<Edge> EdgeList;
	private:
		unsigned int m_index;
		EdgeList m_connections;

	public:
		Node();
		Node(unsigned int index);
		Node(const Node& node);
		void addEdge(Node* to, unsigned int cost);
		EdgeList& getConnections();
		const unsigned int getIndex() const;
		bool operator==(Node& node);
	};

	class Edge
	{
	private:
		Node* to;
		unsigned int cost;
	public:
		Edge(Node* n = nullptr, unsigned int c = 0);
		Edge(const Edge& edge);
		Node* getTo();
		unsigned int getCost() const;

		~Edge();
	};

	class Graph
	{
	private:
		std::list<Node> nodes;
		Node* start;
		Node* goal;
	public:
		Graph() { this->start = nullptr; this->goal = nullptr; }
		Graph(std::vector<std::vector<TD::Cell*>>& map);
		Node& addNode(unsigned int idx);
		Node* getNode(unsigned int idx);
		Node* getStart() { return this->start; };
		Node* getGoal() { return this->goal; };
	};

	class IPathFinder
	{
	protected:
		Node::EdgeList path;

	public:
		virtual bool pathFinder(Graph& graph) = 0;
	};

	class PathRecord
	{
	public:
		ai::Node* node;
		PathRecord* prev;
		
		unsigned int costSoFar;
		ai::Edge connection;

		PathRecord();
		PathRecord(const PathRecord& pathRecord);
		PathRecord(ai::Node* node, ai::Edge edge, PathRecord* prev);

		~PathRecord();

		bool operator==(PathRecord& pathRecord) { return this->node == pathRecord.node; };
	};

	struct SortByCostSoFar
	{
		inline bool operator()(const PathRecord& a, const PathRecord& b);
	};

	class OpenNodeList
	{
	private:
		std::vector<PathRecord> m_recordsQueue;
	public:
		void operator+=(PathRecord& node);
		void operator-=(PathRecord& node);
		const bool isEmpty() const;
		PathRecord& popSmallestCostSoFarNode();
		PathRecord& getSmallestCostSoFarNode();
		PathRecord* find(const Node& node);
		void SortByCostSoFar();
	};

	class CloseNodeList
	{
	private:
		std::vector<PathRecord> m_recordsQueue;
	public:
		void operator+=(const PathRecord& node);
		const bool contains(const Node* node) const;
		const PathRecord* find(const Node& node) const;
	};

	class Dijkstra : public IPathFinder
	{
	private:
	public:
		bool pathFinder(Graph& graph) override;
		Node::EdgeList& getPath() { return this->path; };
		sf::Vector2i getMapIdx(unsigned int pathIndx, unsigned int row);
	};
}