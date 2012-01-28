echo on
echo # execute all ... > _execute.log
echo. >> _execute.log

for /f %%a IN ('dir /b *.iex') do (
	echo # executing %%a ... > %%a.log
	"../Decoder/VMProl16.exe" %%a >> %%a.log
	type %%a.log >> _execute.log
	echo. >> _execute.log
)