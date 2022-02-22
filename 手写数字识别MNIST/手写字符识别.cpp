#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define PI acos(-1);
typedef unsigned char BYTE;
int length, width;
int leftup_x, leftup_y;
vector<Mat> orc_final;
struct 字符分割
{
	int x, y;
	int pixelnum;
}character[1000050];
struct point
{
	double x1, y1, x2, y2;
	double Max_Distance;
}p[1000010];
bool 行数查找最大最小值(字符分割 a, 字符分割 b)
{
	return a.x < b.x;
}
bool 列数查找最大最小值(字符分割 a, 字符分割 b)
{
	return a.y < b.y;
}
bool cmp(point a, point b)
{
	return a.Max_Distance < b.Max_Distance;
}
void Horizontal_SLice(Mat image)
{
	int n = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 1; j < image.cols; j++)
		{
			if (image.ptr<unsigned char>(i)[j - 1] == 0 && image.ptr<unsigned char>(i)[j] == 255)
			{
				character[n].x = i;
				character[n].y = j;
				n++;
			}
			else
				continue;
		}
	}
	sort(character, character + n, 行数查找最大最小值);
	//for (int i = 0; i < n; i++)
	//{
	//	cout << character[i].x << " " << character[i].y << endl;
	//}
	width = character[n - 1].x - character[0].x;
	cout << "宽度：" << width << endl;
	cout << "//----------------------------------------" << endl;
	leftup_x = character[0].x;
	sort(character, character + n, 列数查找最大最小值);
	//for (int i = 0; i < n; i++)
	//{
	//	cout << character[i].x << " " << character[i].y << endl;
	//}
	length = character[n - 1].y - character[0].y;
	cout << "长度：" << length << endl;
	leftup_y = character[0].y;
	cout << leftup_x << " " << leftup_y << endl;

}
Mat Pre_Split(Mat image)
{
	cv::Mat split_picture(width + 10, length + 10, CV_8UC1, cv::Scalar(0, 0, 0));//+5是为了避免因为写字的时候字体写在了边界从而垂直投影确认边界时出错，从而加宽5个像素保证投影时列方向像素值的正确性
	int row = 0, col = 0;
	for (int i = 0; i < width + 10; i++)
	{
		for (int j = 0; j < length + 10; j++)
		{
			split_picture.ptr<BYTE>(i)[j] = image.ptr<BYTE>(leftup_x + i - 5)[leftup_y + j - 5];
		}
	}
	imwrite("D:\\手写字符识别\\split_picture.bmp", split_picture);
	return split_picture;
}
vector<Mat> Shadow(Mat src)//投影
{
	int width = src.cols;
	int height = src.rows;
	vector<int> vertical_ans;
	vector<int> horizontal_ans;
	/*垂直投影*/
	for (int i = 0; i < width; i++)
	{
		int vertical_白色像素点 = 0;
		for (int j = 0; j < height; j++)
		{
			int prePixel = src.at<BYTE>(j, i);
			if (prePixel == 255)
				vertical_白色像素点++;

		}
		vertical_ans.push_back(vertical_白色像素点);
	}
	std::vector<int>::iterator it;
	//for (it = vertical_ans.begin(); it != vertical_ans.end(); it++)
	//{
	//	//cout << *it << " " << endl;
	//}
	/*水平投影*/
	for (int i = 0; i < height; i++)
	{
		int horizontal_白色像素点 = 0;
		for (int j = 0; j < width; j++)
		{
			int prePixel = src.at<BYTE>(i, j);
			if (prePixel == 255)
				horizontal_白色像素点++;

		}
		horizontal_ans.push_back(horizontal_白色像素点);
	}
	//std::vector<int>::iterator it;
	//for (it = vertical_ans.begin(); it != vertical_ans.end(); it++)
	//{
	//	cout << *it << " " << endl;
	//}
		//for (it = horizontal_ans.begin(); it != horizontal_ans.end(); it++)
	//{
	//	cout << *it << " " << endl;
	//}
	Mat 垂直直方图(height, width, CV_8UC1, Scalar(0));	//画出竖直投影直方图
	for (size_t i = 0; i < width; i++)
	{
		line(垂直直方图, Point(i, height), Point(i, height - vertical_ans[i]), Scalar(255), 1, 8);/*draw_img为直方图，Point(i, height_src), Point(i, height_src - vector_num[i])表示
		设定起始点和终结点，起始点是每列每个像素，且像素的y值是位于图像最下方，即是直方图的高度，而终结点是与起始点相对应的，其x值和终结点一致，只是y值是高度减去每行统计出的像素个数，颜色为白色
		，1为线宽，8为线段的类型(Point内的点是行列相反的)*/
	}
	Mat 水平直方图(height, width, CV_8UC1, Scalar(0));	//画出竖直投影直方图
	for (size_t i = 0; i < height; i++)
	{
		line(水平直方图, Point(0, i), Point(horizontal_ans[i], i), Scalar(255), 1, 8);/*draw_img为直方图，Point(i, height_src), Point(i, height_src - vector_num[i])表示
		设定起始点和终结点，起始点是每列每个像素，且像素的y值是位于图像最下方，即是直方图的高度，而终结点是与起始点相对应的，其x值和终结点一致，只是y值是高度减去每行统计出的像素个数，颜色为白色
		，1为线宽，8为线段的类型(Point内的点是行列相反的)*/
	}
	imshow("垂直直方图", 垂直直方图);
	imwrite("D:/手写字符识别/垂直直方图.bmp", 垂直直方图);
	imshow("水平直方图", 水平直方图);
	imwrite("D:/手写字符识别/水平直方图.bmp", 水平直方图);
	bool sliced = false;
	int start_index = 0;
	int end_index = 0;
	vector<Mat> orc_image;
	for (size_t i = 0; i < width; i++)
	{
		if (!sliced&&vertical_ans[i] != 0)
		{
			sliced = true;
			start_index = i;
		}
		else if (sliced&&vertical_ans[i] == 0)
		{
			sliced = false;
			end_index = i;
			Mat sliced_picture = src(Range(0, height), Range(start_index, end_index));
			orc_image.push_back(sliced_picture);
		}
	}
	return orc_image;
}
vector<Mat> Compress_Picture()
{
	std::vector<Mat>::iterator it;
	vector<Mat> compressed_picture;
	for (it = orc_final.begin(); it != orc_final.end(); it++)
	{
		Mat ans = *it;
		Mat dst;
		resize(ans, dst, Size(28, 28), 28 * 1.0 / (ans.rows), 28 * 1.0 / (ans.cols), 3);
		compressed_picture.push_back(dst);
	}
	return compressed_picture;
}
void Rotate(const Mat &srcImage, Mat &destImage, double angle, int x, int y)
{
	Point2f center(x, y);
	Mat M = getRotationMatrix2D(center, angle, 1);
	warpAffine(srcImage, destImage, M, Size(srcImage.cols, srcImage.rows));
}
double Return_Angle(double x1, double y1, double x2, double y2)
{
	double angle; double x3, y3; double a, b, c;
	x3 = x1, y3 = y2;
	a = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	b = sqrt(pow(x1 - x2, 2) + pow(y2 - y2, 2));
	c = sqrt(pow(x1 - x1, 2) + pow(y1 - y2, 2));
	angle = std::acos((a*a + b * b - c * c) / (2 * a*b)) * 180 / PI;
	double k;
	k = (y2 - y1) / (x2 - x1);
	if (k < 0)
		return angle;
	else
		return -90 + angle;
}
void Hough_Transform()
{
	Mat src, gray, dst; Mat midImage, dstImage;
	src = imread("D://手写字符识别//字符分割//2.bmp");
	Canny(src, gray, 150, 200);
	vector<Vec4f> plines;
	int num = 0;
	HoughLinesP(gray, plines, 1, CV_PI / 180, 5, 0, 0);
	Scalar color = Scalar(0, 0, 255);
	for (size_t i = 0; i < plines.size(); i++)
	{
		Vec4f hline = plines[i];
		float MaxD = pow((hline[0] - hline[2]), 2) + pow((hline[1] - hline[3]), 2);
		//cout << MaxD << " ";
		//cout << hline[0] << " " << hline[1] << " " << hline[2] << " " << hline[3] << endl;
		p[num].Max_Distance = MaxD;
		p[num].x1 = hline[0];
		p[num].y1 = hline[1];
		p[num].x2 = hline[2];
		p[num].y2 = hline[3];
		num++;
		//line(dstImage, Point(p[num].x1, p[num].y1), Point(p[num].x2, p[num].y2), Scalar(55, 100, 195), 1, LINE_AA); //Scalar函数用于调节线段颜色，就是你想检测到的线段显示的是什么颜色
		//imshow("dst", dstImage);
	}
	cout << "//-------------" << endl;
	sort(p, p + num, cmp);
	for (int i = 0; i < num; i++)
	{
		cout << p[i].x1 << "--" << p[i].y1 << " " << p[i].x2 << " " << p[i].y2 << " " << p[i].Max_Distance << endl;
	}
	line(src, Point(p[num - 1].x1, p[num - 1].y1), Point(p[num - 1].x2, p[num - 1].y2), Scalar(55, 100, 195), 1, 8); //Scalar函数用于调节线段颜色，就是你想检测到的线段显示的是什么颜色
	imwrite("D://手写字符识别//字符分割//dst.bmp", src);
	double angle = Return_Angle(p[num - 1].x1, p[num - 1].y1, p[num - 1].x2, p[num - 1].y2);
	cout << angle << endl;
	Rotate(src, src, angle, (p[num - 1].x1 + p[num - 1].x2) / 2, (p[num - 1].y1 + p[num - 1].y2) / 2);
}
int main(int argc, char **argv)
{
	Mat src = imread("D:/手写字符识别/test.bmp");
	if (src.empty())
	{
		cout << "手写字符识别文件夹中不存在此图片" << endl;
		system("pause");
	}
	imshow("输入图片", src);
	Mat 灰度图, 阈值图;
	cvtColor(src, 灰度图, COLOR_BGR2GRAY);//转换为灰度图
	threshold(灰度图, 阈值图, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);//THRESH_TOZERO为阈值分割的类型，该类型为像素值大于阈值时设为0，其余设为最大值255，即是取反的效果
	imshow("阈值图", 阈值图);
	Horizontal_SLice(阈值图);
	//Mat Last;
	//Last = Pre_Split(阈值图);
	//imshow("Last", Last);
	orc_final = Shadow(阈值图);
	std::vector<Mat>::iterator it;
	int index01 = 0;
	for (it = orc_final.begin(); it != orc_final.end(); it++)
	{
		Mat ans = *it;
		imwrite("D://手写字符识别//字符分割//" + to_string(index01) + ".bmp", ans);
		index01++;
	}
	vector<Mat> compressed;
	compressed = Compress_Picture();
	int index02 = 0;
	for (it = compressed.begin(); it != compressed.end(); it++)
	{
		Mat ans = *it;
		for (int i = 0; i < ans.rows; i++)
		{
			for (int j = 0; j < ans.cols; j++)
			{
				if (ans.ptr(i)[j] > 0)
				{
					ans.ptr(i)[j] = 255;
				}
				else
					continue;
			}
		}
		imwrite("D://手写字符识别//分割字符28 28//" + to_string(index02) + ".bmp", ans);
		index02++;
	}
	int index03 = 0;
	int Predict[10];
	for (it = compressed.begin(); it != compressed.end(); it++)
	{
		Mat image = *it;
		Mat img_show = image.clone();
		image.convertTo(image, CV_32F);
		image.convertTo(image, CV_32F);
		image = image / 255.0;
		image = image.reshape(1, 1);
		cv::Ptr<cv::ml::SVM> svm = cv::ml::StatModel::load<cv::ml::SVM>("D:/手写字符识别/mnist_svm.xml");
		float ret = svm->predict(image);
		Predict[index03] = ret;
		cv::imshow(to_string(index03), img_show);
		index03++;
	}
	for (int i = 0; i < index03; i++)
	{
		cout << "预测结果：" << Predict[i] << endl;
	}
	return 0;
}
