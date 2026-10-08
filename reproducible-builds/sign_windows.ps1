[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet("ValidateUnsigned", "Prepare", "VerifyOwnedPe", "Finalize")]
    [string]$Mode,

    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[0-9a-fA-F]{40}$")]
    [string]$ExpectedCommit,

    [Parameter(Mandatory = $true)]
    [ValidatePattern("^[0-9]+\.[0-9]+\.[0-9]+$")]
    [string]$ReleaseVersion,

    [Parameter(Mandatory = $true)]
    [long]$SourceDateEpoch,

    [string]$PackageDirectory,
    [string]$StageDirectory,
    [string]$ExpectedSignerSubject,
    [string]$ExpectedReproducibilityGateSha256,
    [string]$LockFile = (Join-Path $PSScriptRoot "windows-dependencies.lock.json")
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Write-Utf8NoBom {
    param(
        [Parameter(Mandatory = $true)][string]$Path,
        [Parameter(Mandatory = $true)][string]$Content
    )

    $encoding = [System.Text.UTF8Encoding]::new($false)
    $normalized = $Content.Replace("`r`n", "`n")
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

function Get-PackageMetadata {
    param(
        [Parameter(Mandatory = $true)][string]$Directory,
        [Parameter(Mandatory = $true)][string]$ExpectedFlavor
    )

    $metadataPath = Join-Path $Directory "build-metadata.json"
    if (!(Test-Path -LiteralPath $metadataPath -PathType Leaf)) {
        throw "Package metadata is missing: $metadataPath"
    }
    $metadata = Get-Content -LiteralPath $metadataPath -Raw | ConvertFrom-Json
    if ([int]$metadata.schemaVersion -ne 1 -or
        [string]$metadata.platform -cne "windows-x64-msvc2022") {
        throw "Unsupported Windows package metadata."
    }
    if ([string]$metadata.artifactFlavor -ne $ExpectedFlavor) {
        throw "Package flavor mismatch: expected=$ExpectedFlavor actual=$($metadata.artifactFlavor)"
    }
    if ([string]$metadata.sourceCommit -ne $ExpectedCommit.ToLowerInvariant()) {
        throw "Package source commit mismatch."
    }
    if ([long]$metadata.sourceDateEpoch -ne $SourceDateEpoch) {
        throw "Package SOURCE_DATE_EPOCH mismatch."
    }
    if ([string]$metadata.releaseVersion -ne $ReleaseVersion) {
        throw "Package release version mismatch."
    }
    if ([string]$metadata.dependencyLockSha256 -ne (Get-Sha256 $LockFile)) {
        throw "Package dependency lock digest mismatch."
    }
    $payloadManifestPath = Join-Path $Directory "payload-manifest.sha256"
    if (!(Test-Path -LiteralPath $payloadManifestPath -PathType Leaf) -or
        [string]$metadata.payloadManifestSha256 -cne (Get-Sha256 $payloadManifestPath)) {
        throw "Package payload manifest digest mismatch."
    }
    $namePrefix = "nunchuk-windows-x64-v$ReleaseVersion"
    $expectedArchive = if ($ExpectedFlavor -eq "unsigned") { "$namePrefix-unsigned.zip" } else { "$namePrefix.zip" }
    $expectedInstaller = if ($ExpectedFlavor -eq "unsigned") { "$namePrefix-unsigned-setup.exe" } else { "$namePrefix-setup.exe" }
    if ([string]$metadata.archive -cne $expectedArchive -or
        [string]$metadata.installer -cne $expectedInstaller) {
        throw "Package artifact names do not match the requested flavor/version."
    }
    return $metadata
}

function Resolve-PackageFile {
    param(
        [Parameter(Mandatory = $true)][string]$Directory,
        [Parameter(Mandatory = $true)][string]$FileName
    )

    if ([System.IO.Path]::GetFileName($FileName) -ne $FileName) {
        throw "Package metadata contains a non-canonical file name: $FileName"
    }
    $path = Join-Path $Directory $FileName
    if (!(Test-Path -LiteralPath $path -PathType Leaf)) {
        throw "Package file is missing: $path"
    }
    return [System.IO.Path]::GetFullPath($path)
}

function Expand-CheckedZip {
    param(
        [Parameter(Mandatory = $true)][string]$ArchivePath,
        [Parameter(Mandatory = $true)][string]$Destination
    )

    Add-Type -AssemblyName System.IO.Compression
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    Reset-Directory $Destination
    $destinationRoot = [System.IO.Path]::GetFullPath($Destination) + [System.IO.Path]::DirectorySeparatorChar
    $archive = [System.IO.Compression.ZipFile]::OpenRead($ArchivePath)
    try {
        foreach ($entry in $archive.Entries) {
            $relative = $entry.FullName.Replace([char]92, [char]47)
            if ([string]::IsNullOrWhiteSpace($relative) -or
                $relative.StartsWith("/") -or
                $relative.Contains(":") -or
                $relative -match '(^|/)\.\.(/|$)') {
                throw "Unsafe ZIP entry: $relative"
            }
            $destinationPath = [System.IO.Path]::GetFullPath((Join-Path $Destination $relative))
            if (!$destinationPath.StartsWith($destinationRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
                throw "ZIP entry escapes the destination: $relative"
            }
            if ($relative.EndsWith("/")) {
                New-Item -ItemType Directory -Path $destinationPath -Force | Out-Null
                continue
            }
            New-Item -ItemType Directory -Path (Split-Path -Parent $destinationPath) -Force | Out-Null
            $input = $entry.Open()
            try {
                $output = [System.IO.File]::Open($destinationPath, [System.IO.FileMode]::CreateNew, [System.IO.FileAccess]::Write, [System.IO.FileShare]::None)
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

function Assert-PayloadManifest {
    param(
        [Parameter(Mandatory = $true)][string]$ManifestPath,
        [Parameter(Mandatory = $true)][string]$Stage,
        [string[]]$IgnoredRelativePaths = @()
    )

    if (!(Test-Path -LiteralPath $ManifestPath -PathType Leaf)) {
        throw "Payload manifest is missing: $ManifestPath"
    }
    $expected = @{}
    foreach ($line in Get-Content -LiteralPath $ManifestPath) {
        if ($line -notmatch '^([0-9a-f]{64})  (.+)$') {
            throw "Invalid payload manifest line: $line"
        }
        $relative = $Matches[2]
        if ($relative.StartsWith("/") -or $relative.Contains(":") -or $relative -match '(^|/)\.\.(/|$)') {
            throw "Unsafe payload manifest path: $relative"
        }
        if ($expected.ContainsKey($relative)) {
            throw "Duplicate payload manifest path: $relative"
        }
        $expected[$relative] = $Matches[1]
    }

    $actual = @{}
    foreach ($file in Get-ChildItem -LiteralPath $Stage -Recurse -File) {
        $relative = [System.IO.Path]::GetRelativePath($Stage, $file.FullName).Replace([char]92, [char]47)
        if ($relative -notin $IgnoredRelativePaths) {
            $actual[$relative] = Get-Sha256 $file.FullName
        }
    }
    if ($actual.Count -ne $expected.Count) {
        throw "Payload file count mismatch: manifest=$($expected.Count) archive=$($actual.Count)"
    }
    foreach ($relative in $expected.Keys) {
        if (!$actual.ContainsKey($relative) -or $actual[$relative] -ne $expected[$relative]) {
            throw "Payload hash mismatch: $relative"
        }
    }
}

function Get-PayloadManifestHash {
    param(
        [Parameter(Mandatory = $true)][string]$ManifestPath,
        [Parameter(Mandatory = $true)][string]$RelativePath
    )

    $hashes = @(
        foreach ($line in Get-Content -LiteralPath $ManifestPath) {
            if ($line -match '^([0-9a-f]{64})  (.+)$' -and $Matches[2] -ceq $RelativePath) {
                $Matches[1]
            }
        }
    )
    if ($hashes.Count -ne 1) {
        throw "Expected exactly one payload manifest hash for $RelativePath; found $($hashes.Count)."
    }
    return [string]($hashes[0])
}

function Assert-NonOwnedPayloadMatchesUnsignedManifest {
    param(
        [Parameter(Mandatory = $true)][string]$ManifestPath,
        [Parameter(Mandatory = $true)][string]$Stage,
        [Parameter(Mandatory = $true)][string[]]$OwnedPeNames,
        [Parameter(Mandatory = $true)][string[]]$ControlFileNames
    )

    $expected = @{}
    foreach ($line in Get-Content -LiteralPath $ManifestPath) {
        if ($line -notmatch '^([0-9a-f]{64})  (.+)$') {
            throw "Invalid retained unsigned payload manifest line: $line"
        }
        $relative = $Matches[2]
        if ($relative.StartsWith("/") -or $relative.Contains(":") -or
            $relative -match '(^|/)\.\.(/|$)' -or $expected.ContainsKey($relative)) {
            throw "Unsafe or duplicate retained unsigned payload path: $relative"
        }
        $expected[$relative] = $Matches[1]
    }

    $actual = @{}
    foreach ($file in Get-ChildItem -LiteralPath $Stage -Recurse -File) {
        $relative = [System.IO.Path]::GetRelativePath($Stage, $file.FullName).Replace([char]92, [char]47)
        if ($relative -notin $ControlFileNames) {
            $actual[$relative] = Get-Sha256 $file.FullName
        }
    }
    if ($actual.Count -ne $expected.Count) {
        throw "Signing stage file set changed: unsigned=$($expected.Count) signed-stage=$($actual.Count)"
    }
    foreach ($relative in $expected.Keys) {
        if (!$actual.ContainsKey($relative)) {
            throw "Signing stage lost canonical unsigned payload file: $relative"
        }
        if ($relative -in $OwnedPeNames) {
            if ($actual[$relative] -eq $expected[$relative]) {
                throw "Owned PE was not changed by the required signing step: $relative"
            }
        }
        elseif ($actual[$relative] -ne $expected[$relative]) {
            throw "Non-owned payload changed after the reproducibility gate: $relative"
        }
    }
}

function Get-SignTool {
    $command = Get-Command signtool.exe -ErrorAction SilentlyContinue
    if ($command) {
        return $command.Source
    }
    $programFilesX86 = [Environment]::GetEnvironmentVariable("ProgramFiles(x86)")
    $kitsRoot = Join-Path $programFilesX86 "Windows Kits\10\bin"
    $candidate = Get-ChildItem -LiteralPath $kitsRoot -Recurse -File -Filter "signtool.exe" -ErrorAction SilentlyContinue | Where-Object {
        $_.DirectoryName.EndsWith("\x64", [System.StringComparison]::OrdinalIgnoreCase)
    } | Sort-Object FullName -Descending | Select-Object -First 1
    if ($null -eq $candidate) {
        throw "signtool.exe is unavailable."
    }
    return $candidate.FullName
}

function Assert-Unsigned {
    param([Parameter(Mandatory = $true)][string]$Path)

    $signature = Get-AuthenticodeSignature -LiteralPath $Path
    if ($signature.Status -ne [System.Management.Automation.SignatureStatus]::NotSigned) {
        throw "Expected an unsigned file before canonical selection: $Path (status=$($signature.Status))"
    }
}

function Assert-NunchukSignature {
    param([Parameter(Mandatory = $true)][string]$Path)

    if ([string]::IsNullOrWhiteSpace($ExpectedSignerSubject)) {
        throw "ExpectedSignerSubject must be configured for signed releases."
    }
    $signature = Get-AuthenticodeSignature -LiteralPath $Path
    if ($signature.Status -ne [System.Management.Automation.SignatureStatus]::Valid) {
        throw "Authenticode verification failed for $Path (status=$($signature.Status); message=$($signature.StatusMessage))"
    }
    if ($null -eq $signature.SignerCertificate -or $signature.SignerCertificate.Subject -cne $ExpectedSignerSubject) {
        throw "Unexpected Authenticode signer for $Path; expected='$ExpectedSignerSubject' actual='$($signature.SignerCertificate.Subject)'"
    }
    if ($null -eq $signature.TimeStamperCertificate) {
        throw "RFC3161 timestamp signature is missing for $Path."
    }
    $signTool = Get-SignTool
    & $signTool verify /pa /all /v $Path 2>&1 | ForEach-Object { Write-Host $_ }
    if ($LASTEXITCODE -ne 0) {
        throw "signtool verification failed for $Path."
    }
    return $signature
}

$LockFile = [System.IO.Path]::GetFullPath($LockFile)
if (!(Test-Path -LiteralPath $LockFile -PathType Leaf)) {
    throw "Dependency lock is missing: $LockFile"
}
if ($PackageDirectory) {
    $PackageDirectory = [System.IO.Path]::GetFullPath($PackageDirectory)
}
if ($StageDirectory) {
    $StageDirectory = [System.IO.Path]::GetFullPath($StageDirectory)
}
$ownedPeNames = @("nunchuk-qt.exe", "qt6keychain.dll", "hwi.exe")
$gateFileName = ".reproducibility-gate.json"
$unsignedManifestFileName = ".unsigned-payload-manifest.sha256"
$controlFileNames = @($gateFileName, $unsignedManifestFileName)

if ($Mode -in @("ValidateUnsigned", "Prepare")) {
    if (!$PackageDirectory -or !$StageDirectory) {
        throw "PackageDirectory and StageDirectory are required for $Mode."
    }
    $metadata = Get-PackageMetadata $PackageDirectory "unsigned"
    $archivePath = Resolve-PackageFile $PackageDirectory ([string]$metadata.archive)
    $installerPath = Resolve-PackageFile $PackageDirectory ([string]$metadata.installer)
    $payloadManifestPath = Join-Path $PackageDirectory "payload-manifest.sha256"
    $packageMetadataPath = Join-Path $PackageDirectory "build-metadata.json"
    Assert-Unsigned $installerPath
    Expand-CheckedZip $archivePath $StageDirectory
    Assert-PayloadManifest $payloadManifestPath $StageDirectory
    foreach ($ownedPeName in $ownedPeNames) {
        Assert-Unsigned (Join-Path $StageDirectory $ownedPeName)
    }
    if ($Mode -eq "Prepare") {
        Copy-Item -LiteralPath $payloadManifestPath -Destination (Join-Path $StageDirectory $unsignedManifestFileName) -Force
        $gate = [ordered]@{
            schemaVersion = 1
            platform = "windows-x64-msvc2022"
            sourceCommit = $ExpectedCommit.ToLowerInvariant()
            sourceDateEpoch = $SourceDateEpoch
            releaseVersion = $ReleaseVersion
            dependencyLockSha256 = (Get-Sha256 $LockFile)
            replicas = 2
            comparison = "byte-for-byte before signing"
            canonicalReplica = "a"
            ownedPeUnsignedSha256 = @($ownedPeNames | ForEach-Object {
                [ordered]@{
                    name = $_
                    sha256 = (Get-PayloadManifestHash $payloadManifestPath $_)
                }
            })
            unsignedArtifacts = [ordered]@{
                archive = [ordered]@{
                    name = [string]$metadata.archive
                    sha256 = (Get-Sha256 $archivePath)
                    size = (Get-Item -LiteralPath $archivePath).Length
                }
                installer = [ordered]@{
                    name = [string]$metadata.installer
                    sha256 = (Get-Sha256 $installerPath)
                    size = (Get-Item -LiteralPath $installerPath).Length
                }
                payloadManifest = [ordered]@{
                    name = "payload-manifest.sha256"
                    sha256 = (Get-Sha256 $payloadManifestPath)
                    size = (Get-Item -LiteralPath $payloadManifestPath).Length
                }
                packageMetadata = [ordered]@{
                    name = "build-metadata.json"
                    sha256 = (Get-Sha256 $packageMetadataPath)
                    size = (Get-Item -LiteralPath $packageMetadataPath).Length
                }
            }
        }
        $gatePath = Join-Path $StageDirectory $gateFileName
        Write-Utf8NoBom $gatePath ((($gate | ConvertTo-Json -Depth 8)) + [char]10)
        Write-Host "Trusted unsigned reproducibility gate: $(Get-Sha256 $gatePath)"
    }
    Write-Host "$Mode verification completed for $PackageDirectory"
    return
}

if ($Mode -eq "VerifyOwnedPe") {
    if (!$StageDirectory) {
        throw "StageDirectory is required for VerifyOwnedPe."
    }
    $gatePath = Join-Path $StageDirectory $gateFileName
    if (!(Test-Path -LiteralPath $gatePath -PathType Leaf)) {
        throw "Trusted unsigned reproducibility gate is missing before PE verification."
    }
    $gateHash = Get-Sha256 $gatePath
    if ($ExpectedReproducibilityGateSha256 -notmatch '^[0-9a-fA-F]{64}$' -or
        $gateHash -cne $ExpectedReproducibilityGateSha256.ToLowerInvariant()) {
        throw "Trusted unsigned reproducibility gate changed before PE verification."
    }
    $gate = Get-Content -LiteralPath $gatePath -Raw | ConvertFrom-Json
    $unsignedManifestPath = Join-Path $StageDirectory $unsignedManifestFileName
    if (!(Test-Path -LiteralPath $unsignedManifestPath -PathType Leaf) -or
        (Get-Sha256 $unsignedManifestPath) -cne [string]$gate.unsignedArtifacts.payloadManifest.sha256) {
        throw "Retained canonical unsigned payload manifest is missing or changed."
    }
    Assert-NonOwnedPayloadMatchesUnsignedManifest `
        -ManifestPath $unsignedManifestPath `
        -Stage $StageDirectory `
        -OwnedPeNames $ownedPeNames `
        -ControlFileNames $controlFileNames
    foreach ($ownedPeName in $ownedPeNames) {
        [void](Assert-NunchukSignature (Join-Path $StageDirectory $ownedPeName))
    }
    Write-Host "Nunchuk-owned PE signature verification: PASS"
    return
}

if (!$PackageDirectory -or !$StageDirectory) {
    throw "PackageDirectory and StageDirectory are required for Finalize."
}
$metadata = Get-PackageMetadata $PackageDirectory "signed"
$archivePath = Resolve-PackageFile $PackageDirectory ([string]$metadata.archive)
$installerPath = Resolve-PackageFile $PackageDirectory ([string]$metadata.installer)
$gatePath = Join-Path $StageDirectory $gateFileName
if (!(Test-Path -LiteralPath $gatePath -PathType Leaf)) {
    throw "Trusted unsigned reproducibility gate is missing from the signing stage."
}
$gateHash = Get-Sha256 $gatePath
if ($ExpectedReproducibilityGateSha256 -notmatch '^[0-9a-fA-F]{64}$' -or
    $gateHash -cne $ExpectedReproducibilityGateSha256.ToLowerInvariant()) {
    throw "Trusted unsigned reproducibility gate digest mismatch: expected=$ExpectedReproducibilityGateSha256 actual=$gateHash"
}
$gate = Get-Content -LiteralPath $gatePath -Raw | ConvertFrom-Json
if ([int]$gate.schemaVersion -ne 1 -or
    [string]$gate.platform -cne "windows-x64-msvc2022" -or
    [string]$gate.sourceCommit -cne $ExpectedCommit.ToLowerInvariant() -or
    [long]$gate.sourceDateEpoch -ne $SourceDateEpoch -or
    [string]$gate.releaseVersion -cne $ReleaseVersion -or
    [string]$gate.dependencyLockSha256 -cne (Get-Sha256 $LockFile) -or
    [int]$gate.replicas -ne 2 -or
    [string]$gate.comparison -cne "byte-for-byte before signing" -or
    [string]$gate.canonicalReplica -cne "a") {
    throw "Trusted unsigned reproducibility gate metadata does not match the final release inputs."
}
$expectedGateArtifacts = [ordered]@{
    archive = "nunchuk-windows-x64-v$ReleaseVersion-unsigned.zip"
    installer = "nunchuk-windows-x64-v$ReleaseVersion-unsigned-setup.exe"
    payloadManifest = "payload-manifest.sha256"
    packageMetadata = "build-metadata.json"
}
foreach ($entry in $expectedGateArtifacts.GetEnumerator()) {
    $entryKey = [string]$entry.Key
    $gateArtifactProperty = $gate.unsignedArtifacts.PSObject.Properties[$entryKey]
    if ($null -eq $gateArtifactProperty) {
        throw "Trusted unsigned gate artifact is missing: $($entry.Key)"
    }
    $gateArtifact = $gateArtifactProperty.Value
    if ([string]$gateArtifact.name -cne [string]$entry.Value -or
        [string]$gateArtifact.sha256 -notmatch '^[0-9a-f]{64}$' -or
        [long]$gateArtifact.size -le 0) {
        throw "Invalid trusted unsigned gate artifact: $($entry.Key)"
    }
}
$unsignedManifestPath = Join-Path $StageDirectory $unsignedManifestFileName
if (!(Test-Path -LiteralPath $unsignedManifestPath -PathType Leaf) -or
    (Get-Sha256 $unsignedManifestPath) -cne [string]$gate.unsignedArtifacts.payloadManifest.sha256) {
    throw "Retained canonical unsigned payload manifest is missing or changed during finalization."
}
$gateOwnedPe = @($gate.ownedPeUnsignedSha256)
if ($gateOwnedPe.Count -ne $ownedPeNames.Count) {
    throw "Trusted gate does not contain the exact unsigned owned PE hash set."
}
$unsignedOwnedHashes = @{}
foreach ($ownedPeName in $ownedPeNames) {
    $gateOwnedPeMatches = @($gateOwnedPe | Where-Object { ([string]$_.name) -ceq $ownedPeName })
    $retainedUnsignedHash = Get-PayloadManifestHash $unsignedManifestPath $ownedPeName
    if ($gateOwnedPeMatches.Count -ne 1 -or
        [string]$gateOwnedPeMatches[0].sha256 -cne $retainedUnsignedHash) {
        throw "Trusted unsigned owned PE hash mismatch: $ownedPeName"
    }
    $unsignedOwnedHashes[$ownedPeName] = $retainedUnsignedHash
}
Assert-NonOwnedPayloadMatchesUnsignedManifest `
    -ManifestPath $unsignedManifestPath `
    -Stage $StageDirectory `
    -OwnedPeNames $ownedPeNames `
    -ControlFileNames $controlFileNames
Assert-PayloadManifest (Join-Path $PackageDirectory "payload-manifest.sha256") $StageDirectory -IgnoredRelativePaths $controlFileNames
$ownedPeSignatures = [ordered]@{}
foreach ($ownedPeName in $ownedPeNames) {
    $ownedPeSignatures[$ownedPeName] = Assert-NunchukSignature (Join-Path $StageDirectory $ownedPeName)
}
$applicationSignature = $ownedPeSignatures["nunchuk-qt.exe"]
$installerSignature = Assert-NunchukSignature $installerPath

$zipVerificationStage = Join-Path $PackageDirectory ".zip-verification"
Expand-CheckedZip $archivePath $zipVerificationStage
try {
    Assert-PayloadManifest (Join-Path $PackageDirectory "payload-manifest.sha256") $zipVerificationStage
    foreach ($ownedPeName in $ownedPeNames) {
        $zippedOwnedPe = Join-Path $zipVerificationStage $ownedPeName
        [void](Assert-NunchukSignature $zippedOwnedPe)
        $stagedOwnedPe = Join-Path $StageDirectory $ownedPeName
        if ((Get-Sha256 $zippedOwnedPe) -ne (Get-Sha256 $stagedOwnedPe)) {
            throw "Signed owned PE in the canonical ZIP differs from staging: $ownedPeName"
        }
    }
}
finally {
    if (Test-Path -LiteralPath $zipVerificationStage) {
        Remove-Item -LiteralPath $zipVerificationStage -Recurse -Force
    }
}

$manifest = [ordered]@{
    schemaVersion = 1
    platform = "windows-x64-msvc2022"
    sourceCommit = $ExpectedCommit.ToLowerInvariant()
    sourceDateEpoch = $SourceDateEpoch
    releaseVersion = $ReleaseVersion
    dependencyLockSha256 = (Get-Sha256 $LockFile)
    reproducibilityGate = [ordered]@{
        metadataSha256 = $gateHash
        replicas = [int]$gate.replicas
        comparison = [string]$gate.comparison
        canonicalReplica = [string]$gate.canonicalReplica
        unsignedArchive = [ordered]@{
            name = [string]$gate.unsignedArtifacts.archive.name
            sha256 = [string]$gate.unsignedArtifacts.archive.sha256
            size = [long]$gate.unsignedArtifacts.archive.size
        }
        unsignedInstaller = [ordered]@{
            name = [string]$gate.unsignedArtifacts.installer.name
            sha256 = [string]$gate.unsignedArtifacts.installer.sha256
            size = [long]$gate.unsignedArtifacts.installer.size
        }
        unsignedPayloadManifest = [ordered]@{
            name = [string]$gate.unsignedArtifacts.payloadManifest.name
            sha256 = [string]$gate.unsignedArtifacts.payloadManifest.sha256
            size = [long]$gate.unsignedArtifacts.payloadManifest.size
        }
        unsignedPackageMetadata = [ordered]@{
            name = [string]$gate.unsignedArtifacts.packageMetadata.name
            sha256 = [string]$gate.unsignedArtifacts.packageMetadata.sha256
            size = [long]$gate.unsignedArtifacts.packageMetadata.size
        }
    }
    signing = [ordered]@{
        provider = "Azure Artifact Signing"
        signerSubject = $applicationSignature.SignerCertificate.Subject
        signerThumbprint = $applicationSignature.SignerCertificate.Thumbprint
        timestamped = $true
        ownedPe = $ownedPeNames
        ownedPeAuthenticode = @($ownedPeNames | ForEach-Object {
            [ordered]@{
                name = $_
                unsignedSha256 = $unsignedOwnedHashes[$_]
                sha256 = (Get-Sha256 (Join-Path $StageDirectory $_))
                signedSha256 = (Get-Sha256 (Join-Path $StageDirectory $_))
                signerSubject = $ownedPeSignatures[$_].SignerCertificate.Subject
                signerThumbprint = $ownedPeSignatures[$_].SignerCertificate.Thumbprint
            }
        })
        installer = [string]$metadata.installer
    }
    artifacts = @(
        [ordered]@{
            name = [string]$metadata.archive
            sha256 = (Get-Sha256 $archivePath)
            size = (Get-Item -LiteralPath $archivePath).Length
            contents = "signed Nunchuk application and locked runtime payload"
        },
        [ordered]@{
            name = [string]$metadata.installer
            sha256 = (Get-Sha256 $installerPath)
            size = (Get-Item -LiteralPath $installerPath).Length
            authenticodeSigner = $installerSignature.SignerCertificate.Subject
            authenticodeThumbprint = $installerSignature.SignerCertificate.Thumbprint
        }
    )
}
$manifestPath = Join-Path $PackageDirectory "release-manifest.json"
Write-Utf8NoBom $manifestPath (($manifest | ConvertTo-Json -Depth 10) + [char]10)

$checksums = @(
    "$(Get-Sha256 $archivePath)  $([System.IO.Path]::GetFileName($archivePath))",
    "$(Get-Sha256 $installerPath)  $([System.IO.Path]::GetFileName($installerPath))",
    "$(Get-Sha256 $manifestPath)  $([System.IO.Path]::GetFileName($manifestPath))"
)
Write-Utf8NoBom (Join-Path $PackageDirectory "SHA256SUMS.txt") (($checksums -join [char]10) + [char]10)

Write-Host "Final signed Windows release verification: PASS"
