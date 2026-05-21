"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");
function isValidClassName(name) {
    return /^[A-Z][A-Za-z0-9]*$/.test(name);
}
function isValidInterfaceName(name) {
    return /^I[A-Z][A-Za-z0-9]*$/.test(name);
}
function generateClassHeader(name, ns) {
    return `#pragma once

#include "jxx/lang/Object.h"

namespace ${ns} {

class ${name} : public jxx::lang::Object {
public:
    ${name}();
    ~${name}() override;

    ${name}(const ${name}&) = delete;
    ${name}& operator=(const ${name}&) = delete;
};

} // namespace ${ns}
`;
}
function generateCpp(name, ns) {
    return `#include "${name}.h"

namespace ${ns} {

${name}::${name}() = default;
${name}::~${name}() = default;

} // namespace ${ns}
`;
}
function generateInterfaceHeader(name, ns) {
    return `#pragma once

#include "jxx/lang/Object.h"

namespace ${ns} {

class ${name} : public jxx::lang::Object {
public:
    virtual ~${name}() = default;

    virtual void example() = 0;
};

} // namespace ${ns}
`;
}
function activate(context) {
    const disposable = vscode.commands.registerCommand("jxx.createClass", async () => {
        const type = await vscode.window.showQuickPick(["Class", "Interface"], { placeHolder: "Select type" });
        if (!type)
            return;
        const name = await vscode.window.showInputBox({ prompt: "Name" });
        if (!name)
            return;
        if (type === "Class" && !isValidClassName(name)) {
            vscode.window.showErrorMessage("Class must be PascalCase");
            return;
        }
        if (type === "Interface" && !isValidInterfaceName(name)) {
            vscode.window.showErrorMessage("Interface must start with I (e.g. IFoo)");
            return;
        }
        const ns = await vscode.window.showInputBox({ prompt: "Namespace", value: "jxx::lang" });
        if (!ns)
            return;
        const folder = vscode.workspace.workspaceFolders?.[0];
        if (!folder)
            return;
        const hPath = path.join(folder.uri.fsPath, `${name}.h`);
        const cppPath = path.join(folder.uri.fsPath, `${name}.cpp`);
        if (type === "Class") {
            fs.writeFileSync(hPath, generateClassHeader(name, ns));
            fs.writeFileSync(cppPath, generateCpp(name, ns));
        }
        else {
            fs.writeFileSync(hPath, generateInterfaceHeader(name, ns));
        }
        vscode.window.showInformationMessage(`${type} ${name} created`);
    });
    context.subscriptions.push(disposable);
}
function deactivate() { }
