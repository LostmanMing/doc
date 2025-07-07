#include <tuple>
#include <type_traits>
typedef enum function_type {
    FUNCTION_TYPE_A,
    FUNCTION_TYPE_B,
    FUNCTION_TYPE_C,
    FUNCTION_TYPE_D,
} function_type_t;

void FunctionA(int a, double b);
void FunctionB(const char* str);
void FunctionC(float x, float y, float z);
void FunctionD(int id, const char* name);

namespace enum_helper {


template<typename F>
inline void for_each_in_parameter_pack(F&& f) {
    // Base case: no parameters to process
}

template<typename F, typename First, typename... Rest>
inline void for_each_in_parameter_pack(F&& f, First const & first, Rest const&... rest) {
    std::forward<F>(f)(first);
    for_each_in_parameter_pack(std::forward<F>(f), rest...);
}

template<typename... Ts,typename F, size_t... Is>
inline void for_each_in_tuple_helper(std::tuple<Ts...> const& t, F&& f, std::index_sequence<Is...>) {
    for_each_in_parameter_pack(std::forward<F>(f), std::get<Is>(t)...);
}

template<typename... Ts, typename F>
void for_each_in_tuple(std::tuple<Ts...> const& t, F&& f) {
    for_each_in_tuple_helper(t, std::forward<F>(f), std::make_index_sequence<sizeof...(Ts)>());
}

inline namespace detail {
using id_t = unsigned int;

template <id_t Value>
using id_v = std::integral_constant<id_t, Value>;

template <typename FuncT>
class Hander {
 public:
    id_t id;
    FuncT func;

    template <id_t EnumValue>
    constexpr Hander(id_v<EnumValue> id, FuncT func)
        : id(EnumValue), func(func) {}
};

template <typename idT, typename FuncT>
constexpr Hander<FuncT> MakeHander(idT id, FuncT func) {
    return Hander<FuncT>(id, func);
}

constexpr auto MakeHandlerFromTuple() {
    return std::tuple<>{};
}

template <typename idT, typename FuncT, typename... Args>
constexpr auto MakeHandlerFromTuple(idT id, FuncT func, Args... args) {
    auto rest = MakeHandlerFromTuple(args...);
    return std::tuple_cat(std::make_tuple(MakeHander(id, func)), rest);
}
}  //  namespace detail

template <typename... Args>
constexpr auto MakeHandlerTable(Args... args) {
    return MakeHandlerFromTuple(args...);
}
}  //  namespace enum_helper
