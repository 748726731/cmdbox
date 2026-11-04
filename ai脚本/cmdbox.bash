# CmdBox Flash 的 bash 集成脚本
# 在 ~/.bashrc 里写一行(路径指向本文件):
#   [ -f ~/cmdbox-flash-v1.0/ai脚本/cmdbox.bash ] && source ~/cmdbox-flash-v1.0/ai脚本/cmdbox.bash
# 作用:按 Ctrl+G 弹出候选界面,选中的命令直接填进当前命令行(不执行)。
# 原理:cmdbox --pick 把界面画在 /dev/tty 上,stdout 只吐一行选中的命令。

#自动找 cmdbox 二进制,顺序:①脚本自己所在的文件夹 ②上一级(仓库根) ③PATH 里
#哪都找不到就留空,按 Ctrl+G 时会提示尚未编译。
#想指到别处,设环境变量 CMDBOX_BIN=/path/to/cmdbox 即可跳过上面的查找。
_cmdself=$(cd "$(dirname "${BASH_SOURCE[0]}")" 2>/dev/null && pwd)
_cmdbin=""
for _cand in "$_cmdself/cmdbox" "$_cmdself/../cmdbox" "$(command -v cmdbox 2>/dev/null)"; do
    if [ -n "$_cand" ] && [ -x "$_cand" ]; then
        _cmdbin=$(cd "$(dirname "$_cand")" && pwd)/$(basename "$_cand")
        break
    fi
done
[ -n "${CMDBOX_BIN:-}" ] && _cmdbin="$CMDBOX_BIN"
unset _cand

_cmdbox_pick()
{
    #还没编译就别让bash报一屏天书
    if [ -z "$_cmdbin" ]; then
        printf '没找到 cmdbox 二进制:脚本旁边、上一级、PATH 里都没有\n' >&2
        printf '先编译: cd %s && make\n' "$(dirname "$_cmdself")" >&2
        return 1
    fi
    if [ ! -x "$_cmdbin" ]; then
        printf 'cmdbox 不可执行: %s\n' "$_cmdbin" >&2
        return 1
    fi

    local saved chosen

    #bind -x 跑这个函数时,bash 的行编辑(readline)还没退出,
    #终端此刻停在 readline 自己的 cbreak 模式(关了 icanon/echo/icrnl)。
    #cmdbox 启动会把"当前设置"当原始状态存着,退出时还原成它,
    #于是 :add 那种"等一行输入"的界面永远等不到换行、也不回显。
    #所以:先摆回正常终端再叫 cmdbox,回来再把 readline 的模式交还给它。
    saved=$(stty -g) || return 1
    stty sane
    #$(...)只收cmdbox的stdout。--pick模式下stdout只有那一行,
    #界面画在/dev/tty,不会混进来。
    chosen=$("$_cmdbin" --pick)
    stty "$saved" 2>/dev/null

    #空=用户按Ctrl+C放弃,命令行一个字都不动
    if [ -n "$chosen" ]; then
        READLINE_LINE="$chosen"       #整行换成选中的命令
        READLINE_POINT=${#chosen}     #光标放到行尾
    fi
}

bind -x '"\C-g":_cmdbox_pick'   #Ctrl+G触发;想换键就改这里
