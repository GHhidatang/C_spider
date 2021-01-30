#include <iostream>
#include <windows.h>
#include <string>
#include <queue> // C++队列的库 C哭了

#include "http.h"

using namespace std;

// 开始抓取函数的声明
bool StartCatch(string url);

int main()
{
    cout << "*********************************" << endl;
    cout << "*\t欢迎使用网络爬虫系统\t*" << endl;
    cout << "*\t  当前版本: 1.0\t\t*" << endl;
    cout << "*\tCopyRight @ 2021\t*" << endl;
    cout << "*********************************" << endl;

    // 在当前程序的位置创建用来存放爬虫数据的文件夹
    CreateDirectory("./Data", NULL);

    cout << "请输入您需要抓取的网页起始URL:\n";
    string url; // c没有的字符串类型string
    cin >> url;

    StartCatch(url);

    system("pause");
    return 0;
}

// 开始抓取
bool StartCatch(string url)
{
    // c++的STL标准模板库编程
    queue<string> q; // URL队列 表示队列的元素都是string(字符串)类型
    q.push(url); // 把传过来的起始URL推入队列

    while (!q.empty()) // q.empty()是用来判断队列q是否为空 队列q为空返回真
    {
        string currentUrl = q.front(); // 从队列q中拿一条URL赋值给currentUrl
        q.pop(); // 从队列q中弹出刚才那条URL

        // 解析URL
        // http://www.dm5.com/m606296/#ipg1

        CHttp http;
        http.AnalyseUrl(url);
        if (false == http.Connect())
        {
            cout << "连接失败" << endl;
        } else
        {
            cout << "连接成功" << endl;
        }

        string html;
        http.GetHtml(html);

        cout << html << endl;
    }

    return true;
}
