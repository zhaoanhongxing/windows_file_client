// windows_client.cpp : 定义控制台应用程序的入口点。
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

    // 所有asio类都需要io_service对象
    io_service iosev;
    // socket对象
    ip::tcp::socket socket(iosev);
    // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
    ip::tcp::endpoint ep(ip::address_v4::from_string("192.168.1.3"), 1000);
    // 连接服务器
    boost::system::error_code ec;
    socket.connect(ep,ec);
    // 如果出错，打印出错信息
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

	//打开文件
	openFile(strFilePath,&pFd);
	dFileSize = getFileSize(pFd);
	cout<< strFilePath<<"size:"<<dFileSize<<endl;//文件大小

	//初始化块数据
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
		cout<<"发送第"<<i++<<"块"<<endl;
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


    // 接收数据
    //char buf[100];
    //size_t len = socket.read_some(buffer(buf), ec);
    //std::cout.write(buf, len);

    return 0;
}

