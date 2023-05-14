#pragma once
#include <string>
#include <vector>

namespace inspector::types
{
    class JavaThread
    {
    private:
        void* _thread;
    public:
        static bool get_current_thread( JavaThread*& thread );
        static bool get_all_threads( std::vector<JavaThread*>& threads );
    public:
        bool get_thread_state( std::int32_t& state );
        bool suspend( );
        bool resume( );
        bool stop( void* exception );
        bool interrupt( );
        bool get_thread_name( std::string& name );
        bool get_thread_priority( std::int32_t& priority );
        bool is_daemon( bool& daemon );
        bool get_thread_group( void*& group );
        bool get_context_class_loader( void*& class_loader );
    };
}