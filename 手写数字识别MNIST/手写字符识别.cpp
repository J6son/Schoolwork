#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define PI acos(-1);
typedef unsigned char BYTE;
int length, width;
int leftup_x, leftup_y;
vector<Mat> orc_final;
struct �ַ��ָ�
{
	int x, y;
	int pixelnum;
}character[1000050];
struct point
{
	double x1, y1, x2, y2;
	double Max_Distance;
}p[1000010];
bool �������������Сֵ(�ַ��ָ� a, �ַ��ָ� b)
{
	return a.x < b.x;
}
bool �������������Сֵ(�ַ��ָ� a, �ַ��ָ� b)
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
	sort(character, character + n, �������������Сֵ);
	//for (int i = 0; i < n; i++)
	//{
	//	cout << character[i].x << " " << character[i].y << endl;
	//}
	width = character[n - 1].x - character[0].x;
	cout << "��ȣ�" << width << endl;
	cout << "//----------------------------------------" << endl;
	leftup_x = character[0].x;
	sort(character, character + n, �������������Сֵ);
	//for (int i = 0; i < n; i++)
	//{
	//	cout << character[i].x << " " << character[i].y << endl;
	//}
	length = character[n - 1].y - character[0].y;
	cout << "���ȣ�" << length << endl;
	leftup_y = character[0].y;
	cout << leftup_x << " " << leftup_y << endl;

}
Mat Pre_Split(Mat image)
{
	cv::Mat split_picture(width + 10, length + 10, CV_8UC1, cv::Scalar(0, 0, 0));//+5��Ϊ�˱�����Ϊд�ֵ�ʱ������д���˱߽�Ӷ���ֱͶӰȷ�ϱ߽�ʱ�����Ӷ��ӿ�5�����ر�֤ͶӰʱ�з�������ֵ����ȷ��
	int row = 0, col = 0;
	for (int i = 0; i < width + 10; i++)
	{
		for (int j = 0; j < length + 10; j++)
		{
			split_picture.ptr<BYTE>(i)[j] = image.ptr<BYTE>(leftup_x + i - 5)[leftup_y + j - 5];
		}
	}
	imwrite("D:\\��д�ַ�ʶ��\\split_picture.bmp", split_picture);
	return split_picture;
}
vector<Mat> Shadow(Mat src)//ͶӰ
{
	int width = src.cols;
	int height = src.rows;
	vector<int> vertical_ans;
	vector<int> horizontal_ans;
	/*��ֱͶӰ*/
	for (int i = 0; i < width; i++)
	{
		int vertical_��ɫ���ص� = 0;
		for (int j = 0; j < height; j++)
		{
			int prePixel = src.at<BYTE>(j, i);
			if (prePixel == 255)
				vertical_��ɫ���ص�++;

		}
		vertical_ans.push_back(vertical_��ɫ���ص�);
	}
	std::vector<int>::iterator it;
	//for (it = vertical_ans.begin(); it != vertical_ans.end(); it++)
	//{
	//	//cout << *it << " " << endl;
	//}
	/*ˮƽͶӰ*/
	for (int i = 0; i < height; i++)
	{
		int horizontal_��ɫ���ص� = 0;
		for (int j = 0; j < width; j++)
		{
			int prePixel = src.at<BYTE>(i, j);
			if (prePixel == 255)
				horizontal_��ɫ���ص�++;

		}
		horizontal_ans.push_back(horizontal_��ɫ���ص�);
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
	Mat ��ֱֱ��ͼ(height, width, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
	for (size_t i = 0; i < width; i++)
	{
		line(��ֱֱ��ͼ, Point(i, height), Point(i, height - vertical_ans[i]), Scalar(255), 1, 8);/*draw_imgΪֱ��ͼ��Point(i, height_src), Point(i, height_src - vector_num[i])��ʾ
		�趨��ʼ����ս�㣬��ʼ����ÿ��ÿ�����أ������ص�yֵ��λ��ͼ�����·�������ֱ��ͼ�ĸ߶ȣ����ս��������ʼ�����Ӧ�ģ���xֵ���ս��һ�£�ֻ��yֵ�Ǹ߶ȼ�ȥÿ��ͳ�Ƴ������ظ�������ɫΪ��ɫ
		��1Ϊ�߿�8Ϊ�߶ε�����(Point�ڵĵ��������෴��)*/
	}
	Mat ˮƽֱ��ͼ(height, width, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
	for (size_t i = 0; i < height; i++)
	{
		line(ˮƽֱ��ͼ, Point(0, i), Point(horizontal_ans[i], i), Scalar(255), 1, 8);/*draw_imgΪֱ��ͼ��Point(i, height_src), Point(i, height_src - vector_num[i])��ʾ
		�趨��ʼ����ս�㣬��ʼ����ÿ��ÿ�����أ������ص�yֵ��λ��ͼ�����·�������ֱ��ͼ�ĸ߶ȣ����ս��������ʼ�����Ӧ�ģ���xֵ���ս��һ�£�ֻ��yֵ�Ǹ߶ȼ�ȥÿ��ͳ�Ƴ������ظ�������ɫΪ��ɫ
		��1Ϊ�߿�8Ϊ�߶ε�����(Point�ڵĵ��������෴��)*/
	}
	imshow("��ֱֱ��ͼ", ��ֱֱ��ͼ);
	imwrite("D:/��д�ַ�ʶ��/��ֱֱ��ͼ.bmp", ��ֱֱ��ͼ);
	imshow("ˮƽֱ��ͼ", ˮƽֱ��ͼ);
	imwrite("D:/��д�ַ�ʶ��/ˮƽֱ��ͼ.bmp", ˮƽֱ��ͼ);
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
	src = imread("D://��д�ַ�ʶ��//�ַ��ָ�//2.bmp");
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
		//line(dstImage, Point(p[num].x1, p[num].y1), Point(p[num].x2, p[num].y2), Scalar(55, 100, 195), 1, LINE_AA); //Scalar�������ڵ����߶���ɫ�����������⵽���߶���ʾ����ʲô��ɫ
		//imshow("dst", dstImage);
	}
	cout << "//-------------" << endl;
	sort(p, p + num, cmp);
	for (int i = 0; i < num; i++)
	{
		cout << p[i].x1 << "--" << p[i].y1 << " " << p[i].x2 << " " << p[i].y2 << " " << p[i].Max_Distance << endl;
	}
	line(src, Point(p[num - 1].x1, p[num - 1].y1), Point(p[num - 1].x2, p[num - 1].y2), Scalar(55, 100, 195), 1, 8); //Scalar�������ڵ����߶���ɫ�����������⵽���߶���ʾ����ʲô��ɫ
	imwrite("D://��д�ַ�ʶ��//�ַ��ָ�//dst.bmp", src);
	double angle = Return_Angle(p[num - 1].x1, p[num - 1].y1, p[num - 1].x2, p[num - 1].y2);
	cout << angle << endl;
	Rotate(src, src, angle, (p[num - 1].x1 + p[num - 1].x2) / 2, (p[num - 1].y1 + p[num - 1].y2) / 2);
}
int main(int argc, char **argv)
{
	Mat src = imread("D:/��д�ַ�ʶ��/test.bmp");
	if (src.empty())
	{
		cout << "��д�ַ�ʶ���ļ����в����ڴ�ͼƬ" << endl;
		system("pause");
	}
	imshow("����ͼƬ", src);
	Mat �Ҷ�ͼ, ��ֵͼ;
	cvtColor(src, �Ҷ�ͼ, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
	threshold(�Ҷ�ͼ, ��ֵͼ, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);//THRESH_TOZEROΪ��ֵ�ָ�����ͣ�������Ϊ����ֵ������ֵʱ��Ϊ0��������Ϊ���ֵ255������ȡ����Ч��
	imshow("��ֵͼ", ��ֵͼ);
	Horizontal_SLice(��ֵͼ);
	//Mat Last;
	//Last = Pre_Split(��ֵͼ);
	//imshow("Last", Last);
	orc_final = Shadow(��ֵͼ);
	std::vector<Mat>::iterator it;
	int index01 = 0;
	for (it = orc_final.begin(); it != orc_final.end(); it++)
	{
		Mat ans = *it;
		imwrite("D://��д�ַ�ʶ��//�ַ��ָ�//" + to_string(index01) + ".bmp", ans);
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
		imwrite("D://��д�ַ�ʶ��//�ָ��ַ�28 28//" + to_string(index02) + ".bmp", ans);
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
		cv::Ptr<cv::ml::SVM> svm = cv::ml::StatModel::load<cv::ml::SVM>("D:/��д�ַ�ʶ��/mnist_svm.xml");
		float ret = svm->predict(image);
		Predict[index03] = ret;
		cv::imshow(to_string(index03), img_show);
		index03++;
	}
	for (int i = 0; i < index03; i++)
	{
		cout << "Ԥ������" << Predict[i] << endl;
	}
	return 0;
}
