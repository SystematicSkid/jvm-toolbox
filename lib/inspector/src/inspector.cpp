#include "inspector.hpp"
#include "interfaces/jvmti_interface.hpp"

int main( )
{
    new inspector::interfaces::JvmtiInterface( nullptr );
    return 0;
}