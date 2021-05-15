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
