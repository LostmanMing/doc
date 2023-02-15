<div align='center'>
  <h1>Vim 笔记</h1>
</div>

# **Contents**
- [**Contents**](#contents)
- [**资源**](#资源)
- [**Vim 快捷键**](#vim-快捷键)
  - [**Navigation**](#navigation)
  - [**Indentation and typography**](#indentation-and-typography)
  - [**Search**](#search)
  - [**Edit in `NORMAL` Mode**](#edit-in-normal-mode)
  - [**Enter `INSERT` Mode from `NORMAL` Mode**](#enter-insert-mode-from-normal-mode)
  - [**`VISUAL` mode shortcuts**](#visual-mode-shortcuts)
  - [**Macro**](#macro)
  - [**File**](#file)
  - [**Split Screen and Tab**](#split-screen-and-tab)
  - [**Vim Settings**](#vim-settings)
  - [**常用技巧**](#常用技巧)


# **资源**
Vim 自带一个交互式的教程，内含你需要了解的最基础的信息，你可以通过终端运行以下命令打开教程：

    $ vimtutor
- [Vim 学习项目](https://github.com/dofy/learn-vim)
# **Vim 快捷键**
## **Navigation**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`h`|left|
|`n`|`j`|down|
|`n`|`k`|up|
|`n`|`l`|right|
|`n`|`w`|One word **forward**, *beginning of word*|
|`n`|`e`|One word **forward**, *end of word*|
|`n`|`b`|One word **backward**|
|`n`|`0`|Beginning of line|
|`n`|`^`|Beginning of line|
|`n`|`$`|End of line|
|`n`|`gg`|Beginning of file|
|`n`|`H`| 移动到屏幕顶端|
|`n`|`M`| 移动到屏幕中间|
|`n`|`L`| 移动到屏幕底部|
|`n`|`:[num-of-line] + Enter` or `<N> gg`|Go to a **specific** line|
|`n`|`ctrl + o`|takes cursor back to older positions|
|`n`|`ctrl + i`|takes cursor back to newer positions|
|`n`|`ctrl + d`|向下移动半页|
|`n`|`ctrl + u`|向上移动半页|
|`n`|`ctrl + f`|向下移动一页|
|`n`|`ctrl + b`|向上移动一页|

## **Indentation and typography**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`>>`|向右缩进当前行|
|`n`|`<<`|向左缩进当前行|
|`n`|`<N> << / >>`|向左/右缩进`N`行|
|`n`|`< / > G`|将从当前行到文件末尾向左/右缩进|
|`n`|`==`|自动排版当前行|
|`n`|`=gg =G`|当前文档全文自动排版|
|`n`|`<N> ==`|对从当前行开始的 `N` 行进行自动排版|
|`n`|`=<N>j`|对从当前行开始的 `N` 行进行自动排版|
|`n`|`=<N>k`|对当前行以及向上 `N` 行进行自动排版|
|`n`|`=%`|光标在 `[`、`{` 上使用，代码块内的代码自动排版|


## **Search**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`*`|向后查找光标当前所在单词|
|`n`|`#`|向前查找光标当前所在单词|
|`n`|`/[search-item] + Enter + n / N`	|Find pattern and go to next match **forward**|
|`n`|`?[search-item] + Enter + n / N`	|Find pattern and go to next match **backward**|
|`n`|`n`|继续查找下一个|
|`n`|`N`|继续查找上一个|
|`n`|`f/F <X>`|当前行内向行尾方向查找并定位到字符 `X`|
|`n`|`t <X>`|当前行内向行尾方向查找并定位到第字符 `X` 之前一个光标|
|`n`|`T <X>`|当前行内向行尾方向查找并定位到字符 `X` 之后一个光标|
|`n`|`;`|继续向当前方向查找下一个字符|
|`n`|`,`|向当前相反方向查找下一个字符|
|`n`|`%`|`%` 对 `(` 和 `)`，`[` 和 `]`，`{` 和 `}` 进行匹配查找，当光标位于其中一个符号上时，按下 `%`，光标会跳到与之匹配的另外一个符号上|


## **Edit in `NORMAL` Mode** 
Operator `Delete` / `Undo` / `Paste` / `Replace`
| Mode | Shortcut             | Description                                                                        |
| ---- | -------------------- | ---------------------------------------------------------------------------------- |
|`n`|`dw`|delete **from the cursor up to the next word** type|
|`n`|`de`|delete **from the cursor up to the end of the word** type|
|`n`|`d$`|delete **from the cursor** to the **end of a line** type|
|`n`|`dd`|delete a **whole line** type|
|`n`|`u`| undo previous actions|
|`n`|`U`| undo all the changes on a line|
|`n`|`CTRL + r`| undo the undo's, Redo changes|
|`n`|`yw`|copy **from the cursor up to the next word** type|
|`n`|`ye`|copy **from the cursor up to the end of the word** type|
|`n`|`y$`|copy **from the cursor** to the **end of a line** type|
|`n`|`yy`|copy a **whole line** type|
|`n`|`p`| 在当前位置后粘贴|
|`n`|`P`| 在当前位置前粘贴|
|`n`|`gu <X>`| 将指定的文本转换为小写,这里的`<X>` 为一些移动命令|
|`n`|`gU <X>`| 将指定的文本转换为大写,这里的`<X>` 为一些移动命令|
|`n`|`g~ <X>`| 将指定的文本转换为相反大小写,这里的`<X>` 为一些移动命令|
|`n`|`r <X>`| 将当前字符替换为 `X`|
|`n`|`R`| Replace mode, to replace more than one character|
|`n`|`:s/[old]/[new] + <Enter>`| substitute new for the **first** old in a line type|
|`n`|`:s/[old]/[new]/g + <Enter>`| substitute new for all 'old's on a line type|
|`n`|`:#,#s/[old]/[new]/g + <Enter>`| substitute phrases between two line #'s type|
|`n`|`:%s/[old]/[new]/g + <Enter>`| substitute all occurrences in the file type|
|`n`|`:%s/[old]/[new]/gc + <Enter>`|  ask for confirmation each time add 'c'|
|`n`|`:! + <command> + <Enter>`| execute that command in external command-line|
|`n`|`J`| 将当前行与下一行合并|
|`n`|`.`| 重复执行上一个命令|

## **Enter `INSERT` Mode from `NORMAL` Mode**
| Mode | Shortcut             | Description                                                                        |
| ---- | -------------------- | ---------------------------------------------------------------------------------- |
| `n`  | `i`                  | Insert **before** *cursor*                                                               |
| `n`  | `a`                  | Append **after** *cursor* , using with `e`                                                               |
| `n`  | `I`                  | Insert at the **beginning** of the line                                                |
| `n`  | `A`                  | Append at the **end** of the line                                                      |
| `n`  | `o`                  | Insert to **next** line                                                                |
| `n`  | `O`                  | Insert to **previous** line                                                            |
| `n`  | `s`                  | 删除当前字符，并进入 `INSERT` 模式                                                        |
| `n`  | `S`                  | 删除当前行并保存到 Vim 剪贴板，同时进入 `INSERT` 模式（等同于 `cc`）     |
| `n`  | `c` + `[motion]` | Delete from before the cursor to `[motion]` and insert. Examples are as follow |
| `n`  | `c` + `w`            | 删除当前单词并保存到 Vim 剪贴板，同时进入 `INSERT` 模式，如果在词中，删除光标后的部分                   |
| `n`  | `c` + `e`            |  Change from the cursor to the end of the word and insert                   |
| `n`  | `c` + `i` + `w`      | 删除当前单词并保存到 Vim 剪贴板，同时进入 `INSERT` 模式，在词中也可删除整个单词  |
| `n`  | `c` + `$`            | Change from before the cursor to end of the line and insert                        |
| `n`  | `c` + `<N>` + `l`            | 删除 N 个字符并保存到 Vim 剪贴板，同时进入 `INSERT` 模式           |
| `n`  | `c` + `c`            | 删除当前行并保存到 Vim 剪贴板，同时进入 `INSERT` 模式           |
| `i`  | `<Esc>`              | Go back to Normal Mode, remap to `jk` recommended                                  |
> `c`/`y`/`d` 都可 写成 `c <X>` 形式 `<X>`部分是对操作内容的描述,如 `c3l` : 剪切后面 3 个字符并插入,`c$`剪切当前位置到行尾的内容并插入
## **`VISUAL` mode shortcuts**
| Mode     | Shortcut   | Description                                 |
| -------- | ---------- | ------------------------------------------- |
| `n`      | `v`        | Enter Visual Character Mode                 |
| `n`      | `V`        | Enter Visual Line Mode                      |
| `V-Line` | `>`; `<`   | Increase Indent; Decrease Indent            |
| `n`      | `Ctrl`+`v` | Enter `Visual Block` Mode     块操作                |
| `v`      | `<Esc>`    | Exit `Visual` Mode, remap to `jk` recommended |

在 `VISUAL` mode 移动光标文本会被选中，可以对这些被选中的文本执行一系列操作。  
例如: `d` 删除选中文本。  
输入 `:` 左下会出现 `:'<,'>`  
`:'<,'>w` + [filename] 会把选中的文字写入之前不存在的文件中。

## **Macro**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`q <N>` | 开启录制宏,`<N>`是给这次宏的录制过程一个存储位置可以是 0-9 或 a-z|
|`n`|`@ <N>` | 唤起宏,`<N>`是给宏的名字，可以在 `@` 前指定执行次数|
|`n`|`@@` |再次唤起最后一次执行的宏|

## **File**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`gf`|跳转到光标所在文件名，适用于`include`某文件时,go to file|
|`n`|`:e <filename> + <Enter>`|打开名为 filename 的文件，若文件不存在则创建之|
|`n`|`:Ex + <Enter>`|打开名为 filename 的文件，若文件不存在则创建之|
|`n`|`:buffer`|查看Vim buffer，即文件历史记录|
|`n`|`:ls`|查看Vim buffer，即文件历史记录|
|`n`|`:bn`|打开缓存中下一个文件|
|`n`|`:bp`|打开缓存中上一个文件|
|`n`|`:b <N>`|打开缓存中第 N 个文件|
|`n`|`:w  <filename> + <Enter>`|  saves the whole current Vim to the filename(filename  does not exist yet)|
|`n`|`:r + <filename> + <Enter>`| retrieves disk file FILENAME and puts it below the cursor position.|
|`n`|`:wa`| 保存全部文件|
|`n`|`:wq` or `ZZ`| 保存并退出|
|`n`|`:q!` or `ZQ`| 强制退出，不保存|
|`n`|`:saveas <new filename>`| 文件另存为|
|`n`|`:w <new filename>`| 文件另存一份名为 `<new filename>` 的副本并继续编辑原文件|



## **Split Screen and Tab**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`:split` or `:sp` | 上下分屏|
|`n`|`:vsplit` or `:vs` | 上下分屏|
|`n`|`:diffsplit` or `:diffs`| `diff` 模式打开一个分屏，后面可以加上 {filename}|
|`n`|`Ctrl-w w` | 激活下一个窗口|
|`n`|`Ctrl-w j` | 激活下方窗口|
|`n`|`Ctrl-w k` | 激活上方窗口|
|`n`|`Ctrl-w h` | 激活左侧窗口|
|`n`|`Ctrl-w l` | 激活右侧窗口|
|`n`|`Ctrl-w L` | 移动到最右侧|
|`n`|`Ctrl-w H` | 移动到最左侧|
|`n`|`Ctrl-w K` | 移动到顶部|
|`n`|`Ctrl-w J` | 移动到底部|
|`n`|`Ctrl-w =` | 平均窗口尺寸|
|`n`|`Ctrl-w +` | 增加高度|
|`n`|`Ctrl-w -` | 缩减高度|
|`n`|`Ctrl-w _` | 最大高度|
|`n`|`Ctrl-w >` | 增加宽度|
|`n`|`Ctrl-w <` | 缩减宽度|
|`n`|`Ctrl-w |` | 最大宽度|
|`n`|`:tabe` or `:tabnew` or `:tabedit` | 打开新标签页|
|`n`|`Ctrl-w gf`  | 在新标签页中打开当前光标所在位置的文件名|
|`n`|`gt` or `:tabnext` or `:tabn` | 下一个标签页|
|`n`|`gT` or `:tabprevious` or `:tabp` | 上一个标签页|
|`n`|`:tabrewind` or `:tabr` or  `:tabfirst` or `:tabfir` | 到第一个标签页|
|`n`|`:tablast` or `:tabl` |到最后一个标签页|
|`n`|`:tabclose` or `:tabc` |关闭当前标签页|
|`n`|`:-tabc` | 关闭上一个标签页|
|`n`|`:+tabc` | 关闭下一个标签页|
|`n`|`:tabonly` or `:tabo`| 关闭其他标签页|

**_注意**：`:tabnew` 和 `:tabedit` 后面都可以跟一个 <空格><文件名> 用以在新标签页中
54  打开指定文件，还可以在 `:` 后面加一个数字，指出新标签页在列表中的位置（从 0 开始）。
## **Vim Settings**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|`n`|`:set ic`  | ignore upper/lower case when searching  |
|`n`|`:set is` |  show partial matches for a search phrase  |
|`n`|`:set hlsearch/hls` | 搜索结果高亮   |
|`n`|`:set number/nu` | 显示行号|
|`n`|`:set relativenumber` | 显示相对行号（这个非常重要，慢慢体会）|
|`n`|`:set autoindent` | 自动缩进|
|`n`|`:set smartindent` | 智能缩进|
|`n`|`:set tabstop=4` | 设置 tab 制表符所占宽度为 4|
|`n`|`:set softtabstop=4` | 设置按 `tab` 时缩进的宽度为 4|
|`n`|`:set shiftwidth=4` | 设置自动缩进宽度为 4|
|`n`|`:set expandtab` | 缩进时将 tab 制表符转换为空格|
|`n`|`:filetype on` | 开启文件类型检测|
|`n`|`:syntax on` | 开启语法高亮|
Prepend "`no`" to switch an option off:   `:set noic`

若要永久生效，需要修改 Vim 的一个自动配置文件，一般文件路径是 `~/.vimrc`


## **常用技巧**
- `xp` / `ddp` 交换两个字符、交换两行。
- `:sh` 临时bash输入终端命令，可用来打开其他vim，`ctrl d` 或 `exit` 返回 vim，目前绑定到`F8`
- 还可以使用`:terminal` 打开一个内嵌终端。
- 在 `:` 后输入命令时， `%` 可以代替当前文件名。
- vim内置的 make 可以调错 `cw` 打开错误窗口 ，如果跳转到其他文件可以直接使用`ctrl o`或者`ctrl i `调回来。
- `:ls` 显示的缓冲区，文件名前 `#` 代表 `ctrl o `能跳转过去的上一个文件，`%a` 代表当前正在编辑的文件，`b + n` 文件名前的序号可以跳转到那个文件 。
- `bp` 绑定到 `F2`,`bn` 绑定到 `F3`,`NERDtree`插件打开目录
