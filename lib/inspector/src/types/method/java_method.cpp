#include "../../inspector.hpp"
#include "java_method.hpp"

bool inspector::types::JavaMethod::get_method_name( std::string& name )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;
    std::string signature;
    std::string generic_signature;

    /* Get the method name */
    return inspector::java_interface->get_method_name( _method, name, signature, generic_signature );
}