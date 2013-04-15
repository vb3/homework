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
#include <fstream>
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

//actions {lcan, lmis, rcan, rmis, lboat, rboat, *parent}
const node r_actions[5] = {{0,1,0,-1,NULL,NULL,NULL}, {0,2,0,-2,NULL,NULL,NULL},
						   {1,0,-1,0,NULL,NULL,NULL}, {1,1,-1,-1,NULL,NULL,NULL},
						   {2,0,-2,0,NULL,NULL,NULL}};
const node l_actions[5] = {{0,-1,0,1,NULL,NULL,NULL}, {0,-2,0,2,NULL,NULL,NULL},
						   {-1,0,1,0,NULL,NULL,NULL}, {-1,-1,1,1,NULL,NULL,NULL},
						   {-2,0,2,0,NULL,NULL,NULL}};
int counter=0;

list<node> graph_search(node start, node goal);
list<node> expand(node *myNode);
bool validate_node(node myNode);



int main(int argc, const char * argv[])
{
	printf("Computing...\n");
	list<node> soln;
	ifstream my_input;
	int lcan, lmis, rcan, rmis;
	bool lboat, rboat;
	string test_input;
	
	
	my_input.open("/Users/vb2303/Documents/OSU/GitHub_homework/CS331/Assignment1/Assignment1/start.txt");
	getline(my_input, test_input);
	stringstream ss(test_input);
	ss >> lmis >> lcan >> lboat;
	
	
	my_input >> lmis; // >> lcan >> lboat;
	my_input >> lcan;
	my_input >> lboat;
	my_input >> rmis;
	my_input >> rcan;
	my_input >> rboat;

	node start = {lcan,lmis,rcan,rmis,lboat,rboat,NULL};
	node goal = {0,0,3,3,false,true,NULL};
	
	soln = graph_search(start, goal);
	
	while (!soln.empty()) {
		printf("%d, %d, %d\n%d, %d, %d\n\n", soln.front().lmis, soln.front().lcan, soln.front().lboat,
									     soln.front().rmis, soln.front().rcan, soln.front().rboat);
		soln.pop_front();
	}
	
	printf("done. counter=%d\n", counter);
	return 0;
}

list<node> graph_search(node start, node goal)
{
	list<node> fringe;
	list<node> closed;
	list<node> solution;
	node current;
	
	fringe.push_front(start);
	while (!fringe.empty()) {
		current = fringe.front();
		fringe.pop_front();
		
		counter++;
		
		if (current == goal) {
			while (!(current == start)) {
				solution.push_front(current);
				current = *current.parent;
				//printf("%d, %d, %d\n%d, %d, %d\n\n", solution.front().lmis, solution.front().lcan, solution.front().lboat,
					   //solution.front().rmis, solution.front().rcan, solution.front().rboat);
			}
			solution.push_front(start);
			
			return solution;
		}
		
		//if solution.front() is not in closed
		if(find(closed.begin(), closed.end(), current) == closed.end()) {
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




