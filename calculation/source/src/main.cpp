#include "StrProc.h"

int main()
{
    std::string strone = "abc";
    std::string strtwo = "abe";
    float t = StrProc::GetStrMatchDegree(strone, strtwo);
    printf("%f\n", t);
    return 0;
}
