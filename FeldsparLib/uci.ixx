export module uci;

class EngineUCI {
    virtual const char* const name() const = 0;
    virtual const char* const author() const = 0;
};
