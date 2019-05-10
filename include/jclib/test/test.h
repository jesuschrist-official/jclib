/*
 * jclib/test/test.h - jclib test framework
 */
#ifndef _jclib_test_test_h_
#define _jclib_test_test_h_


#include <assert.h>
#include <stdio.h>
#include <string.h>


#define JCTEST_MAX  512
#define JCTEST_NAME_MAX  80


#define JCTEST(_name) \
    int main(int argc, char** argv) { \
        printf("Tests '%s'\n", _name); \
        size_t _jc_ntests = 0; \
        char _jc_test_names[JCTEST_MAX][JCTEST_NAME_MAX]; \
        int _jc_test_results[JCTEST_MAX]; \
        int _jc_test_switch = 0; \


#define JCTEST_FUNC(_name) \
    printf("\texecuting test '%s'...\n", _name); \
    assert(_jc_ntests < JCTEST_MAX); \
    strncpy(_jc_test_names[_jc_ntests++], _name, JCTEST_NAME_MAX); \
    _jc_test_names[_jc_ntests - 1][JCTEST_NAME_MAX - 1] = '\0'; \
    _jc_test_results[_jc_ntests - 1] = 1; \
    while (!(_jc_test_switch = (_jc_test_switch + 1) % 2))


#define SET_TEST_FAILURE \
    _jc_test_results[_jc_ntests - 1] = 0; \
    break;


#define JCTEST_END \
    printf("report:\n"); \
    size_t _jc_nsuccess = 0; \
    for (size_t i = 0; i < _jc_ntests; i++) { \
        _jc_nsuccess += _jc_test_results[i]; \
        printf("\t%-40s\t%s\n", \
               _jc_test_names[i], (_jc_test_results[i]) ? "ok" : "failure"); \
    } \
    float _jc_percent = (_jc_nsuccess / (float)_jc_ntests) * 100.0f; \
    printf("result: %zu/%zu (%.2f%%)\n", \
           _jc_nsuccess, _jc_ntests, _jc_percent); \
    return (_jc_nsuccess == _jc_ntests) ? 0 : 1; \
}


#define JCTEST_ASSERT(_expr) \
    if (!(_expr)) { \
        printf("\tassert failure line %d: '" # _expr "'\n", __LINE__); \
        SET_TEST_FAILURE \
    }


#endif
