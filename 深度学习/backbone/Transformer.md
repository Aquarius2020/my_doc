[The Illustrated Transformer – Jay Alammar – Visualizing machine learning one concept at a time. (jalammar.github.io)](https://jalammar.github.io/illustrated-transformer/)







Transformer是一个利用注意力机制来提高模型训练速度的模型。关于注意力机制可以参看[这篇文章](https://zhuanlan.zhihu.com/p/52119092)，trasnformer可以说是完全基于自注意力机制的一个深度学习模型，因为它适用于并行化计算，和它本身模型的复杂程度导致它在精度和性能上都要高于之前流行的RNN循环神经网络。

那什么是transformer呢？

你可以简单理解为它是一个黑盒子，当我们在做文本翻译任务是，我输入进去一个中文，经过这个黑盒子之后，输出来翻译过后的英文。

![img](pic/Transformer/v2-1a4f5b236563d6307acb58cc5a95b2b7_1440w.webp)

那么在这个黑盒子里面都有什么呢？

里面主要有两部分组成：Encoder 和 Decoder

![img](pic/Transformer/v2-8bf3b3ac8836ef1a9f16e1669fb29511_1440w.webp)

当我输入一个文本的时候，该文本数据会先经过一个叫Encoders的模块，对该文本进行编码，然后将编码后的数据再传入一个叫Decoders的模块进行解码，解码后就得到了翻译后的文本，对应的我们称Encoders为编码器，Decoders为解码器。

那么编码器和解码器里边又都是些什么呢？

细心的同学可能已经发现了，上图中的Decoders后边加了个s，那就代表有多个编码器了呗，没错，这个编码模块里边，有很多小的编码器，一般情况下，Encoders里边有6个小编码器，同样的，Decoders里边有6个小解码器。

![img](pic/Transformer/v2-739d9498e0a36296240741be909d35f7_1440w.webp)

我们看到，在编码部分，每一个的小编码器的输入是前一个小编码器的输出，而每一个小解码器的输入不光是它的前一个解码器的输出，还包括了整个编码部分的输出。

那么你可能又该问了，那每一个小编码器里边又是什么呢？

我们放大一个encoder，发现里边的结构是一个自注意力机制加上一个前馈神经网络。

![img](pic/Transformer/v2-8c63aaf7e71b94fdb5d6df89abdaf118_1440w.webp)

我们先来看下self-attention是什么样子的。

我们通过几个步骤来解释：

1、首先，self-attention的输入就是词向量，即整个模型的最初的输入是词向量的形式。那自注意力机制呢，顾名思义就是自己和自己计算一遍注意力，即对每一个输入的词向量，我们需要构建self-attention的输入。在这里，transformer首先将词向量乘上三个矩阵，得到三个新的向量，之所以乘上三个矩阵参数而不是直接用原本的词向量是因为这样增加更多的参数，提高模型效果。对于输入X1(机器)，乘上三个矩阵后分别得到Q1,K1,V1，同样的，对于输入X2(学习)，也乘上三个不同的矩阵得到Q2,K2,V2。

![img](pic/Transformer/v2-15142b393f03a309c926754f00307d46_1440w.webp)

2、那接下来就要计算注意力得分了，这个得分是通过计算Q与各个单词的K向量的点积得到的。我们以X1为例，分别将Q1和K1、K2进行点积运算，假设分别得到得分112和96。

![img](pic/Transformer/v2-42ccd93ac7540619b02ef03faef21c15_1440w.webp)

3、将得分分别除以一个特定数值8（K向量的维度的平方根，通常K向量的维度是64）这能让梯度更加稳定，则得到结果如下：

![img](pic/Transformer/v2-8a98e66c20fb25e96e1f690309ae6166_1440w.webp)

4、将上述结果进行softmax运算得到，softmax主要将分数标准化，使他们都是正数并且加起来等于1。

![img](pic/Transformer/v2-1701b674a3e09ae91301d6cd9727f912_1440w.webp)

5、将V向量乘上softmax的结果，这个思想主要是为了保持我们想要关注的单词的值不变，而掩盖掉那些不相关的单词（例如将他们乘上很小的数字）

![img](pic/Transformer/v2-c18a30a6b8738af5cd1b5c0e2080e695_1440w.webp)

6、将带权重的各个V向量加起来，至此，产生在这个位置上（第一个单词）的self-attention层的输出，其余位置的self-attention输出也是同样的计算方式。

![img](pic/Transformer/v2-3577071e71ccfa49a4f60f4a5187f0ce_1440w.webp)

将上述的过程总结为一个公式就可以用下图表示：

![img](pic/Transformer/v2-0190eb46d1c46efc04926821e69fd377_1440w.webp)

self-attention层到这里就结束了吗？

还没有，论文为了进一步细化自注意力机制层，增加了“多头注意力机制”的概念，这从两个方面提高了自注意力层的性能。

第一个方面，他扩展了模型关注不同位置的能力，这对翻译一下句子特别有用，因为我们想知道“it”是指代的哪个单词。

![img](pic/Transformer/v2-dc386abf38141384c43918689b0bbb64_1440w.webp)

第二个方面，他给了自注意力层多个“表示子空间”。对于多头自注意力机制，我们不止有一组Q/K/V权重矩阵，而是有多组（论文中使用8组），所以每个编码器/解码器使用8个“头”（可以理解为8个互不干扰自的注意力机制运算），每一组的Q/K/V都不相同。然后，得到8个不同的权重矩阵Z，每个权重矩阵被用来将输入向量投射到不同的表示子空间。

经过多头注意力机制后，就会得到多个权重矩阵Z，我们将多个Z进行拼接就得到了self-attention层的输出：

![img](pic/Transformer/v2-1be30f537678c89b2768ed31ff5bb491_1440w.webp)

上述我们经过了self-attention层，我们得到了self-attention的输出，self-attention的输出即是前馈神经网络层的输入，然后前馈神经网络的输入只需要一个矩阵就可以了，不需要八个矩阵，所以我们需要把这8个矩阵压缩成一个，我们怎么做呢？只需要把这些矩阵拼接起来然后用一个额外的权重矩阵与之相乘即可。

![img](pic/Transformer/v2-7394f6eb418b403588b0ca5a6751749f_1440w.webp)

最终的Z就作为前馈神经网络的输入。

接下来就进入了小编码器里边的前馈神经网模块了，关于前馈神经网络，网上已经有很多资料，在这里就不做过多讲解了，只需要知道，前馈神经网络的输入是self-attention的输出，即上图的Z,是一个矩阵，矩阵的维度是（序列长度×D词向量），之后前馈神经网络的输出也是同样的维度。

![img](pic/Transformer/transformer_multi-headed_self-attention-recap.png)



以上就是一个小编码器的内部构造了，一个大的编码部分就是将这个过程重复了6次，最终得到整个编码部分的输出。

然后再transformer中使用了6个encoder，为了解决梯度消失的问题，在Encoders和Decoder中都是用了残差神经网络的结构，即每一个前馈神经网络的输入不光包含上述self-attention的输出Z，还包含最原始的输入。

上述说到的encoder是对输入（机器学习）进行编码，使用的是自注意力机制+前馈神经网络的结构，同样的，在decoder中使用的也是同样的结构。也是首先对输出（machine learning）计算自注意力得分，不同的地方在于，进行过自注意力机制后，将self-attention的输出再与Decoders模块的输出计算一遍注意力机制得分，之后，再进入前馈神经网络模块。

![img](pic/Transformer/v2-5e32534b9a651289cb3eb2b409d5996b_1440w.webp)

以上，就讲完了Transformer编码和解码两大模块，那么我们回归最初的问题，将“机器学习”翻译成“machine learing”，解码器输出本来是一个浮点型的向量，怎么转化成“machine learing”这两个词呢？

是个工作是最后的线性层接上一个softmax，其中线性层是一个简单的全连接神经网络，它将解码器产生的向量投影到一个更高维度的向量（logits）上，假设我们模型的词汇表是10000个词，那么logits就有10000个维度，每个维度对应一个惟一的词的得分。之后的softmax层将这些分数转换为概率。选择概率最大的维度，并对应地生成与之关联的单词作为此时间步的输出就是最终的输出啦！！

假设词汇表维度是6，那么输出最大概率词汇的过程如下：

![img](pic/Transformer/v2-6d0a0d38ab824914942121d1ae78cd0b_1440w.webp)

以上就是Transformer的框架了，但是还有最后一个问题，我们都是到RNN中的每个输入是时序的，是又先后顺序的，但是Transformer整个框架下来并没有考虑顺序信息，这就需要提到另一个概念了：“位置编码”。

Transformer中确实没有考虑顺序信息，那怎么办呢，我们可以在输入中做手脚，把输入变得有位置信息不就行了，那怎么把词向量输入变成携带位置信息的输入呢？

我们可以给每个词向量加上一个有顺序特征的向量，发现sin和cos函数能够很好的表达这种特征，所以通常位置向量用以下公式来表示：

![img](pic/Transformer/v2-a671b951ef42d09c349db12c35175998_1440w.webp)

![img](pic/Transformer/v2-c17ebc4594bd0c0d01fab289abde5ec4_1440w.webp)

最后祭出这张经典的图，最初看这张图的时候可能难以理解，希望大家在深入理解Transformer后再看这张图能够有更深刻的认识。

![img](pic/Transformer/v2-1d9129c9c0d5367591bd093f79155e40_1440w.webp)

![img](pic/Transformer/v2-8fbde14eac35db43cfe1734d4714a7db_1440w.webp)

![img](pic/Transformer/v2-981880051b73b35f68db5ccf1277917e_1440w.webp)

Transformer就介绍到这里了，后来的很多经典的模型比如BERT、GPT-2都是基于Transformer的思想。我们有机会再详细介绍这两个刷新很多记录的经典模型。