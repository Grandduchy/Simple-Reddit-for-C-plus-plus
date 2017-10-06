#include <memory>
#include <string>
#include <functional>
#include <utility>

#include "json.hpp"

#include "MethodListing.hpp"
#include "HelperFunctions.hpp"
#include "RedditUrl.hpp"

namespace redd {

MethodListing::MethodListing(const detail::Method& m) : extra_inputs(std::make_unique<Inputs>()) {
    setDependencyOn(m);
}


MethodListing::Hot MethodListing::hot(const RedditUser& user, const std::string& s) {
    curl->curlErrors();
    if (!user.isComplete()) {
        throw RedditError("RedditUser must be complete");
    }
    Hot hot;
    curl->setHttpHeader("Authorization: bearer " + user.token());
    RedditUrl url("https://oauth.reddit.com/r/" + s + "/hot");
    std::string query_strings = inputsToString();
    if (!query_strings.empty()) {
        url.addQueryString(query_strings);
        url.removeQueryString("t");
    }
    std::string unparsed = curl->simpleGet(url.url());
    nlohmann::json json = nlohmann::json::parse(unparsed);

    using detail::setIfNotNull;
    if (json.find("data") != json.end()) {
        setIfNotNull(hot.after, json["data"], "after", "");
        setIfNotNull(hot.before, json["data"], "before", "");
        setIfNotNull(hot.modhash, json["data"], "modhash", "");
        if (json["data"].find("children") != json["data"].end()) {
            for (auto it = json["data"]["children"].begin(); it != json["data"]["children"].end(); it++) {
                hot.links.push_back(parseLinkT3((*it)["data"]));
            }
        }
    }
    hot.links.shrink_to_fit();
    return hot;
}

MethodListing::New MethodListing::_new(const RedditUser& user, const std::string& s) {
    curl->emptyErrors();
    if (!user.isComplete()) {
        throw RedditError("RedditUser must be complete");
    }
    New local_new;
    curl->setHttpHeader("Authorization: bearer " + user.token());
    RedditUrl url("https://oauth.reddit.com/r/" + s + "/new");
    std::string query_strings = inputsToString();
    if (!query_strings.empty()) {
        url.addQueryString(query_strings);
        url.removeQueryString("t");
    }
    std::string unparsed = curl->simpleGet(url.url());
    nlohmann::json json = nlohmann::json::parse(unparsed);

    using detail::setIfNotNull;
    if (json.find("data") != json.end()) {
        setIfNotNull(local_new.after, json["data"], "after", "");
        setIfNotNull(local_new.before, json["data"], "before", "");
        setIfNotNull(local_new.modhash, json["data"], "modhash", "");
        if (json["data"].find("children") != json["data"].end()) {
            for (auto it = json["data"]["children"].begin(); it != json["data"]["children"].end(); it++) {
                local_new.links.push_back(parseLinkT3((*it)["data"]));
            }
        }
    }
    local_new.links.shrink_to_fit();
    return local_new;
}

MethodListing::Random MethodListing::random(const RedditUser& user, const std::string& s) {
    curl->emptyErrors();
    if (!user.isComplete()) {
        throw RedditError("RedditUser must be complete");
    }
    Random random;
    curl->setHttpHeader("Authorization: bearer " + user.token());
    std::string url("https://oauth.reddit.com/r/" + s + "/random");
    std::string unparsed = curl->simpleGet(url);
    nlohmann::json json = nlohmann::json::parse(unparsed);
    // object comes in form of an array always with size 2.
    if (json.size() != 2) {
        throw RedditError("An error has occured in parsing /api/random endpoint.");
    }

    if (json[0]["data"].find("children") != json[0]["data"].end()) {
        auto& t3 = json[0]["data"]["children"];
        if (t3.size() != 1) {// there is always only one t3 object.
            throw RedditError("An error has occured parsing, no T3 object found.");
        }
        else {
            random.link = parseLinkT3(t3[0]);
        }
    }

    if (json[1]["data"].find("children") != json[1]["data"].end()) {
        for (auto& t1_it : json[1]["data"]["children"]) {
            random.comments.push_back(parseCommentT1(t1_it));
        }
    }

    return random;
}

MethodListing::Top MethodListing::top(const RedditUser &user , const std::string &s) {
    curl->curlErrors();
    if (!user.isComplete()) {
        throw RedditError("RedditUser must be complete");
    }
    Top top;
    curl->setHttpHeader("Authorization: bearer " + user.token());
    RedditUrl url("https://oauth.reddit.com/r/" + s + "/top");
    std::string query_strings = inputsToString();
    if (!query_strings.empty()) {
        url.addQueryString(query_strings);
    }
    std::string unparsed = curl->simpleGet(url.url());
    nlohmann::json json = nlohmann::json::parse(unparsed);

    using detail::setIfNotNull;
    if (json.find("data") != json.end()) {
        setIfNotNull(top.after, json["data"], "after", "");
        setIfNotNull(top.before, json["data"], "before", "");
        setIfNotNull(top.modhash, json["data"], "modhash", "");
        if (json["data"].find("children") != json["data"].end()) {
            for (auto it = json["data"]["children"].begin(); it != json["data"]["children"].end(); it++) {
                top.links.push_back(parseLinkT3((*it)["data"]));
            }
        }
    }
    top.links.shrink_to_fit();
    return top;
}

MethodListing::Controversial MethodListing::controversial(const RedditUser& user, const std::string& s) {
    curl->curlErrors();
    if (!user.isComplete()) {
        throw RedditError("RedditUser must be complete");
    }

    Controversial contro;
    curl->setHttpHeader("Authorization: bearer " + user.token());
    RedditUrl url("https://oauth.reddit.com/r/" + s + "/controversial");
    std::string query_strings = inputsToString();
    if (!query_strings.empty()) {
        url.addQueryString(query_strings);
    }
    std::string unparsed = curl->simpleGet(url.url());
    nlohmann::json json = nlohmann::json::parse(unparsed);

    using detail::setIfNotNull;
    if (json.find("data") != json.end()) {
        setIfNotNull(contro.after, json["data"], "after", "");
        setIfNotNull(contro.before, json["data"], "before", "");
        setIfNotNull(contro.modhash, json["data"], "modhash", "");
        if (json["data"].find("children") != json["data"].end()) {
            for (auto it = json["data"]["children"].begin(); it != json["data"]["children"].end(); it++) {
                contro.links.push_back(parseLinkT3((*it)["data"]));
            }
        }
    }
    contro.links.shrink_to_fit();
    return contro;
}

std::string MethodListing::inputsToString() const {
    std::string str_inputs;
    if (!extra_inputs->after.empty())
        str_inputs.append("after=" + extra_inputs->after + "&");
    if (!extra_inputs->before.empty())
        str_inputs.append("before=" + extra_inputs->before + "&");
    if (!extra_inputs->g.empty())
        str_inputs.append("g=" + extra_inputs->g + "&");
    if (!extra_inputs->t.empty())
        str_inputs.append("t=" + extra_inputs->t + "&");
    if (!extra_inputs->show.empty())
        str_inputs.append("show=" + extra_inputs->show + "&");
    if (extra_inputs->count > 0)
        str_inputs.append("count=" + std::to_string(extra_inputs->count) + "&");
    if (extra_inputs->limit > 0)
        str_inputs.append("limit=" + std::to_string(extra_inputs->limit) + "&");
    if (extra_inputs->sr_detail)
        str_inputs.append("sr_detail=true");

    auto iter = std::find(str_inputs.crbegin(), str_inputs.crend(), '&').base();
    if (str_inputs.end() - iter >= 3) {
        //there is a leading &
        str_inputs.erase(iter);
    }

    return str_inputs;
}

void MethodListing::setInputs(const Inputs& inputs) {
    *extra_inputs = inputs;
}

MethodListing::Inputs& MethodListing::inputs() {
    return *extra_inputs;
}

void MethodListing::resetInputs() {
    *extra_inputs = Inputs();
}



MethodListing::Comment MethodListing::parseCommentT1(const nlohmann::json& json_obj) const {
    Comment comment;
    using detail::setIfNotNull;

    setIfNotNull(comment.author, json_obj, "author", "");
    setIfNotNull(comment.banned_by, json_obj, "banned_by", "");
    setIfNotNull(comment.body, json_obj, "body", "");
    setIfNotNull(comment.id, json_obj, "id", "");
    setIfNotNull(comment.link_id, json_obj, "link_id", "");
    setIfNotNull(comment.name, json_obj, "name", "");
    setIfNotNull(comment.parent_id, json_obj, "parent_id", "");
    setIfNotNull(comment.subreddit, json_obj, "subreddit", "");
    setIfNotNull(comment.subreddit_id, json_obj, "subreddit_id", "");
    setIfNotNull(comment.subreddit_name_prefixed, json_obj, "subreddit_name_prefixed", "");
    setIfNotNull(comment.subreddit_type, json_obj, "subreddit_type", "");

    setIfNotNull(comment.banned_at_utc, json_obj, "banned_at_utc", static_cast<long long>(-1));
    setIfNotNull(comment.created, json_obj, "created", static_cast<long long>(-1));
    setIfNotNull(comment.created_utc, json_obj, "created_utc", static_cast<long long>(-1));
    setIfNotNull(comment.edited, json_obj, "edited", static_cast<long long>(-1));
    auto iter = json_obj.find("edited");
    if (iter != json_obj.end()) {
        if (iter->is_number() && !iter->is_null()) {
            setIfNotNull(comment.edited, json_obj, "edited", static_cast<long long>(-1));
        }
    }

    setIfNotNull(comment.downs, json_obj, "downs", -1);
    setIfNotNull(comment.gilded, json_obj, "gilded", -1);
    setIfNotNull(comment.likes, json_obj, "likes", -1);
    setIfNotNull(comment.num_reports, json_obj, "num_reports", -1);
    setIfNotNull(comment.score, json_obj, "score", -1);
    setIfNotNull(comment.ups, json_obj, "ups", -1);

    setIfNotNull(comment.archived, json_obj, "archived", false);
    setIfNotNull(comment.can_gild, json_obj, "can_gild", false);
    setIfNotNull(comment.can_mod_post, json_obj, "can_mod_post", false);
    setIfNotNull(comment.collapsed, json_obj, "collapsed", false);
    setIfNotNull(comment.edited, json_obj, "edited", false);
    setIfNotNull(comment.is_sumbitter, json_obj, "is_sumbitter", false);
    setIfNotNull(comment.saved, json_obj, "saved", false);
    setIfNotNull(comment.stickied, json_obj, "stickied", false);

    if (json_obj.find("replies")!= json_obj.end()) {
        if (json_obj["replies"].find("data") != json_obj["replies"].end()) {
            auto child_array = json_obj["replies"]["data"].find("children");
            if (child_array != json_obj["replies"]["data"].end()) {
                for (auto& child : *child_array) {
                    comment.children.push_back(parseCommentT1(child));
                }
            }
        }
    }

    return comment;
}


MethodListing::Link MethodListing::parseLinkT3(const nlohmann::json& json_obj) const {
    MethodListing::Link link;
    using detail::setIfNotNull;

    setIfNotNull(link.author, json_obj, "author", "");
    setIfNotNull(link.banned_by, json_obj, "banned_by", "");
    setIfNotNull(link.domain, json_obj, "domain", "");
    setIfNotNull(link.id, json_obj, "id", "");
    setIfNotNull(link.link_flair_text, json_obj, "link_flair_text", "");
    setIfNotNull(link.name, json_obj, "name", "");
    setIfNotNull(link.permalink, json_obj, "permalink", "");
    setIfNotNull(link.selftext, json_obj, "selftext", "");
    setIfNotNull(link.selftext_html, json_obj, "selftext_html", "");
    setIfNotNull(link.subreddit, json_obj, "subreddit", "");
    setIfNotNull(link.subreddit_id, json_obj, "subreddit_id", "");
    setIfNotNull(link.subreddit_name_prefixed, json_obj, "subreddit_name_prefixed", "");
    setIfNotNull(link.subreddit_type, json_obj, "subreddit_type", "");
    setIfNotNull(link.thumbnail, json_obj, "thumbnail", "");
    setIfNotNull(link.title, json_obj, "title", "");
    setIfNotNull(link.url, json_obj, "url", "");
    setIfNotNull(link.whitelist_status, json_obj, "whitelist_status", "");

    setIfNotNull(link.approved_at_utc, json_obj, "approved_at_utc", static_cast<long long>(-1));
    setIfNotNull(link.banned_at_utc, json_obj, "banned_at_utc", static_cast<long long>(-1));
    setIfNotNull(link.created_utc, json_obj, "created_utc", static_cast<long long>(-1));

    /* In the api, if it hasn't been edited it returns the value 'false' and not null for the key 'edited'
     * We must first check if the type is a number to set it to the variable.
     */
    auto iter = json_obj.find("edited");
    if (iter != json_obj.end()) {
        if (iter->is_number() && !iter->is_null()) {
            setIfNotNull(link.edited, json_obj, "edited", static_cast<long long>(-1));
        }
    }



    setIfNotNull(link.downs, json_obj, "downs", -1);
    setIfNotNull(link.gilded, json_obj, "gilded", -1);
    setIfNotNull(link.likes, json_obj, "likes", -1);
    setIfNotNull(link.num_comments, json_obj, "num_comments", -1);
    setIfNotNull(link.num_crossposts, json_obj, "num_crossposts", -1);
    setIfNotNull(link.report_reasons, json_obj, "report_reasons", -1);
    setIfNotNull(link.score, json_obj, "score", -1);
    setIfNotNull(link.ups, json_obj, "ups", -1);
    setIfNotNull(link.view_count, json_obj, "view_count", -1);

    setIfNotNull(link.archived, json_obj, "archived", false);
    setIfNotNull(link.can_gild, json_obj, "can_gild", false);
    setIfNotNull(link.can_mod_post, json_obj, "can_mod_post", false);
    setIfNotNull(link.clicked, json_obj, "clicked", false);
    setIfNotNull(link.hidden, json_obj, "hidden", false);
    setIfNotNull(link.hide_score, json_obj, "hide_score", false);
    setIfNotNull(link.is_crosspostable, json_obj, "is_crosspostable", false);
    setIfNotNull(link.is_self, json_obj, "is_self", false);
    setIfNotNull(link.is_video, json_obj, "is_video", false);
    setIfNotNull(link.locked, json_obj, "locked", false);
    setIfNotNull(link.over_18, json_obj, "over_18", false);
    setIfNotNull(link.spoiler, json_obj, "spoiler", false);
    setIfNotNull(link.stickied, json_obj, "stickied", false);
    setIfNotNull(link.visted, json_obj, "visted", false);

    return link;
}




} //! redd namsepace
