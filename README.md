Java to C++ converter and SDK

With STL and C++ 17 moving from java code base to C++ becomes easier
By implementing all java class hiearchy and interfaces (virtual classes in c++). 
With the help of python converter, moving to c++ from java is easy.

The idea is to make the interface and c++ class act exactly or close to java.
With the help of converter and macros. you can convert applications to run as c++.
Removing garbage collector and JIT.  the use of smart pointers is required as a pattern.
Otherwise memory leaks would ensue.

Prereq:
pip install javalang - used for Abstract tree anaylsis of java source
C++17 compiler with all the C++17 abilities

Open source libraries can be included but must be MIT compatible license so that proprietary applications can link statically
to jxx libraries.  See included open source libraries used.  3rdparty folder contains open source libraries used.
They must be c++17 compatible.  Cross platform and have a cmake file for cross complilation.
So if adding new capabilites to jxx, you will need to implement from ground up.  Cannot include boost
libraries.  The dependecy tree can become large and changes can have a ripple effect.  

WxWidgets is built outside of jxx because it is so large.  Rebuilding each time jxx changes takes a very long time.
See below on how to build wxWidgets for gui sdk part (awt and swing) for jxx.

the other open source packages are included in jxx projects because it may need to rebuilt them.  They are faster to build.

With that said sometimes it is too much work to build from ground up.  But packages needed are scruiized for 
license compatibility and amount of work to include.  Mainly use MIT license.


Pre-req:
- must be MIT compatibility license (for 3rdparty)
- must have a cmake to easily automate build for jxx
- must test via facade wrapper around jxx classes (jxx.<package>.<class> -> uses in private methods <library added>
- public interface must be jxx constraints (derived from Object - memeory deletion, use jxx::Ptr<type> for returns and const jxx::Ptr<type> for parameters)  

For windows:
Need to create an environment variable:   JXX_TIME_ZONE_DATA
Then use provided zoneinfo.tar.gz (from ubuntu linux timezone /usr/shared/zoneinfo)
This provides windows with timezone data.
Also create a file in JXX_TIME_ZONE_DATA/timezone
Set inside file the correct timezone.  Default is GMT
If using linux based systems timezone is already in /usr/shared/zoneinfo


There are two parts to this project:

1) Java to C++ converter (python)
2) C++ Java SDK - JXX (c++)

The JXX is a mimic (as close as possible) to java 8 SDK.
Thread class in Java is Thread class in c++ (namespace jxx::lang)
All non GUI classes are mapped and semantically the same as java.  So that when java code is converted it should run out of the box with no modifications.
There are some caviats like serialization and thread priority.  But if an application just runs with queues and threads, there should be no refactor of the C++ generated code.

The Java to C++ comverter
The python converter will take a single file or directory structure and convert every .java file to a .h and .cpp with same name
And can optionally generate a cmake file to build your new converted project.
Here are cmake options
Pass --cmake in directory or single-file pair mode and a CMakeLists.txt is created at --out:
Options

--cmake-project TranspiledProject
--cmake-target transpiled
--cmake-kind {static|shared|object|interface} (default static)
--cmake-cxx-standard 17 (string value)
--cmake-min 3.20 (CMake minimum version)

When developing additional SDK class / interfaces and methods please do the following:
Make one class per pull request
Make sure a unit test exists in the pull request for new classes
Make sure a benchmark exist and class is same overall time as java or better (if its poor performance, please fix before submitting)


# Note on pull request requirements:
1) Must include one class update / new with interfaces (if applicable)
2) Must include a googletest unit test for all methods and code coverage for the method/function
3) If an algorithm class, must include bench mark test

This will help with proving semantics.  If possible find the java 8 SDK unit test in java and use python converter to convert.  This really helps tests semantics



Other options are


# convert a folder and subfolder to c++ output mimic folder
# use NEW instead of JXX_NEW for similar to java new / c++ new
# include header guards (#ifndef)
python3 java_to_cpp.py --dir ./src_java --out ./cpp_out --cmake \
  --new-macro-style template --new-macro NEW \
  --header-guards \
  --string-include "String.h" \
  --bytearray-include "ByteArray.h" \
  --exceptions-include "Exception.h" \
  --sync-include "Sync.h" \
  --checked-exceptions macro --throws-macro JXX_THROWS





python3 java_to_cpp.py \
  --dir ./src_java \
  --out ./cpp_out \
  --cmake \
  --cmake-project MyTranspiled \
  --cmake-target mylib \
  --cmake-kind static \
  --cmake-cxx-standard 17 \
  --string-include "String.hpp" \
  --bytearray-include "ByteArray.hpp" \
  --exceptions-include "Exception.hpp" \
  --exceptions-include "IOException.hpp" \
  --sync-include "Sync.hpp" \
  --checked-exceptions macro --throws-macro JXX_THROWS
  
  
  
  
  python3 java2cpp17.py --dir ./src_java --out ./cpp_out \
  --primitive-map int=jint \
  --primitive-map boolean=jbool \
  --primitive-map short=jshort

python3 java_to_cpp.py MyFile.java --instanceof-style macro --downcast-style macro

python3 java_to_cpp.py --dir src_java --out outcpp --cmake



To build 3rd party packages
These 3rd party libraries have been modified to support C++17 at a minimum

top level directory do these commands
git submodule add --name libunicode https://github.com/OpenByteSotware/libunicode 3rdparty/libunicode
git submodule add --name libgsl https://github.com/OpenByteSotware/GSL 3rdparty/gsl
git submodule add --name libfmt https://github.com/OpenByteSotware/fmt 3rdparty/fmt
git submodule add --name libwxwidgets https://github.com/OpenByteSotware/wxWidgets 3rdparty/wxWidgets
git submodule add --name libpugixml https://github.com/OpenByteSotware/pugixml 3rdparty/pugixml
git submodule add --name libopenssl https://github.com/OpenByteSotware/openssl 3rdparty/openssl



Building openssl

Need Strawberry perl
nasm
VS console window building for x64
perl Configure VC-WIN64A --prefix=C:\<fullpath>\jxx\3rdparty\openssl --openssl dir=C:\<fullpath>\jxx\3rdparty\openssl\ssl



Array type usage from java int[], boolean[], int[][][]

auto bytes =
    jxx::NEW<
        jxx::lang::ByteArrayType>(
            static_cast<jxx::lang::jint>(16));
			
			using StringArrayType =
    jxx::lang::JxxArray<
        jxx::Ptr<jxx::lang::String>,
        1U>;

auto strings =
    jxx::NEW<StringArrayType>(
        static_cast<jxx::lang::jint>(10));
		
		auto matrix =
    jxx::NEW<
        jxx::lang::IntArray2DType>(
            2,
            3);
			
			
			auto cube =
    jxx::NEW<
        jxx::lang::IntArray3DType>(
            2,
            3,
            4);
			
			
			
			
# Java GUI vs JXX GUI

Interally uses wxWidgets for GUI applications
Wxwidgets is a cross platform GUI toolkit with active changes and is on github.

build wxwidgets first with 
C:\<jxxlocation>\3rdparty\wxWidgets>cmake -B build_wx -DwxUSE_STL=ON -DwxUSE_WEBVIEW=OFF

if you want static link
cmake -B build_wx -DwxUSE_STL=ON -DwxUSE_WEBVIEW=OFF -DwxBUILD_SHARED=OFF

cmake -S . -B build_wx -G "Visual Studio 18 2026" -A x64 -DwxUSE_STC=OFF -DwxUSE_STL=ON -DwxUSE_WEBVIEW=OFF -DwxBUILD_SHARED=OFF -DwxBUILD_MONOLITHIC=OFF -DwxBUILD_COMPATIBILITY=3.1 -DwxBUILD_SAMPLES=OFF -DwxBUILD_TESTS=OFF -DwxBUILD_DEMOS=OFF


if you don't want to use STC disable it, see wxWidget docs for details. 
-DwxUSE_STC=OFF

# java code
public class Main

{

public static void main(String[] args)

{

JFrame frame = new JFrame("Demo");

frame.setVisible(true);

}

}

#C++ jxx code for the same thing
int main()
{
    auto frame =
        jxx::NEW<jxx::swing::JFrame>(
            JXX_STR("Demo"));

    frame->setVisible(true);

    return jxx::swing::SwingUtilities::run();
}




#example
int main()
{
    auto frame =
        jxx::NEW<jxx::swing::JFrame>(
            JXX_STR("Hello"));

    frame->setSize(
        jxx::NEW<jxx::awt::Dimension>(
            800,
            600));

    frame->setVisible(true);

    return jxx::swing::SwingUtilities::run();
}




# Building OpenSSL 1.1.1w for the JXX SDK

This section documents the prerequisites and build steps for producing static OpenSSL 1.1.1w libraries for the JXX SDK on Windows x64 and Linux x64.

> **JXX integration layout**
>
> OpenSSL headers and libraries are stored under:
>
> ```text
> jxx/3rdparty/openssl/
> ```

## 1. Required output

The JXX Windows build uses the static OpenSSL libraries:

```text
jxx/3rdparty/openssl/libssl_static.lib
jxx/3rdparty/openssl/libcrypto_static.lib
```

The JXX Linux build uses:

```text
jxx/3rdparty/openssl/libssl.a
jxx/3rdparty/openssl/libcrypto.a
```

The public OpenSSL headers must be available under:

```text
jxx/3rdparty/openssl/include/openssl/
```

## 2. Windows x64 prerequisites

Install the following applications and Visual Studio components:

1. **Visual Studio 2026 or Visual Studio 2026 Build Tools**
   - Desktop development with C++
   - MSVC x64 compiler and libraries
   - Windows 10 or Windows 11 SDK
   - Windows SDK Resource Compiler (`rc.exe`)

2. **Native Windows Perl**
   - Strawberry Perl or another native Windows Perl distribution is recommended.
   - Do not use MSYS2 Perl for an MSVC build.
   - Verify that Perl reports a native Windows target rather than `x86_64-msys-thread-multi`:

   ```cmd
   perl -v
   ```

3. **NASM**
   - Required for the optimized x64 assembly implementation.

   ```cmd
   nasm -v
   ```

4. **OpenSSL 1.1.1w source archive**
   - Extract the archive to the intended build directory.

## 3. Initialize the Windows x64 build environment

Use an **x64 Native Tools Command Prompt**. Alternatively, initialize an existing Command Prompt with the Build Tools environment:

```cmd
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64
```

For a Professional installation, use its corresponding `VsDevCmd.bat` or `vcvarsall.bat` path.

Verify that the compiler targets x64:

```cmd
cl
```

The output must include:

```text
for x64
```

Verify the required tools:

```cmd
where cl
where nmake
where rc
where perl
where nasm
```

## 4. Windows SDK environment workaround

A correctly configured Visual Studio developer prompt should populate these paths automatically. If `stdlib.h`, `kernel32.lib`, or `rc.exe` cannot be found, add the installed Windows SDK paths explicitly.

The following example uses Windows SDK `10.0.26100.0`:

```cmd
set "WindowsSdkVersion=10.0.26100.0"
set "WindowsSdkRoot=C:\Program Files (x86)\Windows Kits\10"

set "INCLUDE=%WindowsSdkRoot%\Include\%WindowsSdkVersion%\ucrt;%WindowsSdkRoot%\Include\%WindowsSdkVersion%\shared;%WindowsSdkRoot%\Include\%WindowsSdkVersion%\um;%WindowsSdkRoot%\Include\%WindowsSdkVersion%\winrt;%INCLUDE%"

set "LIB=%WindowsSdkRoot%\Lib\%WindowsSdkVersion%\ucrt\x64;%WindowsSdkRoot%\Lib\%WindowsSdkVersion%\um\x64;%LIB%"

set "PATH=%WindowsSdkRoot%\bin\%WindowsSdkVersion%\x64;%PATH%"
```

Confirm that the files exist before using a different SDK version:

```cmd
dir "%WindowsSdkRoot%\Include\%WindowsSdkVersion%\ucrt\stdlib.h"
dir "%WindowsSdkRoot%\Lib\%WindowsSdkVersion%\um\x64\kernel32.lib"
where rc
```

### Compiler sanity check

Create a minimal C file:

```c
#include <stdlib.h>

int main(void)
{
    return EXIT_SUCCESS;
}
```

Build it:

```cmd
cl main.c
```

Do not continue until this command creates `main.exe` without a missing-header or missing-library error.

## 5. Configure OpenSSL for Windows x64

From the extracted OpenSSL 1.1.1w source directory, configure a static x64 MSVC build:

```cmd
perl Configure VC-WIN64A no-shared
```

To set explicit runtime directories, add `--prefix` and `--openssldir`. Avoid installing over an existing source tree unless that layout is intentional.

Example staging directory:

```cmd
perl Configure VC-WIN64A no-shared ^
  --prefix=C:\projects\SourceCode\git_repos\open_source\jxx\3rdparty\openssl-install ^
  --openssldir=C:\projects\SourceCode\git_repos\open_source\jxx\3rdparty\openssl-install\ssl
```

A successful configuration ends with:

```text
OpenSSL has been successfully configured
```

## 6. Build, test, and install on Windows

Build:

```cmd
nmake
```

Run the OpenSSL test suite:

```cmd
nmake test
```

Install into the configured prefix when a staging prefix was specified:

```cmd
nmake install
```

Expected build outputs include:

```text
libcrypto.lib
libcrypto_static.lib
libssl.lib
libssl_static.lib
```

For static JXX integration, use:

```text
libcrypto_static.lib
libssl_static.lib
```

The smaller `libcrypto.lib` and `libssl.lib` files are used with the DLL build and are not the selected JXX static libraries.

## 7. Verify the Windows libraries

Confirm that the generated libraries contain x64 objects:

```cmd
dumpbin /headers libssl_static.lib | findstr /i "machine x64 8664"
dumpbin /headers libcrypto_static.lib | findstr /i "machine x64 8664"
```

Copy the static libraries and headers into the JXX third-party directory if they were built in a separate staging directory:

```text
jxx/3rdparty/openssl/
├── include/
│   └── openssl/
├── libcrypto_static.lib
└── libssl_static.lib
```

## 8. Windows CMake integration

```cmake
set(JXX_OPENSSL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl")

target_include_directories(jxxsdk PRIVATE
    "${JXX_OPENSSL_ROOT}/include"
)

target_link_libraries(jxxsdk PRIVATE
    "${JXX_OPENSSL_ROOT}/libssl_static.lib"
    "${JXX_OPENSSL_ROOT}/libcrypto_static.lib"
    ws2_32
    crypt32
    bcrypt
    advapi32
)
```

Keep `libssl_static.lib` before `libcrypto_static.lib` in the link list because SSL depends on Crypto.

## 9. Linux x64 prerequisites

### Debian or Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y build-essential perl nasm
```

### RHEL, Rocky Linux, or AlmaLinux

```bash
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y perl nasm
```

Verify:

```bash
gcc --version
make --version
perl -v
nasm -v
```

## 10. Configure OpenSSL for Linux x64

From the extracted OpenSSL 1.1.1w source directory:

```bash
./config no-shared \
  --prefix=/opt/openssl-1.1.1w \
  --openssldir=/opt/openssl-1.1.1w/ssl
```

For a position-independent static library suitable for inclusion in a shared JXX library, use:

```bash
./config no-shared -fPIC \
  --prefix=/opt/openssl-1.1.1w \
  --openssldir=/opt/openssl-1.1.1w/ssl
```

## 11. Build, test, and install on Linux

```bash
make -j"$(nproc)"
make test
sudo make install
```

Expected static libraries:

```text
libssl.a
libcrypto.a
```

Copy the Linux libraries into the JXX third-party layout selected by the project, while retaining the shared OpenSSL headers under `jxx/3rdparty/openssl/include`.

## 12. Linux CMake integration

If the Linux static libraries are stored in a platform subdirectory:

```cmake
set(JXX_OPENSSL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl")

target_include_directories(jxxsdk PRIVATE
    "${JXX_OPENSSL_ROOT}/include"
)

target_link_libraries(jxxsdk PRIVATE
    "${JXX_OPENSSL_ROOT}/linux/libssl.a"
    "${JXX_OPENSSL_ROOT}/linux/libcrypto.a"
    dl
    pthread
)
```

## 13. Troubleshooting

### Perl reports `x86_64-msys-thread-multi`

Use a native Windows Perl distribution and ensure its `perl.exe` appears before MSYS2 in `PATH`:

```cmd
where perl
perl -v
```

### `stdlib.h` cannot be found

The Windows SDK UCRT include directory is missing from `INCLUDE`. Add:

```text
C:\Program Files (x86)\Windows Kits\10\Include\<SDK-version>\ucrt
```

Also include the SDK `shared`, `um`, and `winrt` directories.

### `kernel32.lib` cannot be found

The x64 Windows SDK library directories are missing from `LIB`. Add:

```text
C:\Program Files (x86)\Windows Kits\10\Lib\<SDK-version>\ucrt\x64
C:\Program Files (x86)\Windows Kits\10\Lib\<SDK-version>\um\x64
```

### `rc` is not recognized

Add the x64 Windows SDK binary directory to `PATH`:

```text
C:\Program Files (x86)\Windows Kits\10\bin\<SDK-version>\x64
```

Then verify:

```cmd
where rc
```

### Compiler reports `for x86`

The wrong Visual Studio environment is active. Reinitialize it for x64:

```cmd
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=amd64
```

Verify that `cl` reports `for x64` before configuring OpenSSL with `VC-WIN64A`.

## 14. Clean rebuild

When changing architecture or major configuration options, use a clean source tree whenever practical. Otherwise, clean the existing build before reconfiguration:

```cmd
nmake clean
```

Then run `perl Configure VC-WIN64A no-shared` again from the correctly initialized x64 environment.
