## deb 目录结构

> |----DEBIAN
> 
>        |-------control
>                     
>        |-------postinst(postinstallation)
>                     
>        |-------postrm(postremove)
>                     
>        |-------preinst(preinstallation)
>                     
>        |-------prerm(preremove)
>                     
>        |-------copyright(版权)
>                     
>        |-------changlog(修订记录)
>                     
>        |-------conffiles
> 
> |----etc
> 
> |----usr
> 
> |----opt
> 
> |----tmp
> 
> |----boot
> 
>        |-----initrd-vstools.img

## deb 文件结构

| 组成        | 说明                          |
| --------- | --------------------------- |
| 数据包       | 实际要安装的程序数据,data.tar.xxx     |
| 安装信息,控制信息 | 如脚本,安装说明,文档等,control.tar.gz |
| 二进制数据     | 包含文件头等信息,需要特殊软件才能看          |

### 说明

##### control 文件

其实就是各种描述性信息

| 字段 | 用途 | 例子/其他 |
| ---- | ---- | --------- |
|Package	|程序名称|	中间不能有空格|
|Version|	软件版本	| |
|Description|	程序说明|	|
|Section	|软件类别|	utils, net, mail, text, x11|
|Priority|	软件对于系统的重要程度|	required, standard, optional, extra等；|
|Essential|	是否是系统最基本的软件包|	yes/no，若为yes,则不允许卸载（除非强制性卸载）|
|Architecture|	软件所支持的平台架构	|i386, amd64, m68k, sparc, alpha, powerpc等|
|Source	|软件包的源代码名称	 ||
|Depends	|软件所依赖的其他软件包和库文件|	若依赖多个软件包和库文件，采用逗号隔开|
|Pre-Depends|	软件安装前必须安装、||
|配置依赖性的软件包和库文件|	常用于必须的预运行脚本需求||
|Recommends|	推荐安装的其他软件包和库文件	 ||
|Suggests|	建议安装的其他软件包和库文件	||



#### preinst 文件

软件安装前会执行的脚本，比如 stop 程序，解包什么的

#### postinst 文件

程序安装后会执行，比如最后的配置信息，设置环境变量，重启软件等

> \#! /bin/sh          # 必须要以这个开头
> echo "hello world" > /root/mydeb.log

#### prerm 文件

在移除该软件包前需要执行的任务，一般负责停止与该软件包相关联的 daemon 服务

> \#! /bin/sh
> if [[$1==remove]]; then
>     echo "prepare for uninstall xxx"
> fi

#### postrm

软件卸载后执行清理工作，比如修改软件包链接或文件关联，删除由他创建的文件等

> \#! /bin/sh
> rm -rf hello.log



### dpkg 详解

1 >  打包dpkg -b

dpkg -b . mydeb-1.deb

第一个参数为将要打包的目录名（.表示当前目录），第二个参数为生成包的名称<.deb file name>

2 > 安装（解包并配置） dpkg -i|--install <.deb file name>

dpkg -i mydeb-1.deb

//强制安装

dpkg --force-depends -i mydeb-1.deb

解包：
dpkg --unpack mydeb-1.deb

该命令仅对 “mydeb-1.deb”进行解包操作，不会执行包配置工作。

3 > 卸载 dpkg -r|--remove <package> ，删除包，但保留配置文件

dpkg -r my-deb 

dpkg -P|--purge my-deb         

该命令删除包，且删除配置文件。

4 > 查看deb包是否安装/deb包的信息 dpkg -s|--status <package>

dpkg -s my-deb

5 > 查看deb包文件内容 

dpkg -c mydeb-1.deb

6 > 查看当前目录某个deb包的信息

dpkg --info mydeb-1.deb

7 > 解压deb中所要安装的文件

dpkg -x  mydeb-1.deb mydeb-1

第一个参数为所要解压的deb包，第二个参数为将deb包解压到指定的目录

8 >解压deb包中DEBIAN目录下的文件（至少包含control文件）

dpkg -e mydeb-1.deb mydeb-1/DEBIAN

9 > 列出与该包关联的文件 dpkg -L|--listfiles <package>

dpkg -L my-deb 

10 > 配置软件包 dpkg --configure <package>

dpkg --configure my-deb



### 制作 deb 包

准备好可执行的二进制文件，这个二进制文件要可执行，提前要考虑兼容性，如果程序有目录要完整的一个程序目录。 


1->新建软件文件夹 
我们测试名称为JFeng-deb

新建一个名为DEBIAN文件夹 
此文件夹内存放控制信息

在DEBIAN里新建一个文本文档, 名为control, 编码为utf-8, 内容如下所示：

```yml
Package: JFeng
Version: 1.1.0
Architecture: amd64
Section: utils
Priority: optional
Maintainer: MC
Homepage: http://montecarlo.org.cn
Description: Gale debug
```



然后我们创建对应的二进制包安装完成后的路径信息放置在DEBIAN的同级目录下，也就是把当前的目录当成根(“/”)目录,制作完成后安装时，当前目录下除了DEBIAN目录的其他目录都会被默认安装到系统的“/”目录下。

下面是一个程序目录的例子。

```txt
|——JFeng-deb
     |————usr
           |————bin
                 |——可执行文件（安装后，就在你的/usr/bin生成相应的可执行文件）
           |————share
 
                |————icons 
 
                    |——deb.png(图标文件生成到/usr/share/icons/)
 
                |————applications                           
                    |——deb.desktop（桌面文件生成到/usr/share/applications/）
 
     |————DEBIAN(大写、用来制作打包文件)
            |————control(描述deb包的信息必须的文件)
```

完整实验例子目录结构：

```txt
JFeng-deb
├── DEBIAN
│  └── control
├── opt
│  └── JFeng
│      ├── heart
│      └── heart.desktop
└── usr
    ├── bin
    │  └── heart -> /home/wxyz/桌面/JFeng-deb/opt/MyDeb/heart
    └── share
        ├── applications
        │  └── heart.desktop
        └── icons
            └── heart_98.png
```

3->打包

*sudo dpkg -b JFeng-deb/ JFeng-linux-amd64.deb*





正常解压是不会解压出 DEBIAN 目录的，需要单独解压



\#解压出包中的文件到extract目录下
dpkg -X ../openssh-client_6.1p1_i386.deb extract/

\#解压出包的控制信息extract/DEBIAN/下：
dpkg -e ../openssh-client_6.1p1_i386.deb extract/DEBIAN/





