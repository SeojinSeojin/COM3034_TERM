#include <opencv2/opencv.hpp>
#include <iostream>
#include <locale>
#include <chrono>
#include "imageLoader.cpp"

using namespace cv;
using namespace std;

int main()
{
    auto start_time1 = chrono::high_resolution_clock::now();
    TrainTestData<CharImage> data = getCharImageData();
    auto end_time1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end_time1 - start_time1);
    cout << "Elapsed time(Data Load): " << duration1.count() << " milliseconds" << endl;

    Size winSize(38, 42);
    Size blockSize(19, 21);
    Size blockStride(1, 3);
    Size cellSize(19, 21);
    int nbins = 9;

    vector<Mat> trainImages;
    vector<int> trainLabels;
    for (const auto &trainCharImage : data.trainData)
    {
        trainImages.push_back(trainCharImage.src);
        trainLabels.push_back(trainCharImage.targetLabel);
    }

    vector<vector<float>> trainDescriptors;
    HOGDescriptor hog(winSize, blockSize, blockStride, cellSize, nbins);
    for (const auto &img : trainImages)
    {
        vector<float> descriptors;
        hog.compute(img, descriptors);
        trainDescriptors.push_back(descriptors);
    }

    Mat trainData(trainDescriptors.size(), trainDescriptors[0].size(), CV_32FC1);
    Mat trainLabelsMat(trainLabels.size(), 1, CV_32SC1);
    for (int i = 0; i < trainDescriptors.size(); ++i)
    {
        for (int j = 0; j < trainDescriptors[i].size(); ++j)
        {
            trainData.at<float>(i, j) = trainDescriptors[i][j];
        }
        trainLabelsMat.at<int>(i, 0) = trainLabels[i];
    }

    Ptr<ml::KNearest> knn = ml::KNearest::create();
    auto start_time2 = chrono::high_resolution_clock::now();
    knn->train(trainData, ml::ROW_SAMPLE, trainLabelsMat);
    auto end_time2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end_time2 - start_time2);
    cout << "Elapsed time(Train): " << duration2.count() << " milliseconds" << endl;

    int answerCount = 0;
    int totalCount = 0;
    auto start_time3 = chrono::high_resolution_clock::now();
    for (const auto &testCharImage : data.testData)
    {
        totalCount++;
        vector<float> testDescriptors;
        hog.compute(testCharImage.src, testDescriptors);

        Mat testData(1, testDescriptors.size(), CV_32FC1);
        for (int i = 0; i < testDescriptors.size(); ++i)
        {
            testData.at<float>(0, i) = testDescriptors[i];
        }

        Mat res;
        knn->findNearest(testData, 3, res);
        int predictLabel = res.at<float>(0, 0);
        if (predictLabel == testCharImage.targetLabel)
            answerCount++;
        if (totalCount % 100 == 0)
        {
            cout << "\t" << totalCount << " accuracy : " << static_cast<double>(answerCount) / totalCount << endl;
        }
    }
    auto end_time3 = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::milliseconds>(end_time3 - start_time3);
    cout << "Elapsed time(Test): " << duration3.count() << " milliseconds" << endl;

    cout << "test completed" << endl;
    cout << "\t# of correct answer : " << answerCount << endl;
    cout << "\t# of total test set : " << totalCount << endl;

    double accuracy = static_cast<double>(answerCount) / totalCount;
    cout << "\taccuracy : " << accuracy << endl;

    return 0;
}
