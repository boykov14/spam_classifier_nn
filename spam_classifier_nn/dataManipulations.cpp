#include <stdio.h>
#include <vector>
#include <string>
#include <ctype.h>
#include <iostream>
#include <fstream>

#include "matrix.h"
#include "load_dictionary.h"

std::vector<float> extractFeaturesText(std::string filepath, int y);

matrix getData(std::string filepath1, std::string filepath2, int numSpam, int numHam) {

	loadDict("dict.txt");

	//initializing variables to store the data
	std::vector<float> results;
	std::vector<std::vector<float>> x;

	//gatherng data from spam dataset
	for (int i = 1; i <= numSpam; i++) {

		std::string path = filepath1;

		//adding the proper extentions to the filenames
		path += to_string(i);
		path += ".txt";

		//extracting data from the file
		results = extractFeaturesText(path, 1);
		if (results.size() == 0) {
			cerr << "error extracting features" << endl;
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
	matrix finalData(x);

	matrix ones((finalData.getSize())[0], 1, 1);

	finalData.addColumn(ones, 0);

	return finalData;
}
matrix extractData(std::string filename) {
	//opening file
	std::fstream readFile(filename, std::ios::in);

	//checking for errors with file
	if (readFile.fail()) {

		std::cerr << "Error opening " << filename << "." << std::endl;
		exit(1);

	}

	//string to store file input
	std::string input;
	std::vector<std::vector<float>> data;
	int length;

	while (!readFile.eof()) {

		getline(readFile, input);

		//checking for errors with file
		if (readFile.fail() && !readFile.eof()) {

			std::cerr << "Error reading " << filename << "." << std::endl;
			exit(1);

		}

		std::string::size_type pos;
		std::vector<float> vals;


		while (input.length() > 0) {
			if (isdigit(input[0]) || input[0] == '-') {
				vals.push_back(std::stof(input, &pos));
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

	matrix DATA(data);
	return data;
}

std::vector<float> extractFeaturesText(std::string filepath, int y) {
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

	float capitalizedLetters = 0;
	float uncapitalizedLetters = 0;
	float notAlphanum = 0;
	float numWords = 0;
	float numChars = 0;
	float misspelled = 0;
	float punctuation = 0;
	float suspiciousW = 0;
	float numbers = 0;
	float funnyPunct = 0;
	float longestCapitalized = 0;
	float totalCapitalized = 0;
	float numCapStrings = 0;
	float maxCapString = 0;
	float averageCapString = 0;
	float numFCS = 0;

	std::vector<float> numSuspicious(suspicious.size(), 0);
	std::vector<float> numSpecialChar(specialCharacters.size(), 0);

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
		float maxCap = 0;
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
	std::vector<float> results = {
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
	std::vector<float> total_results(0);

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

matrix mapFeatures(matrix data) {
	//calculating length of dataset
	std::vector<int> mn = data.getSize();

	//checking validity of data
	if (mn[0] <= 0) {

		std::cerr << "No Data Provided" << std::endl;
		exit(1);
	}

	//enhancing the data
	matrix output(mn[0], 1, 1);

	int highest_degree = 6;

	for (int i = 1; i <= highest_degree; i++) {
		for (int j = 0; j <= i; j++) {
			matrix to_add(mn[0], 1, 1);

			for (int k = 0; k < mn[0]; k++) {
				to_add[k][0] = pow(data[k][0], i - j) * pow(data[k][1], j);
			}
			output.addColumn(to_add);
		}
	}
	return output;
}

std::pair<matrix, std::vector<matrix>> normalizeFeatures(matrix m) {
	matrix mu = (m.sum()) / (m.getSize()[0]);
	matrix X = m - mu;

	matrix sigma = (((m - mu).power(2)).sum() / (m.getSize()[0])).power(1 / 2);

	X = X.dotDivide(sigma);

	//adding mu and sigma to a vector
	std::vector<matrix> weights = { mu, sigma };

	std::pair<matrix, std::vector<matrix>> returnVal;
	returnVal.first = X;
	returnVal.second = weights;

	return returnVal;
}

matrix normalizeFeatures(matrix m, std::vector<matrix> weights) {
	if (weights.size() == 2) {
		matrix X = m - weights[0];
		X = X.dotDivide(weights[1]);

		return X;
	}
	else {
		std::cerr << "Not the right number of weights" << std::endl;
		exit(1);
	}
}