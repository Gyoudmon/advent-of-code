#lang scribble/lp2

@(require "../literacy.rkt")

@(define-aoc-bib D1CC "Calorie Counting" 1)

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{第一天：计点卡路里}

故事原文见 @~cite[D1CC]。故事中的任务将按如下程序结构展开：

@handbook-chunk[<calorie:*>
                (module tamer racket 
                  <定义函数find-maximum-calorie>
                  <定义函数find-maximum-calories>)]

丛林杂草丛生，没法使用交通工具，也没法空降，因此精灵们传统上只能徒步远征。
当小船快靠岸时，精灵们开始清点存货，食物是重点清点对象之一。具体来说，
也就是每个精灵携带食物的卡路里含量。

于是，精灵们把轮流把自己携带的各种肉类、点心、口粮等食物的卡路里数写在纸上，
一行一项，不同精灵之间用空行隔开。比如，假如精灵们出了如下一份清单：

@tabular[#:style 'boxed
         (list (list @racket[1000]) (list @racket[2000]) (list @racket[3000])
               (list @hspace[1])
               (list @racket[4000])
               (list @hspace[1])
               (list @racket[5000]) (list @racket[6000])
               (list @hspace[1])
               (list @racket[7000]) (list @racket[8000]) (list @racket[9000])
               (list @hspace[1])
               (list @racket[10000]))]

从中可以看到五只精灵所携带食物的卡路里：

@itemlist[

 @item{第一个精灵带了三种食物，卡路里含量分别为 @racket[1000], @racket[2000] 和 @racket[3000]，总计 @racket[6000] 卡。}  
 @item{第二个精灵只带了一种食物，卡路里含量为 @racket[4000] 卡。}
 @item{第三个精灵带了两种食物，卡路里含量分别为 @racket[5000] 和 @racket[6000]，总计 @racket[11000] 卡。}
 @item{第四个精灵带了三种食物，卡路里含量分别为 @racket[7000], @racket[8000] 和 @racket[9000]，总计 @racket[24000] 卡。}
 @item{第五个精灵也只带了一种食物，含 @racket[10000] 大卡。}

 ]

当精灵饿了或馋了时，它们需要知道问谁去要零食，即需要知道谁带的卡路里最多。
上面的例子中，是第四只精灵携带的食物卡路里含量最高，达到了 @racket[24000] 大卡。

于是，为伺候好这些精灵，我们需要定义一个函数 @racketid[find-maximum-calorie]
来计算@question{携带食物所提供的卡路里最多的那只精灵到底提供了多少卡}：

@handbook-chunk[<定义函数find-maximum-calorie>
                (code:comment #,($argv [/dev/datin "与卡路里清单关联的输入源"]))
                (define find-maximum-calorie
                  (λ [/dev/datin]
                    <读取-记录-替换-循环>))]

检阅清单的过程就是个简单的@racketcommentfont{读取-记录-替换}循环（@racketidfont{rrsl}, read-record-substitute-loop），
即逐行读取清单，每读到一行，就尝试将该行内容转化成@bold{正整数}，如果能转化成功，说明读到的是卡路里含量，
应当记在当前精灵的账上；否则，准备换个精灵记账。直到没有更多内容为止，函数返回最大值。

@handbook-chunk[<读取-记录-替换-循环>
                (code:comment #,($argv [max:cal "当前已知最高卡路里数"] [self:cal "当前精灵携带的卡路里总数"]))
                (let rrsl ([max:cal 0]
                           [self:cal 0])
                  (define line (read-line /dev/datin))
                  (if (string? line)
                      (let ([c (string->number line 10)])
                        (if (exact-positive-integer? c)
                            (rrsl max:cal (+ self:cal c))
                            (rrsl <确定最大值> 0)))
                      <确定最大值>))]

算法的总体逻辑不复杂，但是却隐藏着一个极大的陷阱。在上面的代码中，为什么有两处地方填充着“确定最大值”？
例子里只强调了精灵与精灵之间用空行分隔，那最后一个精灵后面有没有空行呢？其实没有，因为每个精灵写清单时
只负责与前面的精灵隔开，不负责后面的精灵。因此，循环结束时，最后一只精灵的账目还没有清点，需要额外处理一下。

@handbook-chunk[<确定最大值>
                (max max:cal self:cal)]

接下来就可以通过传入清单文件来执行了：

@tamer-action[#:requires ["../literacy.rkt"]
              (with-aoc-data-from "mee/01.cc.dat" #:do
                  find-maximum-calorie)]

顺便说一遍，上面的函数定义采用了@bold{纯函数式}风格，虽然有两个@bold{变量}
（@racketvarfont{max:cal} 和 @racketvarfont{self:cal}），但实际上它们的值从头到尾都没有变过；
每一轮循环都相当于是一次（递归）函数调用，并且比对着那俩变量创建了新的@bold{位置}，
本该用来修改它们的值也被放到新的位置去了，而它们原来的位置在哪，里面有什么都已经不重要了。
这种思路需要专业引导，初学者一时半会不能适应是正常现象。

当你在冥思苦想上面那都是些啥玩意的时候，精灵们又开始搞事了：天，这么点卡路里迟早会被消耗完的！
这可不行，你必须再找到另外两个所带食物提供了最大卡路里的精灵。这样，一个精灵携带的食物吃完了，
咱还有俩可以继续腐败。

还是看上面的例子，携带食物提供卡路里最多的三只精灵分别是第四只（@racket[24000] 卡）、
第三只（@racket[11000] 卡）和第五只（@racket[10000] 卡），
总计 @racket[45000] 大卡。

得，继续折腾，这次要计算@question{携带食物所提供的卡路里最多的前三只精灵所能提供的卡路里总和}。
精灵们的这两次问话都比较拗口，但其实不难看出，第一个谜题用第二个谜题的思路来说其实就是
@question{携带食物所提供的卡路里最多的前一只精灵所能提供的卡路里总和}，对不对？也就是说，
这两个谜题其实是一个问题，差别在于数量不同。

本来，你可能会想，不就是前三名吗，我设三个变量不就可以了吗？没错，确实可以，但就是感觉很业余的样子。
你就不怕精灵们又突然一拍脑袋说“我们要前六名的”，“前十名的”，“前n名的”…… 没完没了了。
因此，无论是计算机科学、还是数学，我们都希望能找到一个统一的、优雅的问题求解思路，
以免于被脏活累活所困、从而节省时间思考更有趣的问题。

我们需要关注的仍然是卡路里，只不过数量增加了。于是，自然而然地，@bold{不同数量的同类型数据}
可以构成数组、列表、元组等复合类型。在不同的语言里，这些数据结构都可以是很趁手的选择，
不过，为继续函数式风格，我这里选择用列表来实现。而在别的语言里，比如 Python，
不可变的元组才更符合函数式风格；在 C 里，开箱即用的选项就只有数组了，
也几乎没法体现函数式风格。

相比于上面的 @racketid[find-maximum-calorie] 函数，这个复数形式的 @racketid[find-maximum-calories]
多了一个参数 @racketvalfont{n}：

@handbook-chunk[<定义函数find-maximum-calories>
                (code:comment #,($argv [/dev/datin "与卡路里清单关联的输入源"] [n "需要关注的前几名精灵数量"]))
                (define find-maximum-calories
                  (λ [/dev/datin n]
                    (let rrsl (<初始化卡路里列表>
                               [self:cal 0])
                      (define line (read-line /dev/datin))
                      (if (string? line)
                          (let ([c (string->number line 10)])
                            (if (exact-positive-integer? c)
                                (rrsl calories (+ self:cal c))
                                (rrsl <确定最大值列表> 0)))
                          <高卡路里合计>))))]

算法逻辑也还是大同小异，区别在于主循环里
代表最大值的整型参数变为了列表型参数：

@handbook-chunk[<初始化卡路里列表>
                [calories (make-list n 0)]]

最终结果变为了对最大值列表的求和运算：

@handbook-chunk[<高卡路里合计>
                (apply + <确定最大值列表>)]


以及，针对列表设计的确定最大值逻辑：列表中如果存在比当前卡路里含量低的就替换之。

@handbook-chunk[<确定最大值列表>
                (let* ([smaller? (λ [cal] (< cal self:cal))]
                       [idx (index-where calories smaller?)])
                  (if (not idx)
                      calories
                      (list-set calories idx self:cal)))]

至此，任务完成：

@tamer-action[(with-aoc-data-from "mee/01.cc.dat" #:do
                find-maximum-calories 3)]

我们还可以对比一下这两个谜题在逻辑上的一致：

@tamer-action[(with-aoc-data-from "mee/01.cc.dat" #:do
                find-maximum-calorie)
              (with-aoc-data-from "mee/01.cc.dat" #:do
                find-maximum-calories 1)]

不过，还是要谨记，比较两种算法的执行结果并不是严密地证明。
两个结果相等也可能说明两个算法都不对，只不过用题目给的数据测试不出来。
比如，本次解谜就有可能掉进前文所说的陷阱里，导致漏算了最后一只精灵，
而那只精灵又恰好不影响最终结果。可见，这个挑战活动本身也有问题，
远征之前竟然没有清点人数，太不应该了。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
