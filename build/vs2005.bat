rmdir /s /q vc2005
rmdir /s /q build
premake4 clean
premake4 vs2005
rename build vs2005