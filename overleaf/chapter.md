\section{格式要求}
正文宋体小四，正文行间距固定为23磅。

通过空一行（两次回车）实现段落换行，仅仅是回车并不会产生新的段落。 \par

也可以通过 \verb|\par| 命令来新起一段。

\section{各节一级标题}
我是内容

\subsection{各节二级标题}
你是内容

\subsubsection{各节三级标题}
他是内容

\section{字体字号}
{\songti \bfseries 宋体加粗} {\textbf{English}}

{\songti \itshape 宋体斜体} {\textit{English}}

{\songti \bfseries \itshape 宋体粗斜体} {\textbf{\textit{English}}}

\section{编译}
本模板必须使用XeLaTeX + BibTeX编译，否则会直接报错。 本模板支持多个平台，结合sublime/vscode/overleaf都可以使用。