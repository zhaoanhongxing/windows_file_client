#include<iostream>
#include<string>
using namespace std;
#pragma pack(1)

struct fileTile
{
	char fileName[255];//�ļ���
	double fileSize;//�ļ���С
	double index;//���λ��
	int tileSize;//���С
	char tileData[1024];
};

void openFile(char* pFilePath ,FILE **pFd);//���ļ�
double getFileSize(FILE *pFd);//��ȡ�ļ���С
