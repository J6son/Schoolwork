#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include<opencv2/ml.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;


//С�˴洢ת��
int BigToSmall(int i);
//��ȡimage���ݼ���Ϣ
Mat read_mnist_image(const string fileName);
//��ȡlabel���ݼ���Ϣ
Mat read_mnist_label(const string fileName);

string train_images_path = "D:/��д�ַ�ʶ��/mnist_1998/train-images.idx3-ubyte";
string train_labels_path = "D:/��д�ַ�ʶ��/mnist_1998/train-labels.idx1-ubyte";
string test_images_path = "D:/��д�ַ�ʶ��/mnist_1998/t10k-images.idx3-ubyte";
string test_labels_path = "D:/��д�ַ�ʶ��/mnist_1998/t10k-labels.idx1-ubyte";
/*
����-d degree���˺����е�degree����(��Զ���ʽ�˺���)(Ĭ��3)
����-g r(gama)���˺����е�gamma��������(��Զ���ʽ/rbf/sigmoid�˺���)(Ĭ��1/ k)
����-r coef0���˺����е�coef0����(��Զ���ʽ/sigmoid�˺���)((Ĭ��0)
����-c cost������C-SVC��e -SVR��v-SVR�Ĳ���(��ʧ����)(Ĭ��1)
����-n nu������v-SVC��һ��SVM��v- SVR�Ĳ���(Ĭ��0.5)
����-p p������e -SVR ����ʧ����p��ֵ(Ĭ��0.1)
����-m cachesize������cache�ڴ��С����MBΪ��λ(Ĭ��40)
����-e eps�������������ֹ�о�(Ĭ��0.001)
����-h shrinking���Ƿ�ʹ������ʽ��0��1(Ĭ��1)
����-wi weight�����õڼ���Ĳ���CΪweight*C(C-SVC�е�C)(Ĭ��1)
����-v n: n-fold��������ģʽ��nΪfold�ĸ�����������ڵ���2
��������-gѡ���е�k��ָ���������е���������option -v ����ؽ������ʷ�Ϊn��
*/
int main()
{
	//-------------------ѵ������׼��
	//��ȡѵ����ǩ���� (60000,1) ����Ϊint32
	Mat train_labels = read_mnist_label(train_labels_path);
	imwrite("D:/��д�ַ�ʶ��/train_labels.bmp", train_labels);
	//��ȡѵ��ͼ������ (60000,784) ����Ϊfloat32 ����δ��һ��
	Mat train_images = read_mnist_image(train_images_path);
	imwrite("D:/��д�ַ�ʶ��/train_images.bmp", train_images);
	//��ͼ�����ݹ�һ��
	train_images = train_images / 255.0;
	//��ȡ�������ݱ�ǩ(10000,1) ����Ϊint32
	Mat test_labels = read_mnist_label(test_labels_path);
	imwrite("D:/��д�ַ�ʶ��/test_labels.bmp", test_labels);
	//��ȡ��������ͼ�� (10000,784) ����Ϊfloat32 ����δ��һ��
	Mat test_images = read_mnist_image(test_images_path);
	imwrite("D:/��д�ַ�ʶ��/test_images.bmp", test_images);
	//��һ��
	test_images = test_images / 255.0;
	//-------------------����svmѵ��ģ�Ͳ�����ѵ��
	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	//��������ΪC_SVC������ࡪ��>C��֧������������� n����� ��n��2�����������쳣ֵ��������C���в���ȫ���ࡣ
	svm->setType(cv::ml::SVM::C_SVC);
	//���ú˺�������>����ʽ�ںˣ�
	svm->setKernel(cv::ml::SVM::POLY);
	//������������
	svm->setGamma(3);//�ں˺�����POLY���Ĳ���degree//�˺����е�gamma��������(��Զ���ʽ/rbf/sigmoid�˺���)(Ĭ��1/ k)
	svm->setDegree(3);//�ں˺�����POLY/ RBF/ SIGMOID���Ĳ���//�˺����е�degree����(��Զ���ʽ�˺���)(Ĭ��3)
	svm->setCoef0(1);
	svm->setNu(0.5);
	svm->setP(0);
	//���õ�����ֹ���� 
	svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 1000, 0.0001));
	//��ʼѵ��
	cv::Ptr<cv::ml::TrainData> train_data = cv::ml::TrainData::create(train_images, cv::ml::ROW_SAMPLE, train_labels);
	cout << "Start To Train" << endl;
	svm->train(train_data);
	cout << "Finish Training" << endl;
	//-------------------�ڲ������ݼ���Ԥ�����׼ȷ��
	Mat pre_out;
	//����ֵΪ��һ��ͼ���Ԥ��ֵ pre_outΪ����batch��Ԥ��ֵ����
	cout << "Start To Predict" << endl;
	float ret = svm->predict(test_images, pre_out);
	cout << "Finish Predicting" << endl;
	//����׼ȷ�ʱ��뽫���ֱ�ǩ��Ϊͬһ��������
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
	cout << "�������ݼ��ϵ�׼ȷ��Ϊ��" << acc * 100 << "%" << endl;
	//����ģ��
	svm->save("D:/��д�ַ�ʶ��/mnist_svm.xml");
	getchar();
	return 0;
}

int BigToSmall(int i) {
	//mnist��С�˴洢������Ҫת��Ϊ��˴洢������Ҫ���һ������int��32λ��ÿλ�Ķ�����ֵȻ����ת��
	unsigned char c1, c2, c3, c4;//���޷��ŵ�char���Ͷ���ÿ8λ������
	c1 = i & 255;//&����������;255����11111111;��������˼��ȡ��i�ĺ��λ
	c2 = (i >> 8) & 255;//����8λ
	c3 = (i >> 16) & 255;//����16λ
	c4 = (i >> 24) & 255;//����24λ
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
		file.read((char*)&magic_number, sizeof(magic_number));//���ļ���ʽ��
		file.read((char*)&number_of_images, sizeof(number_of_images));//ͼ������
		file.read((char*)&rows, sizeof(rows));//ÿ��ͼ�������
		file.read((char*)&cols, sizeof(cols));//ÿ��ͼ�������
		magic_number = BigToSmall(magic_number);
		number_of_images = BigToSmall(number_of_images);
		rows = BigToSmall(rows);
		cols = BigToSmall(cols);
		cout << "��File Mode��:" << magic_number
			<< " Pixel Num:" << number_of_images
			<< " Rows:" << rows
			<< " Cols:" << cols << endl;
		cout << "Start" << endl;
		DataMat = Mat::zeros(number_of_images, rows * cols, CV_32FC1);
		for (int i = 0; i < number_of_images; i++) {
			for (int j = 0; j < rows * cols; j++) {
				unsigned char temp = 0;
				file.read((char*)&temp, sizeof(temp));//������ʼλ��Ȼ������ÿ8λ�������ض�ȡ
				//������������һ����ÿ������ֵ��һ��
				float pixel_value = float(temp);//�൱��ÿ8λ��һ���Ҷ�ֵ
				//�����н�����ֵһ����д��Mat��
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
		cout << "��File Mode��:" << magic_number << "  ;Label Num:" << number_of_items << endl;
		cout << "Load Label" << endl;
		//CV_32SC1����32λ�з������� ͨ����Ϊ1
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



