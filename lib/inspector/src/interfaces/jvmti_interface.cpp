#include "./jvmti_interface.hpp"
#include "utility/jni_errors.hpp"

inspector::interfaces::JvmtiInterface::JvmtiInterface( JavaVM* jvm )
{
    /* Clear class variables */
    this->_jvmti_env = nullptr;
    this->_last_error = 0;
    this->_last_error_message = "";

    /* Obtain our interface */
    jint error = jvm->GetEnv( (void**)&this->_jvmti_env, JVMTI_VERSION );
    if( error != JNI_OK )
    {
        this->_last_error = error;
        this->_last_error_message = "Failed to obtain jvmti interface, please try a different interface.";
        /* Append error code to end of error message */
        this->_last_error_message += " Error code: " + inspector::utility::jvm_error_codes[error];
    }
}

inspector::interfaces::JvmtiInterface::~JvmtiInterface( )
{
    this->destroy( );
}
 
bool inspector::interfaces::JvmtiInterface::initialize( )
{
    
}

bool inspector::interfaces::JvmtiInterface::destroy()
{
    return true;
}

std::string inspector::interfaces::JvmtiInterface::get_name()
{
    return std::string( "jvmti" );
}

std::int32_t inspector::interfaces::JvmtiInterface::get_last_error()
{
    return this->_last_error;
}

std::string inspector::interfaces::JvmtiInterface::get_last_error_message()
{
    return this->_last_error_message;
}

void inspector::interfaces::JvmtiInterface::set_last_error( std::int32_t error )
{
    this->_last_error = error;
    this->_last_error_message = inspector::utility::jvmti_error_codes[error];
}

bool inspector::interfaces::JvmtiInterface::get_loaded_classes( std::vector<void*>& classes )
{
    jint class_count = 0;
    jclass* class_ptr = nullptr;

    /* Get all loaded classes via jvmti */
    jvmtiError error = this->_jvmti_env->GetLoadedClasses( &class_count, &class_ptr );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    classes.reserve( class_count );
    for( auto i = 0; i < class_count; i++ )
    {
        classes.emplace_back( class_ptr[i] );
    }
    
    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( class_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_classloader_classes( void* classloader, std::vector<void*>& classes )
{
    /* Ensure classloader is valid */
    if( classloader == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint class_count = 0;
    jclass* class_ptr = nullptr;

    /* Get all loaded classes via jvmti */
    jvmtiError error = this->_jvmti_env->GetClassLoaderClasses( reinterpret_cast<jobject>( classloader ), &class_count, &class_ptr );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    classes.reserve( class_count );
    for( auto i = 0; i < class_count; i++ )
    {
        classes.emplace_back( class_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( class_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_signature( void* class_, std::string& signature, std::string& generic_ptr )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    char* signature_ptr = nullptr;
    char* generic_ptr_ptr = nullptr;

    /* Get class signature */
    jvmtiError error = this->_jvmti_env->GetClassSignature( reinterpret_cast<jclass>( class_ ), &signature_ptr, &generic_ptr_ptr );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Set our strings */
    signature = std::string( signature_ptr );
    generic_ptr = std::string( generic_ptr_ptr );

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( signature_ptr ) );
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( generic_ptr_ptr ) );

    return true;
}

