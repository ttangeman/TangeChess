@echo off

pushd "Code/Shaders"
rm *.cso
fxc /T vs_4_0 /nologo /Fo default.vs.cso default.vs.hlsl
fxc /T ps_4_0 /nologo /Fo fullclear.ps.cso fullclear.ps.hlsl
fxc /T ps_4_0 /nologo /Fo textured.ps.cso textured.ps.hlsl
popd