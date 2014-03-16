// windows_client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "fileData.h"

#include<iostream>
#include<string>
#include<boost/asio.hpp>
#include<boost/array.hpp>

using boost::asio::ip::tcp;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    using namespace boost::asio;

    // ����asio�඼��Ҫio_service����
    io_service iosev;
    // socket����
    ip::tcp::socket socket(iosev);
    // ���Ӷ˵㣬����ʹ���˱������ӣ������޸�IP��ַ����Զ������
    ip::tcp::endpoint ep(ip::address_v4::from_string("192.168.1.3"), 1000);
    // ���ӷ�����
    boost::system::error_code ec;
    socket.connect(ep,ec);
    // ���������ӡ������Ϣ
    if(ec)
    {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return -1;
    }
	
	FILE *pFd = NULL;
	double dFileSize;
	char strFilePath[255];
	cout<<"enter filePath:";
	cin>>strFilePath;

	//���ļ�
	openFile(strFilePath,&pFd);
	dFileSize = getFileSize(pFd);
	cout<< strFilePath<<"size:"<<dFileSize<<endl;//�ļ���С

	//��ʼ��������
	fileTile p;
	strcpy(p.fileName,strFilePath);
	p.fileSize = dFileSize;
	p.index = 1;
	memset(p.tileData,0, sizeof(p.tileData));
	int  tileSize = fread(p.tileData, 1,sizeof(p.tileData), pFd);
	p.tileSize = tileSize;
	size_t sendLen = socket.write_some(buffer((void *)&p,sizeof(p)),ec);
	memset(p.tileData,0, sizeof(p.tileData));

	double curSize = tileSize;
	int i=0;
	while (curSize < p.fileSize)
	{
		cout<<"���͵�"<<i++<<"��"<<endl;
		p.tileSize = fread(p.tileData, 1,sizeof(p.tileData), pFd);
		sendLen = socket.write_some(buffer((void *)&p,sizeof(p)),ec);
		memset(p.tileData,0, sizeof(p.tileData));
		//Sleep(10);
		if (sendLen != sizeof(p))
		{
			cout<<"error!"<<endl;
			return 0;
		}
		curSize += tileSize;
		char str[3];
		socket.read_some(buffer(str),ec);
		if(strcmp(str,"ok") != 0)
		{
			cout<<"stop send msg\n";
			return 0;
		}

	}


    // ��������
    //char buf[100];
    //size_t len = socket.read_some(buffer(buf), ec);
    //std::cout.write(buf, len);

    return 0;
}

