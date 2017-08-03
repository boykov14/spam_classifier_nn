#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <vector>
#include <string>
#include <ctype.h>
#include <iostream>
#include <fstream>

//#include "matrix.h"

template <class T>
class neural_network {

private:

	int layers;
	std::vector<int> dimentions;
	T lol;
	//vector<matrix<T>> Thetas;

public:

	neural_network(std::vector<int> dims);
	~neural_network();
};

/*constructor for the neural network which initializes thetas to random values
@param number of layers and a vector specifying the number of nodes in each layer
*/
template <class T>
neural_network<T>::neural_network(std::vector<int> dims) {
	layers = dims.size();
	dimentions = dims;
	
	//initializing thetas
	for (int i = 1; i < layers; i++) {
		matrix<T> newM(1, 1, 1);
	}
}

/*destructor for the neural network
*/
template <class T>
neural_network<T>::~neural_network() {
	for (int i = 0; i < layers; i++) {
		Thetas[i].~matrix();
	}
}

#endif