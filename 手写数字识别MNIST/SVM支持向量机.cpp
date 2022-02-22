#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include<opencv2/ml.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;


//小端存储转换
int BigToSmall(int i);
//读取image数据集信息
Mat read_mnist_image(const string fileName);
//读取label数据集信息
Mat read_mnist_label(const string fileName);

string train_images_path = "D:/手写字符识别/mnist_1998/train-images.idx3-ubyte";
string train_labels_path = "D:/手写字符识别/mnist_1998/train-labels.idx1-ubyte";
string test_images_path = "D:/手写字符识别/mnist_1998/t10k-images.idx3-ubyte";
string test_labels_path = "D:/手写字符识别/mnist_1998/t10k-labels.idx1-ubyte";
/*
　　-d degree：核函数中的degree设置(针对多项式核函数)(默认3)
　　-g r(gama)：核函数中的gamma函数设置(针对多项式/rbf/sigmoid核函数)(默认1/ k)
　　-r coef0：核函数中的coef0设置(针对多项式/sigmoid核函数)((默认0)
　　-c cost：设置C-SVC，e -SVR和v-SVR的参数(损失函数)(默认1)
　　-n nu：设置v-SVC，一类SVM和v- SVR的参数(默认0.5)
　　-p p：设置e -SVR 中损失函数p的值(默认0.1)
　　-m cachesize：设置cache内存大小，以MB为单位(默认40)
　　-e eps：设置允许的终止判据(默认0.001)
　　-h shrinking：是否使用启发式，0或1(默认1)
　　-wi weight：设置第几类的参数C为weight*C(C-SVC中的C)(默认1)
　　-v n: n-fold交互检验模式，n为fold的个数，必须大于等于2
　　其中-g选项中的k是指输入数据中的属性数。option -v 随机地将数据剖分为n部
*/
int main()
{
	//-------------------训练数据准备
	//读取训练标签数据 (60000,1) 类型为int32
	Mat train_labels = read_mnist_label(train_labels_path);
	imwrite("D:/手写字符识别/train_labels.bmp", train_labels);
	//读取训练图像数据 (60000,784) 类型为float32 数据未归一化
	Mat train_images = read_mnist_image(train_images_path);
	imwrite("D:/手写字符识别/train_images.bmp", train_images);
	//将图像数据归一化
	train_images = train_images / 255.0;
	//读取测试数据标签(10000,1) 类型为int32
	Mat test_labels = read_mnist_label(test_labels_path);
	imwrite("D:/手写字符识别/test_labels.bmp", test_labels);
	//读取测试数据图像 (10000,784) 类型为float32 数据未归一化
	Mat test_images = read_mnist_image(test_images_path);
	imwrite("D:/手写字符识别/test_images.bmp", test_images);
	//归一化
	test_images = test_images / 255.0;
	//-------------------构建svm训练模型并进行训练
	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	//设置类型为C_SVC代表分类——>C类支撑向量分类机。 n类分组 （n≥2），容许用异常值处罚因子C进行不完全分类。
	svm->setType(cv::ml::SVM::C_SVC);
	//设置核函数——>多项式内核：
	svm->setKernel(cv::ml::SVM::POLY);
	//设置其它属性
	svm->setGamma(3);//内核函数（POLY）的参数degree//核函数中的gamma函数设置(针对多项式/rbf/sigmoid核函数)(默认1/ k)
	svm->setDegree(3);//内核函数（POLY/ RBF/ SIGMOID）的参数//核函数中的degree设置(针对多项式核函数)(默认3)
	svm->setCoef0(1);
	svm->setNu(0.5);
	svm->setP(0);
	//设置迭代终止条件 
	svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 1000, 0.0001));
	//开始训练
	cv::Ptr<cv::ml::TrainData> train_data = cv::ml::TrainData::create(train_images, cv::ml::ROW_SAMPLE, train_labels);
	cout << "Start To Train" << endl;
	svm->train(train_data);
	cout << "Finish Training" << endl;
	//-------------------在测试数据集上预测计算准确率
	Mat pre_out;
	//返回值为第一个图像的预测值 pre_out为整个batch的预测值集合
	cout << "Start To Predict" << endl;
	float ret = svm->predict(test_images, pre_out);
	cout << "Finish Predicting" << endl;
	//计算准确率必须将两种标签化为同一数据类型
	pre_out.convertTo(pre_out, CV_8UC1);
	test_labels.convertTo(test_labels, CV_8UC1);
	int num = 0;
	for (int i = 0; i < pre_out.rows; i++)
	{
		if (pre_out.at<uchar>(i, 0) == test_labels.at<uchar>(i, 0))
		{
			num++;
		}
	}
	float acc = float(num) / float(pre_out.rows);
	cout << "测试数据集上的准确率为：" << acc * 100 << "%" << endl;
	//保存模型
	svm->save("D:/手写字符识别/mnist_svm.xml");
	getchar();
	return 0;
}

int BigToSmall(int i) {
	//mnist是小端存储我们需要转换为大端存储所以需要获得一个整形int（32位）每位的二进制值然后反向转换
	unsigned char c1, c2, c3, c4;//用无符号的char类型定义每8位二进制
	c1 = i & 255;//&——与运算;255——11111111;该语句的意思是取出i的后八位
	c2 = (i >> 8) & 255;//右移8位
	c3 = (i >> 16) & 255;//右移16位
	c4 = (i >> 24) & 255;//右移24位
	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

Mat read_mnist_image(const string fileName) {
	int magic_number = 0;
	int number_of_images = 0;
	int rows = 0;
	int cols = 0;
	Mat DataMat;
	ifstream file(fileName, ios::binary);
	if (file.is_open())
	{
		cout << "Success To Open Images" << endl;
		file.read((char*)&magic_number, sizeof(magic_number));//（文件格式）
		file.read((char*)&number_of_images, sizeof(number_of_images));//图像总数
		file.read((char*)&rows, sizeof(rows));//每个图像的行数
		file.read((char*)&cols, sizeof(cols));//每个图像的列数
		magic_number = BigToSmall(magic_number);
		number_of_images = BigToSmall(number_of_images);
		rows = BigToSmall(rows);
		cols = BigToSmall(cols);
		cout << "（File Mode）:" << magic_number
			<< " Pixel Num:" << number_of_images
			<< " Rows:" << rows
			<< " Cols:" << cols << endl;
		cout << "Start" << endl;
		DataMat = Mat::zeros(number_of_images, rows * cols, CV_32FC1);
		for (int i = 0; i < number_of_images; i++) {
			for (int j = 0; j < rows * cols; j++) {
				unsigned char temp = 0;
				file.read((char*)&temp, sizeof(temp));//给定初始位置然后依次每8位进行像素读取
				//可以在下面这一步将每个像素值归一化
				float pixel_value = float(temp);//相当于每8位是一个灰度值
				//按照行将像素值一个个写入Mat中
				DataMat.at<float>(i, j) = pixel_value;
			}
		}
		cout << "Finished" << endl;

	}
	file.close();
	return DataMat;
}

Mat read_mnist_label(const string fileName)
{
	int magic_number;
	int number_of_items;
	Mat LabelMat;
	ifstream file(fileName, ios::binary);
	if (file.is_open())
	{
		cout << "Open Label " << endl;
		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_items, sizeof(number_of_items));
		magic_number = BigToSmall(magic_number);
		number_of_items = BigToSmall(number_of_items);
		cout << "（File Mode）:" << magic_number << "  ;Label Num:" << number_of_items << endl;
		cout << "Load Label" << endl;
		//CV_32SC1代表32位有符号整型 通道数为1
		LabelMat = Mat::zeros(number_of_items, 1, CV_32SC1);
		for (int i = 0; i < number_of_items; i++) {
			char temp = 0;
			file.read((char*)&temp, sizeof(temp));
			LabelMat.at<int>(i, 0) = (int)temp;
		}
		cout << "Finish Label" << endl;
	}
	file.close();
	return LabelMat;
}



