//
//  main.cpp
//  CS 331 Assignment #1: Uninformed and Informed Search
//  Cannibals and Missionaries puzzle
//
//	exec by passing parameters < initial state file > < goal state file > < mode > < output file >
//
//
//  Created by Vinay Bikkina on 4/14/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <stdio.h>
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

//actions {lcan, lmis, rcan, rmis, lboat, rboat, *parent}
const node r_actions[5] = {{0,1,0,-1,NULL,NULL,NULL}, {0,2,0,-2,NULL,NULL,NULL},
						   {1,0,-1,0,NULL,NULL,NULL}, {1,1,-1,-1,NULL,NULL,NULL},
						   {2,0,-2,0,NULL,NULL,NULL}};
const node l_actions[5] = {{0,-1,0,1,NULL,NULL,NULL}, {0,-2,0,2,NULL,NULL,NULL},
						   {-1,0,1,0,NULL,NULL,NULL}, {-1,-1,1,1,NULL,NULL,NULL},
						   {-2,0,2,0,NULL,NULL,NULL}};
int counter=0;

list<node> graph_search_bfs_dfs(node start, node goal, bool dfs);
list<node> expand(node *myNode);
bool validate_node(node myNode);



int main(int argc, const char * argv[])
{
	//get all command line arguments
	string startfile, goalfile, mode, outfile;
	startfile = argv[1];
	goalfile = argv[2];
	mode = argv[3];
	outfile = argv[4];
	
	printf("Computing...\n");
	list<node> soln;
	ifstream infile_start(startfile);
	ifstream infile_goal(goalfile);
	string line;
	node start, goal;
	istringstream *iss;
	
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
	soln = graph_search_bfs_dfs(start, goal, false);
	
	//print solution
	while (!soln.empty()) {
		printf("%d, %d, %d\n", soln.front().lmis, soln.front().lcan, soln.front().lboat);
		printf("%d, %d, %d\n\n", soln.front().rmis, soln.front().rcan, soln.front().rboat);
		soln.pop_front();
	}
	
	printf("done. counter=%d\n", counter);
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
			current = fringe.back();
			fringe.pop_back();
		}
		else {
			current = fringe.front();
			fringe.pop_front();
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




