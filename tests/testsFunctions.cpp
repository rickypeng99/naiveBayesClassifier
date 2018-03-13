//
// Created by Ricky Peng on 3/12/18.
//

#include "../src/ImageData.h"

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

//
// Created by Ricky Peng on 3/9/18.
//
#include "../src/ImageData.h"
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

//
// Created by Ricky Peng on 3/12/18.
//

#include <iomanip>
#include <fstream>
#include "../src/ImageData.h"

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

//
// Created by Ricky Peng on 3/12/18.
//

#include <fstream>
#include <cmath>
#include <iomanip>
#include "../src/ImageData.h"



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













