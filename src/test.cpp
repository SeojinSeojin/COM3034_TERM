#include <iostream>
#include <locale>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "imageLoader.cpp"

using namespace std;
using namespace cv;

int main()
{
    TrainTestData<CharImage> data = getCharImageData();

    int cnt = 0;
    for (const auto &trainCharImage : data.trainData)
    {

        cout << trainCharImage << endl;
        if (cnt < 5)
        {
            cnt++;
            imshow(trainCharImage.targetChar, trainCharImage.src);
            waitKey();
        }
        }
    return 0;
}