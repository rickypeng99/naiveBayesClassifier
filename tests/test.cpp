//
// Created by Ricky Peng on 3/12/18.
//


#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "../src/ImageData.h"
#include <iostream>



TEST_CASE( "Testing boolean transfer", "[testTransBool]" ) {
    REQUIRE(transferBool('#'));
    REQUIRE(transferBool('+'));
    REQUIRE(!transferBool(' '));

}

TEST_CASE( "Testing read labels from files", "[testReadLabels]" ) {
    REQUIRE(readLabelFromFile("traininglabels").at(0) == 5);
}

TEST_CASE("Testing read images from files", "[testReadImages]") {
    vector<ImageData> trainingData;
    readingImages(trainingData, "testimages");
    REQUIRE(!trainingData.at(0).Image[0][0]);
}

TEST_CASE("Testing loading from files", "[testLoadFiles]") {
    Model model;
    model.loadFromFileFrom("model.txt");
    REQUIRE(model.probabilities[0][0][0][0] == 0.997921);
}

TEST_CASE("Testing saving to files", "[testSaveFiles]") {
    Model model;
    model.loadFromFileFrom("model.txt");
    model.saveToFileTo("testSave.txt");
    ifstream inFile("../data/testSave.txt");
    REQUIRE(inFile.is_open());
    double firstValue;
    inFile >> firstValue;
    REQUIRE(firstValue == 0.997921);
}







