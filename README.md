# LDACplus
A LDA toolkit by stl in c++.

20150716

LDA主题模型，用gibbs采样做训练。只实现了训练算法，没有进一步实现推理算法，因为暂时不需要，不过实现起来也不难。

需要一个词表文件，包含训练样本中的所有词语。
需要一个训练样本文件，样本文件中的每一个行代表一个文档，每一行中的词语表示文档中的词语，词语之间用空白符分隔。词频用词语本身了来表示：词语的词频是n，则词语本身在训练样本文档中就出现n次。词语之间的顺序无所谓，不影响训练效果。
