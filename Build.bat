@echo off

SET BuildDir=Build
SET SourceDir=..\Code

SET OptLevel=-Od
SET ConfigurationFlags=-DDEBUG_BUILD=1
SET CompilerFlags=-Zi -nologo -EHsc
SET DisabledWarnings=
SET CommonCompilerFlags=%CompilerFlags% %ConfigurationFlags% %OptLevel% %DisabledWarnings%

SET MainLinkerLibs=-link kernel32.lib user32.lib Winmm.lib d3d11.lib dxgi.lib

IF NOT EXIST %BuildDir% mkdir %BuildDir%

pushd %BuildDir%
cl -FeChess %CommonCompilerFlags% %SourceDir%\Chess.cpp %MainLinkerLibs%
popd
