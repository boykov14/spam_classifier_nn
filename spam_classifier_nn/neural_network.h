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

	matrix<T> forwards_propogation(matrix<T> X);
	float cost(matrix<T> X, matrix<T> y);
	std::vector<matrix<T>> gradient(matrix<T> X, matrix<T> y);
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
		matrix<T> newM(dims[i], dims[i-1] + 1);
		newM.random(0.24);
		Thetas.push_back(newM);
	}
}

/*destructor for the neural network
*/
template <class T>
neural_network<T>::~neural_network() {
	for (int i = 0; i < layers - 1; i++) {
		Thetas[i].~matrix();
	}
}

/**propogates forwards through the neural network
*@returns the resulting matrix
*/
template<class T>
matrix<T> neural_network<T>::forwards_propogation(matrix<T> X) {
	matrix<T> a = X;
	for (int i = 0; i < layers-1; i++) {
		matrix<T> ones(a.getSize()[0], 1, 1);
		a.addColumn(ones, 0);
		a = (a * Thetas[i].transpose()).sigmoid();
	}
	return a;
}

/**determining the cost of the neural network
*@return the cost of the neural network
*/
template<class T>
float neural_network<T>::cost(matrix<T> X, matrix<T> y) {
	matrix<T> a = forwards_propogation(X);
	float cost;
	
	//making sure output of forwards propogation is correct
	if (a.getSize()[0] != y.getSize()[0] || a.getSize()[1] != y.getSize()[1]) {
		std::cerr << "improper output from forwards propogation" << std::endl;
		exit(1);
	}

	//cost = sum(sum((-Y.*log(a3) - (1 - Y).*log(1 - a3)))). / m;
	matrix<T> p1 = (y * -1).dot(a.natLog()) ;
	matrix<T> p2 = (a * -1 + 1).natLog();
	p1 = p1 - (y * -1 + 1).dot(p2);
	float m = X.getSize()[0];
	float total = (p1.sum()).sum()[0][0];
	cost = total/ m;

	return cost;
}

template<class T>
std::vector<matrix<T>> neural_network<T>::gradient(matrix<T> X, matrix<T> y) {
	//variables to store the activations and z values
	std::vector <matrix<T>> A;
	std::vector <matrix<T>> Z;

	matrix<T> a = X;
	matrix<T> z;
	\
	for (int i = 0; i < layers - 1; i++) {
		//adding a column of ones to the activation layers
		matrix<T> ones(a.getSize()[0], 1, 1);
		a.addColumn(ones, 0);

		z = a * Thetas[i].transpose();

		A.push_back(a);
		Z.push_back(z);

		a = (z).sigmoid();
	}
	
	//adding the last activation
	A.push_back(a);
	
	//making sure output of forwards propogation is correct
	if (a.getSize()[0] != y.getSize()[0] || a.getSize()[1] != y.getSize()[1]) {
		std::cerr << "improper output from forwards propogation" << std::endl;
		exit(1);
	}
	
	std::vector<matrix<T>> D(layers - 1);

	//calculating the error for the last layer
	D[layers - 2] = a - y;
	
	for (int i = 0; i < layers - 2; i++) {
		//adding a ones column to z
		matrix<T> ones(Z[layers - 3 - i].getSize()[0], 1, 1);
		Z[layers - 3 - i].addColumn(ones, 0);

		matrix<T> d = (Thetas[layers - 2 - i].transpose() * D[layers - 2 - i].transpose());
		d = d.dot((Z[layers - 3- i].transpose()).sigmoid_gradient());
		d = d.getColumn(1, d.getSize()[1] - 1);
	}
	
	return A;
}

#endif