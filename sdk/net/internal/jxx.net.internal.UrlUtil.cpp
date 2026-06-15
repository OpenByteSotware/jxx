#include "net/internal/jxx.net.internal.UrlUtil.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    inline std::string lower_(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return s;
    }

    inline bool isSchemeChar_(char c)
    {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '+' || c == '-' || c == '.';
    }

    inline void splitAuthority_(const std::string& authority,
                                std::string& userInfo,
                                std::string& host,
                                int& port)
    {
        userInfo.clear();
        host.clear();
        port = -1;

        std::string core = authority;
        const auto at = core.find('@');
        if (at != std::string::npos)
        {
            userInfo = core.substr(0, at);
            core = core.substr(at + 1);
        }

        if (!core.empty() && core.front() == '[')
        {
            const auto close = core.find(']');
            if (close != std::string::npos)
            {
                host = core.substr(0, close + 1);
                if (close + 1 < core.size() && core[close + 1] == ':')
                    port = std::stoi(core.substr(close + 2));
                return;
            }
        }

        const auto colon = core.rfind(':');
        if (colon != std::string::npos && core.find(':') == colon)
        {
            host = core.substr(0, colon);
            const auto p = core.substr(colon + 1);
            if (!p.empty())
                port = std::stoi(p);
        }
        else
        {
            host = core;
        }
    }

    inline std::string hexByte_(unsigned char c)
    {
        static const char* kHex = "0123456789ABCDEF";
        std::string out;
        out.push_back('%');
        out.push_back(kHex[(c >> 4) & 0x0F]);
        out.push_back(kHex[c & 0x0F]);
        return out;
    }
}

namespace jxx::net::internal
{
    ParsedUrl parseUriLike(const std::string& spec)
    {
        ParsedUrl p;
        p.original = spec;

        std::string rest = spec;

        const auto hash = rest.find('#');
        if (hash != std::string::npos)
        {
            p.fragment = rest.substr(hash + 1);
            rest = rest.substr(0, hash);
        }

        const auto colon = rest.find(':');
        if (colon != std::string::npos)
        {
            bool schemeLike = colon > 0;
            for (std::size_t i = 0; i < colon && schemeLike; ++i)
                schemeLike = isSchemeChar_(rest[i]);
            if (schemeLike)
            {
                p.scheme = lower_(rest.substr(0, colon));
                std::string after = rest.substr(colon + 1);
                p.schemeSpecificPart = after;

                if (after.rfind("//", 0) == 0)
                {
                    after = after.substr(2);
                    const auto slash = after.find_first_of("/?");
                    p.authority = (slash == std::string::npos) ? after : after.substr(0, slash);
                    splitAuthority_(p.authority, p.userInfo, p.host, p.port);
                    std::string tail = (slash == std::string::npos) ? std::string() : after.substr(slash);
                    const auto q = tail.find('?');
                    if (q != std::string::npos)
                    {
                        p.path = tail.substr(0, q);
                        p.query = tail.substr(q + 1);
                    }
                    else
                    {
                        p.path = tail;
                    }
                }
                else
                {
                    p.opaque = !after.empty() && after[0] != '/';
                    if (p.opaque)
                    {
                        p.schemeSpecificPart = after;
                    }
                    else
                    {
                        const auto q = after.find('?');
                        if (q != std::string::npos)
                        {
                            p.path = after.substr(0, q);
                            p.query = after.substr(q + 1);
                        }
                        else
                        {
                            p.path = after;
                        }
                    }
                }
                return p;
            }
        }

        const auto q = rest.find('?');
        if (q != std::string::npos)
        {
            p.path = rest.substr(0, q);
            p.query = rest.substr(q + 1);
        }
        else
        {
            p.path = rest;
        }
        p.schemeSpecificPart = p.path + (p.query.empty() ? std::string() : std::string("?") + p.query);
        return p;
    }

    std::string rebuildUri(const ParsedUrl& p)
    {
        std::ostringstream out;
        if (!p.scheme.empty())
            out << p.scheme << ':';
        if (p.opaque)
        {
            out << p.schemeSpecificPart;
        }
        else
        {
            if (!p.authority.empty())
                out << "//" << p.authority;
            out << p.path;
            if (!p.query.empty())
                out << '?' << p.query;
        }
        if (!p.fragment.empty())
            out << '#' << p.fragment;
        return out.str();
    }

    std::string normalizePath(const std::string& path)
    {
        if (path.empty())
            return path;

        const bool abs = path.front() == '/' || (path.size() > 2 && std::isalpha(static_cast<unsigned char>(path[0])) && path[1] == ':' && (path[2] == '/' || path[2] == '\\'));
        std::vector<std::string> parts;
        std::string current;
        for (char c : path)
        {
            const char sep = (c == '\\') ? '/' : c;
            if (sep == '/')
            {
                if (!current.empty())
                {
                    if (current == ".")
                    {
                    }
                    else if (current == "..")
                    {
                        if (!parts.empty() && parts.back() != "..")
                            parts.pop_back();
                        else if (!abs)
                            parts.push_back(current);
                    }
                    else
                    {
                        parts.push_back(current);
                    }
                    current.clear();
                }
            }
            else
            {
                current.push_back(sep);
            }
        }
        if (!current.empty())
        {
            if (current == "..")
            {
                if (!parts.empty() && parts.back() != "..")
                    parts.pop_back();
                else if (!abs)
                    parts.push_back(current);
            }
            else if (current != ".")
            {
                parts.push_back(current);
            }
        }

        std::ostringstream out;
        if (abs)
            out << '/';
        for (std::size_t i = 0; i < parts.size(); ++i)
        {
            if (i > 0)
                out << '/';
            out << parts[i];
        }
        const auto s = out.str();
        return s.empty() ? (abs ? std::string("/") : std::string()) : s;
    }

    std::string resolvePath(const std::string& basePath,
                            const std::string& childPath)
    {
        if (childPath.empty())
            return normalizePath(basePath);
        if (!childPath.empty() && (childPath.front() == '/' || childPath.front() == '\\'))
            return normalizePath(childPath);

        const auto slash = basePath.find_last_of("/\\");
        const std::string prefix = (slash == std::string::npos) ? std::string() : basePath.substr(0, slash + 1);
        return normalizePath(prefix + childPath);
    }

    std::string percentEncodeForm(const std::string& input)
    {
        std::ostringstream out;
        for (unsigned char c : input)
        {
            if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '*')
                out << static_cast<char>(c);
            else if (c == ' ')
                out << '+';
            else
                out << hexByte_(c);
        }
        return out.str();
    }

    std::string percentDecodeForm(const std::string& input)
    {
        std::ostringstream out;
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            const char c = input[i];
            if (c == '+')
            {
                out << ' ';
            }
            else if (c == '%' && i + 2 < input.size())
            {
                const auto hex = input.substr(i + 1, 2);
                const auto value = static_cast<char>(std::stoi(hex, nullptr, 16));
                out << value;
                i += 2;
            }
            else
            {
                out << c;
            }
        }
        return out.str();
    }

    int defaultPortForScheme(const std::string& scheme)
    {
        const auto s = lower_(scheme);
        if (s == "http")
            return 80;
        if (s == "https")
            return 443;
        if (s == "ftp")
            return 21;
        return -1;
    }

    std::string extensionToMime(const std::string& name)
    {
        const auto dot = name.find_last_of('.');
        const std::string ext = (dot == std::string::npos) ? std::string() : lower_(name.substr(dot + 1));
        if (ext == "txt" || ext == "log" || ext == "ini" || ext == "cfg") return "text/plain";
        if (ext == "htm" || ext == "html") return "text/html";
        if (ext == "css") return "text/css";
        if (ext == "js") return "application/javascript";
        if (ext == "json") return "application/json";
        if (ext == "xml") return "application/xml";
        if (ext == "csv") return "text/csv";
        if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
        if (ext == "png") return "image/png";
        if (ext == "gif") return "image/gif";
        if (ext == "bmp") return "image/bmp";
        if (ext == "svg") return "image/svg+xml";
        if (ext == "pdf") return "application/pdf";
        if (ext == "zip") return "application/zip";
        if (ext == "jar") return "application/java-archive";
        return "application/octet-stream";
    }
}
