/*
 * JniCall.h
 *
 *  Created on: Sep 30, 2017
 *      Author: houcheng
 */

#ifndef JNICALL_H_
#define JNICALL_H_

#include <string>
#include "CFunctionCall.h"
using std::string;


class JniCall : public CFunctionCall {
public:
    JniCall();
    JniCall(const JSON& json);
    virtual JniCall* clone();

protected:
    string methodName = "";
    string sharedObjectPath = "";

public:
    void setMethodName(const string &methodName);
    void setSharedObjectPath(const string &sharedObjectPath);

public:
    virtual const string &getSharedObjectPath() const;
    virtual const string &getMethodName() const;

    virtual JSON toJson() const;
    const virtual string toJsonString() const;
};

#endif /* JNICALL_H_ */
