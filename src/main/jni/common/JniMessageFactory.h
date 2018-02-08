#ifndef JNIMESSAGEBUILDER_H_
#define JNIMESSAGEBUILDER_H_

#include "json.hpp"
using JSON = nlohmann::json;
#include "JniMessage.h"
#include <memory>
using std::unique_ptr;

class JniMessageFactory {
public:
    unique_ptr<JniMessage> createMessageFromJson(const JSON& json);
};

#endif /* JNIMESSAGEBUILDER_H_ */
