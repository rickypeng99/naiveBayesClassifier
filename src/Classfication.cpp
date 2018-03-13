//
// Created by Ricky Peng on 3/12/18.
// This class is used to classify the images
//

#include <fstream>
#include <cmath>
#include <iomanip>
#include "ImageData.h"


/**
 *
 * @param model the probability model
 * @param arr the array of classes counts
 * This function will be classifying the test images
 */
void determineImage(Model& model, const int arr[10]) {

    vector<ImageData> trainingData;

    readingImages(trainingData, "testimages");

    vector<int> guessingLabels;
    vector<double> posteriors;

    for (auto &image : trainingData) {
        double posteriorPossibility[10];
        //adding the log(P(class) to each of the posterior possibility first
        for (int loop = 0; loop < CLASS_NUM; loop++) {
            posteriorPossibility[loop] = log10(static_cast<float>(arr[loop]));
        }

        generatingPosteriors(model, guessingLabels, posteriors, image, posteriorPossibility);


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


/**
 *
 * @param model the probability model
 * @param guessingLabels the guessing from thr AI
 * @param posteriors the maximum posterior probability array
 * @param image the ImageData object
 * @param posteriorPossibility the posterior probability array
 * This functions will be calculating the highest posterior possibility for every class,
 * the array will then be used to classify the images
 */
void generatingPosteriors(const Model &model, vector<int> &guessingLabels, vector<double> &posteriors,
                          const ImageData &image, double *posteriorPossibility) {
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







