#ifndef METHOD_HPP
#define METHOD_HPP

#include <memory>

#include "Curl.hpp"
#include "json.hpp"
#include "RedditUser.hpp"

namespace redd {

class MethodObserver;

namespace detail {
class Method {
    friend class ::redd::MethodObserver;
public:
    explicit Method() : curl(std::make_shared<detail::Curl>()), wkptr(curl) {}
    Method(Method& m);
    virtual ~Method() = 0;

protected:
    std::shared_ptr<detail::Curl> curl;
    std::weak_ptr<detail::Curl> wkptr;

    void setDependencyOn(const Method& method) &;
    void createNewHandle() &;

    void checkJsonErrors(const nlohmann::json& json) const;
    void strToJson(const std::string& src, nlohmann::json& json) const;
    inline void setToken(const RedditUser&);
};

inline void Method::setToken(const RedditUser& user) {
    if (!user.isComplete()) {
        throw RedditError("RedditUser must be complete");
    }
    curl->setHttpHeader("Authorization: bearer " + user.token());
}


}//! detail namespace


}//! redd namespace

#endif // METHOD_HPP
