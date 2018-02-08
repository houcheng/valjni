/*
 * JniException.h
 *
 * The JniException depends on modified backward.hpp (BFD enabled) and backward.cpp.
 * The system should install the binutils-dev and the linker should add -lbfd.
 */

#ifndef SRC_LIB_JNIEXCEPTION_H_
#define SRC_LIB_JNIEXCEPTION_H_

#include <typeinfo>
#include <stdexcept>
#include <execinfo.h>
#include <string>
#include "backward.hpp"

#include <string>
#include <iostream>
#include <sstream>

class JniException : public std::exception {
public:
    JniException() _GLIBCXX_USE_NOEXCEPT  {
        st.load_here(32);
    }

    // Have to convert and store, as the what is constant and can not call any non-const methods.
    JniException(std::string message1): JniException() {
        std::ostringstream os;
        backward::Printer p;

        p.print(st, os);
        message = message1 + "\n" + os.str();
    }
    void print(FILE* fp = stderr) _GLIBCXX_USE_NOEXCEPT {
        backward::Printer p;
        p.print(st, fp);
    }

    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override {
        return message.c_str();
    };

private:
    backward::StackTrace st;
    std::string message;
};

class JsonErrorException: public JniException { };

#endif /* SRC_LIB_JNIEXCEPTION_H_ */
