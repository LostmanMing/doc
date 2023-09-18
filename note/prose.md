- c++14 引入自定义字面量后缀，缓解例如字符串字面量需要加 `string()` 才能进行相加等操作的繁琐，写 `"hello"s` 相当于写 `operator""s("hello",5)` ,即相当于 `string("hello",5)`,后缀可以自己重载。
    ```cpp
    string operator""s(const char *s, size_t len){
        return string(s,len);
    }
    ```

- `STL` 比较方便的将多个数字拼接成字符串并且有格式化方法,不使用 `fmt` 库
    ```cpp
    #include <string>
    #include <iostream>
    #include "sstream"
    #include "iomanip"
    int main() {
        std::stringstream ss;
        std::string s = (dynamic_cast<std::ostringstream&>(std::stringstream() << std::setprecision(100) << 3.14f)).str();
        std::cout << s << std::endl;
        return 0;
    }
    ```

- C语言中 `strlen` 求的是从传入的指针开始，知道遇到第一个 `'\0'` 为止的字符串长度，与 `sizeof` 相比，C类型字符数组作为参数传递会退化成普通指针，导致 `sizeof` 后得到一个指针的大小，而 `sizeof` 可以正常使用。

- 为什么c++中.h和.cpp可以分离,而模板不可以
    ```
    普通函数可以分离原因：
    封装与接口分离：你可以在头文件中定义接口，而在源文件中实现接口。这使得其他人只需要知道接口即可使用你的代码，而不必了解其具体实现。

    编译优化：只有当源文件（.cpp）发生变化时，它才会被重新编译。这意味着如果你更改了一个类的实现但没有更改其接口，那么只需要重新编译那个类，而使用该类的其他代码则不需要重新编译。

    避免重复包含：使用预处理器指令（例如#ifndef，#define和#endif）可以防止头文件被重复包含，这是头文件/源文件分离策略的一个重要组成部分。

    模板，不可分离原因：
    编译时实例化：模板不是普通的函数或类，它们是编译器用来生成特定类型的函数或类的“模板”。编译器必须在编译时看到模板的完整定义（即模板的所有代码），才能为特定的类型生成相应的实例。

    独特的链接模型：与普通函数或类不同，模板的实例化会在每个使用它的.cpp文件中进行。这意味着，如果模板的定义不可见，编译器就无法创建其实例。
    ```
