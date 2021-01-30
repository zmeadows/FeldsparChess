export module unstd.io;

import unstd.core;

import<cstdio>;
import<memory>;
import<string>;

export template <typename Callable>
void for_each_line_in_process_stdout(const std::string& cmd, Callable&& f)
{
    char buffer[128] = {'\0'};

    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) return;
    while (fgets(buffer, 128, pipe.get())) {
        result += buffer;
    }
    if (result.empty()) return;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = result.find("\n", prev)) != std::string::npos) {
        const std::string line = result.substr(prev, pos - prev);
        const bool keep_going = f(line);
        if (!keep_going) break;
        prev = pos + 1;
    }

    // U64 capacity = 256;
    // char buffer[256];
    // char* line = alloc_buffer<char>(capacity);

    // if (buffer == nullptr) {
    //     return false;
    // }

    // FILE* cmd_output = fopen(cmd.cstr(), "r");
    // if (cmd_output == nullptr) {
    //     return false;
    // }

    // while (fgets(buffer, capacity, output) != nullptr) {
    //     if (line_buffer[capacity - 1] == '\n') {
    //         line_buffer[capacity - 1] = '\0';
    //         if (capacity > std::numeric_limits::max<U64>::max() / 2) {
    //             return false;
    //         }
    //         capacity *= 2;
    //         buffer = realloc_buffer<char>(buffer, capacity);
    //     }

    //     f(StringRef(line_buffer));
    // }
}
