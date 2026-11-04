# cmdbox.bash —— bash 集成脚本（CmdBox Flash）

一个文件，全部逻辑在里面。作用：在命令行按 **Ctrl+G** 弹出 cmdbox 候选界面，选中的命令**填进命令行但不执行**。

不依赖任何第三方工具，只要 bash 4+（因为有 `bind -x`）和编译好的 `cmdbox` 二进制。

---

## 它怎么工作

1. `bind -x '"\C-g":_cmdbox_pick'` —— 把 Ctrl+G 绑到函数 `_cmdbox_pick`。
2. 函数里执行 `chosen=$("$_cmdbin" --pick)`。`--pick` 模式下 cmdbox 把界面画在 `/dev/tty`，**stdout 只吐一行**（选中的命令，或者什么都不吐）。
3. 拿到非空结果 → 写 `READLINE_LINE`（整行替换成选中的命令）和 `READLINE_POINT`（光标放到行尾）。空结果 → 什么都不做，命令行原样不动。

**“填进行里但不执行”就是这一点的全部意思**：脚本只改 `READLINE_LINE`，不调用 `eval`，也不替你按回车。

中间还有一段 `stty -g` / `stty sane` / `stty "$saved"`，原因见文末「三个容易踩的点」第 3 条。

---

## 依赖：二进制在哪

脚本会自动找，顺序是：

1. 脚本自己所在目录（`ai脚本/cmdbox`）
2. 上一级，也就是仓库根（`cmdbox-flash-v1.0/cmdbox`）
3. `PATH` 里的 `cmdbox`

想跳过查找，直接指定：

```sh
export CMDBOX_BIN=/path/to/cmdbox
```

二进制没编译好时，函数会往 stderr 打一行提示（`cd 仓库 && make`）并返回 1，**不会污染命令行**，也不会有别的副作用。

> 编译：
>
> ```sh
> cd ~/cmdbox-flash-v1.0
> make
> ```

---

## 安装

在 `~/.bashrc` 里加一行（已经有旧的就**替换**，别两行都留——旧脚本把路径写死成 `~/cmdbox-标准/cmdbox`，绑的是 v1.0 那个程序，两个都留着会抢同一个 Ctrl+G）：

```sh
[ -f ~/cmdbox-flash-v1.0/ai脚本/cmdbox.bash ] && source ~/cmdbox-flash-v1.0/ai脚本/cmdbox.bash
```

`[ -f ... ]` 是防护：文件不在就静默跳过，不会让新终端报错。

然后 `source ~/.bashrc`，或者开个新终端。

---

## 验证

```sh
bind -X | grep cmdbox          # 应输出  "\C-g" "_cmdbox_pick"
```

然后在命令行按 Ctrl+G：

- 界面出现，↑↓ 选择，Tab 或 Ctrl+C 退出
- 选中一条 → 命令行那一行变成该命令，光标在行尾，**没有执行**
- 按 Ctrl+C 放弃 → 命令行一个字都不动

---

## 换触发键

改脚本最后一行，例如：

```sh
bind -x '"\C-t":_cmdbox_pick'
```

注意避开 bash 自带键（Ctrl+R 反查历史、Ctrl+X、Ctrl+W 等），撞了会被覆盖或行为混乱。

---

## 三个容易踩的点

1. **`--pick` 模式的 stdout 必须干净**。`$(...)` 会把 stdout 收到的**所有**字节当成命令行内容。所以界面必须画在 `/dev/tty`，程序里任何提示语、日志都不能往 stdout 写——否则会被当成“选中的命令”填进命令行。
2. **只有空 = 放弃**。脚本靠 `[ -n "$chosen" ]` 判断，所以“不输出任何东西”和“输出一个空行”对 bash 是等价的（`$()` 会吃掉尾部换行），两种都能正确表达放弃。
3. **必须先把终端摆回正常模式**。`bind -x` 执行函数时 readline 还没退出，终端停在它自己的 cbreak 模式（关了 `icanon`/`echo`/`icrnl`）。cmdbox 启动会把当前设置当成“原始状态”保存，退出时还原成它——于是 `:add` 那种等一行输入的界面永远等不到换行，也不回显。脚本用 `stty -g` 存、`stty sane` 摆正、选完 `stty "$saved"` 还回去，就是为了这个。

---

## 本文件夹改名了怎么办

脚本会自己定位，**不用改脚本里的路径**，但要改一处：

- `~/.bashrc` 里 `source` 那一行的路径（本 README 的“安装”一节）。

如果二进制被挪到了既不在脚本旁边、也不在仓库根、也不在 `PATH` 的地方，才需要设 `CMDBOX_BIN`。
