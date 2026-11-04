# cmdbox-flash-v1.0

## 下载方式:

### First:

Code->zip->解压->(根据自己意愿换路径)->换好路径后把路径复制下来

### Second:

```text
cd .....(复制的路径搞上去)
make(MakeFile脚本 自动g++编译)
source AIscript/cmdbox.bash
```

## 更新内容:

- 把字典树砍了，以后不会因为ASCII码的大小排序导致 id小的命令 排在id大的命令后面 了.
- 优化了一下id分配。
- 把界面调了一下，tag note id放在cmd下面那一行
- enter界面不会有额外无用的命令例如quit help list了
- enter界面想要退出到搜索界面 按ctrl+c就行,搜索界面想回到正常终端按ctrl+c 或者 tab补全命令。
- ESC假死改了。
- 有时候页面会闪烁的情况也优化了。
- 搜索界面按上下键滚动那块调整了一下。
- 把中文名改成英文名
