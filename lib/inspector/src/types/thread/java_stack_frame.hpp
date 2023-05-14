#pragma once
#include <string>
#include <vector>

namespace inspector::types
{
    class JavaStackFrame
    {
    private:
        void* _thread;
    public:
        static bool get_all_stack_traces( std::int32_t max_frame_count, std::vector<JavaStackFrame>& frames );
    public:
        bool get_stack_trace( std::int32_t start_depth, std::int32_t max_frame_count, std::vector<JavaStackFrame>& frames );
        bool get_frame_count( std::int32_t& count );
        bool get_frame_location( std::int32_t depth, void*& method, std::int32_t& locations );
    };
}
