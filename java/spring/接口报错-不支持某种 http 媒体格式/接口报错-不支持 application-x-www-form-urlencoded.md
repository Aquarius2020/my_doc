

Resolved [org.springframework.web.HttpMediaTypeNotSupportedException: Content type 'application/x-www-form-urlencoded;charset=UTF-8' not supported]



基本知识

| Content-Type           | 说明                       | 案例                           | 备注                                                   |
| ---------------------- | -------------------------- | ------------------------------ | ------------------------------------------------------ |
| 请求头中的content-type | 描述请求实体对应的MIME信息 | Content-Type: application/json | 简单理解就是：客户端告诉服务端，我传的数据是什么类型   |
| 响应头中的content-type | 描述响应实体对应的MIME信息 | text/html; charset=UTF8        | 简单理解就是：服务端告诉客户端，我返回的数据是什么类型 |

媒体类型（通常称为 Multipurpose Internet Mail Extensions 或 MIME 类型 ）是一种标准，用来表示文档、文件或字节流的性质和格式。它在IETF RFC 6838中进行了定义和标准化。

## MIME有哪些类型

| 类型        | 描述                                                         | 典型案例                                                     |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| text        | 表明文件是普通文本，理论上是人类可读                         | text/plain, text/html, text/css, text/javascript             |
| image       | 表明是某种图像。不包括视频，但是动态图（比如动态gif）也使用image类型 | image/gif, image/png, image/jpeg, image/bmp, image/webp, image/x-icon, image/vnd.microsoft.icon |
| audio       | 表明是某种音频文件                                           | audio/midi, audio/mpeg, audio/webm, audio/ogg, audio/wav     |
| video       | 表明是某种视频文件                                           | video/webm, video/ogg                                        |
| application | 表明是某种**二进制数据**                                     | application/octet-stream, application/pkcs12, application/vnd.mspowerpoint,  application/xhtml+xml, application/xml, application/pdf |

对于text文件类型若没有特定的subtype，就使用 text/plain。类似的，二进制文件没有特定或已知的 subtype，即使用 application/octet-stream。

application/octet-stream通常与标识二进制文件，常见的场景是：上传文件； 服务端在处理这种类型的MIME时通常就是把数据流写入文件。

## document.contentType

浏览器在请求资源成功后，会把当前页面对应链接的content-type挂载到document.contentType中

比如：

1、请求一个网站后，访问document.contentType 可以得到 text/html 2、请求一个css文件，访问document.contentType 可以得到 text/css  3、请求一个js文件，访问document.contentType 可以得到 application/javascript



回到问题的开始,当前请求不支持 xxx contentType. 说明, 服务器不支持前端请求 xxx 格式.  那么,就该看看 接口是怎么定义的

特别是接口中的 `@RequestMapping`

```java
@RequestMapping(
  value= {"/hello"},
  comsumes ={"application/x-www-form-urlencoded"},			// 只接受指定类型的参数
  produces = { "text/csv", "application/json" }					// 只允许指定类型的返回
)
void test(){
  
}
```

报错,说明入参和指定类型不匹配,修改一下类型就行了





























## 常见的 Content-Type 讲解



## application/x-www-form-urlencoded

用于POST请求提交数据的格式之一。

常用于FORM表单提交，如果不设置enctype属性,默认为application/x-www-form-urlencoded方式提交数据。

x-www-form-urlencoded有这几个特性：

- Content-Type都指定为application/x-www-form-urlencoded;
- 提交的表单数据会转换为键值对并按照key1=val&key2=val2的方式进行编码,key和val都进行了URL转码。大部分服务端语言都对这种方式有很好的支持。

另外,如利用AJAX提交数据时,也可使用这种方式。例如在jQuery中,Content-Type默认值都是"application/x-www-form-urlencoded;charset=utf-8"。



## application/json

用于POST请求提交数据的格式之一。

application/json在响应头和请求头中都很常见，消息主体是序列化后的JSON字符串。

```js
fetch('url', {
    method: 'POST',
    body: JSON.stringify({
        xxx: 'xxx',
    }),
    header: {
        'Content-Type': 'application/json',
    },
});
```

## text/plain

如果没有指定响应头中的content-type，那么content-type默认就是text/plain，纯文本，浏览器中会直接以文本形式展示出来。

## multipart类型

multipart中有的重要类型：  *multipart/form-data*  multipart/byteranges

### multipart/form-data

这个是老生常谈的话题，multipart/form-data通常用在需要文件上传的场景：

- HTML FORM：表单提交很常见，通常在需要上传文件时，会把FORM的enctype设置为multipart/form-data
- AjAX POST：提交异步POST请求时配合 formData 使用

### multipart/byteranges

这个类型通常用来标明当前的数据是分片传输的，分片传输





















