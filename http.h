//
// Created by 32119 on 2021/1/30.
//

// 头文件的作用: 防止头文件重复包含
#ifndef INC_1_30_HTTP_H
#define INC_1_30_HTTP_H

#include <string>

class CHttp
{
public:
    std::string m_host; // 域名
    std::string m_object; // 资源路径
    SOCKET m_socket; // 客户端套接字
public:
    bool AnalyseUrl(std::string url); // 定义一个公开的成员方法，解析url
    bool InitSock(); // 初始化网络
    bool Connect(); // 定义一个公开的成员方法， 连接网络
    bool GetHtml(std::string &html); // 获取网页内容
};

#endif //INC_1_30_HTTP_H
