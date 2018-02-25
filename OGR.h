#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static unsigned int rows;
static unsigned int cols;

Mat RgbToBinary(string img_name)
{
	/*function takes as argment initial RGB image and returnes the same but binarised image*/
	Mat graf_img = imread(img_name);
	//namedWindow("graff", WINDOW_AUTOSIZE);
	//imshow("graff", graf_img);
	Mat graf_gray;
	cvtColor(graf_img, graf_gray, CV_RGB2GRAY);
	//namedWindow("grey_graff", WINDOW_AUTOSIZE);
	//imshow("grey_graff", graf_gray);
	Mat graf_binary;
	threshold(graf_gray, graf_binary, 200, 255, THRESH_BINARY_INV);
	return graf_binary;
}

vector<pair<unsigned, unsigned>> GetGrafPixels(Mat graf_binary)
{
	/*function gets graff bynary image and fined all pixels that blong to 
	graff(all white colored pixels)*/
	int i;
	int j;

	rows = graf_binary.rows;
	cols = graf_binary.cols;
	vector<pair<unsigned, unsigned>> graf_pixels;

	for (i = 0; i < rows; ++i)
		for (j = 0; j < cols; ++j)
			if ((unsigned)(graf_binary.at<uchar>(i, j)) == 255)
				graf_pixels.push_back(make_pair(i, j));
	return graf_pixels;

}

void GetVertexesWithMyAlg(vector<pair<unsigned, unsigned>> graf_pixels, Mat graf_binary)
{
	/*function takes graf pixel cordinates and graff binary imege and finds graffvertexes using erosion
	algorithm(cycleic deleted graff boundary pixels by setting their color to background color)*/
	int i;
	int j;
	int deleted_pixels;
	int deleted_pixels_prev;
	Mat graf_vertexes;
	//use copyTo for deep copy
	graf_binary.copyTo(graf_vertexes);
	for (j = 0; j < 3; ++j)
	{
		if (j > 0 )
			deleted_pixels_prev = deleted_pixels;
		deleted_pixels = 0;
		for (i = 0; i < graf_pixels.size(); ++i)
			//clearing border pixels
			if (graf_pixels[i].first == 0 || graf_pixels[i].first == rows - 1 || graf_pixels[i].second == 0 || graf_pixels[i].second == cols - 1)
			{
				graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
				++deleted_pixels;
				continue;
			}
			//checking (i - 1, j - 1) pixel
			else if (graf_pixels[i].first > 0 && graf_pixels[i].second > 0)
				if (graf_binary.at<uchar>(graf_pixels[i].first - 1, graf_pixels[i].second - 1) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i - 1, j) pixel
			else if (graf_pixels[i].first > 0)
				if ((graf_binary.at<uchar>(graf_pixels[i].first - 1, graf_pixels[i].second)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i - 1, j + 1) pixel
			else if (graf_pixels[i].first > 0 && graf_pixels[i].second + 1 < cols)
				if ((graf_binary.at<uchar>(graf_pixels[i].first - 1, graf_pixels[i].second + 1)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i, j + 1) pixel
			else if (graf_pixels[i].second + 1 < cols)
				if ((graf_binary.at<uchar>(graf_pixels[i].first, graf_pixels[i].second + 1)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i + 1, j + 1) pixel
			else if ((graf_pixels[i].first + 1) < rows && (graf_pixels[i].second + 1) < cols)
				if ((graf_binary.at<uchar>(graf_pixels[i].first + 1, graf_pixels[i].second + 1)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i + 1, j) pixel
			else if ((graf_pixels[i].first + 1) < rows)
				if ((graf_binary.at<uchar>(graf_pixels[i].first + 1, graf_pixels[i].second)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i + 1, j - 1) pixel
			else if ((graf_pixels[i].first + 1) < rows && graf_pixels[i].second > 0)
				if ((graf_binary.at<uchar>(graf_pixels[i].first + 1, graf_pixels[i].second - 1)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
			//checking (i, j - 1) pixel
			else if (graf_pixels[i].second > 0)
				if ((graf_binary.at<uchar>(graf_pixels[i].first, graf_pixels[i].second - 1)) == 0)
				{
					graf_vertexes.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 0;
					++deleted_pixels;
					continue;
				}
		//update graf_binary after every cycle with one pixel thinner graff
		graf_vertexes.copyTo(graf_binary);
		if (j > 0 && deleted_pixels < ((deleted_pixels_prev * 70) / 100))
		{
			cout << "breaking out of cycle at j == "<< j;
			break;
		}
	}
	namedWindow("graff_vertexes", WINDOW_AUTOSIZE);
	imshow("graff_vertexes", graf_vertexes);
}

void CheckIsVertex()
{
	/*finction checks if all remaining pixels belongs to vertexes, if pixels remains in the 
	plase where edges are crosing, function deletes them. */

}

void GetEdgesWithMyAlg(vector<pair<unsigned, unsigned>> graf_pixels, Mat graf_binary)
{
	/**/
}

void GetEdgesWithOpencv(Mat graf_binary)
{
	/*finding graff edges using opencv library functions
	bt it didn't find all edges*/
	/*Mat edges;
	Canny(graf_binary, edges, 0, 0);
	imwrite("edges.jpg", edges);
	imshow("Canny edges", edges);*/
	vector<Vec4i> lines;
	// Find hough lines 
	HoughLinesP(graf_binary, lines, 1, CV_PI / 180, 100, 100, 20);

	// Prepare blank mat with same sizes as image
	Mat Blank(graf_binary.rows, graf_binary.cols, CV_8UC3, Scalar(0, 0, 0));

	// Draw lines into image and Blank images
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];

		line(graf_binary, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), 2, CV_AA);

		line(Blank, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 255), 2, CV_AA);

	}
	imwrite("houg2.jpg", Blank);
	imshow("Edges Structure", Blank);
}