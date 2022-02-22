#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define PI acos(-1);
typedef unsigned char BYTE;
int length, width;
int leftup_x, leftup_y;
double R_sum, B_sum, G_sum;
double R_average, B_average, G_average;
vector<Mat> orc_final, new_orc_final;
int value;
void callBack(int, void*);  //�������ص�����
Mat �Ҷ�ͼ, ��ֵͼ, Compressed_��ֵͼ, �����ĻҶ�ͼ;
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
struct object_rect {
	int x;
	int y;
	int width;
	int height;
};
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
Mat WhiteBalance(Mat src)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			R_sum += (int)src.ptr(i, j)[2];
			G_sum += (int)src.ptr(i, j)[1];
			B_sum += (int)src.ptr(i, j)[0];
		}
	}
	R_average = R_sum / sizeof(src);
	B_average = B_sum / sizeof(src);
	G_average = G_sum / sizeof(src);
	double KR, KG, KB;
	KB = (R_average + G_average + B_average) / (3 * B_average);
	KG = (R_average + G_average + B_average) / (3 * G_average);
	KR = (R_average + G_average + B_average) / (3 * R_average);
	Mat image(src.rows, src.cols, CV_8UC1, Scalar(0));
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			image.ptr(i, j)[2] = KB * src.ptr(i, j)[2];
			image.ptr(i, j)[1] = KG * src.ptr(i, j)[1];
			image.ptr(i, j)[0] = KR * src.ptr(i, j)[0];
		}
	}
	return image;
}
void Pre_Slice(Mat image)
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
	width = character[n - 1].x - character[0].x;
	cout << "��ȣ�" << width << endl;
	cout << "//----------------------------------------" << endl;
	leftup_x = character[0].x;
	sort(character, character + n, �������������Сֵ);
	length = character[n - 1].y - character[0].y;
	cout << "���ȣ�" << length << endl;
	leftup_y = character[0].y;
	cout << leftup_x << " " << leftup_y << endl;
}
Mat Pre_Split(Mat image)
{
	Mat sliced_picture;
	vector<int> horizontal_ans;
	for (int i = 0; i < image.rows; i++)
	{
		int horizontal_��ɫ���ص� = 0;
		for (int j = 0; j < image.cols; j++)
		{
			int prePixel = image.at<BYTE>(i, j);
			if (prePixel == 255)
				horizontal_��ɫ���ص�++;

		}
		horizontal_ans.push_back(horizontal_��ɫ���ص�);
	}
	Mat ˮƽֱ��ͼ(image.cols, image.rows, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
	for (size_t i = 0; i < image.rows; i++)
	{
		line(ˮƽֱ��ͼ, Point(-1, i), Point(-1 + horizontal_ans[i], i), Scalar(255), 1, 8);/*draw_imgΪֱ��ͼ��Point(i, height_src), Point(i, height_src - vector_num[i])��ʾ
		�趨��ʼ����ս�㣬��ʼ����ÿ��ÿ�����أ������ص�yֵ��λ��ͼ�����·�������ֱ��ͼ�ĸ߶ȣ����ս��������ʼ�����Ӧ�ģ���xֵ���ս��һ�£�ֻ��yֵ�Ǹ߶ȼ�ȥÿ��ͳ�Ƴ������ظ�������ɫΪ��ɫ
		��1Ϊ�߿�8Ϊ�߶ε�����(Point�ڵĵ��������෴��)*/
	}
	bool sliced = false;
	int start_index = 0;
	int end_index = 0;
	for (size_t i = 0; i < image.rows; i++)
	{
		if (!sliced&&horizontal_ans[i] != 0)
		{
			sliced = true;
			start_index = i;
		}
		else if (sliced&&horizontal_ans[i] == 0)
		{
			sliced = false;
			end_index = i;
			sliced_picture = image(Range(start_index, end_index), Range(0, image.cols));
		}
	}
	return sliced_picture;
}
vector<Mat> Vertical_Shadow(Mat src)//��ֱͶӰ
{
	int width = src.cols;
	int height = src.rows;
	vector<int> vertical_ans;
	vector<int> horizontal_ans;
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
	Mat ��ֱֱ��ͼ(height, width, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
	for (size_t i = 0; i < width; i++)
	{
		line(��ֱֱ��ͼ, Point(i, height), Point(i, height - vertical_ans[i]), Scalar(255), 1, 8);/*draw_imgΪֱ��ͼ��Point(i, height_src), Point(i, height_src - vector_num[i])��ʾ
		�趨��ʼ����ս�㣬��ʼ����ÿ��ÿ�����أ������ص�yֵ��λ��ͼ�����·�������ֱ��ͼ�ĸ߶ȣ����ս��������ʼ�����Ӧ�ģ���xֵ���ս��һ�£�ֻ��yֵ�Ǹ߶ȼ�ȥÿ��ͳ�Ƴ������ظ�������ɫΪ��ɫ
		��1Ϊ�߿�8Ϊ�߶ε�����(Point�ڵĵ��������෴��)*/
	}
	imwrite("D:/��д�ַ�ʶ��//��ֱֱ��ͼ.bmp", ��ֱֱ��ͼ);
	Mat ˮƽֱ��ͼ(height, width, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
	for (size_t i = 0; i < height; i++)
	{
		line(ˮƽֱ��ͼ, Point(0, i), Point(horizontal_ans[i], i), Scalar(255), 1, 8);/*draw_imgΪֱ��ͼ��Point(i, height_src), Point(i, height_src - vector_num[i])��ʾ
		�趨��ʼ����ս�㣬��ʼ����ÿ��ÿ�����أ������ص�yֵ��λ��ͼ�����·�������ֱ��ͼ�ĸ߶ȣ����ս��������ʼ�����Ӧ�ģ���xֵ���ս��һ�£�ֻ��yֵ�Ǹ߶ȼ�ȥÿ��ͳ�Ƴ������ظ�������ɫΪ��ɫ
		��1Ϊ�߿�8Ϊ�߶ε�����(Point�ڵĵ��������෴��)*/
	}
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
void Shift_To_28(vector<Mat> src, int thresholdnum)
{
	int num = 0;
	std::vector<Mat>::iterator it;
	for (it = src.begin(); it != src.end(); it++)
	{
		cv::Mat img = *it;
		int w = img.cols;
		int h = img.rows;
		float t = 1.;
		int dst_w = 40, dst_h = 40;
		cv::Mat dst = cv::Mat(cv::Size(dst_w, dst_h), CV_8UC1, cv::Scalar(0));
		float len = t * std::max(w, h);
		float len_ = std::min(dst_w, dst_h);
		float ratio = len_ / len;
		int w_ = ratio * w;
		int h_ = ratio * h;
		int delta_x = (dst_w - w_) / (2 * ratio);
		int delta_y = (dst_h - h_) / (2 * ratio);
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				float u = (i + delta_x) / len;
				float v = (j + delta_y) / len;
				int x = u * len_;
				int y = v * len_;
				dst.at<uchar>(y, x) = img.at<uchar>(j, i);
			}
		}
		Mat Mask = cv::Mat(cv::Size(40, 40), CV_8UC1, cv::Scalar(0));
		resize(dst, dst, Size(28, 28));
		for (int i = 0; i < dst.rows; i++)
		{
			for (int j = 0; j < dst.cols; j++)
			{
				Mask.ptr(i + 6)[j + 6] = dst.ptr(i)[j];
			}
		}
		resize(Mask, dst, Size(28, 28));
		float average = 0, sum = 0;
		for (int i = 0; i < dst.rows; i++)
		{
			for (int j = 0; j < dst.cols; j++)
			{
				sum += dst.ptr(i)[j];
			}
		}
		average = sum / (dst.rows*dst.cols);
		cout << "��" << num << "��" << "ͼ��ƽ������ֵ�� " << average << endl;
		if (average < thresholdnum)
		{
			for (int i = 0; i < dst.rows; i++)
			{
				for (int j = 0; j < dst.cols; j++)
				{
					if ((int)dst.ptr(i)[j] > 0)
					{
						dst.ptr(i)[j] = 255;
					}
					else
						dst.ptr(i)[j] = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < dst.rows; i++)
			{
				for (int j = 0; j < dst.cols; j++)
				{
					if ((int)dst.ptr(i)[j] > 0)
					{
						if ((int)dst.ptr(i)[j] + (int)average > 255)
						{
							dst.ptr(i)[j] = 255;
						}
						else
							dst.ptr(i)[j] = (int)dst.ptr(i)[j] + (int)average;
					}
					else
						dst.ptr(i)[j] = 0;
				}
			}
		}
		imwrite("D://��д�ַ�ʶ��//�ָ��ַ�2828//" + to_string(num) + ".bmp", dst);
		num++;
	}
}
vector<Mat> Horizontal_Shadow(vector<Mat> ans)
{
	std::vector<int>::iterator it01;
	std::vector<Mat>::iterator it02;
	int index = 0;
	vector<Mat> image;
	for (it02 = ans.begin(); it02 != ans.end(); it02++)
	{
		Mat src;
		src = *it02;
		int width = src.cols;
		int height = src.rows;
		vector<int> horizontal_ans;
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
		Mat ˮƽֱ��ͼ(height, width, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
		for (size_t i = 0; i < height; i++)
		{
			line(ˮƽֱ��ͼ, Point(-1, i), Point(-1 + horizontal_ans[i], i), Scalar(255), 1, 8);/*draw_imgΪֱ��ͼ��Point(i, height_src), Point(i, height_src - vector_num[i])��ʾ
			�趨��ʼ����ս�㣬��ʼ����ÿ��ÿ�����أ������ص�yֵ��λ��ͼ�����·�������ֱ��ͼ�ĸ߶ȣ����ս��������ʼ�����Ӧ�ģ���xֵ���ս��һ�£�ֻ��yֵ�Ǹ߶ȼ�ȥÿ��ͳ�Ƴ������ظ�������ɫΪ��ɫ
			��1Ϊ�߿�8Ϊ�߶ε�����(Point�ڵĵ��������෴��)*/
		}
		bool sliced = false;
		int start_index = 0;
		int end_index = 0;
		for (size_t i = 0; i < height; i++)
		{
			if (!sliced&&horizontal_ans[i] != 0)
			{
				sliced = true;
				start_index = i;
			}
			else if (sliced&&horizontal_ans[i] == 0)
			{
				sliced = false;
				end_index = i;
				Mat sliced_picture = src(Range(start_index, end_index), Range(0, src.cols));
				image.push_back(sliced_picture);
			}
		}
		index++;
	}
	return image;
}
vector<Mat> Check_WhitePixel(vector<Mat> src)
{
	vector<Mat> image;
	std::vector<Mat>::iterator it;
	double sum = 0, average = 0;
	for (it = src.begin(); it != src.end(); it++)
	{
		Mat ans = *it;
		for (int i = 0; i < ans.rows; i++)
		{
			for (int j = 0; j < ans.cols; j++)
			{
				if ((int)ans.ptr(i)[j] > 0)
				{
					sum++;
				}
				else
					continue;
			}
		}
	}
	average = sum / src.size();
	for (it = src.begin(); it != src.end(); it++)
	{
		int num = 0;
		Mat ans = *it;
		for (int i = 0; i < ans.rows; i++)
		{
			for (int j = 0; j < ans.cols; j++)
			{
				if ((int)ans.ptr(i)[j] > 0)
				{
					num++;
				}
				else
					continue;
			}
		}
		if (num >= average * 0.1)
		{
			image.push_back(ans);
		}
	}
	return image;
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
Mat threshold_deal(Mat mat, int threhold_num)
{
	Mat new_mat(mat.rows, mat.cols, CV_8UC1, Scalar(0));	//������ֱͶӰֱ��ͼ
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			if ((int)mat.ptr(i)[j] < threhold_num)
			{
				new_mat.ptr(i)[j] = 255;
			}
			else
			{
				new_mat.ptr(i)[j] = 0;
			}
		}
	}
	return new_mat;
}
void Hough_Transform(string path)
{
	Mat src, gray, dst; Mat midImage, dstImage;
	src = imread(path);
	Canny(src, gray, 150, 200);
	vector<Vec4f> plines;
	int num = 0;
	HoughLinesP(gray, plines, 1, CV_PI / 180, 5, 0, 0);
	Scalar color = Scalar(0, 0, 255);
	for (size_t i = 0; i < plines.size(); i++)
	{
		Vec4f hline = plines[i];
		float MaxD = pow((hline[0] - hline[2]), 2) + pow((hline[1] - hline[3]), 2);
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
void ThresholdNum_Slider(Mat Canvas)
{
	namedWindow("��д�ַ�ʶ��ָ�", 0);//Size((840 * 1.0 / ��ֵͼ.rows)*��ֵͼ.cols, 840)
	resizeWindow("��д�ַ�ʶ��ָ�", (360 * 1.0 / Canvas.rows)*Canvas.cols, 360);
	imshow("��д�ַ�ʶ��ָ�", Canvas);
	value = 32;  //����������ʱ�ĳ�ֵ
	createTrackbar("��ǰ��ֵ", "��д�ַ�ʶ��ָ�", &value, 255, callBack);
	//waitKey();
}
static void callBack(int, void*)
{
	namedWindow("��ֵͼ", 0);//Size((840 * 1.0 / ��ֵͼ.rows)*��ֵͼ.cols, 840)
	resizeWindow("��ֵͼ", (360 * 1.0 / �Ҷ�ͼ.rows)*�Ҷ�ͼ.cols, 360);
	��ֵͼ = threshold_deal(�Ҷ�ͼ, value);
	imwrite("D://��д�ַ�ʶ��//��ֵͼ.bmp", ��ֵͼ);
	resize(��ֵͼ, Compressed_��ֵͼ, Size((840 * 1.0 / ��ֵͼ.rows)*��ֵͼ.cols, 840), 3);
	imwrite("D://��д�ַ�ʶ��//��������ֵͼ.bmp", Compressed_��ֵͼ);
	imshow("��ֵͼ", ��ֵͼ);
}
int main(int argc, char **argv)
{
	bool istrainning = false;
	Mat src = imread("D:/��д�ַ�ʶ��/test5.bmp");
	vector<Mat> last_orc;
	int x, y;
	cout <<"��ֵ�˲����ں˴�С Size(x,y)�� ";
	cin >> x >> y;
	blur(src, src, Size(x, y), Point(-1, -1));
	if (src.empty())
	{
		cout << "��д�ַ�ʶ���ļ����в����ڴ�ͼƬ" << endl;
		system("pause");
	}
	//-------------------------------------------------------------------------
	//Mat whitebalance_src;
	//whitebalance_src = WhiteBalance(src);
	//Mat whitebalance_src;
	//cvtColor(src, whitebalance_src, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
	//normalize(whitebalance_src, whitebalance_src, 1.0, 0, NORM_MINMAX);
	//Mat result = whitebalance_src * 255;
	//result.convertTo(whitebalance_src, CV_8UC1);
	//imwrite("D://��д�ַ�ʶ��//whitebalance_src.bmp", whitebalance_src);
	//-------------------------------------------------------------------------
	src = WhiteBalance(src);
	imwrite("D://��д�ַ�ʶ��//��ƽ��.bmp", src);
	�Ҷ�ͼ = imread("D://��д�ַ�ʶ��//��ƽ��.bmp", 0);
	imwrite("D://��д�ַ�ʶ��//�Ҷ�ͼ.bmp", �Ҷ�ͼ);
	ThresholdNum_Slider(�Ҷ�ͼ);
	//-------------------------------------------------------------------------
	//cvtColor(src, �Ҷ�ͼ, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
	/*normalize(�Ҷ�ͼ, �Ҷ�ͼ, 1.0, 0, NORM_MINMAX);
	�����ĻҶ�ͼ = �Ҷ�ͼ * 255;
	imwrite("D://��д�ַ�ʶ��//�����ĻҶ�ͼ.bmp", �����ĻҶ�ͼ);*/
	//blur(whitebalance_src, whitebalance_src, Size(4, 4), Point(-1, -1));
	//threshold(�Ҷ�ͼ, ��ֵͼ, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);//THRESH_TOZEROΪ��ֵ�ָ�����ͣ�������Ϊ����ֵ������ֵʱ��Ϊ0��������Ϊ���ֵ255������ȡ����Ч��
	//-------------------------------------------------------------------------
	while (true)
	{
		int c = waitKey(10);
		if (c == 27)
		{
			istrainning = false;
			cout << "���¼��̡�ESC��" << endl;
			//��ֵͼ = threshold_deal(�Ҷ�ͼ, 64);
			orc_final = Vertical_Shadow(Compressed_��ֵͼ);
			std::vector<Mat>::iterator it;
			int index01 = 0;
			for (it = orc_final.begin(); it != orc_final.end(); it++)
			{
				Mat ans = *it;
				imwrite("D:/��д�ַ�ʶ��//�ָ��ַ�//" + to_string(index01) + ".bmp", ans);
				index01++;
			}
			new_orc_final = Horizontal_Shadow(orc_final);
			int index02 = 0;
			for (it = new_orc_final.begin(); it != new_orc_final.end(); it++)
			{
				Mat ans = *it;
				imwrite("D:/��д�ַ�ʶ��//�ָ��ַ�2828//" + to_string(index02) + ".bmp", ans);
				index02++;
			}
			last_orc = Check_WhitePixel(new_orc_final);
			cout << "������28*28ͼ��ָ���ֵ�� " << endl;
			int threhold;
			cin >> threhold;
			if (!istrainning)
			{
				Shift_To_28(last_orc, threhold);
			}
		}
		if (c == 49)
		{
			istrainning = true;
			cout << "���¼��̡�1��" << endl;
			int index04 = 0;
			int Predict[10];
			for (int i = 0; i < last_orc.size(); i++)
			{
				Mat image = imread("D://��д�ַ�ʶ��//�ָ��ַ�2828//" + to_string(i) + ".bmp", 0);
				imshow(to_string(index04), image);
				Mat img_show = image.clone();
				image.convertTo(image, CV_32F);
				image = image / 255.0;
				image = image.reshape(1, 1);
				cv::Ptr<cv::ml::SVM> svm = cv::ml::StatModel::load<cv::ml::SVM>("D:/��д�ַ�ʶ��/mnist_svm.xml");
				float ret = svm->predict(image);
				//Predict[index04] = ret;
				//cv::imshow(to_string(index04), img_show);
				cout << "Ԥ������" << ret << endl;
				index04++;
			}
			//for (int i = 0; i < index04; i++)
			//{
			//	cout << "Ԥ������" << Predict[i] << endl;
			//}
		}
	}
}