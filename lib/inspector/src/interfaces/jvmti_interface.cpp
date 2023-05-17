#include "./jvmti_interface.hpp"
#include "types/types.hpp"
#include "utility/jni_errors.hpp"
#include "jvmti_interface.hpp"

#include <Windows.h>

inspector::interfaces::JvmtiInterface::JvmtiInterface( )
{
    /* Clear class variables */
    this->_jvmti_env = nullptr;
    this->_last_error = 0;
    this->_last_error_message = "";

    if( !this->initialize( ) )
    {
        /* Error handling */
        if( this->_last_error != JVMTI_ERROR_NONE )
        {
            /* Append error code to end of error message */
            this->_last_error_message += " Error code: " + inspector::utility::jvmti_error_codes[this->_last_error];
        }
    }
}

inspector::interfaces::JvmtiInterface::~JvmtiInterface( )
{
    this->destroy( );
}
 
bool inspector::interfaces::JvmtiInterface::initialize( )
{
    printf( "Initializing jvmti interface...\n" );
    /* Get all created Java VMS */
    jint vm_count = 0;
    JavaVM* jvm = nullptr;
    jint error = JNI_GetCreatedJavaVMs( &jvm, 1, &vm_count );
    /* Error handling */
    if( error != JNI_OK )
    {
        this->set_last_error( error );
        return false;
    }

    /* Ensure we have at least one Java VM */
    if( vm_count == 0 )
    {
        this->set_last_error( JVMTI_ERROR_INTERNAL );
        return false;
    }

    jint result = jvm->GetEnv( reinterpret_cast<void**>(&env), JNI_VERSION_1_6 );
	if ( result == JNI_EDETACHED )
    {
		result = jvm->AttachCurrentThread( reinterpret_cast<void**>( &env ), nullptr );
    }
    if ( result != JNI_OK )
    {
        this->set_last_error( result );
        return false;
    }

    /* Obtain our interface */
    error = jvm->GetEnv( (void**)&this->_jvmti_env, JVMTI_VERSION );
    if( error != JNI_OK )
    {
        this->_last_error = error;
        this->_last_error_message = "Failed to obtain jvmti interface, please try a different interface.";
        /* Append error code to end of error message */
        this->_last_error_message += " Error code: " + inspector::utility::jvm_error_codes[error];
    }

    /* initialize all capabilities for jvmti */
    jvmtiCapabilities capabilities;
    memset( &capabilities, 0, sizeof( jvmtiCapabilities ) );
    //capabilities.can_tag_objects = 1;
    //capabilities.can_generate_field_modification_events = 1;
    //capabilities.can_generate_field_access_events = 1;
    //capabilities.can_get_bytecodes = 1;
    //capabilities.can_get_synthetic_attribute = 1;
    //capabilities.can_get_owned_monitor_info = 1;
    //capabilities.can_get_current_contended_monitor = 1;
    //capabilities.can_get_monitor_info = 1;
    //capabilities.can_pop_frame = 1;
    //capabilities.can_redefine_classes = 1;
    //capabilities.can_signal_thread = 1;
    capabilities.can_get_source_file_name = 1;
    //capabilities.can_get_line_numbers = 1;
    //capabilities.can_get_source_debug_extension = 1;
    //capabilities.can_access_local_variables = 1;
    //capabilities.can_maintain_original_method_order = 1;
    //capabilities.can_generate_single_step_events = 1;
    //capabilities.can_generate_exception_events = 1;
    //capabilities.can_generate_frame_pop_events = 1;
    //capabilities.can_generate_breakpoint_events = 1;
    //capabilities.can_suspend = 1;
    //capabilities.can_redefine_any_class = 1;
    //capabilities.can_get_current_thread_cpu_time = 1;
    //capabilities.can_get_thread_cpu_time = 1;
    //capabilities.can_generate_method_entry_events = 1;
    //capabilities.can_generate_method_exit_events = 1;
    capabilities.can_generate_all_class_hook_events = 1;
    //capabilities.can_generate_compiled_method_load_events = 1;
    //capabilities.can_generate_monitor_events = 1;
    //capabilities.can_generate_vm_object_alloc_events = 1;
    //capabilities.can_generate_native_method_bind_events = 1;
    //capabilities.can_generate_garbage_collection_events = 1;
    //capabilities.can_generate_object_free_events = 1;
    //capabilities.can_force_early_return = 1;
    //capabilities.can_get_owned_monitor_stack_depth_info = 1;
    //capabilities.can_get_constant_pool = 1;
    //capabilities.can_set_native_method_prefix = 1;
    //capabilities.can_retransform_classes = 1;
    //capabilities.can_retransform_any_class = 1;
    //capabilities.can_generate_resource_exhaustion_heap_events = 1;
    //capabilities.can_generate_resource_exhaustion_threads_events = 1;
    //capabilities.can_generate_early_vmstart = 1;
    //capabilities.can_generate_early_class_hook_events = 1;
    //capabilities.can_generate_sampled_object_alloc_events = 1;

    /* Set our capabilities */
    error = this->_jvmti_env->AddCapabilities( &capabilities );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

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

bool inspector::interfaces::JvmtiInterface::get_stack_trace( void* thread, std::int32_t start_depth, std::int32_t max_frame_count, std::vector<void*>& frames )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    jint frame_count = 0;
    jvmtiFrameInfo* frame_ptr = new jvmtiFrameInfo[max_frame_count];

    /* Get stack trace via jvmti */
    jvmtiError error = this->_jvmti_env->GetStackTrace( reinterpret_cast<jthread>( thread ), 
        start_depth,
        max_frame_count,
        frame_ptr,
        &frame_count
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    frames.reserve( frame_count );
    for( auto i = 0; i < frame_count; i++ )
    {
        frames.emplace_back( frame_ptr[i].method );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( frame_ptr ) );
    /* Free */
    delete[] frame_ptr;

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_all_stack_traces( std::int32_t max_frame_count, std::vector<void*>& frames )
{
    jint thread_count = 0;
    jvmtiStackInfo* stack_info = nullptr;

    /* Get all threads via jvmti */
    jvmtiError error = this->_jvmti_env->GetAllStackTraces( max_frame_count,
        &stack_info,
        &thread_count
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    frames.reserve( thread_count );
    for( auto i = 0; i < thread_count; i++ )
    {
        frames.emplace_back( stack_info[i].frame_buffer );
    }

    /* Deallocate memory */
    /* 
        JVMTI: Note that this buffer is allocated to include the jvmtiFrameInfo buffers pointed
        to by jvmtiStackInfo.frame_buffer. These buffers must not be separately deallocated.
    */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( stack_info ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_frame_count( void* thread, std::int32_t& count )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get frame count via jvmti */
    jvmtiError error = this->_jvmti_env->GetFrameCount( reinterpret_cast<jthread>( thread ), 
        reinterpret_cast<jint*>( &count )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_frame_location( void* thread, std::int32_t depth, void*& method, std::int32_t& location )
{
    /* Ensure thread is valid */
    if( thread == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get frame location via jvmti */
    jvmtiError error = this->_jvmti_env->GetFrameLocation( reinterpret_cast<jthread>( thread ), 
        depth,
        reinterpret_cast<jmethodID*>( &method ),
        reinterpret_cast<jlocation*>( &location )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_object_size( void* object, std::int32_t& size )
{
    /* Ensure object is valid */
    if( object == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get object size via jvmti */
    jvmtiError error = this->_jvmti_env->GetObjectSize( reinterpret_cast<jobject>( object ), 
        reinterpret_cast<jlong*>( &size )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_object_hash_code( void* object, std::int32_t& hash_code )
{
    /* Ensure object is valid */
    if( object == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get object hash code via jvmti */
    jvmtiError error = this->_jvmti_env->GetObjectHashCode( reinterpret_cast<jobject>( object ), 
        reinterpret_cast<jint*>( &hash_code )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_loaded_classes( std::vector<std::unique_ptr<inspector::types::JavaClass>>& classes )
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
        classes.emplace_back( std::make_unique<inspector::types::JavaClass>( class_ptr[i] ) );
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
    if( signature_ptr != nullptr )
    {
        signature = std::string( signature_ptr );
    }
    else
    {
        signature = "";
    }
    if( generic_ptr_ptr != nullptr )
    {
        generic_ptr = std::string( generic_ptr_ptr );
    }
    else
    {
        generic_ptr = "";
    }

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

bool inspector::interfaces::JvmtiInterface::get_class_methods( void* class_, std::vector<std::unique_ptr<inspector::types::JavaMethod>>& methods )
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
        methods.emplace_back( std::make_unique<inspector::types::JavaMethod>( method_ptr[i] ) );
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

bool inspector::interfaces::JvmtiInterface::get_field_name( void* class_, void* field, std::string& name, std::string& signature, std::string& generic_ptr )
{
    /* Ensure field is valid */
    if( field == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    char* name_ptr = nullptr;
    char* signature_ptr = nullptr;
    char* generic_ptr_ptr = nullptr;

    /* Get field name */
    jvmtiError error = this->_jvmti_env->GetFieldName( reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jfieldID>( field ),
        &name_ptr,
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
    name = std::string( name_ptr );
    signature = std::string( signature_ptr );
    generic_ptr = std::string( generic_ptr_ptr );

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( name_ptr ) );
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( signature_ptr ) );
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( generic_ptr_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_field_modifiers( void* class_, void* field, std::int32_t& modifiers )
{
    /* Ensure field is valid */
    if( field == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get field modifiers */
    jvmtiError error = this->_jvmti_env->GetFieldModifiers( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jfieldID>( field ),
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

bool inspector::interfaces::JvmtiInterface::get_field_declaring_class( void* class_, void* field, void*& declaring_class )
{
    /* Ensure field is valid */
    if( field == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get field declaring class */
    jvmtiError error = this->_jvmti_env->GetFieldDeclaringClass( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jfieldID>( field ),
        reinterpret_cast<jclass*>( &declaring_class ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false; 
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_field_offset( void* class_, void* field, std::int64_t& offset )
{
    /* Ensure field is valid */
    if( field == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get field offset */
    offset = reinterpret_cast<std::int64_t>( field ) << 2;
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_field_is_synthetic( void* class_, void* field, bool& is_synthetic )
{
    /* Ensure field is valid */
    if( field == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get field is synthetic */
    jvmtiError error = this->_jvmti_env->IsFieldSynthetic( 
        reinterpret_cast<jclass>( class_ ), 
        reinterpret_cast<jfieldID>( field ),
        reinterpret_cast<jboolean*>( &is_synthetic ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false; 
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_method_name( void* method, std::string& name, std::string& signature, std::string& generic_ptr )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    char* name_ptr = nullptr;
    char* signature_ptr = nullptr;
    char* generic_ptr_ptr = nullptr;

    /* Get method name */
    jvmtiError error = this->_jvmti_env->GetMethodName( reinterpret_cast<jmethodID>( method ), 
        &name_ptr,
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
    if( name_ptr != nullptr )
    {
        name = std::string( name_ptr );
    }
    else
    {
        name = "";
    }
    if( signature_ptr != nullptr )
    {
        signature = std::string( signature_ptr );
    }
    else
    {
        signature = "";
    }
    if( generic_ptr_ptr != nullptr )
    {
        generic_ptr = std::string( generic_ptr_ptr );
    }
    else
    {
        generic_ptr = "";
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( name_ptr ) );
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( signature_ptr ) );
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( generic_ptr_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_method_modifiers( void* method, std::int32_t& modifiers )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get method modifiers */
    jvmtiError error = this->_jvmti_env->GetMethodModifiers( 
        reinterpret_cast<jmethodID>( method ),
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

bool inspector::interfaces::JvmtiInterface::get_method_declaring_class( void* method, std::unique_ptr<inspector::types::JavaClass>& declaring_class )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jclass declaring_class_ptr = nullptr;
    
    /* Get method declaring class */
    jvmtiError error = this->_jvmti_env->GetMethodDeclaringClass( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jclass*>( &declaring_class_ptr ) 
    );

    declaring_class = std::make_unique<inspector::types::JavaClass>( declaring_class_ptr );

    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false; 
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_max_locals( void* method, std::int32_t& max_locals )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get max locals */
    jvmtiError error = this->_jvmti_env->GetMaxLocals( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jint*>( &max_locals ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_arguments_size( void* method, std::int32_t& arguments_size )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get arguments size */
    jvmtiError error = this->_jvmti_env->GetArgumentsSize( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jint*>( &arguments_size ) 
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_line_number_table( void* method, std::vector<std::pair<std::int32_t, std::int32_t>>& line_number_table )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint entry_count = 0;
    jvmtiLineNumberEntry* entry_ptr = nullptr;

    /* Get line number table */
    jvmtiError error = this->_jvmti_env->GetLineNumberTable( 
        reinterpret_cast<jmethodID>( method ),
        &entry_count,
        &entry_ptr
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    line_number_table.reserve( entry_count );
    for( auto i = 0; i < entry_count; i++ )
    {
        line_number_table.emplace_back( std::make_pair( (std::int32_t)entry_ptr[i].start_location, (std::int32_t)entry_ptr[i].line_number ) );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( entry_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_method_location( void* method, std::int32_t& start_location, std::int32_t& end_location )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get method location */
    jvmtiError error = this->_jvmti_env->GetMethodLocation( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jlocation*>( &start_location ),
        reinterpret_cast<jlocation*>( &end_location )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_local_variable_table( void* method, std::vector<void*>& local_variable_table )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint entry_count = 0;
    jvmtiLocalVariableEntry* entry_ptr = nullptr;

    /* Get local variable table */
    jvmtiError error = this->_jvmti_env->GetLocalVariableTable( 
        reinterpret_cast<jmethodID>( method ),
        &entry_count,
        &entry_ptr
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    local_variable_table.reserve( entry_count );
    for( auto i = 0; i < entry_count; i++ )
    {
        /* TODO */
        //local_variable_table.emplace_back( &entry_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( entry_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_bytecodes( void* method, std::vector<unsigned char>& bytecodes )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    jint bytecode_count = 0;
    unsigned char* bytecode_ptr = nullptr;

    /* Get bytecodes */
    jvmtiError error = this->_jvmti_env->GetBytecodes( 
        reinterpret_cast<jmethodID>( method ),
        &bytecode_count,
        &bytecode_ptr
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Reserve space on our vector */
    bytecodes.reserve( bytecode_count );
    for( auto i = 0; i < bytecode_count; i++ )
    {
        bytecodes.emplace_back( bytecode_ptr[i] );
    }

    /* Deallocate memory */
    this->_jvmti_env->Deallocate( reinterpret_cast<unsigned char*>( bytecode_ptr ) );

    return true;
}

bool inspector::interfaces::JvmtiInterface::get_method_is_native( void* method, bool& is_native )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get method is native */
    jvmtiError error = this->_jvmti_env->IsMethodNative( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jboolean*>( &is_native )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_method_is_synthetic( void* method, bool& is_synthetic )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get method is synthetic */
    jvmtiError error = this->_jvmti_env->IsMethodSynthetic( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jboolean*>( &is_synthetic )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::get_method_is_obsolete( void* method, bool& is_obsolete )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }
    
    /* Get method is obsolete */
    jvmtiError error = this->_jvmti_env->IsMethodObsolete( 
        reinterpret_cast<jmethodID>( method ),
        reinterpret_cast<jboolean*>( &is_obsolete )
    );
    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }
    
    return true;
}

bool inspector::interfaces::JvmtiInterface::set_method_breakpoint( void* method, std::int64_t location )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Set method breakpoint */
    jvmtiError error = this->_jvmti_env->SetBreakpoint( 
        reinterpret_cast<jmethodID>( method ),
        static_cast<jlocation>( location )
    );

    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false; 
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::clear_method_breakpoint( void* method, std::int64_t location )
{
    /* Ensure method is valid */
    if( method == nullptr )
    {
        this->set_last_error( JVMTI_ERROR_NULL_POINTER );
        return false;
    }

    /* Clear method breakpoint */
    jvmtiError error = this->_jvmti_env->ClearBreakpoint( 
        reinterpret_cast<jmethodID>( method ),
        static_cast<jlocation>( location )
    );

    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false; 
    }

    return true;
}

bool inspector::interfaces::JvmtiInterface::set_class_file_load_event( void* callback )
{
    /* Ensure callback is valid */
    if( callback == nullptr )
    {
        jvmtiError error = this->_jvmti_env->SetEventNotificationMode( JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr );
        this->set_last_error( error );
        return false;
    }
    /* Get current capabilities */
    jvmtiCapabilities capabilities;
    jvmtiError error = this->_jvmti_env->GetCapabilities( &capabilities );
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Set class file load event capability */
    capabilities.can_generate_all_class_hook_events = 1;
    error = this->_jvmti_env->AddCapabilities( &capabilities );
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Set class file load event */
    error = this->_jvmti_env->SetEventNotificationMode( JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr );
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false;
    }

    /* Set class file load event callback */
    this->_event_callbacks.ClassFileLoadHook = reinterpret_cast<jvmtiEventClassFileLoadHook>( callback );
    error = this->_jvmti_env->SetEventCallbacks( &this->_event_callbacks, sizeof( this->_event_callbacks ) );

    /* Error handling */
    if( error != JVMTI_ERROR_NONE )
    {
        this->set_last_error( error );
        return false; 
    }

    return true;
}