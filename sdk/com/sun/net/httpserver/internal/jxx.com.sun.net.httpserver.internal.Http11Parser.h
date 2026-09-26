#pragma once
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
namespace jxx::com::sun::net::httpserver::internal { struct ParsedRequest { std::string method,target,version; std::vector<std::pair<std::string,std::string>> headers; std::vector<unsigned char> body; bool keepAlive=false; }; class Http11Parser final { public: enum class Result{NeedMore,Complete,Error}; Result parse(const unsigned char*data,std::size_t size,ParsedRequest&out,std::size_t&consumed,std::string&error)const; private: static bool parseContentLength(const std::vector<std::pair<std::string,std::string>>&h,std::size_t&value);}; }
