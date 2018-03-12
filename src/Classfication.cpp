//
// Created by Ricky Peng on 3/12/18.
//

#include <fstream>
#include <cmath>
#include <iomanip>
#include "ImageData.h"

void printHighestAndLowestPost(vector<ImageData> &trainingData, vector<int> &guessingLabels, vector<double> &posteriors,
                               vector<int> &labels);

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
    vector<double> posteriors;

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
        posteriors.push_back(max);



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

    printHighestAndLowestPost(trainingData, guessingLabels, posteriors, labels);


    double classCount[10];
    for (int i : labels) {
        classCount[i]++;
    }
    produceConfusionMatrix(matrix, classCount);


    cout << "The AI has successfully recognized " << (double)correct/10.0 << "% of the testing images." << endl;

}

void printHighestAndLowestPost(vector<ImageData> &trainingData, vector<int> &guessingLabels, vector<double> &posteriors,
                               vector<int> &labels) {
    double maxPost = posteriors.at(0);
    double lowPost = 1000.0;
    vector<ImageData> highestPost;
    vector<ImageData> lowestPost;
    ImageData highest;
    ImageData lowest;

    for (int i = 0; i < CLASS_NUM; i++) {
        for (unsigned long label = 0; label < labels.size(); label++) {
            if (labels.at(label) == i && labels.at(label) == guessingLabels.at(label) && posteriors.at(label) > maxPost) {
                maxPost = posteriors.at(label);
                highest = trainingData.at(label);
            }
            if(labels.at(label) == i && labels.at(label) == guessingLabels.at(label) && posteriors.at(label) < lowPost)
            {
                lowPost = posteriors.at(label);
                lowest = trainingData.at(label);
            }
        }
        highestPost.push_back(highest);
        lowestPost.push_back(lowest);
        maxPost = -1000.0;
        lowPost = 1000.0;
    }


    ofstream saveFile("../data/highestPosterior.txt");
    for (ImageData image : highestPost) {
        for (int i = 0; i < IMAGE_SIZE; i++) {
            for (int j = 0; j < IMAGE_SIZE; j++) {
                saveFile << image.Image[i][j];
            }
            saveFile << endl;
        }
    }

    ofstream saveFile2("../data/lowestPosterior.txt");
    for (ImageData image : lowestPost) {
        for (int i = 0; i < IMAGE_SIZE; i++) {
            for (int j = 0; j < IMAGE_SIZE; j++) {
                saveFile2 << image.Image[i][j];
            }
            saveFile2 << endl;
        }
    }
}






