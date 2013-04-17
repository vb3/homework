//
//  main.cpp
//  CS 331 Assignment #1: Uninformed and Informed Search
//  Cannibals and Missionaries puzzle
//
//	exec by passing parameters < initial state file > < goal state file > < mode > < output file >
//	The mode argument is either:
//	bfs (for breadth-first search)
//	dfs (for depth-first search)
//	iddfs (for iterative deepening depth-first search)
//	astar (for A-Star search below)
//
//  Created by Vinay Bikkina on 4/14/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct node {
	int lcan, lmis, rcan, rmis;
	bool lboat, rboat;
	node *parent;
	
	bool operator==(const node& rhs)
	{
		return (*this).lcan == rhs.lcan &&
			   (*this).lmis == rhs.lmis &&
			   (*this).rcan == rhs.rcan &&
			   (*this).rmis == rhs.rmis &&
			   (*this).lboat == rhs.lboat &&
			   (*this).rboat == rhs.rboat;
	}
	
	node operator+(const node& rhs)
	{
		node sum;
		sum.lcan = (*this).lcan + rhs.lcan;
		sum.lmis = (*this).lmis + rhs.lmis;
		sum.rcan = (*this).rcan + rhs.rcan;
		sum.rmis = (*this).rmis + rhs.rmis;
		sum.lboat = !(*this).lboat;
		sum.rboat = !(*this).rboat;
		sum.parent = this;
		return sum;
	}
};

struct depth_node {
	node *myNode;
	int depth;
};

//actions {lcan, lmis, rcan, rmis, lboat, rboat, *parent}
const node r_actions[5] = {{0,1,0,-1,NULL,NULL,NULL}, {0,2,0,-2,NULL,NULL,NULL},
						   {1,0,-1,0,NULL,NULL,NULL}, {1,1,-1,-1,NULL,NULL,NULL},
						   {2,0,-2,0,NULL,NULL,NULL}};
const node l_actions[5] = {{0,-1,0,1,NULL,NULL,NULL}, {0,-2,0,2,NULL,NULL,NULL},
						   {-1,0,1,0,NULL,NULL,NULL}, {-1,-1,1,1,NULL,NULL,NULL},
						   {-2,0,2,0,NULL,NULL,NULL}};
int counter=0;

list<node> graph_search_bfs_dfs(node start, node goal, bool dfs);
list<node> graph_search_iddfs(depth_node start, depth_node goal, int depth);
list<node> expand(node *myNode);
list<depth_node> depth_expand(node *myNode, int depth);
bool validate_node(node myNode);



int main(int argc, const char * argv[])
{
	//get all command line arguments
	list<node> soln;
	ifstream infile_start(argv[1]), infile_goal(argv[2]);
	ofstream outfile(argv[4]);
	string line;
	node start, goal;
	istringstream *iss;
	string mode = argv[3];
	
	cout << "Computing... ";
	outfile << "Computing... ";
	
	//fill start and goal nodes
	getline(infile_start, line);
	iss = new istringstream(line);
	*iss >> start.lmis; iss->ignore();
	*iss >> start.lcan; iss->ignore();
	*iss >> start.lboat; iss->ignore();
	getline(infile_start, line);
	delete iss; iss = new istringstream(line);
	iss->str(line);
	*iss >> start.rmis; iss->ignore();
	*iss >> start.rcan; iss->ignore();
	*iss >> start.rboat; iss->ignore();
	getline(infile_goal, line);
	delete iss; iss = new istringstream(line);
	*iss >> goal.lmis; iss->ignore();
	*iss >> goal.lcan; iss->ignore();
	*iss >> goal.lboat; iss->ignore();
	getline(infile_goal, line);
	delete iss; iss = new istringstream(line);
	iss->str(line);
	*iss >> goal.rmis; iss->ignore();
	*iss >> goal.rcan; iss->ignore();
	*iss >> goal.rboat; iss->ignore();
	
	//close files
	infile_goal.close();
	infile_start.close();
	
	//compute
	if (mode == "bfs") {
		soln = graph_search_bfs_dfs(start, goal, false);
	}
	else if (mode == "dfs") {
		soln = graph_search_bfs_dfs(start, goal, true);
	}
	else if (mode == "iddfs"){
		depth_node myStart;
		depth_node myGoal;
		int test_depth = 0;
		
		myStart.myNode = new node(start);
		myStart.depth = 0;
		myGoal.myNode = new node(goal);
		myGoal.depth = 0;
		
		while (soln.empty())
			soln = graph_search_iddfs(myStart, myGoal, test_depth++);
		
		//cout << "test_depth: " << test_depth << endl;
	}
	else if (mode == "astar"){
		
	}
	else {
		cout << "ERROR: Not valid mode '" << mode << "'." << endl;
		exit(-1);
	}
	cout << "in " << mode << " mode." << endl << endl;
	outfile << "in " << mode << " mode." << endl << endl;
	
	//print solution
	while (!soln.empty()) {
		cout << soln.front().lmis << ", ";
		cout << soln.front().lcan << ", ";
		cout << soln.front().lboat << endl;
		
		cout << soln.front().rmis << ", ";
		cout << soln.front().rcan << ", ";
		cout << soln.front().rboat << endl << endl;
		
		outfile << soln.front().lmis << ", ";
		outfile << soln.front().lcan << ", ";
		outfile << soln.front().lboat << endl;
		
		outfile << soln.front().rmis << ", ";
		outfile << soln.front().rcan << ", ";
		outfile << soln.front().rboat << endl << endl;

		soln.pop_front();
	}
	
	cout << "done. counter=" << counter << endl;
	outfile << "done. counter=" << counter << endl;
	return 0;
}

list<node> graph_search_bfs_dfs(node start, node goal, bool dfs)
{
	list<node> fringe;
	list<node> closed;
	list<node> solution;
	node current;
	
	fringe.push_front(start);
	while (!fringe.empty()) {
		if (dfs) {
			current = fringe.front();
			fringe.pop_front();
		}
		else {
			current = fringe.back();
			fringe.pop_back();
		}
		
		if (current == goal) {
			while (!(current == start)) {
				solution.push_front(current);
				current = *current.parent;
			}
			solution.push_front(start);
			
			return solution;
		}
		
		//if solution.front() is not in closed
		if(find(closed.begin(), closed.end(), current) == closed.end()) {
			counter++;
			closed.push_front(current);
			
			node *savedNode = new node;
			*savedNode = current;
			
			//add all expanded nodes to fringe
			fringe.splice(fringe.begin(), expand(savedNode));
		}
	}
	
	return fringe;
}

list<node> graph_search_iddfs(depth_node start, depth_node goal, int depth)
{
	list<depth_node> fringe;
	list<node> closed;
	list<node> solution;
	depth_node current;
	
	counter = 0;
	
	fringe.push_front(start);
	while (!fringe.empty()) {
		current = fringe.front();
		fringe.pop_front();
		
		if (*current.myNode == *goal.myNode) {
			node soln = *current.myNode;
			while (!(soln == *start.myNode)) {
				solution.push_front(soln);
				soln = *soln.parent;
			}
			solution.push_front(*start.myNode);
			
			return solution;
		}
		
		//if solution.front() is not in closed
		if(find(closed.begin(), closed.end(), *current.myNode) == closed.end()) {
			counter++;
			closed.push_front(*current.myNode);
			
			node *savedNode = new node;
			*savedNode = *current.myNode;
			
			//add all expanded nodes to fringe
			if (depth > current.depth)
				fringe.splice(fringe.begin(), depth_expand(savedNode, current.depth));
		}
	}
	
	return solution;
}

list<node> expand(node *myNode) {
	list<node> successors;
	node s;
	int i;
	for (i=0; i<5; i++) {
		s = (*myNode) + (myNode->lboat ? l_actions[i] : r_actions[i]);
		
		if (validate_node(s)) successors.push_front(s);
	}
	
	return successors;
}

list<depth_node> depth_expand(node *myNode, int depth) {
	list<depth_node> successors;
	depth_node s;
	s.depth = depth + 1;
	
	int i;
	for (i=0; i<5; i++) {
		s.myNode = new node;
		
		*s.myNode = (*myNode) + (myNode->lboat ? l_actions[i] : r_actions[i]);
		if (validate_node(*s.myNode)) successors.push_front(s);
	}
	
	return successors;
}

bool validate_node(node myNode){
	//check if everything is positive
	if (myNode.lcan < 0 || myNode.lmis < 0 ||
		myNode.rcan < 0 || myNode.rmis < 0) {
		return false;
	}
	
	//check if more missionaries than cannibals
	if ((myNode.rmis != 0 && myNode.rmis < myNode.rcan) ||
		(myNode.lmis != 0 && myNode.lmis < myNode.lcan)) {
		return false;
	}
	
	return true;
}




