[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet("a", "b")]
    [string]$Replica,

    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[0-9a-fA-F]{40}$")]
    [string]$ExpectedCommit,

    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[0-9]+\.[0-9]+\.[0-9]+$")]
    [string]$ReleaseVersion,

    [Parameter(Mandatory = $true)]
    [long]$SourceDateEpoch,

    [string]$SourceDirectory = (Split-Path -Parent $PSScriptRoot),
    [string]$WorkDirectory = "C:\nunchuk-repro",
    [string]$OutputDirectory = "C:\nunchuk-repro\build-output",
    [string]$LockFile = (Join-Path $PSScriptRoot "windows-dependencies.lock.json")
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

function Invoke-Checked {
    param(
        [Parameter(Mandatory = $true)][string]$FilePath,
        [Parameter()][string[]]$ArgumentList = @(),
        [Parameter()][string]$WorkingDirectory
    )

    $oldLocation = Get-Location
    try {
        if ($WorkingDirectory) {
            Set-Location $WorkingDirectory
        }
        Write-Host ">> $FilePath $($ArgumentList -join ' ')"
        & $FilePath @ArgumentList
        if ($LASTEXITCODE -ne 0) {
            throw "Command failed with exit code $LASTEXITCODE ($FilePath)"
        }
    }
    finally {
        Set-Location $oldLocation
    }
}

function Write-Utf8NoBom {
    param(
        [Parameter(Mandatory = $true)][string]$Path,
        [Parameter(Mandatory = $true)][string]$Content
    )

    $encoding = [System.Text.UTF8Encoding]::new($false)
    $normalized = $Content.Replace(([char]13).ToString() + [char]10, [char]10)
    [System.IO.File]::WriteAllText($Path, $normalized, $encoding)
}

function Get-Sha256 {
    param([Parameter(Mandatory = $true)][string]$Path)
    return (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToLowerInvariant()
}

function Get-VerifiedDownload {
    param(
        [Parameter(Mandatory = $true)]$Dependency,
        [Parameter(Mandatory = $true)][string]$Destination
    )

    New-Item -ItemType Directory -Path (Split-Path -Parent $Destination) -Force | Out-Null
    Invoke-WebRequest -Uri ([string]$Dependency.url) -OutFile $Destination
    $actual = Get-Sha256 $Destination
    $expected = ([string]$Dependency.sha256).ToLowerInvariant()
    if ($actual -ne $expected) {
        throw "SHA-256 mismatch for $($Dependency.url): expected=$expected actual=$actual"
    }
}

function Reset-Directory {
    param([Parameter(Mandatory = $true)][string]$Path)

    $fullPath = [System.IO.Path]::GetFullPath($Path)
    $root = [System.IO.Path]::GetPathRoot($fullPath)
    if ($fullPath -eq $root -or $fullPath.Length -lt 10) {
        throw "Refusing to reset unsafe directory: $fullPath"
    }
    if (Test-Path -LiteralPath $fullPath) {
        Remove-Item -LiteralPath $fullPath -Recurse -Force
    }
    New-Item -ItemType Directory -Path $fullPath -Force | Out-Null
}

function Initialize-PinnedRepository {
    param(
        [Parameter(Mandatory = $true)][string]$Repository,
        [Parameter(Mandatory = $true)][string]$Commit,
        [Parameter(Mandatory = $true)][string]$Destination
    )

    Reset-Directory $Destination
    Invoke-Checked git @("init", "--quiet") $Destination
    Invoke-Checked git @("remote", "add", "origin", $Repository) $Destination
    Invoke-Checked git @("fetch", "--quiet", "--depth=1", "origin", $Commit) $Destination
    Invoke-Checked git @("-c", "advice.detachedHead=false", "checkout", "--quiet", "--detach", $Commit) $Destination
    $actual = (& git -C $Destination rev-parse HEAD).Trim()
    if ($actual -ne $Commit) {
        throw "Pinned checkout mismatch for $Repository; expected=$Commit actual=$actual"
    }
}

function Assert-CompileDatabaseUsesDynamicCrt {
    param(
        [Parameter(Mandatory = $true)][string]$Path,
        [Parameter(Mandatory = $true)][string]$Label
    )

    if (!(Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "$Label compile database is missing: $Path"
    }
    $raw = Get-Content -LiteralPath $Path -Raw
    if ($raw -match '(?i)(?<![A-Za-z0-9_])[/-]MTd?(?![A-Za-z0-9_])') {
        throw "$Label contains /MT or /MTd. Commit the /MD fix in CMake; this release script never rewrites source files."
    }
    if ($raw -match '(?i)(?<![A-Za-z0-9_])[/-]NODEFAULTLIB(?::[A-Za-z0-9_.-]+)?') {
        throw "$Label contains /NODEFAULTLIB and can silently remove the dynamic CRT."
    }
    if ($raw -notmatch '(?i)(?<![A-Za-z0-9_])[/-]MDd?(?![A-Za-z0-9_])') {
        throw "$Label does not prove use of the dynamic MSVC CRT (/MD)."
    }
    Write-Host "$Label dynamic CRT verification: PASS"
}

$SourceDirectory = [System.IO.Path]::GetFullPath($SourceDirectory)
$WorkDirectory = [System.IO.Path]::GetFullPath($WorkDirectory)
$OutputDirectory = [System.IO.Path]::GetFullPath($OutputDirectory)
$LockFile = [System.IO.Path]::GetFullPath($LockFile)

if (!(Test-Path -LiteralPath $LockFile -PathType Leaf)) {
    throw "Dependency lock file is missing: $LockFile"
}
if ($SourceDirectory.StartsWith($WorkDirectory + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "WorkDirectory must not contain the source checkout."
}
if ($WorkDirectory.StartsWith($SourceDirectory + [System.IO.Path]::DirectorySeparatorChar, [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "WorkDirectory must be outside the source checkout."
}
if ($env:VSCMD_ARG_TGT_ARCH -ne "x64") {
    throw "MSVC x64 developer environment is not active."
}
foreach ($tool in @("cl.exe", "link.exe", "lib.exe", "dumpbin.exe")) {
    Get-Command $tool -ErrorAction Stop | Out-Null
}

$actualCommit = (& git -C $SourceDirectory rev-parse HEAD).Trim()
if ($actualCommit -ne $ExpectedCommit.ToLowerInvariant()) {
    throw "Source checkout mismatch: expected=$ExpectedCommit actual=$actualCommit"
}
$cmakeText = Get-Content -LiteralPath (Join-Path $SourceDirectory "CMakeLists.txt") -Raw
$cmakeVersions = @([regex]::Matches(
    $cmakeText,
    '(?im)^\s*project\s*\([^\)\r\n]*\bVERSION\s+([0-9]+\.[0-9]+\.[0-9]+)\b'
) | ForEach-Object { $_.Groups[1].Value } | Sort-Object -Unique)
$mainText = Get-Content -LiteralPath (Join-Path $SourceDirectory "app\main.cpp") -Raw
$mainVersions = @([regex]::Matches(
    $mainText,
    'QCoreApplication::setApplicationVersion\s*\(\s*"([0-9]+\.[0-9]+\.[0-9]+)"\s*\)'
) | ForEach-Object { $_.Groups[1].Value })
if ($cmakeVersions.Count -ne 1 -or $cmakeVersions[0] -cne $ReleaseVersion -or
    $mainVersions.Count -ne 1 -or $mainVersions[0] -cne $ReleaseVersion) {
    throw "Release version mismatch: requested=$ReleaseVersion cmake=$($cmakeVersions -join ',') app=$($mainVersions -join ',')"
}
$commitEpochText = (& git -C $SourceDirectory show -s --format=%ct HEAD).Trim()
$commitEpoch = [long]::Parse($commitEpochText, [System.Globalization.CultureInfo]::InvariantCulture)
if ($commitEpoch -ne $SourceDateEpoch) {
    throw "SOURCE_DATE_EPOCH must equal the checked-out commit epoch: expected=$commitEpoch actual=$SourceDateEpoch"
}
$trackedChanges = @(& git -C $SourceDirectory status --porcelain=v1 --ignore-submodules=none)
if ($trackedChanges.Count -ne 0) {
    throw "Tracked source checkout is dirty before build: $($trackedChanges -join '; ')"
}
$submoduleStatus = @(& git -C $SourceDirectory submodule status --recursive)
$badSubmodules = @($submoduleStatus | Where-Object { $_ -match '^[+\-U]' })
if ($badSubmodules.Count -ne 0) {
    throw "Submodule checkout does not match gitlinks: $($badSubmodules -join '; ')"
}

$env:SOURCE_DATE_EPOCH = $SourceDateEpoch.ToString([System.Globalization.CultureInfo]::InvariantCulture)
$env:TZ = "UTC"
$env:QT_RCC_SOURCE_DATE_OVERRIDE = $env:SOURCE_DATE_EPOCH
$env:VCPKG_BINARY_SOURCES = "clear"
$env:VCPKG_FEATURE_FLAGS = "-binarycaching"
$env:VCPKG_FORCE_SYSTEM_BINARIES = "1"

$lock = Get-Content -LiteralPath $LockFile -Raw | ConvertFrom-Json
if ([int]$lock.schemaVersion -ne 1) {
    throw "Unsupported Windows dependency lock schema: $($lock.schemaVersion)"
}
if ([string]$lock.qt.version -ne "6.11.1") {
    throw "The release lock must pin Qt 6.11.1."
}
if ([string]$lock.sources.qtKeychain.tag -ne "0.15.0") {
    throw "The release lock must pin QtKeychain 0.15.0."
}
if ([string]$lock.runtime.qtTlsOpenSsl.version -ne "3.5.7") {
    throw "The Qt TLS runtime must remain locked to OpenSSL 3.5.7 LTS."
}

Reset-Directory $WorkDirectory
New-Item -ItemType Directory -Path $OutputDirectory -Force | Out-Null
$downloads = Join-Path $WorkDirectory "downloads"
$toolsDirectory = Join-Path $WorkDirectory "tools"
$dependenciesDirectory = Join-Path $WorkDirectory "dependencies"
$installedDirectory = Join-Path $WorkDirectory "installed"
$buildDirectory = Join-Path $WorkDirectory "build"
foreach ($directory in @($downloads, $toolsDirectory, $dependenciesDirectory, $installedDirectory, $buildDirectory)) {
    New-Item -ItemType Directory -Path $directory -Force | Out-Null
}
$buildSource = Join-Path $WorkDirectory "source"
New-Item -ItemType Directory -Path $buildSource -Force | Out-Null
& robocopy.exe $SourceDirectory $buildSource /MIR /XD .git /XF .git /R:1 /W:1 /NFL /NDL /NJH /NJS /NP
if ($LASTEXITCODE -ge 8) {
    throw "Failed to create the deterministic source copy; robocopy exit code=$LASTEXITCODE"
}
$sourceTimestamp = [DateTimeOffset]::FromUnixTimeSeconds($SourceDateEpoch).UtcDateTime
foreach ($item in Get-ChildItem -LiteralPath $buildSource -Recurse -Force) {
    $item.CreationTimeUtc = $sourceTimestamp
    $item.LastAccessTimeUtc = $sourceTimestamp
    $item.LastWriteTimeUtc = $sourceTimestamp
}

$cmakeArchive = Join-Path $downloads "cmake.zip"
Get-VerifiedDownload $lock.toolchain.cmake $cmakeArchive
$cmakeRoot = Join-Path $toolsDirectory "cmake"
Expand-Archive -LiteralPath $cmakeArchive -DestinationPath $cmakeRoot -Force
$cmakeExe = Get-ChildItem -LiteralPath $cmakeRoot -Recurse -File -Filter cmake.exe | Select-Object -First 1
if ($null -eq $cmakeExe) {
    throw "cmake.exe is missing after extracting the pinned archive."
}
$cmakeBin = $cmakeExe.Directory.FullName

$ninjaArchive = Join-Path $downloads "ninja.zip"
Get-VerifiedDownload $lock.toolchain.ninja $ninjaArchive
$ninjaRoot = Join-Path $toolsDirectory "ninja"
Expand-Archive -LiteralPath $ninjaArchive -DestinationPath $ninjaRoot -Force
$ninjaExe = Join-Path $ninjaRoot "ninja.exe"
if (!(Test-Path -LiteralPath $ninjaExe -PathType Leaf)) {
    throw "ninja.exe is missing after extracting the pinned archive."
}

$aqtExe = Join-Path $toolsDirectory "aqt.exe"
Get-VerifiedDownload $lock.toolchain.aqt $aqtExe
$env:Path = "$cmakeBin;$ninjaRoot;$env:Path"
Invoke-Checked $cmakeExe.FullName @("--version")
Invoke-Checked $ninjaExe @("--version")
Invoke-Checked $aqtExe @("version")

$qtRoot = Join-Path $WorkDirectory "qt"
$qtArguments = @(
    "install-qt",
    [string]$lock.qt.host,
    [string]$lock.qt.target,
    [string]$lock.qt.version,
    [string]$lock.qt.architecture,
    "--outputdir",
    $qtRoot,
    # aqt's default mirror rotation can pick an unofficial mirror missing
    # Updates.xml/checksums for a given version ("Failed to locate XML data
    # for Qt version ..."). Pin the official host to avoid that flakiness.
    "--base",
    "https://download.qt.io",
    "-m"
)
$qtArguments += @($lock.qt.modules | ForEach-Object { [string]$_ })
Invoke-Checked $aqtExe $qtArguments

$qtDirectory = Join-Path $qtRoot "$($lock.qt.version)\$($lock.qt.directoryName)"
$requiredQtFiles = @(
    "bin\Qt6Core.dll",
    "bin\Qt6Multimedia.dll",
    "bin\Qt6NetworkAuth.dll",
    "bin\Qt6ShaderTools.dll",
    "bin\Qt6WebEngineCore.dll",
    "bin\Qt6WebEngineWidgets.dll",
    "bin\windeployqt.exe",
    "plugins\platforms\qwindows.dll",
    "plugins\tls\qopensslbackend.dll",
    "plugins\tls\qschannelbackend.dll"
)
foreach ($relativePath in $requiredQtFiles) {
    $path = Join-Path $qtDirectory $relativePath
    if (!(Test-Path -LiteralPath $path -PathType Leaf)) {
        throw "Pinned Qt installation is incomplete: $relativePath"
    }
}
$graphicalEffectsPrivate = Get-ChildItem -LiteralPath (Join-Path $qtDirectory "qml\Qt5Compat\GraphicalEffects\private") -File -Filter "*graphicaleffectsprivateplugin.dll" -ErrorAction SilentlyContinue | Select-Object -First 1
if ($null -eq $graphicalEffectsPrivate) {
    throw "Qt 5 Compat GraphicalEffects private plugin is missing."
}

$vcpkgDirectory = Join-Path $dependenciesDirectory "vcpkg"
Initialize-PinnedRepository ([string]$lock.sources.vcpkg.repository) ([string]$lock.sources.vcpkg.commit) $vcpkgDirectory
Invoke-Checked (Join-Path $vcpkgDirectory "bootstrap-vcpkg.bat") @("-disableMetrics") $vcpkgDirectory
$vcpkgExe = Join-Path $vcpkgDirectory "vcpkg.exe"
$triplet = [string]$lock.sources.vcpkg.triplet
$tripletFile = @(
    (Join-Path $vcpkgDirectory "triplets\$triplet.cmake"),
    (Join-Path $vcpkgDirectory "triplets\community\$triplet.cmake")
) | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } | Select-Object -First 1
if (!$tripletFile) {
    throw "Pinned vcpkg triplet does not exist: $triplet"
}
$tripletText = Get-Content -LiteralPath $tripletFile -Raw
if ($tripletText -notmatch 'VCPKG_LIBRARY_LINKAGE\s+static' -or $tripletText -notmatch 'VCPKG_CRT_LINKAGE\s+dynamic') {
    throw "$triplet must use static libraries and dynamic MSVC CRT."
}
$vcpkgPackages = @(
    "boost-algorithm",
    "boost-asio",
    "boost-bind",
    "boost-format",
    "boost-multi-index",
    "boost-process",
    "boost-signals2",
    "boost-tokenizer",
    "zeromq",
    "libevent",
    "berkeleydb",
    "sqlite3"
) | ForEach-Object { "{0}:{1}" -f $_, $triplet }
Invoke-Checked $vcpkgExe (@("install") + $vcpkgPackages + @("--clean-after-build")) $vcpkgDirectory
$vcpkgList = @(& $vcpkgExe list)
if ($LASTEXITCODE -ne 0) {
    throw "Unable to verify the exact vcpkg package set."
}
$unexpectedVcpkgOpenSsl = @($vcpkgList | Where-Object { $_ -match '^openssl:' })
if ($unexpectedVcpkgOpenSsl.Count -ne 0) {
    throw "vcpkg unexpectedly installed OpenSSL; the application and Qt TLS runtime must both use locked OpenSSL 3.5.7: $($unexpectedVcpkgOpenSsl -join '; ')"
}

$opensslArchive = Join-Path $downloads "openssl-$($lock.runtime.qtTlsOpenSsl.version).tar.gz"
Get-VerifiedDownload $lock.runtime.qtTlsOpenSsl $opensslArchive
$perlExe = (Get-Command perl.exe -ErrorAction Stop).Source
Get-Command nmake.exe -ErrorAction Stop | Out-Null
Get-Command nasm.exe -ErrorAction Stop | Out-Null
$oldClAppend = $env:_CL_
$oldLinkAppend = $env:_LINK_
$env:_CL_ = "/MD /Brepro /ZH:SHA_256 /pathmap:$WorkDirectory=/_/work"
$env:_LINK_ = "/Brepro"
try {
    $opensslStaticSource = Join-Path $dependenciesDirectory "openssl-static-source"
    New-Item -ItemType Directory -Path $opensslStaticSource -Force | Out-Null
    Invoke-Checked tar.exe @("-xzf", $opensslArchive, "-C", $opensslStaticSource, "--strip-components=1")
    $opensslStaticInstall = Join-Path $installedDirectory "openssl-static"
    $opensslStaticConfigure = @(
        "Configure",
        [string]$lock.runtime.qtTlsOpenSsl.configureTarget,
        "no-shared",
        "no-tests",
        "--prefix=$opensslStaticInstall",
        "--openssldir=$(Join-Path $opensslStaticInstall 'ssl')",
        "--libdir=lib"
    )
    Invoke-Checked $perlExe $opensslStaticConfigure $opensslStaticSource
    Invoke-Checked nmake.exe @() $opensslStaticSource
    Invoke-Checked nmake.exe @("install_sw") $opensslStaticSource

    $opensslSharedSource = Join-Path $dependenciesDirectory "openssl-shared-source"
    New-Item -ItemType Directory -Path $opensslSharedSource -Force | Out-Null
    Invoke-Checked tar.exe @("-xzf", $opensslArchive, "-C", $opensslSharedSource, "--strip-components=1")
    $opensslInstall = Join-Path $installedDirectory "openssl-runtime"
    $opensslSharedConfigure = @(
        "Configure",
        [string]$lock.runtime.qtTlsOpenSsl.configureTarget,
        "shared",
        "no-tests",
        "--prefix=$opensslInstall",
        "--openssldir=$(Join-Path $opensslInstall 'ssl')",
        "--libdir=lib"
    )
    Invoke-Checked $perlExe $opensslSharedConfigure $opensslSharedSource
    Invoke-Checked nmake.exe @() $opensslSharedSource
    Invoke-Checked nmake.exe @("install_sw") $opensslSharedSource
}
finally {
    $env:_CL_ = $oldClAppend
    $env:_LINK_ = $oldLinkAppend
}

$opensslStaticCrypto = Join-Path $opensslStaticInstall "lib\libcrypto.lib"
$opensslStaticSsl = Join-Path $opensslStaticInstall "lib\libssl.lib"
foreach ($library in @($opensslStaticCrypto, $opensslStaticSsl)) {
    if (!(Test-Path -LiteralPath $library -PathType Leaf)) {
        throw "The OpenSSL 3.5.7 static archive is missing: $library"
    }
}
foreach ($dllName in @($lock.runtime.qtTlsOpenSsl.files | ForEach-Object { [string]$_ })) {
    $dllPath = Join-Path $opensslInstall "bin\$dllName"
    if (!(Test-Path -LiteralPath $dllPath -PathType Leaf)) {
        throw "The dynamic Qt TLS runtime is missing: $dllPath"
    }
}
$opensslVersionOutput = (& (Join-Path $opensslInstall "bin\openssl.exe") version)
if ($LASTEXITCODE -ne 0 -or $opensslVersionOutput.Trim() -notmatch '^OpenSSL 3\.5\.7(?:\s|$)') {
    throw "Built Qt TLS runtime does not report exact OpenSSL 3.5.7: $opensslVersionOutput"
}
$opensslStaticVersionOutput = (& (Join-Path $opensslStaticInstall "bin\openssl.exe") version)
if ($LASTEXITCODE -ne 0 -or $opensslStaticVersionOutput.Trim() -notmatch '^OpenSSL 3\.5\.7(?:\s|$)') {
    throw "Built application OpenSSL does not report exact OpenSSL 3.5.7: $opensslStaticVersionOutput"
}

$pathMaps = "/Brepro /ZH:SHA_256 /pathmap:$buildSource=/_/src /pathmap:$WorkDirectory=/_/work"
$linkerFlags = "/Brepro /INCREMENTAL:NO /PDBALTPATH:%_PDB%"
$staticLinkerFlags = "/Brepro"

$tlsProbeSource = Join-Path $dependenciesDirectory "qt-tls-probe"
$tlsProbeBuild = Join-Path $buildDirectory "qt-tls-probe"
New-Item -ItemType Directory -Path $tlsProbeSource -Force | Out-Null
$tlsProbeMain = @'
#include <QCoreApplication>
#include <QDebug>
#include <QSslSocket>

int main(int argc, char **argv) {
    QCoreApplication application(argc, argv);
    if (!QSslSocket::setActiveBackend(QStringLiteral("openssl"))) {
        qCritical() << "OpenSSL TLS backend is unavailable";
        return 10;
    }
    if (QSslSocket::activeBackend() != QStringLiteral("openssl")) {
        qCritical() << "Unexpected TLS backend" << QSslSocket::activeBackend();
        return 11;
    }
    if (!QSslSocket::supportsSsl()) {
        qCritical() << "Qt reports that SSL is unsupported";
        return 12;
    }
    const QString runtimeVersion = QSslSocket::sslLibraryVersionString();
    qInfo().noquote() << "Qt TLS runtime:" << runtimeVersion;
    if (!runtimeVersion.startsWith(QStringLiteral("OpenSSL 3.5.7"))) {
        qCritical().noquote() << "Unexpected Qt TLS OpenSSL runtime:" << runtimeVersion;
        return 13;
    }
    return 0;
}
'@
$tlsProbeCmake = @'
cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0091 NEW)
project(qt_tls_probe LANGUAGES CXX)
find_package(Qt6 6.11.1 EXACT COMPONENTS Core Network REQUIRED)
add_executable(qt-tls-probe main.cpp)
target_link_libraries(qt-tls-probe PRIVATE Qt6::Core Qt6::Network)
set_property(TARGET qt-tls-probe PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
target_compile_features(qt-tls-probe PRIVATE cxx_std_17)
'@
Write-Utf8NoBom (Join-Path $tlsProbeSource "main.cpp") ($tlsProbeMain + [char]10)
Write-Utf8NoBom (Join-Path $tlsProbeSource "CMakeLists.txt") ($tlsProbeCmake + [char]10)
$tlsProbeConfigure = @(
    "-S", $tlsProbeSource,
    "-B", $tlsProbeBuild,
    "-G", "Ninja",
    "-DCMAKE_MAKE_PROGRAM=$ninjaExe",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=$qtDirectory",
    "-DCMAKE_POLICY_DEFAULT_CMP0091=NEW",
    '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL',
    "-DCMAKE_CXX_FLAGS=$pathMaps",
    "-DCMAKE_EXE_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
)
Invoke-Checked $cmakeExe.FullName $tlsProbeConfigure
Assert-CompileDatabaseUsesDynamicCrt (Join-Path $tlsProbeBuild "compile_commands.json") "Qt TLS probe"
Invoke-Checked $cmakeExe.FullName @("--build", $tlsProbeBuild, "--parallel", "2")
$tlsProbeCandidates = @(Get-ChildItem -LiteralPath $tlsProbeBuild -Recurse -File -Filter "qt-tls-probe.exe")
if ($tlsProbeCandidates.Count -ne 1) {
    throw "Expected exactly one Qt TLS probe executable; found $($tlsProbeCandidates.Count)."
}
Copy-Item -LiteralPath $tlsProbeCandidates[0].FullName -Destination (Join-Path $OutputDirectory "qt-tls-probe.exe") -Force

$eventLibrary = Join-Path $vcpkgDirectory "installed\$triplet\lib\event.lib"
if (!(Test-Path -LiteralPath $eventLibrary -PathType Leaf)) {
    throw "Pinned static libevent archive is missing: $eventLibrary"
}

$olmDirectory = Join-Path $dependenciesDirectory "olm"
$olmInstall = Join-Path $installedDirectory "olm"
Initialize-PinnedRepository ([string]$lock.sources.olm.repository) ([string]$lock.sources.olm.commit) $olmDirectory
$olmBuild = Join-Path $buildDirectory "olm"
$olmConfigure = @(
    "-S", $olmDirectory,
    "-B", $olmBuild,
    "-G", "Ninja",
    "-DCMAKE_MAKE_PROGRAM=$ninjaExe",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_INSTALL_PREFIX=$olmInstall",
    "-DCMAKE_POLICY_DEFAULT_CMP0091=NEW",
    '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL',
    "-DCMAKE_C_FLAGS=$pathMaps",
    "-DCMAKE_CXX_FLAGS=$pathMaps",
    "-DCMAKE_EXE_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_SHARED_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_STATIC_LINKER_FLAGS=$staticLinkerFlags",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    "-DBUILD_SHARED_LIBS=OFF",
    "-DOLM_TESTS=OFF"
)
Invoke-Checked $cmakeExe.FullName $olmConfigure
Assert-CompileDatabaseUsesDynamicCrt (Join-Path $olmBuild "compile_commands.json") "Olm"
Invoke-Checked $cmakeExe.FullName @("--build", $olmBuild, "--parallel", "2")
Invoke-Checked $cmakeExe.FullName @("--install", $olmBuild)

$qtKeychainDirectory = Join-Path $dependenciesDirectory "qtkeychain"
$qtKeychainInstall = Join-Path $installedDirectory "qtkeychain"
Initialize-PinnedRepository ([string]$lock.sources.qtKeychain.repository) ([string]$lock.sources.qtKeychain.commit) $qtKeychainDirectory
$qtKeychainBuild = Join-Path $buildDirectory "qtkeychain"
$qtKeychainConfigure = @(
    "-S", $qtKeychainDirectory,
    "-B", $qtKeychainBuild,
    "-G", "Ninja",
    "-DCMAKE_MAKE_PROGRAM=$ninjaExe",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_INSTALL_PREFIX=$qtKeychainInstall",
    "-DCMAKE_PREFIX_PATH=$qtDirectory",
    "-DCMAKE_TOOLCHAIN_FILE=$(Join-Path $vcpkgDirectory 'scripts\buildsystems\vcpkg.cmake')",
    "-DVCPKG_TARGET_TRIPLET=$triplet",
    "-DCMAKE_POLICY_DEFAULT_CMP0091=NEW",
    '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL',
    "-DCMAKE_C_FLAGS=$pathMaps",
    "-DCMAKE_CXX_FLAGS=$pathMaps",
    "-DCMAKE_EXE_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_SHARED_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_STATIC_LINKER_FLAGS=$staticLinkerFlags",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    "-DBUILD_WITH_QT6=ON",
    "-DBUILD_TEST_APPLICATION=OFF",
    "-DBUILD_TRANSLATIONS=OFF"
)
Invoke-Checked $cmakeExe.FullName $qtKeychainConfigure
Assert-CompileDatabaseUsesDynamicCrt (Join-Path $qtKeychainBuild "compile_commands.json") "QtKeychain"
Invoke-Checked $cmakeExe.FullName @("--build", $qtKeychainBuild, "--parallel", "2")
Invoke-Checked $cmakeExe.FullName @("--install", $qtKeychainBuild)
$qtKeychainDll = Join-Path $qtKeychainInstall "bin\qt6keychain.dll"
if (!(Test-Path -LiteralPath $qtKeychainDll -PathType Leaf)) {
    throw "QtKeychain 0.15.0 did not install qt6keychain.dll."
}

$applicationBuild = Join-Path $buildDirectory "nunchuk"
$prefixPath = "$qtDirectory;$qtKeychainInstall;$olmInstall"
$applicationConfigure = @(
    "-S", $buildSource,
    "-B", $applicationBuild,
    "-G", "Ninja",
    "-DCMAKE_MAKE_PROGRAM=$ninjaExe",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=$prefixPath",
    "-DCMAKE_TOOLCHAIN_FILE=$(Join-Path $vcpkgDirectory 'scripts\buildsystems\vcpkg.cmake')",
    "-DVCPKG_TARGET_TRIPLET=$triplet",
    "-DCMAKE_POLICY_DEFAULT_CMP0091=NEW",
    '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL',
    "-DCMAKE_C_FLAGS=$pathMaps",
    "-DCMAKE_CXX_FLAGS=$pathMaps",
    "-DCMAKE_EXE_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_SHARED_LINKER_FLAGS=$linkerFlags",
    "-DCMAKE_STATIC_LINKER_FLAGS=$staticLinkerFlags",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    "-Devent_lib:FILEPATH=$eventLibrary",
    "-DOlm_DIR=$(Join-Path $olmInstall 'lib\cmake\Olm')",
    "-DQt6Keychain_DIR=$(Join-Path $qtKeychainInstall 'lib\cmake\Qt6Keychain')",
    "-DOPENSSL_ROOT_DIR=$opensslStaticInstall",
    "-DOPENSSL_INCLUDE_DIR:PATH=$(Join-Path $opensslStaticInstall 'include')",
    "-DOPENSSL_SSL_LIBRARY:FILEPATH=$opensslStaticSsl",
    "-DOPENSSL_CRYPTO_LIBRARY:FILEPATH=$opensslStaticCrypto",
    "-DOPENSSL_USE_STATIC_LIBS=TRUE",
    "-DUR__DISABLE_TESTS=ON",
    "-DBUILD_SHARED_LIBS=OFF"
)
Invoke-Checked $cmakeExe.FullName $applicationConfigure
Assert-CompileDatabaseUsesDynamicCrt (Join-Path $applicationBuild "compile_commands.json") "Nunchuk"
$applicationCache = Get-Content -LiteralPath (Join-Path $applicationBuild "CMakeCache.txt") -Raw
foreach ($library in @($opensslStaticCrypto, $opensslStaticSsl)) {
    $normalizedLibrary = $library.Replace([char]92, [char]47)
    if ($applicationCache -notmatch [regex]::Escape($normalizedLibrary)) {
        throw "CMakeCache does not select the pinned OpenSSL 3.5.7 static archive: $normalizedLibrary"
    }
}
$ninjaFile = Get-Content -LiteralPath (Join-Path $applicationBuild "build.ninja") -Raw
if ($ninjaFile -match '(?i)(?<![A-Za-z0-9_])[/-]NODEFAULTLIB(?::[A-Za-z0-9_.-]+)?') {
    throw "Generated Nunchuk link graph contains /NODEFAULTLIB."
}
Invoke-Checked $cmakeExe.FullName @("--build", $applicationBuild, "--parallel", "2")

$postBuildSourceChanges = @(& git -C $SourceDirectory status --porcelain=v1 --ignore-submodules=none)
if ($postBuildSourceChanges.Count -ne 0) {
    throw "The original checkout changed during the build: $($postBuildSourceChanges -join '; ')"
}

$applicationCandidates = @(Get-ChildItem -LiteralPath $applicationBuild -Recurse -File -Filter "nunchuk-qt.exe")
if ($applicationCandidates.Count -ne 1) {
    throw "Expected exactly one nunchuk-qt.exe, found $($applicationCandidates.Count)."
}
$applicationOutput = Join-Path $OutputDirectory "nunchuk-qt.exe"
Copy-Item -LiteralPath $applicationCandidates[0].FullName -Destination $applicationOutput -Force

$headers = (& dumpbin.exe /HEADERS $applicationOutput 2>&1 | Out-String)
if ($LASTEXITCODE -ne 0 -or $headers -notmatch '8664 machine \(x64\)') {
    throw "Built nunchuk-qt.exe is not an x64 PE."
}
if ((Get-AuthenticodeSignature -LiteralPath $applicationOutput).Status -ne [System.Management.Automation.SignatureStatus]::NotSigned) {
    throw "The replica build must remain unsigned until both replicas compare equal."
}

$lockHash = Get-Sha256 $LockFile
$clPath = (Get-Command cl.exe -ErrorAction Stop).Source
$clVersion = (Get-Item -LiteralPath $clPath).VersionInfo.FileVersion
$buildInfo = [ordered]@{
    schemaVersion = 1
    platform = "windows-x64-msvc2022"
    sourceCommit = $ExpectedCommit.ToLowerInvariant()
    sourceDateEpoch = $SourceDateEpoch
    releaseVersion = $ReleaseVersion
    dependencyLockSha256 = $lockHash
    qtVersion = [string]$lock.qt.version
    qtArchitecture = [string]$lock.qt.architecture
    qtModules = @($lock.qt.modules | ForEach-Object { [string]$_ })
    qtKeychainCommit = [string]$lock.sources.qtKeychain.commit
    olmCommit = [string]$lock.sources.olm.commit
    vcpkgCommit = [string]$lock.sources.vcpkg.commit
    vcpkgTriplet = $triplet
    applicationOpenSslVersion = [string]$lock.runtime.qtTlsOpenSsl.version
    qtTlsOpenSslVersion = [string]$lock.runtime.qtTlsOpenSsl.version
    qtTlsOpenSslSha256 = [string]$lock.runtime.qtTlsOpenSsl.sha256
    msvcFileVersion = $clVersion
    runnerImage = [string]$env:ImageOS
    runnerImageVersion = [string]$env:ImageVersion
}
$buildInfoJson = $buildInfo | ConvertTo-Json -Depth 8
Write-Utf8NoBom (Join-Path $OutputDirectory "build-info.json") ($buildInfoJson + [char]10)

Write-Host "Windows replica $Replica build completed: $applicationOutput"
