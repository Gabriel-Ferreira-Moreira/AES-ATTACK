#ifndef TEST_H
#define TEST_H

#include <iomanip>      // <<<< adicionado
#include "gtest/gtest.h"

typedef enum { OUTPUT_FORMAT_DEC, OUTPUT_FORMAT_HEX } output_format_t;

template <class T1, class T2>
::testing::AssertionResult ArraysMatch(const T1 *expected, const T2 *actual, size_t array_len,
                                       output_format_t output_fmt = OUTPUT_FORMAT_DEC) {
    for (size_t i(0); i < array_len; ++i) {
        if (expected[i] != actual[i]) {
            ::testing::Message msg;
            size_t width1 = 2 * sizeof(T1), width2 = 2 * sizeof(T2);

            switch (output_fmt) {
                case OUTPUT_FORMAT_HEX:
                    msg << "expected[" << i << "] (0x" << std::setw(width1) << std::setfill('0') << std::hex
                        << +expected[i] << ") != actual[" << std::dec << std::setw(0) << i << "] (0x"
                        << std::setw(width2) << std::setfill('0') << std::hex << +actual[i] << ")";
                    break;

                case OUTPUT_FORMAT_DEC:
                    msg << "expected[" << i << "] (" << +expected[i] << ") != actual[" << i << "] (" << +actual[i]
                        << ")";
                    break;

                default:
                    msg << "Invalid output format for printing mismatched elements";
                    break;
            }

            return ::testing::AssertionFailure() << msg;
        }
    }

    return ::testing::AssertionSuccess();
}

template <class T1, class T2, size_t array_len>
::testing::AssertionResult ArraysMatch(const T1 (&expected)[array_len], const T2 (&actual)[array_len],
                                       output_format_t output_fmt = OUTPUT_FORMAT_DEC) {
    return ArraysMatch(expected, actual, array_len, output_fmt);
}

#endif
