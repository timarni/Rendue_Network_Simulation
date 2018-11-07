#include "random.h"
#include "network.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Network::resize(const size_t& networkSize){
	values.resize(networkSize);
	RNG.normal(values);
}

bool Network::add_link(const size_t& node_1, const size_t& node_2){
  size_t nbNodes(size());
  
  if(node_1 >= nbNodes or node_2 >= nbNodes or node_1==node_2){
    return false;
  }
  
  else {
	vector<size_t> Ngb (neighbors(node_1));
	
	for (size_t i = 0; i < Ngb.size(); ++i){
		if(Ngb[i] == node_2){ return false; } //if they are already connected
	}  
	
	links.insert(std::pair<size_t,size_t>(node_1, node_2));
	links.insert(std::pair<size_t,size_t>(node_2, node_1));
	return true;
	}
}


size_t Network::random_connect(const double& lamda){
	
	links.clear();
	size_t sumConnections(0);
	std::random_device rd;
	std::mt19937 randomNumbers(rd());
	std::uniform_int_distribution<> dis_unif_int(0, size());
	
	for (size_t node=0; node < size(); ++node){
				
		size_t myDegree (RNG.poisson(lamda)); //- links.count(node)); if (myDegree > 0){} would be better but this here doesn't work, overflow, programm does not end to run
		
		for(size_t i = 0; i < myDegree; ++i){
			
			size_t newConnectionPoint(node);
			
			while ((newConnectionPoint == node)) { //we could add: or (links.find(newConnectionPoint)->second == node)) { //if the link is already established it shouldn't do it again or can there be several links between two nodes?
			
				newConnectionPoint = dis_unif_int(randomNumbers);
			}
				
			links.insert(std::pair<size_t,size_t>(node, newConnectionPoint));
			links.insert(std::pair<size_t,size_t>(newConnectionPoint, node));
			++sumConnections;
		}
	}
	return sumConnections;		
}

size_t Network::set_values(const std::vector<double>& newValues){
	size_t s(size());
	size_t n(newValues.size());
	
	if(n<s){
		for (size_t i = 0; i < n; ++i){
			values[i] = newValues[i];
		}
	return n;
	}
	
	else if (n>s){
		for (size_t i = 0; i < s; ++i){
			values[i] = newValues[i];
		}
	return s;
	}
	
	else {values = newValues; return size();} //When n==s
}


size_t Network::size() const {return values.size();}


size_t Network::degree(const size_t &_n) const{ return links.count(_n); }

double Network::value(const size_t &_n) const { 
	if (_n < size()) { return values[_n]; }
	else {return 0;}
}

std::vector<double> Network::sorted_values() const{
  std::vector<double> return_value(values);
  std::sort(return_value.begin(), return_value.end(), std::greater<double>());
  return return_value;
}
    
std::vector<size_t> Network::neighbors(const size_t& myNode) const{
	std::vector<size_t> myNeighbors;
	if (myNode < size()){
		for(auto I = links.begin(); I != links.end(); I++) {	
			if (I->first == myNode){myNeighbors.push_back(I->second);}	
		}
	}
	return myNeighbors;
}

