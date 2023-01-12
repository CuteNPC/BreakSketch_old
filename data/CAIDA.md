



#### 如何使用已解析好的CAIDA2018数据集？

使用二进制文件读取方式，读取数据文件，如（130000.dat）。格式内容是连续存储的数据包，每个数据包前13字节是五元组（SrcIP+DstIP+SrcPort+DstPort+Protocal），接下来8个字节是时间戳（double，单位：秒）。





#### CAIDA数据集申请

- 账号：liu.peng@pku.edu.cn
- 密码：liu1qaz2020
- 账号、密码、数据请勿外传
- 数据集下载地址：

  - https://data.caida.org/datasets/passive-2018
  - https://data.caida.org/datasets/passive-2016
  - 如果要使用数据集，请在文章里添加引用：
  - "The CAIDA UCSD Anonymized Internet Traces Dataset - [dates used], http://www.caida.org/data/passive/passive_dataset.xml"
- **注意：CAIDA数据集pcap文件不包含帧头**



#### 解析网站原始数据的代码

- ```parser.cpp```: 解析pcap文件和times文件的代码
- 使用说明：```./parser.out  src.pcap  src.times  result```
  - ```src.pcap```: pcap原始文件
  - ```src.times```: 包含精确时间（纳秒级别）的原始文件。每一行对应```src.pcap```中的一个包。
  - ```result```: 解析后的数据存储文件
- ```result```（解析后的数据格式说明）
  - （按顺序）13字节的五元组  +  8字节时间戳（double，单位：秒）
  - 五元组（按顺序）：SrcIP+DstIP+SrcPort+DstPort+Protocal
  - 每个数据文件（例如，dataset/130000.dat）包含一分钟的包数据
  - 每个文件对应的统计数据可以在20180315-130000.UTC中找到（例如，130000.dat对应的是equinix-nyc.dirA.20180315-130000.UTC.anon.pcap.stats）

- 如果想要从CAIDA网站下载新的数据，可以使用上面的parser.cpp进行解析。
