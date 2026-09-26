#pragma once
#include <atomic><memory><mutex><string><thread><vector>
#include "net/internal/jxx.net.internal.NetPlatform.h"
namespace jxx::com::sun::net::httpserver::internal { class Http11Engine final { public: using Handler=void(*)(const std::string&,std::string&,void*); Http11Engine();~Http11Engine();void bind(const std::string&address,unsigned short port,int backlog);void start(Handler handler,void*state);void stop(); private:void acceptLoop();void serve(::jxx::net::internal::NativeSocket socket);::jxx::net::internal::NativeSocket listener_=::jxx::net::internal::invalidSocket();std::atomic<bool>running_{false};std::thread acceptThread_;Handler handler_=nullptr;void*state_=nullptr;std::mutex threadsMutex_;std::vector<std::thread>threads_;}; }
