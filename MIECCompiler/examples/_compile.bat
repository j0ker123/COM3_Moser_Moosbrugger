for /f %%a IN ('dir /b *.miec') do (
	echo # compiling... > %%a.log
	"../Debug/MIECCompiler.exe" %%a >> %%a.log
)