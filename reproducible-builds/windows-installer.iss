#ifndef AppVersion
  #error AppVersion is required
#endif
#ifndef AppVersionNumeric
  #error AppVersionNumeric is required
#endif
#ifndef OutputBaseFilename
  #error OutputBaseFilename is required
#endif
#ifndef OutputDir
  #error OutputDir is required
#endif

#define MyAppName "Nunchuk"
#define MyAppExeName "nunchuk-qt.exe"

[Setup]
AppId={{8F1B9C2A-4D3E-4A21-9C7B-1E2F3A4B5C6D}
AppName={#MyAppName}
AppVersion={#AppVersion}
AppPublisher=Nunchuk
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
OutputDir={#OutputDir}
OutputBaseFilename={#OutputBaseFilename}
Compression=lzma2/max
CompressionThreads=1
LZMANumBlockThreads=1
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0
PrivilegesRequired=admin
WizardStyle=modern
CloseApplications=yes
RestartApplications=no
TimeStampsInUTC=yes
VersionInfoVersion={#AppVersionNumeric}
VersionInfoCompany=Nunchuk
VersionInfoDescription=Nunchuk Bitcoin Wallet Installer
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#AppVersionNumeric}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; package_windows.ps1 writes this include in ordinal path order. Each entry
; uses notimestamp; Inno Setup explicitly provides that flag for reproducible
; builds and forbids combining it with recursesubdirs.
#include "windows-payload-files.iss"

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
