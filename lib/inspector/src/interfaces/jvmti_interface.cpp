#include "./jvmti_interface.hpp"
#include "utility/jni_errors.hpp"
#include "jvmti_interface.hpp"

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
    return true;
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

bool inspector::interfaces::JvmtiInterface::get_thread_state( void* thread, std::int32_t& state )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Get thread state */
    jvmtiError error = this->_jvmti_env->GetThreadState( reinterpret_cast<jthread>( thread ), 
        reinterpret_cast<jint*>( &state ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_current_thread( void*& thread )
{
    /* Get current thread */
    jvmtiError error = this->_jvmti_env->GetCurrentThread( reinterpret_cast<jthread*>( &thread ) );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_all_threads( std::vector<void*>& threads )
{
    jint thread_count = 0;
    jthread* thread_ptr = nullptr;

    /* Get all threads via jvmti */
    jvmtiError error = this->_jvmti_env->GetAllThreads( &thread_count, &thread_ptr );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    /* Reserve space on our vector */
    threads.reserve( thread_count );
    for( auto i = 0; i < thread_count; i++ )
    {
        threads.emplace_back( thread_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( thread_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::suspend_thread( void* thread )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Suspend thread */
    jvmtiError error = this->_jvmti_env->SuspendThread( reinterpret_cast<jthread>( thread ) );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::resume_thread( void* thread )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Resume thread */
    jvmtiError error = this->_jvmti_env->ResumeThread( reinterpret_cast<jthread>( thread ) );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::stop_thread( void* thread, void* exception )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Stop thread */
    jvmtiError error = this->_jvmti_env->StopThread( reinterpret_cast<jthread>( thread ), 
        reinterpret_cast<jobject>( exception )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::interrupt_thread( void* thread )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Interrupt thread */
    jvmtiError error = this->_jvmti_env->InterruptThread( reinterpret_cast<jthread>( thread ) );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_thread_info( void* thread, std::string& name, std::int32_t& priority, bool& daemon, void*& thread_group, void*& context_class_loader )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jvmtiThreadInfo thread_info = { 0 };

    /* Get thread info */
    jvmtiError error = this->_jvmti_env->GetThreadInfo( reinterpret_cast<jthread>( thread ), 
        &thread_info
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Set our strings */
    name = std::string( thread_info.name );
    priority = thread_info.priority;
    daemon = thread_info.is_daemon;
    thread_group = thread_info.thread_group;
    context_class_loader = thread_info.context_class_loader;

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( thread_info.name ) );

    return true;
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
    jvmtiError error = this->_jvmti_env->GetClassLoaderClasses( reinterpret_cast<jobject>( classloader ),
        &class_count,
        &class_ptr 
    );
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
    jvmtiError error = this->_jvmti_env->GetClassSignature( reinterpret_cast<jclass>( class_ ), 
        &signature_ptr,
        &generic_ptr_ptr 
    );
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

bool inspector::interfaces::JvmtiInterface::get_class_status( void* class_, std::int16_t& status )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Get class status */
    jvmtiError error = this->_jvmti_env->GetClassStatus( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jint*>( &status ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_source_file_name( void* class_, std::string& source_file_name )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    char* source_file_ptr = nullptr;

    /* Get class signature */
    jvmtiError error = this->_jvmti_env->GetSourceFileName( reinterpret_cast<jclass>( class_ ), 
        &source_file_ptr
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Set our strings */
    source_file_name = std::string( source_file_ptr );

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( source_file_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_modifiers( void *class_, std::int32_t &modifiers )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get class modifiers */
    jvmtiError error = this->_jvmti_env->GetClassModifiers( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jint*>( &modifiers ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_methods( void* class_, std::vector<void*>& methods )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint method_count = 0;
    jmethodID* method_ptr = nullptr;

    /* Get all loaded classes via jvmti */
    jvmtiError error = this->_jvmti_env->GetClassMethods( reinterpret_cast<jclass>( class_ ), 
        &method_count,
        &method_ptr 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    methods.reserve( method_count );
    for( auto i = 0; i < method_count; i++ )
    {
        methods.emplace_back( method_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( method_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_fields( void* class_, std::vector<void*>& fields )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint field_count = 0;
    jfieldID* field_ptr = nullptr;

    /* Get all loaded classes via jvmti */
    jvmtiError error = this->_jvmti_env->GetClassFields( reinterpret_cast<jclass>( class_ ), 
        &field_count,
        &field_ptr 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    fields.reserve( field_count );
    for( auto i = 0; i < field_count; i++ )
    {
        fields.emplace_back( field_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( field_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_interfaces( void* class_, std::vector<void*>& interfaces )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint interface_count = 0;
    jclass* interface_ptr = nullptr;

    /* Get all loaded classes via jvmti */
    jvmtiError error = this->_jvmti_env->GetImplementedInterfaces( reinterpret_cast<jclass>( class_ ), 
        &interface_count,
        &interface_ptr 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    interfaces.reserve( interface_count );
    for( auto i = 0; i < interface_count; i++ )
    {
        interfaces.emplace_back( interface_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( interface_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_version( void* class_, std::int32_t& minor, std::int32_t& major )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get class version */
    jvmtiError error = this->_jvmti_env->GetClassVersionNumbers( 
        reinterpret_cast<jclass>( class_ ),
        reinterpret_cast<jint*>( &minor ), 
        reinterpret_cast<jint*>( &major ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_constant_pool( void* class_, std::vector<unsigned char>& constant_pool )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    jint constant_pool_count = 0;
    jint constant_pool_byte_count = 0;
    unsigned char* constant_pool_bytes_ptr = nullptr;

    /* Get all loaded classes via jvmti */
    jvmtiError error = this->_jvmti_env->GetConstantPool( reinterpret_cast<jclass>( class_ ), 
        &constant_pool_count,
        &constant_pool_byte_count,
        &constant_pool_bytes_ptr
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    /* Reserve space on our vector */
    constant_pool.reserve( constant_pool_count );
    for( auto i = 0; i < constant_pool_count; i++ )
    {
        constant_pool.emplace_back( constant_pool_bytes_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( constant_pool_bytes_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_is_interface( void* class_, bool& is_interface )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get class is interface */
    jvmtiError error = this->_jvmti_env->IsInterface( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jboolean*>( &is_interface ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_is_array( void* class_, bool& is_array )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get class is array */
    jvmtiError error = this->_jvmti_env->IsArrayClass( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jboolean*>( &is_array ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_class_loader( void* class_, void*& class_loader )
{
    /* Ensure class is valid */
    if( class_ == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get class loader */
    jvmtiError error = this->_jvmti_env->GetClassLoader( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jobject*>( &class_loader ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}