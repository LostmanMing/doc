<div align='center'>
  <h1>Vim 笔记</h1>
</div>

# Contents
- [Contents](#contents)
- [**资源**](#资源)
- [**Vim 快捷键**](#vim-快捷键)
  - [**Navigation**](#navigation)
  - [Edit in `NORMAL` Mode](#edit-in-normal-mode)
  - [**Enter `INSERT` Mode from `NORMAL` Mode**](#enter-insert-mode-from-normal-mode)
  - [`VISUAL` mode shortcuts](#visual-mode-shortcuts)


# **资源**
Vim 自带一个交互式的教程，内含你需要了解的最基础的信息，你可以通过终端运行以下命令打开教程：

    $ vimtutor


# **Vim 快捷键**
## **Navigation**
|Mode|ShortCut|Description|
|:---:|:---:|:---:|
|n|`h`|left|
|n|`j`|down|
|n|`k`|up|
|n|`l`|right|
|n|`w`|One word **forward**, *beginning of word*|
|n|`e`|One word **forward**, *end of word*|
|n|`b`|One word **backward**|
|n|`0`|Beginning of line|
|n|`^`|Beginning of line|
|n|`$`|End of line|
|n|`gg`|Beginning of file|
v|n|`:[num-of-line] + Enter`|Go to a **specific** line|
|n|`/[search-item] + Enter + n / N`	|Find pattern and go to next match **forward**|
|n|`?[search-item] + Enter + n / N`	|Find pattern and go to next match **backward**|
|n|`ctrl + o`|takes cursor back to older positions|
|n|`ctrl + i`|takes cursor back to newer positions|


## Edit in `NORMAL` Mode  
Operator `Delete` / `Undo` / `Paste` / `Replace`
| Mode | Shortcut             | Description                                                                        |
| ---- | -------------------- | ---------------------------------------------------------------------------------- |
|n|`dw`|delete **from the cursor up to the next word** type|
|n|`de`|delete **from the cursor up to the end of the word** type|
|n|`d$`|delete **from the cursor** to the **end of a line** type|
|n|`dd`|delete a **whole line** type|
|n|`u`| undo previous actions|
|n|`U`| undo all the changes on a line|
|n|`CTRL + r`| undo the undo's, Redo changes|
|n|`yw`|copy **from the cursor up to the next word** type|
|n|`ye`|copy **from the cursor up to the end of the word** type|
|n|`y$`|copy **from the cursor** to the **end of a line** type|
|n|`yy`|copy a **whole line** type|
|n|`p`| paste the text that has **just been deleted or copied**|
|n|`r + char`| replace the character **under the cursor**|
|n|`R`| Replace mode, to replace more than one character|
|n|`:s/[old]/[new] + Enter`| substitute new for the **first** old in a line type|
|n|`:s/[old]/[new]/g + Enter`| substitute new for all 'old's on a line type|
|n|`:#,#s/[old]/[new]/g + Enter`| substitute phrases between two line #'s type|
|n|`:%s/[old]/[new]/g + Enter`| substitute all occurrences in the file type|
|n|`:%s/[old]/[new]/gc + Enter`|  ask for confirmation each time add 'c'|
|n|`:! + [command] + Enter`| execute that command in external command-line|
|n|`:w + [filename] + Enter`|  saves the whole current Vim to the filename(filename  does not exist yet)|
|n|`:r + [filename] + Enter`| retrieves disk file FILENAME and puts it below the cursor position.|


## **Enter `INSERT` Mode from `NORMAL` Mode**
| Mode | Shortcut             | Description                                                                        |
| ---- | -------------------- | ---------------------------------------------------------------------------------- |
| `n`  | `i`                  | Insert **before** *cursor*                                                               |
| `n`  | `a`                  | Append **after** *cursor* , using with `e`                                                               |
| `n`  | `I`                  | Insert at the **beginning** of the line                                                |
| `n`  | `A`                  | Append at the **end** of the line                                                      |
| `n`  | `o`                  | Insert to **next** line                                                                |
| `n`  | `O`                  | Insert to **previous** line                                                            |
| `n`  | `c` + `[motion]` | Delete from before the cursor to `[motion]` and insert. Examples are as follow |
| `n`  | `c` + `w`            | Change from the cursor to end of current word and insert                    |
| `n`  | `c` + `e`            |  Change from the cursor to the end of the word and insert                   |
| `n`  | `c` + `i` + `w`      | Change current word and insert                                                     |
| `n`  | `c` + `$`            | Change from before the cursor to end of the line and insert                        |
| `i`  | `<Esc>`              | Go back to Normal Mode, remap to `jk` recommended                                  |

## `VISUAL` mode shortcuts
| Mode     | Shortcut   | Description                                 |
| -------- | ---------- | ------------------------------------------- |
| `n`      | `v`        | Enter Visual Character Mode                 |
| `n`      | `V`        | Enter Visual Line Mode                      |
| `V-Line` | `>`; `<`   | Increase Indent; Decrease Indent            |
| `n`      | `Ctrl`+`v` | Enter Visual Block Mode                     |
| `v`      | `<Esc>`    | Exit Visual Mode, remap to `jk` recommended |

在 `VISUAL` mode 移动光标文本会被选中，可以对这些被选中的文本执行一系列操作。  
例如: `d` 删除选中文本。  
输入 `:` 左下会出现 `:'<,'>`  
`:'<,'>w` + [filename] 会把选中的文字写入之前不存在的文件中。


`:set ic`   ignore upper/lower case when searching  
`:set is`   show partial matches for a search phrase  
`:set hls`  highlight all matching phrases   
Prepend "no" to switch an option off:   `:set noic`