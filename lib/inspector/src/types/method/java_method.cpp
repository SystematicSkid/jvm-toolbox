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

bool inspector::types::JavaMethod::get_method_signature( std::string& signature )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;
    std::string name;
    std::string generic_signature;

    /* Get the method signature */
    return inspector::java_interface->get_method_name( _method, name, signature, generic_signature );
}

bool inspector::types::JavaMethod::get_method_generic_signature( std::string& generic_signature )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;
    std::string name;
    std::string signature;

    /* Get the method generic signature */
    return inspector::java_interface->get_method_name( _method, name, signature, generic_signature );
}

bool inspector::types::JavaMethod::get_method_declaring_class( std::unique_ptr<JavaClass>& class_ )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method declaring class */
    return inspector::java_interface->get_method_declaring_class( _method, class_ );
}

bool inspector::types::JavaMethod::get_method_modifiers( std::int32_t& modifiers )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method modifiers */
    return inspector::java_interface->get_method_modifiers( _method, modifiers );
}

bool inspector::types::JavaMethod::get_max_locals( std::int32_t& max_locals )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method max locals */
    return inspector::java_interface->get_max_locals( _method, max_locals );
}

bool inspector::types::JavaMethod::get_arguments_size( std::int32_t& arguments_size )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method arguments size */
    return inspector::java_interface->get_arguments_size( _method, arguments_size );
}

bool inspector::types::JavaMethod::get_line_number_table( std::vector<std::pair<std::int32_t, std::int32_t>>& line_number_table )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method line number table */
    return inspector::java_interface->get_line_number_table( _method, line_number_table );
}

bool inspector::types::JavaMethod::get_method_location( std::int32_t& start, std::int32_t& end )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method location */
    return inspector::java_interface->get_method_location( _method, start, end );
}

bool inspector::types::JavaMethod::get_local_variable_table( std::vector<void*>& local_variable_table )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method local variable table */
    return inspector::java_interface->get_local_variable_table( _method, local_variable_table );
}

bool inspector::types::JavaMethod::get_bytecodes( std::vector<std::uint8_t>& bytecodes )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method bytecodes */
    return inspector::java_interface->get_bytecodes( _method, bytecodes );
}

bool inspector::types::JavaMethod::get_method_is_native( bool& is_native )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method is native */
    return inspector::java_interface->get_method_is_native( _method, is_native );
}

bool inspector::types::JavaMethod::get_method_is_synthetic( bool& is_synthetic )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method is synthetic */
    return inspector::java_interface->get_method_is_synthetic( _method, is_synthetic );
}

bool inspector::types::JavaMethod::get_method_is_obsolete( bool& is_obsolete )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Get the method is obsolete */
    return inspector::java_interface->get_method_is_obsolete( _method, is_obsolete );
}

bool inspector::types::JavaMethod::set_method_breakpoint( std::int64_t location )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Set the method breakpoint */
    return inspector::java_interface->set_method_breakpoint( _method, location );
}

bool inspector::types::JavaMethod::clear_method_breakpoint( std::int64_t location )
{
    /* Ensure we have an inspector interface setup */
    if ( !inspector::java_interface )
        return false;

    /* Clear the method breakpoint */
    return inspector::java_interface->clear_method_breakpoint( _method, location );
}