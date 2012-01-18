///////////////////////////////////////////////////////////////////////////////////
// Class CodeGenProl16 is used to generate executable code for the PROL16 processor
//

#include <string>
#include "CodeGenProl16.h"

#ifdef _DEBUG
   #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
   #define new DEBUG_NEW
#endif

namespace MIEC 
{
   const int CodeGenProl16::cRegisterCount = 8;

   CodeGenProl16::CodeGenProl16()
      : cGrowSize(1024)
   {
      mBufLength = cGrowSize;
      mpBuffer = new BYTE[mBufLength];
      mBufPos = 0;
   }
   
   CodeGenProl16::~CodeGenProl16()
   {
      delete mpBuffer;
   }

   // data transfer
   WORD CodeGenProl16::LoadI(BYTE aReg, WORD aData)
   {
      AppendCommand(OPCODE_LOADI, aReg, 0);
      WORD codePosData = mBufPos;
      AppendWord(aData);
      return codePosData;
   }

   void CodeGenProl16::Load(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_LOAD, aRegA, aRegB);
   }

   void CodeGenProl16::Store(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_STORE, aRegA, aRegB);
   }

   void CodeGenProl16::Move(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_MOVE, aRegA, aRegB);
   }

   // arithmetic operations
   void CodeGenProl16::Add(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_ADD, aRegA, aRegB);
   }

   void CodeGenProl16::AddC(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_ADDC, aRegA, aRegB);
   }

   void CodeGenProl16::Sub(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_SUB, aRegA, aRegB);
   }

   void CodeGenProl16::SubC(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_SUBC, aRegA, aRegB);
   }

   void CodeGenProl16::Comp(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_COMP, aRegA, aRegB);
   }

   void CodeGenProl16::Inc(BYTE aRegA)
   {
      AppendCommand(OPCODE_INC, aRegA, 0);
   }

   void CodeGenProl16::Dec(BYTE aRegA)
   {
      AppendCommand(OPCODE_DEC, aRegA, 0);
   }

   // logical operations
   void CodeGenProl16::And(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_AND, aRegA, aRegB);
   }

   void CodeGenProl16::Or(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_OR, aRegA, aRegB);
   }

   void CodeGenProl16::XOr(BYTE aRegA, BYTE aRegB)
   {
      AppendCommand(OPCODE_XOR, aRegA, aRegB);
   }

   void CodeGenProl16::Not(BYTE aRegA)
   {
      AppendCommand(OPCODE_NOT, aRegA, 0);
   }

   void CodeGenProl16::ShL(BYTE aRegA)
   {
      AppendCommand(OPCODE_SHL, aRegA, 0);
   }

   void CodeGenProl16::ShR(BYTE aRegA)
   {
      AppendCommand(OPCODE_SHR, aRegA, 0);
   }

   void CodeGenProl16::ShLC(BYTE aRegA)
   {
      AppendCommand(OPCODE_SHLC, aRegA, 0);
   }

   void CodeGenProl16::ShRC(BYTE aRegA)
   {
      AppendCommand(OPCODE_SHRC, aRegA, 0);
   }

   // program branch
   void CodeGenProl16::Jump(BYTE aRegA)
   {
      AppendCommand(OPCODE_JUMP, aRegA, 0);
   }

   void CodeGenProl16::JumpC(BYTE aRegA)
   {
      AppendCommand(OPCODE_JUMPC, aRegA, 0);
   }

   void CodeGenProl16::JumpZ(BYTE aRegA)
   {
      AppendCommand(OPCODE_JUMPZ, aRegA, 0);
   }

   void CodeGenProl16::Nop()
   {
      AppendCommand(OPCODE_NOP, 0, 0);
   }

   void CodeGenProl16::Sleep()
   {
      AppendCommand(OPCODE_SLEEP, 0, 0);
   }

   // gets the actual code-position
   WORD CodeGenProl16::GetCodePosition() const
   {
      return mBufPos;
   }

   // stores the address specified by aAddress at the code-position specified by aCodePos
   void CodeGenProl16::SetAddress(WORD aCodePos, WORD aAddress)
   {
      if (aCodePos < mBufPos - sizeof(WORD))
      {
         *((WORD*)(mpBuffer + aCodePos)) = aAddress;
      }
      else _ASSERT(false);
   }

   // output to console
   void CodeGenProl16::PrintInt(BYTE aRegA)
   {
      AppendCommand(OPCODE_PRINTINT, aRegA, 0);
   }

   void CodeGenProl16::PrintString(BYTE aRegA)
   {
      AppendCommand(OPCODE_PRINTSTR, aRegA, 0);
   }

   // write extecutable
   void CodeGenProl16::WriteIex(std::wstring const& arFileName)
   {
      FILE *stream = _wfopen(arFileName.data(), L"wb");
      if (stream != NULL)
      {
         int written = fwrite(mpBuffer, sizeof(BYTE), mBufPos, stream);
         if (written != mBufPos)
         {
            wprintf(L"write file '%s' failed: %s", arFileName, __wcserror(NULL));
         }
         fclose(stream);
      }
      else
      {
         wprintf(L"open file '%s' failed: %s", arFileName, __wcserror(NULL));
      }
   }

   void CodeGenProl16::AppendCommand(BYTE aCommand, BYTE aRegA, BYTE aRegB)
   {
      if (aRegA >= cRegisterCount)
      {
         wprintf(L"Register overflow Ra: %d\n", (int)aRegA);
      }
      if (aRegB >= cRegisterCount)
      {
         wprintf(L"Register overflow Rb: %d\n", (int)aRegB);
      }

      AppendWord((aCommand << 10) | (aRegA << 5) | aRegB);
   }

   void CodeGenProl16::AppendWord(WORD aWord)
   {
      if (mBufPos + sizeof(WORD) > mBufLength)
      {
         BYTE* pNewBuffer = new BYTE[mBufLength + cGrowSize];
         memcpy(pNewBuffer, mpBuffer, mBufLength);
         delete mpBuffer;
         mpBuffer = pNewBuffer;
         mBufLength += cGrowSize;
      }
      *((WORD*)(mpBuffer + mBufPos)) = aWord;
      mBufPos += sizeof(WORD);
   }
} // namespace


