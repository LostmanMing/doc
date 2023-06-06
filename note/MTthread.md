<div align='center'>
  <h1>多线程编程</h1>
</div>

---------------------------------------------------------------------------

- [线程](#线程)
  - [*std::thread*](#stdthread)
    - [1. ***join()***](#1-join)
    - [2. ***detach()***](#2-detach)
- [异步](#异步)
  - [1. ***std::future \& std::async***](#1-stdfuture--stdasync)
    - [`get()`](#get)
    - [`wait()`](#wait)
    - [`wait_for`](#wait_for)
    - [`wait_until()`](#wait_until)
    - [`std::launch::deffer`](#stdlaunchdeffer)
  - [2. ***std::promise***](#2-stdpromise)
- [互斥量](#互斥量)
  - [1. ***std::mutex***](#1-stdmutex)
    - [`lock()/unlock()`](#lockunlock)
    - [`try_lock()`](#try_lock)
  - [2. ***std::timed\_mutex***](#2-stdtimed_mutex)
    - [`try_lock_for()`](#try_lock_for)
    - [`try_lock_until()`](#try_lock_until)
  - [3. ***std::lock\_guard***](#3-stdlock_guard)
  - [4. ***std::unique\_lock***](#4-stdunique_lock)
    - [`std::defer_lock`](#stddefer_lock)
    - [`std::try_to_lock`](#stdtry_to_lock)
    - [`std::adopt_lock`](#stdadopt_lock)
- [死锁](#死锁)
  - [不同线程的死锁问题](#不同线程的死锁问题)
    - [`std::lock`](#stdlock)
    - [`std::scoped_lock`](#stdscoped_lock)
  - [同一个线程的死锁问题](#同一个线程的死锁问题)
    - [***std::recursive\_mutex***](#stdrecursive_mutex)
- [读写锁](#读写锁)
  - [***std::shared\_mutex***](#stdshared_mutex)
    - [`lock_shared()` / `unlock_shared()`](#lock_shared--unlock_shared)
    - [`std::shared_lock`](#stdshared_lock)
- [条件变量](#条件变量)
  - [***condition\_variable***](#condition_variable)
    - [`wait()` / `cv.notify_one()`](#wait--cvnotify_one)
    - [`notify_all() `](#notify_all-)
    - [`wait_for()` / `wait_until()`](#wait_for--wait_until)
- [原子](#原子)
  - [***std::atomic***](#stdatomic)
    - [`fetch_add` / `store` / `load`](#fetch_add--store--load)
    - [`exchange`](#exchange)
    - [`compare_exchange_strong`](#compare_exchange_strong)
- [自旋锁](#自旋锁)


# 线程 
## *std::thread*
### 1. ***join()***
- 有了多线程的话，文件下载和用户交互分别在两个线程，同时独立运行.但是用户交互所在的主线程退出后，文件下载所在的子线程，因为从属于这个主线程，也被迫退出了。
- 可以用 `std::thread` 类的成员函数 `join()` 来等待该进程结束。
    ```cpp
    #include <iostream>
    #include <thread>
    #include <string>

    void download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    int main() {
        std::thread t1([&] {
            download("hello.zip");
        });
        interact();
        std::cout << "Waiting for child thread..." << std::endl;
        t1.join();
        std::cout << "Child thread exited!" << std::endl;
        return 0;
    }
    ```
### 2. ***detach()***
- 当线程所在的函数退出时，就会调用 `std::thread` 的解构函数，这会销毁 t1 线程。
- **解决方案**：调用成员函数 `detach()` 分离该线程——意味着线程的生命周期不再由当前 `std::thread` 对象管理，而是在线程退出以后自动销毁自己。

    ```cpp
    #include <iostream>
    #include <thread>
    #include <string>

    void download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    void myfunc() {
        std::thread t1([&] {
            download("hello.zip");
        });
        t1.detach();
        // t1 所代表的线程被分离了，不再随 t1 对象销毁
    }

    int main() {
        myfunc();
        interact();
        return 0;
    }
    ```
- 但是 `detach` 的问题是**进程**退出时候不会等待所有子线程执行完毕。所以另一种解法是把 `t1` 对象移动到一个全局变量去，从而延长其生命周期到 `myfunc` 函数体外。
    ```cpp
    #include <iostream>
    #include <thread>
    #include <string>
    #include <vector>

    void download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    std::vector<std::thread> pool;

    void myfunc() {
        std::thread t1([&] {
            download("hello.zip");
        });
        // 移交控制权到全局的 pool 列表，以延长 t1 的生命周期
        pool.push_back(std::move(t1));
    }

    int main() {
        myfunc();
        interact();
        for (auto &t: pool) t.join();  // 等待池里的线程全部执行完毕
        return 0;
    }
    ```

- 或者定义一个线程池类，解构函数调用 `join()`
- C++20 引入了 `std::jthread `类，和 `std::thread` 不同在于：他的**解构函数**里会**自动**调用 `join()` 函数，从而保证 `pool `解构时会自动等待全部线程执行完毕。

    ```cpp
    std::vector<std::jthread> pool
    void myfunc() {
        std::thread t1([&] {
            download("hello.zip");
        });
        // 移交控制权到全局的 pool 列表，以延长 t1 的生命周期
        pool.push_back(std::move(t1));
    }

    int main() {
        myfunc();
        interact();
        for (auto &t: pool) t.join();  // 等待池里的线程全部执行完毕
        return 0;
    }
    ```
# 异步
## 1. ***std::future & std::async***
###  `get()`
 - `std::async` 接受一个**带返回值**的 `lambda`，自身返回一个 `std::future` 对象。
 - `lambda` 的函数体将在另一个线程里执行。
 - 接下来你可以在 `main `里面做一些别的事情，`download` 会持续在后台悄悄运行。
- 最后调用 `future` 的 `get()`方法，如果此时 `download` 还没完成，会等待 `download` 完成，并获取 `download` 的返回值。
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <future>

    int download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
        return 404;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    int main() {
        std::future<int> fret = std::async([&] {      //调用一个线程执行lambda
            return download("hello.zip"); 
        });
        interact();
        int ret = fret.get();                   //相当于有返回值的 join()
        std::cout << "Download result: " << ret << std::endl;
        return 0;
    }
    ```
### `wait()`
- 区别在于 `wait()` 没有返回值，只是线程结束，**相当于将一个 `get()` 拆成 `wait() + get()`，中间可以进行一些操作**。

### `wait_for`
- 只要线程没有执行完，`wait()` 会无限等下去。
- 而 `wait_for()` 则可以指定一个最长等待时间，用 `chrono` 里的类表示单位。他会返回一个 `std::future_status` 表示等待是否成功。
- 如果超过这个时间线程还没有执行完毕，则放弃等待，返回 `future_status::timeout`。
- 如果线程在指定的时间内执行完毕，则认为等待成功，返回 `future_status::ready`。
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <future>

    int download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
        return 404;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    int main() {
        std::future<int> fret = std::async([&] {
            return download("hello.zip"); 
        });
        interact();
        while (true) {
            std::cout << "Waiting for download complete..." << std::endl;
            auto stat = fret.wait_for(std::chrono::milliseconds(1000));   //在规定时间内线程执行完毕则为ready，否则为timeout
            if (stat == std::future_status::ready) {
                std::cout << "Future is ready!!" << std::endl;
                break;
            } else {
                std::cout << "Future not ready!!" << std::endl;
            }
        }
        int ret = fret.get();
        std::cout << "Download result: " << ret << std::endl;
        return 0;
    }
    ```
### `wait_until()`
> 同理还有 `wait_until()` 其参数是一个时间点。

### `std::launch::deffer`
- `std::async` 的第一个参数可以设为 `std::launch::deferred`，这时不会创建一个线程来执行，他只会把 `lambda` 函数体内的运算推迟到 `future` 的 `get()` 被调用时。也就是 `main` 中的 `interact` 计算完毕后。
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <future>

    int download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
        return 404;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    int main() {
        std::future<int> fret = std::async(std::launch::deferred, [&] {    //纯粹的调用，并没有长创建新的线程，只是函数上的异步，并非线程上的异步
            return download("hello.zip"); 
        });
        interact();
        int ret = fret.get();
        std::cout << "Download result: " << ret << std::endl;
        return 0;
    }
    ```
## 2. ***std::promise***
- 如果不想让 `std::async` 帮你自动创建线程，想要手动创建线程，可以直接用 `std::promise`。
- 然后在线程返回的时候，用 `set_value()` 设置返回值。在主线程里，用 `get_future()` 获取其 `std::future` 对象，进一步 `get()` 可以等待并获取线程返回值.
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <future>

    int download(std::string file) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Downloading " << file
                    << " (" << i * 10 << "%)..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
        std::cout << "Download complete: " << file << std::endl;
        return 404;
    }

    void interact() {
        std::string name;
        std::cin >> name;
        std::cout << "Hi, " << name << std::endl;
    }

    int main() {
        std::promise<int> pret;     
        std::thread t1([&] {                  //不用future + async自动创建线程，手动创建线程
            auto ret = download("hello.zip");
            pret.set_value(ret);                //设置值
        });
        std::future<int> fret = pret.get_future();    //获得未来对象

        interact();
        int ret = fret.get();     // get()等待线程结束，获得返回值
        std::cout << "Download result: " << ret << std::endl;

        t1.join();
        return 0;
    }
    ``` 
- `future` 为了三五法则，删除了拷贝构造/赋值函数。如果需要浅拷贝，实现共享同一个 `future` 对象，可以用 `std::shared_future`.
- 如果不需要返回值，`std::async` 里 `lambda` 的返回类型可以为 `void`， 这时 `future` 对象的类型为 `std::future<void>`。
- 同理有 `std::promise<void>`，他的 `set_value()` 不接受参数，仅仅作为**同步**用，不传递任何实际的值。

#  互斥量
## 1. ***std::mutex***
- 调用 `std::mutex` 的 `lock()` 时，会检测 `mutex` 是否已经上锁。
- 如果没有锁定，则对 mutex 进行上锁。
- 如果已经锁定，则陷入等待，直到 mutex 被另一个线程解锁后，才再次上锁。
- 而调用 unlock() 则会进行解锁操作。
- 这样，就可以保证 mtx.lock() 和 mtx.unlock() 之间的代码段，同一时间只有一个线程在执行，从而避免数据竞争。

### `lock()/unlock()`

    ```cpp 
    #include <iostream>
    #include <string>
    #include <thread>
    #include <vector>
    #include <mutex>

    int main() {
        std::vector<int> arr;
        std::mutex mtx;
        std::thread t1([&] {
            for (int i = 0; i < 1000; i++) {
                mtx.lock();
                arr.push_back(1);
                mtx.unlock();
            }
        });
        std::thread t2([&] {
            for (int i = 0; i < 1000; i++) {
                mtx.lock();
                arr.push_back(2);
                mtx.unlock();
            }
        });
        t1.join();
        t2.join();
        return 0;
    }
    ```
### `try_lock()`
-  `lock()` 如果发现 mutex 已经上锁的话，会等待他直到他解锁。
-  也可以用**无阻塞**的 `try_lock()`，他在上锁失败时不会陷入等待，而是直接返回 `false`；如果上锁成功，则会返回 `true`。
    ```cpp
    #include <cstdio>
    #include <mutex>

    std::mutex mtx1;

    int main() {
        if (mtx1.try_lock())   //上锁成功，返回true
            printf("succeed\n");
        else
            printf("failed\n");

        if (mtx1.try_lock())   //上锁失败，返回false并继续执行
            printf("succeed\n");
        else
            printf("failed\n");

        mtx1.unlock();
        return 0;
    }
    ```
## 2. ***std::timed_mutex***
### `try_lock_for()`
- `try_lock()` 碰到已经上锁的情况，会立即返回 `false`。
- 如果需要等待，但仅限一段时间，可以用 `std::timed_mutex` 的 `try_lock_for()` 函数，他的参数是**最长等待时间**，同样是由 `chrono` 指定时间单位。超过这个时间还没成功就会“不耐烦地”失败并返回 `false`；如果这个时间内上锁成功则返回 `true`。
 ### `try_lock_until()`
 - 接收时间点，时间点前成功上锁返回 `true`，到了时间点还没有成功上锁返回 `false`。
## 3. ***std::lock_guard***
- 根据 RAII 思想，可将锁的持有视为资源，上锁视为锁的获取，解锁视为锁的释放.
- `std::lock_guard` 就是这样一个工具类，他的构造函数里会调用 `mtx.lock()`，解构函数会调用 `mtx.unlock()`。从而退出函数作用域时能够自动解锁，避免程序员粗心不小心忘记解锁。


    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <vector>
    #include <mutex>

    int main() {
        std::vector<int> arr;
        std::mutex mtx;
        std::thread t1([&] {
            for (int i = 0; i < 1000; i++) {
                std::lock_guard<std::mutex> grd(mtx);
                arr.push_back(1);
            }
        });
        std::thread t2([&] {
            for (int i = 0; i < 1000; i++) {
                std::lock_guard<std::mutex> grd(mtx);
                arr.push_back(2);
            }
        });
        t1.join();
        t2.join();
        return 0;
    }
    ```
## 4. ***std::unique_lock***
- `std::lock_guard` 严格在解构时 `unlock()`，但是有时候我们会希望提前 `unlock()`。这时可以用 `std::unique_lock`，他额外存储了一个 flag 表示是否已经被释放。他会在解构检测这个 flag，如果没有释放，则调用 `unlock()`，否则不调用。
- 可以直接调用 `unique_lock` 的 `unlock()` 函数来提前解锁，但是即使忘记解锁也没关系，退出作用域时候他还会自动检查一遍要不要解锁。
- 其实 `std::unique_lock` 具有 `mutex` 的所有成员函数：`lock()`, `unlock()`, `try_lock()`, `try_lock_for()` 等。除了他会在解构时按需自动调用 `unlock()`。
- 因为 `std::lock_guard` 无非是调用其构造参数名为 `lock()` 的成员函数，所以 `std::unique_lock` 也可以作为 `std::lock_guard` 的构造参数！
- 这种只要具有某些指定名字的成员函数，就判断一个类是否满足某些功能的思想，在 `Python` 称为鸭子类型，而 `C++` 称为 `concept`（概念）。比起虚函数和动态多态的接口抽象，`concept` 使实现和接口更加解耦合且没有性能损失。

### `std::defer_lock`
- 构造函数没有名字，但可以在后面加上一个tag类型，实现构造函数重载
- `std::unique_lock` 的构造函数还可以有一个额外参数，那就是 `std::defer_lock`。
- 指定了这个参数的话，`std::unique_lock` 不会在构造函数中调用 `mtx.lock()`，需要之后再手动调用 `grd.lock()` 才能上锁
- 好处依然是即使忘记 `grd.unlock()` 也能够自动调用 `mtx.unlock()`

    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <vector>
    #include <mutex>

    int main() {
        std::vector<int> arr;
        std::mutex mtx;
        std::thread t1([&] {
            for (int i = 0; i < 1000; i++) {
                std::unique_lock grd(mtx);
                arr.push_back(1);//可以手动unlock()，也可以让unique_lock自动解锁
            }
        });
        std::thread t2([&] {
            for (int i = 0; i < 1000; i++) {
                std::unique_lock grd(mtx, std::defer_lock);
                printf("before the lock\n");
                grd.lock();   //手动选择上锁时机
                arr.push_back(2);
                grd.unlock();
                printf("outside of lock\n");
            }
        });
        t1.join();
        t2.join();
        return 0;
    }
    ```
### `std::try_to_lock`
- 和无参数相比，他会调用 `mtx.try_lock()` 而不是 `mtx.lock()`。之后，可以用 `grd.owns_lock()` 判断是否上锁成功。
    ```cpp
    #include <cstdio>
    #include <thread>
    #include <mutex>

    int main() {
        std::mutex mtx;
        std::thread t1([&] {
            std::unique_lock grd(mtx, std::try_to_lock);
            if (grd.owns_lock())
                printf("t1 success\n");
            else
                printf("t1 failed\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });

        std::thread t2([&] {
            std::unique_lock grd(mtx, std::try_to_lock);  //构造时不调用lock(),而是try_lock()
            if (grd.owns_lock()) //拿到try_lock()的返回值
                printf("t2 success\n");
            else
                printf("t2 failed\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });

        t1.join();
        t2.join();
        return 0;
    }
    ```
### `std::adopt_lock`
- 如果当前 `mutex` **已经上锁了**，但是之后仍然希望用 `RAII` 思想在解构时候自动调用 `unlock()`，可以用 `std::adopt_lock` 作为 `std::unique_lock` 或 `std::lock_guard` 的第二个参数，这时他们会默认 `mtx` 已经上锁
    ```cpp
    #include <cstdio>
    #include <thread>
    #include <mutex>

    int main() {
        std::mutex mtx;
        std::thread t1([&] {
            std::unique_lock grd(mtx);
            printf("t1 owns the lock\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });

        std::thread t2([&] {
            mtx.lock();     //已经上锁
            std::unique_lock grd(mtx, std::adopt_lock);  //之后无需解锁
            printf("t2 owns the lock\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });

        t1.join();
        t2.join();
        return 0;
    }
    ```
#  死锁
## 不同线程的死锁问题
- 由于同时执行的两个线程，他们中发生的指令不一定是同步的，因此有可能出现这种情况：
- t1 执行 mtx1.lock()。
- t2 执行 mtx2.lock()。
- t1 执行 mtx2.lock()：失败，陷入等待
- t2 执行 mtx1.lock()：失败，陷入等待
- 双方都在等着对方释放锁，但是因为等待而无法释放锁，从而要无限制等下去。
- 这种现象称为**死锁**（dead-lock）。
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <mutex>

    int main() {
        std::mutex mtx1;
        std::mutex mtx2;

        std::thread t1([&] {
            for (int i = 0; i < 1000; i++) {
                mtx1.lock();     //1
                mtx2.lock();     //3  永远等待
                mtx2.unlock();
                mtx1.unlock();
            }
        });
        std::thread t2([&] {
            for (int i = 0; i < 1000; i++) {
                mtx2.lock();    //2
                mtx1.lock();    //3   永远等待
                mtx1.unlock();
                mtx2.unlock();
            }
        });
        t1.join();
        t2.join();
        return 0;
    }
    ```
**解决办法**
- 最为简单的方法，就是==一个线程永远不要同时持有两个锁，分别上锁，这样也可以避免死锁==。
- 其实，只需==保证双方上锁的顺序一致，即可避免死锁==。因此这里调整 t2 也变为先锁 mtx1，再锁 mtx2。
- 用 `std::lock` 同时对多个上锁
### `std::lock`
- 如果没办法保证上锁顺序一致，可以用标准库的 `std::lock(mtx1, mtx2, ...)` 函数，一次性对多个` mutex` 上锁。
- 他接受任意多个 `mutex` 作为参数，并且他保证在==无论任意线程中调用的顺序是否相同，都不会产生死锁问题==。
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <mutex>

    int main() {
        std::mutex mtx1;
        std::mutex mtx2;

        std::thread t1([&] {
            for (int i = 0; i < 1000; i++) {
                std::lock(mtx1, mtx2);       //同时对mtx1, mtx2上锁并保证mtx1, mtx2不会出现死锁问题
                mtx1.unlock();
                mtx2.unlock();
            }
        });
        std::thread t2([&] {
            for (int i = 0; i < 1000; i++) {
                std::lock(mtx2, mtx1);
                mtx2.unlock();
                mtx1.unlock();
            }
        });
        t1.join();
        t2.join();
        return 0;
    }
    ```
### `std::scoped_lock`
- 和 `std::lock_guard` 相对应，`std::lock` 也有 `RAII` 的版本 `std::scoped_lock`。只不过他可以同时对多个 `mutex` 上锁。
    ```cpp
    #include <iostream>
    #include <string>
    #include <thread>
    #include <mutex>

    int main() {
        std::mutex mtx1;
        std::mutex mtx2;

        std::thread t1([&] {
            for (int i = 0; i < 1000; i++) {
                std::scoped_lock grd(mtx1, mtx2); //无需手动unlock(),std::lock的 RAII 版本
                // do something
            }
        });
        std::thread t2([&] {
            for (int i = 0; i < 1000; i++) {
                std::scoped_lock grd(mtx2, mtx1);
                // do something
            }
        });
        t1.join();
        t2.join();
        return 0;
    }
    ```
## 同一个线程的死锁问题
- 同一个线程重复调用 lock() 也会造成死锁
- 除了两个线程同时持有两个锁会造成死锁外，即使只有一个线程一个锁，如果 `lock()` 以后又调用 `lock()`，也会造成死锁。

**解决办法**
- 其中一个位置不要再上锁，并写文档注释调用它前一定保证`mutex`已上锁。
- 如果实在不能改的话，可以用 `std::recursive_mutex`。他会自动判断是不是同一个线程 `lock()` 了多次同一个锁，如果是则让计数器加1，之后 `unlock()` 会让计数器减1，减到0时才真正解锁。但是相比普通的 std::mutex 有一定性能损失。
- 同理还有 `std::recursive_timed_mutex`，如果你同时需要 `try_lock_for()` / `try_lock_until()`的话。
### ***std::recursive_mutex***
- `std::recursive_mutex`。他会自动判断是不是同一个线程 `lock()` 了多次同一个锁，如果是则让计数器加1，之后 `unlock()` 会让计数器减1，减到0时才真正解锁。但是相比普通的 std::mutex 有一定性能损失
    ```cpp

    #include <iostream>
    #include <mutex>

    std::recursive_mutex mtx1;

    void other() {
        mtx1.lock();
        // do something
        mtx1.unlock();
    }

    void func() {
        mtx1.lock();
        other();
        mtx1.unlock();
    }

    int main() {
        func();
        return 0;
    }
    ```
#  读写锁
- 除了避免多个线程对一个对象做修改的问题，还有一个问题就是多线程同时读取一个对象。
- 多个线程不允许同时修改一个对象，但多个线程可以同时读取一个对象。
  
- **普通实现一个线程安全的 vector**

    ```cpp
    #include <iostream>
    #include <thread>
    #include <vector>
    #include <mutex>
    class MTVector {
        std::vector<int> m_arr;
        mutable std::mutex m_mtx;     //mutable 可以让 m_mtx 在 const 成员函数中改变，size() 在逻辑上仍是 const 的。

    public:
        void push_back(int val) {
            m_mtx.lock();
            m_arr.push_back(val);
            m_mtx.unlock();
        }

        size_t size() const {             //尽管保证了线程安全，但效率太低，读取时也要等待解锁，其实只要保证读取时不会修改数据即可，对多个读取线程无需上锁。
            m_mtx.lock();
            size_t ret = m_arr.size();
            m_mtx.unlock();
            return ret;
        }
    };

    int main() {
        MTVector arr;

        std::thread t1([&] () {
            for (int i = 0; i < 1000; i++) {
                arr.push_back(i);
            }
        });

        std::thread t2([&] () {
            for (int i = 0; i < 1000; i++) {
                arr.push_back(1000 + i);
            }
        });

        t1.join();
        t2.join();

        std::cout << arr.size() << std::endl;

        return 0;
    }
    ```
-  尽管保证了线程安全，但效率太低，读取时也要等待解锁，其实只要**保证读取时不会修改数据，修改时不会有人读取即可**，对多个读取线程无需上锁。
- 针对这种更具体的情况，又发明了读写锁，他**允许的状态**有：
- n个人读取，没有人写入。
- 1个人写入，没有人读取。
- 没有人读取，也没有人写入。
- 结论：**读可以共享，写必须独占，且写和读不能共存**。
- 为此，标准库提供了 `std::shared_mutex`。
## ***std::shared_mutex***
- 上锁时，要指定你的需求是**读**还是**写**，负责调度的读写锁会帮你判断要不要等待。
- 使用 `lock_shared()` 和 `unlock_shared()` 的组合。

### `lock_shared()` / `unlock_shared()`
- **读写锁实现线程安全vector**
    ```cpp
    #include <iostream>
    #include <thread>
    #include <vector>
    #include <shared_mutex>

    class MTVector {
        std::vector<int> m_arr;
        mutable std::shared_mutex m_mtx;

    public:
        void push_back(int val) {
            m_mtx.lock();            //修改数据时正常上锁
            m_arr.push_back(val);
            m_mtx.unlock();
        }

        size_t size() const {
            m_mtx.lock_shared();            //读取数据时使用共享锁，读取时无需等解锁，
            size_t ret = m_arr.size();
            m_mtx.unlock_shared();
            return ret;
        }
    };

    int main() {
        MTVector arr;

        std::thread t1([&] () {
            for (int i = 0; i < 1000; i++) {
                arr.push_back(i);
            }
        });

        std::thread t2([&] () {
            for (int i = 0; i < 1000; i++) {
                arr.push_back(1000 + i);
            }
        });

        t1.join();
        t2.join();

        std::cout << arr.size() << std::endl;

        return 0;
    }
    ```
### `std::shared_lock`
- 正如 `std::unique_lock` 针对` lock()`，也可以用 `std::shared_lock` 针对 `lock_shared()`, 符合 `RAII` 思想。
- `shared_lock` 同样支持 `defer_lock`，`try_to_lock` 等 tag 做参数，`owns_lock()` 判断等。
    ```cpp
    #include <iostream>
    #include <thread>
    #include <vector>
    #include <mutex>
    #include <shared_mutex>

    class MTVector {
        std::vector<int> m_arr;
        mutable std::shared_mutex m_mtx;

    public:
        void push_back(int val) {
            std::unique_lock grd(m_mtx);     //使用 unique_lock 替代 lock()
            m_arr.push_back(val);
        }

        size_t size() const {
            std::shared_lock grd(m_mtx);     //使用 shared_lock 替代 lock_shared()
            return m_arr.size();
        }
    };

    int main() {
        MTVector arr;

        std::thread t1([&] () {
            for (int i = 0; i < 1000; i++) {
                arr.push_back(i);
            }
        });

        std::thread t2([&] () {
            for (int i = 0; i < 1000; i++) {
                arr.push_back(1000 + i);
            }
        });

        t1.join();
        t2.join();

        std::cout << arr.size() << std::endl;

        return 0;
    }
    ```

#  条件变量

## ***condition_variable***

- 等待被唤醒，满足某个条件才能继续执行

### `wait()` / `cv.notify_one()`
- `wait()`将会让当前线程陷入等待.
- 在其他线程中调用 `cv.notify_one()` 则会唤醒那个陷入等待的线程。
- `std::condition_variable` 必须和 `std::unique_lock<std::mutex>` 一起用
    ```cpp
    #include <iostream>
    #include <thread>
    #include <mutex>
    #include <condition_variable>

    int main() {
        std::condition_variable cv;            //定义条件变量
        std::mutex mtx;

        std::thread t1([&] {
            std::unique_lock lck(mtx);         //配合std::unique_lock<std::mutex>一起使用
            cv.wait(lck);                       //等待

            std::cout << "t1 is awake" << std::endl;
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        std::cout << "notifying..." << std::endl;
        cv.notify_one();  // will awake t1

        t1.join();

        return 0;
    }
    ```
- `wait()` 还可以额外指定一个参数，变成 `cv.wait(lck, expr)` 的形式，其中 `expr` 是个 `lambda` 表达式，只有其返回值为 `true` 时才会真正唤醒，否则继续等待。
    ```cpp
    #include <iostream>
    #include <thread>
    #include <mutex>
    #include <condition_variable>

    int main() {
        std::condition_variable cv;
        std::mutex mtx;
        bool ready = false;

        std::thread t1([&] {
            std::unique_lock lck(mtx);
            cv.wait(lck, [&] { return ready; });    //不仅要等待唤醒，还要满足lambda中的表达式才行

            std::cout << "t1 is awake" << std::endl;
        });

        std::cout << "notifying not ready" << std::endl;
        cv.notify_one();  // useless now, since ready = false

        ready = true;                   //条件满足，再次执行notify_one即可唤醒
        std::cout << "notifying ready" << std::endl;
        cv.notify_one();  // awakening t1, since ready = true

        t1.join();

        return 0;
    }
    ```

### `notify_all() `

- `cv.notify_one()` 只会唤醒其中一个等待中的线程，而 `cv.notify_all()` 会唤醒全部。
  
- 这就是为什么 `wait()` 需要一个 `unique_lock` 作为参数，**因为要保证多个线程被唤醒时，只有一个能够被启动**。如果不需要，在 `wait()` 返回后调用 `lck.unlock()` 即可。
    ```cpp
    #include <iostream>
    #include <thread>
    #include <vector>
    #include <mutex>
    #include <condition_variable>

    int main() {
        std::condition_variable cv;
        std::mutex mtx;

        std::thread t1([&] {
            std::unique_lock lck(mtx);             //配合unique_lock可以保证只有一个线程在等待，t1上锁并等待唤醒，t2,t3都被锁住
            cv.wait(lck);
            std::cout << "t1 is awake" << std::endl;
        });

        std::thread t2([&] {
            std::unique_lock lck(mtx);
            cv.wait(lck);
            std::cout << "t2 is awake" << std::endl;
        });

        std::thread t3([&] {
            std::unique_lock lck(mtx);
            cv.wait(lck);
            std::cout << "t3 is awake" << std::endl;
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        std::cout << "notifying one" << std::endl;
        cv.notify_one();  // awakening t1 only

        std::this_thread::sleep_for(std::chrono::milliseconds(400));

        std::cout << "notifying all" << std::endl;
        cv.notify_all();  // awakening t1 and t2

        t1.join();
        t2.join();
        t3.join();

        return 0;
    }
    ```
> **注意**
- `std::condition_variable` 仅仅支持 `std::unique_lock<std::mutex>` 作为 `wait` 的参数，如果需要用其他类型的 `mutex` 锁，可以用 `std::condition_variable_any`。
- 他还有 `wait_for()` 和 `wait_until()` 函数，分别接受 `chrono` **时间段**和**时间点**作为参数。详见：[cppreference](https://en.cppreference.com/w/cpp/thread/condition_variable/wait_for)。

### `wait_for()` / `wait_until()`

# 原子
- 一些代码看起来我们只写了一句，但 cpu 看起来其实是多个指令，此时使用多线程会破坏原子性。
**经典案例：多个线程修改同一个计数器**
  - 多个线程同时往一个 int 变量里累加，这样肯定会出错，因为 counter += i 在 CPU 看来会变成三个指令：
  - 读取 counter 变量到 rax 寄存器
  - rax 寄存器的值加上 1
  - 把 rax 写入到 counter 变量
    ```cpp
    #include <iostream>
    #include <thread>

    int main() {
        int counter = 0;

        std::thread t1([&] {
            for (int i = 0; i < 10000; i++) {
                counter += 1;
            }
        });

        std::thread t2([&] {
            for (int i = 0; i < 10000; i++) {
                counter += 1;
            }
        });

        t1.join();
        t2.join();

        std::cout << counter << std::endl;

        return 0;
    }
    //实际执行结果小于20000，因为破坏了原子性
    ```
   - 即使编译器优化成 `add [counter]`, 也没用，因为现代 CPU 为了高效，使用了大量奇技淫巧，比如他会把一条汇编指令拆分成很多微指令 (micro-ops)，三个甚至有点保守估计了。
   - 问题是，如果有多个线程同时运行，顺序是不确定的：
  - `t1`：读取 `counter` 变量，到 `rax` 寄存器
  - `t2`：读取 `counter` 变量，到 `rax` 寄存器
  - `t1`：`rax` 寄存器的值加上 1
  - `t2`：`rax` 寄存器的值加上 1
  - `t1`：把 `rax` 写入到 `counter` 变量
  - `t2`：把 `rax` 写入到 `counter` 变量
  - 如果是这种顺序，最后 `t1` 的写入就被 `t2` 覆盖了，从而 `counter` 只增加了 1，而没有像预期的那样增加 2。
  - 更不用说现代 CPU 还有高速缓存，乱序执行，指令级并行等优化策略，你根本不知道每条指令实际的先后顺序。
**暴力解决：用 mutex 上锁**
    ```cpp
    #include <iostream>
    #include <thread>
    #include <mutex>

    int main() {
        std::mutex mtx;
        int counter = 0;

        std::thread t1([&] {
            for (int i = 0; i < 10000; i++) {
                mtx.lock();
                counter += 1;
                mtx.unlock();
            }
        });

        std::thread t2([&] {
            for (int i = 0; i < 10000; i++) {
                mtx.lock();
                counter += 1;
                mtx.unlock();
            }
        });

        t1.join();
        t2.join();

        std::cout << counter << std::endl;

        return 0;
    }
    ```
  - 这样的确可以防止多个线程同时修改 `counter` 变量，从而不会冲突。
  - 问题：`mutex` 太过重量级，他会让线程被挂起，从而需要通过系统调用，进入**内核层**，调度到其他线程执行，**有很大的开销**。
  - 可我们只是想要修改一个小小的 `int` 变量而已，用昂贵的 `mutex` 严重影响了效率。
  
## ***std::atomic***
- **有专门的硬件指令加持**
- 因此可以用更轻量级的 `atomic`，对他的 `+=` 等操作，会被编译器转换成专门的指令。
- CPU 识别到该指令时，会锁住内存总线，放弃乱序执行等优化策略（将该指令视为一个同步点，强制同步掉之前所有的内存操作），从而向你保证该操作是原子 (atomic) 的（取其不可分割之意），不会加法加到一半另一个线程插一脚进来。
- 对于程序员，只需把 `int` 改成 `atomic<int>` 即可，也不必像 `mutex` 那样需要手动上锁解锁，因此用起来也更直观。
    ```cpp
    #include <iostream>
    #include <thread>
    #include <atomic>

    int main() {
        std::atomic<int> counter = 0;       //保证对该对象非原子操作的原子性

        std::thread t1([&] {
            for (int i = 0; i < 10000; i++) {
                counter += 1;
            }
        });

        std::thread t2([&] {
            for (int i = 0; i < 10000; i++) {
                counter += 1;
            }
        });

        t1.join();
        t2.join();

        std::cout << counter << std::endl;

        return 0;
    }
    ```
- 对于 `atomic` 对象， `+=`、`-=`、`&=`、`|=`、`^=`、`++`、`--`等操作都可以保证原子性。
- **注意**：`counter = counter + 1` 不能保证原子性。
### `fetch_add` / `store` / `load`
- 除了用方便的运算符重载之外，还可以直接调用相应的函数名，比如：
- `fetch_add` 对应于 `+=`
- `store` 对应于 `=`
- `load` 用于读取其中的 `int` 值

- 强于 `+=` 的一点是， `fetch_add` **会返回其旧值**
    ```cpp
    #include <iostream>
    #include <thread>
    #include <atomic>
    #include <vector>

    int main() {
        std::atomic<int> counter;
        counter.store(0);

        std::vector<int> data(20000);

        std::thread t1([&] {
            for (int i = 0; i < 10000; i++) {
                int index = counter.fetch_add(1);    //index 保存的是旧值
                data[index] = i;
            }
        });

        std::thread t2([&] {
            for (int i = 0; i < 10000; i++) {
                int index = counter.fetch_add(1);
                data[index] = i + 10000;
            }
        });

        t1.join();
        t2.join();

        std::cout << data[10000] << std::endl;

        return 0;
    }
    ```
- 这个特点使得他可以用于并行地往一个列表里追加数据：追加写入的索引就是 `fetch_add` 返回的旧值。
- 当然这里也可以 `counter++`，不过要**追加多个**的话还是得用到 `counter.fetch_add(n)`

### `exchange`
- 读取的同时写入
- `exchange(val)` 会把 `val` 写入原子变量，同时返回其**旧值**。
    ```cpp
    #include <iostream>
    #include <atomic>

    int main() {
        std::atomic<int> counter;

        counter.store(0);

        int old = counter.exchange(3);       //返回旧值 0 并写入3
        std::cout << "old=" << old << std::endl;  // 0

        int now = counter.load();            // 拿出现在的值
        std::cout << "cnt=" << now << std::endl;  // 3

        return 0;
    }
    ```
### `compare_exchange_strong`
- `compare_exchange_strong(old, val) `会读取原子变量的值，比较他是否和 `old` 相等：
- 如果不相等，则把原子变量的值写入 `old`。
- 如果相等，则把 `val` 写入原子变量。
- 返回一个 `bool` 值，表示是否相等。
- 注意 `old` 这里传的其实是一个引用，因此 `compare_exchange_strong` 可修改他的值。
    ```cpp
    #include <iostream>
    #include <atomic>

    int main() {
        boolalpha(std::cout);
        std::atomic<int> counter;

        counter.store(2);

        int old = 1;
        bool equal = counter.compare_exchange_strong(old, 3);    //判断 counter 是否等于 old，等于返回true，并写入第二个参数 3，否则返回false，不写入
        std::cout << "equal=" << equal << std::endl;  // false
        std::cout << "old=" << old << std::endl;  // 2

        int now = counter.load();
        std::cout << "cnt=" << now << std::endl;  // 3

        old = 2;
        equal = counter.compare_exchange_strong(old, 3);
        std::cout << "equal=" << equal << std::endl;  // true
        std::cout << "old=" << old << std::endl;  // 2

        now = counter.load();
        std::cout << "cnt=" << now << std::endl;  // 3

        return 0;
    }
    ```

# 自旋锁
相比于系统调用阻塞线程，自旋锁更加轻量级，没有上下文的互换。  
如果进线程无法取得锁，进线程**不会立刻放弃CPU时间片**，而是**一直循环尝试获取锁**，直到获取为止。如果**别的线程长时期占有锁**那么**自旋就是在浪费CPU做无用功**，但是自旋锁一般应用于**加锁时间很短**的场景，这个时候**效率比较高**。    
自旋锁 `spinlock` 得自己用 `atomic<bool>` 或者说 `CAS` 实现。  
`CAS(Compare and Swap)` 是一种**原子操作**，上面介绍的 `compare_exchange_weak` 和 `compare_exchange_strong` 都是 `stl` 中 `atomic` 库中的一个函数
如
```cpp
atomic<bool> flag_;
flag_.compare_exchange_weak(expected, true)
```
 当前值与期望值(`expect`)相等时，修改当前值为设定值(`desired`)，返回 `true`  
 当前值与期望值(`expect`)不等时，将期望值(`expect`)修改为当前值，返回 `false`

 - 基于 `atomic<bool>` 实现的简单自旋锁
    ```cpp
    #include <iostream>
    #include <thread>
    #include <atomic>

    std::atomic<bool> lock(false);  // 原子变量用于表示自旋锁的状态

    void criticalSection(int threadId) {
        while (lock.exchange(true)) {
            // 自旋等待，直到获取到自旋锁
        }
        // 进入临界区
        std::cout << "Thread " << threadId << " entered the critical section." << std::endl;

        // 模拟临界区的工作
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 离开临界区
        std::cout << "Thread " << threadId << " exited the critical section." << std::endl;

        lock.store(false);  // 释放自旋锁
    }

    int main() {
        std::thread t1(criticalSection, 1);
        std::thread t2(criticalSection, 2);

        t1.join();
        t2.join();

        return 0;
    }
    ```
- 基于 `CAS` 实现的简单自旋锁
    ```cpp
    #include <atomic>

    class SpinLock
    {
    public:
        SpinLock() : flag_(false) {}

        void lock(){
            bool expected = false;
            while (!flag_.compare_exchange_weak(expected, true))
            {
                expected = false;
                // 自旋等待，直到成功获取到自旋锁
                // 使用compare_exchange_weak函数尝试将flag_的值从expected（false）替换为true
                // 如果替换成功，则表示获取到了自旋锁，循环结束
                // 如果替换失败，则将expected重置为false，再次尝试获取自旋锁
                // 这里使用compare_exchange_weak而不是compare_exchange_strong是为了性能考虑
            }
        }
        void unlock(){
            flag_.store(false);
            // 释放自旋锁，将flag_设置为false，表示自旋锁不再被持有
        }
    private:
        std::atomic<bool> flag_;
        // 使用std::atomic<bool>来表示自旋锁的状态
        // 原子变量保证了多线程环境下的原子性操作，避免竞态条件
    };
    ```
