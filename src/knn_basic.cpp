#include <iostream>
#include <locale>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <chrono>
#include "imageLoader.cpp"

using namespace std;
using namespace cv;

int main()
{
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    auto start_time1 = chrono::high_resolution_clock::now();
    TrainTestData<CharImage> data = getCharImageData();
    auto end_time1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end_time1 - start_time1);
    cout << "Elapsed time(Data Load): " << duration1.count() << " milliseconds" << endl;

    Mat trainImages, trainLabels;

    int cnt = 0;
    for (const auto &trainCharImage : data.trainData)
    {
        Mat flattenRoi;
        trainCharImage.src.convertTo(flattenRoi, CV_32F);
        flattenRoi = flattenRoi.reshape(1, 1);

        trainImages.push_back(flattenRoi);
        trainLabels.push_back(trainCharImage.targetLabel);
    }
    auto start_time2 = chrono::high_resolution_clock::now();
    knn->train(trainImages, ml::ROW_SAMPLE, trainLabels);
    auto end_time2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end_time2 - start_time2);
    cout << "Elapsed time(Train): " << duration2.count() << " milliseconds" << endl;

    int answerCount = 0;
    int totalCount = 0;
    auto start_time3 = chrono::high_resolution_clock::now();
    for (const auto &testCharImage : data.testData)
    {
        totalCount++;
        Mat flattenRoi;
        testCharImage.src.convertTo(flattenRoi, CV_32F);
        flattenRoi = flattenRoi.reshape(1, 1);

        Mat res;
        knn->findNearest(flattenRoi, 4, res);
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