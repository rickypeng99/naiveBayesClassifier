//
// Created by Ricky Peng on 3/9/18.
//
#include "ImageData.h"
#include <fstream>
#include <cmath>
#include <iostream>

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
                    saveFile<< probabilities[width][height][num][binary] << " ";
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

    //ifstream inFile("../data/model.txt");

    while(cin >> data) {
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

    return false;
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

void countClassFrequency (int arr[10], vector<int> labels) {
    for (int i : labels) {
        arr[i]++;
    }
}




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

    int correct = 0;
    for (unsigned long i = 0; i < labels.size(); i++) {
        if (labels.at(i) == guessingLabels.at(i)) {
            correct++;
        }
    }


    cout << "The AI has successfully recognized " << (double)correct/1000.0*100.0 << "% of the testing images." << endl;

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
