for /f %%a IN ('dir /b *.iex') do (
	echo # executing... > %%a.log
	"../Decoder/VMProl16.exe" %%a >> %%a.log
)