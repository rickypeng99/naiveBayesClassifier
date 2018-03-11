//
// Created by Ricky Peng on 3/9/18.
//

#ifndef NAIVEBAYES_IMAGEDATA_H
#define NAIVEBAYES_IMAGEDATA_H


//To produce binary features from this data we will treat
// pixel i,j as a feature and say that FI,j has a value of 0
// if it is a background pixel or white and a value of 1
// if it is a foreground pixel and not white.

//feature is a pixel

#include <iostream>
#include <vector>

using namespace std;

const int IMAGE_SIZE = 28;
const int CLASS_NUM = 10;
const int BINARY = 2;
//laplace smoothing
const int K = 1;


class ImageData {
public:
    bool Image[28][28];
};


class Model{
public:
    bool saveToFile();
    bool loadFromFile();
double probabilities[28][28][10][2];//value of features, 1 or 0
double prob_of_class[10];
};
#endif //NAIVEBAYES_IMAGEDATA_H
