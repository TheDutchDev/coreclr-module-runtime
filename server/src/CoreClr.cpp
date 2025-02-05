//
// Created by parallels on 1/29/19.
//

#include "CoreClr.h"
#include <semver.h>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <sstream>

#include "c-api/func_table.h"

std::mutex mtx; // mutex for critical section
std::condition_variable cv; // condition variable for critical section
CoreClrDelegate_t hostResourceExecute;
CoreClrDelegate_t hostResourceExecuteUnload;
CoreClrDelegate_t hostStopRuntime;

void CoreClr_SetResourceLoadDelegates(CoreClrDelegate_t resourceExecute, CoreClrDelegate_t resourceExecuteUnload,
                                      CoreClrDelegate_t stopRuntime)
{
    std::unique_lock<std::mutex> lck(mtx);
    hostResourceExecute = resourceExecute;
    hostResourceExecuteUnload = resourceExecuteUnload;
    hostStopRuntime = stopRuntime;
    cv.notify_all();
}

CoreClr::CoreClr(alt::ICore* core)
{
    this->core = core;
    /*_initializeCoreCLR = nullptr;
    _shutdownCoreCLR = nullptr;
    _createDelegate = nullptr;
    _executeAssembly = nullptr;*/

    char_t buffer[256]; //MAX_PATH
    size_t buffer_size = sizeof(buffer) / sizeof(char_t);
    int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
    if (rc != 0)
    {
        core->LogError(cs::Log::LOG_PREFIX, "invalid get_hostfxr_path " + std::to_string(rc));
    }
    else
    {
#ifdef _WIN32
        std::wstring bufferWString(buffer);
        _coreClrLib = LoadLibraryEx(std::string(bufferWString.begin(), bufferWString.end()).c_str(), nullptr, 0);
        if (_coreClrLib == nullptr)
        {
            core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to find CoreCLR dll"));
            return;
        }

        _initializeFxr = (hostfxr_initialize_for_runtime_config_fn)GetProcAddress(
            _coreClrLib, "hostfxr_initialize_for_runtime_config");
        _getDelegate = (hostfxr_get_runtime_delegate_fn)GetProcAddress(_coreClrLib, "hostfxr_get_runtime_delegate");
        _runApp = (hostfxr_run_app_fn)GetProcAddress(_coreClrLib, "hostfxr_run_app");
        _initForCmd = (hostfxr_initialize_for_dotnet_command_line_fn)GetProcAddress(
            _coreClrLib, "hostfxr_initialize_for_dotnet_command_line");
        _closeFxr = (hostfxr_close_fn)GetProcAddress(_coreClrLib, "hostfxr_close");
#else
    _coreClrLib = dlopen(std::string(buffer).c_str(), RTLD_NOW | RTLD_LOCAL);
    if (_coreClrLib == nullptr) {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to find CoreCLR dll [") + std::string(buffer) + "]: " + dlerror());
        return;
    }
    _initializeFxr = (hostfxr_initialize_for_runtime_config_fn) dlsym(_coreClrLib,
                                                                      "hostfxr_initialize_for_runtime_config");
    _getDelegate = (hostfxr_get_runtime_delegate_fn) dlsym(_coreClrLib, "hostfxr_get_runtime_delegate");
    _runApp = (hostfxr_run_app_fn) dlsym(_coreClrLib, "hostfxr_run_app");
    _initForCmd = (hostfxr_initialize_for_dotnet_command_line_fn) dlsym(_coreClrLib,
                                                                        "hostfxr_initialize_for_dotnet_command_line");
    _closeFxr = (hostfxr_close_fn) dlsym(_coreClrLib, "hostfxr_close");
#endif
        if (_initializeFxr == nullptr || _getDelegate == nullptr || _closeFxr == nullptr || _runApp == nullptr ||
            _initForCmd == nullptr)
        {
            core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to find CoreCLR dll methods"));
            return;
        }
        return;
    }

    // Load hostfxr and get desired exports
#ifdef _WIN32
    char pf[MAX_PATH];
    SHGetSpecialFolderPath(
        nullptr,
        pf,
        CSIDL_PROGRAM_FILES,
        FALSE);

    const char* windowsProgramFilesPath = "/dotnet/host/fxr/";
    auto defaultPath = new char[strlen(windowsProgramFilesPath) + strlen(pf) + 1];
    strcpy(defaultPath, pf);
    strcat(defaultPath, windowsProgramFilesPath);

    const char* dotnetProgramFilesPath = "/dotnet/";
    dotnetDirectory = new char[strlen(dotnetProgramFilesPath) + strlen(pf) + 1];
    strcpy(dotnetDirectory, pf);
    strcat(dotnetDirectory, dotnetProgramFilesPath);

    GetPath(core, defaultPath);
    delete[] defaultPath;
#else
    auto currDotnetDirectory = "/usr/share/dotnet/";
    dotnetDirectory = new char[strlen(currDotnetDirectory) + 1];
    strcpy(dotnetDirectory, currDotnetDirectory);
    GetPath(core, "/usr/share/dotnet/host/fxr/");
#endif
#ifdef _WIN32
    const char* fileName = "/hostfxr.dll";

    auto fullPath = new char[strlen(fileName) + strlen(runtimeDirectory) + 1];
    strcpy(fullPath, runtimeDirectory);
    strcat(fullPath, fileName);

    _coreClrLib = LoadLibraryEx(fullPath, nullptr, 0);
    delete[] fullPath;
    if (_coreClrLib == nullptr)
    {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to find CoreCLR dll"));
        return;
    }

    _initializeFxr = (hostfxr_initialize_for_runtime_config_fn)GetProcAddress(
        _coreClrLib, "hostfxr_initialize_for_runtime_config");
    _getDelegate = (hostfxr_get_runtime_delegate_fn)GetProcAddress(_coreClrLib, "hostfxr_get_runtime_delegate");
    _runApp = (hostfxr_run_app_fn)GetProcAddress(_coreClrLib, "hostfxr_run_app");
    _initForCmd = (hostfxr_initialize_for_dotnet_command_line_fn)GetProcAddress(
        _coreClrLib, "hostfxr_initialize_for_dotnet_command_line");
    _closeFxr = (hostfxr_close_fn)GetProcAddress(_coreClrLib, "hostfxr_close");
#else
    const char* fileName = "/libhostfxr.so";
    char fullPath[strlen(fileName) + strlen(runtimeDirectory) + 1];
    strcpy(fullPath, runtimeDirectory);
    strcat(fullPath, fileName);
    _coreClrLib = dlopen(fullPath, RTLD_NOW | RTLD_LOCAL);
    if (_coreClrLib == nullptr) {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to find CoreCLR dll [") + fullPath + "]: " + dlerror());
        return;
    }
    _initializeFxr = (hostfxr_initialize_for_runtime_config_fn) dlsym(_coreClrLib,
                                                                      "hostfxr_initialize_for_runtime_config");
    _getDelegate = (hostfxr_get_runtime_delegate_fn) dlsym(_coreClrLib, "hostfxr_get_runtime_delegate");
    _runApp = (hostfxr_run_app_fn) dlsym(_coreClrLib, "hostfxr_run_app");
    _initForCmd = (hostfxr_initialize_for_dotnet_command_line_fn) dlsym(_coreClrLib,
                                                                        "hostfxr_initialize_for_dotnet_command_line");
    _closeFxr = (hostfxr_close_fn) dlsym(_coreClrLib, "hostfxr_close");
#endif
    if (_initializeFxr == nullptr || _getDelegate == nullptr || _closeFxr == nullptr || _runApp == nullptr ||
        _initForCmd == nullptr)
    {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to find CoreCLR dll methods"));
        return;
    }
}

CoreClr::~CoreClr()
{
    if (hostStopRuntime != nullptr)
    {
        hostStopRuntime(nullptr, 0);
    }
    if (thread != nullptr)
    {
        thread->join();
        delete thread;
    }
    delete[] runtimeDirectory;
    delete[] dotnetDirectory;
    delete version;
    if (cxt != nullptr)
    {
        _closeFxr(cxt);
    }
}

/*bool CoreClr::GetDelegate(alt::ICore* core, void* runtimeHost, unsigned int domainId, const char* moduleName,
                          const char* classPath, const char* methodName, void** callback) {
    if (runtimeHost == nullptr || domainId == 0) {
        server->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Core CLR host not loaded"));
        return false;
    }
    int result = _createDelegate(runtimeHost, domainId, moduleName, classPath, methodName, callback);
    if (result < 0) {
        if (this->PrintError(server, result)) {
            return false;
        }
        server->LogInfo(cs::Log::LOG_PREFIX,
                std::string("coreclr-module: Unable to get ") + moduleName + ":" + classPath + "." + methodName +
                " domain:" +
                domainId);
        return false;
    }
    return true;
}*/

//TODO: don't include own dll or exe ect that is in the directory
/*alt::Array<std::string> CoreClr::getTrustedAssemblies(alt::ICore* core, const char* appPath) {
    alt::Array<std::string> assemblies;
    const char* const tpaExtensions[] = {".ni.dll", ".dll", ".ni.exe", ".exe", ".winmd"};

    const char* directories[] = {runtimeDirectory//, appPath// };

    for (auto path : directories) {
        auto directory = opendir(path);
        if (directory == nullptr) {
            server->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Runtime directory not found"));
            return assemblies;
        }
        server->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Runtime directory found"));
        struct dirent* entry;
        struct stat sb{};
        for (auto ext : tpaExtensions) {
            size_t extLength = strlen(ext);
            while ((entry = readdir(directory)) != nullptr) {
                switch (entry->d_type) {
                    case DT_REG:
                        break;

                        // Handle symlinks and file systems that do not support d_type
#ifndef _WIN32
                    case DT_LNK:
#endif
                    case DT_UNKNOWN: {
                        std::string fullFilename;

                        fullFilename.append(path);
                        fullFilename.append(PATH_SEPARATOR);
                        fullFilename.append(entry->d_name);

                        if (stat(fullFilename.c_str(), &sb) == -1) {
                            continue;
                        }

                        if (!S_ISREG(sb.st_mode)) {
                            continue;
                        }
                    }
                        break;

                    default:
                        continue;
                }

                // Check if the extension matches the one we are looking for
                size_t extPos = strlen(entry->d_name) - extLength;
                //server->LogInfo(cs::Log::LOG_PREFIX, std::string(ext) + "," + extLength + "," + entry->d_name);
                if (extPos <= 0 || memcmp(ext, entry->d_name + extPos, extLength) != 0) {
                    continue;
                }

                //std::string filenameWithoutExt(filename.substr(0, extPos));

                // Ensure assemblies are unique in the list
                //if (assemblies.find(filenameWithoutExt) != assemblies.end()) {//TODO: use alt::Set which doesnt exists
                //    continue;
                //}

                if (strcmp(entry->d_name, "AltV.Net.dll") == 0) {
                    continue;
                }
                assemblies.Push(
                        std::string(path) + PATH_SEPARATOR + entry->d_name);
            }
            // rewind directory to search for next extension
            rewinddir(directory);
        }
#ifndef _WIN32
        closedir(directory);
#endif
    }
    return assemblies;
}*/

//TODO: use APP_PATHS via path from main assembly because all assemblies are most likely in same path
/*void CoreClr::CreateAppDomain(alt::ICore* core, alt::IResource* resource, const char* appPath, void** runtimeHost,
                              unsigned int* domainId, bool executable, uint64_t resourceIndex, const char* domainName) {
    std::string tpaList = "";

    //TODO: check if useless list separator at the end is fine
    for (auto &tpa : getTrustedAssemblies(server, appPath)) {
        tpaList = tpaList + tpa;
        tpaList = tpaList + LIST_SEPARATOR;
    }

    auto nativeDllPaths = std::string(appPath) + LIST_SEPARATOR + runtimeDirectory;

    const char* propertyKeys[] = {
            "TRUSTED_PLATFORM_ASSEMBLIES",
            "APP_PATHS",
            "APP_NI_PATHS",
            "NATIVE_DLL_SEARCH_DIRECTORIES",
            "System.GC.Server",
            "System.Globalization.Invariant"};

    const char* propertyValues[] = {
            tpaList.c_str(),
            appPath,
            appPath,
            nativeDllPaths.c_str(),
            "true",
            "true"};

    int result = _initializeCoreCLR(
            appPath,
            domainName,
            sizeof(propertyKeys) / sizeof(char*),
            propertyKeys,
            propertyValues,
            runtimeHost,
            domainId);

    if (result < 0) {
        server->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Unable to create app domain: 0x"));
        this->PrintError(server, result);
    } else {
        server->LogInfo(std::string("coreclr-module: Created app domain: 0x") + appPath);
    }

    if (executable) {
        auto executablePath = std::string(appPath) + PATH_SEPARATOR + resource->GetMain();

        server->LogInfo(std::string("coreclr-module: Prepare for executing assembly:") + executablePath);
        int exitCode = -1;
        const char* args[1];
        char resourceIndexChar = resourceIndex + '0';
        char resourceIndexString[1];
        resourceIndexString[0] = resourceIndexChar;
        args[0] = resourceIndexString;
        result = _executeAssembly(
                *runtimeHost,
                *domainId,
                1,
                args,
                executablePath.c_str(),
                (unsigned int*) &exitCode
        );

        if (result < 0) {
            exitCode = -1;
            server->LogInfo(
                    std::string("coreclr-module: Unable to execute assembly in app path:") + executablePath);
            this->PrintError(server, result);
        } else {
            server->LogInfo(std::string("coreclr-module: Assembly executed"));
        }
    }
}*/

/*int CoreClr::Execute(alt::ICore* core, alt::IResource* resource, const char* appPath, uint64_t resourceIndex,
                     void** runtimeHost,
                     const unsigned int* domainId) {
    auto executablePath = std::string(appPath) + PATH_SEPARATOR + resource->GetMain();
    server->LogInfo(std::string("coreclr-module: Prepare for executing assembly:") + executablePath);
    int exitCode = -1;
    const char* args[1];
    char resourceIndexChar = resourceIndex + '0';
    char resourceIndexString[1];
    resourceIndexString[0] = resourceIndexChar;
    args[0] = resourceIndexString;
    int result = _executeAssembly(
            *runtimeHost,
            *domainId,
            1,
            args,
            executablePath.c_str(),
            (unsigned int*) &exitCode
    );

    if (result < 0) {
        exitCode = -1;
        server->LogInfo(
                std::string("coreclr-module: Unable to execute assembly in app path:") + executablePath);
        this->PrintError(server, result);
    } else {
        server->LogInfo(std::string("coreclr-module: Assembly executed"));
    }
    return result;
}*/

/*void CoreClr::Shutdown(alt::ICore* core, void* runtimeHost,
                       unsigned int domainId) {
    if (cxt != nullptr) {
        _closeFxr(cxt);
    }
    if (_shutdownCoreCLR == nullptr) return;
    int latchedExitCode = 0;
    int result = _shutdownCoreCLR(runtimeHost, domainId, &latchedExitCode);
    if (result < 0) {
        server->LogInfo(std::string("coreclr-module: Unable to shutdown host"));
    } else {
        server->LogInfo(std::string("coreclr-module: Host successfully shotted down"));
    }
}*/

void CoreClr::GetPath(alt::ICore* core, const char* defaultPath)
{
    auto directory = opendir(defaultPath);
    if (directory == nullptr)
    {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: dotnet core sdk not found in ") + defaultPath);
        return;
    }
    struct dirent* entry;
    char* greatest = nullptr;
    semver_t greatest_version = {};
    semver_t compare_version = {};
    while ((entry = readdir(directory)) != nullptr)
    {
        if (entry->d_type == DT_DIR && memcmp(entry->d_name, ".", 1) != 0 && memcmp(entry->d_name, "..", 2) != 0)
        {
            core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: version found: ") + entry->d_name);
            if (greatest == nullptr)
            {
                if (semver_parse(entry->d_name, &greatest_version))
                {
                    core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: invalid version found: ") + entry->d_name);
                    continue;
                }
                greatest = entry->d_name;
                continue;
            }
            if (semver_parse(entry->d_name, &compare_version))
            {
                core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: invalid version found: ") + entry->d_name);
                continue;
            }
            if (semver_compare(compare_version, greatest_version) > 0)
            {
                semver_free(&greatest_version);
                greatest_version = compare_version;
                greatest = entry->d_name;
            }
            else
            {
                semver_free(&compare_version);
            }
            /*auto compareCache = new char[strlen(entry->d_name)];
            strcpy(compareCache, entry->d_name);
            auto compareCache2 = new char[strlen(greatest)];
            strcpy(compareCache2, greatest);
            if (tail_lt(compareCache, compareCache2)) {
                greatest = entry->d_name;
            }
            delete[] compareCache;
            delete[] compareCache2;*/
        }
    }
    if (greatest == nullptr)
    {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: No dotnet sdk version found"));
        return;
    }
    else
    {
        semver_free(&greatest_version);
    }
    core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: greatest version: ") + greatest);
    size_t size = strlen(defaultPath) + strlen(greatest) + 1;
    runtimeDirectory = (char*)malloc(size);
    memset(runtimeDirectory, '\0', size);
    strcpy(runtimeDirectory, defaultPath);
    strcat(runtimeDirectory, greatest);
    auto greatestVersionStrLen = strlen(greatest);
    char* currVersion = (char*)malloc(greatestVersionStrLen + 1);
    memcpy(currVersion, greatest, greatestVersionStrLen);
    currVersion[greatestVersionStrLen] = '\0';
    this->version = currVersion;
}

//TODO: https://github.com/rashiph/DecompliedDotNetLibraries/blob/6056fc6ff7ae8fb3057c936d9ebf36da73f990a6/mscorlib/System/__HResults.cs
/*bool CoreClr::PrintError(alt::ICore* core, int errorCode) {
    if (errorCode == -2146234304) {
        server->LogInfo(
                std::string(
                        "coreclr-module: Your server needs to be compiled and needs to target (<TargetFramework>netcoreappX.X</TargetFramework>) with the same .net core version that is installed on your workstation"));
        return true;
    } else if (errorCode == -2147024894) {
        server->LogInfo(
                std::string(
                        "coreclr-module: You need to place the AltV.Net.dll in your resource directory. Use publish to generate all dlls."));
        return true;
    }
    server->LogInfo(std::string(strerror(errno)));
    char* x_str = new char[10];
    sprintf(x_str, "%d", errorCode);
    server->LogInfo(
            std::string(x_str));
    delete[] x_str;
    return false;
}*/

/*#ifdef _WIN32
const char_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    char_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}
#endif*/

struct thread_user_data
{
    hostfxr_run_app_fn runApp;
    hostfxr_close_fn closeFxr;
    hostfxr_handle cxt;
};

void thread_proc(struct thread_user_data* userData)
{
    int rc = userData->runApp(userData->cxt);
    if (rc != 0)
    {
        userData->closeFxr(userData->cxt);
        std::stringstream stream;
        stream << "Run App failed: " << std::hex << std::showbase << rc;
        alt::ICore::Instance().LogError(cs::Log::LOG_PREFIX, stream.str());
    }
    delete userData;
}

void CoreClr::GenerateRuntimeConfigText(std::ofstream* outfile)
{
    if (version == nullptr)
    {
        core->LogError(cs::Log::LOG_PREFIX, "Unknown coreclr version");
        return;
    }
    semver_t sem_ver;
    if (semver_parse_version(version, &sem_ver) != 0)
    {
        core->LogError(cs::Log::LOG_PREFIX, "Couldn't parse coreclr version");
        return;
    }
    auto minor_version = std::to_string(sem_ver.major) + std::string(".") + std::to_string(sem_ver.minor);
    auto patch_version = std::to_string(sem_ver.major) + std::string(".") + std::to_string(sem_ver.minor) +
        std::string(".") + std::to_string(sem_ver.patch);
    (*outfile) << std::string("{\n"
            "  \"runtimeOptions\": {\n"
            "    \"tfm\": \"netcoreapp" + minor_version + "\",\n"
            "    \"framework\": {\n"
            "      \"name\": \"Microsoft.NETCore.App\",\n"
            "      \"version\": \"") + patch_version + "\"\n"
        "    }\n"
        "  }\n"
        "}";
    semver_free(&sem_ver);
}

bool CoreClr::CreateRuntimeConfigFile()
{
    auto fileName = (std::string(core->GetRootDirectory()) + DIR_SEPARATOR + std::string(
        "AltV.Net.Host.runtimeconfig.json"));
    struct stat buffer{};
    auto exists = (stat(fileName.c_str(), &buffer) == 0);
    if (exists) return false;
    std::ofstream outfile;
    outfile.open(fileName.c_str());
    GenerateRuntimeConfigText(&outfile);
    outfile << std::flush;
    outfile.close();
    return true;
}

void CoreClr::DeleteRuntimeConfigFile()
{
    remove(
        (std::string(core->GetRootDirectory()) + DIR_SEPARATOR + std::string("AltV.Net.Host.runtimeconfig.json")).
        c_str());
}

void CoreClr::CreateManagedHost()
{
    auto result = CreateRuntimeConfigFile();
    // Load .NET Core
    /*void* load_assembly_and_get_function_pointer = nullptr;
    hostfxr_initialize_parameters initializeParameters = {};
    initializeParameters.host_path = "";
    initializeParameters.dotnet_root = dotnetDirectory;
    initializeParameters.size = sizeof(hostfxr_initialize_parameters);*/
    int rc;
    const char_t* args[1];
    args[0] = STR("AltV.Net.Host.dll");
    rc = _initForCmd(1, args, nullptr, &cxt);
    if (rc != 0)
    {
        if (rc == 0x80008094)
        {
            core->LogError(cs::Log::LOG_PREFIX,
                "Make sure you have AltV.Net.Host.dll and AltV.Net.Host.runtimeconfig.json in the folder of the altv-server executable or binary.");
        }
        std::stringstream stream;
        stream << "Init for cmd failed: " << std::hex << std::showbase << rc;
        core->LogError(cs::Log::LOG_PREFIX, stream.str());
        _closeFxr(cxt);
        if (result)
        {
            DeleteRuntimeConfigFile();
        }
        return;
    }

    if (result)
    {
        DeleteRuntimeConfigFile();
    }

    auto userData = new struct thread_user_data;
    userData->runApp = _runApp;
    userData->closeFxr = _closeFxr;
    userData->cxt = cxt;
    thread = new std::thread(thread_proc, userData);
}

bool CoreClr::ExecuteManagedResource(const char* resourcePath, const char* resourceName,
                                     const char* resourceMain, alt::IResource* resource)
{
    if (cxt == nullptr) return false;
    std::unique_lock<std::mutex> lck(mtx);
    while (hostResourceExecute == nullptr) { cv.wait(lck); }
    if (hostResourceExecute == nullptr)
    {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Core CLR host not loaded"));
        return false;
    }

    // Run managed code

    struct lib_args
    {
        //string resourcePath, string resourceName, string resourceMain, int resourceIndex
        const char* resourcePath;
        const char* resourceName;
        const char* resourceMain;
        alt::ICore* corePointer;
        alt::IResource* resourcePointer;
        const function_table_t* funcTable;
    };
    lib_args args
    {
        resourcePath,
        resourceName,
        resourceMain,
        core,
        resource,
        get_func_table()
    };

    hostResourceExecute(&args, sizeof(args));
    return true;
}

bool CoreClr::ExecuteManagedResourceUnload(const char* resourcePath, const char* resourceMain)
{
    if (cxt == nullptr) return false;
    std::unique_lock<std::mutex> lck(mtx);
    while (hostResourceExecuteUnload == nullptr) { cv.wait(lck); }
    if (hostResourceExecuteUnload == nullptr)
    {
        core->LogInfo(cs::Log::LOG_PREFIX, std::string("coreclr-module: Core CLR host not loaded"));
        return false;
    }

    // Run managed code

    struct lib_args
    {
        const char* resourcePath;
        const char* resourceMain;
    };
    lib_args args
    {
        resourcePath,
        resourceMain,
    };

    hostResourceExecuteUnload(&args, sizeof(args));
    return true;
}
