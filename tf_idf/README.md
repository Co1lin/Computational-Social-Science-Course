本程序计算输入文件data.in中已经分好词的词语的TF、IDF、TF_IDF值，并将Rank List结果输出到"result.out"。

Tips：  
Mac版本使用Clion编译；Windows版本使用VS（2019）编译。对应文件夹是各自的工程文件夹，里面可以找到相应的源文件、可执行文件。  

data.in的编码问题是导致读入词汇出错的常见问题。经实测，Windows下用Notepad++编辑的ANSI编码格式可以正常读入，编码为UTF-8时读入的是乱码。Mac下一般情况可正常读入。  

data.in文件对应（上次课程中的）poem_words.txt文件（程序设计时是按照这个文件的格式设计的）。此文件除了可能的编码问题外，里面还有类似于“？？”、“21”的非正常词汇，程序已将其排除在外。  

目前Mac和Windows下，跑出的结果相近，但是IDF的Rank List（里面主要是生僻字）可能由于生僻字的编码原因，结果完全不同。此外，两种环境下读入的词汇数量并不一致，原因未知。  

Mac下由于Clion中可执行文件的工作目录并不与main.cpp相同，因此读取、输出文件的路径不同。因此添加了main2.cpp，这份代码的文件读入与输出直接就在main2.cpp编译出的文件exec.out所在的目录。（将data.in和exec.out放在一起运行即可。）
