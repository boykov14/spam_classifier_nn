#include <stdio.h>
#include <iostream>

#include "neural_network.h"
#include "matrix.h"
#include "data_manipulations.h"

//specifying the type for all of the operations of the neural_network, data_extraction, and matricies
#define TYPE float

int main() {

	//extracting data
	data_extractor<TYPE> data("C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\SpamTrain\\message", "C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\HamTrain\\message", 1078, 1080);
	data_extractor<TYPE> testData("C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\SpamTest\\message", "C:\\home\\Anton\\coding\\data\\grade_12_spamClassifier\\HamSpamToStudents\\HamTest\\message", 528, 463);

	//getting the dimentions of the data matrix
	std::vector<int> mn = data.getData().getSize();
	std::vector<int> mn2 = testData.getData().getSize();

	//checking that the test and training data match
	if (mn[1] != mn2[1]) {
		std::cerr << "test and train data do not match" << std::endl;
		exit(1);
	}


	//separating X and Y
	matrix<TYPE> X = data.getData().getColumn(1, mn[1] - 2);
	matrix<TYPE> Y = data.getData().getColumn(mn[1] - 1);
	matrix<TYPE> X2 = testData.getData().getColumn(1, mn2[1] - 2);
	matrix<TYPE> Y2 = testData.getData().getColumn(mn2[1] - 1);

	//setting up the neural network
	std::vector<int> layers = { X.getSize()[1],25,1 };
	neural_network<TYPE> net(layers);

	/*std::cout << */(net.gradient(X, Y));

	std::cout << "done" << std::endl;
	std::getchar();
}