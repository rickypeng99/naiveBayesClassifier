//
// Created by Ricky Peng on 3/12/18.
//

#include <fstream>
#include <cmath>
#include <iomanip>
#include "ImageData.h"
void determineImage(Model& model, const int arr[10]) {

    ifstream inFile;
    inFile.open("../data/testimages");

    string line;
    int count = 0;


    ImageData imageData = ImageData();
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

    vector<int> guessingLabels;

    for (auto &image : trainingData) {
        double posteriorPossibility[10];
        //adding the log(P(class) to each of the posterior possibility first
        for (int loop = 0; loop < CLASS_NUM; loop++) {
            posteriorPossibility[loop] = log10(static_cast<float>(arr[loop]));
        }

        double temp;
        for (int i = 0; i < IMAGE_SIZE; i++) {
            for (int j = 0; j < IMAGE_SIZE; j++) {
                if (image.Image[i][j] == 0) {
                    for (int classNum = 0; classNum < CLASS_NUM; classNum++) {
                        temp = log10(model.probabilities[i][j][classNum][0]);
                        posteriorPossibility[classNum] += temp;
                    }
                } else {
                    for (int classNum = 0; classNum < CLASS_NUM; classNum++) {
                        temp = log10(model.probabilities[i][j][classNum][1]);
                        posteriorPossibility[classNum] += temp;
                    }
                }
            }
        }
        double max = posteriorPossibility[0];
        double maxPos = 0;
        for (int i = 0; i < CLASS_NUM; i++) {
            if (posteriorPossibility[i] > max) {
                max = posteriorPossibility[i];
                maxPos = i;
            }
        }

        guessingLabels.push_back(static_cast<int &&>(maxPos));



    }
    vector<int> labels = readLabelFromFile("testlabels");

    double matrix[10][10]; //confusion matrix
    for (int i =0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = 0;
        }
    }

    int correct = 0;
    for (unsigned long i = 0; i < labels.size(); i++) {
        if (labels.at(i) == guessingLabels.at(i)) {
            matrix[labels.at(i)][guessingLabels.at(i)]++;
            correct++;
        } else {
            matrix[labels.at(i)][guessingLabels.at(i)]++;
        }
    }

    double classCount[10];
    for (int i : labels) {
        classCount[i]++;
    }
    produceConfusionMatrix(matrix, classCount);


    cout << "The AI has successfully recognized " << (double)correct/10.0 << "% of the testing images." << endl;

}

void produceConfusionMatrix(double matrix[10][10], const double count[10]) {

    cout << "The confusion matrix is: " << endl;
    for (int i =0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            double val = matrix[i][j];
            matrix[i][j] = val / count[i];
        }
    }

    for (int i =0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << setw(12) << matrix[i][j] << setw(12);
        }
        cout << endl;
    }

    cout << "---------------------------------------" << endl;

}

