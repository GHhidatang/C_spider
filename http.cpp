//
// Created by 32119 on 2021/1/30.
//

#include <iostream>
#include <WinSock2.h>
#include "http.h"
#pragma comment(lib, "ws2_32.lib")

bool CHttp::AnalyseUrl(std::string url)
{
    // http://www.dm5.com/m606296/#ipg1
    // 字符串截取,分割
    if (url.empty()) // ""
        return false;

    if (url.length() <= 8) // http://
        return false;

    int pos = url.find("https://");
    if (pos == std::string::npos)
        return false;

    pos = url.find('/', 8);
    if (pos == std::string::npos)
    {
        CHttp::m_host = url.substr(8);
        CHttp::m_object = "/";
    } else
    {
        CHttp::m_host = url.substr(8, pos-8);
        CHttp::m_object = url.substr(pos);
    }

    if (m_host.empty())
        return false;

    std::cout << "域名:" << CHttp::m_host << "\t" << "资源:" << CHttp::m_object << std::endl;

    return true;
}

bool CHttp::InitSock()
{
    WSADATA wd;
    if (0 != WSAStartup(MAKEWORD(2,2), &wd))
        return false;

    // 判断加载过来的套接字库版本是否一致
    if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
        return false;

    // 创建套接字
    /*
     * AF_INET: IPv4
     * SOCK_STREAM: TCP
     * */
    CHttp::m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET)
        return false;


    return true;
}

bool CHttp::Connect()
{
    // 初始化网络
    if (false == CHttp::InitSock())
        return false;

    // 解析IP地址
    hostent * host = gethostbyname(CHttp::m_host.c_str()); // c_str()把string 对象转换成c中的字符串样式
    if (host == NULL)
        return false;

    // 连接web服务器
    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(80);
    memcpy(&sa.sin_addr, host->h_addr, 4);

    if (SOCKET_ERROR == connect(CHttp::m_socket, (sockaddr *)&sa, sizeof(sockaddr)))
        return false;



    return true;
}

bool CHttp::GetHtml(std::string &html)
{
//    GET https://www.baidu.com/sugrec?cb=jQuery111105730925087161411_1612008586009&ie=utf-8&wd=%E7%90%A6%E7%8E%89&prod=open_image&t=0.9794714874573169&_=1612008586012 HTTP/1.1
//    Host: www.baidu.com
//    Connection: keep-alive
//    User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.104 Safari/537.36
//    Accept: */*
//    Sec-Fetch-Site: same-site
//    Sec-Fetch-Mode: no-cors
//    Sec-Fetch-Dest: script
//    Referer: https://image.baidu.com/
//    Accept-Encoding: gzip, deflate, br
//    Accept-Language: zh-CN,zh;q=0.9
//    Cookie: BAIDUID=623B2880BCD1DA39922A6EF5836B60B8:FG=1; BIDUPSID=CD1E33625CB5B6419EB0BE5A79B882F6; PSTM=1595603807; BDUSS=03VEdlVGxtdHRrN35BZmltVkFDQzE5MFdPSHNvdDJvamxrbHRDMGVxOUZOWWxmSVFBQUFBJCQAAAAAAAAAAAEAAAB92EXzd29haXd1a3VvbmcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEWoYV9FqGFfM2; BDUSS_BFESS=03VEdlVGxtdHRrN35BZmltVkFDQzE5MFdPSHNvdDJvamxrbHRDMGVxOUZOWWxmSVFBQUFBJCQAAAAAAAAAAAEAAAB92EXzd29haXd1a3VvbmcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEWoYV9FqGFfM2; BD_UPN=12314753; __yjs_duid=1_f84dbea44ec6d1db86d9e68db12f6f831611676252932; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; H_WISE_SIDS=107313_110085_127969_131423_144966_156286_156929_159936_161568_162003_162371_162898_163390_163566_163568_163929_164164_164297_164635_164955_165134_165136_165328_165583_165736_166148_166184_166919_167303_167321_167422_167537_167671_168030_168034_168097_168113_168343_168394_168402_168540_168564_168572_168617_168621_168629_168647_168719_168796; plus_cv=1::m:7.94e+147; H_PS_PSSID=33425_33513_33582_33260_33272_31253_33459_26350_22158; delPer=0; BD_CK_SAM=1; PSINO=6; BAIDUID_BFESS=623B2880BCD1DA39922A6EF5836B60B8:FG=1; COOKIE_SESSION=5456_0_9_6_5_2_0_0_9_2_1_0_84086_0_1_0_1611995730_0_1611995729%7C9%2325_221_1611739021%7C9; ZD_ENTRY=baidu; H_PS_645EC=9b9c6xDq%2FJF1vubJe0TPX4s1tSbtFEtq6pZxOuPYF5v4e%2FETdNmtL59Q64y4B9m31oh1; BD_HOME=1; sugstore=1; BA_HECTOR=2lagag2lah84alak1u1g1aj3t0r; BDRCVFR[feWj1Vr5u3D]=I67x6TjHwwYf0; BDSVRTM=22; BDRCVFR[dG2JNJb_ajR]=mk3SLVN4HKm; BDRCVFR[X_XKQks0S63]=mk3SLVN4HKm; userFrom=www.baidu.com; ab_sr=1.0.0_YzhhNWM2NDQ3NzVkZDUzMjIxNWRiMWNiYTc0NDI1MWNlN2RmMTA5ZGM0MTdjYzQ3ZmNiOTg4MTE3Yzg4OTdkMjc1ZTk3NDgwNmI3YzgyMzhkYWE4OTAzMWJkMDM3YmQzZWRkOWIwMGMwYmExOGQ1MjVmYzQ3NTQ4MDRiODcwYWI=

    std::string info;
    info = info + "GET " + CHttp::m_object + " HTTP/1.1\r\n";
    info = info + "Host" + CHttp::m_host + "\r\n";
    info = info + "Connection: Close\r\n";
    info = info + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.104 Safari/537.36\r\n";
    info = info + "\r\n";

    if (SOCKET_ERROR == send(CHttp::m_socket, info.c_str(), info.length(), 0))
        return false;

    // 接收数据
    char ch = 0;
    while (recv(m_socket, &ch, sizeof(ch), 0))
    {
        html += ch;
    }

    return true;
}
