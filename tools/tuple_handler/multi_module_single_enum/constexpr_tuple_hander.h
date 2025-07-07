#include <tuple>
#include <type_traits>
#include <cstdio>
typedef enum function_cbid {
    MODULE_FUNCTION_INVALID = 0,
    MODULE_A_FUNCTION_A_CBID = 1,
    MODULE_A_FUNCTION_B_CBID = 2,
    MODULE_B_FUNCTION_C_CBID = 3,
    MODULE_B_FUNCTION_D_CBID = 4
} function_cbid;

typedef enum function_mod {
    MODULE_INVALID = 0,
    MODULE_A = 1,
    MODULE_B = 2,
} function_mod;

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
template <id_t EnumValue>
using id_v = std::integral_constant<id_t, EnumValue>;

template <function_mod Mod>
constexpr id_t ModuleIdBase = 0;

template <>
constexpr id_t ModuleIdBase<MODULE_A> = MODULE_A_FUNCTION_A_CBID;

template <>
constexpr id_t ModuleIdBase<MODULE_B> = MODULE_B_FUNCTION_C_CBID;

template <typename idT>
constexpr function_mod IdToModuleId = []() constexpr {
    constexpr id_t id = idT::value;
    if constexpr (id >= MODULE_A_FUNCTION_A_CBID && id <= MODULE_A_FUNCTION_B_CBID) {
        return MODULE_A;
    } else if constexpr (id >= MODULE_B_FUNCTION_C_CBID && id <= MODULE_B_FUNCTION_D_CBID) {
        return MODULE_B;
    } else {
        return MODULE_INVALID;
    }
}();

template <typename FuncT>
class Handler {
 public:
    id_t id;
    id_t original_id;
    FuncT func;
    
    template <id_t EnumValue, id_t BaseValue>
    constexpr Handler(id_v<EnumValue> id, id_v<BaseValue>, FuncT func)
        : id(EnumValue - BaseValue), original_id(EnumValue), func(func) {}
};

// "Make" function for Handler to automatically deduce type
template <typename idT, typename funcT>
constexpr Handler<funcT> MakeHander(idT id, funcT func) {
    constexpr function_mod mod = IdToModuleId<idT>;
    return Handler<funcT>(id, id_v<ModuleIdBase<mod>>{}, func);
}


template <function_mod mod, typename... FuncT>
class ModuleHandlerTable {
 public:
    using tuple_t = std::tuple<Handler<FuncT>...>;
    tuple_t handlers;
    // other members and methods as needed

    constexpr explicit ModuleHandlerTable(tuple_t t)
    : handlers(t) {}
    
    void process() const {
        // Example processing function that iterates over handlers
        for_each_in_tuple(handlers, [](const auto& handler) {
            // Here you can call handler.func with appropriate arguments
            // For demonstration, we will just print the id
            printf("Handler ID: %u, cbid: %u\n", handler.id, handler.original_id);
        });
    }
};

// Base case: empty args
template <function_mod mod>
constexpr auto MakeModuleHandlerTuple() {
    return std::tuple<>{};
}

// Pre-declaration before recursive call -- needed since they can call each other
template <function_mod mod,
          typename IdT,
          typename FuncT,
          typename... Rest,
          std::enable_if_t<IdToModuleId<IdT> == mod, int> = 0>
constexpr auto MakeModuleHandlerTuple(IdT id, FuncT func, Rest... rest);

template <function_mod mod,
          typename IdT,
          typename FuncT,
          typename... Rest,
          std::enable_if_t<IdToModuleId<IdT> != mod, int> = 0>
constexpr auto MakeModuleHandlerTuple(IdT id, FuncT func, Rest... rest);


// recursive case1: mod = IdToModuleId<IdT> 
template <function_mod mod,
          typename IdT,
          typename FuncT,
          typename... Rest,
          std::enable_if_t<IdToModuleId<IdT> == mod, int>>
constexpr auto MakeModuleHandlerTuple(IdT id, FuncT func, Rest... rest) {
    auto restTuple = MakeModuleHandlerTuple<mod>(rest...);
    return std::tuple_cat(
        std::make_tuple(MakeHander(id, func)),
        restTuple);
}

// recursive case2: mod != IdToModuleId<IdT>
template <function_mod mod,
          typename IdT,
          typename FuncT,
          typename... Rest,
          std::enable_if_t<IdToModuleId<IdT> != mod, int>>
constexpr auto MakeModuleHandlerTuple(IdT id, FuncT func, Rest... rest) {
    return MakeModuleHandlerTuple<mod>(rest...);
}

template <function_mod mod, typename... FuncT>
constexpr auto MakeModuleTableFromTuple(std::tuple<Handler<FuncT>...> t) {
    return ModuleHandlerTable<mod, FuncT...>(t);
}

// "Make" function for ModuleHandlerTable to automatically deduce type
template <function_mod mod, typename... Args>
constexpr auto MakeModuleHandlerTable(Args... args) {
    const auto handler_tuple = MakeModuleHandlerTuple<mod>(args...);
    return MakeModuleTableFromTuple<mod>(handler_tuple);
}

}  // namespace detail

template <typename... Args>
constexpr auto MakeHandlerTable(Args&&... args) {
    return std::make_tuple(
        MakeModuleHandlerTable<MODULE_A>(args...),
        MakeModuleHandlerTable<MODULE_B>(args...)
    );
}


}  // namespace enum_helper