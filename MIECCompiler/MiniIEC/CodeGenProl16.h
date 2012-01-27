///////////////////////////////////////////////////////////////////////////////////
// Class CodeGenProl16 is used to generate executable code for the PROL16 processor
//

#ifndef CODEGENPROL16_H_
#define CODEGENPROL16_H_

#include "GlobalDef.h"

namespace MIEC {

class CodeGenProl16 {
public:
    CodeGenProl16();
    ~CodeGenProl16();

    // data transfer
    WORD LoadI(BYTE aReg, WORD aData);
    void Load(BYTE aRegA, BYTE aRegB);
    void Store(BYTE aRegA, BYTE aRegB);
    void Move(BYTE aRegA, BYTE aRegB);

    // arithmetic operations
    void Add(BYTE aRegA, BYTE aRegB);
    void AddC(BYTE aRegA, BYTE aRegB);
    void Sub(BYTE aRegA, BYTE aRegB);
    void SubC(BYTE aRegA, BYTE aRegB);
    void Comp(BYTE aRegA, BYTE aRegB);
    void Inc(BYTE aRegA);
    void Dec(BYTE aRegA);

    // logical operations
    void And(BYTE aRegA, BYTE aRegB);
    void Or(BYTE aRegA, BYTE aRegB);
    void XOr(BYTE aRegA, BYTE aRegB);
    void Not(BYTE aRegA);
    void ShL(BYTE aRegA);
    void ShR(BYTE aRegA);
    void ShLC(BYTE aRegA);
    void ShRC(BYTE aRegA);

    // program branch
    void Jump(BYTE aRegA);
    void JumpC(BYTE aRegA);
    void JumpZ(BYTE aRegA);
    void Nop();
    void Sleep();

    // gets the actual code-position
    WORD GetCodePosition() const;
    // stores the address specified by aAddress at the code-position specified by aCodePos
    void SetAddress(WORD aCodePos, WORD aAddress);

    // output to console
    void PrintInt(BYTE aRegA);
    void PrintString(BYTE aRegA);

    // write extecutable
    void WriteIex(std::wstring const& arFileName);

private:
    void AppendCommand(BYTE aCommand, BYTE aRegA, BYTE aRegB);
    void AppendWord(WORD aWord);

public:
    static const int cRegisterCount;

private:
	CodeGenProl16(const CodeGenProl16&);
	CodeGenProl16& operator= (const CodeGenProl16&);

    BYTE* mpBuffer;
    unsigned int mBufLength;
    WORD mBufPos;
    const unsigned int cGrowSize;
};

} // namespace MIEC

#endif // #ifndef CODEGENPROL16_H_
