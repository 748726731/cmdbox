# CmdBox Flash v1.0

**cmdbox-v1.0 的轻量版。** 同一套终端命令收藏夹，砍掉字典树和整屏搜索，只留下「输入即筛、Tab 补全、Ctrl+G 填命令行」。
7 个源文件、629 行 C++、一个 67 KB 的二进制，没有第三方依赖，适合直接当 bash 插件用。

> 交互式代码地图见 [`网页介绍.html`](网页介绍.html)：左边是 main 的时间线 / 链表 / 终端 / KMP，右边同步高亮源码，另有一个能真按键的模拟器。

---

## 1. 它是什么

一个跑在终端里的命令本：

- 所有命令存在二进制同目录的 `data.txt`，一行一条：`id|命令|注释|标签`。
- 输入任意子串，实时过滤 **命令 / 注释 / 标签 / id**（KMP 子串匹配，不是只匹配开头）。
- `↑↓` 选，`Tab` 补全，`Ctrl+C` 退出。
- `--pick` 模式配合 bash 脚本：按 `Ctrl+G` 弹出候选，选中后**只把命令填进当前命令行，不执行**。

和 v1.0 最大的区别是「更小、更少状态」：没有 trie，没有 Enter 搜索页，没有 `:list/:help/:quit`。

---

## 2. 快速开始

仓库自带 `Makefile`（`database/inlcude` 的拼写就是源码里的真实目录名，别改成 include）：

```sh
make
```

运行：

```sh
./cmdbox
# 或者
make run
```

`make clean` 清掉二进制，`make install` 装到 `~/.local/bin/cmdbox`。

首次运行如果同目录没有 `data.txt`，会装入 6 条预制命令（git / g++ / make / ls）并立刻写出一份。

### 当插件用（Ctrl+G）

```sh
# ~/.bashrc 里加一行
[ -f ~/cmdbox-flash-v1.0/ai脚本/cmdbox.bash ] && source ~/cmdbox-flash-v1.0/ai脚本/cmdbox.bash
```

```sh
source ~/.bashrc
# 之后在 bash 里按 Ctrl+G
```

脚本会在「自己所在目录 → 上一级（仓库根）→ PATH」里找 `cmdbox`，也可以用 `CMDBOX_BIN=/path/to/cmdbox` 指定。选中后写 `READLINE_LINE`，光标放到行尾，**不执行**。

---

## 3. 按键与指令

| 按键 | 行为 |
|---|---|
| 普通字符 | 追加到输入，候选实时过滤，选中项回到第一条 |
| `Backspace` | 删一个完整 UTF-8 字符（中文不会只删半个） |
| `↑` / `↓` | 在候选里移动，边界有检查 |
| `Tab` | 把选中命令填进输入框；`--pick` 模式下填完立即退出，把这一行交回 bash |
| `Enter` | 只有 `:` 开头才当指令；其它输入**直接清空，不搜索、不执行** |
| `Ctrl+C` | 退出程序；在 `:add` / `:edit` 的输入界面里只取消这一次输入 |

指令只有三个：

```text
:add            依次输入 命令 / 注释（可空） / 标签（默认 misc）
:del 编号        删除一条
:edit 编号       三个字段回车保留旧值
```

`:quit`、`:list`、`:help` 在 Flash 里**不存在**，输入会得到「未知命令」。退出统一用 `Ctrl+C`。

编号规则：`nextFreeId` 从 1 往上找最小的空号；新命令按 id 升序插入。所以删掉 `#3` 后再 `:add`，新命令会拿回 `#3`，列表始终按编号排。

---

## 4. 和 cmdbox-v1.0 的差距

| 维度 | cmdbox-v1.0 | cmdbox-flash-v1.0 |
|---|---|---|
| 规模 | 8 个源文件 / 715 行 / 31 个函数 | 7 个源文件 / 629 行 / 27 个函数 |
| 二进制 | 84664 B | 67048 B |
| 候选来源 | `trie.cpp` 字典树，按**前缀**下钻 | 每帧扫链表 + KMP，按**子串**匹配 cmd/note/tag/id |
| `Enter` | 不带冒号 → 整屏全文搜索页 | 不带冒号 → 清空输入，什么都不做 |
| 指令 | `:list :add :edit :del :help :quit` | 只有 `:add :edit :del` |
| 编号 | `nextId` 只增不减，按首次出现顺序 | `nextFreeId` 复用空号，按 id 升序插入 |
| 写盘 | 先写 `.tmp` 再 `rename` | 直接 `ofstream` 覆盖写 |
| 构建 | `Makefile`（`-Wall -O2`，带 install） | `Makefile`（同样的目标，头文件路径用 `inlcude`） |
| 单按 ESC | 等待后续字节，假死 | `select` 零超时，直接当 `Other` 返回 |
| `:add` 时 Ctrl+C | 杀进程 | `askLine` 返回 false，取消并回列表 |
| 候选滚动 | `start = sel>=6 ? sel-5 : 0` 现算 | `top` 持久，`sel` 越界才滚 |
| 文件读取校验 | `parts.size()!=4` 跳过，`parseInt` 失败跳过 | **没有**列数/解析校验 |
| 界面表头 | `Enter搜索  :help帮助` | 无（少两个入口） |

### 为什么砍

- **删 trie**：省掉 `trie.h/trie.cpp` 和 5 个函数（67 行），App 里不再挂树，启动不再 `commit` 重建。代价是每次按键都 `O(n)` 扫全表重建候选——命令几十条时无感，上千条就该把「每帧重建」修掉（见第 7 节）。
- **删 Enter 搜索**：实时筛选已经覆盖了「找命令」，整屏搜索页是第二套重复交互，还多一个「按任意键返回」的状态。删掉后 `Enter` 只服务 `:` 指令，main 的按键分支少一整块。
- **删 `:list/:help/:quit`**：`list` 和实时筛选重复，`help` 和界面表头重复，`quit` 和 `Ctrl+C` 重复。`runCommand` 只剩三个分支，返回类型也从 `bool` 变成 `void`。
- **直接写盘**：少了临时文件和 `rename`，代码更短；风险是写到一半断电会留下半截 `data.txt`。要拿它存重要命令，建议换回 v1.0 的原子写。

### 补了什么

- **ESC 不假死**：`readKey` 在 `c==27` 时用 `select(1,&s,0,0,&tv)` 零超时探后续字节，没有就 `return Other`，不会卡在 `read` 上。
- **命令界面 Ctrl+C 不杀进程**：`askLine` 自己调 `readKey`，`Quit` 返回 false；`addFlow/editFlow` 看到 false 就把 `msg` 设成「已取消」退回列表。
- **编号有序**：`nextFreeId` + 按 id 升序插入，候选顺序不再取决于 data.txt 的书写顺序。
- **候选滚动**：`draw(..., int &top)` 用持久的 `top`，选中项移出 1–6 行时窗口跟着走。

---

## 5. 代码结构

```text
Makefile                          make / make run / clean / install
LICENSE                           MIT
main.cpp                  142 行  入口、主循环、按键分派、--pick
database/
  inlcude/                          ← 目录名就是 inlcude
    list.h                  35 行   链表与 App 的结构体声明
    storage存放.h            6 行   读写接口
  src/
    list.cpp                86 行   双向链表、nextFreeId、按 id 插入
    storage存放.cpp          97 行   parseInt / splitLine / 读写 data.txt
backend/
  include/{Enter.h,kmp_search.h}
  src/
    Enter.cpp              120 行   :add/:del/:edit、askLine、runCommand
    kmp_search.cpp          83 行   getnext / kmpAll / highlight
frontend/
  include/{linux.h,screen.h}
  src/
    linux.cpp               78 行   rawOn/rawOff/readKey
    screen.cpp              23 行   draw
ai脚本/
  cmdbox.bash                      Ctrl+G 集成
  README.md                        脚本说明
data.txt                          7 条命令（含一条 Hello 测试数据）
下一版本升级内容.txt                待办
网页介绍.html                       交互式代码地图
```

编译期只有一个翻译单元：`main.cpp` 顶部 `#include` 了 6 个 `.cpp`（list / storage / kmp / linux / Enter / screen），加上 main 自己正好 7 个源文件，所以没有链接步骤，也不需要目标文件。

---

## 6. data.txt 格式

```text
id|命令|注释|标签
1|git add -A|暂存所有改动|git
2|git commit -m "fix"|提交并写明本次修复内容|git
...
7|Hello||tag
```

- 字段内不能出现 `|`：`:add` / `:edit` 会用 `hasBar` 拒绝，否则读回来会被切成更多段。
- 注释可以为空；标签为空时默认 `misc`。
- 文件不存在 → 装 6 条预制命令；文件存在但**列数不对**，这一版不会跳过（见第 7 节）。
- 读取时 `\r` 会被去掉，兼容 Windows 换行。
- id 是 `int`，`parseInt` 限制最多 9 位；解析失败时 id 保持 0（v1.0 会跳过该行）。

---

## 7. 这一版修了什么，还留着什么

对照 v1.0 的 `下一版本升级内容.txt`（`id排序 / 宽度折断 / ESC假死亡 / 命令界面Ctrl+C / 颜色码重发 / oldT锁定 / 删Enter搜索`）：

**已处理**

- ✅ **id 排序**：`nextFreeId` + 升序插入，删号可复用。
- ✅ **ESC 假死**：`select` 零超时。
- ✅ **命令界面 Ctrl+C**：`askLine` 返回 false 取消，不再杀进程。
- ✅ **删掉 Enter 搜索**：回车只处理 `:` 指令。
- ✅ **颜色码重发**：`highlight` 用 `last` 记住上一段的命中状态，连续同色只在开头发一次颜色码（v1.0 每个字符头都重发 `RESET` + 颜色）。

**Flash 的 `下一版本升级内容.txt` 里还开着的**

1. **宽度太小会折断**：没有检测终端宽度，长命令/窄窗口会换行破坏布局。
2. **oldT 正常锁定**：`oldT` 仍是全局 `termios`，多实例/嵌套调用时保存与恢复不够严谨。
3. **main 的 while 每轮都重建链表**：每按一个键都 `O(n)` 扫全表 + 拼字符串 + KMP；命令多了会卡。
4. **ASCII 的 id 也可以排序**：现在 id 必须是整数；如果 id 允许字符串，排序规则要重新定义。

**这一版新引入/仍然存在的**

- `loadItems` 不校验 `parts.size()`，坏行可能越界读 `parts[1..3]`（v1.0 有校验）。
- `saveItems` 非原子写，断电可能留下半截文件。
- `parseInt` 是 `void`，非法 id 静默变成 0，坏了也不报。
- 没有终端宽度处理，长行会折行（同上第 1 条）。
- `:add` 成功只提示「已添加」，不显示新编号。

---

## 8. 设计取舍一句话

Flash 不是「功能更多的 v1.0」，而是「把 v1.0 里为演示数据结构而存在的部分拿掉」：trie、全屏搜索、帮助系统、构建系统都不影响「把一条命令塞进命令行」这件事。
代价是查询从「按前缀下钻」退化成「每帧全表扫」，以及写盘不再抗断电——所以它更适合当插件，而不是当唯一的命令仓库。

---

## 9. 许可

Flash 沿用上游的 MIT 许可证，见仓库根目录的 `LICENSE`（Copyright (c) 2026 748726731）。
