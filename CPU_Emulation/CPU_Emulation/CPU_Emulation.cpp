#include <iostream>
#include <fstream>
#include<iomanip>

using namespace std;
// ----------------------------- Instructions ----------------------------

//Data transfer operations.
void IN(unsigned short& IR, unsigned char regs[], unsigned char& eflags, ifstream& txt_file)
{
    unsigned char reg_num = (IR & 0b0000011100000000) >> 8;

    unsigned char temp = 0;
    if (!txt_file.get((char&)temp))
    {
        eflags = eflags | 0b00000100;
        return;
    }

    regs[reg_num] = temp;

    if (txt_file.eof()) eflags = eflags | 0b00000100; // EOF flag
}

void OUT(unsigned short& IR, unsigned char regs[], ofstream& rez_file)
{
    unsigned char reg_num = 0;
    reg_num = (IR & 0b0000000001110000) >> 4;
    rez_file << regs[reg_num];
}

void MOV(unsigned short& IR, unsigned char regs[])
{
    unsigned char dest = 0;
    unsigned char src = 0;

    dest = (IR & 0b0000011100000000) >> 8;
    src = (IR & 0b0000000001110000) >> 4;

    regs[dest] = regs[src];
}

void MOVC(unsigned short& IR, unsigned char regs[])
{
    unsigned char dest = 0;
    unsigned char constant = 0;

    dest = (IR & 0b0000011100000000) >> 8;
    constant = (IR & 0b0000000011111111);

    regs[dest] = constant;
}

//Jump operations
void JMP(unsigned short& IR, unsigned char*& PC)
{
    signed char constant = 0;
    constant = (IR & 0b0000000011111111);
    PC = PC + constant - 2;

}
void JFE(unsigned short& IR, unsigned char*& PC, unsigned char eflags)
{
    signed char constant = 0;
    constant = (IR & 0b0000000011111111);
    if (((eflags & 0b00000100) >> 2) == 1)
    {
        PC = PC + constant - 2;
    }
}
void JZ(unsigned short& IR, unsigned char*& PC, unsigned char eflags)
{
    signed char constant = 0;
    constant = (IR & 0b0000000011111111);
    if (((eflags & 0b00010000) >> 4) == 1)
    {
        PC = PC + constant - 2;
    }
}
void JNZ(unsigned short& IR, unsigned char*& PC, unsigned char eflags)
{
    signed char constant = 0;
    constant = (IR & 0b0000000011111111);
    if (((eflags & 0b00010000) >> 4) == 0)
    {
        PC = PC + constant - 2;
    }
}
void JC(unsigned short& IR, unsigned char*& PC, unsigned char eflags)
{
    signed char constant = 0;
    constant = (IR & 0b0000000011111111);
    if ((eflags & 0b00000001) == 1)
    {
        PC = PC + constant - 2;
    }
}
void JO(unsigned short& IR, unsigned char*& PC, unsigned char eflags)
{
    signed char constant = 0;
    constant = (IR & 0b0000000011111111);
    if (((eflags & 0b00100000) >> 5) == 1)
    {
        PC = PC + constant - 2;
    }
}

//Arithmetic and bit operations.
void INC(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    unsigned char dest = 0;
    dest = (IR & 0b0000011100000000) >> 8;
    //flags
    if (regs[dest] == 255) eflags = eflags | 0b00000001; // carry flag

    if (regs[dest] == 127) eflags = eflags | 0b00100000; // overflow flag

    regs[dest]++;
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag;

}

void DEC(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    unsigned char dest = 0;
    dest = (IR & 0b0000011100000000) >> 8;

    //flags
    if (regs[dest] == 0) eflags = eflags | 0b00000001; // carry flag

    if (regs[dest] == 128) eflags = eflags | 0b00100000; // overflow flag

    regs[dest]--;
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}

void ADD(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    bool mode = 0;
    unsigned char dest = 0;
    unsigned char src1 = 0;
    unsigned char src2 = 0;
    mode = (IR & 0b0000000010000000) >> 7;
    dest = (IR & 0b0000011100000000) >> 8;
    src1 = (IR & 0b0000000001110000) >> 4;

    if (mode == 0)
    {
        unsigned char temp1 = regs[dest];
        unsigned char temp2 = regs[src1];
        regs[dest] = temp1 + temp2;

        //flags
        if (regs[dest] < temp1 || regs[dest] < temp2) eflags = eflags | 0b00000001; // carry flag

        unsigned char sign1 = temp1 >> 7;
        unsigned char sign2 = temp2 >> 7;
        unsigned char sign3 = regs[dest] >> 7;
        if (sign1 == sign2 && sign1 != sign3) eflags = eflags | 0b00100000; // overflow flag
    }
    else if (mode == 1)
    {
        src2 = (IR & 0b0000000000000111);
        unsigned char temp1 = regs[src1];
        unsigned char temp2 = regs[src2];
        regs[dest] = temp1 + temp2;

        //flags
        if (regs[dest] < temp1 || regs[dest] < temp2) eflags = eflags | 0b00000001; // carry flag
        unsigned char sign1 = temp1 >> 7;
        unsigned char sign2 = temp2 >> 7;
        unsigned char sign3 = regs[dest] >> 7;
        if (sign1 == sign2 && sign1 != sign3) eflags = eflags | 0b00100000; // overflow flag
    }
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag

}

void SUB(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    bool mode = 0;
    unsigned char dest = 0;
    unsigned char src1 = 0;
    mode = (IR & 0b0000000010000000) >> 7;
    dest = (IR & 0b0000011100000000) >> 8;
    src1 = (IR & 0b0000000001110000) >> 4;

    if (mode == 0)
    {
        unsigned char temp1 = regs[dest];
        unsigned char temp2 = regs[src1];
        regs[dest] = temp1 - temp2;

        //flags
        if (temp2 > temp1) eflags = eflags | 0b00000001; // carry flag

        unsigned char sign1 = temp1 >> 7;
        unsigned char sign2 = temp2 >> 7;
        unsigned char sign3 = regs[dest] >> 7;
        if (sign1 != sign2 && sign1 != sign3) eflags = eflags | 0b00100000; // overflow flag
    }
    else if (mode == 1)
    {
        unsigned char src2 = 0;
        src2 = (IR & 0b0000000000000111);
        unsigned char temp1 = regs[src1];
        unsigned char temp2 = regs[src2];

        regs[dest] = temp1 - temp2;

        //flags
        if (temp2 > temp1) eflags = eflags | 0b00000001; // carry flag
        unsigned char sign1 = temp1 >> 7;
        unsigned char sign2 = temp2 >> 7;
        unsigned char sign3 = regs[dest] >> 7;
        if (sign1 != sign2 && sign1 != sign3) eflags = eflags | 0b00100000; // overflow flag

    }
    //flags
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}

void XOR(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    bool mode = 0;
    unsigned char dest = 0;
    unsigned char src1 = 0;
    mode = (IR & 0b0000000010000000) >> 7;
    dest = (IR & 0b0000011100000000) >> 8;
    src1 = (IR & 0b0000000001110000) >> 4;

    if (mode == 0)
    {
        regs[dest] = regs[dest] ^ regs[src1];
    }
    else if (mode == 1)
    {
        unsigned char src2 = 0;
        src2 = (IR & 0b0000000000000111);
        regs[dest] = regs[src1] ^ regs[src2];
    }
    //flags
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}

void OR(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    bool mode = 0;
    unsigned char dest = 0;
    unsigned char src1 = 0;
    mode = (IR & 0b0000000010000000) >> 7;
    dest = (IR & 0b0000011100000000) >> 8;
    src1 = (IR & 0b0000000001110000) >> 4;

    if (mode == 0)
    {
        regs[dest] = regs[dest] | regs[src1];
    }
    else if (mode == 1)
    {
        unsigned char src2 = 0;
        src2 = (IR & 0b0000000000000111);
        regs[dest] = regs[src1] | regs[src2];
    }
    //flags
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}

void AND(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    bool mode = 0;
    unsigned char dest = 0;
    unsigned char src1 = 0;
    mode = (IR & 0b0000000010000000) >> 7;
    dest = (IR & 0b0000011100000000) >> 8;
    src1 = (IR & 0b0000000001110000) >> 4;

    if (mode == 0)
    {
        regs[dest] = regs[dest] & regs[src1];
    }
    else if (mode == 1)
    {
        unsigned char src2 = 0;
        src2 = (IR & 0b0000000000000111);
        regs[dest] = regs[src1] & regs[src2];
    }
    //flags
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}
void LSR(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    unsigned char dest = 0;
    unsigned char src = 0;
    dest = (IR & 0b0000011100000000) >> 8;
    src = (IR & 0b0000000001110000) >> 4;

    regs[dest] = regs[src] >> 1;

    //flags
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}

void LSL(unsigned short& IR, unsigned char regs[], unsigned char& eflags)
{
    eflags = eflags & 0b00000010;
    unsigned char dest = 0;
    unsigned char src = 0;
    dest = (IR & 0b0000011100000000) >> 8;
    src = (IR & 0b0000000001110000) >> 4;

    regs[dest] = regs[src] << 1;

    //veliaveles
    if (regs[dest] == 0) eflags = eflags | 0b00010000; // zero flag
}

// -----------------------------------------------------------------------

int main()
{
    unsigned char regs[8];
    unsigned char RAM[256];
    unsigned char* PC = RAM;
    unsigned short IR = 0;
    unsigned char eflags = 0b00000010;

    // Reading the instructions

    ifstream bin_file;
    bin_file.open("MYDECRYPTOR.bin", ios::binary | ios::in);

    ifstream txt_file;
    txt_file.open("MYTEXT.txt");

    ofstream rez_file;
    rez_file.open("rez.txt");

    char symbol;
    while (bin_file.get(symbol)) // writes the instructions to RAM
    {
        *PC = symbol;
        PC++;
    }

    /// ---------------------------- Main loop ----------------------------

    PC = RAM; // resets the PC

    while (true)
    {
        //Fetch
        IR = 0;
        IR = ((*PC) << 8) | *(PC + 1);
        PC += 2;

        //Decode
        unsigned char op_code = 0;
        op_code = (IR & 0b1111100000000000) >> 11;

        if (op_code == 0x00) {} // NOP
        else if (op_code == 0x01) IN(IR, regs, eflags, txt_file);
        else if (op_code == 0x02) OUT(IR, regs, rez_file);
        else if (op_code == 0x03) MOV(IR, regs);
        else if (op_code == 0x04) MOVC(IR, regs);
        else if (op_code == 0x05) JMP(IR, PC);
        else if (op_code == 0x06) JZ(IR, PC, eflags);
        else if (op_code == 0x07) JNZ(IR, PC, eflags);
        else if (op_code == 0x08) JC(IR, PC, eflags);
        else if (op_code == 0x09) JO(IR, PC, eflags);
        else if (op_code == 0x0A) JFE(IR, PC, eflags);
        else if (op_code == 0x0B) break; // HLT
        else if (op_code == 0x0C) INC(IR, regs, eflags);
        else if (op_code == 0x0D) DEC(IR, regs, eflags);
        else if (op_code == 0x0E) ADD(IR, regs, eflags);
        else if (op_code == 0x0F) SUB(IR, regs, eflags);
        else if (op_code == 0x10) XOR(IR, regs, eflags);
        else if (op_code == 0x11) OR(IR, regs, eflags);
        else if (op_code == 0x12) AND(IR, regs, eflags);
        else if (op_code == 0x13) LSR(IR, regs, eflags);
        else if (op_code == 0x14) LSL(IR, regs, eflags);
    }
    cout << hex << (unsigned int)eflags;
    cout << endl;
    cout << (char)131;

    txt_file.close();
    rez_file.close();
    bin_file.close();

    return 0;
}