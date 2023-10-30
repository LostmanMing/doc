- 通过事件可以实现cuda流之间的同步，事件测量等，cudaEventCreate创建事件，cudaEventRecord将事件在流的某个位置标记下来，cudaEventSynchronize事件同步，保证某个事件已经完成。cudaSreamWaitEvent,流之间通过事件同步
- cuda异步拷贝  cudaMemcpyAsync 
    1. 异步数据拷贝在显示创建的CUDA stream中创建
    2. 异步数据拷贝与主机线程异步执行
    3. 用异步数据拷贝的主机内存必须是页锁定内存
    与主机同步：cudaStreamSynchronize
- shuffle用来对同一线程束的线程进行数据交换
- 共享内存屏障，线程块珊栏和线程网格珊栏进行共享内存的同步
- cuda每个流处理器都有64k的片上内存，被L1缓存和共享内存共有
- cuda动态共享内存格式：`extern __shared__ int dynamic_arr[];`，调用核函数时动态制定内存大小 `dynamic_mem<<<grids,block,block.x>>>();`,第三个参数
- cuda页锁定内存，cudaMallocHost/cudaFreeHost,页锁定机制机内存中的页面交换出去（即写入磁盘），页锁定内存是不能被操作系统交换出去的内存，这意味着这些内存页面始终驻留在物理内存中，避免了可能的页面交换开销。
- cuda零拷贝内存cudaHostAlloc/cudaFreeHost，零拷贝内存允许GPU直接从主机内存中读写，而无需显式地将数据从主机拷贝到设备或反之，零拷贝内存最适合于GPU和CPU需要共享大量数据但只进行少量计算的场景，可以直接申请完内存后，在主机直接赋值，而不需要使用cudaMemcpy进行主机和设备内存之间的拷贝。
- cuda中的symbol可以理解为被 __device__ 修饰的全局变量
- cuda共享内存和L1缓存共享一块64k的硬件存储区域
- cuda每个SM中寄存器在线程间分配，寄存器数量有限，大概在32k/64k左右，每个线程占用的寄存器数量越多，表示在这个流处理器上所能运行的线程越少，整体效率就越低。每个SM中的共享内存大小也是固定的，48k左右，共享内存在线程块间分配，每个线程块占用的共享内存越多，流处理器上所能运行的线程块越少。
- cuda线程束（warp）是cuda架构的基本执行单位，，一般warpSize为32，即32个线程同时执行相同的指令，在gpu硬件层面真正的并行执行，如果核函数中有分支，可能会出现线程束分支，导致性能下降，可以通过nvprof检测线程束分支，虽然编译器会进行优化，但程序还是要尽可能避免。

- std::vector指向数组的指针在栈上存储，而实际的数组在堆上存放，因为有动态扩容，vector 的 data() 可以获取数组原始指针,如果发生 扩容或者vector析构了，会发生指针失效，resize和reverse分别针对size和capacity,有初始化和未初始化的区别。

- mysql四种隔离级别以及存在和解决的问题
    1. **读未提交**（read uncommitted）最低的隔离级别 没提交就读到了

        事务A能够读到事务B未提交的数据。  
        存在的问题：**脏读**  
        这种隔离级别是理论上的，一般数据库都是二档起步
    2. **读已提交**（read committed）  提交了才能读到

        事务A只能读到事务B已经提交的数据
        这种隔离级别解决了 **脏读** 问题   
        存在的问题：**不可重复读**   
        这种隔离级别是比较真实的数据，每次读到的数据是绝对的真实。
    3. **可重复读**（repeatable read） 提交之后也都不到

        事务A开启后不管多久，每一次在事务A中读到的数据都是一致的。即使事务B已经提交了修改的数据，事务A读到的数据还是不变，即可重复读。  
        这种隔离级别解决了 **不可重复读** 问题
        存在的问题：**幻读** ，每次读到的数据都是幻影，不真实。

    4. **串行化/序列化** (serializable) 最高的隔离级别

        不能并发事务，排队执行。  
        线程同步（事务同步），每次读取到的数据都是最真实的，效率最低。


- 由于最开始的一字节ascll码只能支持英文数字，为了能更好支持其他语言，提出了wchar等，但UTF-8可变长编码出现后，解决了这个问题，wchar失业了。但本来UTF-16中文只需要2字节，UTF-8需要3字节。Unicode 是一个字符编码方案，它的目标是为世界上所有的文本字符提供一个唯一的编号。GBK是由中国提出的，UTF是Unicode为了表示所有字符提出的。

- string_view 是 string 的弱引用，是一种浅拷贝，效率比较高，不可以写入，因为 string 本体被修改时，有可能会扩容导致ptr和len改变，此时 string_view 会失效，所以使用了 string_view 后尽量不要改写原 string。string_view 由于只是 原 string 的视图，只保存原指针的地址和长度，所以进行切片(substr())等操作会很快，不需要创建新的字符串，只需要移动指针和长度即可。
    ```bash
    # 常见容器及其弱引用
    string          string_view
    wstring         wstring_view
    vector<T>       span<T>
    unique_ptr<T>   T*
    shared_ptr<T>   weak_ptr<T>
    ```

- string 的 find 函数在找不到指定字符串时会返回 size_t 类型的 -1，由于 size_t 是无符号类型，所以 -1 会补码为类型的最大值，这样看起来很不专业，所以可以用std::string::npos 来代替 size_t(-1),即 `s.find(c) != string::npos`
    ```cpp
    s.find(c) != string::npos
    s.find(c) != s.npos
    s.find(c) != size_t(-1)
    s.find(c) != -1
    ```

- c++14 引入自定义字面量后缀，缓解例如字符串字面量需要加 `string()` 才能进行相加等的繁琐操作，写 `"hello"s` 相当于写 `operator""s("hello",5)` ,即相当于 `string("hello",5)`,后缀可以自己重载。
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

- C语言中 `strlen` 求的是从传入的指针开始，知道遇到第一个 `'\0'` 为止的字符串长度，与 `sizeof` 相比，C类型字符数组作为参数传递会退化成普通指针，导致 `sizeof` 后得到一个指针的大小，而 `strlen` 可以正常使用。

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
