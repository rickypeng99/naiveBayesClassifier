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

vector<int> readLabelFromFile() {
    ifstream inFile;
    inFile.open("../data/traininglabels");
    vector<int> labels;
    int nextInt;
    while (inFile >> nextInt) {
        labels.push_back(nextInt);

    }
    return labels;
}

void countClassFrequency (int arr[10], vector<int> labels) {
    for (int i : labels) {
        arr[i]++;
    }
}


void addStatisticToProbability(Model& model, const vector<int> &labels, const vector<ImageData> &trainingData) {
    auto sizeOfLabels = static_cast<int>(labels.size());
    auto sizeOfTraining = static_cast<int>(trainingData.size());
    if (sizeOfLabels != sizeOfTraining) {
        cout << "The label file doesn't match the image file, I CAN'T BE TRAINED";
        exit(0);
    }

    //Initialize the array (preventing a strange array bug)
    for (int num = 0; num < CLASS_NUM; num++) {
        for (int width = 0; width < IMAGE_SIZE; width++) {
            for (int height = 0; height < IMAGE_SIZE; height++) {
                for (int binary = 0; binary < 2; binary++) {
                     model.probabilities[width][height][num][binary] = 0;
                }
            }
        }
    }


    for (int pos = 0; pos < trainingData.size(); pos++) {
        ImageData currentImage = trainingData.at((pos));
        for (int i = 0; i < IMAGE_SIZE; i++) {
            for (int j = 0; j < IMAGE_SIZE; j++) {
                bool currentBoolean = currentImage.Image[i][j];
                model.probabilities[i][j][labels[pos]][currentBoolean]++;

            }
        }
    }

    /*for (int num = 0; num < CLASS_NUM; num++) {
        for (int width = 0; width < IMAGE_SIZE; width++) {
            for (int height = 0; height < IMAGE_SIZE; height++) {
                for (int binary = 0; binary < 2; binary++) {
                    cout << model.probabilities[width][height][num][binary] << " ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }*/




}

void makeProbability(Model& model, int arr[10]) {


    for (int width = 0; width < IMAGE_SIZE; width++) {
        for (int height = 0; height < IMAGE_SIZE; height++) {
            for (int num = 0; num < CLASS_NUM; num++) {
                for (int binary = 0; binary < BINARY; binary++) {
                    double temp = model.probabilities[width][height][num][binary];
                    model.probabilities[width][height][num][binary] =
                            (K + temp) / (K + K + arr[num]);
                }
            }
        }
    }

    for (int num = 0; num < CLASS_NUM; num++) {
        for (int width = 0; width < IMAGE_SIZE; width++) {
            for (int height = 0; height < IMAGE_SIZE; height++) {
                for (int binary = 0; binary < 2; binary++) {
                    cout << model.probabilities[width][height][num][binary] << " ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

}



int main() {

    ifstream inFile;
    inFile.open("../data/trainingimages");

    string line;
    int count = 0;


    ImageData imageData;
    vector<ImageData> trainingData;
    while (getline(inFile, line)) {
        for (int i = 0; i < IMAGE_SIZE; i++) {
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
    vector<int> result = readLabelFromFile();
    int classFrequencyArray [10];
    countClassFrequency(classFrequencyArray, result);


    Model model;
    addStatisticToProbability(model, result, trainingData);
    makeProbability(model, classFrequencyArray);






}










/**
 * printing the first and second image from the trainingData vector
 */
/*printImage(trainingData.at(0).Image);
    cout << endl;
    printImage(trainingData.at(1).Image);*/


/*for (int i : result) {
        cout << i << endl;
    }

    cout << trainingData.size() << endl;
    cout << endl;
    cout << result.size() << endl;*/

/*for (int i : classFrequencyArray) {
        cout << sizeof(classFrequencyArray)/sizeof(classFrequencyArray[0]) << endl;
    }*/
