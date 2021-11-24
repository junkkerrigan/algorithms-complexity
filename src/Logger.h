#ifndef ALGORITHMS_COMPLEXITY_LOGGER_H
#define ALGORITHMS_COMPLEXITY_LOGGER_H

class Logger {
public:
    static void debug(const std::any& value);
    static void debug(const std::any& value1, const std::any& value2);
    static void debug(const std::any& value1, const std::any& value2, const std::any& value3);

    static void info(const std::any& value);
    static void info(const std::any& value1, const std::any& value2);
    static void info(const std::any& value1, const std::any& value2, const std::any& value3);

private:
    static void print(const std::any& value);
};

#endif //ALGORITHMS_COMPLEXITY_LOGGER_H
