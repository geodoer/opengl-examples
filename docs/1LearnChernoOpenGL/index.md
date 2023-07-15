Cherno OpenGL课程学习笔记。

## 课程内容

1. OpenGL接口的讲解与使用

## 课程评价

1. 讲解的很细致，深入浅出。一层一层去讲，让读者知道某些概念具体是做什么的，而每一层举的又是最小案例
2. 写代码前，会讲原理
3. 写代码时，会带读者去写，并刻意会犯一些错误，再讲解原因并解决
4. 写代码时，会从0开始，然后不断迭代抽象，边写边带观众思考

## 课程链接
课程视频：[【译】TheCherno-OpenGL系列教程_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1Ni4y1o7Au/?vd_source=28f192ec73ac2df3cc6eaa0100ebde4a)

相关链接

1. [作者cherno的网站](patreon.com/thecherno)
2. 译者的代码GITHUB: [Github地址](https://github.com/speauty/ChernoOpenGL)；[GITEE](https://gitee.com/speauty/ChernoOpenGL)
3. [OpenGL的文档：docs.gl](https://docs.gl/)

## 工程说明

| 子工程 | 对应课程 | 说明 | 
| - | - | - |
| 13UseImgui | P22 | 使用ImGUI；添加控制transform的按钮 |
| 14DoubleDraw | P23 | 批量渲染。一帧中，改变偏移矩阵，多次绘制物体 |
| 15TestProject | P24, P25 | 1. 测试框架：创建一个抽象的测试类与菜单<br/>2. 并添加“清除颜色测试” |