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

    vector<string> filePaths = {"final_cnn_10.onnx", "final_cnn_20_ck_es_100.onnx", "final_cnn_20_ck_es_dp.onnx", "final_cnn_20.onnx", "init_cnn_10.onnx", "really_init_cnn_10.onnx"};

    for (const auto &filePath : filePaths)
    {
        string fullFilePath = "resources/networks/" + filePath;
        dnn::Net net = dnn::readNetFromONNX(fullFilePath);
        cout << filePath << " start" << endl;

        int answerCount = 0;
        int totalCount = 0;
        auto start_time3 = chrono::high_resolution_clock::now();
        for (const auto &testCharImage : data.testData)
        {
            totalCount++;
            Mat inputBlob = dnn::blobFromImage(testCharImage.src, 1.0, Size(), Scalar(), true, false);
            net.setInput(inputBlob);
            Mat output = net.forward();

            Mat scores = output.reshape(1, 1);

            Point classIdPoint;
            double confidence;
            minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);

            int predictLabel = classIdPoint.x;
            if (predictLabel == testCharImage.targetLabel)
                answerCount++;
            // else
            // {
            //     cout << "predict : " << ALL_CHARS[predictLabel] << " | answer : " << ALL_CHARS[testCharImage.targetLabel] << endl;
            // }
        }
        auto end_time3 = chrono::high_resolution_clock::now();
        auto duration3 = chrono::duration_cast<chrono::milliseconds>(end_time3 - start_time3);
        cout << "Elapsed time(Test): " << duration3.count() << " milliseconds" << endl;

        cout << "test completed" << endl;
        cout << "\t# of correct answer : " << answerCount << endl;
        cout << "\t# of total test set : " << totalCount << endl;

        double accuracy = static_cast<double>(answerCount) / totalCount;
        cout << "\taccuracy : " << accuracy << endl;
    }

    return 0;
}
