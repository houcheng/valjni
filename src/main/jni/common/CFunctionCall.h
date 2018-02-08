/*
 * JniCall.h
 *
 *  Created on: Sep 29, 2017
 *      Author: houcheng
 */

#ifndef CFUNCTIONCALL_H_
#define CFUNCTIONCALL_H_

#include <vector>
#include "json.hpp"

using JSON = nlohmann::json;
using std::vector;
using std::unique_ptr;
class JniValue;
class BasicJniValue;

class CFunctionCall {
public:
    CFunctionCall();
    CFunctionCall(const JSON& json);
    virtual ~CFunctionCall();

protected:
    unique_ptr<JniValue> returnValue;
    vector<unique_ptr<JniValue>> arguments;

public:
    JniValue* getReturnValue() const;
    const BasicJniValue* getArgument(int i) const ;
    vector<unique_ptr<JniValue>>& getArguments();

    void setReturnValue(JniValue* return_value);
    void pushArgument(JniValue* argument);
};

#endif /* CFUNCTIONCALL_H_ */
