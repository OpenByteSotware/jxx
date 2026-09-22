#include "com/google/gson/jxx.com.google.gson.FieldNamingPolicy.h"
#include <cctype>
namespace com::google::gson { namespace {
class PolicyStrategy final : public ::jxx::lang::Object, public FieldNamingStrategy {
public:
    explicit PolicyStrategy(FieldNamingPolicy policy) : policy_(policy) {}
    ::jxx::Ptr<::jxx::lang::String> translateName(
        const ::jxx::Ptr<::jxx::lang::String>& fieldName) override {
        if (fieldName == nullptr) return nullptr;
        auto input = fieldName->utf8();
        if (input.empty() || policy_ == FieldNamingPolicy::IDENTITY) return fieldName;
        if (policy_ == FieldNamingPolicy::UPPER_CAMEL_CASE ||
            policy_ == FieldNamingPolicy::UPPER_CAMEL_CASE_WITH_SPACES) {
            std::string output;
            for (std::size_t index = 0; index < input.size(); ++index) {
                const auto current = input[index];
                if (policy_ == FieldNamingPolicy::UPPER_CAMEL_CASE_WITH_SPACES &&
                    index != 0U && std::isupper(static_cast<unsigned char>(current))) output.push_back(' ');
                output.push_back(index == 0U
                    ? static_cast<char>(std::toupper(static_cast<unsigned char>(current)))
                    : current);
            }
            return ::jxx::NEW<::jxx::lang::String>(output);
        }
        char separator = '_';
        if (policy_ == FieldNamingPolicy::LOWER_CASE_WITH_DASHES) separator = '-';
        else if (policy_ == FieldNamingPolicy::LOWER_CASE_WITH_DOTS) separator = '.';
        std::string output;
        for (std::size_t index = 0; index < input.size(); ++index) {
            const auto current = static_cast<unsigned char>(input[index]);
            if (index != 0U && std::isupper(current)) output.push_back(separator);
            output.push_back(static_cast<char>(std::tolower(current)));
        }
        return ::jxx::NEW<::jxx::lang::String>(output);
    }
private:
    FieldNamingPolicy policy_;
};
}
::jxx::Ptr<FieldNamingStrategy> FieldNamingPolicies::strategy(FieldNamingPolicy policy) {
    return std::make_shared<PolicyStrategy>(policy);
}
} // namespace com::google::gson
