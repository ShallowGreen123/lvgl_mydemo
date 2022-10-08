@echo off

cd /d  %~dp0

set FONT_PATH=fonts
set OUT_PATH=out

rmdir /s /q %OUT_PATH%
mkdir %OUT_PATH%

for /r %FONT_PATH% %%f in (*.c) do (
	echo Convert: %%~nxf
	LVGL-Font-Binary.exe "%%f" "%OUT_PATH%"
)
