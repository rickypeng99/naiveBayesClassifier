//
// Created by Ricky Peng on 3/9/18.
//
#include "ImageData.h"
#include <iostream>
#include <fstream>

using namespace std;

//loop that reads the string from lines
//read the characters stick them into the arrays
void Model::saveToFile() {

}

void Model::loadFromFile() {

}

bool transferBool(const char &index) {
    return index == '#' || index == '+';
}

int main() {
    ImageData imageData{};

    ifstream inFile;
    inFile.open("../data/trainingimages");

    string line;
    int count = 0;
    while (getline(inFile, line) && count < 28) {

        for (int i = 0; i < 28; i++) {
            imageData.Image[count][i] = transferBool(line.at(i));
        }

        count++;
    }


    int height = 28;
    int width = 28;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << imageData.Image[i][j] << ' ';
        }
        std::cout << std::endl;
    }

}





