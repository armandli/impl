#define REPEAT_PATTERN(pattern, repeats, ...) \
REPEAT_PATTERN_EXPAND ## repeats (pattern, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_0(pattern, ...)

#define REPEAT_PATTERN_EXPAND_1(pattern, ...) \
REPEAT_PATTERN_EXPAND_0(pattern, __VA_ARGS__) pattern(1, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_2(pattern, ...) \
REPEAT_PATTERN_EXPAND_1(pattern, __VA_ARGS__) pattern(2, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_3(pattern, ...) \
REPEAT_PATTERN_EXPAND_2(pattern, __VA_ARGS__) pattern(3, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_4(pattern, ...) \
REPEAT_PATTERN_EXPAND_3(pattern, __VA_ARGS__) pattern(4, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_5(pattern, ...) \
REPEAT_PATTERN_EXPAND_4(pattern, __VA_ARGS__) pattern(5, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_6(pattern, ...) \
REPEAT_PATTERN_EXPAND_5(pattern, __VA_ARGS__) pattern(6, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_7(pattern, ...) \
REPEAT_PATTERN_EXPAND_6(pattern, __VA_ARGS__) pattern(7, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_8(pattern, ...) \
REPEAT_PATTERN_EXPAND_7(pattern, __VA_ARGS__) pattern(8, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_9(pattern, ...) \
REPEAT_PATTERN_EXPAND_8(pattern, __VA_ARGS__) pattern(9, __VA_ARGS__)

#define REPEAT_PATTERN_EXPAND_10(pattern, ...) \
REPEAT_PATTERN_EXPAND_9(pattern, __VA_ARGS__) pattern(10, __VA_ARGS__)
