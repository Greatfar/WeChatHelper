## 前言

* 微信助手（微信加粉计数器），把客服加好友信息传到服务器，用于统计加粉数量，并对客服微信进行监管（防止客服跳单）
* 当前适配 微信 windows 版 `3.6.0.18`

## 项目介绍

* 编译环境为 [Visual Studio 2022](https://visualstudio.microsoft.com/zh-hans/downloads/) ， VS2017以上版本也可以，根据打开时的提示升级依赖，或安装对应的 [Windows SDK](https://developer.microsoft.com/zh-cn/windows/downloads/sdk-archive/)  `10.0.19041.0` ，并使用 `Visual Studio Installer` 安装 `适用于 v143 生成工具的 c++ 14.30 (17.0) MFC`

* `微信 for windows 3.6.0.18` 下载链接（复制到浏览器打开）：[http://file.greatfar.cn/down/HSdyx2rDrL0y](http://file.greatfar.cn/down/HSdyx2rDrL0y)

* 网络请求基于 [libcurl 7.83.1](https://curl.se/)

* 项目分为两个端，WeChatHelper 和 WeChatTools WeChatHelper 作为客户端负责和服务端进行通信，将服务端传回的数据显示到界面。 WeChatTools 作为服务端，注入到微信进程，进行取数据和HOOK的相关操作，并且将取回的数据发回给客户端。

* 客户端和服务端之间采用 `WM_COPYDATA` 的方式（windows message 消息机制）进行进程通讯，互相传输数据。

* 由于使用了内联汇编，`debug` 和 `release` 都需要使用 `x86` 模式，不要使用 `x64`，否则会报错 `使用了非标准扩展不支持在此结构上使用_asm关键字`

![_asm](assets/asm.png)

* 运行环境，Visual C++ 桌面开发

![_asm](assets/dev.png)

## 效果演示

下面演示部分效果

### 初始化

将 WeChatHelper.exe 、 WeChatTools.dll 、libcurl.dll 、config.ini 放在同一个目录下，打开 WeChatHelper.exe 会自动唤起微信，登录微信即可

![1563680573456](assets/main.png)

## 声明

**本项目仅供技术研究，请勿用于任何商业用途，请勿用于非法用途，如有任何人凭此做何非法事情，均与作者无关，特此声明。**
