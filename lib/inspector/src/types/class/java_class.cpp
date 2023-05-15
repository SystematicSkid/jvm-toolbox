#include "../../inspector.hpp"
#include "java_class.hpp"

bool inspector::types::JavaClass::get_class_signature( std::string& signature, std::string& generic_ptr )
{
        /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the source file name */
    return inspector::java_interface->get_class_signature( _class, signature, generic_ptr );
}

bool inspector::types::JavaClass::get_class_source_file_name( std::string& source_file_name )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the source file name */
    return inspector::java_interface->get_class_source_file_name( _class, source_file_name );
}

bool inspector::types::JavaClass::get_class_methods( std::vector<std::unique_ptr<inspector::types::JavaMethod>>& methods )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the methods */
    return inspector::java_interface->get_class_methods( _class, methods );
}