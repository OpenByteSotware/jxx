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
    class Redirect final : public ClassBase<Redirect, Object> {
    public:
        enum class Type { PIPE, INHERIT, READ, WRITE, APPEND };
        static jxx::Ptr<Redirect> PIPE;
        static jxx::Ptr<Redirect> INHERIT;
        static jxx::Ptr<Redirect> from(const jxx::Ptr<jxx::io::File>& file);
        static jxx::Ptr<Redirect> to(const jxx::Ptr<jxx::io::File>& file);
        static jxx::Ptr<Redirect> appendTo(const jxx::Ptr<jxx::io::File>& file);
        Type type() const;
        jxx::Ptr<jxx::io::File> file() const;
        jbool equals(const jxx::Ptr<Object>& other) const override;
        jint hashCode() const override;
    private:
        Redirect(Type type, const jxx::Ptr<jxx::io::File>& file);
        Type type_;
        jxx::Ptr<jxx::io::File> file_;
    };

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


    jxx::Ptr<Redirect> redirectInput() const;
    jxx::Ptr<Redirect> redirectOutput() const;
    jxx::Ptr<Redirect> redirectError() const;
    jxx::Ptr<ProcessBuilder> redirectInput(const jxx::Ptr<Redirect>& source);
    jxx::Ptr<ProcessBuilder> redirectOutput(const jxx::Ptr<Redirect>& destination);
    jxx::Ptr<ProcessBuilder> redirectError(const jxx::Ptr<Redirect>& destination);
    jxx::Ptr<ProcessBuilder> redirectInput(const jxx::Ptr<jxx::io::File>& file);
    jxx::Ptr<ProcessBuilder> redirectOutput(const jxx::Ptr<jxx::io::File>& file);
    jxx::Ptr<ProcessBuilder> redirectError(const jxx::Ptr<jxx::io::File>& file);
    jxx::Ptr<ProcessBuilder> inheritIO();
    jbool redirectErrorStream() const;
    jxx::Ptr<ProcessBuilder> redirectErrorStream(jbool redirect);

    jxx::Ptr<Process> start();

private:
    std::vector<std::string> command_;
    std::unordered_map<std::string,std::string> environment_;
    jxx::Ptr<jxx::io::File> directory_;
    jbool redirectErrorStream_ = false;
    jxx::Ptr<Redirect> inputRedirect_;
    jxx::Ptr<Redirect> outputRedirect_;
    jxx::Ptr<Redirect> errorRedirect_;
};
} // namespace jxx::lang
