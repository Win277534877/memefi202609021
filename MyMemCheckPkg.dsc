[Defines]
  PLATFORM_VERSION             = 1.0
  DSC_SPECIFICATION            = 0x00010005
  OUTPUT_DIRECTORY             = Build/MyMemCheckPkg
  SUPPORTED_ARCHITECTURES      = X64
  BUILD_TARGETS                = RELEASE|DEBUG

[Packages]
  MdePkg/MdePkg.dec
  MyMemCheckPkg/MyMemCheckPkg.dec

[LibraryClasses]
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf

[Components]
  MyMemCheckPkg/MemFilterApp/MemFilterApp.inf
