echo on
rem Copy required DLLs
copy "wxWidgets\3.1.3\lib\vc14x_x64_dll\wxbase313u_vc14x_x64.dll" "..\Release\"
copy "wxWidgets\3.1.3\lib\vc14x_x64_dll\wxmsw313u_core_vc14x_x64.dll" "..\Release\"
copy "Fronter\Resources\converter.ico" "..\Release\"
copy "Fronter\Resources\msvcp140_codecvt_ids.dll" "..\Release\"
copy "Fronter\Resources\vcruntime140_1.dll" "..\Release\"
copy "Fronter\Resources\VC_redist.x64.exe" "..\Release\"

rem And our native Localizations
mkdir "..\Release\Configuration"
copy "Fronter\Resources\*.yml" "..\Release\Configuration\"
