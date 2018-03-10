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

class ImageData {
public:
    bool Image[28][28];
};

vector<ImageData> trainingData; //use to to store all the images
vector<int> labels;

class Model{

    void saveToFile();
    void loadFromFile();
double probabilities[28][28][10][2];//value of features, 1 or 0
//[10] represents 10 numbers
//i, j ,class, value
//given this class, the value of position ij has the probability is this value.

//go through the training data, for every Fij, i add one corresponding to what label equals to.
double prob_of_class[10];
};
#endif //NAIVEBAYES_IMAGEDATA_H
