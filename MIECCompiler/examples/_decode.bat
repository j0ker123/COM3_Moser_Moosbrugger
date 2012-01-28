echo on
echo # decode all ... > _decode.log
echo. >> _decode.log

for /f %%a IN ('dir /b *.iex') do (
	echo # decode %%a ... >> _decode.log
	"../Decoder/DecoderProl16.exe" %%a > %%a.asm
)