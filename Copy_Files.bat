echo on
rem Copy required DLLs
copy "wx\3.1.3\lib\vc14x_x64_dll\wxbase313u_vc14x_x64.dll" "Release\"
copy "wx\3.1.3\lib\vc14x_x64_dll\wxmsw313u_core_vc14x_x64.dll" "Release\"
copy "Fronter\Resources\converter.ico" "Release\"

rem Create Configuration
del "Release\Configuration" /Q
rmdir "Release\Configuration" /S /Q
xcopy "Fronter\Configuration" "Release\Configuration" /Y /E /I


del "Release\CK2ToEU4" /Q
rmdir "Release\CK2ToEU4" /S /Q
xcopy "..\CK2ToEU4\Release\CK2ToEU4" "Release\CK2ToEU4" /Y /E /I
