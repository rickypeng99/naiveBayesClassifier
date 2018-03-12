//
// Created by Ricky Peng on 3/9/18.
//

#include "ImageData.h"

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


    for (unsigned long pos = 0; pos < trainingData.size(); pos++) {
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

void makeProbability(Model& model, const int arr[10]) {


    for (int width = 0; width < IMAGE_SIZE; width++) {
        for (int height = 0; height < IMAGE_SIZE; height++) {
            for (int num = 0; num < CLASS_NUM; num++) {
                for (int binary = 0; binary < BINARY; binary++) {
                    double temp = model.probabilities[width][height][num][binary];
                    model.probabilities[width][height][num][binary] =
                            (K + temp) / (double)(K + K + arr[num]);
                }
            }
        }
    }

    //
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

void calculateProbabilityOfClass(Model& model, const int arr[10]) {
    for (int i = 0 ; i < 10; i ++) {
        model.prob_of_class[i] = arr[i] / 5000.0;
    }

}
