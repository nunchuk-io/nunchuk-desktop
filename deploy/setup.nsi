Name "Nunchuk"

RequestExecutionLevel highest
SetCompressor /SOLID lzma
SetDatablockOptimize on
SetDateSave off

# Uncomment these lines when investigating reproducibility errors
#SetCompress off
#SetDatablockOptimize off

!define NAME "Nunchuk"
!define COMPANY "Nunchuk Inc."
!define URL "https://nunchuk.io/"
!define MAIN_EXECUTABLE "nunchuk-qt.exe"
!define UNINSTALLER_FILENAME "uninstall.exe"
!define BUILD_DIR "${PROJECT_DIR}/build"
!define BINARY_DIR "${BUILD_DIR}/output"
!define REGKEY "SOFTWARE\$(^Name)"

OutFile "${BUILD_DIR}/artifacts/nunchuk-windows-v${VERSION}-x64-setup-unsigned.exe"

# MUI Symbol Definitions
!define MUI_ICON "${PROJECT_DIR}/res/Nunchuk.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${PROJECT_DIR}/deploy/nsis-wizard.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${PROJECT_DIR}/deploy/nsis-header.bmp"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_REGISTRY_KEY ${REGKEY}
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${NAME}"
!define MUI_FINISHPAGE_RUN "$WINDIR\explorer.exe"
!define MUI_FINISHPAGE_RUN_PARAMETERS $INSTDIR\nunchuk-qt.exe
!define MUI_UNICON "${PROJECT_DIR}/res/Nunchuk.ico"
#!define MUI_UNWELCOMEFINISHPAGE_BITMAP ""
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

# Included files
!include Sections.nsh
!include MUI2.nsh
!include x64.nsh
!include nsDialogs.nsh

# Variables
Var StartMenuGroup
Var /GLOBAL FoundOldVersionFlag ; 0 = Not Found, 1 = Found

# Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_FUNCTION ShowOldVersionWarning
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE English

# Installer attributes
InstallDir $PROGRAMFILES64\Nunchuk
CRCCheck force
XPStyle on
BrandingText " "
ShowInstDetails show
VIProductVersion ${VERSION}.0
VIAddVersionKey ProductName "${NAME}"
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey CompanyName "${COMPANY}"
VIAddVersionKey CompanyWebsite "${URL}"
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey FileDescription "Installer for ${NAME}"
VIAddVersionKey LegalCopyright "Copyright (C) 2020-2025 ${COMPANY}"
InstallDirRegKey HKCU "${REGKEY}" Path
ShowUninstDetails show

# Installer sections
Section -Main SEC0000
    SetOutPath $INSTDIR
    SetOverwrite on
  
    File /r "${BINARY_DIR}\*.*"
    
    WriteRegStr HKCU "${REGKEY}\Components" Main 1
SectionEnd

Section -post SEC0001
    WriteRegStr HKCU "${REGKEY}" Path $INSTDIR
    SetOutPath $INSTDIR
    WriteUninstaller $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    CreateDirectory $SMPROGRAMS\$StartMenuGroup
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\$(^Name).lnk" $INSTDIR\nunchuk-qt.exe
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk" $INSTDIR\uninstall.exe
    CreateShortcut "$DESKTOP\$(^Name).lnk" "$INSTDIR\nunchuk-qt.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "${NAME} version ${VERSION}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" Publisher "${COMPANY}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" URLInfoAbout "${URL}"
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon $INSTDIR\nunchuk-qt.exe
    WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString $INSTDIR\uninstall.exe
    WriteRegDWORD HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd

# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKCU "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    GoTo done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend

# Uninstaller sections
Section /o -un.Main UNSEC0000
    Delete /REBOOTOK $INSTDIR\nunchuk-qt.exe
    RMDir /r /REBOOTOK "$INSTDIR\*.*"
    DeleteRegValue HKCU "${REGKEY}\Components" Main
SectionEnd

Section -un.post UNSEC0001
    DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\$(^Name).lnk"
    Delete /REBOOTOK "$SMSTARTUP\$(^Name).lnk"
    Delete "$DESKTOP\$(^Name).lnk"
   
    DeleteRegValue HKCU "${REGKEY}" StartMenuGroup
    DeleteRegValue HKCU "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKCU "${REGKEY}\Components"
    DeleteRegKey /IfEmpty HKCU "${REGKEY}"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RmDir /REBOOTOK $INSTDIR
    Push $R0
    StrCpy $R0 $StartMenuGroup 1
    StrCmp $R0 ">" no_smgroup
no_smgroup:
    Pop $R0
SectionEnd

# Installer functions
Function .onInit
    InitPluginsDir
    ${If} ${RunningX64}
      ; disable registry redirection (enable access to 64-bit portion of registry)
      SetRegView 64
    ${Else}
      MessageBox MB_OK|MB_ICONSTOP "Cannot install 64-bit version on a 32-bit system."
      Abort
    ${EndIf}
  
  # --- Check for old version using Inno Setup ---
    IfFileExists "$PROGRAMFILES32\Nunchuk\unins000.exe" OldVerDetected SkipAppDataCheck

SkipAppDataCheck:
    ${IfNot} $FoundOldVersionFlag == 1
        IfFileExists "$LOCALAPPDATA\Programs\Nunchuk\unins000.exe" OldVerDetected NoOldVerDetected
    ${EndIf}

OldVerDetected:
    StrCpy $FoundOldVersionFlag 1
    DetailPrint "Old Inno Setup version detected. Will warn user at finish."
    Goto CheckDone

NoOldVerDetected:
    DetailPrint "No old Inno Setup version detected."

CheckDone:
    DetailPrint "Proceeding with installation."
    # --> End of check <--
FunctionEnd

# Uninstaller functions
Function un.onInit
    ReadRegStr $INSTDIR HKCU "${REGKEY}" Path
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
    !insertmacro SELECT_UNSECTION Main ${UNSEC0000}
FunctionEnd

Function un.onGUIEnd
    MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 \
      "Do you also want to permanently remove the database?" IDNO SkipRemove
    RMDir /r "$APPDATA\Nunchuk"
SkipRemove:
  
FunctionEnd

Function .onGUIEnd
    ${If} $FoundOldVersionFlag == 1
        MessageBox MB_OK|MB_ICONINFORMATION \
          "Setup has finished successfully.$\n$\nAn older version of Nunchuk was also detected on your system.$\n$\nIt is recommended to uninstall the old version manually using 'Add/Remove Programs' (Apps & Features) to avoid potential conflicts or duplicate entries."
    ${EndIf}
FunctionEnd
