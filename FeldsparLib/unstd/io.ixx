export module unstd.io;

import unstd.core;

import <format>;
import <iostream>;
import <string>;

export template <typename Callable>
void for_each_line_in_process_stdout(const std::string& cmd, Callable&& f)
{
    char buffer[128] = {'\0'};

    std::string result;
    auto pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return;
    while (fgets(buffer, 128, pipe)) {
        result += buffer;
    }
    _pclose(pipe);

    if (result.empty()) {
        return;
    }

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = result.find("\n", prev)) != std::string::npos) {
        const std::string line = result.substr(prev, pos - prev);
        const bool keep_going = f(line);
        if (!keep_going) break;
        prev = pos + 1;
    }
}

export template <class... Args>
void to_stdout(std::string_view fmt, Args&& ...args)
{
    std::cout << std::vformat(fmt, std::make_format_args(args...));
}

export template <class... Args>
void to_stderr(std::string_view fmt, Args&& ...args)
{
    std::cerr << std::vformat(fmt, std::make_format_args(args...));
}
