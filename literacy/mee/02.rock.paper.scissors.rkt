#lang scribble/lp2

@(require "../literacy.rkt")

@(define-aoc-bib D2RPS "Rock Paper Scissors" 2)

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-root-story{第二天：猜拳大赛}

故事原文见 @~cite[D2RPS]。故事中的任务将按如下程序结构展开：

@handbook-chunk[<rochambo:*>
                (module tamer racket/base
                  <定义比赛规则函数>
                  <定义策略解密函数>
                  <定义策略规则函数>
                  <定义函数simulate-with-guessed-strategy>
                  <定义函数simulate-with-designed-strategy>)]

精灵们开始在沙滩上搭帐篷。为了让自己的帐篷能够尽可能的离食物仓库近一些，
一场超大型的猜拳大赛正在如火如荼的进行着。

猜拳是一种双人对决类游戏，一场比赛下来双方要来上很多轮。每一轮，
两位选手同时决定在石头、布、剪刀三种手型里任选其一，
然后依据如下简单规则决出胜负：石头砸烂剪刀、剪刀碎掉布、布以柔克刚包裹石头；
而如果双方手型一致，则算平局。

为答谢你昨天的帮助，有一只小精灵愿意给你一份加密策略，声称一定能帮你获得优胜。
这份加密策略看起来是这样的：

@tabular[#:style 'boxed #:sep @hspace[1]
         (list (list "A" "Y")
               (list "B" "X")
               (list "C" "Z"))]

精灵说：“左边那一列代表对手出的手型， A 指代石头，B 指代布，C 指代剪刀。
右边那列……” 不巧，这个时候别的精灵喊她过去帮忙搭帐篷了，她也就真的撇下你
去帮忙了。

没办法，你只好自己猜了，第二列肯定指我自己要出的手型，并且， X 指石头，
Y 指布，Z 指剪刀。考虑到每次都赢对手就太可疑了，这份策略一定是精心设计过的。

得分最高的人将获得比赛优胜，总得分即是每一轮得分的总和。每一轮得分又分为两部分：
手型分（出石头得 @tamer-datum['shape-score 'rock] 分，出布得
@tamer-datum['shape-score 'paper] 分，出剪刀得
@tamer-datum['shape-score 'scissor] 分）和结局分（输了不得分，
平局得 @racket[3] 分，赢了就能得到 @racket[6] 分）。

提炼上述比赛规则，无论是选手可出的手型，还是每轮比赛的结局，都只有有限的几个取值，
这样的数据类型通常会被定义为@bold{枚举类型}。在 Racket 中，枚举类型一般会直接使用
@racket[Symbol]类型的数据来表示，比如 石头(@racket['rock])、布(@racket['paper])、
剪刀(@racket['scissor])；赢(@racket['win])、平局(@racket['draw])、
输(@racket['lose])@handbook-footnote{在真实应用中，猜拳手型可以更为精确的定义为:
 @racket[(define-type RPSShape (U 'rock 'paper 'scissor))]，当函数参数被声明为
 只接受 @racket[RPSShape] 类型的值时，就只能传入上述三个符号，否则编译失败。}。
于是，围绕这两个枚举类型，我们可以定义如下四个函数分别计算手型分、结局分、本轮得分，
以及每一轮的结局：

@handbook-chunk[<定义比赛规则函数>
                (code:comment #,($argv [shape "当前手型"]))
                (define (shape-score shape)
                  (case shape
                    [(rock)    1]
                    [(paper)   2]
                    [(scissor) 3]
                    [else      0]))

                (code:comment #,($argv [outcome "当前结局"]))
                (define (outcome-score outcome)
                  (case outcome
                    [(win)  6]
                    [(draw) 3]
                    [(lose) 0]
                    [else   0]))

                (code:comment #,($argv [sf:play "本轮己方手型"] [outcome "本轮结局"]))
                (define (round-score sf:play outcome)
                  (+ (shape-score sf:play)
                     (outcome-score outcome)))

                (code:comment #,($argv [op:play "对手出手手型"] [sf:play "我方出手手型"]))
                (define (round-end op:play sf:play)
                  (cond [(eq? sf:play op:play) 'draw]
                        [(eq? sf:play 'rock)
                         (if (eq? op:play 'scissor) 'win 'lose)]
                        [(eq? sf:play 'paper)
                         (if (eq? op:play 'rock) 'win 'lose)]
                        [(eq? sf:play 'scissor)
                         (if (eq? op:play 'paper) 'win 'lose)]
                        [else #false]))]

其实，你并不真的确定那只精灵是真想帮你，还是另有所图。不管怎么说，即使想自己找到
一个靠谱的策略，先试着跟着这个策略走也未尝不可，起码能有个比较。于是，从上面那个例子
可以得到如下信息：

@itemlist[

 @item{第一轮，对方出石头（A），你出布（Y）。你获胜，因而得到 @racket[8] 分，其中
  @tamer-datum['shape-score 'paper] 分因为手型布，
  @tamer-datum['outcome-score 'win] 分因为结局赢；}

 @item{第二轮，对方出布（B），你出石头（X）。你输了，因而得到 @racket[1] 分，其中
  @tamer-datum['shape-score 'rock] 分因为手型石头，
  @tamer-datum['outcome-score 'lose] 分因为结局输；}
 
 @item{第三轮，两人都出了剪刀，平局，因而得到 @racket[6] 分，即
  手型剪刀 @tamer-datum['shape-score 'scissor]
  分加上平局分 @tamer-datum['outcome-score 'draw] 分。}
                                       
 @item{你的最终得分是 @racket[15] （@racket[8] @racket[+] @racket[1] @racket[+] @racket[6]）。}

 ]

于是，抱着试试看的心态，我们需要一个函数 @racketid[simulate-with-guessed-strategy]
来计算@question{严格跟着那份自己理解的加密策略走能得到多高的分}：

@handbook-chunk[<定义函数simulate-with-guessed-strategy>
                (code:comment #,($argv [/dev/datin "与加密策略关联的输入源"]))
                (define simulate-with-guessed-strategy
                  (lambda [/dev/datin]
                    <读取-模拟-累加-循环>))]

回忆昨天的思路，今天的任务是一个@racketcommentfont{读取-模拟-累加}循环（@racketidfont{rsal}, read-simulate-accumulate-loop），
即逐行读取策略指导，每读到一行，就按照该轮策略模拟比赛，然后将结果累加。直到没有更多内容为止，函数返回总得分。

@handbook-chunk[<读取-模拟-累加-循环>
                (code:comment #,($argv [total "比赛结束时的总得分"]))
                (let rsal ([total 0])
                  (define line (read-line /dev/datin))
                  (if (string? line)
                      (let ([op:ch (string-ref line 0)]
                            [sf:ch (string-ref line 2)])
                        (rsal (+ total <模拟本轮比赛>)))
                      total))]

本任务的总体结构比昨天的简单，但细节是魔鬼。头脑风暴一下，模拟比赛的过程似乎还挺繁琐，
要先解密策略文件：

@handbook-chunk[<定义策略解密函数>
                (code:comment #,($argv [ch "加密策略文件中的字符"]))
                (define (char->shape ch)
                  (case ch
                    [(#\A #\X) 'rock]
                    [(#\B #\Y) 'paper]
                    [(#\C #\Z) 'scissor]
                    [else #false]))]

一般来说，诚意十足的精灵给的策略文件不会出现乱七八糟的输入，因此可以放心直接读取。
但是在真实的软件中，一定要检查用户输入是否符合契约。这里，当出现其他字符时，
我们简单返回 @racket[#false]，告诉 @racketid[rsal] 可以跳过这一行。
这样就能正确模拟比赛、得到该轮得分了：

@handbook-chunk[<模拟本轮比赛>
                (let ([op:play (char->shape op:ch)]
                      [sf:play (char->shape sf:ch)])
                  (if (and (symbol? op:play)
                           (symbol? sf:play))
                      (let ([outcome (round-end op:play sf:play)])
                        <本轮比赛得分>)
                      0))]

注意，别忘了每轮得分都由两部分构成：手型分和结局分：

@handbook-chunk[<本轮比赛得分>
                (round-score sf:play outcome)]

接下来就可以通过传入策略文件来模拟了：

@tamer-action[#:requires ["../aoc.rkt"]
              (with-aoc-data-from "mee/02.rps.dat" #:do
                simulate-with-guessed-strategy)]

那只精灵帮其他精灵搭完帐篷后又溜出来找你了，继续之前的规则解读：“右边那一列决定了
你策略上的输赢， X 指输掉比赛，Y 指跟对手打平，Z 指你必须赢。总之，祝你好运。”

这么一解释，这文件倒是确实更像个机密策略了。比如，按照前面的例子，这次可以得到如下信息：

@itemlist[

 @item{第一轮，对方出石头（A），你需要平局（Y），也只能出石头。因而得到 @racket[4]
  分（@tamer-datum['shape-score 'rock] @racket[+] @tamer-datum['outcome-score 'draw]）；}

 @item{第二轮，对方出布（B），你出石头以策略上输掉（X）比赛，因而得到 @racket[1]
  分（@tamer-datum['shape-score 'rock] @racket[+] @tamer-datum['outcome-score 'lose]）；}
 
 @item{第三轮，你需要出石头以赢得（Z）对手的剪刀（C），最后得到 @racket[7]
  分（@tamer-datum['shape-score 'rock] @racket[+] @tamer-datum['outcome-score 'win]）。}
                                       
 @item{你的最终得分是 @racket[12] （@racket[4] @racket[+] @racket[1] @racket[+] @racket[7]）。}

 ]

于是，我们先来补全真正的策略规则函数:

@handbook-chunk[<定义策略规则函数>
                (code:comment #,($argv [ch "加密策略文件中的字符"]))
                (define (char->outcome ch)
                  (case ch
                    [(#\X) 'lose]
                    [(#\Y) 'draw]
                    [(#\Z) 'win]
                    [else #false]))

                (code:comment #,($argv [op:play "对手手型"] [outcome "策略结局"]))
                (define (smart-shape op:play outcome)
                  (cond [(eq? outcome 'draw) op:play]
                        [(eq? op:play 'rock)
                         (if (eq? outcome 'win) 'paper 'scissor)]
                        [(eq? op:play 'paper)
                         (if (eq? outcome 'win) 'scissor 'rock)]
                        [(eq? op:play 'scissor)
                         (if (eq? outcome 'win) 'rock 'paper)]
                        [else #false]))]

然后是真策略函数 @racketid[simulate-with-designed-strategy]，
以计算@question{严格跟着那份精灵的加密策略走能得到多高的分}：

@handbook-chunk[<定义函数simulate-with-designed-strategy>
                (code:comment #,($argv [/dev/datin "与加密策略关联的输入源"]))
                (define simulate-with-designed-strategy
                  (lambda [/dev/datin]
                    (let rsal ([total 0])
                      (define line (read-line /dev/datin))
                      (if (string? line)
                          (let ([op:ch (string-ref line 0)]
                                [sf:ch (string-ref line 2)])
                            (rsal (+ total <模拟策略>)))
                          total))))]

这两个主函数大同小异，唯一的不同则关乎于每轮得分的构成。
对于你自己猜测的策略，其本质是@question{已知手型求结局}；
而精灵策略的本意则是@question{已知结局求手型}。

@handbook-chunk[<模拟策略>
                (let* ([op:play (char->shape op:ch)]
                       [outcome (char->outcome sf:ch)]
                       [sf:play (smart-shape op:play outcome)])
                  (if (and (symbol? op:play)
                           (symbol? sf:play))
                      <本轮比赛得分>
                      0))]

至此，任务完成：

@tamer-action[(with-aoc-data-from "mee/02.rps.dat" #:do
                simulate-with-designed-strategy)]

不过，貌似就本任务中的输入数据来说，这个机密策略还不如自己瞎猜的那个。

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-reference[]
