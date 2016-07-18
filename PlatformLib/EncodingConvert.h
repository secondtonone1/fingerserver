#ifndef __LYNX_PLATFORM_LIB_ENCODING_CONVERT_H__
#define __LYNX_PLATFORM_LIB_ENCODING_CONVERT_H__

#include <string>
#include <locale>
#include <cwchar>
#include <cstddef> 
#include "MemoryManager.h"

#define CODECVT_DO_LENGTH_CONST const

namespace Lynx
{
    class _PlatformExport utf8_codecvt_facet : public std::codecvt<wchar_t, char, std::mbstate_t>  
    {
    public:
        explicit utf8_codecvt_facet(std::size_t no_locale_manage=0)
            : std::codecvt<wchar_t, char, std::mbstate_t>(no_locale_manage) {}
    protected:
        virtual std::codecvt_base::result do_in(std::mbstate_t& state, 
            const char * from, const char * from_end, const char * & from_next,
            wchar_t * to, wchar_t * to_end, wchar_t*& to_next) const;

        virtual std::codecvt_base::result do_out(std::mbstate_t & state, const wchar_t * from,
            const wchar_t * from_end, const wchar_t*  & from_next,
            char * to, char * to_end, char * & to_next) const;

        bool invalid_continuing_octet(unsigned char octet_1) const 
        {
            return (octet_1 < 0x80|| 0xbf< octet_1);
        }

        bool invalid_leading_octet(unsigned char octet_1)   const 
        {
            return (0x7f < octet_1 && octet_1 < 0xc0) || (octet_1 > 0xfd);
        }

        // continuing octets = octets except for the leading octet
        static unsigned int get_cont_octet_count(unsigned   char lead_octet) 
        {
            return get_octet_count(lead_octet) - 1;
        }

        static unsigned int get_octet_count(unsigned char   lead_octet);

        // How many "continuing octets" will be needed for this word
        // ==   total octets - 1.
        int get_cont_octet_out_count(wchar_t word) const ;

        virtual bool do_always_noconv() const throw() { return false; }

        // UTF-8 isn't really stateful since we rewind on partial conversions
        virtual std::codecvt_base::result do_unshift(std::mbstate_t&,
            char * from, char * /*to*/, char * & next) const 
        {
            next = from;
            return ok;
        }

        virtual int do_encoding() const throw() 
        {
            const int variable_byte_external_encoding=0;
            return variable_byte_external_encoding;
        }

        // How many char objects can I process to get <= max_limit
        // wchar_t objects?
        virtual int do_length(CODECVT_DO_LENGTH_CONST std::mbstate_t &,
            const char * from, const char * from_end, std::size_t max_limit) const;

        // Largest possible value do_length(state,from,from_end,1) could return.
        virtual int do_max_length() const throw () 
        {
            return 6; // largest UTF-8 encoding of a UCS-4 character
        }
    };

    class _PlatformExport EncodingConvert
    {
    public:
        // 转换字符编码从ANSI到UNICODE(wchar_t) 
        static BasicStringW   ansi2Unicode(const BasicString& s, 
            const std::codecvt<wchar_t, char, mbstate_t>& cvt);
        // 转换字符编码从UNICODE到ANSI
        static BasicString    unicode2Ansi(const BasicStringW& s, 
            const std::codecvt<wchar_t, char, mbstate_t>& cvt);
        // 转换字符编码从UTF-8到UNICODE(wchar_t)
        static BasicStringW   utf82Unicode(const BasicString& s);
        // 转换字符编码从UNICODE到UTF-8
        static BasicString    unicode2Utf8(const BasicStringW& s);
        // 转换字符编码从UNICODE到MBCS
        static BasicString    unicode2Mbcs(const BasicStringW& s, 
            const std::locale& loc = std::locale());
        // 转换字符编码从MBCS到UNICODE
        static BasicStringW   mbcs2Unicode(const BasicString& s, 
            const std::locale& loc = std::locale());
        // 转换字符编码从UTF-8到MBCS
        static BasicString    utf82Mbcs(const BasicString& s, 
            const std::locale& loc = std::locale());
        // 转换字符编码从MBCS到UTF-8
        static BasicString    mbcs2Utf8(const BasicString& s, 
            const std::locale& loc = std::locale());
        // 转换字符编码从ANSI到UTF-8
        static BasicString    ansi2Utf8(const BasicString& s,
            const std::locale& loc = std::locale());
        // 转换字符编码从UTF-8到ANSI
        static BasicString    utf82Ansi(const BasicString& s,
            const std::locale& loc = std::locale());
    };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_ENCODING_CONVERT_H__

/***************************************************************************************************************************
ANSI/ASCII : 由美国国家标准委员会指定的是基于拉丁字母的一套电脑编码系统。
它主要用于显示现代英语和其他西欧语言。
它是现今最通用的单字节编码系统，并等同于国际标准ISO 646。（from wiki）

MBCS : 多字节字符集，是一种替代 Unicode 以支持无法用单字节表示的字符集（如日文和中文）的方法。
为国际市场编程时应考虑使用 Unicode 或 MBCS，或使程序能够通过更改开关来生成支持两种字符集之一的程序。
最常见的 MBCS 实现是双字节字符集 (DBCS)。一般来说，Visual C++（尤其是 MFC）完全支持 DBCS。（from msdn）

UNICODE : 是业界的一种标准，它可以使电脑得以呈现世界上数十种的文字系统。
Unicode 是基于通用字元集（Universal Character Set）的标准来发展，
并且同时也以书本的形式（The Unicode Standard，目前第五版由Addison-Wesley Professional出版，ISBN-10: 0321480910）对外发表。
Unicode 包含了超过十万个字元（在西元 2005 年， Unicode 的第十万个字元被采纳且认可成为标准之一）、
一组可用以作为视觉参考的代码图表、
一套编码方法与一组标准字元编码、一套包含了上标字、下标字等字元特性的列举等。(from wiki)

UTF-8 : 英文全称为8-bit UCS/Unicode Transformation Format，是针对Unicode 的一种可变长度字元编码。
从名称可以看出，UTF-8是专为UCS/Unicode设计的传输格式。它可以用来表示 Unicode 标准中的任何字元，
而且其编码串流中的第一个位元组仍与 ASCII 兼容，令原来处理 ASCII 字符的软件无需或只作少量改动后，
便可继续使用。因此，它逐渐成为电子邮件、网页及其他储存或传送文字的应用中，优先采用的编码。(from wiki)

基本的名词解释完了，现在来仔细的解释一下Charset/encoding（字符集/编码）。
Charset很容易和encoding搞混，也是刚开始接触字符编码问题是最容易被晕掉的概念。
字符集的概念，实际上，包含两个方面，一个，是字符的集合，即所谓的Charset，
一个是编码方案，也就是所谓的encoding。
所谓字符的集合，意即一个字符集，定义了它所包含的所有符号，这实际上正是字符集名字的真正含义。
也就是说，狭义上的字符集，并不包含编码方案，它仅仅是定义了哪些符号属于这个字符集。
但是，通常来说，一个字符集并不仅仅定义字符集合，同时，它还为每个符号定义一个二进制编码，
所以，当我们提到GB2312的时候，我们并不仅仅是指GB2312字符集，
同时，也指明了编码方案是GB2312，即Charset= GB2312,encoding=GB2312，
这说明，我们的文档中不包含GB2312以外的字符，而它们的二进制编码采用GB2312规定的编码方式。
简单的情况的确如此，字符集等于编码，编码等于字符集。

但是，通常把我们搞晕的，正是一个例外，Unicode。
Unicode字符集本身定义的编码方案通常称为UCS-2，或者一个更通用的名字， UTF-16。
然而，由于UTF-16不能和现行的基于ascii的编码方案兼容，比较重点的问题在于0x0，
在基于ascii的编码方案中，一个8位的 0x0总是表示一个字符串的结束的，
而UTF-16则不然，它的一个字符，
完全有可能在高8位或者低8位上等于0x0，这会导致很多应用程序错误，
尤其是在网络传输协议当中可能导致大量的字符串错误截断。
于是，有了UTF-8，UTF-8提供了一个跟ascii兼容的unicode字符集编码方案。
网络上常见的说法说UTF-8是1到3位变长编码，这是错误的，UTF-8是1到6位变长编码，
3位的说法来源于大多数常用汉字被包括在3位编码的范畴以内，
而另外，从现行的Unicode规范来讲，UTF-8实际上是1到4位的编码，
因为再加上两位编码所扩展的范围现在Unicode还没有定义任何字符。 
UTF-8的编码方案首先保证跟127个标准ascii字符兼容，也就是说，在UTF-8方案下，
Unicode的0x000000–0x00007F范围的字符被表达为0x0-0x7F的一个字节的二进制编码。
其次，UTF-8保证，所有0x7F以上的字符,在被转译成多字节字符时，每个字节的最高位一定为1，
这实际上也是大多数MBCS方案的基本原则，
否则应用程序没法识别多字节字符的字节组合方式或者出现错误的0x0。
问题在于，一般的DBCS双字节方案可以简单的根据高位是否为1而判定单字节还是双字节，
而UTF-8是变长的，应用程序需要知道如何组合连续的字节数据，
按照UTF-8的规定，除了最高的一个字节外，其余的所有字节均以10开头，而最高字节的开头，
110表示连续2位，1110表示连续3位，11110表示连续4位。
（一个具体字符的UTF-8编码值根据一个简单的对应算法从UTF-16得到，这里就不详细讲述，请自行google）。
因此，我们必须明确一个概念，UTF-8是 unicode字符集的一个编码方案，
当我们在说到UTF-8字符和Unicode字符的时候，
在某些情况下，它们在逻辑上是等价的，但是，他们并不是同一个东西，
因为Unicode字符在二进制上还有一个选择就是原生的UTF-16编码。

总结一下，Unicode字符集规定的标准编码方案是UCS-2（UTF-16），用两个字节表示一个Unicode字符，
而事实上，UCS-4 （UTF-32）也已经被提出了，用4个字节表示一个Unicode字符，然后，
一个常用的Unicode编码方案—UTF-8,它用1到4（6）位的变长字节来表示一个Unicode字符，
并可以从一个简单的转换算法从UTF-16直接得到。
这三个编码方案（encoding）都对应于Unicode字符集（Charset）。

然后，需要解释一下Codepage-代码页，codepage实际上是一张表，
通常的codepage是一个从unicode到其他mbcs的转换索引表，
比如windows上常用的MS936代码页，实际上就是GB2312到unicode的转换表，
我们知道，windows是完全基于 unicode的，
MS的应用程序也大多是基于unicode开发的，他们对GB2312的支持，
正是来源于codpage932，通过cp932的转换，
应用程序可以在unicode和gb2312之间来回转换。
需要多一句嘴的是关于日文编码，IBM和微软都提供了几个工业上常用的日文编码的代码页，比如对于shift-jis，
IBM的代码页是CCSID943，
这也是java在转换时使用的代码页，而MS的代码页是MS936，他们两者都是针对 shift-jis的代码页，
算是两个并行的工业标准，在某些字符上转换结果并不一致，
这可能会导致应用程序错误，
比如我遇到的典型问题是通过java程序将shift-jis数据转换为unicode后传输给VC编写的程序再转换为shift-jis，
由于在两次转换中使用了不兼容的代码页，导致数据错误。
这并没有什么特别的解决办法，只能是查找特殊字符列表作特殊处理。

另外一个问题，就是所谓的基于unicode的应用程序，在应用程序内部字符串究竟以什么形式表达，
一般来说，
是以UCS-2也就是UTF-16 的二进制形式来表达的，UTF-8一般只是作为数据传输格式和文档保存格式。
具体一点，
对于java代码，javac编译程序在你没有指定源文件编码的情况下，总是将其认为是本地缺省编码，
比如在简体中文windows上会被认识为GB2312，
而在日文windows上会被识别为shift-jis，然后实际上，在编译的时候，gb或者jis字符都会被转换成对应的UCS-2二进制值。
而对于C++程序，当你简单的用 ”中” 这样的引号引用的形式的时候，
编译器会简单的将其识别为本地编码并且在编译后的二进制代码中被表达为本地编码的二进制值
（我不知道c++编译器能否指定源文件编码。。。我不太熟悉c++），
但是，如果你用 L”中” 这样的标准c++的unicode字符定义方式来定义字符时，将会发生和javac编译程序一样的事情，
编译程序会主动的将字符正确的以本地编码识别并转换成UCS-2值保存为编译后的二进制代码。
C++程序中，如果以L宏定义字符串，
可以在程序中直接得到Unicode值，即UTF-16编码值，但在 java中，实际上我们并不能直接得到字符串的二进制值，
不过可以通过String的getbytes方法指定UTF-16编码得到。至于各种编码之间的转换，这里就不赘述了。

最后，解释一下关于font的问题，其实，很多时候我们遇到的乱码并没有出现编码转换错误，
只是由于应用程序指定的字体没有包含对应的字符图像而已，
这种情况，通常我们可以看到有部分字符是可以正常显示的。Font文件实际上也是一个索引文件，
一个encoding索引加上位图。
现在的字体文件通常包含两个编码索引，一个unicode，一个该字体对应语言的常用编码，
应用程序在显示字符的时候，通过查找对应的索引而得到字符的位图。
******************************************************************************************************************************/



