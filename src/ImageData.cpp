//
// Created by Ricky Peng on 3/9/18.
//
#include "ImageData.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

//loop that reads the string from lines
//read the characters stick them into the arrays
bool Model::saveToFile() {

    ofstream saveFile("../data/model.txt");
    if (!saveFile) {
        return false;
    }

    for (int num = 0; num < CLASS_NUM; num++) {
        for (int width = 0; width < IMAGE_SIZE; width++) {
            for (int height = 0; height < IMAGE_SIZE; height++) {
                for (int binary = 0; binary < 2; binary++) {
                    saveFile << setw(12) << probabilities[width][height][num][binary] << setw(12);
                }
            }
            saveFile << endl;
        }
        saveFile << endl;
    }
    saveFile.close();
    return true;
}

bool Model::loadFromFile() {
    double data = 0.0;
    int i = 0;
    int j = 0;
    int classNum = 0;
    int binary = 0;

    string fileName;
    cin >> fileName;

    ifstream inFile("../data/" + fileName);

    if (!inFile.is_open()) {
        cout << "The file is not existed! Please input the correct name" <<endl;
        exit(0);

    }

    while(inFile >> data) {
        probabilities[i][j][classNum][binary] = data;
        binary++;
        if (binary > 1) {
            j++;
            binary = 0;
        }
        if (j > 27) {
            i++;
            j = 0;
        }
        if (i > 27) {
            classNum++;
            i=0;
            j=0;
        }
    }



    cout << "The model.txt is successfully imported!" << endl;
    cout << "---------------------------------------" << endl;

    return true;
}





bool transferBool(const char &index) {
    return index == '#' || index == '+';
}

void printImage(bool (&array)[IMAGE_SIZE][IMAGE_SIZE] ) {
    int height = IMAGE_SIZE;
    int width = IMAGE_SIZE;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << array[i][j] << ' ';
        }
        cout << endl;
    }
}

vector<int> readLabelFromFile(const string &fileName) {
    ifstream inFile;
    inFile.open("../data/" + fileName);
    vector<int> labels;
    int nextInt;
    while (inFile >> nextInt) {
        labels.push_back(nextInt);

    }
    return labels;
}





int main() {
    ifstream in("../data/model.txt");
    if (in.peek() == std::ifstream::traits_type::eof()) {

        ifstream inFile;
        inFile.open("../data/trainingimages");

        string line;
        int count = 0;


        ImageData imageData{};
        vector<ImageData> trainingData;
        while (getline(inFile, line)) {
            for (unsigned long i = 0; i < IMAGE_SIZE; i++) {
                imageData.Image[count][i] = transferBool(line.at(i));
            }

            count++;

            if (count == IMAGE_SIZE) {
                //create a temporary imageData to save current value
                ImageData temp = imageData;
                //put the temp into the vector
                trainingData.push_back(temp);
                //clear the memory of image array
                memset(imageData.Image, 0, sizeof(imageData.Image));

            }
        }

        vector<int> result = readLabelFromFile("traininglabels");
        int classFrequencyArray[10];
        countClassFrequency(classFrequencyArray, result);


        // TRAINING PHASE
        cout << "Training the AI, using the K value of " << K << " for laplace smoothing" << endl;
        Model model{};
        //count the amount of C = class at F = Fij
        addStatisticToProbability(model, result, trainingData);
        //transfer the counting to possibilities
        makeProbability(model, classFrequencyArray);
        //Getting P(class)
        calculateProbabilityOfClass(model, classFrequencyArray);

        cout << "Finished training your AI with 5000 images" << endl;
        cout << "The model has been saved into model.txt" << endl;

        model.saveToFile();
        //CLASSIFICATION PHASE
        determineImage(model, classFrequencyArray);
    } else {

        Model model;
        model.loadFromFile();
        vector<int> result = readLabelFromFile("traininglabels");
        int classFrequencyArray[10];
        countClassFrequency(classFrequencyArray, result);
        //CLASSIFICATION PHASE
        determineImage(model, classFrequencyArray);
    }
}
