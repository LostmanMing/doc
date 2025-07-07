#include "constexpr_tuple_hander.h"

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
    auto HanderTable = enum_helper::MakeHandlerTable(
        enum_helper::id_v<MODULE_A_FUNCTION_A_CBID>{}, FunctionA,
        enum_helper::id_v<MODULE_A_FUNCTION_B_CBID>{}, FunctionB,
        enum_helper::id_v<MODULE_B_FUNCTION_C_CBID>{}, FunctionC,
        enum_helper::id_v<MODULE_B_FUNCTION_D_CBID>{}, FunctionD);

    enum_helper::for_each_in_tuple(HanderTable, [](const auto& handler) {
        handler.process();
    });
    
}