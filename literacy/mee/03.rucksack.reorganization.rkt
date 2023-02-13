#lang scribble/lp2

@(require "../literacy.rkt")

@(define the-core-in-thinking
   @focus{如何将每一行杂乱无章的原始文本转化为对我们有用的数据和信息})

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{第三天：背包重组}

@aoc-desc[3
 #:keywords ["序列"]]

故事中的任务将按如下程序结构展开：

@handbook-chunk[<rucksack-reorganization:*>
                (module advent racket/base
                  <定义物品类型优先级函数>
                  <定义物品类型字典构造函数>
                  <定义共有物品类型查找函数>
                  <定义函数find-misplaced-item>
                  <定义函数find-badge-item>)]

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
 @item{@bold{查错}过程不像之前的任务那么直接了当，本次任务需要你自己去设计算法来解决；}
 @item{@bold{处理}过程更不明确，精灵只说要你找到放错了的物品类型，没说要怎么处理它们。}
]

按照精灵们说话说不全的毛病，我们应该提前规划好自己的合作策略，以不变应万变。无论他们之后会提出什么要求，
找到放错了的物品类型都是第一步。之后无论精灵想做什么，你只需要提供给他们一份@racketoutput{出错物品类型清单}，
而不必每次都从头开始读取他们整理的物品清单。

在继续之前，我们先提炼一下，前两天的大循环有什么共同点？或者说，那两个大循环里是否隐藏着相同的小循环？
既然这么问了，那必然是有的，这个基本的小循环就是 @question{将清单里的每一行作为字符串读出来}。
即，将输入文件按行转化为一个字符串列表。然后才是针对每一行字符串的操作，比如统计卡路里；再比如模拟猜拳比赛，等等。

因此，像这样@racketcommentfont{针对清单中的每一行的操作}就转化为了@racketcommentfont{针对字符串列表中
的每一项的操作}。那么，知道这一点有什么好处呢？一般高级一点的程序语言都会内置一系列函数，比如 @racket[file->lines]
或 @racket[in-lines]，直接帮你做好了@question{按行从文件里读出一个字符串列表}。因为，这个小循环真的很常用。
语言作者不知道你会怎么处理每一行字符串，但无论你做什么，都得先有那一行字符串才行。

至此，我们的思维已经悄悄提升，正式将@defterm{列表}(@racket[list])作为一个基本数据类型来思考。@tech{列表}是所有
宣称自己支持@bold{函数式编程}的程序语言的基本数据类型。譬如，Python 有 list 和 tuple 等；C++ 有 list 和
vector 等； C 比较麻烦，没有现成的可用。注意，数组不能替代@tech{列表}，因为数组要求所有项都同为一个类型，
列表则没有这个要求。

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
两段中都有的字符、或没有更多的字符为止。这是最简单的算法，但是也是效率最低的算法。

回忆一下，当碰到不认识的汉字或英文单词时，你会怎么来认识它？不过，现在我们要重点思考的内容，不是你根据什么规则来快速查字典，
而是为什么你会想到字典这个东西？字典代表的是一种什么样的东西呢？像字典这样的@bold{能够根据关键字快速定位其内容}的数据
结构称为@defterm{哈希表}（Hash Table），简称哈希（@racket[hash]），它也是一种相当基础也相当重要的数据类型。将列表作为基
础数据类型的语言一般也会直接提供一种或几种哈希类型，不过它们的名字就可能五花八门了。譬如，Python 的哈希表就叫字典（dict）；
C++ 的哈希表叫映射（map）；C 不直接提供哈希表。

总之，计算机也有查字典的权利和能力。于是，@question{找到放错了的物品类型}问题就转化成了@racketcommentfont{构造字典-查字典}
问题。这个字典里只包含某个背包的一个隔间里的全部物品类型。

@handbook-chunk[<构造背包隔间的物品类型字典>
                (define dict (make-item-dict line midpos))]

直觉上看，构造字典似乎也是一个常用操作，值得单独定义一个函数 @racket[make-item-dict] 来做这件事。

@handbook-chunk[<定义物品类型字典构造函数>
                (code:comment #,($argv [items "物品清单字符串"] [end "物品清单字符串循环结束的位置"]))
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

同理，从字典中查找共有物品也是一个常用操作，单独定义成函数 @racket[make-shared-item-dict]，其返回值也是一个字典，
该字典只包括共有物品类型。

@handbook-chunk[<定义共有物品类型查找函数>
                (code:comment #,($argv [dict "源字典"] [items "物品清单字符串"] [start "物品清单字符串循环开始的位置"]))
                (define (make-shared-item-dict dict items start)
                  (for/hash ([ch (in-string items start)]
                             #:when (hash-has-key? dict ch))
                    <返回物品类型字典中的条目>))]

该函数几乎与 @racket[make-item-dict] 一样，除了 @racket[in-string] 循环提取字符的范围外，还多了一个 @racket[#:when]
用于指明循环执行的条件，即：只有源字典中已经存在的关键字才会作为新字典的关键字返回。

@handbook-chunk[<查找字典中存在的物品类型>
                (car
                 (hash-keys
                  (make-shared-item-dict dict line midpos)))]

现在我们姑且假设每个背包里都有且只有一个共有物品类型，因此上面的代码片段并未做容错处理。但在真实的软件中，必须要考虑其他情况，
有可能不存在（或存在多个）共有物品类型，并有可能导致运行时错误。

至此，我们完成了对物品清单的解析。至于为什么要同时定义 @racket[find-misplaced-items] 和 @racket[find-misplaced-item]
两个函数，后面这个看似完全多余啊，算法可以直接放在 @racket[find-misplaced-items] 循环里面吗？可以啊，不影响本次任务。
不过，上面虽然定义了（复数形式的） @racket[find-misplaced-items]，但它仅仅只是出现在本文档中用作演示文件输入小循环，其代码并没有
实际包含在最终的可执行程序里。因为这个循环过于常用，我们完全没必要每次都敲一遍，把精力省下来关注 @the-core-in-thinking 才是正经。

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

函数的返回值跟例子里的结论不能说完全相同，简直就是一摸一样啊。

以上，是我们为应对精灵的信口开河而做的准备工作。接下来就该处理精灵们真正要你解决的问题了。为了定好出错物品类型的重组顺序，精灵们规定
每种类型的物品都有一个优先级：

@itemlist[
 @item{小写字母代表的物品从 @litchar{a} 到 @litchar{z} 分别对应着从 @litchar{1} 到 @litchar{26} 这 @racket[26] 个数字；} 
 @item{大写字母代表的物品从 @litchar{A} 到 @litchar{Z} 分别对应着从 @litchar{27} 到 @litchar{52} 这 @racket[26] 个数字。}
 ]

@handbook-chunk[<定义物品类型优先级函数>
                (code:comment #,($argv [ich "代表物品类型的字符"]))
                (define (item-priority ich)
                  (cond [(char<=? #\a ich #\z)
                         (+ (char->integer ich) (- 97) 1)]
                        [(char<=? #\A ich #\Z)
                         (+ (char->integer ich) (- 65) 27)]
                        [else 0]))

                (code:comment #,($argv [items "代表共有物品类型的字符列表"]))
                (define (priority-sum items)
                  (for/sum ([ich (in-list items)]
                            #:when (char? ich))
                    (item-priority ich)))]

优先级函数没啥特别要说的，如果对其中的神秘数字 @racket[97] 和 @racket[65] 感兴趣，可以@bold{谷歌}关键字 @bold{ASCII 码}，
看看字母 @litchar{A} 和 @litchar{a} 在该编码中的具体位置。

在前面的例子中，六个放错了的物品类型的优先级分别是 @tamer-datum['item-priority #\p](@litchar{p})、
 @tamer-datum['item-priority #\L](@litchar{L})、 @tamer-datum['item-priority #\P](@litchar{P})、
 @tamer-datum['item-priority #\v](@litchar{v})、 @tamer-datum['item-priority #\t](@litchar{t})
和 @tamer-datum['item-priority #\s](@litchar{s})，它们的和为 @tamer-datum['priority-sum '(#\p #\L #\P #\v #\t #\s)]。

于是，第一个谜题是@question{在找到所有放错了的物品类型之后，计算它们的优先级之和}。

@tamer-action[#:requires ["../aoc.rkt"]
              (code:comment "注意，函数 read-aoc-data 隐藏了读文件小循环")
              (define all-misplaced-items
                (read-aoc-data
                 #:from "mee/03_rr.aoc"
                 #:for-each-do find-misplaced-item))
              (priority-sum all-misplaced-items)]

在你忙着找出所有放错了的物品类型之后，精灵们又碰到新的麻烦了。在本次远征中，出于安全考虑，精灵们决定三人一组成立小队。
每个小队都有自己的专属徽章，徽章上面的内容即是每队三只精灵都共同携带的@bold{一种}物品类型。也就是说，如果某队的徽章
是个弓箭，那该队所有三只精灵的背包里都会有弓箭，并且其他所有物品都最多只能有两只精灵携带（否则队徽上就不只一种物品了）。

问题在于，不少精灵忘了在队徽上贴“真品”标识了，不得已只好回收所有队徽从新贴标签；然而又没人记录哪一个徽章属于哪个队。
于是，这些干活毛手毛脚的精灵只好来求助你了。

假如，精灵们给的物品类型清单就是按组写的，每三行代表一个组，每组一个队徽，那队徽就是该组的三行清单里那唯一的共同物品类型。
于是，从本任务前面的例子中可以得到两个精灵小队，第一队背包里有

@tabular[#:style 'boxed #:sep @hspace[1]
         (list (list "vJrwpWtwJgWrhcsFMMfFFhFp")
               (list "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL")
               (list "PmmdzqPrVvPwwTWBwg"))]
队里三只精灵唯一共同持有的物品是 @litchar{r}；第二队背包里有：
@tabular[#:style 'boxed #:sep @hspace[1]
         (list (list "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn")
               (list "ttgJtRGJQctTZtZT")
               (list "CrZsJsPPZsGzwwsLwLmpwMDw"))]
队里三只精灵唯一共同持有的物品是 @litchar{Z}。

这两队的徽章的优先级之和是 @tamer-datum['priority-sum '(#\r #\Z)]（@tamer-datum['item-priority #\r](@litchar{r})
@racket[+] @tamer-datum['item-priority #\Z](@litchar{Z})）。

于是，第二个谜题是@question{在找到所有小队的徽章之后，计算它们所代表的物品类型的优先级之和}。

@tech{列表}和@tech{哈希表}是本次任务中的核心数据结构，但是我们更应该关注的依然还是@idea{它们的存到所引发的思维方式的提升}，
更通俗一点地说就是@idea{问题与问题之间的相互转化}。

在前一个谜题中，我们已经能够理解，关注文件操作大循环的意义不大，核心应该放在@|the-core-in-thinking|。但是在第二个谜题中，
一个很明显的不同是，这个问题需要一次读取三条物品清单。那么，原先针对单个物品清单的大循环还能直接用吗？比如，

@handbook-chunk[<定义函数find-badge-items>
                (code:comment #,($argv [/dev/datin "与背包物品清单关联的输入源"]))
                (define (find-badge-items /dev/datin)
                  (for/list ([lines <针对每一组物品清单>])
                    (find-badge-item lines)))]

对比该函数与 @racket[find-misplaced-items]，它俩结构完全一致，唯一的差别仅有@racketcommentfont{每次读取的行数}。
这跟第一天的两个谜题有点像，那天我们选了列表作为解决方案，本题也可沿用同样的思路。于是，

@handbook-chunk[<针对每一组物品清单>
                (in-port <创建每次读三行函数> /dev/datin)]

现在你应该不会再对 @litchar{in-} 打头的函数陌生了。譬如，前面的 @racket[in-lines] 函数的功能是
@racketcommentfont{从文件中每次读取一行，并依次提供给循环使用}。这里不妨大胆猜猜看，这个 @racket[in-port] 函数的功能
应该也是读取文件，但要按照它的第一个参数的要求来读。本任务中是要@racketcommentfont{一次读取三行}，每次读取之后返回
一个包含三个字符串的列表。

实际上，@racket[in-lines] 就是当第一个参数是函数 @racket[read-line] 时的 @racket[in-port]。有点拗口，现在需要
记住的是，函数本身也是可以当作参数传递给其他函数的，只要那些函数被声明为可以接受函数类型的参数。这其实就是数学中的
@bold{复合函数}概念，而我们一直在提的@bold{函数式编程}跟数学函数的关系相当密切。

于是，对于本题，我们只需提供一个@racketcommentfont{每次读取三行}的函数 @racket[read-3-lines] 即可：

@handbook-chunk[<创建每次读三行函数>
                (make-read-lines 3)]

只是，这个 @racket[read-3-lines] 不如@racketcommentfont{每次读取一行}的 @racket[read-line] 常用。一般语言
不会直接提供，需要我们自己去写。该函数逻辑并不复杂，无非就是重复3次 @racket[read-line]，所以我直接提供了函数
@racket[make-read-lines]，用于创建一个@racketcommentfont{每次读取n行}的函数。该函数特殊在，它的返回值是另一个函数。
考虑到真实软件中需要检查输入文件是否合乎契约，这个返回的函数最后一次实际读到的列表的大小可能会小于n。

以下例子演示了如何借助 @racket[make-read-lines] 分别读取前三天任务的前三行输入数据。注意区分 @racket[read-line]
和 @racket[(make-read-lines 1)] 的区别。

@tamer-action[(with-aoc-data-from "mee/01_cc.aoc" #:do
                read-line)
              (with-aoc-data-from "mee/01_cc.aoc" #:do
                (make-read-lines 1))
              (with-aoc-data-from "mee/02_rps.aoc" #:do
                (make-read-lines 2))
              (with-aoc-data-from "mee/03_rr.aoc" #:do
                (make-read-lines 3))]

到这里，有关文件输入大循环的奥秘就都解开了。从现在开始，我们将不再关注文件输入大循环，而只关注@question{我们需要读取
什么样的数据}和@question{如何处理这些数据}。处理后者的函数是重点，处理前者的函数只是文件原始内容和后者之间的@bold{管道}。
就好像你家浴室的花洒，水管里只能流出水柱，但从花洒里出来的水就跟花洒的形状有关了，不同形状的水适用于不同的洗澡目的。

最后来完成第二个谜题的核心部分，也即@question{如何处理每三行物品清单}的函数 @racket[find-badge-item]：

@handbook-chunk[<定义函数find-badge-item>
                (code:comment #,($argv [items "小队所有精灵的物品清单列表"]))
                (define find-badge-item
                  (lambda [items]
                    (car
                     (hash-keys
                      <依次构造共有物品类型字典>))))]

从@racketcommentfont{构造字典-查字典}的角度来看，该函数的功能仍然是从字典中找出唯一存在的物品类型，差别在于@question{从
 原先的两个隔间里找}变成了@question{从三个背包里找}。构造字典的过程仅差在循环开始或结束的位置。于是，上面靠直觉单独定义的函数
@racket[make-item-dict] 和 @racket[make-shared-item-dict] 就派上用场了：

@handbook-chunk[<依次构造共有物品类型字典>
                (for/fold ([dct <第一个背包的物品类型字典>])
                          ([item (in-list (cdr items))])
                  (make-shared-item-dict dct item 0))]

从第二个背包开始，查找共有物品的过程就没有差别了，都是@racketcommentfont{在上次循环产生的共有物品字典中查找本次循环中有的物品}。
这里使用了一类特殊循环 @racket[for/fold]，它允许循环过程能够直接操作上一次循环产生的结果。从这个描述中，你看到@bold{递归}的影子了吗？
实际上，@italic{fold} 是@bold{函数式编程}中的一种既典型又灵活的高级运算，中文直译为@bold{折叠}，在其他语言里也可能以别的同义概念
的形式出现。总之，其他 @litchar{for/} 型循环都可以用 @italic{fold} 操作来实现。不过，今天的信息量太多了，这个以后再详细说吧。

@italic{fold} 操作需要一个初始值，那自然就是第一个背包的物品清单字典：

@handbook-chunk[<第一个背包的物品类型字典>
                (let ([item (car items)])
                  (make-item-dict item (string-length item)))]

同理，我们可以通过直接传入前面例子中的字符串列表来测试 @racket[find-badge-item]：

@tamer-action[(find-badge-item
               (list "vJrwpWtwJgWrhcsFMMfFFhFp"
                     "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL"
                     "PmmdzqPrVvPwwTWBwg"))
              (find-badge-item
               (list "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn"
                     "ttgJtRGJQctTZtZT"
                     "CrZsJsPPZsGzwwsLwLmpwMDw"))]

至此，任务完成：

@tamer-action[(define all-badge-items
                (read-aoc-data
                 #:from "mee/03_rr.aoc"
                 #:with (make-read-lines 3)
                 #:for-each-do find-badge-item))
              (priority-sum all-badge-items)]

在本任务中，精灵们还算识相，没有提过分的要求。比如在优先级上做文章，要求你给个重组方案。不过，我倒是还想多提一个问题：
对于 C 这种不直接提供@tech{列表}和@tech{哈希表}的语言来说，就用唯一开箱即用的数组，如何完成本任务？既然本任务跟优先级
关系不大，那作者如此设置的用意是什么呢？由于优先级与代表物品类型的字符是一一对应关系，那显然用优先级作为哈希表的关键字
也没有问题；又因为优先级是连续自然数，适合用作数组的索引，且几乎不会浪费存储空间。因此，在本任务中，使用数组与使用@tech{列表}
和@tech{哈希表}差别不算大，请课后自行写程序验证这个说法。

计算机最擅长处理的数据类型显然是数字，然而人类极其不擅长将数字与概念建立联系。因此总想着在程序中给数字起个有意义的名字，
要不然过不了多久自己的程序都会变成天书。但又不得不想出一整套规则用数字来表示概念以便让计算机高效干活。这个拧巴……

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
