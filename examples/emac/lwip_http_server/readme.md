
## How use

运行下面的命令，编译本例程

```bash

$ make APP=lwip_http_server

```

编译无误后，将程序下载到芯片，运行后程序即可启动一个 **Web server** 服务，默认测试静态 IP 是 **192.168.1.221**，端口是 http 专用的 **80** 端口

在不同环境下进行测试，IP 地址等网络环境需要自行修改为你自己的测试环境，端口的定义在 **http_server.h** 中

保证网络配置正确后，下载到 BL706 中运行，使用浏览器输入测试 IP 地址，即可访问 Web 网页，点击开关按钮即可控制 GPIO6 上的 LED 亮灭。