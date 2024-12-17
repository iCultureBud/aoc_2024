#include <chrono>
#include <iostream>
#include <print>
#include <source_location>
#include <sstream>
#include <string_view>
#include <vector>

#define PROFILE(body)                                                          \
    do {                                                                       \
        auto const& loc = std::source_location::current();                     \
        auto const start = std::chrono::high_resolution_clock::now();          \
        body();                                                                \
        auto const end = std::chrono::high_resolution_clock::now();            \
        auto const duration =                                                  \
            std::chrono::duration_cast<std::chrono::microseconds>(end -        \
                                                                  start);      \
        std::puts(std::format("Time taken in {0}:{1}: {2} us",                 \
                              loc.function_name(),                             \
                              loc.line(),                                      \
                              duration.count())                                \
                      .c_str());                                               \
    } while (0)


std::vector<size_t> parse_nums(std::string_view const line, char delim) {
    std::vector<size_t> parts;
    parts.reserve(16);
    size_t start {};

    auto const len = line.size();
    while (start < len) {
        size_t end = line.find(delim, start);
        if (end == std::string_view::npos) {
            end = len;
        }

        size_t num {};
        std::from_chars(line.data() + start, line.data() + end, num);
        parts.push_back(num);

        start = end + 1;
    }

    return parts;
}

constexpr bool divmod(size_t dividend,
                      size_t divisor,
                      size_t& quotient,
                      size_t& remainder) {
    if (divisor == 0) {
        return false;
    }
    quotient = dividend / divisor;
    remainder = dividend % divisor;
    return true;
}

bool valid(size_t i,
           size_t testValue,
           std::vector<size_t> const& eq,
           bool isConcat) {
    auto const curr = eq[i];

    if (i == 0) {
        return curr == testValue;
    }

    size_t divValue {};
    size_t modValue {};

    if (divmod(testValue, curr, divValue, modValue) && modValue == 0 &&
        valid(i - 1, divValue, eq, isConcat)) {
        return true;
    }

    if (testValue > curr && valid(i - 1, testValue - curr, eq, isConcat)) {
        return true;
    }

    if (isConcat) {
        size_t multiplier {1};
        size_t temp {curr};
        while (temp > 0) {
            multiplier *= 10;
            temp /= 10;
        }

        if (divmod(testValue, multiplier, divValue, modValue) &&
            modValue == curr && valid(i - 1, divValue, eq, isConcat)) {
            return true;
        }
    }

    return false;
}

std::pair<size_t, size_t> solve(std::vector<std::string_view> const& input) {
    size_t part1Result = 0;
    size_t part2Result = 0;

    for (auto const line : input) {
        auto const colon {line.find(':')};
        auto const eq_view {line.substr(colon + 2)};
        auto const eq {parse_nums(eq_view, ' ')};
        auto const eq_len {eq.size()};

        size_t value = 0;
        std::from_chars(line.data(), line.data() + colon, value);

        if (valid(eq_len - 1, value, eq, false)) {
            part1Result += value;
        }

        if (valid(eq_len - 1, value, eq, true)) {
            part2Result += value;
        }
    }

    return {part1Result, part2Result};
}

void run(std::vector<std::string_view> const& input) {
    auto const [p1, p2] {solve(input)};
    std::println("Part 1: {0}\nPart 2: {1}", p1, p2);
}

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    PROFILE([&] {
        std::string buffer;
        buffer.reserve(36 * 1024 * 1024);

        buffer.assign(std::istreambuf_iterator<char>(std::cin),
                      std::istreambuf_iterator<char>());

        std::vector<std::string_view> input;

        size_t start = 0;
        for (size_t i = 0; i < buffer.size(); ++i) {
            if (buffer[i] == '\n' || buffer[i] == '\r') {
                input.emplace_back(buffer.data() + start, i - start);
                start = i + 1;
            }
        }
        if (start < buffer.size()) {
            input.emplace_back(buffer.data() + start, buffer.size() - start);
        }

        PROFILE([&] { run(input); });
    });

    return 0;
}
