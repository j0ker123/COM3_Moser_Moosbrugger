for /f %%a IN ('dir /b *.iex') do (
	"../Decoder/DecoderProl16.exe" %%a > %%a.asm
)