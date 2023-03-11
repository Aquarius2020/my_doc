

tar 解压重命名

> 如文件 abc.v1.tar 解压后是 abc.v1/xxx
> tar -C abc 发现变成 abc/abc.v1

其实想要的是 abc,并且跳过原来的 abc.v1 目录

mkdir abc
tar -xf **--strip-components=1** abc.v1.tar -C abc

> –strip-components=NUMBER 解压时从文件名中清除 NUMBER个引导部分
> –strip-component=1 代表解压出来的文件，剥离前一个路径
> –strip-component=2 代表解压出来的文件，剥离前两个路径


