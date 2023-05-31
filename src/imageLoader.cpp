#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <vector>
#include <set>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "utils.cpp"
#include "allChars.cpp"

using namespace std;
using namespace cv;

class CharImage
{
public:
    string targetChar;
    int targetLabel;
    Mat src;
    CharImage(int fontIndex, int label, string targetChar, bool useBlur = true)
    {
        this->targetChar = targetChar;
        this->targetLabel = label;
        string filename = "resources/images/webFonts/" + targetChar + "/" + to_string(fontIndex) + ".jpg";
        Mat tempSrc = imread(filename, IMREAD_GRAYSCALE);
        if (tempSrc.empty())
        {
            throw runtime_error("failed to read image : " + filename);
        }
        if (useBlur)
            medianBlur(tempSrc, this->src, 3);
        else
            this->src = tempSrc;
    }
    friend std::ostream &operator<<(std::ostream &os, const CharImage &charImage)
    {
        os << "\tCharImage: " << charImage.targetLabel << ", " << charImage.targetChar;
        return os;
    };
};

template <typename T>
class TrainTestData
{
public:
    vector<T> trainData;
    vector<T> testData;
    TrainTestData(vector<T> trainData, vector<T> testData)
    {
        this->trainData = trainData;
        this->testData = testData;
    }
};

vector<CharImage> getAllCharImage(int charSize)
{
    vector<CharImage> charImages = {};

    int label = 0;
    for (string targetChar : ALL_CHARS)
    {
        for (int fontIdx = 1; fontIdx <= charSize; fontIdx++)
        {
            try
            {
                CharImage newCharImage(fontIdx, label, targetChar);
                charImages.push_back(newCharImage);
            }
            catch (exception &e)
            {
                cout << e.what() << endl;
            }
        }
        label++;
    }

    cout << "read image" << endl;
    cout << "\t# of total data : " << charImages.size() << endl;

    return charImages;
}

TrainTestData<CharImage> getCharImageData(float trainRatio = 0.9, int charSize = 30)
{
    vector<CharImage> allCharImages = getAllCharImage(charSize);
    vector<CharImage> trainData, testData;
    int testCount = charSize * (1 - trainRatio);

    for (int i = 0; i < allCharImages.size(); i++)
    {
        int testRangeStart = (i / charSize) % 3;
        int testRangeEnd = testRangeStart + testCount;
        if (i % charSize >= testRangeStart && i % charSize < testRangeEnd)
            testData.push_back(allCharImages[i]);
        else
            trainData.push_back(allCharImages[i]);
    }

    cout << "data split completed, " << endl;
    cout << "\t# of train data : " << trainData.size() << endl;
    cout << "\t# of test data : " << testData.size() << endl;

    return TrainTestData<CharImage>(trainData, testData);
}