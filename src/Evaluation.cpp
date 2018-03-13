//
// Created by Ricky Peng on 3/12/18.
// This class provides evaluations to the results
//

#include <iomanip>
#include <fstream>
#include "ImageData.h"


/**
 *
 * @param matrix the confusion matrix (only counts when passed in)
 * @param count the count of classes array
 */
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

/**
 *
 * @param trainingData the images
 * @param guessingLabels the guessing from AI
 * @param posteriors the maximum posterior array
 * @param labels the actual labels of the images
 * This function will produce the most and least typical images of classes
 *
 */
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
        saveFile << endl;
    }

    ofstream saveFile2("../data/lowestPosterior.txt");
    for (ImageData image : lowestPost) {
        for (int i = 0; i < IMAGE_SIZE; i++) {
            for (int j = 0; j < IMAGE_SIZE; j++) {
                saveFile2 << image.Image[i][j];
            }
            saveFile2 << endl;
        }
        saveFile2 << endl;

    }
}

