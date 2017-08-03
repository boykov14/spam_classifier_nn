#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <vector>
#include <string>
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "matrix.h"

template <class T>
class neural_network {

private:

	int layers;
	std::vector<int> dimentions;
	std::vector<matrix<T>> Thetas;

public:

	neural_network(std::vector<int> dims);
	~neural_network();

	matrix<T> forwards_propogation();
	float cost();
};

/*constructor for the neural network which initializes thetas to random values
@param number of layers and a vector specifying the number of nodes in each layer
*/
template <class T>
neural_network<T>::neural_network(std::vector<int> dims) {
	layers = dims.size();
	dimentions = dims;
	
	//initializing thetas
	for (int i = 1; i <= layers; i++) {
		matrix<T> newM(dims[i], dims[i-1]);
		newM.random(0.24);
		Thetas.push_back(newM);
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

/**propogates forwards through the neural network
*@returns the resulting matrix
*/
template<class T>
matrix<T> neural_network<T>::forwards_propogation() {
	return matrix<T> lol(1, 1);
}

/**determining the cost of the neural network
*@return the cost of the neural network
*/
template<class T>
float neural_network<T>::cost() {
	return 1.1;
}

#endif