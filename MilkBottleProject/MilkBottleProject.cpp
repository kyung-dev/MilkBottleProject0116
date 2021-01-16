#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;


Mat src_gray;	//회색영상으로 만든 영상 저장하는 변수
Mat detected_edges;	//윤곽선으로 만든 영상 저장하는 변수

int lowThreshold = 36;	//낮은 임계값
int highThreshold = 0;	//높은 임계값
int maxX = 0, minX = 0, maxY = 0, minY = 0;	//윤곽선에 해당하는 픽셀 중 가장 밑, 가장 위에 있는 픽셀좌표를 저장 


int main(int argc, char** argv) {

	Mat src = imread("bottle.png", IMREAD_COLOR); // 프로젝트 우클릭, '파일탐색기에서 폴더열기' 클릭해서 나오는 위치에 이미지 저장
	Mat src2 = imread("bottle.png", IMREAD_COLOR);	// 


	if (src.empty())	// 이미지파일을 해당 위치에 놔두지 않았을 때
	{
		std::cout << "Could not open or find the image!\n" << std::endl;
		std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
		return -1;
	}

	cvtColor(src, src_gray, COLOR_BGR2GRAY);	// 영상을 회색으로 변경
	blur(src_gray, detected_edges, Size(3, 3));	// 윤곽선 얻기전 전처리 과정
	Canny(detected_edges, detected_edges, lowThreshold, highThreshold, 3);	//임계값을 기반으로 윤곽선을 따서 detected_edges변수에 저장

	maxY = 0;
	minY = detected_edges.rows;

	for (int y = 0; y < detected_edges.rows; y++) {			// 윤곽선중 가장 위에 있는 픽셀을 찾기 위해 모든 픽셀을 검사하고 
		for (int x = 0; x < detected_edges.cols; x++) {		// 윤곽선에 해당하는 픽셀의 좌표가 maxY변수보다 크면
			if (detected_edges.at<uchar>(y, x) == 255) {	// 그 좌표를 변수에 초기화 해줌

				src2.at<Vec3b>(y, x)[0] = 255; // 해당 픽셀을 파란색으로 변경 
				src2.at<Vec3b>(y, x)[1] = 0; // BGR 순서 
				src2.at<Vec3b>(y, x)[2] = 0;

				if (y >= maxY) {
					maxY = y;
					maxX = x;
				}
				if (y <= minY) {
					minY = y;
					minX = x;

				}
			}
		}

	}

	//제일 윗점이 잘 찾아졌나 src2영상에 표시해서 확인해봄
	src2.at<Vec3b>(minY, minX)[0] = 0; // 해당 픽셀을 빨간색으로 변경 
	src2.at<Vec3b>(minY, minX)[1] = 0; // BGR 순서 
	src2.at<Vec3b>(minY, minX)[2] = 255;

	src2.at<Vec3b>(maxY, maxX)[0] = 0; // 해당 픽셀을 팔간색으로 변경 
	src2.at<Vec3b>(maxY, maxX)[1] = 0; // BGR 순서 
	src2.at<Vec3b>(maxY, maxX)[2] = 255;

	std::cout << "min x : " << minX << ", min y : " << minY << ", max x : " << maxX << "max y :" << maxY;	//변수값 출력

	namedWindow("Canny Edge", WINDOW_AUTOSIZE);	// 출력창 생성
	imshow("Canny Edge", detected_edges);	// 윤곽선처리한 영상 출력
	imwrite("contour.png", detected_edges);	// 윤곽선처리한 영상 contour이름으로 저장

	imshow("src2", src2);	// 원본에 가장 윗 픽셀, 밑 픽셀 찾은 거 표시한 영상 출력





	waitKey(0);
	return 0;

}