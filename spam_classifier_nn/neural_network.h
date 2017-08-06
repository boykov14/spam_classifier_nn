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
	std::vector<matrix<T>> back_propogation(matrix<T> X, matrix<T> y, T a, unsigned int iterations, matrix<T> XT, matrix<T> yT);
	T percent_error(matrix<T> X, matrix<T> y);
	void gradient_check(matrix<T> X, matrix<T> y);

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

/**method which takes the gradient of the neural network
*@param a matrix X of test cases and a corresponding matrix y with labels for every test case in X
*@return the resulting gradient matricies
*/
template<class T>
std::vector<matrix<T>> neural_network<T>::gradient(matrix<T> X, matrix<T> y) {
	//variables to store the activations and z values
	std::vector <matrix<T>> A;
	std::vector <matrix<T>> Z;

	matrix<T> a = X;
	matrix<T> z;
	
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
		matrix<T> d;
		if (i == 0) {
			d = (Thetas[layers - 2 - i].transpose() * D[layers - 2 - i].transpose());
		}
		else {
			d = (Thetas[layers - 2 - i].transpose() * D[layers - 2 - i]);
		}
		d = d.dot((Z[layers - 3- i].transpose()).sigmoid_gradient());
		d = d.getRows(1, d.getSize()[0] - 1);
		D[layers - 3 - i] = d;
	}

	//geting the number of test cases
	float m = float(X.getSize()[0]);

	for (int i = 0; i < layers - 2; i++) {
		D[i] = (D[i] * A[i])/m;
	}
	D[layers - 2] = (D[layers - 2].transpose() * A[layers - 2])/m;
	
	return D;
}

/**method which runs backpropogation on the neural network to minimize it's cost function
*@param matrix of test cases X, matrix of labels y, learning rate alpha, and the number of iterations, also training cases x and y
*@return the resulting weights
*/
template<class T>
std::vector<matrix<T>> neural_network<T>::back_propogation(matrix<T> X, matrix<T> y, T alpha, unsigned int iterations, matrix<T> XT, matrix<T> yT) {
	for (int i = 0; i < iterations; i++) {
		std::vector<matrix<T>> D = gradient(X, y);
		
		for (int j = 0; j < layers - 1; j++) {
			matrix<T> error = D[j] * alpha;
			Thetas[j] = Thetas[j] - error;
			
		}
		if (i % 100 == 0) {
			std::cout << percent_error(XT, yT) << "    " << percent_error(X, y) <<std::endl;
			alpha = alpha * 0.99;
		}
	}

	std::cout << percent_error(X, y) << std::endl;
	return Thetas;
}

template<class T>
T neural_network<T>::percent_error(matrix<T> X, matrix<T> y) {
	matrix<T> result = forwards_propogation(X);
	float correct = 0;

	for (int i = 0; i < X.getSize()[0]; i++) {
		if (result[i][0] >= 0.5) {
			result[i][0] = 1;
		}
		else {
			result[i][0] = 0;
		}

		if (result[i][0] == y[i][0]) {
			correct += 1;
		}
	}

	return 1 - correct / (float(X.getSize()[0]));
}

/*function to test the gradient function
*/
template<class T>
void neural_network<T>::gradient_check(matrix<T> X, matrix<T> y) {
	std::vector<matrix<T>>  grad = gradient(X,  y);
	std::vector<matrix<T>>  manual_grad(grad.size());
	T e = 0.0001;

	for (int i = 0; i < grad.size(); i++) {
		vector<int> mn = Thetas[i].getSize();
		matrix<T> theta(mn[0], mn[1]);
		for (int j = 0; j <mn[0]; j++) {
			for (int k = 0; k < mn[1]; k++) {
				//varying thetas[i] jk to get the gradient with respect to that weight
				Thetas[i][j][k] = Thetas[i][j][k] + e;
				float cost1 = cost(X, y);
				Thetas[i][j][k] = Thetas[i][j][k] - 2 * e;
				float cost2 = cost(X, y);

				//returning thetas[i] jk to its original value
				Thetas[i][j][k] = Thetas[i][j][k] + e;

				//calculating the gradient
				theta[j][k] = (cost1 - cost2)/(2 * e);
				//std::cout << "1";
			}
			//std::cout << "\n";
		}
		//std::cout << "\n";
		manual_grad[i] = theta;
	}

	for (int i = 0; i < grad.size(); i++) {
		grad[i].display();
		manual_grad[i].display();
		std::cout << "\n";
	}
}

#endif