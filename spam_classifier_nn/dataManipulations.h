#ifndef DATAMANIPULATIONS_H
#define DATAMANIPULATIONS_H

#include <string>
#include "matrix.h"

matrix getData(std::string filepath1, std::string filepath2, int numSpam, int numHam);
std::vector<float> extractFeaturesText(std::string filepath, int y);
matrix extractData(std::string filename);
matrix mapFeatures(matrix data);
std::pair<matrix, std::vector<matrix>> normalizeFeatures(matrix m);
matrix normalizeFeatures(matrix m, std::vector<matrix> weights);

#endif
