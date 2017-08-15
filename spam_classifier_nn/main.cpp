#include <stdio.h>
#include <iostream>

#include "neural_network.h"
#include "matrix.h"
#include "data_manipulations.h"

//specifying the type for all of the operations of the neural_network, data_extraction, and matricies
#define TYPE double

int main() {
	
	//extracting data
	data_extractor<TYPE> dataSpam("C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\spam\\message", 1606, 1);
	data_extractor<TYPE> dataHam("C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\ham\\message", 1543, 0);

	//getting the dimentions of the data matrix
	std::vector<int> mns = dataSpam.getData().getSize();
	std::vector<int> mnh = dataHam.getData().getSize();

	/*std::vector<int> mn2 = testData.getData().getSize();

	//checking that the test and training data match
	if (mn[1] != mn2[1]) {
		std::cerr << "test and train data do not match" << std::endl;
		exit(1);
	}*/

	//determining number of examples for training and testing
	int trainingSpam = int(mns[0] * 0.6);
	int trainingHam = int(mnh[0] * 0.6);
	std::cout << mns[0] << " " << trainingSpam << " " << trainingHam << std::endl;

	//separating test and train
	matrix<TYPE> train = dataSpam.getData().getRows(0, trainingSpam);
	train.addRow(dataHam.getData().getRows(0, trainingHam));
	matrix<TYPE> test = dataSpam.getData().getRows(trainingSpam + 1, mns[0] - 1);
	test.addRow(dataHam.getData().getRows(trainingHam + 1, mnh[0] - 1));


	std::vector<int> mn1 = test.getSize();
	std::vector<int> mn2 = train.getSize();

	//checking that the test and training data match
	if (mn1[1] != mn2[1]) {
		std::cerr << "test and train data do not match" << std::endl;
		exit(1);
	}

	//separating X and Y
	matrix<TYPE> X = train.getColumn(1, mn1[1] - 2);
	matrix<TYPE> Y = train.getColumn(mn1[1] - 1);
	matrix<TYPE> X2 = test.getColumn(1, mn2[1] - 2);
	matrix<TYPE> Y2 = test.getColumn(mn2[1] - 1);

	//setting up the neural network
	std::vector<int> layers = { X.getSize()[1], 40, 40, 1 };
	neural_network<TYPE> net(layers);

	net.gradient(X, Y);

	std::cout << "running backprop..." << std::endl;
	net.back_propogation(X, Y, 0.1, 10000, X2, Y2);

	std::cout << "done" << std::endl;
	getchar();
	

	/*
	matrix<TYPE> X(5, 10);
	X.random(5);
	matrix<TYPE> Y(3, 1, 1);
	matrix<TYPE> m1(1, 1, 0);
	for (int i = 0; i < 2; i++) {
		Y.addRow(m1);
	}
	

	std::vector<int> layers = { X.getSize()[1], 5, 1 };
	neural_network<TYPE> net(layers);

	net.gradient_check(X, Y);
	
	std::cout << "done" << std::endl;
	std::getchar();
	*/
}