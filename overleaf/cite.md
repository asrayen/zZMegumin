% Chapter 3

\chapter{引用与链接}

\section{脚注}
注释是对论文中特定名词或新名词的注解。注释可用页末注或篇末注的一种。选择页末注的应在注释与正文之间加细线分隔，线宽度为1磅，线的长度不应超过纸张的三分之一宽度。同一页类列出多个注释的，应根据注释的先后顺序编排序号。字体为宋体5号，注释序号以“\circled{1}、\circled{2}”等数字形式标示在被注释词条的右上角。页末或篇末注释条目的序号应按照“\circled{1}、\circled{2}”等数字形式与被注释词条保持一致。示例：这里有个注释\footnote{我是解释注释的}。

\section{引用文中小节}\label{sec:ref}
如引用小节~\ref{sec:ref}

\section{引用参考文献}
这是一个参考文献引用的范例\cite{kuhn2004man}

还可以采用上标的引用方式\upcite{江泽民1989能源发展趋势及主要节能措施}

引用多个文献\cite{kuhn2004man,江泽民2008新时期我国信息技术产业的发展,江泽民1989能源发展趋势及主要节能措施}

文献引用需要配合BibTeX使用，很多工具可以直接生成BibTeX文件（EndNote, NoteExpress, 百度学术，谷歌学术），此处不作介绍。

\section{链接相关}
模板使用了hyperref处理相关链接，使用\verb|href|可以生成超链接，链接周围的方框在打印时不会出现。可以在cls文件中修改相应的hypersetup项来关闭方框：\verb|\hypersetup{hidelinks}|。如果需要输出网址，可以使用\verb|url|包，示例：\url{https://github.com}。



% Encoding: UTF-8

@article{江泽民1989能源发展趋势及主要节能措施,
  title={能源发展趋势及主要节能措施},
  author={江泽民},
  journal={上海交通大学学报},
  volume={23},
  number={3},
  pages={1--16},
  year={1989},
  language={Chinese}
}

@book{kuhn2004man,
  title={The Man who Changed China: The Life and Legacy of Jiang Zemin},
  author={Kuhn, R.L.},
  isbn={9781400054749},
  lccn={ocm55624581},
  year={2004},
  publisher={Crown Publishers}
}

@article{江泽民2008新时期我国信息技术产业的发展,
  author={江泽民},
  title={新时期我国信息技术产业的发展},
  journal={上海交通大学学报},
  year={2008},
  volume={42},
  number={10},
  pages={1589-1607},
  month={1}
}


[1] 刘学良. 多智能体系统协调控制中的若干问题研究[D/OL]. 广州: 华南理工大学, 2012[2025-03-29]. https://kns.cnki.net/KCMS/detail/detail.aspx?dbcode=CDFD&dbname=CDFD1214&filename=1012452816.nh.
[2] 高晶英. 多智能体系统协调控制若干问题研究[D/OL]. 长春: 吉林大学, 2017[2025-03-29]. https://kns.cnki.net/KCMS/detail/detail.aspx?dbcode=CDFD&dbname=CDFDLAST2017&filename=1017152360.nh.
[3] 彭科. 带领导者的多智能体系统中的一致性问题研究[D/OL]. 上海: 上海交通大学, 2009[2025-03-29]. https://kns.cnki.net/KCMS/detail/detail.aspx?dbcode=CDFD&dbname=CDFD0911&filename=2010113233.nh.
