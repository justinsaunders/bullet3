rmdir /s /q vc2010
rmdir /s /q build
premake4 clean
premake4 vs2010
rename build vs2010