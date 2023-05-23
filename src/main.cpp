#include <iostream>
#include <locale>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "imageLoader.cpp"
#include <opencv2/ml.hpp>

using namespace std;
using namespace cv;

int main()
{
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    TrainTestData<CharImage> data = getCharImageData();

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
    knn->train(trainImages, ml::ROW_SAMPLE, trainLabels);

    cout << "train completed" << endl;

    int answerCount = 0;
    int totalCount = 0;
    int t = 0;
    for (const auto &testCharImage : data.testData)
    {
        totalCount++;
        Mat flattenRoi;
        testCharImage.src.convertTo(flattenRoi, CV_32F);
        flattenRoi = flattenRoi.reshape(1, 1);

        Mat res;
        knn->findNearest(flattenRoi, 3, res);
        int predictLabel = res.at<float>(0, 0);
        if (predictLabel == testCharImage.targetLabel)
            answerCount++;
        else if (t++ < 5)
        {
            imshow(testCharImage.targetChar, testCharImage.src);
            cout << t << " : " << testCharImage.targetLabel << " " << predictLabel << endl;
            waitKey();
        }
    }

    cout << "test completed" << endl;
    cout << "\t# of correct answer : " << answerCount << endl;
    cout << "\t# of total test set : " << totalCount << endl;

    double accuracy = static_cast<double>(answerCount) / totalCount;
    cout << "\taccuracy : " << accuracy << endl;

    return 0;
}