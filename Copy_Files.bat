echo on
rem Copy required DLLs
copy "wx\3.1.3\lib\vc14x_x64_dll\wxbase313u_vc14x_x64.dll" "..\Release\"
copy "wx\3.1.3\lib\vc14x_x64_dll\wxmsw313u_core_vc14x_x64.dll" "..\Release\"
copy "Fronter\Resources\converter.ico" "..\Release\"
copy "Fronter\Resources\msvcp140_codecvt_ids.dll" "..\Release\"
copy "Fronter\Resources\vcruntime140_1.dll" "..\Release\"
copy "Fronter\Resources\VC_redist.x64.exe" "..\Release\"

rem Create Configuration
rem del "Release\Configuration" /Q
rem rmdir "Release\Configuration" /S /Q
rem xcopy "Fronter\Configuration" "Release\Configuration" /Y /E /I


rem del "Release\CK2ToEU4" /Q
rem rmdir "Release\CK2ToEU4" /S /Q
rem xcopy "..\CK2ToEU4\Release\CK2ToEU4" "Release\CK2ToEU4" /Y /E /I
