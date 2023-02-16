docker 中编译 docker

docker 采用的是 CS 架构,跟 mysql 的 tcp/ip 连接一样,默认用的是一个 socket(/var/run/docker.sock),因此,一种可行的方法就是保证镜像中包含 docker 客户端就行了,通过-v,将宿主机的 socket 映射到 docker 内部

> docker run -itd -v /var/run/docker.sock:/var/run/docker.sock image_id



看起来像是 docker 运行在 docker 中,实际上容器内仅作为 docker 命令行工具(客户端),而实际的编译任务仍然交给宿主机内的 docker-engine(服务端)



CS 架构的好处大概就是这种了,客户端和服务端可以不同放在一台机器上,这样的灵活性很高



延伸一下:
甚至可以用远程的服务器或者一个云服务器作为 docker-engine
