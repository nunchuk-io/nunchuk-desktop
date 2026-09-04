[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[0-9a-fA-F]{40}$")]
    [string]$ExpectedCommit,

    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[0-9]+\.[0-9]+\.[0-9]+$")]
    [string]$ReleaseVersion,

    [Parameter(Mandatory = $true)]
    [long]$SourceDateEpoch,

    [Parameter(Mandatory = $true)]
    [ValidateSet("unsigned", "signed")]
    [string]$ArtifactFlavor,

    [string]$SourceDirectory = (Split-Path -Parent $PSScriptRoot),
    [string]$ApplicationExe,
    [string]$BuildInfoFile,
    [string]$TlsProbeExe,
    [string]$ExistingStageDirectory,
    [string]$QtDirectory = "C:\nunchuk-repro\qt\6.11.1\msvc2022_64",
    [string]$QtKeychainDirectory = "C:\nunchuk-repro\installed\qtkeychain",
    [string]$OpenSslDirectory = "C:\nunchuk-repro\installed\openssl-runtime",
    [string]$WorkDirectory = "C:\nunchuk-repro\package",
    [string]$OutputDirectory = "C:\nunchuk-repro\artifacts",
    [string]$LockFile = (Join-Path $PSScriptRoot "windows-dependencies.lock.json"),
    [string]$InstallerRecipe = (Join-Path $PSScriptRoot "windows-installer.iss"),
    [switch]$RequireSignedApplication,
    [switch]$SmokeTest
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

function Copy-DirectoryContents {
    param(
        [Parameter(Mandatory = $true)][string]$Source,
        [Parameter(Mandatory = $true)][string]$Destination
    )

    foreach ($item in Get-ChildItem -LiteralPath $Source -Force) {
        Copy-Item -LiteralPath $item.FullName -Destination $Destination -Recurse -Force
    }
}

function Assert-RequiredFile {
    param(
        [Parameter(Mandatory = $true)][string]$Root,
        [Parameter(Mandatory = $true)][string]$RelativePath
    )

    $path = Join-Path $Root $RelativePath
    if (!(Test-Path -LiteralPath $path -PathType Leaf)) {
        throw "Staging is missing required runtime file: $RelativePath"
    }
}

function Assert-StagedMatch {
    param(
        [Parameter(Mandatory = $true)][string]$Root,
        [Parameter(Mandatory = $true)][string]$Filter,
        [Parameter(Mandatory = $true)][string]$Label
    )

    $match = Get-ChildItem -LiteralPath $Root -Recurse -File -Filter $Filter -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($null -eq $match) {
        throw "Staging is missing required runtime component: $Label"
    }
}

function Get-OrdinalFiles {
    param([Parameter(Mandatory = $true)][string]$Root)

    $relativePaths = [string[]]@(Get-ChildItem -LiteralPath $Root -Recurse -File | ForEach-Object {
        [System.IO.Path]::GetRelativePath($Root, $_.FullName)
    })
    [System.Array]::Sort($relativePaths, [System.StringComparer]::Ordinal)
    foreach ($relativePath in $relativePaths) {
        Get-Item -LiteralPath (Join-Path $Root $relativePath)
    }
}

function New-DeterministicZip {
    param(
        [Parameter(Mandatory = $true)][string]$Source,
        [Parameter(Mandatory = $true)][string]$Destination,
        [Parameter(Mandatory = $true)][long]$Epoch
    )

    Add-Type -AssemblyName System.IO.Compression
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    $timestamp = [DateTimeOffset]::FromUnixTimeSeconds($Epoch)
    if ($timestamp.Year -lt 1980 -or $timestamp.Year -gt 2107) {
        throw "SOURCE_DATE_EPOCH is outside the ZIP timestamp range."
    }
    if (Test-Path -LiteralPath $Destination) {
        Remove-Item -LiteralPath $Destination -Force
    }
    $files = @(Get-OrdinalFiles $Source)
    $stream = [System.IO.File]::Open($Destination, [System.IO.FileMode]::CreateNew, [System.IO.FileAccess]::ReadWrite, [System.IO.FileShare]::None)
    try {
        $archive = [System.IO.Compression.ZipArchive]::new($stream, [System.IO.Compression.ZipArchiveMode]::Create, $false)
        try {
            foreach ($file in $files) {
                $relative = [System.IO.Path]::GetRelativePath($Source, $file.FullName).Replace([char]92, [char]47)
                $entry = $archive.CreateEntry($relative, [System.IO.Compression.CompressionLevel]::NoCompression)
                $entry.LastWriteTime = $timestamp
                $entry.ExternalAttributes = 0
                $input = [System.IO.File]::OpenRead($file.FullName)
                try {
                    $output = $entry.Open()
                    try {
                        $input.CopyTo($output)
                    }
                    finally {
                        $output.Dispose()
                    }
                }
                finally {
                    $input.Dispose()
                }
            }
        }
        finally {
            $archive.Dispose()
        }
    }
    finally {
        $stream.Dispose()
    }
}

$SourceDirectory = [System.IO.Path]::GetFullPath($SourceDirectory)
$WorkDirectory = [System.IO.Path]::GetFullPath($WorkDirectory)
$OutputDirectory = [System.IO.Path]::GetFullPath($OutputDirectory)
$LockFile = [System.IO.Path]::GetFullPath($LockFile)
$InstallerRecipe = [System.IO.Path]::GetFullPath($InstallerRecipe)

foreach ($file in @($LockFile, $InstallerRecipe)) {
    if (!(Test-Path -LiteralPath $file -PathType Leaf)) {
        throw "Required packaging input is missing: $file"
    }
}
$lock = Get-Content -LiteralPath $LockFile -Raw | ConvertFrom-Json
if ([string]$lock.qt.version -ne "6.11.1" -or [string]$lock.sources.qtKeychain.tag -ne "0.15.0") {
    throw "Packaging lock is not the approved Qt 6.11.1 / QtKeychain 0.15.0 lock."
}

$env:SOURCE_DATE_EPOCH = $SourceDateEpoch.ToString([System.Globalization.CultureInfo]::InvariantCulture)
$env:TZ = "UTC"
Reset-Directory $WorkDirectory
Reset-Directory $OutputDirectory
$stage = Join-Path $WorkDirectory "stage"
New-Item -ItemType Directory -Path $stage -Force | Out-Null

if ($ExistingStageDirectory) {
    if ($ArtifactFlavor -ne "signed") {
        throw "ExistingStageDirectory is reserved for the canonical signed payload."
    }
    $ExistingStageDirectory = [System.IO.Path]::GetFullPath($ExistingStageDirectory)
    if (!(Test-Path -LiteralPath $ExistingStageDirectory -PathType Container)) {
        throw "Existing signed stage is missing: $ExistingStageDirectory"
    }
    Copy-DirectoryContents $ExistingStageDirectory $stage
    foreach ($controlFileName in @(".reproducibility-gate.json", ".unsigned-payload-manifest.sha256")) {
        $controlFilePath = Join-Path $stage $controlFileName
        if (!(Test-Path -LiteralPath $controlFilePath -PathType Leaf)) {
            throw "The canonical signed stage is missing control file: $controlFileName"
        }
        Remove-Item -LiteralPath $controlFilePath -Force
    }
}
else {
    if (!$ApplicationExe -or !(Test-Path -LiteralPath $ApplicationExe -PathType Leaf)) {
        throw "ApplicationExe is required when ExistingStageDirectory is not supplied."
    }
    if (!$BuildInfoFile -or !(Test-Path -LiteralPath $BuildInfoFile -PathType Leaf)) {
        throw "BuildInfoFile is required when ExistingStageDirectory is not supplied."
    }
    if (!$TlsProbeExe -or !(Test-Path -LiteralPath $TlsProbeExe -PathType Leaf)) {
        throw "TlsProbeExe is required to prove the staged Qt OpenSSL backend."
    }
    $windeployqt = Join-Path $QtDirectory "bin\windeployqt.exe"
    $qtKeychainDll = Join-Path $QtKeychainDirectory "bin\qt6keychain.dll"
    foreach ($file in @($windeployqt, $qtKeychainDll)) {
        if (!(Test-Path -LiteralPath $file -PathType Leaf)) {
            throw "Required deployment tool/runtime is missing: $file"
        }
    }

    Copy-Item -LiteralPath $ApplicationExe -Destination (Join-Path $stage "nunchuk-qt.exe") -Force
    Copy-Item -LiteralPath $BuildInfoFile -Destination (Join-Path $stage "build-info.json") -Force
    Copy-Item -LiteralPath $qtKeychainDll -Destination (Join-Path $stage "qt6keychain.dll") -Force

    $scannerShim = Join-Path $QtDirectory "bin\qt6keychain.dll"
    $createdScannerShim = $false
    if (Test-Path -LiteralPath $scannerShim -PathType Leaf) {
        if ((Get-Sha256 $scannerShim) -ne (Get-Sha256 $qtKeychainDll)) {
            throw "Qt bin contains a different qt6keychain.dll scanner shim."
        }
    }
    else {
        Copy-Item -LiteralPath $qtKeychainDll -Destination $scannerShim -Force
        $createdScannerShim = $true
    }

    $oldPath = $env:Path
    $env:Path = "$stage;$(Split-Path -Parent $qtKeychainDll);$(Join-Path $QtDirectory 'bin');$env:Path"
    try {
        Invoke-Checked $windeployqt @(
            "--release",
            "--force",
            "--verbose", "2",
            "--compiler-runtime",
            "--qmldir", $SourceDirectory,
            (Join-Path $stage "nunchuk-qt.exe")
        )
    }
    finally {
        $env:Path = $oldPath
        if ($createdScannerShim -and (Test-Path -LiteralPath $scannerShim -PathType Leaf)) {
            Remove-Item -LiteralPath $scannerShim -Force
        }
    }

    $explicitQtFiles = [ordered]@{
        "bin\Qt6ShaderTools.dll" = "Qt6ShaderTools.dll"
        "bin\Qt6Svg.dll" = "Qt6Svg.dll"
        "plugins\imageformats\qgif.dll" = "imageformats\qgif.dll"
        "plugins\imageformats\qjpeg.dll" = "imageformats\qjpeg.dll"
        "plugins\imageformats\qsvg.dll" = "imageformats\qsvg.dll"
        "plugins\iconengines\qsvgicon.dll" = "iconengines\qsvgicon.dll"
        "plugins\tls\qopensslbackend.dll" = "tls\qopensslbackend.dll"
        "plugins\tls\qschannelbackend.dll" = "tls\qschannelbackend.dll"
    }
    foreach ($entry in $explicitQtFiles.GetEnumerator()) {
        $source = Join-Path $QtDirectory $entry.Key
        $destination = Join-Path $stage $entry.Value
        if (!(Test-Path -LiteralPath $source -PathType Leaf)) {
            throw "Explicit Qt runtime is missing: $source"
        }
        New-Item -ItemType Directory -Path (Split-Path -Parent $destination) -Force | Out-Null
        Copy-Item -LiteralPath $source -Destination $destination -Force
    }

    foreach ($dllName in @($lock.runtime.qtTlsOpenSsl.files | ForEach-Object { [string]$_ })) {
        $source = Join-Path $OpenSslDirectory "bin\$dllName"
        if (!(Test-Path -LiteralPath $source -PathType Leaf)) {
            throw "OpenSSL 3.5.7 Qt TLS DLL is missing: $source"
        }
        Copy-Item -LiteralPath $source -Destination (Join-Path $stage $dllName) -Force
    }

    $hwiArchive = Join-Path $WorkDirectory "hwi.zip"
    Get-VerifiedDownload $lock.runtime.hwi $hwiArchive
    $hwiExtracted = Join-Path $WorkDirectory "hwi"
    Expand-Archive -LiteralPath $hwiArchive -DestinationPath $hwiExtracted -Force
    $hwiCandidates = @(Get-ChildItem -LiteralPath $hwiExtracted -Recurse -File -Filter "hwi.exe")
    if ($hwiCandidates.Count -ne 1) {
        throw "Expected exactly one hwi.exe in the locked HWI archive; found $($hwiCandidates.Count)."
    }
    Copy-Item -LiteralPath $hwiCandidates[0].FullName -Destination (Join-Path $stage "hwi.exe") -Force
}

$sqlDirectory = Join-Path $stage "sqldrivers"
Assert-RequiredFile $stage "sqldrivers\qsqlite.dll"
Get-ChildItem -LiteralPath $sqlDirectory -File -Filter "*.dll" | Where-Object {
    $_.Name -ne "qsqlite.dll"
} | Remove-Item -Force

$multimediaDirectory = Join-Path $stage "multimedia"
$ffmpegPlugins = @(Get-ChildItem -LiteralPath $multimediaDirectory -File -Filter "*ffmpeg*mediaplugin.dll" -ErrorAction SilentlyContinue)
if ($ffmpegPlugins.Count -ne 1) {
    throw "Expected exactly one Qt FFmpeg multimedia plugin; found $($ffmpegPlugins.Count)."
}
Get-ChildItem -LiteralPath $multimediaDirectory -File -Filter "*.dll" | Where-Object {
    $_.FullName -ne $ffmpegPlugins[0].FullName
} | Remove-Item -Force

$tlsDirectory = Join-Path $stage "tls"
$allowedTlsPlugins = @("qopensslbackend.dll", "qschannelbackend.dll")
Get-ChildItem -LiteralPath $tlsDirectory -File -Filter "*.dll" | Where-Object {
    $_.Name -notin $allowedTlsPlugins
} | Remove-Item -Force
$positionDirectory = Join-Path $stage "position"
if (Test-Path -LiteralPath $positionDirectory -PathType Container) {
    Remove-Item -LiteralPath $positionDirectory -Recurse -Force
}

$requiredFiles = @(
    "nunchuk-qt.exe",
    "hwi.exe",
    "qt6keychain.dll",
    "Qt6Core.dll",
    "Qt6Multimedia.dll",
    "Qt6NetworkAuth.dll",
    "Qt6Qml.dll",
    "Qt6Quick.dll",
    "Qt6ShaderTools.dll",
    "Qt6Svg.dll",
    "Qt6WebEngineCore.dll",
    "Qt6WebEngineWidgets.dll",
    "libcrypto-3-x64.dll",
    "libssl-3-x64.dll",
    "msvcp140.dll",
    "vcruntime140.dll",
    "vcruntime140_1.dll",
    "platforms\qwindows.dll",
    "imageformats\qgif.dll",
    "imageformats\qjpeg.dll",
    "imageformats\qsvg.dll",
    "iconengines\qsvgicon.dll",
    "sqldrivers\qsqlite.dll",
    "tls\qopensslbackend.dll",
    "tls\qschannelbackend.dll",
    "build-info.json"
)
foreach ($relativePath in $requiredFiles) {
    Assert-RequiredFile $stage $relativePath
}
Assert-StagedMatch (Join-Path $stage "qml\Qt5Compat\GraphicalEffects\private") "*graphicaleffectsprivateplugin.dll" "Qt5Compat GraphicalEffects private plugin"
Assert-StagedMatch $stage "QtWebEngineProcess.exe" "QtWebEngineProcess.exe"
Assert-StagedMatch $stage "qtwebengine_resources.pak" "Qt WebEngine resources"
Assert-StagedMatch $stage "qtwebengine_resources_100p.pak" "Qt WebEngine 100 percent resources"
Assert-StagedMatch $stage "qtwebengine_resources_200p.pak" "Qt WebEngine 200 percent resources"
Assert-StagedMatch $stage "icudtl.dat" "Qt WebEngine ICU data"
Assert-StagedMatch $stage "v8_context_snapshot.bin" "Qt WebEngine V8 snapshot"
Assert-StagedMatch (Join-Path $stage "translations\qtwebengine_locales") "en-US.pak" "Qt WebEngine en-US locale"

if (!$ExistingStageDirectory) {
    $stagedTlsProbe = Join-Path $stage "qt-tls-probe.exe"
    Copy-Item -LiteralPath $TlsProbeExe -Destination $stagedTlsProbe -Force
    $oldPath = $env:Path
    $oldPluginPath = $env:QT_PLUGIN_PATH
    $env:Path = "$stage;$env:Path"
    $env:QT_PLUGIN_PATH = $stage
    try {
        Invoke-Checked $stagedTlsProbe @() $stage
    }
    finally {
        $env:Path = $oldPath
        $env:QT_PLUGIN_PATH = $oldPluginPath
        if (Test-Path -LiteralPath $stagedTlsProbe -PathType Leaf) {
            Remove-Item -LiteralPath $stagedTlsProbe -Force
        }
    }
    Write-Host "Qt qopensslbackend / OpenSSL 3.5.7 runtime smoke test: PASS"
}

$buildInfo = Get-Content -LiteralPath (Join-Path $stage "build-info.json") -Raw | ConvertFrom-Json
if ([string]$buildInfo.sourceCommit -ne $ExpectedCommit.ToLowerInvariant()) {
    throw "build-info source commit does not match the requested release commit."
}
if ([long]$buildInfo.sourceDateEpoch -ne $SourceDateEpoch) {
    throw "build-info SOURCE_DATE_EPOCH mismatch."
}
if ([string]$buildInfo.releaseVersion -ne $ReleaseVersion) {
    throw "build-info release version mismatch."
}
if ([string]$buildInfo.qtVersion -ne "6.11.1" -or
    [string]$buildInfo.applicationOpenSslVersion -ne "3.5.7" -or
    [string]$buildInfo.qtTlsOpenSslVersion -ne "3.5.7") {
    throw "build-info does not prove the locked Qt/OpenSSL runtime."
}
if ([string]$buildInfo.dependencyLockSha256 -ne (Get-Sha256 $LockFile)) {
    throw "build-info dependency lock digest mismatch."
}

$debugQtDlls = @(Get-ChildItem -LiteralPath $stage -Recurse -File -Filter "Qt6*d.dll")
if ($debugQtDlls.Count -ne 0) {
    throw "Release staging contains Qt debug DLLs: $($debugQtDlls.Name -join ', ')"
}
$reparsePoints = @(Get-ChildItem -LiteralPath $stage -Recurse -Force | Where-Object {
    ($_.Attributes -band [System.IO.FileAttributes]::ReparsePoint) -ne 0
})
if ($reparsePoints.Count -ne 0) {
    throw "Release staging must not contain reparse points: $($reparsePoints.FullName -join ', ')"
}

$applicationSignature = Get-AuthenticodeSignature -LiteralPath (Join-Path $stage "nunchuk-qt.exe")
if ($RequireSignedApplication) {
    foreach ($ownedPeName in @("nunchuk-qt.exe", "qt6keychain.dll", "hwi.exe")) {
        $ownedSignature = Get-AuthenticodeSignature -LiteralPath (Join-Path $stage $ownedPeName)
        if ($ownedSignature.Status -ne [System.Management.Automation.SignatureStatus]::Valid) {
            throw "The final package requires a valid Authenticode signature on $ownedPeName."
        }
    }
}
elseif ($applicationSignature.Status -ne [System.Management.Automation.SignatureStatus]::NotSigned) {
    throw "Unsigned replica unexpectedly contains a signed nunchuk-qt.exe."
}

$peFiles = @(Get-ChildItem -LiteralPath $stage -Recurse -File | Where-Object {
    $_.Extension -in @(".exe", ".dll")
})
foreach ($file in $peFiles) {
    $headers = (& dumpbin.exe /HEADERS $file.FullName 2>&1 | Out-String)
    if ($LASTEXITCODE -ne 0 -or $headers -notmatch '8664 machine \(x64\)') {
        throw "Staged runtime is not an x64 PE: $($file.FullName)"
    }
}

$stagedNames = @{}
foreach ($file in $peFiles) {
    $stagedNames[$file.Name.ToLowerInvariant()] = $true
}
$missingDependencies = [System.Collections.Generic.HashSet[string]]::new()
foreach ($file in $peFiles) {
    $dependencies = @(& dumpbin.exe /DEPENDENTS $file.FullName 2>&1)
    if ($LASTEXITCODE -ne 0) {
        [void]$missingDependencies.Add("$($file.Name): dumpbin failed")
        continue
    }
    foreach ($line in $dependencies) {
        if ($line -notmatch '^\s+([A-Za-z0-9_.+\-]+\.dll)\s*$') {
            continue
        }
        $dependency = $Matches[1]
        $key = $dependency.ToLowerInvariant()
        if ($stagedNames.ContainsKey($key) -or $key.StartsWith("api-ms-win-") -or $key.StartsWith("ext-ms-win-")) {
            continue
        }
        $system32 = Join-Path "$env:WINDIR\System32" $dependency
        $windows = Join-Path $env:WINDIR $dependency
        if (!(Test-Path -LiteralPath $system32) -and !(Test-Path -LiteralPath $windows)) {
            [void]$missingDependencies.Add("$($file.Name) -> $dependency")
        }
    }
}
if ($missingDependencies.Count -ne 0) {
    throw "Missing staged DLL dependencies: $($missingDependencies | Sort-Object | Out-String)"
}

$fixedTimestamp = [DateTimeOffset]::FromUnixTimeSeconds($SourceDateEpoch).UtcDateTime
foreach ($item in Get-ChildItem -LiteralPath $stage -Recurse -Force) {
    $item.CreationTimeUtc = $fixedTimestamp
    $item.LastAccessTimeUtc = $fixedTimestamp
    $item.LastWriteTimeUtc = $fixedTimestamp
}

$payloadFiles = @(Get-OrdinalFiles $stage)
$payloadManifestLines = foreach ($file in $payloadFiles) {
    $relative = [System.IO.Path]::GetRelativePath($stage, $file.FullName).Replace([char]92, [char]47)
    "$(Get-Sha256 $file.FullName)  $relative"
}
$payloadManifestPath = Join-Path $OutputDirectory "payload-manifest.sha256"
Write-Utf8NoBom $payloadManifestPath (($payloadManifestLines -join [char]10) + [char]10)

$packageWork = Join-Path $WorkDirectory "inno"
New-Item -ItemType Directory -Path $packageWork -Force | Out-Null
Copy-Item -LiteralPath $InstallerRecipe -Destination (Join-Path $packageWork "windows-installer.iss") -Force
$innoFileLines = foreach ($file in $payloadFiles) {
    $relative = [System.IO.Path]::GetRelativePath($stage, $file.FullName)
    $subdirectory = Split-Path -Parent $relative
    $destination = if ([string]::IsNullOrEmpty($subdirectory)) {
        "{app}"
    }
    else {
        "{app}\$subdirectory"
    }
    $sourceEscaped = $file.FullName.Replace('"', '""')
    $destinationEscaped = $destination.Replace('"', '""')
    'Source: "{0}"; DestDir: "{1}"; Flags: ignoreversion notimestamp' -f $sourceEscaped, $destinationEscaped
}
Write-Utf8NoBom (Join-Path $packageWork "windows-payload-files.iss") (($innoFileLines -join [char]10) + [char]10)

$innoInstaller = Join-Path $WorkDirectory "innosetup.exe"
Get-VerifiedDownload $lock.toolchain.innoSetup $innoInstaller
$innoRoot = Join-Path $WorkDirectory "inno-tool"
$innoInstallArguments = @(
    "/VERYSILENT",
    "/SUPPRESSMSGBOXES",
    "/NORESTART",
    "/SP-",
    "/CURRENTUSER",
    "/DIR=$innoRoot"
)
$innoProcess = Start-Process -FilePath $innoInstaller -ArgumentList $innoInstallArguments -Wait -PassThru
if ($innoProcess.ExitCode -ne 0) {
    throw "Pinned Inno Setup installer failed with exit code $($innoProcess.ExitCode)."
}
$iscc = Join-Path $innoRoot "ISCC.exe"
if (!(Test-Path -LiteralPath $iscc -PathType Leaf)) {
    throw "ISCC.exe is missing after installing the pinned Inno Setup package."
}

$namePrefix = "nunchuk-windows-x64-v$ReleaseVersion"
if ($ArtifactFlavor -eq "unsigned") {
    $archiveName = "$namePrefix-unsigned.zip"
    $installerBaseName = "$namePrefix-unsigned-setup"
}
else {
    $archiveName = "$namePrefix.zip"
    $installerBaseName = "$namePrefix-setup"
}
$numericVersion = ($ReleaseVersion -split "-", 2)[0] + ".0"
$innoOutput = Join-Path $WorkDirectory "installer-output"
New-Item -ItemType Directory -Path $innoOutput -Force | Out-Null
$isccArguments = @(
    "/Qp",
    "/DAppVersion=$ReleaseVersion",
    "/DAppVersionNumeric=$numericVersion",
    "/DOutputBaseFilename=$installerBaseName",
    "/DOutputDir=$innoOutput",
    (Join-Path $packageWork "windows-installer.iss")
)
Invoke-Checked $iscc $isccArguments $packageWork
$installerPath = Join-Path $innoOutput "$installerBaseName.exe"
if (!(Test-Path -LiteralPath $installerPath -PathType Leaf)) {
    throw "Inno Setup did not produce the expected installer: $installerPath"
}
if ((Get-AuthenticodeSignature -LiteralPath $installerPath).Status -ne [System.Management.Automation.SignatureStatus]::NotSigned) {
    throw "The installer must remain unsigned until the canonical payload has been selected."
}

$archivePath = Join-Path $OutputDirectory $archiveName
New-DeterministicZip $stage $archivePath $SourceDateEpoch
Copy-Item -LiteralPath $installerPath -Destination (Join-Path $OutputDirectory "$installerBaseName.exe") -Force

$packageMetadata = [ordered]@{
    schemaVersion = 1
    platform = "windows-x64-msvc2022"
    artifactFlavor = $ArtifactFlavor
    sourceCommit = $ExpectedCommit.ToLowerInvariant()
    sourceDateEpoch = $SourceDateEpoch
    releaseVersion = $ReleaseVersion
    dependencyLockSha256 = (Get-Sha256 $LockFile)
    payloadManifestSha256 = (Get-Sha256 $payloadManifestPath)
    archive = $archiveName
    installer = "$installerBaseName.exe"
}
$packageMetadataJson = $packageMetadata | ConvertTo-Json -Depth 6
Write-Utf8NoBom (Join-Path $OutputDirectory "build-metadata.json") ($packageMetadataJson + [char]10)

if ($SmokeTest) {
    $smokeDirectory = Join-Path $WorkDirectory "installer-smoke"
    $smokeLog = Join-Path $WorkDirectory "installer-smoke.log"
    $smokeArguments = @(
        "/VERYSILENT",
        "/SUPPRESSMSGBOXES",
        "/NORESTART",
        "/NOCANCEL",
        "/SP-",
        "/NOICONS",
        "/DIR=$smokeDirectory",
        "/TASKS=",
        "/LOG=$smokeLog"
    )
    $smokeProcess = Start-Process -FilePath $installerPath -ArgumentList $smokeArguments -Wait -PassThru
    if ($smokeProcess.ExitCode -ne 0) {
        if (Test-Path -LiteralPath $smokeLog -PathType Leaf) {
            Get-Content -LiteralPath $smokeLog -Tail 250
        }
        throw "Installer smoke test failed with exit code $($smokeProcess.ExitCode)."
    }
    foreach ($relativePath in @(
        "nunchuk-qt.exe",
        "hwi.exe",
        "qt6keychain.dll",
        "Qt6NetworkAuth.dll",
        "libcrypto-3-x64.dll",
        "libssl-3-x64.dll",
        "tls\qopensslbackend.dll",
        "unins000.exe"
    )) {
        Assert-RequiredFile $smokeDirectory $relativePath
    }
    $uninstaller = Join-Path $smokeDirectory "unins000.exe"
    $uninstallProcess = Start-Process -FilePath $uninstaller -ArgumentList @("/VERYSILENT", "/NORESTART") -Wait -PassThru
    if ($uninstallProcess.ExitCode -ne 0) {
        throw "Installer smoke cleanup failed with exit code $($uninstallProcess.ExitCode)."
    }
}

Write-Host "Windows $ArtifactFlavor package completed: $OutputDirectory"
