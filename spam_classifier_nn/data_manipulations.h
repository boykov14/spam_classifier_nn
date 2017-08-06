#ifndef DATA_MANIPULATIONS_H
#define DATA_MANIPULATIONS_H

#include <stdio.h>
#include <vector>
#include <string>
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "matrix.h"
#include "load_dictionary.h"

template <class T>
class data_extractor {


private:
	matrix<T> data;
	int X;
	int Y;

	std::vector<T> extractFeaturesText(std::string filepath, int y);

public:

	data_extractor(std::string filepath1, std::string filepath2, int numSpam, int numHam);
	~data_extractor();

	matrix<T> extractData(std::string filename);
	std::vector<matrix<T>> normalizeFeatures();
	friend matrix<T> normalizeFeatures(matrix<T> m, std::vector<matrix<T>> weights);
	matrix<T> getData();
};

template <class T>
data_extractor<T>::data_extractor(std::string filepath1, std::string filepath2, int numSpam, int numHam) {

	loadDict("dictionary.txt");

	//initializing variables to store the data
	std::vector<T> results;
	std::vector<std::vector<T>> x;

	//gatherng data from spam dataset
	for (int i = 1; i <= numSpam; i++) {

		std::string path = filepath1;

		//adding the proper extentions to the filenames
		path += to_string(i);
		path += ".txt";

		//extracting data from the file
		results = extractFeaturesText(path, 1);
		if (results.size() == 0) {
			std::cerr << "error extracting features" << std::endl;
		}

		x.push_back(results);
	}

	//gathering the ham data
	for (int i = 1; i <= numHam; i++) {

		string path = filepath2;

		//adding the proper file extentions
		path += to_string(i);
		path += ".txt";

		//extracting the data into the vect
		results = extractFeaturesText(path, 0);

		x.push_back(results);
	}

	unloadDictionary();

	//collecting data into a matrix and adding ones to the first column
	matrix<T> finalData(x);

	matrix<T> ones((finalData.getSize())[0], 1, 1);

	finalData.addColumn(ones, 0);

	data = finalData;
}

template <class T>
data_extractor<T>::~data_extractor() {
	data.~matrix();
}

/**function to extract data from a file where the data is stored in matrix form
*@param the filepath of the file
*@return the data stored in the file as a matrix
*/
template <class T>
matrix<T> data_extractor<T>::extractData(std::string filename) {
	//opening file
	std::fstream readFile(filename, std::ios::in);

	//checking for errors with file
	if (readFile.fail()) {

		std::cerr << "Error opening " << filename << "." << std::endl;
		exit(1);

	}

	//string to store file input
	std::string input;
	std::vector<std::vector<T>> data;
	int length;

	while (!readFile.eof()) {

		getline(readFile, input);

		//checking for errors with file
		if (readFile.fail() && !readFile.eof()) {

			std::cerr << "Error reading " << filename << "." << std::endl;
			exit(1);

		}

		std::string::size_type pos;
		std::vector<T> vals;


		while (input.length() > 0) {
			if (isdigit(input[0]) || input[0] == '-') {
				vals.push_back((T)(std::stof(input, &pos)));
				input = input.substr(pos);
			}
			else {
				input = input.substr(1);
			}
		}

		if (vals.size() > 0) {
			data.push_back(vals);
		}

	}

	matrix<T> DATA(data);
	return data;
}

/**function used by the constructor to extract features from every file in the provided folder
*@param the filepath to a given text message and integer y which determines what class this text message belongs to
*@return a vector of the final extracted features
*/
template <class T>
std::vector<T> data_extractor<T>::extractFeaturesText(std::string filepath, int y) {
	//array to store list of suspicious words
	std::vector<std::string> suspicious = {
		"best",
		"buy",
		"buying",
		"simple",
		"affordable",
		"minutes",
		"hours",
		"today",
		"this",
		"week",
		"day",
		"month",
		"sale",
		"sales",
		"saves",
		"life",
		"lowest",
		"rates",
		"purchase",
		"product",
		"service",
		"upgrade",
		"need",
		"amazing",
		"wonderful",
		"awesome",
		"money",
		"win",
		"company",
		"business",
		"opportunity",
		"opportunities",
		"corporation",
		"sell",
		"dollars",
		"dollar",
		"free",
		"pharmacy","prescription","generic","help","make","remove","order","mail","receive","business","credit","direct","conference","project","addresses","reply","taken","off","mailing","list","confident","hair","loss","product","lonely","drug","drugs","normal","medications","prescribed","internet",
		"meds","original","message","office","Viagra","Cialis","urgent","response","assistance","friend","need","transaction","rate","workplace","WORKPLACE"
		"trial","men","girl","girls","penis","growth","loan","urgent","rolex","pils","enlargment","supplement","offer","limited","inch",
		"lowest","bigger","sex","manhood","prize","claims","naked","honey","stranger","love","blond","orgasm","hot","profile" };

	std::vector<char> specialCharacters = { '$','%','#',':',',','[',']','!','(',')','?','<','|','+','^' };

	//opening file
	std::fstream readFile(filepath, std::ios::in);

	//checking for errors with file
	if (readFile.fail()) {

		std::cerr << "Error opening " << filepath << "." << std::endl;
		exit(1);

	}

	//initializing variables necessary for extracting the features
	std::string word;

	T capitalizedLetters = 0;
	T uncapitalizedLetters = 0;
	T notAlphanum = 0;
	T numWords = 0;
	T numChars = 0;
	T misspelled = 0;
	T punctuation = 0;
	T suspiciousW = 0;
	T numbers = 0;
	T funnyPunct = 0;
	T longestCapitalized = 0;
	T totalCapitalized = 0;
	T numCapStrings = 0;
	T maxCapString = 0;
	T averageCapString = 0;
	T numFCS = 0;

	std::vector<T> numSuspicious(suspicious.size(), 0);
	std::vector<T> numSpecialChar(specialCharacters.size(), 0);

	//starting reading the file from a location well into the message
	readFile.seekg(1000, std::ios::beg);

	//checking for errors with file
	if (readFile.fail()) {

		std::cerr << "Error seeking " << filepath << "." << std::endl;
		exit(1);

	}

	//reading fro the file to extract the features
	while (!readFile.eof()) {

		//getline(readFile, word);
		readFile >> word;

		//checking for errors with file
		if (readFile.fail() && !readFile.eof()) {

			std::cerr << "Error reading " << filepath << "." << std::endl;
			exit(1);

		}

		//adding the length of the word to the char count
		numChars += word.length();

		//looking for the longest string of capitalized letters in the word and if it is entirely capitalized and if previous char was a capital
		T maxCap = 0;
		bool cap = true;
		bool prev = true;

		//checking for capitalized letters, digits and punctuation
		for (int i = 0; i < word.length(); i++) {
			if (word[i] >= 0) {
				if (!isupper(word[i])) {
					cap = false;
					//prev = false;
				}
				if (islower(word[i])) {
					uncapitalizedLetters += 1;
				}
				if (isupper(word[i])) {
					capitalizedLetters += 1;
					//if (prev) {
					maxCap += 1;
					//}
				}
				if (ispunct(word[i])) {
					punctuation += 1;
				}
				if (isdigit(word[i])) {
					numbers += 1;
				}
				if (!isalnum(word[i])) {
					notAlphanum += 1;
				}
				for (int j = 0; j < specialCharacters.size(); j++) {
					if (specialCharacters[j] == word[i]) {
						numSpecialChar[j] += 1;
						funnyPunct += 1;
					}
				}
			}
		}

		if (maxCap > longestCapitalized) {
			longestCapitalized = maxCap;
		}

		if (cap) {
			if (maxCap > maxCapString) {
				maxCapString = maxCap;
			}
			averageCapString += maxCap;
			numFCS += 1;
		}
		totalCapitalized += maxCap;
		if (maxCap > 0) {
			numCapStrings += 1;
		}
		//}

		//adding to number of words
		numWords += 1;

		//removing unnesecary punctuation at the end of the word
		std::string Pword = "";
		for (int i = word.length() - 1; i >= 0; i--) {
			if (word[i] >= 0) {
				if (isalpha(word[i])) {
					for (int j = 0; j <= i; j++) {
						Pword += word[j];
					}
					i = -1;
				}
			}
		}

		//check for misspelled words
		if (check(Pword) != 1) {
			misspelled += 1;
		}

		//converting word to lowercase, removing unnecessary punctuation to check if it matches one of the suspicious words 
		std::string Lword = "";
		for (int i = 0; i < word.length(); i++) {
			if (word[i] > 0) {
				if (!ispunct(word[i])) {
					Lword += tolower(word[i]);
				}
				else {
					if (word[i] == 39) {
						Lword += word[i];
					}
				}
			}
		}

		//checking for suspicious words
		for (int i = 0; i < suspicious.size(); i++) {
			if (suspicious[i].compare(Lword) == 0) {
				numSuspicious[i] += 1;
			}
		}
	}

	//create a vector with the results and return it
	std::vector<T> results = {
		capitalizedLetters / (numChars + 1) * 100,
		misspelled / (numWords + 1) * 100,
		punctuation / (numChars + 1) * 100,
		suspiciousW / (numWords + 1) * 100,
		numbers / (numChars + 1) * 100,
		funnyPunct / (punctuation + 1) * 100,
		uncapitalizedLetters / (numChars + 1) * 100,
		notAlphanum / (numChars + 1) * 100,
		longestCapitalized,
		totalCapitalized / (numCapStrings + 1),
		maxCapString,
		averageCapString / (numFCS + 1)

	};

	//dividing number of suspicious words by the number of words
	for (int i = 0; i < numSuspicious.size(); i++) {
		numSuspicious[i] = 100 * numSuspicious[i] / (numWords + 1);
	}

	//dividing number of suspicious words by the number of words
	for (int i = 0; i < numSpecialChar.size(); i++) {
		numSpecialChar[i] = 100 * numSpecialChar[i] / (numWords + 1);
	}

	//initializing a vector to store the final results
	std::vector<T> total_results(0);

	//adding the data to this new vector
	total_results.insert(std::end(total_results), std::begin(numSuspicious), std::end(numSuspicious));
	total_results.insert(std::end(total_results), std::begin(numSpecialChar), std::end(numSpecialChar));
	total_results.insert(std::end(total_results), std::begin(results), std::end(results));

	//adding y to the end of the data collected
	total_results.push_back(y);

	//closing the file
	readFile.close();

	//checking for errors with file
	if (readFile.fail()) {

		//cerr << "Error closing " << filename << "." << endl;
		return total_results;

	}

	else {
		//returning the results
		return total_results;
	}
}

template <class T>
std::vector<matrix<T>> data_extractor<T>::normalizeFeatures() {
	matrix<T> mu = (data.sum()) / (data.getSize()[0]);
	matrix<T> X = data - mu;

	matrix<T> sigma = (((data - mu).power(2)).sum() / (data.getSize()[0])).power(1 / 2);

	X = X.dotDivide(sigma);

	//adding mu and sigma to a vector
	std::vector<matrix<T>> weights = { mu, sigma };

	return weights;
}

template <class T>
matrix<T> normalizeFeatures(matrix<T> m, std::vector<matrix<T>> weights) {
	if (weights.size() == 2) {
		matrix<T> X = m - weights[0];
		X = X.dotDivide(weights[1]);

		return X;
	}
	else {
		std::cerr << "Not the right number of weights" << std::endl;
		exit(1);
	}
}

template <class T>
matrix<T> data_extractor<T>::getData() {
	return data;
}

#endif