# 词法分析器

## 未完成的功能：

​		1、字符不被空格中断时的报错功能，例如123abc会被识别为123、abc两个字符

​		2、界符、运算符的支持

​		3、对于指定文件的检测

​		4、回退时对缓存区指针、文件指针的修改

​		5、从文件中获取关键字表

## 待完善的功能：

​		1、回退功能需要额外读取文件

​		2、win和 linux下换行的不同

## 待测试的功能：

​		1、超过一个缓存的试验

​		2、字符被缓存区截断

## 修复的功能：

​		1、关键字、自定义标识符、无符号常量、无符号浮点常量的Token串插入、查找功能

​		2、简单的报错机制

​		3、文件可以正确的读取到末尾了

0.1  4-22-2020