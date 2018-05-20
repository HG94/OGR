#include "OGR.h"

int main()
{
	Mat graf_binary;
	graf_binary = RgbToBinary("graff.jpg");
	
	namedWindow("binary_graff", WINDOW_AUTOSIZE);
	imshow("binary_graff", graf_binary);
	
	vector<pair<unsigned, unsigned>> graf_pixels = GetGrafPixels(graf_binary);
	//cout << "hello" << (unsigned)(graf_binary.at<uchar>(0, 105)) <<"buy";

	/*to check that we find correct pixels
	Mat graf_pix(rows, cols, CV_8UC1, Scalar(255));
	for (int i = 0; i < graf_pixels.size(); ++i)
		graf_pix.at<uchar>(graf_pixels[i].first, graf_pixels[i].second) = 165;
	namedWindow("graff", WINDOW_AUTOSIZE);
	imshow("graff", graf_pix);*/

	Mat graf_vertexes;
	graf_vertexes = GetVertexesWithMyAlg(graf_pixels, graf_binary);

	Mat graf_filled_vertexes;
	graf_filled_vertexes = fillVertexes(graf_pixels, graf_vertexes, graf_binary);

	GetEdgesWithMyAlg(graf_pixels, graf_binary, graf_filled_vertexes);

	waitKey(0);
	return 0;
}

