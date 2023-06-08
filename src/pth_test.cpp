#include <opencv2/opencv.hpp>
#include <iostream>
#include <locale>
#include <chrono>
#include "imageLoader.cpp"

using namespace cv;
using namespace std;

int main()
{
    dnn::Net net = dnn::readNetFromONNX("resources/networks/final_torch_cnn_model.onnx");

    auto start_time1 = chrono::high_resolution_clock::now();
    TrainTestData<CharImage> data = getCharImageData();
    auto end_time1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end_time1 - start_time1);
    cout << "Elapsed time(Data Load): " << duration1.count() << " milliseconds" << endl;

    int answerCount = 0;
    int totalCount = 0;
    auto start_time3 = chrono::high_resolution_clock::now();
    for (const auto &testCharImage : data.testData)
    {
        totalCount++;
        Mat resizedTestCharImage;
        resize(testCharImage.src, resizedTestCharImage, Size(28, 28));

        Mat inputBlob = dnn::blobFromImage(resizedTestCharImage, 1.0, Size(), Scalar(), true, false);
        net.setInput(inputBlob);
        Mat output = net.forward();

        Mat scores = output.reshape(1, 1);

        Point classIdPoint;
        double confidence;
        minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);

        int predictLabel = classIdPoint.x;
        if (predictLabel == testCharImage.targetLabel)
            answerCount++;
        else
        {
            cout << "predict : " << ALL_CHARS[predictLabel] << " | answer : " << ALL_CHARS[testCharImage.targetLabel] << endl;
        }
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
