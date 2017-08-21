#ifndef CPU_H
#define CPU_H

#include <memory>
#include <string>
#include <fstream>
#include "clock.hpp"
#include "registers.hpp"
#include "MMU.hpp"
#include "rom.hpp"

using namespace std;

class CPU 
{
	public:
		CPU();
		virtual ~CPU() {};

		int step();
		void reset();
		void loadROM(ifstream&);

	private:
		typedef void (CPU::*InstrFunc)(WORD);

		struct instruction
		{
			string assembly;
			string description;
			int operandLength;
			int cycles;
			InstrFunc func;
		};

		unique_ptr<Registers> registers;
		unique_ptr<MMU> mmu;
		unique_ptr<Clock> clock;

		// represents type of rom
		string romType;

		void writeStack(WORD);

		void add(BYTE&, BYTE);
		void addWord(WORD&, WORD);
		void subtract(BYTE n);
		void sbc(BYTE n);
		void andd(BYTE n);
		void xorr(BYTE n);
		BYTE increment(BYTE val);
		BYTE decrement(BYTE val);

		void nop(WORD){}
		void ld_bc_nn(WORD op){ registers->bc.w = op; }
		void ld_bc_a(WORD){ mmu->writeByte(registers->bc.w, registers->af.b.b1); }
		void inc_bc(WORD){ registers->bc.w++; }
		void inc_b(WORD){ registers->bc.b.b1 = increment(registers->bc.b.b1); }
		void dec_b(WORD){ registers->bc.b.b1 = decrement(registers->bc.b.b1); }
		void ld_b_n(WORD op){ registers->bc.w = op; }
		void rlca(WORD) {
			bool carry = registers->af.b.b1 & 0x80;

			if (carry) {
				flagSet(*registers, flag_c);
			} else {
				flagClear(*registers, flag_c);
			}

			registers->af.b.b1 <<= 1; // shift left 1
			registers->af.b.b1 += carry;
			flagClear(registers, flag_h | flag_n | flag_z);
		};
		void ld_nn_sp(WORD op) { mmu->writeWord(op, registers->sp.w); }
		void add_hl_bc(WORD) {}
		// 	{"ADD (HL), (BC)", "Add (BC) to (HL).", 0, 8, NULL}, // 0x09
		// 	{"LD A, (BC)", "Put value (BC) into A.", 0, 8, NULL}, // 0x0A
		// 	{"DEC (BC)", "Decrement register (BC).", 0, 8, NULL}, // 0x0B
		// 	{"INC C", "Increment register C.", 0, 4, NULL}, // 0x0C
		// 	{"DEC C", "Decrement register C.", 0, 4, NULL}, // 0x0D
		// 	{"LD C, n", "Put value C into n.", 1, 8, NULL}, // 0x0E
		// 	{"RRCA", "Rotate A right.Old bit 0 to Carry flag.", 0, 4, NULL}, // 0x0F
		// 	{"STOP", "Halt CPU & LCD display until button is pressed.", 0, 4, NULL}, // 0x10
		// 	{"LD (DE), nn", "Put value nn into (DE).", 2, 12, NULL}, // 0x11
		// 	{"LD (DE), A", "Put value A into (DE).", 0, 8, NULL}, // 0x12
		// 	{"INC (DE)", "Increment register (DE).", 0, 8, NULL}, // 0x13
		// 	{"INC D", "Increment register D.", 0, 4, NULL}, // 0x14
		// 	{"DEC D", "Decrement register E.", 0, 4, NULL}, // 0x15
		// 	{"LD D, n", "Put value D into n.", 1, 8, NULL}, // 0x16
		// 	{"RLA", "Rotate A left through Carry flag.", 0, 4, NULL}, // 0x17
		// 	{"JR n", "Add n to current address and jump to it.", 1, 8, NULL}, // 0x18
		// 	{"ADD (HL), (DE)", "Add (DE) to (HL).", 0, 8, NULL}, // 0x19
		// 	{"LD A, (DE)", "Put value (DE) into A.", 0, 8, NULL}, // 0x1A
		// 	{"DEC (DE)", "Decrement register (DE).", 0, 8, NULL}, // 0x1B
		// 	{"INC E", "Increment register E.", 0, 4, NULL}, // 0x1C
		// 	{"DEC E", "Decrement register E.", 0, 4, NULL}, // 0x1D
		// 	{"LD E, n", "Put value E into n.", 1, 8, NULL}, // 0x1E
		// 	{"RRA", "Rotate A right through carry flag.", 0, 4, NULL}, // 0x1F
		// 	{"JR NZ, n", "If Z flag is reset, add n to current address and jump to it.", 1, 8, NULL}, // 0x20
		// 	{"LD (HL), nn", "Put value nn into (HL).", 2, 12, NULL}, // 0x21
		// 	{"LDI (HL), A", "Put A into memory address (HL). Increment (HL).", 0, 8, NULL}, // 0x22
		// 	{"INC (HL)", "Increment register (HL).", 0, 8, NULL}, // 0x23
		// 	{"INC H", "Increment register H.", 0, 4, NULL}, // 0x24
		// 	{"DEC H", "Decrement register H.", 0, 4, NULL}, // 0x25
		// 	{"LD H, n", "Put value H into n.", 1, 8, NULL}, // 0x26
		// 	{"DAA", "Decimal adjust register A.", 0, 4, NULL}, // 0x27
		// 	{"JR Z, n", "If Z flag is set, add n to current address and jump to it.", 1, 8, NULL}, // 0x28
		// 	{"ADD (HL), (HL)", "Add (HL) to (HL).", 0, 8, NULL}, // 0x29
		// 	{"LDI A, (HL)", "Put value at address HL into A. Increment (HL).", 0, 8, NULL}, // 0x2A
		// 	{"DEC (HL)", "Decrement register (HL).", 0, 8, NULL}, // 0x2B
		// 	{"INC L", "Increment register L.", 0, 4, NULL}, // 0x2C
		// 	{"DEC L", "Decrement register L.", 0, 4, NULL}, // 0x2D
		// 	{"LD L, n", "Put value L into n.", 1, 8, NULL}, // 0x2E
		// 	{"CPL", "Complement A register. (Flip all bits).", 0, 4, NULL}, // 0x2F
		// 	{"JR NC, n", "If C flag is reset, add n to current address and jump to it.", 1, 8, NULL}, // 0x30
		// 	{"LD (SP), nn", "Put value nn into (SP).", 2, 12, NULL}, // 0x31
		// 	{"LDD (HL), A", "Put A into memory address (HL). Decrement (HL).", 0, 8, NULL}, // 0x32
		// 	{"INC (SP)", "Increment register (SP).", 0, 8, NULL}, // 0x33
		// 	{"INC (HL)", "Increment register (HL).", 0, 12, NULL}, // 0x34
		// 	{"DEC (HL)", "Decrement register (HL).", 0, 12, NULL}, // 0x35
		// 	{"LD (HL), n", "Put value n into (HL).", 1, 12, NULL}, // 0x36
		// 	{"SCF", "Set Carry flag.", 0, 4, NULL}, // 0x37
		// 	{"JR C, n", "If C flag is set, add n to current address and jump to it.", 1, 8, NULL}, // 0x38
		// 	{"ADD (HL), (SP)", "Add (SP) to (HL).", 0, 8, NULL}, // 0x39
		// 	{"LDD A, (HL)", "Put value at address (HL) into A. Decrement (HL).", 0, 8, NULL}, // 0x3A
		// 	{"DEC (SP)", "Decrement register (SP).", 0, 8, NULL}, // 0x3B
		// 	{"INC A", "Increment register A.", 0, 4, NULL}, // 0x3C
		// 	{"DEC A", "Decrement register A.", 0, 4, NULL}, // 0x3D
		// 	{"LD A, #", "Put value # into A.", 1, 8, NULL}, // 0x3E
		// 	{"CCF", "Complement carry flag. If C flag is set, then reset it. If C flag is reset, then set it.", 0, 4, NULL}, // 0x3F
		// 	{"LD B, B", "Put value B into B.", 0, 4, NULL}, // 0x40
		// 	{"LD B, C", "Put value C into B.", 0, 4, NULL}, // 0x41
		// 	{"LD B, D", "Put value D into B.", 0, 4, NULL}, // 0x42
		// 	{"LD B, E", "Put value E into B.", 0, 4, NULL}, // 0x43
		// 	{"LD B, H", "Put value H into B.", 0, 4, NULL}, // 0x44
		// 	{"LD B, L", "Put value L into B.", 0, 4, NULL}, // 0x45
		// 	{"LD B, (HL)", "Put value (HL) into B.", 0, 8, NULL}, // 0x46
		// 	{"LD B, A", "Put value A into B.", 0, 4, NULL}, // 0x47
		// 	{"LD C, B", "Put value B into C.", 0, 4, NULL}, // 0x48
		// 	{"LD C, C", "Put value C into C.", 0, 4, NULL}, // 0x49
		// 	{"LD C, D", "Put value D into C.", 0, 4, NULL}, // 0x4A
		// 	{"LD C, E", "Put value E into C.", 0, 4, NULL}, // 0x4B
		// 	{"LD C, H", "Put value H into C.", 0, 4, NULL}, // 0x4C
		// 	{"LD C, L", "Put value L into C.", 0, 4, NULL}, // 0x4D
		// 	{"LD C, (HL)", "Put value (HL) into C.", 0, 8, NULL}, // 0x4E
		// 	{"LD C, A", "Put value A into C.", 0, 4, NULL}, // 0x4F
		// 	{"LD D, B", "Put value B into D.", 0, 4, NULL}, // 0x50
		// 	{"LD D, C", "Put value C into D.", 0, 4, NULL}, // 0x51
		// 	{"LD D, D", "Put value D into D.", 0, 4, NULL}, // 0x52
		// 	{"LD D, E", "Put value E into D.", 0, 4, NULL}, // 0x53
		// 	{"LD D, H", "Put value H into D.", 0, 4, NULL}, // 0x54
		// 	{"LD D, L", "Put value L into D.", 0, 4, NULL}, // 0x55
		// 	{"LD D, (HL)", "Put value (HL) into D.", 0, 8, NULL}, // 0x56
		// 	{"LD D, A", "Put value A into D.", 0, 4, NULL}, // 0x57
		// 	{"LD E, B", "Put value B into E.", 0, 4, NULL}, // 0x58
		// 	{"LD E, C", "Put value C into E.", 0, 4, NULL}, // 0x59
		// 	{"LD E, D", "Put value D into E.", 0, 4, NULL}, // 0x5A
		// 	{"LD E, E", "Put value E into E.", 0, 4, NULL}, // 0x5B
		// 	{"LD E, H", "Put value H into E.", 0, 4, NULL}, // 0x5C
		// 	{"LD E, L", "Put value L into E.", 0, 4, NULL}, // 0x5D
		// 	{"LD E, (HL)", "Put value (HL) into E.", 0, 8, NULL}, // 0x5E
		// 	{"LD E, A", "Put value A into E.", 0, 4, NULL}, // 0x5F
		// 	{"LD H, B", "Put value B into H.", 0, 4, NULL}, // 0x60
		// 	{"LD H, C", "Put value C into H.", 0, 4, NULL}, // 0x61
		// 	{"LD H, D", "Put value D into H.", 0, 4, NULL}, // 0x62
		// 	{"LD H, E", "Put value E into H.", 0, 4, NULL}, // 0x63
		// 	{"LD H, H", "Put value H into H.", 0, 4, NULL}, // 0x64
		// 	{"LD H, L", "Put value L into H.", 0, 4, NULL}, // 0x65
		// 	{"LD H, (HL)", "Put value (HL) into H.", 0, 8, NULL}, // 0x66
		// 	{"LD H, A", "Put value A into A.", 0, 4, NULL}, // 0x67
		// 	{"LD L, B", "Put value B into L.", 0, 4, NULL}, // 0x68
		// 	{"LD L, C", "Put value C into L.", 0, 4, NULL}, // 0x69
		// 	{"LD L, D", "Put value D into L.", 0, 4, NULL}, // 0x6A
		// 	{"LD L, E", "Put value E into L.", 0, 4, NULL}, // 0x6B
		// 	{"LD L, H", "Put value H into L.", 0, 4, NULL}, // 0x6C
		// 	{"LD L, L", "Put value L into L.", 0, 4, NULL}, // 0x6D
		// 	{"LD L, (HL)", "Put value (HL) into L.", 0, 8, NULL}, // 0x6E
		// 	{"LD L, A", "Put value A into L.", 0, 4, NULL}, // 0x6F
		// 	{"LD (HL), B", "Put value B into (HL).", 0, 8, NULL}, // 0x70
		// 	{"LD (HL), C", "Put value C into (HL).", 0, 8, NULL}, // 0x71
		// 	{"LD (HL), D", "Put value D into (HL).", 0, 8, NULL}, // 0x72
		// 	{"LD (HL), E", "Put value E into (HL).", 0, 8, NULL}, // 0x73
		// 	{"LD (HL), H", "Put value H into (HL).", 0, 8, NULL}, // 0x74
		// 	{"LD (HL), L", "Put value L into (HL).", 0, 8, NULL}, // 0x75
		// 	{"HALT", "Power down CPU until an interrupt occurs.", 0, 4, NULL}, // 0x76
		// 	{"LD (HL), A", "Put value A into (HL).", 0, 8, NULL}, // 0x77
		// 	{"LD A, B", "Put value B into A.", 0, 4, NULL}, // 0x78
		// 	{"LD A, C", "Put value C into A.", 0, 4, NULL}, // 0x79
		// 	{"LD A, D", "Put value D into A.", 0, 4, NULL}, // 0x7A
		// 	{"LD A, E", "Put value E into A.", 0, 4, NULL}, // 0x7B
		// 	{"LD A, H", "Put value H into A.", 0, 4, NULL}, // 0x7C
		// 	{"LD A, L", "Put value L into A.", 0, 4, NULL}, // 0x7D
		// 	{"LD A, (HL)", "Put value (HL) into A", 0, 8, NULL}, // 0x7E
		// 	{"LD A, A", "Put value A into A", 0, 4, NULL}, // 0x7F
		void add_b(WORD n) { add(registers->bc.b.b1); }
		void add_c(WORD n) { add(registers->bc.b.b2); }
		void add_d(WORD n) { add(registers->de.b.b1); }
		void add_e(WORD n) { add(registers->de.b.b2); }
		void add_h(WORD n) { add(registers->hl.b.b1); }
		void add_l(WORD n) { add(registers->hl.b.b2); }
		void add_hl(WORD n) { add(registers->hl.w); }
		void add_a(WORD n) { add(registers->af.b.b1); }
		void adc_b(WORD n) { adc(registers->bc.b.b1); }
		void adc_c(WORD n) { adc(registers->bc.b.b2); }
		void adc_d(WORD n) { adc(registers->de.b.b1); }
		void adc_e(WORD n) { adc(registers->de.b.b2); }
		void adc_h(WORD n) { adc(registers->hl.b.b1); }
		void adc_l(WORD n) { adc(registers->hl.b.b2); }
		void adc_hl(WORD n) { adc(registers->hl.w); }
		void adc_a(WORD n) { adc(registers->af.b.b1); }
		void sub_b(WORD n) { subtract(registers->bc.b.b1); }
		void sub_c(WORD n) { subtract(registers->bc.b.b2); }
		void sub_d(WORD n) { subtract(registers->de.b.b1); }
		void sub_e(WORD n) { subtract(registers->de.b.b2); }
		void sub_h(WORD n) { subtract(registers->hl.b.b1); }
		void sub_l(WORD n) { subtract(registers->hl.b.b2); }
		void sub_hl(WORD n) { subtract(registers->hl.w); }
		void sub_a(WORD n) { subtract(registers->af.b.b1); }
		void sbc_b(WORD n) { sbc(registers->bc.b.b1); }
		void sbc_c(WORD n) { sbc(registers->bc.b.b2); }
		void sbc_d(WORD n) { sbc(registers->de.b.b1); }
		void sbc_e(WORD n) { sbc(registers->de.b.b2); }
		void sbc_h(WORD n) { sbc(registers->hl.b.b1); }
		void sbc_l(WORD n) { sbc(registers->hl.b.b2); }
		void sbc_hl(WORD n) { sbc(registers->hl.w); }
		void sbc_a(WORD n) { sbc(registers->af.b.b1); }
		void and_b(WORD n) { andd(registers->bc.b.b1); }
		void and_c(WORD n) { andd(registers->bc.b.b2); }
		void and_d(WORD n) { andd(registers->de.b.b1); }
		void and_e(WORD n) { andd(registers->de.b.b2); }
		void and_h(WORD n) { andd(registers->hl.b.b1); }
		void and_l(WORD n) { andd(registers->hl.b.b2); }
		void and_hl(WORD n) { andd(registers->hl.w); }
		void and_a(WORD n) { andd(registers->af.b.b1); }
		// 	{"XOR B", "Logical exclusive OR B with register A, result in A.", 0, 4, NULL}, // 0xA8
		// 	{"XOR C", "Logical exclusive OR C with register A, result in A.", 0, 4, NULL}, // 0xA9
		// 	{"XOR D", "Logical exclusive OR D with register A, result in A.", 0, 4, NULL}, // 0xAA
		// 	{"XOR E", "Logical exclusive OR E with register A, result in A.", 0, 4, NULL}, // 0xAB
		// 	{"XOR H", "Logical exclusive OR H with register A, result in A.", 0, 4, NULL}, // 0xAC
		// 	{"XOR L", "Logical exclusive OR L with register A, result in A.", 0, 4, NULL}, // 0xAD
		// 	{"XOR (HL)", "Logical exclusive OR (HL) with register A, result in A.", 0, 8, NULL}, // 0xAE
		// 	{"XOR A", "Logical exclusive OR A with register A, result in A.", 0, 4, NULL}, // 0xAF
		// 	{"OR B", "Logical OR B with register A, result in A.", 0, 4, NULL}, // 0xB0
		// 	{"OR C", "Logical OR C with register A, result in A.", 0, 4, NULL}, // 0xB1
		// 	{"OR D", "Logical OR D with register A, result in A.", 0, 4, NULL}, // 0xB2
		// 	{"OR E", "Logical OR E with register A, result in A.", 0, 4, NULL}, // 0xB3
		// 	{"OR H", "Logical OR H with register A, result in A.", 0, 4, NULL}, // 0xB4
		// 	{"OR L", "Logical OR L with register A, result in A.", 0, 4, NULL}, // 0xB5
		// 	{"OR (HL)", "Logical OR (HL) with register A, result in A.", 0, 8, NULL}, // 0xB6
		// 	{"OR A", "Logical OR A with register A, result in A.", 0, 4, NULL}, // 0xB7
		// 	{"CP B", "Compare A with B. A - B but results are thrown away.", 0, 4, NULL}, // 0xB8
		// 	{"CP C", "Compare A with C. A - C but results are thrown away.", 0, 4, NULL}, // 0xB9
		// 	{"CP D", "Compare A with D. A - D but results are thrown away.", 0, 4, NULL}, // 0xBA
		// 	{"CP E", "Compare A with E. A - E but results are thrown away.", 0, 4, NULL}, // 0xBB
		// 	{"CP H", "Compare A with H. A - H but results are thrown away.", 0, 4, NULL}, // 0xBC
		// 	{"CP L", "Compare A with L. A - L but results are thrown away.", 0, 4, NULL}, // 0xBD
		// 	{"CP (HL)", "Compare A with (HL). A - (HL) but results are thrown away.", 0, 8, NULL}, // 0xBE
		// 	{"CP A", "Compare A with A. A - A but results are thrown away.", 0, 4, NULL}, // 0xBF
		// 	{"RET NZ", "Return if Z flag is reset.", 0, 8, NULL}, // 0xC0
		// 	{"POP (BC)", "Pop two bytes off stack into register pair (BC). Increment (SP) twice.", 0, 12, NULL}, // 0xC1
		// 	{"JP NZ, nn", "Jump to address nn if Z flag is reset.", 1, 12, NULL}, // 0xC2
		// 	{"JP nn", "Jump to address nn.", 2, 12, &CPU::jp_nn}, // 0xC3
		// 	{"CALL NZ, nn", "If Z flag is reset, call address nn.", 2, 12, NULL}, // 0xC4
		// 	{"PUSH (BC)", "Push register pair (BC) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xC5
		// 	{"ADD A, #", "Add # into A.", 1, 8, NULL}, // 0xC6
		// 	{"RST 00H", "Push present address onto stack. Jump to address $0000.", 0, 32, NULL}, // 0xC7
		// 	{"RET Z", "Return if Z flag is set.", 0, 8, NULL}, // 0xC8
		// 	{"RET", "Pop two bytes from stack & jump to that address.", 0, 8, NULL}, // 0xC9
		// 	{"JP Z, nn", "Jump to address nn if Z flag is set.", 2, 12, NULL}, // 0xCA
		// 	{"CB n", "Extended instruction set n.", 1, 8, NULL}, // 0xCB
		// 	{"CALL Z, nn", "If Z flag is set, call address nn.", 2, 12, NULL}, // 0xCC
		void call_nn(WORD op) { writeStack(op); registers->pc = op; }
		// 	{"ADC A, #", "Add # + Carry flag to A.", 1, 8, NULL}, // 0xCE
		// 	{"RST 08H", "Push present address onto stack. Jump to address $0080.", 0, 32, NULL}, // 0xCF
		// 	{"RET NC", "Return if C flag is reset.", 0, 8, NULL}, // 0xD0
		// 	{"POP (DE)", "Pop two bytes off stack into register pair (DE). Increment (SP) twice.", 0, 12, NULL}, // 0xD1
		// 	{"JP NC, nn", "Jump to address nn if C flag is reset.", 2, 12, NULL}, // 0xD2
		// 	{"Undefined 0xD3", "Undefined", 0, 0, NULL}, // 0xD3
		// 	{"CALL NC, nn", "If C flag is reset, call address nn.", 2, 12, NULL}, // 0xD4
		// 	{"PUSH (DE)", "Push register pair (DE) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xD5
		// 	{"SUB #", "Subtract # from A.", 1, 8, NULL}, // 0xD6
		// 	{"RST 10H", "Push present address onto stack. Jump to address $0010.", 0, 32, NULL}, // 0xD7
		// 	{"RET C", "Return if C flag is set.", 0, 8, NULL}, // 0xD8
		// 	{"RETI", "Pop two bytes from stack & jump to that address then enable interrupts.", 0, 8, NULL}, // 0xD9
		// 	{"JP C, nn", "Jump to address nn if C flag is set.", 2, 12, NULL}, // 0xDA
		// 	{"Undefined 0xD8", "Undefined.", 0, 0, NULL}, // 0xDB
		// 	{"CALL C, nn", "If C flag is set, call address nn.", 2, 12, NULL}, // 0xDC
		// 	{"Undefined 0xDD", "Undefined.", 0, 0, NULL}, // 0xDD
		// 	{"SBC A, n", "Subtract n + Carry flag from A.", 1, 8, NULL}, // 0xDE
		// 	{"RST 18H", "Push present address onto stack. Jump to address $0018.", 0, 32, NULL}, // 0xDF
		// 	{"LDH (n), A", "Put A into memory address $FF00+n.", 1, 12, NULL}, // 0xE0
		// 	{"POP (HL)", "Pop two bytes off stack into register pair (HL). Increment (SP) twice.", 0, 16, NULL}, // 0xE1
		// 	{"LD (C), A", "Put A into address $FF00 + register C", 0, 8, NULL}, // 0xE2
		// 	{"Undefined 0xE3", "Undefined.", 0, 0, NULL}, // 0xE3
		// 	{"Undefined 0xE4", "Undefined.", 0, 0, NULL}, // 0xE4
		// 	{"PUSH (HL)", "Push register pair (HL) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xE5
		// 	{"AND #", "Logically AND # with A, result in A.", 1, 8, NULL}, // 0xE6
		// 	{"RST 20H", "Push present address onto stack. Jump to address $0020.", 0, 32, NULL}, // 0xE7
		// 	{"ADD # to (SP)", "Add # to (SP).", 1, 16, NULL}, // 0xE8
		// 	{"JP (HL)", "Jump to address contained in (HL).", 0, 4, NULL}, // 0xE9
		// 	{"LD (nn), A", "Put value A into (nn).", 2, 16, NULL}, // 0xEA
		// 	{"Undefined 0xEB", "Undefined.", 0, 0, NULL}, // 0xEB
		// 	{"Undefined 0xEC", "Undefined.", 0, 0, NULL}, // 0xEC
		// 	{"Undefined 0xED", "Undefined.", 0, 0, NULL}, // 0xED
		// 	{"XOR #", "Logical exclusive OR # with register A, result in A.", 1, 8, NULL}, // 0xEE
		// 	{"RST 28H", "Push present address onto stack. Jump to address $0028.", 0, 32, NULL}, // 0xEF
		// 	{"LDH A, (n)", "Put memory address $FF00+n into A.", 1, 12, NULL}, // 0xF0
		// 	{"POP (AF)", "Pop two bytes off stack into register pair (AF). Increment (SP) twice.", 0, 12, NULL}, // 0xF1
		// 	{"LD A, (C)", "Put value at address $FF00 + register C into A.", 0, 8, NULL}, // 0xF2
		// 	{"DI", "Disables interrupts after instruction after DI is executed.", 0, 4, NULL}, // 0xF3
		// 	{"Undefined 0xF4", "Undefined.", 0, 0, NULL}, // 0xF4
		// 	{"PUSH (AF)", "Push register pair (AF) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xF5
		// 	{"OR #", "Logical OR # with register A, result in A.", 1, 8, NULL}, // 0xF6
		// 	{"RST 30H", "Push present address onto stack. Jump to address $0030.", 0, 32, NULL}, // 0xF7
		// 	{"LDHL (SP), n", "Put (SP)+n effective address into (HL).", 1, 12, NULL}, // 0xF8
		// 	{"LD (SP), (HL)", "Put (HL) into (SP).", 0, 8, NULL}, // 0xF9
		// 	{"LD A, (nn)", "Put value (nn) into A.", 2, 16, NULL}, // 0xFA
		// 	{"EI", "Enable interrupts after instruction after EI is executed.", 0, 4, NULL}, // 0xFB
		// 	{"Undefined 0xFC", "Undefined.", 0, 0, NULL}, // 0xFC
		// 	{"Undefined 0xFD", "Undefined.", 0, 0, NULL}, // 0xFD
		// 	{"CP n", "Compare A with n.", 1, 8, NULL}, // 0xFE
		// 	{"RST 38H", "Push present address onto stack. Jump to address $0038.", 0, 32, NULL}, // 0xFF
		// };

		// // Total of 256 extended instructions possible. "CB prefix"
		// const struct instruction extendedInstructions[256] = 
		// {
		// 	{"RLC B", "Rotate B left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x00
		// 	{"RLC C", "Rotate C left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x01
		// 	{"RLC D", "Rotate D left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x02
		// 	{"RLC E", "Rotate E left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x03
		// 	{"RLC H", "Rotate H left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x04
		// 	{"RLC L", "Rotate L left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x05
		// 	{"RLC (HL)", "Rotate (HL) left. Old bit 7 to Carry flag.", 0, 16, NULL}, // 0x06
		// 	{"RLC A", "Rotate A left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x07
		// 	{"RRC B", "Rotate B right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x08
		// 	{"RRC C", "Rotate C right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x09
		// 	{"RRC D", "Rotate D right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0A
		// 	{"RRC E", "Rotate E right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0B
		// 	{"RRC H", "Rotate H right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0C
		// 	{"RRC L", "Rotate L right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0D
		// 	{"RRC (HL)", "Rotate (HL) right. Old bit 0 to Carry flag.", 0, 16, NULL}, // 0x0E
		// 	{"RRC A", "Rotate A right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0F
		// 	{"RL B", "Rotate B left through Carry flag.", 0, 8, NULL}, // 0x10
		// 	{"RL C", "Rotate C left through Carry flag.", 0, 8, NULL}, // 0x11
		// 	{"RL D", "Rotate D left through Carry flag.", 0, 8, NULL}, // 0x12
		// 	{"RL E", "Rotate E left through Carry flag.", 0, 8, NULL}, // 0x13
		// 	{"RL H", "Rotate H left through Carry flag.", 0, 8, NULL}, // 0x14
		// 	{"RL L", "Rotate L left through Carry flag.", 0, 8, NULL}, // 0x15
		// 	{"RL (HL)", "Rotate (HL) left through Carry flag.", 0, 16, NULL}, // 0x16
		// 	{"RL A", "Rotate A left through Carry flag.", 0, 8, NULL}, // 0x17
		// 	{"RR B", "Rotate B right through Carry flag.", 0, 8, NULL}, // 0x18
		// 	{"RR C", "Rotate C right through Carry flag.", 0, 8, NULL}, // 0x19
		// 	{"RR D", "Rotate D right through Carry flag.", 0, 8, NULL}, // 0x1A
		// 	{"RR E", "Rotate E right through Carry flag.", 0, 8, NULL}, // 0x1B
		// 	{"RR H", "Rotate H right through Carry flag.", 0, 8, NULL}, // 0x1C
		// 	{"RR L", "Rotate L right through Carry flag.", 0, 8, NULL}, // 0x1D
		// 	{"RR (HL)", "Rotate (HL) right through Carry flag.", 0, 16, NULL}, // 0x1E
		// 	{"RR A", "Rotate A right through Carry flag.", 0, 8, NULL}, // 0x1F
		// 	{"SLA B", "Rotate B left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x20
		// 	{"SLA C", "Rotate C left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x21
		// 	{"SLA D", "Rotate D left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x22
		// 	{"SLA E", "Rotate E left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x23
		// 	{"SLA H", "Rotate H left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x24
		// 	{"SLA L", "Rotate L left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x25
		// 	{"SLA (HL)", "Rotate (HL) left into Carry. LSB of n set to 0.", 0, 16, NULL}, // 0x26
		// 	{"SLA A", "Rotate A left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x27
		// 	{"SRA B", "Rotate B right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x28
		// 	{"SRA C", "Rotate C right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x29
		// 	{"SRA D", "Rotate D right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2A
		// 	{"SRA E", "Rotate E right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2B
		// 	{"SRA H", "Rotate H right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2C
		// 	{"SRA L", "Rotate L right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2D
		// 	{"SRA (HL)", "Rotate (HL) right into Carry. MSB doesn't change.", 0, 16, NULL}, // 0x2E
		// 	{"SRA A", "Rotate A right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2F
		// 	{"SWAP B", "Swap upper and lower nibbles of B", 0, 8, NULL}, // 0x30
		// 	{"SWAP C", "Swap upper and lower nibbles of C", 0, 8, NULL}, // 0x31
		// 	{"SWAP D", "Swap upper and lower nibbles of D", 0, 8, NULL}, // 0x32
		// 	{"SWAP E", "Swap upper and lower nibbles of E", 0, 8, NULL}, // 0x33
		// 	{"SWAP H", "Swap upper and lower nibbles of H", 0, 8, NULL}, // 0x34
		// 	{"SWAP L", "Swap upper and lower nibbles of L", 0, 8, NULL}, // 0x35
		// 	{"SWAP (HL)", "Swap upper and lower nibbles of (HL)", 0, 16, NULL}, // 0x36
		// 	{"SWAP A", "Swap upper and lower nibbles of A", 0, 8, NULL}, // 0x37
		// 	{"SRL B", "Shift B right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x38
		// 	{"SRL C", "Shift C right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x39
		// 	{"SRL D", "Shift D right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3A
		// 	{"SRL E", "Shift E right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3B
		// 	{"SRL H", "Shift H right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3C
		// 	{"SRL L", "Shift L right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3D
		// 	{"SRL (HL)", "Shift (HL) right into Carry. MSB set to 0.", 0, 16, NULL}, // 0x3E
		// 	{"SRL A", "Shift A right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3F
		// 	{"BIT 0, B", "Test bit 0 in register B.", 0, 8, NULL}, // 0x40
		// 	{"BIT 0, C", "Test bit 0 in register C.", 0, 8, NULL}, // 0x41
		// 	{"BIT 0, D", "Test bit 0 in register D.", 0, 8, NULL}, // 0x42
		// 	{"BIT 0, E", "Test bit 0 in register E.", 0, 8, NULL}, // 0x43
		// 	{"BIT 0, H", "Test bit 0 in register H.", 0, 8, NULL}, // 0x44
		// 	{"BIT 0, L", "Test bit 0 in register L.", 0, 8, NULL}, // 0x45
		// 	{"BIT 0, (HL)", "Test bit 0 in register (HL).", 0, 16, NULL}, // 0x46
		// 	{"BIT 0, A", "Test bit 0 in register A.", 0, 8, NULL}, // 0x47
		// 	{"BIT 1, B", "Test bit 1 in register B.", 0, 8, NULL}, // 0x48
		// 	{"BIT 1, C", "Test bit 1 in register C.", 0, 8, NULL}, // 0x49
		// 	{"BIT 1, D", "Test bit 1 in register D.", 0, 8, NULL}, // 0x4A
		// 	{"BIT 1, E", "Test bit 1 in register E.", 0, 8, NULL}, // 0x4B
		// 	{"BIT 1, H", "Test bit 1 in register H.", 0, 8, NULL}, // 0x4C
		// 	{"BIT 1, L", "Test bit 1 in register L.", 0, 8, NULL}, // 0x4D
		// 	{"BIT 1, (HL)", "Test bit 1 in register (HL).", 0, 16, NULL}, // 0x4E
		// 	{"BIT 1, A", "Test bit 1 in register A.", 0, 8, NULL}, // 0x4F
		// 	{"BIT 2, B", "Test bit 2 in register B.", 0, 8, NULL}, // 0x50
		// 	{"BIT 2, C", "Test bit 2 in register C.", 0, 8, NULL}, // 0x51
		// 	{"BIT 2, D", "Test bit 2 in register D.", 0, 8, NULL}, // 0x52
		// 	{"BIT 2, E", "Test bit 2 in register E.", 0, 8, NULL}, // 0x53
		// 	{"BIT 2, H", "Test bit 2 in register H.", 0, 8, NULL}, // 0x54
		// 	{"BIT 2, L", "Test bit 2 in register L.", 0, 8, NULL}, // 0x55
		// 	{"BIT 2, (HL)", "Test bit 2 in register (HL).", 0, 16, NULL}, // 0x56
		// 	{"BIT 2, A", "Test bit 2 in register A.", 0, 8, NULL}, // 0x57
		// 	{"BIT 3, B", "Test bit 3 in register B.", 0, 8, NULL}, // 0x58
		// 	{"BIT 3, C", "Test bit 3 in register C.", 0, 8, NULL}, // 0x59
		// 	{"BIT 3, D", "Test bit 3 in register D.", 0, 8, NULL}, // 0x5A
		// 	{"BIT 3, E", "Test bit 3 in register E.", 0, 8, NULL}, // 0x5B
		// 	{"BIT 3, H", "Test bit 3 in register H.", 0, 8, NULL}, // 0x5C
		// 	{"BIT 3, L", "Test bit 3 in register L.", 0, 8, NULL}, // 0x5D
		// 	{"BIT 3, (HL)", "Test bit 3 in register (HL).", 0, 16, NULL}, // 0x5E
		// 	{"BIT 3, A", "Test bit 3 in register A.", 0, 8, NULL}, // 0x5F
		// 	{"BIT 4, B", "Test bit 4 in register B.", 0, 8, NULL}, // 0x60
		// 	{"BIT 4, C", "Test bit 4 in register C.", 0, 8, NULL}, // 0x61
		// 	{"BIT 4, D", "Test bit 4 in register D.", 0, 8, NULL}, // 0x62
		// 	{"BIT 4, E", "Test bit 4 in register E.", 0, 8, NULL}, // 0x63
		// 	{"BIT 4, H", "Test bit 4 in register H.", 0, 8, NULL}, // 0x64
		// 	{"BIT 4, L", "Test bit 4 in register L.", 0, 8, NULL}, // 0x65
		// 	{"BIT 4, (HL)", "Test bit 4 in register (HL).", 0, 16, NULL}, // 0x66
		// 	{"BIT 4, A", "Test bit 4 in register A.", 0, 8, NULL}, // 0x67
		// 	{"BIT 5, B", "Test bit 5 in register B.", 0, 8, NULL}, // 0x68
		// 	{"BIT 5, C", "Test bit 5 in register C.", 0, 8, NULL}, // 0x69
		// 	{"BIT 5, D", "Test bit 5 in register D.", 0, 8, NULL}, // 0x6A
		// 	{"BIT 5, E", "Test bit 5 in register E.", 0, 8, NULL}, // 0x6B
		// 	{"BIT 5, H", "Test bit 5 in register H.", 0, 8, NULL}, // 0x6C
		// 	{"BIT 5, L", "Test bit 5 in register L.", 0, 8, NULL}, // 0x6D
		// 	{"BIT 5, (HL)", "Test bit 5 in register (HL).", 0, 16, NULL}, // 0x6E
		// 	{"BIT 5, A", "Test bit 5 in register A.", 0, 8, NULL}, // 0x6F
		// 	{"BIT 6, B", "Test bit 6 in register B.", 0, 8, NULL}, // 0x70
		// 	{"BIT 6, C", "Test bit 6 in register C.", 0, 8, NULL}, // 0x71
		// 	{"BIT 6, D", "Test bit 6 in register D.", 0, 8, NULL}, // 0x72
		// 	{"BIT 6, E", "Test bit 6 in register E.", 0, 8, NULL}, // 0x73
		// 	{"BIT 6, H", "Test bit 6 in register H.", 0, 8, NULL}, // 0x74
		// 	{"BIT 6, L", "Test bit 6 in register L.", 0, 8, NULL}, // 0x75
		// 	{"BIT 6, (HL)", "Test bit 6 in register (HL).", 0, 16, NULL}, // 0x76
		// 	{"BIT 6, A", "Test bit 6 in register A.", 0, 8, NULL}, // 0x77
		// 	{"BIT 7, B", "Test bit 7 in register B.", 0, 8, NULL}, // 0x78
		// 	{"BIT 7, C", "Test bit 7 in register C.", 0, 8, NULL}, // 0x79
		// 	{"BIT 7, D", "Test bit 7 in register D.", 0, 8, NULL}, // 0x7A
		// 	{"BIT 7, E", "Test bit 7 in register E.", 0, 8, NULL}, // 0x7B
		// 	{"BIT 7, H", "Test bit 7 in register H.", 0, 8, NULL}, // 0x7C
		// 	{"BIT 7, L", "Test bit 7 in register L.", 0, 8, NULL}, // 0x7D
		// 	{"BIT 7, (HL)", "Test bit 7 in register (HL).", 0, 16, NULL}, // 0x7E
		// 	{"BIT 7, A", "Test bit 7 in register A.", 0, 8, NULL}, // 0x7F
		// 	{"RES 0, B", "Reset bit 0 in register B.", 0, 8, NULL}, // 0x80
		// 	{"RES 0, C", "Reset bit 0 in register C.", 0, 8, NULL}, // 0x81
		// 	{"RES 0, D", "Reset bit 0 in register D.", 0, 8, NULL}, // 0x82
		// 	{"RES 0, E", "Reset bit 0 in register E.", 0, 8, NULL}, // 0x83
		// 	{"RES 0, H", "Reset bit 0 in register H.", 0, 8, NULL}, // 0x84
		// 	{"RES 0, L", "Reset bit 0 in register L.", 0, 8, NULL}, // 0x85
		// 	{"RES 0, (HL)", "Reset bit 0 in register (HL).", 0, 16, NULL}, // 0x86
		// 	{"RES 0, A", "Reset bit 0 in register A.", 0, 8, NULL}, // 0x87
		// 	{"RES 1, B", "Reset bit 1 in register B.", 0, 8, NULL}, // 0x88
		// 	{"RES 1, C", "Reset bit 1 in register C.", 0, 8, NULL}, // 0x89
		// 	{"RES 1, D", "Reset bit 1 in register D.", 0, 8, NULL}, // 0x8A
		// 	{"RES 1, E", "Reset bit 1 in register E.", 0, 8, NULL}, // 0x8B
		// 	{"RES 1, H", "Reset bit 1 in register H.", 0, 8, NULL}, // 0x8C
		// 	{"RES 1, L", "Reset bit 1 in register L.", 0, 8, NULL}, // 0x8D
		// 	{"RES 1, (HL)", "Reset bit 1 in register (HL).", 0, 16, NULL}, // 0x8E
		// 	{"RES 1, A", "Reset bit 1 in register A.", 0, 8, NULL}, // 0x8F
		// 	{"RES 2, B", "Reset bit 2 in register B.", 0, 8, NULL}, // 0x90
		// 	{"RES 2, C", "Reset bit 2 in register C.", 0, 8, NULL}, // 0x91
		// 	{"RES 2, D", "Reset bit 2 in register D.", 0, 8, NULL}, // 0x92
		// 	{"RES 2, E", "Reset bit 2 in register E.", 0, 8, NULL}, // 0x93
		// 	{"RES 2, H", "Reset bit 2 in register H.", 0, 8, NULL}, // 0x94
		// 	{"RES 2, L", "Reset bit 2 in register L.", 0, 8, NULL}, // 0x95
		// 	{"RES 2, (HL)", "Reset bit 2 in register (HL).", 0, 16, NULL}, // 0x96
		// 	{"RES 2, A", "Reset bit 2 in register A.", 0, 8, NULL}, // 0x97
		// 	{"RES 3, B", "Reset bit 3 in register B.", 0, 8, NULL}, // 0x98
		// 	{"RES 3, C", "Reset bit 3 in register C.", 0, 8, NULL}, // 0x99
		// 	{"RES 3, D", "Reset bit 3 in register D.", 0, 8, NULL}, // 0x9A
		// 	{"RES 3, E", "Reset bit 3 in register E.", 0, 8, NULL}, // 0x9B
		// 	{"RES 3, H", "Reset bit 3 in register H.", 0, 8, NULL}, // 0x9C
		// 	{"RES 3, L", "Reset bit 3 in register L.", 0, 8, NULL}, // 0x9D
		// 	{"RES 3, (HL)", "Reset bit 3 in register (HL).", 0, 16, NULL}, // 0x9E
		// 	{"RES 3, A", "Reset bit 3 in register A.", 0, 8, NULL}, // 0x9F
		// 	{"RES 4, B", "Reset bit 4 in register B.", 0, 8, NULL}, // 0xA0
		// 	{"RES 4, C", "Reset bit 4 in register C.", 0, 8, NULL}, // 0xA1
		// 	{"RES 4, D", "Reset bit 4 in register D.", 0, 8, NULL}, // 0xA2
		// 	{"RES 4, E", "Reset bit 4 in register E.", 0, 8, NULL}, // 0xA3
		// 	{"RES 4, H", "Reset bit 4 in register H.", 0, 8, NULL}, // 0xA4
		// 	{"RES 4, L", "Reset bit 4 in register L.", 0, 8, NULL}, // 0xA5
		// 	{"RES 4, (HL)", "Reset bit 4 in register (HL).", 0, 16, NULL}, // 0xA6
		// 	{"RES 4, A", "Reset bit 4 in register A.", 0, 8, NULL}, // 0xA7
		// 	{"RES 5, B", "Reset bit 5 in register B.", 0, 8, NULL}, // 0xA8
		// 	{"RES 5, C", "Reset bit 5 in register C.", 0, 8, NULL}, // 0xA9
		// 	{"RES 5, D", "Reset bit 5 in register D.", 0, 8, NULL}, // 0xAA
		// 	{"RES 5, E", "Reset bit 5 in register E.", 0, 8, NULL}, // 0xAB
		// 	{"RES 5, H", "Reset bit 5 in register H.", 0, 8, NULL}, // 0xAC
		// 	{"RES 5, L", "Reset bit 5 in register L.", 0, 8, NULL}, // 0xAD
		// 	{"RES 5, (HL)", "Reset bit 5 in register (HL).", 0, 16, NULL}, // 0xAE
		// 	{"RES 5, A", "Reset bit 5 in register A.", 0, 8, NULL}, // 0xAF
		// 	{"RES 6, B", "Reset bit 6 in register B.", 0, 8, NULL}, // 0xB0
		// 	{"RES 6, C", "Reset bit 6 in register C.", 0, 8, NULL}, // 0xB1
		// 	{"RES 6, D", "Reset bit 6 in register D.", 0, 8, NULL}, // 0xB2
		// 	{"RES 6, E", "Reset bit 6 in register E.", 0, 8, NULL}, // 0xB3
		// 	{"RES 6, H", "Reset bit 6 in register H.", 0, 8, NULL}, // 0xB4
		// 	{"RES 6, L", "Reset bit 6 in register L.", 0, 8, NULL}, // 0xB5
		// 	{"RES 6, (HL)", "Reset bit 6 in register (HL).", 0, 16, NULL}, // 0xB6
		// 	{"RES 6, A", "Reset bit 6 in register A.", 0, 8, NULL}, // 0xB7
		// 	{"RES 7, B", "Reset bit 7 in register B.", 0, 8, NULL}, // 0xB8
		// 	{"RES 7, C", "Reset bit 7 in register C.", 0, 8, NULL}, // 0xB9
		// 	{"RES 7, D", "Reset bit 7 in register D.", 0, 8, NULL}, // 0xBA
		// 	{"RES 7, E", "Reset bit 7 in register E.", 0, 8, NULL}, // 0xBB
		// 	{"RES 7, H", "Reset bit 7 in register H.", 0, 8, NULL}, // 0xBC
		// 	{"RES 7, L", "Reset bit 7 in register L.", 0, 8, NULL}, // 0xBD
		// 	{"RES 7, (HL)", "Reset bit 7 in register (HL).", 0, 16, NULL}, // 0xBE
		// 	{"RES 7, A", "Reset bit 7 in register A.", 0, 8, NULL}, // 0xBF
		// 	{"SET 0, B", "Set bit 0 in register B.", 0, 8, NULL}, // 0xC0
		// 	{"SET 0, C", "Set bit 0 in register C.", 0, 8, NULL}, // 0xC1
		// 	{"SET 0, D", "Set bit 0 in register D.", 0, 8, NULL}, // 0xC2
		// 	{"SET 0, E", "Set bit 0 in register E.", 0, 8, NULL}, // 0xC3
		// 	{"SET 0, H", "Set bit 0 in register H.", 0, 8, NULL}, // 0xC4
		// 	{"SET 0, L", "Set bit 0 in register L.", 0, 8, NULL}, // 0xC5
		// 	{"SET 0, (HL)", "Set bit 0 in register (HL).", 0, 16, NULL}, // 0xC6
		// 	{"SET 0, A", "Set bit 0 in register A.", 0, 8, NULL}, // 0xC7
		// 	{"SET 1, B", "Set bit 1 in register B.", 0, 8, NULL}, // 0xC8
		// 	{"SET 1, C", "Set bit 1 in register C.", 0, 8, NULL}, // 0xC9
		// 	{"SET 1, D", "Set bit 1 in register D.", 0, 8, NULL}, // 0xCA
		// 	{"SET 1, E", "Set bit 1 in register E.", 0, 8, NULL}, // 0xCB
		// 	{"SET 1, H", "Set bit 1 in register H.", 0, 8, NULL}, // 0xCC
		// 	{"SET 1, L", "Set bit 1 in register L.", 0, 8, NULL}, // 0xCD
		// 	{"SET 1, (HL)", "Set bit 1 in register (HL).", 0, 16, NULL}, // 0xCE
		// 	{"SET 1, A", "Set bit 1 in register A.", 0, 8, NULL}, // 0xCF
		// 	{"SET 2, B", "Set bit 2 in register B.", 0, 8, NULL}, // 0xD0
		// 	{"SET 2, C", "Set bit 2 in register C.", 0, 8, NULL}, // 0xD1
		// 	{"SET 2, D", "Set bit 2 in register D.", 0, 8, NULL}, // 0xD2
		// 	{"SET 2, E", "Set bit 2 in register E.", 0, 8, NULL}, // 0xD3
		// 	{"SET 2, H", "Set bit 2 in register H.", 0, 8, NULL}, // 0xD4
		// 	{"SET 2, L", "Set bit 2 in register L.", 0, 8, NULL}, // 0xD5
		// 	{"SET 2, (HL)", "Set bit 2 in register (HL).", 0, 16, NULL}, // 0xD6
		// 	{"SET 2, A", "Set bit 2 in register A.", 0, 8, NULL}, // 0xD7
		// 	{"SET 3, B", "Set bit 3 in register B.", 0, 8, NULL}, // 0xD8
		// 	{"SET 3, C", "Set bit 3 in register C.", 0, 8, NULL}, // 0xD9
		// 	{"SET 3, D", "Set bit 3 in register D.", 0, 8, NULL}, // 0xDA
		// 	{"SET 3, E", "Set bit 3 in register E.", 0, 8, NULL}, // 0xDB
		// 	{"SET 3, H", "Set bit 3 in register H.", 0, 8, NULL}, // 0xDC
		// 	{"SET 3, L", "Set bit 3 in register L.", 0, 8, NULL}, // 0xDD
		// 	{"SET 3, (HL)", "Set bit 3 in register (HL).", 0, 16, NULL}, // 0xDE
		// 	{"SET 3, A", "Set bit 3 in register A.", 0, 8, NULL}, // 0xDF
		// 	{"SET 4, B", "Set bit 4 in register B.", 0, 8, NULL}, // 0xE0
		// 	{"SET 4, C", "Set bit 4 in register C.", 0, 8, NULL}, // 0xE1
		// 	{"SET 4, D", "Set bit 4 in register D.", 0, 8, NULL}, // 0xE2
		// 	{"SET 4, E", "Set bit 4 in register E.", 0, 8, NULL}, // 0xE3
		// 	{"SET 4, H", "Set bit 4 in register H.", 0, 8, NULL}, // 0xE4
		// 	{"SET 4, L", "Set bit 4 in register L.", 0, 8, NULL}, // 0xE5
		// 	{"SET 4, (HL)", "Set bit 4 in register (HL).", 0, 16, NULL}, // 0xE6
		// 	{"SET 4, A", "Set bit 4 in register A.", 0, 8, NULL}, // 0xE7
		// 	{"SET 5, B", "Set bit 5 in register B.", 0, 8, NULL}, // 0xE8
		// 	{"SET 5, C", "Set bit 5 in register C.", 0, 8, NULL}, // 0xE9
		// 	{"SET 5, D", "Set bit 5 in register D.", 0, 8, NULL}, // 0xEA
		// 	{"SET 5, E", "Set bit 5 in register E.", 0, 8, NULL}, // 0xEB
		// 	{"SET 5, H", "Set bit 5 in register H.", 0, 8, NULL}, // 0xEC
		// 	{"SET 5, L", "Set bit 5 in register L.", 0, 8, NULL}, // 0xED
		// 	{"SET 5, (HL)", "Set bit 5 in register (HL).", 0, 16, NULL}, // 0xEE
		// 	{"SET 5, A", "Set bit 5 in register A.", 0, 8, NULL}, // 0xEF
		// 	{"SET 6, B", "Set bit 6 in register B.", 0, 8, NULL}, // 0xF0
		// 	{"SET 6, C", "Set bit 6 in register C.", 0, 8, NULL}, // 0xF1
		// 	{"SET 6, D", "Set bit 6 in register D.", 0, 8, NULL}, // 0xF2
		// 	{"SET 6, E", "Set bit 6 in register E.", 0, 8, NULL}, // 0xF3
		// 	{"SET 6, H", "Set bit 6 in register H.", 0, 8, NULL}, // 0xF4
		// 	{"SET 6, L", "Set bit 6 in register L.", 0, 8, NULL}, // 0xF5
		// 	{"SET 6, (HL)", "Set bit 6 in register (HL).", 0, 16, NULL}, // 0xF6
		// 	{"SET 6, A", "Set bit 6 in register A.", 0, 8, NULL}, // 0xF7
		// 	{"SET 7, B", "Set bit 7 in register B.", 0, 8, NULL}, // 0xF8
		// 	{"SET 7, C", "Set bit 7 in register C.", 0, 8, NULL}, // 0xF9
		// 	{"SET 7, D", "Set bit 7 in register D.", 0, 8, NULL}, // 0xFA
		// 	{"SET 7, E", "Set bit 7 in register E.", 0, 8, NULL}, // 0xFB
		// 	{"SET 7, H", "Set bit 7 in register H.", 0, 8, NULL}, // 0xFC
		// 	{"SET 7, L", "Set bit 7 in register L.", 0, 8, NULL}, // 0xFD
		// 	{"SET 7, (HL)", "Set bit 7 in register (HL).", 0, 16, NULL}, // 0xFE
		// 	{"SET 7, A", "Set bit 7 in register A.", 0, 8, NULL}, // 0xFF

		void jp_nn(WORD op){ registers->pc = op; }

		// Total of 256 instructions possible.
		const struct instruction instructionsTable[256] = 
		{
			{"NOP", "No operation.", 0, 4, &CPU::nop}, // 0x00
			{"LD (BC), nn", "Put value nn into (BC).", 2, 12, &CPU::ld_bc_nn}, // 0x01
			{"LD (BC), A", "Put value A into (BC).", 0, 8, &CPU::ld_bc_a}, // 0x02
			{"INC (BC)", "Increment register (BC).", 0, 8, &CPU::inc_bc}, // 0x03
			{"INC B", "Increment register B.", 0, 4, &CPU::inc_b}, // 0x04
			{"DEC B", "Decrement register B.", 0, 4, &CPU::dec_b}, // 0x05
			{"LD B, n", "Put value B into n.", 1, 8, NULL}, // 0x06
			{"RLCA", "Rotate A left. Ild bit 7 to Carry flag.", 0, 4, NULL}, // 0x07
			{"LD nn, (SP)", "Put (SP) at address n", 2, 20, NULL}, // 0x08
			{"ADD (HL), (BC)", "Add (BC) to (HL).", 0, 8, NULL}, // 0x09
			{"LD A, (BC)", "Put value (BC) into A.", 0, 8, NULL}, // 0x0A
			{"DEC (BC)", "Decrement register (BC).", 0, 8, NULL}, // 0x0B
			{"INC C", "Increment register C.", 0, 4, NULL}, // 0x0C
			{"DEC C", "Decrement register C.", 0, 4, NULL}, // 0x0D
			{"LD C, n", "Put value C into n.", 1, 8, NULL}, // 0x0E
			{"RRCA", "Rotate A right.Old bit 0 to Carry flag.", 0, 4, NULL}, // 0x0F
			{"STOP", "Halt CPU & LCD display until button is pressed.", 0, 4, NULL}, // 0x10
			{"LD (DE), nn", "Put value nn into (DE).", 2, 12, NULL}, // 0x11
			{"LD (DE), A", "Put value A into (DE).", 0, 8, NULL}, // 0x12
			{"INC (DE)", "Increment register (DE).", 0, 8, NULL}, // 0x13
			{"INC D", "Increment register D.", 0, 4, NULL}, // 0x14
			{"DEC D", "Decrement register E.", 0, 4, NULL}, // 0x15
			{"LD D, n", "Put value D into n.", 1, 8, NULL}, // 0x16
			{"RLA", "Rotate A left through Carry flag.", 0, 4, NULL}, // 0x17
			{"JR n", "Add n to current address and jump to it.", 1, 8, NULL}, // 0x18
			{"ADD (HL), (DE)", "Add (DE) to (HL).", 0, 8, NULL}, // 0x19
			{"LD A, (DE)", "Put value (DE) into A.", 0, 8, NULL}, // 0x1A
			{"DEC (DE)", "Decrement register (DE).", 0, 8, NULL}, // 0x1B
			{"INC E", "Increment register E.", 0, 4, NULL}, // 0x1C
			{"DEC E", "Decrement register E.", 0, 4, NULL}, // 0x1D
			{"LD E, n", "Put value E into n.", 1, 8, NULL}, // 0x1E
			{"RRA", "Rotate A right through carry flag.", 0, 4, NULL}, // 0x1F
			{"JR NZ, n", "If Z flag is reset, add n to current address and jump to it.", 1, 8, NULL}, // 0x20
			{"LD (HL), nn", "Put value nn into (HL).", 2, 12, NULL}, // 0x21
			{"LDI (HL), A", "Put A into memory address (HL). Increment (HL).", 0, 8, NULL}, // 0x22
			{"INC (HL)", "Increment register (HL).", 0, 8, NULL}, // 0x23
			{"INC H", "Increment register H.", 0, 4, NULL}, // 0x24
			{"DEC H", "Decrement register H.", 0, 4, NULL}, // 0x25
			{"LD H, n", "Put value H into n.", 1, 8, NULL}, // 0x26
			{"DAA", "Decimal adjust register A.", 0, 4, NULL}, // 0x27
			{"JR Z, n", "If Z flag is set, add n to current address and jump to it.", 1, 8, NULL}, // 0x28
			{"ADD (HL), (HL)", "Add (HL) to (HL).", 0, 8, NULL}, // 0x29
			{"LDI A, (HL)", "Put value at address HL into A. Increment (HL).", 0, 8, NULL}, // 0x2A
			{"DEC (HL)", "Decrement register (HL).", 0, 8, NULL}, // 0x2B
			{"INC L", "Increment register L.", 0, 4, NULL}, // 0x2C
			{"DEC L", "Decrement register L.", 0, 4, NULL}, // 0x2D
			{"LD L, n", "Put value L into n.", 1, 8, NULL}, // 0x2E
			{"CPL", "Complement A register. (Flip all bits).", 0, 4, NULL}, // 0x2F
			{"JR NC, n", "If C flag is reset, add n to current address and jump to it.", 1, 8, NULL}, // 0x30
			{"LD (SP), nn", "Put value nn into (SP).", 2, 12, NULL}, // 0x31
			{"LDD (HL), A", "Put A into memory address (HL). Decrement (HL).", 0, 8, NULL}, // 0x32
			{"INC (SP)", "Increment register (SP).", 0, 8, NULL}, // 0x33
			{"INC (HL)", "Increment register (HL).", 0, 12, NULL}, // 0x34
			{"DEC (HL)", "Decrement register (HL).", 0, 12, NULL}, // 0x35
			{"LD (HL), n", "Put value n into (HL).", 1, 12, NULL}, // 0x36
			{"SCF", "Set Carry flag.", 0, 4, NULL}, // 0x37
			{"JR C, n", "If C flag is set, add n to current address and jump to it.", 1, 8, NULL}, // 0x38
			{"ADD (HL), (SP)", "Add (SP) to (HL).", 0, 8, NULL}, // 0x39
			{"LDD A, (HL)", "Put value at address (HL) into A. Decrement (HL).", 0, 8, NULL}, // 0x3A
			{"DEC (SP)", "Decrement register (SP).", 0, 8, NULL}, // 0x3B
			{"INC A", "Increment register A.", 0, 4, NULL}, // 0x3C
			{"DEC A", "Decrement register A.", 0, 4, NULL}, // 0x3D
			{"LD A, #", "Put value # into A.", 1, 8, NULL}, // 0x3E
			{"CCF", "Complement carry flag. If C flag is set, then reset it. If C flag is reset, then set it.", 0, 4, NULL}, // 0x3F
			{"LD B, B", "Put value B into B.", 0, 4, NULL}, // 0x40
			{"LD B, C", "Put value C into B.", 0, 4, NULL}, // 0x41
			{"LD B, D", "Put value D into B.", 0, 4, NULL}, // 0x42
			{"LD B, E", "Put value E into B.", 0, 4, NULL}, // 0x43
			{"LD B, H", "Put value H into B.", 0, 4, NULL}, // 0x44
			{"LD B, L", "Put value L into B.", 0, 4, NULL}, // 0x45
			{"LD B, (HL)", "Put value (HL) into B.", 0, 8, NULL}, // 0x46
			{"LD B, A", "Put value A into B.", 0, 4, NULL}, // 0x47
			{"LD C, B", "Put value B into C.", 0, 4, NULL}, // 0x48
			{"LD C, C", "Put value C into C.", 0, 4, NULL}, // 0x49
			{"LD C, D", "Put value D into C.", 0, 4, NULL}, // 0x4A
			{"LD C, E", "Put value E into C.", 0, 4, NULL}, // 0x4B
			{"LD C, H", "Put value H into C.", 0, 4, NULL}, // 0x4C
			{"LD C, L", "Put value L into C.", 0, 4, NULL}, // 0x4D
			{"LD C, (HL)", "Put value (HL) into C.", 0, 8, NULL}, // 0x4E
			{"LD C, A", "Put value A into C.", 0, 4, NULL}, // 0x4F
			{"LD D, B", "Put value B into D.", 0, 4, NULL}, // 0x50
			{"LD D, C", "Put value C into D.", 0, 4, NULL}, // 0x51
			{"LD D, D", "Put value D into D.", 0, 4, NULL}, // 0x52
			{"LD D, E", "Put value E into D.", 0, 4, NULL}, // 0x53
			{"LD D, H", "Put value H into D.", 0, 4, NULL}, // 0x54
			{"LD D, L", "Put value L into D.", 0, 4, NULL}, // 0x55
			{"LD D, (HL)", "Put value (HL) into D.", 0, 8, NULL}, // 0x56
			{"LD D, A", "Put value A into D.", 0, 4, NULL}, // 0x57
			{"LD E, B", "Put value B into E.", 0, 4, NULL}, // 0x58
			{"LD E, C", "Put value C into E.", 0, 4, NULL}, // 0x59
			{"LD E, D", "Put value D into E.", 0, 4, NULL}, // 0x5A
			{"LD E, E", "Put value E into E.", 0, 4, NULL}, // 0x5B
			{"LD E, H", "Put value H into E.", 0, 4, NULL}, // 0x5C
			{"LD E, L", "Put value L into E.", 0, 4, NULL}, // 0x5D
			{"LD E, (HL)", "Put value (HL) into E.", 0, 8, NULL}, // 0x5E
			{"LD E, A", "Put value A into E.", 0, 4, NULL}, // 0x5F
			{"LD H, B", "Put value B into H.", 0, 4, NULL}, // 0x60
			{"LD H, C", "Put value C into H.", 0, 4, NULL}, // 0x61
			{"LD H, D", "Put value D into H.", 0, 4, NULL}, // 0x62
			{"LD H, E", "Put value E into H.", 0, 4, NULL}, // 0x63
			{"LD H, H", "Put value H into H.", 0, 4, NULL}, // 0x64
			{"LD H, L", "Put value L into H.", 0, 4, NULL}, // 0x65
			{"LD H, (HL)", "Put value (HL) into H.", 0, 8, NULL}, // 0x66
			{"LD H, A", "Put value A into A.", 0, 4, NULL}, // 0x67
			{"LD L, B", "Put value B into L.", 0, 4, NULL}, // 0x68
			{"LD L, C", "Put value C into L.", 0, 4, NULL}, // 0x69
			{"LD L, D", "Put value D into L.", 0, 4, NULL}, // 0x6A
			{"LD L, E", "Put value E into L.", 0, 4, NULL}, // 0x6B
			{"LD L, H", "Put value H into L.", 0, 4, NULL}, // 0x6C
			{"LD L, L", "Put value L into L.", 0, 4, NULL}, // 0x6D
			{"LD L, (HL)", "Put value (HL) into L.", 0, 8, NULL}, // 0x6E
			{"LD L, A", "Put value A into L.", 0, 4, NULL}, // 0x6F
			{"LD (HL), B", "Put value B into (HL).", 0, 8, NULL}, // 0x70
			{"LD (HL), C", "Put value C into (HL).", 0, 8, NULL}, // 0x71
			{"LD (HL), D", "Put value D into (HL).", 0, 8, NULL}, // 0x72
			{"LD (HL), E", "Put value E into (HL).", 0, 8, NULL}, // 0x73
			{"LD (HL), H", "Put value H into (HL).", 0, 8, NULL}, // 0x74
			{"LD (HL), L", "Put value L into (HL).", 0, 8, NULL}, // 0x75
			{"HALT", "Power down CPU until an interrupt occurs.", 0, 4, NULL}, // 0x76
			{"LD (HL), A", "Put value A into (HL).", 0, 8, NULL}, // 0x77
			{"LD A, B", "Put value B into A.", 0, 4, NULL}, // 0x78
			{"LD A, C", "Put value C into A.", 0, 4, NULL}, // 0x79
			{"LD A, D", "Put value D into A.", 0, 4, NULL}, // 0x7A
			{"LD A, E", "Put value E into A.", 0, 4, NULL}, // 0x7B
			{"LD A, H", "Put value H into A.", 0, 4, NULL}, // 0x7C
			{"LD A, L", "Put value L into A.", 0, 4, NULL}, // 0x7D
			{"LD A, (HL)", "Put value (HL) into A", 0, 8, NULL}, // 0x7E
			{"LD A, A", "Put value A into A", 0, 4, NULL}, // 0x7F
			{"ADD A, B", "Add B to A", 0, 4, &CPU::add_b}, // 0x80
			{"ADD A, C", "Add C to A", 0, 4, &CPU::add_c}, // 0x81
			{"ADD A, D", "Add D to A", 0, 4, &CPU::add_d}, // 0x82
			{"ADD A, E", "Add E to A", 0, 4, &CPU::add_e}, // 0x83
			{"ADD A, H", "Add H to A", 0, 4, &CPU::add_h}, // 0x84
			{"ADD A, L", "Add L to A", 0, 4, &CPU::add_l}, // 0x85
			{"ADD A, (HL)", "Add (HL) to A", 0, 8, &CPU::add_hl}, // 0x86
			{"ADD A, A", "Add A to A", 0, 4, &CPU::add_a}, // 0x87
			{"ADC A, B", "Add B + Carry flag to A.", 0, 4, &CPU::adc_b}, // 0x88
			{"ADC A, C", "Add C + Carry flag to A.", 0, 4, &CPU::adc_c}, // 0x89
			{"ADC A, D", "Add D + Carry flag to A.", 0, 4, &CPU::adc_d}, // 0x8A
			{"ADC A, E", "Add E + Carry flag to A.", 0, 4, &CPU::adc_e}, // 0x8B
			{"ADC A, H", "Add H + Carry flag to A.", 0, 4, &CPU::adc_h}, // 0x8C
			{"ADC A, L", "Add L + Carry flag to A.", 0, 4, &CPU::adc_l}, // 0x8D
			{"ADC A, (HL)", "Add (HL) + Carry flag to A.", 0, 8, &CPU::adc_hl}, // 0x8E
			{"ADC A, A", "Add A + Carry flag to A.", 0, 4, &CPU::adc_a}, // 0x8F
			{"SUB B", "Subtract B from A.", 0, 4, &CPU::sub_b}, // 0x90
			{"SUB C", "Subtract C from A.", 0, 4, &CPU::sub_c}, // 0x91
			{"SUB D", "Subtract D from A.", 0, 4, &CPU::sub_d}, // 0x92
			{"SUB E", "Subtract E from A.", 0, 4, &CPU::sub_e}, // 0x93
			{"SUB H", "Subtract H from A.", 0, 4, &CPU::sub_h}, // 0x94
			{"SUB L", "Subtract L from A.", 0, 4, &CPU::sub_l}, // 0x95
			{"SUB (HL)", "Subtract (HL) from A.", 0, 8, &CPU::sub_hl}, // 0x96
			{"SUB A", "Subtract A from A.", 0, 4, &CPU::sub_a}, // 0x97
			{"SBC A, B", "Subtract B + Carry flag from A.", 0, 4, &CPU::sbc_b}, // 0x98
			{"SBC A, C", "Subtract C + Carry flag from A.", 0, 4, &CPU::sbc_c}, // 0x99
			{"SBC A, D", "Subtract D + Carry flag from A.", 0, 4, &CPU::sbc_d}, // 0x9A
			{"SBC A, E", "Subtract E + Carry flag from A.", 0, 4, &CPU::sbc_e}, // 0x9B
			{"SBC A, H", "Subtract H + Carry flag from A.", 0, 4, &CPU::sbc_h}, // 0x9C
			{"SBC A, L", "Subtract L + Carry flag from A.", 0, 4, &CPU::sbc_l}, // 0x9D
			{"SBC A, (HL)", "Subtract (HL) + Carry flag from A.", 0, 8, &CPU::sbc_hl}, // 0x9E
			{"SBC A, A", "Subtract A + Carry flag from A.", 0, 4, &CPU::sbc_a}, // 0x9F
			{"AND B", "Logically AND B with A, result in A.", 0, 4, NULL}, // 0xA0
			{"AND C", "Logically AND C with A, result in A.", 0, 4, NULL}, // 0xA1
			{"AND D", "Logically AND D with A, result in A.", 0, 4, NULL}, // 0xA2
			{"AND E", "Logically AND E with A, result in A.", 0, 4, NULL}, // 0xA3
			{"AND H", "Logically AND H with A, result in A.", 0, 4, NULL}, // 0xA4
			{"AND L", "Logically AND L with A, result in A.", 0, 4, NULL}, // 0xA5
			{"AND (HL)", "Logically AND (HL) with A, result in A.", 0, 8, NULL}, // 0xA6
			{"AND A", "Logically AND A with A, result in A.", 0, 4, NULL}, // 0xA7
			{"XOR B", "Logical exclusive OR B with register A, result in A.", 0, 4, NULL}, // 0xA8
			{"XOR C", "Logical exclusive OR C with register A, result in A.", 0, 4, NULL}, // 0xA9
			{"XOR D", "Logical exclusive OR D with register A, result in A.", 0, 4, NULL}, // 0xAA
			{"XOR E", "Logical exclusive OR E with register A, result in A.", 0, 4, NULL}, // 0xAB
			{"XOR H", "Logical exclusive OR H with register A, result in A.", 0, 4, NULL}, // 0xAC
			{"XOR L", "Logical exclusive OR L with register A, result in A.", 0, 4, NULL}, // 0xAD
			{"XOR (HL)", "Logical exclusive OR (HL) with register A, result in A.", 0, 8, NULL}, // 0xAE
			{"XOR A", "Logical exclusive OR A with register A, result in A.", 0, 4, NULL}, // 0xAF
			{"OR B", "Logical OR B with register A, result in A.", 0, 4, NULL}, // 0xB0
			{"OR C", "Logical OR C with register A, result in A.", 0, 4, NULL}, // 0xB1
			{"OR D", "Logical OR D with register A, result in A.", 0, 4, NULL}, // 0xB2
			{"OR E", "Logical OR E with register A, result in A.", 0, 4, NULL}, // 0xB3
			{"OR H", "Logical OR H with register A, result in A.", 0, 4, NULL}, // 0xB4
			{"OR L", "Logical OR L with register A, result in A.", 0, 4, NULL}, // 0xB5
			{"OR (HL)", "Logical OR (HL) with register A, result in A.", 0, 8, NULL}, // 0xB6
			{"OR A", "Logical OR A with register A, result in A.", 0, 4, NULL}, // 0xB7
			{"CP B", "Compare A with B. A - B but results are thrown away.", 0, 4, NULL}, // 0xB8
			{"CP C", "Compare A with C. A - C but results are thrown away.", 0, 4, NULL}, // 0xB9
			{"CP D", "Compare A with D. A - D but results are thrown away.", 0, 4, NULL}, // 0xBA
			{"CP E", "Compare A with E. A - E but results are thrown away.", 0, 4, NULL}, // 0xBB
			{"CP H", "Compare A with H. A - H but results are thrown away.", 0, 4, NULL}, // 0xBC
			{"CP L", "Compare A with L. A - L but results are thrown away.", 0, 4, NULL}, // 0xBD
			{"CP (HL)", "Compare A with (HL). A - (HL) but results are thrown away.", 0, 8, NULL}, // 0xBE
			{"CP A", "Compare A with A. A - A but results are thrown away.", 0, 4, NULL}, // 0xBF
			{"RET NZ", "Return if Z flag is reset.", 0, 8, NULL}, // 0xC0
			{"POP (BC)", "Pop two bytes off stack into register pair (BC). Increment (SP) twice.", 0, 12, NULL}, // 0xC1
			{"JP NZ, nn", "Jump to address nn if Z flag is reset.", 1, 12, NULL}, // 0xC2
			{"JP nn", "Jump to address nn.", 2, 12, &CPU::jp_nn}, // 0xC3
			{"CALL NZ, nn", "If Z flag is reset, call address nn.", 2, 12, NULL}, // 0xC4
			{"PUSH (BC)", "Push register pair (BC) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xC5
			{"ADD A, #", "Add # into A.", 1, 8, NULL}, // 0xC6
			{"RST 00H", "Push present address onto stack. Jump to address $0000.", 0, 32, NULL}, // 0xC7
			{"RET Z", "Return if Z flag is set.", 0, 8, NULL}, // 0xC8
			{"RET", "Pop two bytes from stack & jump to that address.", 0, 8, NULL}, // 0xC9
			{"JP Z, nn", "Jump to address nn if Z flag is set.", 2, 12, NULL}, // 0xCA
			{"CB n", "Extended instruction set n.", 1, 8, NULL}, // 0xCB
			{"CALL Z, nn", "If Z flag is set, call address nn.", 2, 12, NULL}, // 0xCC
			{"CALL nn", "Push address of next instruction onto stack and then jump to address nn.", 2, 12, &CPU::call_nn}, // 0xCD
			{"ADC A, #", "Add # + Carry flag to A.", 1, 8, NULL}, // 0xCE
			{"RST 08H", "Push present address onto stack. Jump to address $0080.", 0, 32, NULL}, // 0xCF
			{"RET NC", "Return if C flag is reset.", 0, 8, NULL}, // 0xD0
			{"POP (DE)", "Pop two bytes off stack into register pair (DE). Increment (SP) twice.", 0, 12, NULL}, // 0xD1
			{"JP NC, nn", "Jump to address nn if C flag is reset.", 2, 12, NULL}, // 0xD2
			{"Undefined 0xD3", "Undefined", 0, 0, NULL}, // 0xD3
			{"CALL NC, nn", "If C flag is reset, call address nn.", 2, 12, NULL}, // 0xD4
			{"PUSH (DE)", "Push register pair (DE) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xD5
			{"SUB #", "Subtract # from A.", 1, 8, NULL}, // 0xD6
			{"RST 10H", "Push present address onto stack. Jump to address $0010.", 0, 32, NULL}, // 0xD7
			{"RET C", "Return if C flag is set.", 0, 8, NULL}, // 0xD8
			{"RETI", "Pop two bytes from stack & jump to that address then enable interrupts.", 0, 8, NULL}, // 0xD9
			{"JP C, nn", "Jump to address nn if C flag is set.", 2, 12, NULL}, // 0xDA
			{"Undefined 0xD8", "Undefined.", 0, 0, NULL}, // 0xDB
			{"CALL C, nn", "If C flag is set, call address nn.", 2, 12, NULL}, // 0xDC
			{"Undefined 0xDD", "Undefined.", 0, 0, NULL}, // 0xDD
			{"SBC A, n", "Subtract n + Carry flag from A.", 1, 8, NULL}, // 0xDE
			{"RST 18H", "Push present address onto stack. Jump to address $0018.", 0, 32, NULL}, // 0xDF
			{"LDH (n), A", "Put A into memory address $FF00+n.", 1, 12, NULL}, // 0xE0
			{"POP (HL)", "Pop two bytes off stack into register pair (HL). Increment (SP) twice.", 0, 16, NULL}, // 0xE1
			{"LD (C), A", "Put A into address $FF00 + register C", 0, 8, NULL}, // 0xE2
			{"Undefined 0xE3", "Undefined.", 0, 0, NULL}, // 0xE3
			{"Undefined 0xE4", "Undefined.", 0, 0, NULL}, // 0xE4
			{"PUSH (HL)", "Push register pair (HL) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xE5
			{"AND #", "Logically AND # with A, result in A.", 1, 8, NULL}, // 0xE6
			{"RST 20H", "Push present address onto stack. Jump to address $0020.", 0, 32, NULL}, // 0xE7
			{"ADD # to (SP)", "Add # to (SP).", 1, 16, NULL}, // 0xE8
			{"JP (HL)", "Jump to address contained in (HL).", 0, 4, NULL}, // 0xE9
			{"LD (nn), A", "Put value A into (nn).", 2, 16, NULL}, // 0xEA
			{"Undefined 0xEB", "Undefined.", 0, 0, NULL}, // 0xEB
			{"Undefined 0xEC", "Undefined.", 0, 0, NULL}, // 0xEC
			{"Undefined 0xED", "Undefined.", 0, 0, NULL}, // 0xED
			{"XOR #", "Logical exclusive OR # with register A, result in A.", 1, 8, NULL}, // 0xEE
			{"RST 28H", "Push present address onto stack. Jump to address $0028.", 0, 32, NULL}, // 0xEF
			{"LDH A, (n)", "Put memory address $FF00+n into A.", 1, 12, NULL}, // 0xF0
			{"POP (AF)", "Pop two bytes off stack into register pair (AF). Increment (SP) twice.", 0, 12, NULL}, // 0xF1
			{"LD A, (C)", "Put value at address $FF00 + register C into A.", 0, 8, NULL}, // 0xF2
			{"DI", "Disables interrupts after instruction after DI is executed.", 0, 4, NULL}, // 0xF3
			{"Undefined 0xF4", "Undefined.", 0, 0, NULL}, // 0xF4
			{"PUSH (AF)", "Push register pair (AF) onto stack. Decrement (SP) twice.", 0, 16, NULL}, // 0xF5
			{"OR #", "Logical OR # with register A, result in A.", 1, 8, NULL}, // 0xF6
			{"RST 30H", "Push present address onto stack. Jump to address $0030.", 0, 32, NULL}, // 0xF7
			{"LDHL (SP), n", "Put (SP)+n effective address into (HL).", 1, 12, NULL}, // 0xF8
			{"LD (SP), (HL)", "Put (HL) into (SP).", 0, 8, NULL}, // 0xF9
			{"LD A, (nn)", "Put value (nn) into A.", 2, 16, NULL}, // 0xFA
			{"EI", "Enable interrupts after instruction after EI is executed.", 0, 4, NULL}, // 0xFB
			{"Undefined 0xFC", "Undefined.", 0, 0, NULL}, // 0xFC
			{"Undefined 0xFD", "Undefined.", 0, 0, NULL}, // 0xFD
			{"CP n", "Compare A with n.", 1, 8, NULL}, // 0xFE
			{"RST 38H", "Push present address onto stack. Jump to address $0038.", 0, 32, NULL}, // 0xFF
		};

		// Total of 256 extended instructions possible. "CB prefix"
		const struct instruction extendedInstructions[256] = 
		{
			{"RLC B", "Rotate B left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x00
			{"RLC C", "Rotate C left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x01
			{"RLC D", "Rotate D left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x02
			{"RLC E", "Rotate E left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x03
			{"RLC H", "Rotate H left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x04
			{"RLC L", "Rotate L left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x05
			{"RLC (HL)", "Rotate (HL) left. Old bit 7 to Carry flag.", 0, 16, NULL}, // 0x06
			{"RLC A", "Rotate A left. Old bit 7 to Carry flag.", 0, 8, NULL}, // 0x07
			{"RRC B", "Rotate B right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x08
			{"RRC C", "Rotate C right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x09
			{"RRC D", "Rotate D right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0A
			{"RRC E", "Rotate E right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0B
			{"RRC H", "Rotate H right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0C
			{"RRC L", "Rotate L right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0D
			{"RRC (HL)", "Rotate (HL) right. Old bit 0 to Carry flag.", 0, 16, NULL}, // 0x0E
			{"RRC A", "Rotate A right. Old bit 0 to Carry flag.", 0, 8, NULL}, // 0x0F
			{"RL B", "Rotate B left through Carry flag.", 0, 8, NULL}, // 0x10
			{"RL C", "Rotate C left through Carry flag.", 0, 8, NULL}, // 0x11
			{"RL D", "Rotate D left through Carry flag.", 0, 8, NULL}, // 0x12
			{"RL E", "Rotate E left through Carry flag.", 0, 8, NULL}, // 0x13
			{"RL H", "Rotate H left through Carry flag.", 0, 8, NULL}, // 0x14
			{"RL L", "Rotate L left through Carry flag.", 0, 8, NULL}, // 0x15
			{"RL (HL)", "Rotate (HL) left through Carry flag.", 0, 16, NULL}, // 0x16
			{"RL A", "Rotate A left through Carry flag.", 0, 8, NULL}, // 0x17
			{"RR B", "Rotate B right through Carry flag.", 0, 8, NULL}, // 0x18
			{"RR C", "Rotate C right through Carry flag.", 0, 8, NULL}, // 0x19
			{"RR D", "Rotate D right through Carry flag.", 0, 8, NULL}, // 0x1A
			{"RR E", "Rotate E right through Carry flag.", 0, 8, NULL}, // 0x1B
			{"RR H", "Rotate H right through Carry flag.", 0, 8, NULL}, // 0x1C
			{"RR L", "Rotate L right through Carry flag.", 0, 8, NULL}, // 0x1D
			{"RR (HL)", "Rotate (HL) right through Carry flag.", 0, 16, NULL}, // 0x1E
			{"RR A", "Rotate A right through Carry flag.", 0, 8, NULL}, // 0x1F
			{"SLA B", "Rotate B left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x20
			{"SLA C", "Rotate C left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x21
			{"SLA D", "Rotate D left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x22
			{"SLA E", "Rotate E left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x23
			{"SLA H", "Rotate H left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x24
			{"SLA L", "Rotate L left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x25
			{"SLA (HL)", "Rotate (HL) left into Carry. LSB of n set to 0.", 0, 16, NULL}, // 0x26
			{"SLA A", "Rotate A left into Carry. LSB of n set to 0.", 0, 8, NULL}, // 0x27
			{"SRA B", "Rotate B right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x28
			{"SRA C", "Rotate C right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x29
			{"SRA D", "Rotate D right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2A
			{"SRA E", "Rotate E right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2B
			{"SRA H", "Rotate H right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2C
			{"SRA L", "Rotate L right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2D
			{"SRA (HL)", "Rotate (HL) right into Carry. MSB doesn't change.", 0, 16, NULL}, // 0x2E
			{"SRA A", "Rotate A right into Carry. MSB doesn't change.", 0, 8, NULL}, // 0x2F
			{"SWAP B", "Swap upper and lower nibbles of B", 0, 8, NULL}, // 0x30
			{"SWAP C", "Swap upper and lower nibbles of C", 0, 8, NULL}, // 0x31
			{"SWAP D", "Swap upper and lower nibbles of D", 0, 8, NULL}, // 0x32
			{"SWAP E", "Swap upper and lower nibbles of E", 0, 8, NULL}, // 0x33
			{"SWAP H", "Swap upper and lower nibbles of H", 0, 8, NULL}, // 0x34
			{"SWAP L", "Swap upper and lower nibbles of L", 0, 8, NULL}, // 0x35
			{"SWAP (HL)", "Swap upper and lower nibbles of (HL)", 0, 16, NULL}, // 0x36
			{"SWAP A", "Swap upper and lower nibbles of A", 0, 8, NULL}, // 0x37
			{"SRL B", "Shift B right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x38
			{"SRL C", "Shift C right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x39
			{"SRL D", "Shift D right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3A
			{"SRL E", "Shift E right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3B
			{"SRL H", "Shift H right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3C
			{"SRL L", "Shift L right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3D
			{"SRL (HL)", "Shift (HL) right into Carry. MSB set to 0.", 0, 16, NULL}, // 0x3E
			{"SRL A", "Shift A right into Carry. MSB set to 0.", 0, 8, NULL}, // 0x3F
			{"BIT 0, B", "Test bit 0 in register B.", 0, 8, NULL}, // 0x40
			{"BIT 0, C", "Test bit 0 in register C.", 0, 8, NULL}, // 0x41
			{"BIT 0, D", "Test bit 0 in register D.", 0, 8, NULL}, // 0x42
			{"BIT 0, E", "Test bit 0 in register E.", 0, 8, NULL}, // 0x43
			{"BIT 0, H", "Test bit 0 in register H.", 0, 8, NULL}, // 0x44
			{"BIT 0, L", "Test bit 0 in register L.", 0, 8, NULL}, // 0x45
			{"BIT 0, (HL)", "Test bit 0 in register (HL).", 0, 16, NULL}, // 0x46
			{"BIT 0, A", "Test bit 0 in register A.", 0, 8, NULL}, // 0x47
			{"BIT 1, B", "Test bit 1 in register B.", 0, 8, NULL}, // 0x48
			{"BIT 1, C", "Test bit 1 in register C.", 0, 8, NULL}, // 0x49
			{"BIT 1, D", "Test bit 1 in register D.", 0, 8, NULL}, // 0x4A
			{"BIT 1, E", "Test bit 1 in register E.", 0, 8, NULL}, // 0x4B
			{"BIT 1, H", "Test bit 1 in register H.", 0, 8, NULL}, // 0x4C
			{"BIT 1, L", "Test bit 1 in register L.", 0, 8, NULL}, // 0x4D
			{"BIT 1, (HL)", "Test bit 1 in register (HL).", 0, 16, NULL}, // 0x4E
			{"BIT 1, A", "Test bit 1 in register A.", 0, 8, NULL}, // 0x4F
			{"BIT 2, B", "Test bit 2 in register B.", 0, 8, NULL}, // 0x50
			{"BIT 2, C", "Test bit 2 in register C.", 0, 8, NULL}, // 0x51
			{"BIT 2, D", "Test bit 2 in register D.", 0, 8, NULL}, // 0x52
			{"BIT 2, E", "Test bit 2 in register E.", 0, 8, NULL}, // 0x53
			{"BIT 2, H", "Test bit 2 in register H.", 0, 8, NULL}, // 0x54
			{"BIT 2, L", "Test bit 2 in register L.", 0, 8, NULL}, // 0x55
			{"BIT 2, (HL)", "Test bit 2 in register (HL).", 0, 16, NULL}, // 0x56
			{"BIT 2, A", "Test bit 2 in register A.", 0, 8, NULL}, // 0x57
			{"BIT 3, B", "Test bit 3 in register B.", 0, 8, NULL}, // 0x58
			{"BIT 3, C", "Test bit 3 in register C.", 0, 8, NULL}, // 0x59
			{"BIT 3, D", "Test bit 3 in register D.", 0, 8, NULL}, // 0x5A
			{"BIT 3, E", "Test bit 3 in register E.", 0, 8, NULL}, // 0x5B
			{"BIT 3, H", "Test bit 3 in register H.", 0, 8, NULL}, // 0x5C
			{"BIT 3, L", "Test bit 3 in register L.", 0, 8, NULL}, // 0x5D
			{"BIT 3, (HL)", "Test bit 3 in register (HL).", 0, 16, NULL}, // 0x5E
			{"BIT 3, A", "Test bit 3 in register A.", 0, 8, NULL}, // 0x5F
			{"BIT 4, B", "Test bit 4 in register B.", 0, 8, NULL}, // 0x60
			{"BIT 4, C", "Test bit 4 in register C.", 0, 8, NULL}, // 0x61
			{"BIT 4, D", "Test bit 4 in register D.", 0, 8, NULL}, // 0x62
			{"BIT 4, E", "Test bit 4 in register E.", 0, 8, NULL}, // 0x63
			{"BIT 4, H", "Test bit 4 in register H.", 0, 8, NULL}, // 0x64
			{"BIT 4, L", "Test bit 4 in register L.", 0, 8, NULL}, // 0x65
			{"BIT 4, (HL)", "Test bit 4 in register (HL).", 0, 16, NULL}, // 0x66
			{"BIT 4, A", "Test bit 4 in register A.", 0, 8, NULL}, // 0x67
			{"BIT 5, B", "Test bit 5 in register B.", 0, 8, NULL}, // 0x68
			{"BIT 5, C", "Test bit 5 in register C.", 0, 8, NULL}, // 0x69
			{"BIT 5, D", "Test bit 5 in register D.", 0, 8, NULL}, // 0x6A
			{"BIT 5, E", "Test bit 5 in register E.", 0, 8, NULL}, // 0x6B
			{"BIT 5, H", "Test bit 5 in register H.", 0, 8, NULL}, // 0x6C
			{"BIT 5, L", "Test bit 5 in register L.", 0, 8, NULL}, // 0x6D
			{"BIT 5, (HL)", "Test bit 5 in register (HL).", 0, 16, NULL}, // 0x6E
			{"BIT 5, A", "Test bit 5 in register A.", 0, 8, NULL}, // 0x6F
			{"BIT 6, B", "Test bit 6 in register B.", 0, 8, NULL}, // 0x70
			{"BIT 6, C", "Test bit 6 in register C.", 0, 8, NULL}, // 0x71
			{"BIT 6, D", "Test bit 6 in register D.", 0, 8, NULL}, // 0x72
			{"BIT 6, E", "Test bit 6 in register E.", 0, 8, NULL}, // 0x73
			{"BIT 6, H", "Test bit 6 in register H.", 0, 8, NULL}, // 0x74
			{"BIT 6, L", "Test bit 6 in register L.", 0, 8, NULL}, // 0x75
			{"BIT 6, (HL)", "Test bit 6 in register (HL).", 0, 16, NULL}, // 0x76
			{"BIT 6, A", "Test bit 6 in register A.", 0, 8, NULL}, // 0x77
			{"BIT 7, B", "Test bit 7 in register B.", 0, 8, NULL}, // 0x78
			{"BIT 7, C", "Test bit 7 in register C.", 0, 8, NULL}, // 0x79
			{"BIT 7, D", "Test bit 7 in register D.", 0, 8, NULL}, // 0x7A
			{"BIT 7, E", "Test bit 7 in register E.", 0, 8, NULL}, // 0x7B
			{"BIT 7, H", "Test bit 7 in register H.", 0, 8, NULL}, // 0x7C
			{"BIT 7, L", "Test bit 7 in register L.", 0, 8, NULL}, // 0x7D
			{"BIT 7, (HL)", "Test bit 7 in register (HL).", 0, 16, NULL}, // 0x7E
			{"BIT 7, A", "Test bit 7 in register A.", 0, 8, NULL}, // 0x7F
			{"RES 0, B", "Reset bit 0 in register B.", 0, 8, NULL}, // 0x80
			{"RES 0, C", "Reset bit 0 in register C.", 0, 8, NULL}, // 0x81
			{"RES 0, D", "Reset bit 0 in register D.", 0, 8, NULL}, // 0x82
			{"RES 0, E", "Reset bit 0 in register E.", 0, 8, NULL}, // 0x83
			{"RES 0, H", "Reset bit 0 in register H.", 0, 8, NULL}, // 0x84
			{"RES 0, L", "Reset bit 0 in register L.", 0, 8, NULL}, // 0x85
			{"RES 0, (HL)", "Reset bit 0 in register (HL).", 0, 16, NULL}, // 0x86
			{"RES 0, A", "Reset bit 0 in register A.", 0, 8, NULL}, // 0x87
			{"RES 1, B", "Reset bit 1 in register B.", 0, 8, NULL}, // 0x88
			{"RES 1, C", "Reset bit 1 in register C.", 0, 8, NULL}, // 0x89
			{"RES 1, D", "Reset bit 1 in register D.", 0, 8, NULL}, // 0x8A
			{"RES 1, E", "Reset bit 1 in register E.", 0, 8, NULL}, // 0x8B
			{"RES 1, H", "Reset bit 1 in register H.", 0, 8, NULL}, // 0x8C
			{"RES 1, L", "Reset bit 1 in register L.", 0, 8, NULL}, // 0x8D
			{"RES 1, (HL)", "Reset bit 1 in register (HL).", 0, 16, NULL}, // 0x8E
			{"RES 1, A", "Reset bit 1 in register A.", 0, 8, NULL}, // 0x8F
			{"RES 2, B", "Reset bit 2 in register B.", 0, 8, NULL}, // 0x90
			{"RES 2, C", "Reset bit 2 in register C.", 0, 8, NULL}, // 0x91
			{"RES 2, D", "Reset bit 2 in register D.", 0, 8, NULL}, // 0x92
			{"RES 2, E", "Reset bit 2 in register E.", 0, 8, NULL}, // 0x93
			{"RES 2, H", "Reset bit 2 in register H.", 0, 8, NULL}, // 0x94
			{"RES 2, L", "Reset bit 2 in register L.", 0, 8, NULL}, // 0x95
			{"RES 2, (HL)", "Reset bit 2 in register (HL).", 0, 16, NULL}, // 0x96
			{"RES 2, A", "Reset bit 2 in register A.", 0, 8, NULL}, // 0x97
			{"RES 3, B", "Reset bit 3 in register B.", 0, 8, NULL}, // 0x98
			{"RES 3, C", "Reset bit 3 in register C.", 0, 8, NULL}, // 0x99
			{"RES 3, D", "Reset bit 3 in register D.", 0, 8, NULL}, // 0x9A
			{"RES 3, E", "Reset bit 3 in register E.", 0, 8, NULL}, // 0x9B
			{"RES 3, H", "Reset bit 3 in register H.", 0, 8, NULL}, // 0x9C
			{"RES 3, L", "Reset bit 3 in register L.", 0, 8, NULL}, // 0x9D
			{"RES 3, (HL)", "Reset bit 3 in register (HL).", 0, 16, NULL}, // 0x9E
			{"RES 3, A", "Reset bit 3 in register A.", 0, 8, NULL}, // 0x9F
			{"RES 4, B", "Reset bit 4 in register B.", 0, 8, NULL}, // 0xA0
			{"RES 4, C", "Reset bit 4 in register C.", 0, 8, NULL}, // 0xA1
			{"RES 4, D", "Reset bit 4 in register D.", 0, 8, NULL}, // 0xA2
			{"RES 4, E", "Reset bit 4 in register E.", 0, 8, NULL}, // 0xA3
			{"RES 4, H", "Reset bit 4 in register H.", 0, 8, NULL}, // 0xA4
			{"RES 4, L", "Reset bit 4 in register L.", 0, 8, NULL}, // 0xA5
			{"RES 4, (HL)", "Reset bit 4 in register (HL).", 0, 16, NULL}, // 0xA6
			{"RES 4, A", "Reset bit 4 in register A.", 0, 8, NULL}, // 0xA7
			{"RES 5, B", "Reset bit 5 in register B.", 0, 8, NULL}, // 0xA8
			{"RES 5, C", "Reset bit 5 in register C.", 0, 8, NULL}, // 0xA9
			{"RES 5, D", "Reset bit 5 in register D.", 0, 8, NULL}, // 0xAA
			{"RES 5, E", "Reset bit 5 in register E.", 0, 8, NULL}, // 0xAB
			{"RES 5, H", "Reset bit 5 in register H.", 0, 8, NULL}, // 0xAC
			{"RES 5, L", "Reset bit 5 in register L.", 0, 8, NULL}, // 0xAD
			{"RES 5, (HL)", "Reset bit 5 in register (HL).", 0, 16, NULL}, // 0xAE
			{"RES 5, A", "Reset bit 5 in register A.", 0, 8, NULL}, // 0xAF
			{"RES 6, B", "Reset bit 6 in register B.", 0, 8, NULL}, // 0xB0
			{"RES 6, C", "Reset bit 6 in register C.", 0, 8, NULL}, // 0xB1
			{"RES 6, D", "Reset bit 6 in register D.", 0, 8, NULL}, // 0xB2
			{"RES 6, E", "Reset bit 6 in register E.", 0, 8, NULL}, // 0xB3
			{"RES 6, H", "Reset bit 6 in register H.", 0, 8, NULL}, // 0xB4
			{"RES 6, L", "Reset bit 6 in register L.", 0, 8, NULL}, // 0xB5
			{"RES 6, (HL)", "Reset bit 6 in register (HL).", 0, 16, NULL}, // 0xB6
			{"RES 6, A", "Reset bit 6 in register A.", 0, 8, NULL}, // 0xB7
			{"RES 7, B", "Reset bit 7 in register B.", 0, 8, NULL}, // 0xB8
			{"RES 7, C", "Reset bit 7 in register C.", 0, 8, NULL}, // 0xB9
			{"RES 7, D", "Reset bit 7 in register D.", 0, 8, NULL}, // 0xBA
			{"RES 7, E", "Reset bit 7 in register E.", 0, 8, NULL}, // 0xBB
			{"RES 7, H", "Reset bit 7 in register H.", 0, 8, NULL}, // 0xBC
			{"RES 7, L", "Reset bit 7 in register L.", 0, 8, NULL}, // 0xBD
			{"RES 7, (HL)", "Reset bit 7 in register (HL).", 0, 16, NULL}, // 0xBE
			{"RES 7, A", "Reset bit 7 in register A.", 0, 8, NULL}, // 0xBF
			{"SET 0, B", "Set bit 0 in register B.", 0, 8, NULL}, // 0xC0
			{"SET 0, C", "Set bit 0 in register C.", 0, 8, NULL}, // 0xC1
			{"SET 0, D", "Set bit 0 in register D.", 0, 8, NULL}, // 0xC2
			{"SET 0, E", "Set bit 0 in register E.", 0, 8, NULL}, // 0xC3
			{"SET 0, H", "Set bit 0 in register H.", 0, 8, NULL}, // 0xC4
			{"SET 0, L", "Set bit 0 in register L.", 0, 8, NULL}, // 0xC5
			{"SET 0, (HL)", "Set bit 0 in register (HL).", 0, 16, NULL}, // 0xC6
			{"SET 0, A", "Set bit 0 in register A.", 0, 8, NULL}, // 0xC7
			{"SET 1, B", "Set bit 1 in register B.", 0, 8, NULL}, // 0xC8
			{"SET 1, C", "Set bit 1 in register C.", 0, 8, NULL}, // 0xC9
			{"SET 1, D", "Set bit 1 in register D.", 0, 8, NULL}, // 0xCA
			{"SET 1, E", "Set bit 1 in register E.", 0, 8, NULL}, // 0xCB
			{"SET 1, H", "Set bit 1 in register H.", 0, 8, NULL}, // 0xCC
			{"SET 1, L", "Set bit 1 in register L.", 0, 8, NULL}, // 0xCD
			{"SET 1, (HL)", "Set bit 1 in register (HL).", 0, 16, NULL}, // 0xCE
			{"SET 1, A", "Set bit 1 in register A.", 0, 8, NULL}, // 0xCF
			{"SET 2, B", "Set bit 2 in register B.", 0, 8, NULL}, // 0xD0
			{"SET 2, C", "Set bit 2 in register C.", 0, 8, NULL}, // 0xD1
			{"SET 2, D", "Set bit 2 in register D.", 0, 8, NULL}, // 0xD2
			{"SET 2, E", "Set bit 2 in register E.", 0, 8, NULL}, // 0xD3
			{"SET 2, H", "Set bit 2 in register H.", 0, 8, NULL}, // 0xD4
			{"SET 2, L", "Set bit 2 in register L.", 0, 8, NULL}, // 0xD5
			{"SET 2, (HL)", "Set bit 2 in register (HL).", 0, 16, NULL}, // 0xD6
			{"SET 2, A", "Set bit 2 in register A.", 0, 8, NULL}, // 0xD7
			{"SET 3, B", "Set bit 3 in register B.", 0, 8, NULL}, // 0xD8
			{"SET 3, C", "Set bit 3 in register C.", 0, 8, NULL}, // 0xD9
			{"SET 3, D", "Set bit 3 in register D.", 0, 8, NULL}, // 0xDA
			{"SET 3, E", "Set bit 3 in register E.", 0, 8, NULL}, // 0xDB
			{"SET 3, H", "Set bit 3 in register H.", 0, 8, NULL}, // 0xDC
			{"SET 3, L", "Set bit 3 in register L.", 0, 8, NULL}, // 0xDD
			{"SET 3, (HL)", "Set bit 3 in register (HL).", 0, 16, NULL}, // 0xDE
			{"SET 3, A", "Set bit 3 in register A.", 0, 8, NULL}, // 0xDF
			{"SET 4, B", "Set bit 4 in register B.", 0, 8, NULL}, // 0xE0
			{"SET 4, C", "Set bit 4 in register C.", 0, 8, NULL}, // 0xE1
			{"SET 4, D", "Set bit 4 in register D.", 0, 8, NULL}, // 0xE2
			{"SET 4, E", "Set bit 4 in register E.", 0, 8, NULL}, // 0xE3
			{"SET 4, H", "Set bit 4 in register H.", 0, 8, NULL}, // 0xE4
			{"SET 4, L", "Set bit 4 in register L.", 0, 8, NULL}, // 0xE5
			{"SET 4, (HL)", "Set bit 4 in register (HL).", 0, 16, NULL}, // 0xE6
			{"SET 4, A", "Set bit 4 in register A.", 0, 8, NULL}, // 0xE7
			{"SET 5, B", "Set bit 5 in register B.", 0, 8, NULL}, // 0xE8
			{"SET 5, C", "Set bit 5 in register C.", 0, 8, NULL}, // 0xE9
			{"SET 5, D", "Set bit 5 in register D.", 0, 8, NULL}, // 0xEA
			{"SET 5, E", "Set bit 5 in register E.", 0, 8, NULL}, // 0xEB
			{"SET 5, H", "Set bit 5 in register H.", 0, 8, NULL}, // 0xEC
			{"SET 5, L", "Set bit 5 in register L.", 0, 8, NULL}, // 0xED
			{"SET 5, (HL)", "Set bit 5 in register (HL).", 0, 16, NULL}, // 0xEE
			{"SET 5, A", "Set bit 5 in register A.", 0, 8, NULL}, // 0xEF
			{"SET 6, B", "Set bit 6 in register B.", 0, 8, NULL}, // 0xF0
			{"SET 6, C", "Set bit 6 in register C.", 0, 8, NULL}, // 0xF1
			{"SET 6, D", "Set bit 6 in register D.", 0, 8, NULL}, // 0xF2
			{"SET 6, E", "Set bit 6 in register E.", 0, 8, NULL}, // 0xF3
			{"SET 6, H", "Set bit 6 in register H.", 0, 8, NULL}, // 0xF4
			{"SET 6, L", "Set bit 6 in register L.", 0, 8, NULL}, // 0xF5
			{"SET 6, (HL)", "Set bit 6 in register (HL).", 0, 16, NULL}, // 0xF6
			{"SET 6, A", "Set bit 6 in register A.", 0, 8, NULL}, // 0xF7
			{"SET 7, B", "Set bit 7 in register B.", 0, 8, NULL}, // 0xF8
			{"SET 7, C", "Set bit 7 in register C.", 0, 8, NULL}, // 0xF9
			{"SET 7, D", "Set bit 7 in register D.", 0, 8, NULL}, // 0xFA
			{"SET 7, E", "Set bit 7 in register E.", 0, 8, NULL}, // 0xFB
			{"SET 7, H", "Set bit 7 in register H.", 0, 8, NULL}, // 0xFC
			{"SET 7, L", "Set bit 7 in register L.", 0, 8, NULL}, // 0xFD
			{"SET 7, (HL)", "Set bit 7 in register (HL).", 0, 16, NULL}, // 0xFE
			{"SET 7, A", "Set bit 7 in register A.", 0, 8, NULL}, // 0xFF
		};
};

#endif