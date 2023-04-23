# CRC(循环冗余校验)

【参考资料】

https://en.wikipedia.org/wiki/Cyclic_redundancy_check

https://zh.wikipedia.org/wiki/%E5%BE%AA%E7%92%B0%E5%86%97%E9%A4%98%E6%A0%A1%E9%A9%97

https://wiki.segger.com/CRC

https://info.support.huawei.com/info-finder/encyclopedia/zh/CRC.html

https://segmentfault.com/a/1190000018094567

# 1.字节序

字节序:https://zh.wikipedia.org/wiki/%E5%AD%97%E8%8A%82%E5%BA%8F

**字节顺序**，又称**端序**或**尾序**（英语：**Endianness**），在[计算机科学](https://zh.wikipedia.org/wiki/计算机科学)领域中，指[电脑内存](https://zh.wikipedia.org/wiki/存储器)中或在数字通信链路中，组成[多字节的字](https://zh.wikipedia.org/wiki/字_(计算机))的[字节](https://zh.wikipedia.org/wiki/字节)的排列顺序。

在几乎所有的机器上，多字节对象都被存储为连续的字节序列。例如在[C语言](https://zh.wikipedia.org/wiki/C语言)中，一个类型为`int`的变量`x`地址为`0x100`，那么其对应地址表达式`&x`的值为`0x100`。且`x`的四个字节将被存储在[电脑内存](https://zh.wikipedia.org/wiki/存储器)的`0x100, 0x101, 0x102, 0x103`位置。[[1\]](https://zh.wikipedia.org/wiki/字节序#cite_note-1)

## 1.1 大小端

字节的排列方式有两个通用规则。例如，将一个多位数的低位放在较小的地址处，高位放在较大的地址处，则称**小端序**；反之则称**大端序**。在网络应用中，字节序是一个必须被考虑的因素，因为不同机器类型可能采用不同标准的字节序，所以均按照网络标准转化。

```
变量x类型为int值为0x01234567
		小端序		  大端序
0x100	0x67		0x01
0x101	0x45		0x23
0x102	0x23		0x45
0x103	0x01		0x67
	
	0x0A0B0C0D
大端序：
	--------->	地址增长方向
....|0x0A|0x0B|0x0C|0x0D|....

小端序：
	--------->	地址增长方向
....|0x0D|0x0C|0x0B|0x0A|....
	<---------	地址增长方向
....|0x0A|0x0B|0x0C|0x0D|....
最低有效位（LSB）是0x0D 存储在最低的内存地址处

混合序：32bit在PDP-11的存储方式
	--------->	地址增长方向
....|0x0B|0x0A|0x0D|0x0C|....
高16bit和低16bit以大端序存储，但16bit内部以小端存储


```

## 1.2 处理器体系

x86、MOS Technology 6502、Z80、VAX、PDP-11等处理器为小端序；
Motorola 6800、Motorola 68000、PowerPC 970、System/370、SPARC（除V9外）等处理器为大端序；
ARM、PowerPC（除PowerPC 970外）、DEC Alpha、SPARC V9、MIPS、PA-RISC及IA64的字节序是可配置的。

## 1.3 网络序

网络传输一般采用大端序，也被称之为网络字节序，或网络序。IP协议中定义大端序为网络字节序。

Berkeley套接字定义了一组转换函数，用于16和32bit整数在网络序和本机字节序之间的转换。htonl，htons用于本机序转换到网络序；ntohl，ntohs用于网络序转换到本机序。

## 1.4 位序

小端序（先传低位）的串行协议

- [RS-232](https://zh.wikipedia.org/wiki/RS-232)
- [RS-422](https://zh.wikipedia.org/wiki/RS-422)
- [RS-485](https://zh.wikipedia.org/wiki/RS-485)
- [USB](https://zh.wikipedia.org/wiki/USB)
- [以太网](https://zh.wikipedia.org/wiki/以太网)（虽然高字节先传，但每一字节内低位先传）

大端序（先传高位）的串行协议

- [I2C](https://zh.wikipedia.org/wiki/I2C)协议
- [SPI](https://zh.wikipedia.org/wiki/序列周邊介面)协议
- [摩尔斯电码](https://zh.wikipedia.org/wiki/摩尔斯电码)

# 2.循环冗余校验

https://zh.wikipedia.org/wiki/%E5%BE%AA%E7%92%B0%E5%86%97%E9%A4%98%E6%A0%A1%E9%A9%97



**循环冗余校验**( CRC **)**是一种[错误检测代码，](https://en.wikipedia.org/wiki/Error_correcting_code)常用于数字[网络](https://en.wikipedia.org/wiki/Telecommunications_network)和存储设备，用于检测数字数据的意外更改。进入这些系统的数据块会根据其内容的[多项式除法的余数附加一个短](https://en.wikipedia.org/wiki/Polynomial_long_division)*校验值。*在检索时，重复计算，如果检查值不匹配，则可以采取纠正措施来防止数据损坏。CRC 可用于[纠错](https://en.wikipedia.org/wiki/Error_detection_and_correction)（请参阅[位过滤器](https://en.wikipedia.org/wiki/Mathematics_of_cyclic_redundancy_checks#Bitfilters)）。[[1\]](https://en.wikipedia.org/wiki/Cyclic_redundancy_check#cite_note-1)

之所以称为 CRC，是因为*校验*（数据验证）值是一种*冗余（它在不添加*[信息的情况](https://en.wikipedia.org/wiki/Entropy_(information_theory))下扩展消息）并且该[算法](https://en.wikipedia.org/wiki/Algorithm)基于[*循环*码](https://en.wikipedia.org/wiki/Cyclic_code)。[CRC 很受欢迎，因为它们易于在二进制硬件](https://en.wikipedia.org/wiki/Computer_hardware)中实现，易于进行数学分析，并且特别擅长检测由传输通道中的[噪声引起的常见错误。](https://en.wikipedia.org/wiki/Noise_(electronics))因为校验值的长度是固定的，所以生成它的[函数偶尔会被用作](https://en.wikipedia.org/wiki/Function_(mathematics))[哈希函数](https://en.wikipedia.org/wiki/Hash_function)。

当其校验值是*n*位长时，CRC 称为*n*位 CRC 。对于给定的*n*，可能有多个 CRC，每个都有不同的多项式。这样的多项式具有最高次数*n*，这意味着它有*n* + 1项。换句话说，多项式的长度为*n* + 1；它的编码需要*n* + 1位。请注意，**大多数多项式规范会丢弃[MSB](https://en.wikipedia.org/wiki/Most_significant_bit)或[LSB](https://en.wikipedia.org/wiki/Least_significant_bit)，因为它们始终为 1。**

最简单的错误检测系统，即[奇偶校验位](https://en.wikipedia.org/wiki/Parity_bit)，实际上是一个 1 位的 CRC：它使用生成多项式 *x* + 1（两项），[[3\]](https://en.wikipedia.org/wiki/Cyclic_redundancy_check#cite_note-Ergen-2008-3)并命名为 CRC-1。

## 2.1 数据完整性

CRC 专门设计用于防止通信通道上的常见错误类型，它们可以为所传递消息的[完整性](https://en.wikipedia.org/wiki/Data_integrity)提供快速和合理的保证。但是，它们不适用于防止故意更改数据。

首先，由于没有身份验证，攻击者可以编辑消息并重新计算 CRC，而不会检测到替换。当与数据一起存储时，CRC 和加密哈希函数本身并不能防止对数据的*故意修改。*任何需要防止此类攻击的应用程序都必须使用密码身份验证机制，例如[消息身份验证代码](https://en.wikipedia.org/wiki/Message_authentication_code)或[数字签名](https://en.wikipedia.org/wiki/Digital_signatures)（通常基于[密码哈希](https://en.wikipedia.org/wiki/Cryptographic_hash)函数）。

## 2.2 计算

In this example, we shall encode 14 bits of message with a 3-bit CRC, with a polynomial *x*3 + *x* + 1. The polynomial is written in binary as the coefficients; a 3rd-degree polynomial has 4 coefficients (1*x*3 + 0*x*2 + 1*x* + 1). In this case, the coefficients are 1, 0, 1 and 1. The result of the calculation is 3 bits long, which is why it is called a 3-bit CRC. However, you need 4 bits to explicitly state the polynomial.

Start with the message to be encoded:

```
11010011101100
```

This is first padded with zeros corresponding to the bit length *n* of the CRC. This is done so that the resulting code word is in [systematic](https://en.wikipedia.org/wiki/Systematic_code) form. Here is the first calculation for computing a 3-bit CRC:

```
11010011101100 000 <--- input right padded by 3 bits
1011               <--- divisor (4 bits) = x³ + x + 1
------------------
01100011101100 000 <--- result
```

The algorithm acts on the bits directly above the divisor in each step. The result for that iteration is the bitwise XOR of the polynomial divisor with the bits above it. The bits not above the divisor are simply copied directly below for that step. The divisor is then shifted right to align with the highest remaining 1 bit in the input, and the process is repeated until the divisor reaches the right-hand end of the input row. Here is the entire calculation:

```
11010011101100 000 <--- input right padded by 3 bits
1011               <--- divisor
01100011101100 000 <--- result (note the first four bits are the XOR with the divisor beneath, the rest of the bits are unchanged)
 1011              <--- divisor ...
00111011101100 000
  1011
00010111101100 000
   1011
00000001101100 000 <--- note that the divisor moves over to align with the next 1 in the dividend (since quotient for that step was zero)
       1011             (in other words, it doesn't necessarily move one bit per iteration)
00000000110100 000
        1011
00000000011000 000
         1011
00000000001110 000
          1011
00000000000101 000
           101 1
-----------------
00000000000000 100 <--- remainder (3 bits).  Division algorithm stops here as dividend is equal to zero.
```

每一次Xor都是固定不动的生成项与其对应的数据首位“消1”。

那我们就可以假想出一个与生成项长度一致的“盒子”，取出一部分的数据出来若首位是1时就进行一次Xor，遇到0则左移到1为止，左移造成的右端的空缺用0补充。而这里0希望理解为一种“存储”，它“存储” 生成项中未和数据进行计算的那一部分，按顺序先后附加被计算数据的后面，当先一部分的数据全部计算之后，实际上“盒子”中剩下都是未和数据计算的部分的“和”11011 xor 10110 = 11011 xor ( 10000 xor 00100 xor 00010)(这里实际上就是Xor的交换律到后面就会体会到他的强大)

![图片描述](https://segmentfault.com/img/bVbn5m4)



## 2.3 数学公式

CRC为[校验和](https://zh.wikipedia.org/wiki/校验和)的一种，是两个字节数据流采用二进制除法（没有进位，使用[XOR](https://zh.wikipedia.org/wiki/XOR)来代替减法）相除所得到的余数。其中被除数是需要计算校验和的信息数据流的二进制表示；除数是一个长度为![(n+1)](https://wikimedia.org/api/rest_v1/media/math/render/svg/b30a29cfd35628469f9dbffea4804f5b422f3037)的预定义（短）的二进制数，通常用多项式的系数来表示。在做除法之前，要在信息数据之后先加上![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)个0。

CRC是基于[有限域](https://zh.wikipedia.org/wiki/有限域)**GF(2)**（即[除以2](https://zh.wikipedia.org/wiki/除以2)的[同余](https://zh.wikipedia.org/wiki/同余)）的[多项式环](https://zh.wikipedia.org/wiki/多项式环)。简单的来说，就是所有系数都为0或1（又叫做二进制）的多项式系数的集合，并且集合对于所有的代数操作都是封闭的。例如：

![(x^{3}+x)+(x+1)=x^{3}+2x+1\equiv x^{3}+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/ac743b44be7cd4295a10a0b4796fc7d9c11fb47e)

2会变成0，因为对系数的加法运算都会再取2的模数。乘法也是类似的：

![(x^{2}+x)(x+1)=x^{3}+2x^{2}+x\equiv x^{3}+x](https://wikimedia.org/api/rest_v1/media/math/render/svg/c05b762b1f89934a5e740d72a767cea43842d8a0)

同样可以对多项式作除法并且得到商和余数。例如，如果用*x*3 + *x*2 + *x*除以*x* + 1。会得到：

![{\frac {(x^{3}+x^{2}+x)}{(x+1)}}=(x^{2}+1)-{\frac {1}{(x+1)}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/7f54294988f879cfcf02f798b6f4d2501b6de8f9)

也就是说，

![(x^{3}+x^{2}+x)=(x^{2}+1)(x+1)-1](https://wikimedia.org/api/rest_v1/media/math/render/svg/e702a4b8a7d4906525d0a79fe3e33e4de48e7d04)

等价于

![(x^{2}+x+1)x=(x^{2}+1)(x+1)-1](https://wikimedia.org/api/rest_v1/media/math/render/svg/dc73dd92435b032e7eb76cd613ebeeafe5971baf)

这里除法得到了商*x*2 + 1和余数-1，因为是奇数所以最后一位是1。

字符串中的每一位其实就对应了这样类型的多项式的系数。为了得到CRC，首先将其乘以![x^{n}](https://wikimedia.org/api/rest_v1/media/math/render/svg/150d38e238991bc4d0689ffc9d2a852547d2658d)，这里n是一个固定多项式的**[阶数](https://zh.wikipedia.org/w/index.php?title=多項式的階&action=edit&redlink=1)**，然后再将其除以这个固定的多项式，余数的系数就是CRC。

在上面的等式中，![x^{2}+x+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/78235883dfed13f5c0c7b6fb5aa82c002a1ac649)表示了本来的信息位是`111`, ![x+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/16df430ed7a23df9b160a5bbd957f306a0c3baa7)是所谓的**钥匙**，而余数1也就是![x^{0}](https://wikimedia.org/api/rest_v1/media/math/render/svg/1871ffeb57c11624b375dbb7157d5887c706eb87)就是CRC. key的最高次为1,所以将原来的信息乘上![x^{1}](https://wikimedia.org/api/rest_v1/media/math/render/svg/92004fb9ae631cb2d99594f120855e50b75578e3)来得到![x^{3}+x^{2}+x](https://wikimedia.org/api/rest_v1/media/math/render/svg/ca4a78b924c532d1222a6cd4ff9cb742c805279b)，也可视为原来的信息位补1个零成为`1110`。

一般来说，其形式为：

![M(x)\cdot x^{n}=Q(x)\cdot K(x)-R(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/f72ac19eb6168035011ca0ea3e4c849bf4108f9c)

这里M(x)是原始的信息多项式。K(x)是![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)阶的“钥匙”多项式。![M(x)\cdot x^{n}](https://wikimedia.org/api/rest_v1/media/math/render/svg/6cd6a76fb2ef2ab419b5000c04b80db5426fe8fb)表示了将原始信息后面加上![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)个0。R(x)是余数多项式，即是CRC“校验和”。在通信中，发送者在原始的信息数据M后附加上�![n](https://wikimedia.org/api/rest_v1/media/math/render/svg/a601995d55609f2d9f5e233e36fbe9ea26011b3b)位的R（替换本来附加的0）再发送。接收者收到M和R后，检查![M(x)\cdot x^{n}+R(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/0afb8cbd152e34ce6a156732934a54c46b9831fb)是否能被![K(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/19a621a6fbcc971649157a42ea42a599c22eee94)整除。如果是，那么接收者认为该信息是正确的。值得注意的是![M(x)\cdot x^{n}+R(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/0afb8cbd152e34ce6a156732934a54c46b9831fb)就是发送者所想要发送的数据。这个串又叫做*codeword*.

CRCs经常被叫做“[校验和](https://zh.wikipedia.org/wiki/校验和)”，但是这样的说法严格来说并不是准确的，因为技术上来说，校验“和”是通过加法来计算的，而不是CRC这里的除法。

## 2.4 CRC8

