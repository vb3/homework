//
//  main.cpp
//  CS 331 Assignment #1: Uninformed and Informed Search
//  Cannibals and Missionaries puzzle
//
//
//  Created by Vinay Bikkina on 4/14/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <iostream>
#include <list>
#include <algorithm>
#include <stdio.h>

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

//actions
const node r_actions[5] = {{0,1,0,-1,NULL,NULL,NULL}, {0,2,0,-2,NULL,NULL,NULL},
						   {1,0,-1,0,NULL,NULL,NULL}, {1,1,-1,-1,NULL,NULL,NULL},
						   {2,0,-2,0,NULL,NULL,NULL}};
const node l_actions[5] = {{0,-1,0,1,NULL,NULL,NULL}, {0,-2,0,2,NULL,NULL,NULL},
						   {-1,0,1,0,NULL,NULL,NULL}, {-1,-1,1,1,NULL,NULL,NULL},
						   {-2,0,2,0,NULL,NULL,NULL}};


list<node> graph_search(node start, node goal);
list<node> expand(node myNode);
bool validate_node(node myNode);

int main(int argc, const char * argv[])
{
	list<node> soln;
	printf("test\n");
	
	node start = {3,3,0,0,true,false,NULL};
	node goal = {0,0,3,3,false,true,NULL};
	
	soln = graph_search(start, goal);
	
	while (!soln.empty()) {
		printf("%d, %d, %d\n%d, %d, %d\n\n", soln.front().lmis, soln.front().lcan, soln.front().lboat,
									     soln.front().rmis, soln.front().rcan, soln.front().rboat);
		soln.pop_front();
	}
	
	printf("done\n");
	return 0;
}

list<node> graph_search(node start, node goal)
{
	list<node> fringe;
	list<node> closed;
	list<node> solution;
	node current;
	int counter=0;
	
	fringe.push_front(start);
	while (!fringe.empty()) {
		current = fringe.front();
		fringe.pop_front();
		
		counter++;
		
		if (current == goal) {
			while (!(current == start)) {
				solution.push_front(current);
				current = *current.parent;
				printf("%d, %d, %d\n%d, %d, %d\n\n", solution.front().lmis, solution.front().lcan, solution.front().lboat,
					   solution.front().rmis, solution.front().rcan, solution.front().rboat);
			}
			solution.push_front(start);
			
			return solution;
		}
		
		//if solution.front() is not in closed
		if(find(closed.begin(), closed.end(), current) == closed.end()) {
			closed.push_front(current);
			
			//add all expanded nodes to fringe
			fringe.splice(fringe.begin(), expand(current));
			//fringe.insert(fringe.begin(), expand(current).begin(), expand(current).end());
		}
	}
	
	return fringe;
}

list<node> expand(node myNode) {
	list<node> successors;
	node s;
	int i;
	for (i=0; i<5; i++) {
		s = myNode + (myNode.lboat ? l_actions[i] : r_actions[i]);
		
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




