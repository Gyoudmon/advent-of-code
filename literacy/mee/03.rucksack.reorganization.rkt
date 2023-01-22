#lang scribble/lp2

@(require "../literacy.rkt")

@(define-aoc-bib D3RR "Rucksack Reorganization" 3)

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{第三天：背包重组}

故事原文见 @~cite[D3RR]。故事中的任务将按如下程序结构展开：

@handbook-chunk[<rucksack-reorganization:*>
                (module tamer racket/base
                  <定义物品类型优先级函数>
                  <定义物品类型字典构造函数>
                  <定义函数find-misplaced-item>)]

有一只精灵的重要任务是将丛林探险物资装进背包，但不幸的是，这只精灵并没有严格照着打包指令做事，因此有部分物品需要重新整理。

每个背包都有两个大隔间，所有物品按类型只能被装进其中一个隔间里，那只精灵搞砸了的地方就在这：
每个背包里都@bold{恰好有一类}物品装进了两个隔间里。

其他精灵已经整理出了所有已装包的物品清单，但是他们希望你来帮着找错。这份清单看起来长这样：

@tabular[#:style 'boxed #:sep @hspace[1]
         (list (list "vJrwpWtwJgWrhcsFMMfFFhFp")
               (list "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL")
               (list "PmmdzqPrVvPwwTWBwg")
               (list "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn")
               (list "ttgJtRGJQctTZtZT")
               (list "CrZsJsPPZsGzwwsLwLmpwMDw"))]

其中每一个大写字母和小写字母都代表一种类型的物品，也就是说，清单是区分大小写的，@litchar{A}和@litchar{a}代表不同类型的物品。
老规矩，一个背包里的所有物品都写在同一行，又因为每个背包的两个大隔间里的物品类型数量相同。因此，对于清单里的每一行，
前半部分表示第一个隔间的物品清单，后半部分表示第二个隔间的物品清单。

于是，从上面的例子里，我们能得到如下信息：

@itemlist[

 @item{第一个背包里装着 @litchar{vJrwpWtwJgWrhcsFMMfFFhFp}，其中第一个隔间里有 @litchar{vJrwpWtwJgWr}，
  第二个隔间里有 @litchar{hcsFMMfFFhFp}。那个弄错了的物品类型是同时装进了两个隔间的 @litchar{p}；}

 @item{第二个背包的两个隔间里分别装着 @litchar{jqHRNqRjqzjGDLGL} 和 @litchar{rsFMfFZSrLrFZsSL}，
  装错了的物品类型是 @litchar{L}；}
                                       
 @item{第三个背包里装着 @litchar{PmmdzqPrV} 和 @litchar{vPwwTWBwg}，弄错的是 @litchar{P}；}
 
 @item{同理，后面三个背包里装错了的物品类型分别是 @litchar{v}、@litchar{t} 和 @litchar{s}。}
 
 ]

到这里，今天的谜面就已经都出来了。按照前两天的经验，我们不妨就先开始定义主函数 @racketid[find-misplaced-items]
用以找出所有装错了的物品类型。而该函数的内部应该也是一个类似@racketcommentfont{读取-查错-处理}的大循环。
没错，主程序基本结构仍然是个大循环。但这里有两个小差异：

@itemlist[#:style 'ordered
 @item{@bold{查错}的过程不像之前的任务那么直接了当，本次任务需要你自己去设计算法来解决；}
 @item{@bold{处理}的过程更不明确，精灵只说要你找到放错了的物品类型，没说要怎么处理它们。}
]

按照精灵们说话说不全的毛病，我们应该提前规划好自己的合作策略，以不变应万变。无论他们之后会提出什么要求，
找到放错了的物品类型都是第一步。之后无论精灵想做什么，你只需要提供给他们一份@racketoutput{出错物品类型清单}，
而不必每次都从头开始读取他们整理的物品清单。

在继续之前，我们先提炼一下，前两天的大循环有什么共同点？或者说，那两个大循环里是否隐藏着相同的小循环？
既然这么问了，那必然是有的，这个基本的小循环就是 @question{将清单里的每一行作为字符串读出来}。
即，将输入文件按行转化为一个字符串列表。然后才是针对每一行字符串的操作，比如统计卡路里；再比如模拟猜拳比赛，等等。

因此，像这样@racketcommentfont{针对清单中的每一行的操作}就转化为了@racketcommentfont{针对字符串列表中
的每一项的操作}。那么，知道这一点有什么好处呢？一般高级一点的程序语言都会内置一系列函数，比如 @racket[file->lines]
或 @racket[in-lines]，直接帮你做好了@question{按行从文件里读出一个字符串列表}。因为，这个基本的小循环真的很常用。
语言作者不知道你会怎么处理每一行字符串，但无论你做什么，都得先有那一行字符串才行。

至此，我们的思维已经悄悄提升，正式将@defterm{列表}(@racket[list])作为一个基本数据类型来思考。@tech{列表}是所有
宣称自己支持@bold{函数式编程}的程序语言的基本数据类型。譬如，Python 的有 list 和 tuple 等；C++ 有 list 和
vector 等； C 比较麻烦，没有现成的可用。注意，数组不能替代@tech{列表}，因为数组要求所有项都同为一个类型，
列表则没有这个要求。实际上，列表里的项往往不是同一类型。

@handbook-chunk[<定义函数find-misplaced-items>
                (code:comment #,($argv [/dev/datin "与背包物品清单关联的输入源"]))
                (define (find-misplaced-items /dev/datin)
                  (for/list ([line <针对物品清单的每一行>])
                    <从清单行中找出那个放错了的物品类型>))]

这次主函数代码量少了很多，但是增加了巨量的信息，需要慢慢消化。首先，回归正题，我们需要做的事是，针对物品清单中的第一行，
从中找出放错了的那个物品类型：

@handbook-chunk[<从清单行中找出那个放错了的物品类型>
                (find-misplaced-item line)]

如此这般处理完物品清单中的单个行，接下来自然就是针对@racketcommentfont{每一个下一行}都做同样的事，直到没有更多的行：

@handbook-chunk[<针对物品清单的每一行>
                (in-lines /dev/datin)]

这段描述是不是很眼熟？正是前两天任务中需要你受累一键一键敲出来的大循环。这次，在我们深刻理解了这个大循环的前提下，
就不必再敲那么多次键盘了。函数 @racket[in-lines] 的作用是“按行读出字符串，并将每一行依次保存在变量
@racketcommentfont{line}中，供循环使用”，@racket[for/list] 的名字暴露出了它(与 @racket[in-lines] 配合时)
自带循环效果，并保证将每一轮循环处理得到的结果都搜集到列表（而不是别的什么数据结构）中去。

关于@bold{函数式编程}，现在我们只说这么多，最通俗易懂的部分。深呼吸，放松一下，做点简单的事开心一下：完成函数
@racket[find-misplaced-item] 的本体。

@handbook-chunk[<定义函数find-misplaced-item>
                (code:comment #,($argv [line "背包物品清单中的一行字符串"]))
                (define find-misplaced-item
                  (lambda [line]
                    (define midpos (/ (string-length line) 2))
                    <构造背包隔间的物品类型字典>
                    <查找字典中存在的物品类型>))]

通过前面的例子，这个函数的功能已经很明确了，就是@question{依次检查后半段的每一个字符，看它是否出现在前半段中}，或者
@question{依次检查前半段的每一个字符，看它是否出现在后半段中}。从关键字@bold{依次}可以推断，算法的主要结构肯定也是
循环，因此一种肯定有效的算法可能是这样的：依次提取半段中的字符，再拿这个字符依次跟另半段中的每一个字符比较，直到找到
两段中都有的字符、或没有更多的字符，为止。这是最简单的算法，但是也是效率最低的算法。

回忆一下，当碰到不认识的汉字或英文单词时，你会怎么来认识它？不过，现在我们要重点思考的内容，不是你根据什么规则来快速查字典，
而是为什么你会想到字典这个东西？字典代表的是一种什么样的东西呢？像字典这样的@bold{能够根据关键字快速定位其内容}的数据
结构称为@bold{哈希表}（Hash Table），简称哈希（@racket[hash]），它也是一种相当基础也相当重要的数据类型。将列表作为基
础数据类型的语言一般也会直接提供一种或几种哈希类型，不过它们的名字就可能五花八门了。譬如，Python 的哈希就叫字典（dict）；
C++ 的哈希叫映射（map）；C 不直接提供哈希表。

总之，计算机也有查字典的权利和能力。于是，@question{找到放错了的物品类型}问题就转化成了@racketcommentfont{构造字典-查字典}
问题。这个字典里只包含某个背包的一个隔间里的全部物品类型。

@handbook-chunk[<构造背包隔间的物品类型字典>
                (define dict (make-item-dict line midpos))]

直觉上看，构造字典似乎也是一个常用操作，值得单独定义一个函数 @racket[make-item-dict] 来做这件事。

@handbook-chunk[<定义物品类型字典构造函数>
                (code:comment #,($argv [items "物品清字符串"] [end "物品清字符串循环结束的位置"]))
                (define (make-item-dict items end)
                  (for/hash ([ch (in-string items 0 end)])
                    <返回物品类型字典中的条目>))]

这个 @racket[for/hash] 和 @racket[in-string] 组合似乎有点眼熟。没错，算法中的循环又被它俩隐藏了。函数 @racket[in-string]
负责依次提取字符串指定范围内的字符，并分别保存在变量 @racketcommentfont{ch} 中供循环使用。@racket[for/hash] 则负责把每次
循环的结果都搜集进一个哈希表里。

每次往字典里添加条目必须要提供两个值，一个代表关键字（key），一个代表该关键字在字典里的内容（value）。譬如，
英汉字典里的每一个词条，英文单词是其关键字，其对应的汉语翻译是其在字典里的内容。在本任务中，我们关注某个物品是否存在，
那该物品的类型即是关键字；而其在字典中的内容是关于其“存在与否”的信息，也即布尔真值 @racket[#true]。

@handbook-chunk[<返回物品类型字典中的条目>
                (values ch #true)]

注意，跟其他(绝大多数)语言不同的是，Racket 函数可以有@bold{多个}返回值。@racket[for/hash] 要求每轮循环结束提供两个值，
那我们就通过 @racket[values] 函数一次返回两个值。

Python 可以通过@bold{自动序列装包/拆包}达到同样的目的，但那仍然只是返回一个值。其具体过程是：将多个返回值搜集进一个元组，
然后将这个元组作为单个值返回，接着再一个一个把这个元组里的值取出来分别赋值给其他变量。

@handbook-chunk[<查找字典中存在的物品类型>
                (for/first ([ch (in-string line midpos)]
                            #:when (hash-has-key? dict ch))
                  ch)]

这次隐藏循环的是 @racket[for/first]，它的意思@racketcommentfont{收集到循环执行一次的结果就立即退出}，因此确定循环是否执行
的条件至关重要。在本任务中，即：一旦出现字典中存在的物品类型就不用再继续找下去了，直接返回该物品类型即可。

特别的，如果输入参数不合理导致循环一次都没有执行，那函数最终会返回 @racket[#false]。在真实的软件中，必须要考虑这种情况。

至此，我们完成了对物品清单的解析。至于为什么要同时定义 @racket[find-misplaced-items] 和 @racket[find-misplaced-item]
两个函数，后面这个看似完全多余啊，算法可以直接放在 @racket[find-misplaced-items] 循环里面吗？可以啊，不影响本次任务。
不过，上面虽然定义了（复数形式的） @racket[find-misplaced-items]，但它仅仅只是出现在本文档中用作演示常用循环，其代码并没有
实际包含在最终的可执行程序里。因为这个循环过于常用，我们完全没必要每次都敲一遍，把精力省下来关注@question{如何将每一行
杂乱无章的原始文本转化为对我们有用的数据和信息}才是正经。

而且，还记得我们上面分析的结论吗：@racketcommentfont{针对清单中的每一行的操作}就转化为了@racketcommentfont{针对字符串列表中
的每一项的操作}。换句话说，我们可以直接调用 @racket[find-misplaced-item]，并传给它一个字符串作为参数，用于测试算法是否靠谱，
而不用每次都费劲去准备一个文件作为数据源。比如，就用任务例子中的字符串好了：

@tamer-action[(find-misplaced-item
               "vJrwpWtwJgWrhcsFMMfFFhFp")
              (find-misplaced-item
               "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL")
              (find-misplaced-item
               "PmmdzqPrVvPwwTWBwg")
              (find-misplaced-item
               "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn")
              (find-misplaced-item
               "ttgJtRGJQctTZtZT")
              (find-misplaced-item
               "CrZsJsPPZsGzwwsLwLmpwMDw")]

函数的输出跟例子不能说完全相同，简直就是一摸一样啊。

不过呢，本算法仍然还有改进空间，一丝不苟地为一个隔间创建完整的字典总觉得很没必要（比如，当放错了的物品恰好在两个隔间里都是第一个时），
可不可以一边创建一边检查呢？请课后思考并尽量写程序验证你想法。【提示：同时给两个隔间分别创建一个字典，检查时两个隔间同时交叉比较。】

以上，是我们为应对精灵的信口开河而做的准备工作。接下来就该处理精灵们真正要你解决的问题了。为了定好出错物品类型的重组顺序，精灵们规定
每种类型的物品都有一个优先级：

@itemlist[
 @item{小写字母代表的物品从 @litchar{a} 到 @litchar{z} 分别对应着从 @litchar{1} 到 @litchar{26} 这 @racket[26] 个数字；} 
 @item{大写字母代表的物品从 @litchar{A} 到 @litchar{Z} 分别对应着从 @litchar{27} 到 @litchar{52} 这 @racket[26] 个数字。}
 ]

@handbook-chunk[<定义物品类型优先级函数>
                (code:comment #,($argv [ich "代表物品类型的字符"]))
                (define (item-type-priority ich)
                  (cond [(char<=? #\a ich #\z)
                         (+ (char->integer ich) (- 97) 1)]
                        [(char<=? #\A ich #\Z)
                         (+ (char->integer ich) (- 65) 27)]
                        [else 0]))

                (code:comment #,($argv [items "代表物品类型的字符列表"]))
                (define (priority-sum items)
                  (for/sum ([ich (in-list items)]
                            #:when (char? ich))
                    (item-type-priority ich)))]

优先级函数没啥特别要说的，如果对其中的神秘数字 @racket[97] 和 @racket[65] 感兴趣，可以@bold{谷歌}关键字 @bold{ASCII 码}，
看看字母 @litchar{A} 和 @litchar{a} 在该编码中的具体位置。

在前面的例子中，六个放错了的物品类型的优先级分别是 @tamer-datum['item-type-priority #\p](@litchar{p})、
 @tamer-datum['item-type-priority #\L](@litchar{L})、 @tamer-datum['item-type-priority #\P](@litchar{P})、
 @tamer-datum['item-type-priority #\v](@litchar{v})、 @tamer-datum['item-type-priority #\t](@litchar{t})
和 @tamer-datum['item-type-priority #\s](@litchar{s})，它们的和为 @tamer-datum['priority-sum '(#\p #\L #\P #\v #\t #\s)]。

于是，在找到所有放错了的物品类型之后，请计算@question{它们的优先级之和}。

@tamer-action[#:requires ["../aoc.rkt"]
              (code:comment "注意，函数 read-aoc-data 隐藏了读文件小循环")
              (define all-misplaced-items
                (read-aoc-data
                 #:from "mee/03.rr.dat"
                 #:for-each-do find-misplaced-item))
              (priority-sum all-misplaced-items)]

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
