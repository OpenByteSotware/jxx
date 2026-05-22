using Microsoft.VisualStudio.Shell;
using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.Design;

internal sealed class CreateJxxClassCommand
{
    public static async Task InitializeAsync(AsyncPackage package)
    {
        await ThreadHelper.JoinableTaskFactory.SwitchToMainThreadAsync();

        var commandService = await package.GetServiceAsync(typeof(IMenuCommandService)) as OleMenuCommandService;

        var cmdId = new CommandID(Guid.Parse("A7D7B6D8-1111-2222-3333-444444444444"), 0x0100);
        var menuItem = new MenuCommand(Execute, cmdId);

        commandService.AddCommand(menuItem);
    }

    private static void Execute(object sender, EventArgs e)
    {
        ThreadHelper.ThrowIfNotOnUIThread();

        string dir = Environment.CurrentDirectory;
        string ns = "jxx::lang";

        var dlg = new CreateJxxClassDialog(ns);
        if (dlg.ShowDialog() != true)
            return;

        string name = dlg.ClassName;

        File.WriteAllText(name + ".h", GenerateClass(name, ns));
    }

    static string GenerateClass(string n, string ns) => $@"#pragma once

#include ""lang/jxx_types.h""
#include ""lang/jxx.lang.buildin_array.h""
#include ""lang/jxx.lang.Object.h""

namespace {ns} {{

class {n} : public jxx::lang::Object {{
public:
    using Ptr = jxx::Ptr<{n}>;

    static Ptr create();
}};

}}";
}