echo on
echo # compile all ... > _compile.log
echo. >> _compile.log

for /f %%a IN ('dir /b *.miec') do (
	echo # compiling %%a ... > %%a.log
	"../Debug/MIECCompiler.exe" %%a >> %%a.log
	type %%a.log >> _compile.log
)