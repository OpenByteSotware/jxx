#pragma once

#include <string>
#include <vector>

namespace jxx::net::internal
{
    struct ParsedUrl
    {
        std::string original;
        std::string scheme;
        std::string authority;
        std::string userInfo;
        std::string host;
        int port = -1;
        std::string path;
        std::string query;
        std::string fragment;
        std::string schemeSpecificPart;
        bool opaque = false;
    };

    ParsedUrl parseUriLike(const std::string& spec);
    std::string rebuildUri(const ParsedUrl& p);
    std::string normalizePath(const std::string& path);
    std::string resolvePath(const std::string& basePath,
                            const std::string& childPath);
    std::string percentEncodeForm(const std::string& input);
    std::string percentDecodeForm(const std::string& input);
    int defaultPortForScheme(const std::string& scheme);
    std::string extensionToMime(const std::string& name);
}
