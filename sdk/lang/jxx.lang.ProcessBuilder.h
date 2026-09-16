#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::io { class File; }
namespace jxx::lang {
class Process;

class ProcessBuilder final : public ClassBase<ProcessBuilder, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<ProcessBuilder, JxxSuper>;

    explicit ProcessBuilder(
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& command);

    jxx::Ptr<ProcessBuilder> command(
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& command);
    jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> command() const;

    jxx::Ptr<jxx::io::File> directory() const;
    jxx::Ptr<ProcessBuilder> directory(const jxx::Ptr<jxx::io::File>& directory);

    jxx::Ptr<ProcessBuilder> environment(
        const jxx::Ptr<String>& name,
        const jxx::Ptr<String>& value);
    jxx::Ptr<String> environment(const jxx::Ptr<String>& name) const;
    jxx::Ptr<ProcessBuilder> clearEnvironment();

    jbool redirectErrorStream() const;
    jxx::Ptr<ProcessBuilder> redirectErrorStream(jbool redirect);

    jxx::Ptr<Process> start();

private:
    std::vector<std::string> command_;
    std::unordered_map<std::string,std::string> environment_;
    jxx::Ptr<jxx::io::File> directory_;
    jbool redirectErrorStream_ = false;
};
} // namespace jxx::lang
