#include <stdio.h>
#include <iostream>

#include "linearClassifier.h"
#include "matrix.h"
#include "dataManipulations.h"

int main() {

	//extracting data
	matrix data = getData("C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\SpamTrain\\message", "C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\HamTrain\\message", 1078, 1080);
	matrix testData = getData("C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\SpamTest\\message", "C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\HamTest\\message", 528, 463);

	//getting the dimentions of the data matrix
	std::vector<int> mn = data.getSize();
	std::vector<int> mn2 = data.getSize();

	//checking that the test and training data match
	if (mn[1] != mn2[1]) {
		std::cerr << "test and train data do not match" << std::endl;
		exit(1);
	}


	//separating X and Y
	matrix X = data.getColumn(0, mn[1] - 2);
	matrix Y = data.getColumn(mn[1] - 1);
	matrix X2 = testData.getColumn(0, mn2[1] - 2);
	matrix Y2 = testData.getColumn(mn2[1] - 1);

	//setting up the initial weights lambada , alpha, and the number of iterations for gradient descent and normalizing X
	matrix theta(X.getSize()[1], 1, 0);

	float lambada = 0;

	float alpha = 0.012;

	int iterations = 5000;

	//normalizing the data
	std::pair<matrix, std::vector<matrix>> normalized = normalizeFeatures(X);
	X = normalized.first;
	X2 = normalizeFeatures(X2, normalized.second);

	std::cout << percentError(theta, X, Y, lambada) << std::endl;
	std::cout << percentError(theta, X2, Y2, lambada) << std::endl;

	//beggin gradient descent
	std::cout << "Running gradient descent...\n";
	theta = gradientDescent(theta, X, Y, lambada, alpha, iterations);

	//display the final weights and cost
	theta.display();
	std::cout << percentError(theta, X2, Y2, lambada);

	std::getchar();
}