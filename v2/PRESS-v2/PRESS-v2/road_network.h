//
//  road_network.h
//  PRESS-v2
//
//  Created by Renchu Song on 6/2/14.
//  Copyright (c) 2014 Renchu Song. All rights reserved.
//

#ifndef PRESS_v2_road_network_h
#define PRESS_v2_road_network_h

#include <iostream>
#include <vector>
#include "file_processor.h"
#include "utility.h"

class Node;

// Edge of graph
class Edge {
public:
	int id;												// node id
	Node *startNode, *endNode;							// two endpoints
	int geoSize = 0;									// geometry point number
	vector<EcldPoint*> geometry;						// geometry discription
	double len;
	
	Edge(int id, Node* startNode, Node* endNode) {
		this->id = id;
		this->startNode = startNode;
		this->endNode = endNode;
		this->geometry.clear();
	}
	
	// Add a new geometry point
	void appendGeometry(EcldPoint* point) {
		++this->geoSize;
		this->geometry.push_back(point);
	}
	
	// Set the length of the edge to double value
	void setLength(double len) {
		this->len = len;
	}
	
	// Use geometry to calculate the length of the edge
	void setLength() {
		if (this->geometry.empty()) {
			throw "Geometry not initialized";
		}
		this->len = 0;
		for (int i = 1; i < geoSize; ++i) {
			this->len += ecldDistance(this->geometry[i - 1], this->geometry[i]);
		}
	}
	
	// display edge structure
	void display() {
		cout << "Edge " << this->id << ":" << endl;
		for (int i = 0; i < this->geoSize; ++i) {
			cout << "(" << geometry[i]->x << "," << geometry[i]->y << ") ";
		}
		cout << "Length: " << this->len << endl;
	}
};

// Node of graph
class Node {
public:
	int id;												// node id
	EcldPoint* location = NULL;							// node location
	int edgeNumber = 0;									// number of edges start from the node
	vector<Edge*> outEdges;								// edges start from the node
	
	Node(int id, double x, double y) {
		this->id = id;
		this->location = new EcldPoint(x, y);
		this->outEdges.clear();
	}
	
	// Add a new edge start from the node
	void appendOutEdge(Edge* e) {
		++this->edgeNumber;
		this->outEdges.push_back(e);
	}
	
	// display the node structure
	void display() {
		cout << "Node " << this->id << ":" << endl;
		cout << "location (" << location->x << " " << location-> y << ")" << endl;
	}
	
	// delete node
	~Node() {
		delete this->location;
	}
};

// Road network graph
class Graph {
public:
	int edgeNumber;										// Number of edges in the graph
	vector<Edge*> edgeList;								// Edges in the graph
	int nodeNumber;										// Number of nodes in the graph
	vector<Node*> nodeList;								// Nodes in the graph
	
	Graph(FileReader* frNode, FileReader* frEdge, FileReader* frGeo) {
		this->nodeNumber = 0;
		int tmpId = 0;
		while ((tmpId = frNode->nextInt()) != EOF) {
			++this->nodeNumber;
			nodeList.push_back(new Node(tmpId, frNode->nextDouble(), frNode->nextDouble()));
		}
		
		this->edgeNumber = 0;
		while ((tmpId = frEdge->nextInt()) != EOF) {
			++edgeNumber;
			
			Node* start = getNode(frEdge->nextInt());
			Node* end = getNode(frEdge->nextInt());
			Edge* edge = new Edge(tmpId, start, end);
			
			edgeList.push_back(edge);
			start->appendOutEdge(edge);
			
			frEdge->nextInt();
		}
		
		for (int i = 0; i < edgeNumber; ++i) {
			tmpId = frGeo->nextInt();
			Edge* edge = getEdge(tmpId);
			frGeo->nextChar();
			frGeo->nextChar();
			while (frGeo->nextChar() != '^') {}
			frGeo->nextInt();
			while (frGeo->nextChar() == '^') {
				double x = frGeo->nextDouble();
				frGeo->nextChar();
				double y = frGeo->nextDouble();
				edge->appendGeometry(new EcldPoint(x, y));
			}
			edge->setLength();
		}
	}
	
	// delete graph
	~Graph() {
		for (int i = 0; i < edgeNumber; ++i) {
			delete edgeList[i];
		}
		for (int i = 0; i < nodeNumber; ++i) {
			delete nodeList[i];
		}
	}
	
	// Get node by node id
	Node* getNode(int id) {
		if (nodeList[id]->id == id) {
			return nodeList[id];
		} else {
			for (int i = 0; i < nodeNumber; ++i) {
				if (nodeList[i]->id == id) {
					return nodeList[i];
				}
			}
		}
		return NULL;
	}
	
	// Get edge by edge id
	Edge* getEdge(int id) {
		if (edgeList[id]->id == id) {
			return edgeList[id];
		} else {
			for (int i = 0; i < edgeNumber; ++i) {
				if (edgeList[i]->id == id) {
					return edgeList[i];
				}
			}
		}
		return NULL;
	}
	
	// display the graph
	void display() {
		cout << "NODES" << endl;
		for (int i = 0; i < nodeNumber; ++i) {
			nodeList[i]->display();
			cout << "Linked edges:" << endl;
			for (int j = 0; j < nodeList[i]->edgeNumber; ++j) {
				cout << nodeList[i]->outEdges[j]->id << " ";
			}
			cout << endl << endl;
		}
		cout << endl << endl;
		
		cout << "EDGES" << endl;
		for (int i = 0; i < edgeNumber; ++i) {
			edgeList[i]->display();
			cout << "Endpoints:" << edgeList[i]->startNode->id << " " << edgeList[i]->endNode->id << endl << endl;
		}
		cout << endl;
	}
	
	// Calculate shortest path table and store
	void calSPTable(FileWriter* fw) {
		double* shortLen = new double[this->edgeNumber];
		int* pre = new int[this->edgeNumber];
		
		for (int s = 0; s < edgeNumber; ++s) {
			for (int j = 0; j < edgeNumber; ++j) {			//
				shortLen[j] = 1e100;
			}
			
		}
		
		delete[] shortLen;
		delete[] pre;
	}
};

#endif
