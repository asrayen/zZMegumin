\section{公式的使用}
在文中引用公式可以这么写：$a^2+b^2=c^2$这是勾股定理，他还可以表示为$c=\sqrt{a^2+b^2}$，还可以让公式单独一段并且加上编号。注意，公式前请不要空行。
\begin{equation}
\sin^2{\theta}+\cos^2{\theta}=1 \label{eq:pingfanghe}
\end{equation}

还可以通过添加标签在正文中引用公式，如式\eqref{eq:pingfanghe}。

我们还可以轻松打出一个漂亮的矩阵：
\begin{equation}
  \mathbf{A}=
  \left[\begin{matrix}
    1&2&3&4\\
    11&22&33&44\\
  \end{matrix}\right] \times
  \left[\begin{matrix}
    22&24\\
    32&34\\
    42&44\\
    52&54\\
  \end{matrix}\right]
\end{equation}

或者多行对齐的公式：
\begin{equation}
  \begin{aligned}
    f_1(x)&=(x+y)^2\\
          &=x^2+2xy+y^2
  \end{aligned}
\end{equation}


\section{插图的使用}

\LaTeX 环境下可以使用常见的图片格式：JPEG、PNG、PDF、EPS等。当然也可以使用\LaTeX 直接绘制矢量图形，可以参考pgf/tikz等包中的相关内容。需要注意的是，无论采用什么方式绘制图形，首先考虑的是图片的清晰程度以及图片的可理解性，过于不清晰的图片将可能会浪费很多时间。

图示例如下：

\begin{figure}[!htb]
  \centering
  \includegraphics[width=0.3\textwidth]
  {figures/whulogo.png}\\
  \caption{插图示例}
  \label{fig:whu}
\end{figure}

\verb|[htbp]|选项分别是此处、页顶、页底、独立一页。\verb|[width=\textwidth]|让图片占满整行，或\verb|[width=2cm]|直接设置宽度。可以随时在文中进行引用，如图~\ref{fig:whu}，建议缩放时保持图像的宽高比不变。

\section{表格的使用}

表格的输入可能会比较麻烦，可以使用在线的工具，如~\href{https://www.tablesgenerator.com/}{Tables Generator}~能便捷的创建表格，也可以使用离线的工具，如~\href{https://ctan.org/pkg/excel2latex}{Excel2LaTeX}~支持从Excel表格转换成\LaTeX{}表格。\href{https://en.wikibooks.org/wiki/LaTeX/Tables}{LaTeX/Tables}~上及~\href{https://www.tug.org/pracjourn/2007-1/mori/mori.pdf}{Tables in LaTeX}~也有更多的示例能够参考。

\subsection{普通表格}
下面是一些普通表格的示例：

\begin{table}[ht]
  \centering
  \caption{简单表格}
  \label{tab:1}
  \begin{tabular}{|l|c|r|}
    \hline
    我是& 一只 & 普通\\
    \hline
    的& 表格& 呀\\
    \hline
  \end{tabular}
\end{table}

\begin{table}[ht]
  \centering
  \caption{一般三线表}
  \label{tab:2}
  \begin{tabular}{ccc}
    \hline
    姓名& 学号& 性别\\
    \hline
    张三& 001& 男\\
    李四& 002& 女\\
    \hline
  \end{tabular}
\end{table}

\subsection{跨页表格}
跨页表格常用于附录（把正文懒得放下的实验数据统统放在附录的表中），以下是一个跨页表格的示例：

{\centering
  \begin{longtable}{ccccccccc}
  \caption{跨页表格示例} \\
  \toprule
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  \midrule
  \endfirsthead

  \multicolumn{1}{l}{接上一页} \\
  \toprule
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  \midrule
  \endhead

  \bottomrule
  \hline \\
  \multicolumn{9}{r}{{转下一页}} \\
  \endfoot

  \bottomrule
  \endlastfoot    

  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\
  1     & 0 & 5  & 1  & 2  & 3  & 4  &  5 & 6 \\

  \end{longtable}
}

\subsection{统计表格}
要创建占满整个文字宽度的表格需要使用到tabularx，如不需要，使用tabular就行。引用表格与其它引用一样，只需要：表~\ref{tab:3}，统计表格一般是三线表形式。

\begin{table}[ht]
  \centering
  \caption{统计数据表格}
  \label{tab:3}
  \begin{tabularx}{\textwidth}{CCCC}
    \toprule
    序号&年龄&身高&体重\\
    \midrule
    1&14&156&42\\
    2&16&158&45\\
    3&14&162&48\\
    4&15&163&50\\
    \cmidrule{2-4} %添加2-4列的中线
    平均&15&159.75&46.25\\
    \bottomrule
  \end{tabularx}
\end{table}

\section{列表的使用}
下面演示了创建有序及无序列表，如需其它样式，\href{https://www.latex-tutorial.com/tutorials/lists/}{LaTeX Lists}~上有更多的示例。

\subsection{有序列表}
这是一个计数的列表
  \begin{enumerate}
      \item 第一项
          \begin{enumerate}
              \item 第一项中的第一项
              \item 第一项中的第二项
          \end{enumerate}
      \item 第二项
    \begin{enumerate}[label=(\roman*)]
      \item 第一项中的第一项
      \item 第一项中的第二项
    \end{enumerate}
      \item 第三项
  \end{enumerate}

\subsection{不计数列表}
  这是一个不计数的列表
  \begin{itemize}
      \item 第一项
      \begin{itemize}
          \item 第一项中的第一项
          \item 第一项中的第二项
      \end{itemize}
      \item 第二项
      \item 第三项
  \end{itemize}

\section{定理的使用}
\begin{theorem}
  设向量$\boldsymbol a\neq\boldsymbol 0$，那么向量$\boldsymbol b//\boldsymbol a$的充分必要条件是：存在唯一的实数$\lambda$，使$\boldsymbol b=\lambda \boldsymbol a$。
\end{theorem}
\begin{definition}
  这是一条定义。
\end{definition}
\begin{lemma}
  这是一条引理。
\end{lemma}
\begin{corollary}
  对数轴上任意一点$P$，轴上有向线段$\vec {OP}$都可唯一地表示为点$P$的坐标与轴上单位向量$\boldsymbol e_u$的乘积：$\vec {OP}=u \boldsymbol e_u$。
\end{corollary}
\begin{proposition}
  这是一条性质。
\end{proposition}
\begin{example}
  这是一条例。
\end{example}
\begin{remark}
  这是一条注。
\end{remark}