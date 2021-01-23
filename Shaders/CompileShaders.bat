@echo off

del *.cso
fxc /T vs_4_0 /nologo /Fo Default.vs.cso Default.vs.hlsl
fxc /T ps_4_0 /nologo /Fo FullClear.ps.cso FullClear.ps.hlsl
fxc /T ps_4_0 /nologo /Fo Textured.ps.cso Textured.ps.hlsl