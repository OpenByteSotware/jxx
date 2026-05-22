using Microsoft.VisualStudio.Shell;
using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

[PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
[InstalledProductRegistration("JXX Generator", "Generate JXX classes", "1.0")]
[ProvideMenuResource("Menus.ctmenu", 1)]
[Guid("C7616DD2-9C1B-4106-BF48-03ECD7527C62")]
public sealed class JxxPackage : AsyncPackage
{
    protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
    {
        await CreateJxxClassCommand.InitializeAsync(this);
    }
}
