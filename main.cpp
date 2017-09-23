#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "src/json.hpp"
#include "src/Reddit.hpp"
using namespace redd;

int main() {
    RedditUser user("aosdasd","123321","5eg8ASWEt25FuQ","8jDMZngnDkzz0K51KJ2D2c2XvqQ");
    RedditSimpleClient client;
    try {
        user.setToken(client.requestToken(user));
    } catch(RedditError& err) {
        std::cout << err << std::endl;
    }
    RedditAccount me;
    std::cout << user.token();
    RedditAccount::Friends n = me.friends(user);
    for (auto& g : n.people) {
        std::cout << g.name << std::endl;
    }
    return 0;
}
