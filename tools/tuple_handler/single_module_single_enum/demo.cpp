#include "constexpr_tuple_handler.h"

void FunctionA(int a, double b) {
    // Implementation for FunctionA
}
void FunctionB(const char* str) {
    // Implementation for FunctionB
}
void FunctionC(float x, float y, float z) {
    // Implementation for FunctionC
}

void FunctionD(int id, const char* name) {
    // Implementation for FunctionD
}



int main() {
    // this table is build all at compile time
    auto HanderTable = enum_helper::MakeHandlerTable(
        enum_helper::id_v<FUNCTION_TYPE_A>{}, FunctionA,
        enum_helper::id_v<FUNCTION_TYPE_B>{}, FunctionB,
        enum_helper::id_v<FUNCTION_TYPE_C>{}, FunctionC,
        enum_helper::id_v<FUNCTION_TYPE_D>{}, FunctionD);
    
    enum_helper::for_each_in_tuple(HanderTable, [](const auto& handler) {
        // Here you can use handler.id and handler.func
        // For example, you can call the function:
        if constexpr (std::is_invocable_v<decltype(handler.func), int, double>) {
            handler.func(1, 2.0); // Example call for FunctionA
            printf("FunctionA called with id: %u\n", handler.id);
        } else if constexpr (std::is_invocable_v<decltype(handler.func), const char*>) {
            handler.func("Hello"); // Example call for FunctionB
            printf("FunctionB called with id: %u\n", handler.id);
        } else if constexpr (std::is_invocable_v<decltype(handler.func), float, float, float>) {
            handler.func(1.0f, 2.0f, 3.0f); // Example call for FunctionC
            printf("FunctionC called with id: %u\n", handler.id);
        } else if constexpr (std::is_invocable_v<decltype(handler.func), int, const char*>) {
            handler.func(42, "World"); // Example call for FunctionD
            printf("FunctionD called with id: %u\n", handler.id);
        }
    });
}