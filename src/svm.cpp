#include <iostream>
#include <locale>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "imageLoader.cpp"
#include <opencv2/ml.hpp>
#include "opencv2/opencv.hpp"
#include <chrono>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
    // HOGDescriptor(글씨영상하나크기, 블록크기, 블록 이동 크기, 셀 크기)   // 블록 = 2x2 셀
    HOGDescriptor hog(Size(38, 42), Size(38, 42), Size(19, 21), Size(19, 21), 9);

    Ptr<SVM> svm = SVM::create();
    auto start_time1 = std::chrono::high_resolution_clock::now();
    TrainTestData<CharImage> data = getCharImageData();
    auto end_time1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time1 - start_time1);
    std::cout << "Elapsed time(Load Data): " << duration1.count() << " milliseconds" << endl
              << endl;

    Mat trainHOG, trainLabels;
    for (const auto &trainCharImage : data.trainData)
    {
        vector<float> desc;
        hog.compute(trainCharImage.src, desc);

        Mat desc_mat(desc);
        desc_mat.convertTo(desc_mat, CV_32F);

        Mat transpose_desc = desc_mat.t();

        trainHOG.push_back(transpose_desc.reshape(1, 1));
        trainLabels.push_back(trainCharImage.targetLabel);
    }
    svm->setType(SVM::Types::C_SVC);
    svm->setKernel(SVM::KernelTypes::RBF);
    svm->setC(312.5);
    svm->setGamma(0.50625);

    auto start_time2 = std::chrono::high_resolution_clock::now();
    svm->train(trainHOG, ml::ROW_SAMPLE, trainLabels);
    auto end_time2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time2 - start_time2);
    std::cout << "Elapsed time(Train): " << duration2.count() << " milliseconds" << endl;

    cout << "train completed" << endl;
    cout << "getC: " << svm->getC() << endl;
    cout << "getGamma: " << svm->getGamma() << endl;

    auto start_time3 = std::chrono::high_resolution_clock::now();
    int answerCount = 0;
    int totalCount = 0;
    int t = 0;
    for (const auto &testCharImage : data.testData)
    {
        totalCount++;

        vector<float> desc;
        hog.compute(testCharImage.src, desc);

        Mat desc_mat(desc);
        desc_mat.convertTo(desc_mat, CV_32F);

        Mat transpose_desc = desc_mat.reshape(1, 1);

        int predictLabel = svm->predict(transpose_desc);
        if (predictLabel == testCharImage.targetLabel)
            answerCount++;
        else if (t++ < 5)
            cout << t << " : " << testCharImage.targetLabel << " " << predictLabel << endl;
    }
    auto end_time3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time3 - start_time3);
    std::cout << "Elapsed time(Test): " << duration3.count() << " milliseconds" << std::endl;

    cout << "test completed" << endl;
    cout << "\t# of correct answer : " << answerCount << endl;
    cout << "\t# of total test set : " << totalCount << endl;

    double accuracy = static_cast<double>(answerCount) / totalCount;
    cout << "\taccuracy : " << accuracy << endl;

    return 0;
}