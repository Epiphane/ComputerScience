#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "decode.hpp"

using namespace std;

int classify_type(const ALL_Types);

unsigned int getPCData(ifstream & in, unsigned int pc) {
   unsigned int addr, data;
   while ((in >> addr) && addr != pc) {
      in >> data;
   } 
   if (in >> data) {
      cout << hex << addr << ":     ";
      return data;
   }
   else {
      cout << "BAD PC" << endl;
      exit(1);
   }

}

unsigned int getNextValidInstr(ifstream & in) {
   unsigned int addr, data;
   while (in >> addr >> data && data > 0x10000) {
   }
   if (addr != 0xda) {
      cout << hex << addr << ":     ";
   }
   return data;
}

Register pc;
Options opts;

int main(int argc, char ** argv) {
   unsigned int addr, data;
   int c;
   string s, filename;
   ALL_Types temp;
   while ((c = getopt(argc, argv, "pdiwsc:f:")) != -1) {
      switch(c) {
#if 0 // will include later in lab 6
         case 'p':
            opts.program = true;
            break;
         case 'd':
            opts.dump = true;
            break;
         case 'i':
            opts.instrs = true;
            break;
         case 'w':
            opts.writes = true;
            break;
         case 's':
            opts.stats = true;
            break;
         case 'c':
            //caches = Caches(atoi(optarg));
            break;
#endif
         case 'f':
            filename = optarg;
            break;
      }
   }

   if (filename == "") {
      cerr << "ERROR: no source file specified; "
         << "run this program with -f filename" << endl;
      exit(1);
   }  
   ifstream in(filename.c_str());
   in >> hex;                    // in takes hex numbers
   in >> s;
   if (s != "PC") {
      cout << "Invalid .sim file" << endl;
      exit(1);
   }
   in >> addr;
   pc = addr;
   in >> s;
   in >> s;
   cout << "Starting at PC " << hex << pc << endl;
   data = getPCData(in, pc);
   temp = ALL_Types(data);
   classify_type(temp);

   while(data = getNextValidInstr(in)) {
      temp = ALL_Types(data);
      if (classify_type(temp) == -2) 
         break;
   }
   return 0;
}

char *reg(unsigned int reg);
int classify_type(const ALL_Types data) {
   if (data.type.alu.instr.class_type.type_check == ALU_TYPE) {
      if (data.type.alu.instr.lsli.op == ALU_LSLI_OP) {
         cout << "lsl ";
         cout << reg(data.type.alu.instr.lsli.rd) << ", ";
         cout << reg(data.type.alu.instr.lsli.rm) << ", ";
         cout << "#" << data.type.alu.instr.lsli.imm << endl;
      }
      else if (data.type.alu.instr.lsri.op == ALU_LSRI_OP) {
         cout << "lsr ";
         cout << reg(data.type.alu.instr.lsri.rd) << ", ";
         cout << reg(data.type.alu.instr.lsri.rm) << ", ";
         cout << "#" << data.type.alu.instr.lsri.imm << endl;
      }
      else if (data.type.alu.instr.asri.op == ALU_ASRI_OP) {
         cout << "asr ";
         cout << reg(data.type.alu.instr.asri.rd) << ", ";
         cout << reg(data.type.alu.instr.asri.rm) << ", ";
         cout << "#" << data.type.alu.instr.asri.imm << endl;
      }
      else if (data.type.alu.instr.addr.op == ALU_ADDR_OP) {
         cout << "add ";
         cout << reg(data.type.alu.instr.addr.rd) << ", ";
         cout << reg(data.type.alu.instr.addr.rn) << ", ";
         cout << reg(data.type.alu.instr.addr.rm) << endl;
      }
      else if (data.type.alu.instr.subr.op == ALU_SUBR_OP) {
         cout << "sub ";
         cout << reg(data.type.alu.instr.subr.rd) << ", ";
         cout << reg(data.type.alu.instr.subr.rn) << ", ";
         cout << reg(data.type.alu.instr.subr.rm) << endl;
      }
      else if (data.type.alu.instr.add3i.op == ALU_ADD3I_OP) {
         cout << "adds ";
         cout << reg(data.type.alu.instr.add3i.rd) << ", ";
         cout << reg(data.type.alu.instr.add3i.rn) << ", ";
         cout << "#" << data.type.alu.instr.add3i.imm << endl;
      }
      else if (data.type.alu.instr.sub3i.op == ALU_SUB3I_OP) {
         cout << "subs ";
         cout << reg(data.type.alu.instr.sub3i.rd) << ", ";
         cout << reg(data.type.alu.instr.sub3i.rn) << ", ";
         cout << "#" << data.type.alu.instr.sub3i.imm << endl;
      }
      else if (data.type.alu.instr.add8i.op == ALU_ADD8I_OP) {
         cout << "adds ";
         cout << reg(data.type.alu.instr.add8i.rdn) << ", ";
         cout << "#" << data.type.alu.instr.add8i.imm << endl;
      }
      else if (data.type.alu.instr.sub8i.op == ALU_SUB8I_OP) {
         cout << "subs ";
         cout << reg(data.type.alu.instr.sub8i.rdn) << ", ";
         cout << "#" << data.type.alu.instr.sub8i.imm << endl;
      }
      else if (data.type.alu.instr.cmp.op == ALU_CMP_OP) {
         cout << "cmp ";
         cout << reg(data.type.alu.instr.cmp.rdn) << ", ";
         cout << "#" << data.type.alu.instr.cmp.imm << endl;
      }
      else if (data.type.alu.instr.mov.op == ALU_MOV_OP) {
         cout << "movs ";
         cout << reg(data.type.alu.instr.cmp.rdn) << ", ";
         cout << "#" << data.type.alu.instr.cmp.imm << endl;
      }
      return ALU_TYPE;
   }
   else if (data.type.dp.instr.class_type.type_check == DP_TYPE) {
      switch (data.type.dp.instr.DP_Instr.op) {
         case 0:
            cout << "and ";
            break;
         case 1:
            cout << "eor ";
            break;
         case 2:
            cout << "lsl ";
            break;
         case 3:
            cout << "lsr ";
            break;
         case 4:
            cout << "asr ";
            break;
         case 5:
            cout << "adc ";
            break;
         case 6:
            cout << "sbc ";
            break;
         case 7:
            cout << "ror ";
            break;
         case 8:
            cout << "tst ";
            break;
         case 9:
            cout << "rsb ";
            break;
         case 10:
            cout << "cmp ";
            break;
         case 11:
            cout << "cmn ";
            break;
         case 12:
            cout << "orr ";
            break;
         case 13:
            cout << "mul ";
            break;
         case 14:
            cout << "bic ";
            break;
         case 15:
            cout << "mvn ";
            break;
      }
      cout << reg(data.type.dp.instr.DP_Instr.rdn) << ", ";
      cout << reg(data.type.dp.instr.DP_Instr.rm) << endl;
      return DP_TYPE;
   }
   else if (data.type.sp.instr.class_type.type_check == SP_TYPE) {
      if (data.type.sp.instr.add.op == 0) { // ADD
         cout << "add ";
         cout << reg(data.type.sp.instr.add.N * 8 + data.type.sp.instr.add.rn);
         cout << ", " << reg(data.type.sp.instr.add.rm);
         if (data.type.sp.instr.add.rm != 15) // Stack pointer isn't Rm
            cout << ", " << reg(data.type.sp.instr.add.rn);
         cout << endl;
      }
      else if (data.type.sp.instr.cmp.op == 1) { // CMP
         cout << "cmp ";
         cout << reg(data.type.sp.instr.cmp.N * 8 + data.type.sp.instr.cmp.rn);
         cout << ", " << reg(data.type.sp.instr.cmp.rm) << endl;
      }
      else if (data.type.sp.instr.mov.op == 2) { // MOV
         cout << "mov ";
         cout << reg(data.type.sp.instr.mov.N * 8 + data.type.sp.instr.mov.rn);
         cout << ", " << reg(data.type.sp.instr.mov.rm) << endl;
      }
      else {
         if (data.type.sp.instr.blx.N == 1) { // BLX
            cout << "blx ";
         else (data.type.sp.instr.bx.N == 0) { // BX
            cout << "bx ";

         cout << reg(data.type.sp.instr.blx.rm) << endl;
      }

      return SP_TYPE;
   }
   else if (data.type.uncond.instr.class_type.type_check == UNCOND_TYPE) {
      cout << "b ";
      cout << "0x" << hex << data.type.uncond.instr.b.imm;
      return UNCOND_TYPE;
   }
   else if (data.type.misc.instr.class_type.type_check == MISC_TYPE) {
         if (data.type.misc.instr.sxth.op == 8) { // SXTH
            cout << "sxth ";
            cout << reg(data.type.misc.instr.sxth.rd) << ", ";
            cout << reg(data.type.misc.instr.sxth.rm) << endl;
         }
         else if (data.type.misc.instr.sxtb.op == 9) { // SXTB
            cout << "sxtb ";
            cout << reg(data.type.misc.instr.sxtb.rd) << ", ";
            cout << reg(data.type.misc.instr.sxtb.rm) << endl;
         }
         else if (data.type.misc.instr.uxth.op == 10) { // UXTH
            cout << "uxth ";
            cout << reg(data.type.misc.instr.uxth.rd) << ", ";
            cout << reg(data.type.misc.instr.uxth.rm) << endl;
         }
         else if (data.type.misc.instr.uxtb.op == 11) { // UXTB
            cout << "uxtb ";
            cout << reg(data.type.misc.instr.uxtb.rd) << ", ";
            cout << reg(data.type.misc.instr.uxtb.rm) << endl;
         }
         else if (data.type.misc.instr.rev.op == 40) { // SXTB
            cout << "rev ";
            cout << reg(data.type.misc.instr.rev.rd) << ", ";
            cout << reg(data.type.misc.instr.rev.rm) << endl;
         }
         else if (data.type.misc.instr.rev16.op == 41) { // SXTB
            cout << "rev16 ";
            cout << reg(data.type.misc.instr.rev16.rd) << ", ";
            cout << reg(data.type.misc.instr.rev16.rm) << endl;
         }
         else if (data.type.misc.instr.revsh.op == 43) { // SXTB
            cout << "revsh ";
            cout << reg(data.type.misc.instr.revsh.rd) << ", ";
            cout << reg(data.type.misc.instr.revsh.rm) << endl;
         }
         else if (data.type.misc.instr.bkpt.op == 14) { // BKPT
            cout << bkpt << "# " << data.type.misc.instr.bkpt.imm << endl;
         }
         else if (data.type.misc.instr.push.op == MISC_PUSH_OP) {
            cout << "push ";
            cout << "{";
            if (data.type.misc.instr.push.reg_list & 1) {
               cout << "r0, ";
            }
            if (data.type.misc.instr.push.reg_list & 2) {
               cout << "r1, ";
            }
            if (data.type.misc.instr.push.reg_list & 4) {
               cout << "r2, ";
            }
            if (data.type.misc.instr.push.reg_list & 8) {
               cout << "r3, ";
            }
            if (data.type.misc.instr.push.reg_list & 16) {
               cout << "r4, ";
            }
            if (data.type.misc.instr.push.reg_list & 32) {
               cout << "r5, ";
            }
            if (data.type.misc.instr.push.reg_list & 64) {
               cout << "r6, ";
            }
            if (data.type.misc.instr.push.reg_list & 128) {
               cout << "r7, ";
            }
            cout << "lr}" << endl;
         }
         else if (data.type.misc.instr.sub.op == MISC_SUB_OP) {
            cout << "sub sp, #";
            cout << setbase(10) << (static_cast<unsigned int>(data.type.misc.instr.sub.imm)<< 2) << endl;
         }
         return MISC_TYPE;
      }

   // Complete the rest of these
      else if (data.type.cond.instr.class_type.type_check == COND_TYPE) {
         cout << "b ";
         cout << data.printCond(data.type.cond.instr.cond) << " ";
         cout << "0x" << hex << imm << endl;
         return COND_TYPE;
      }
      else if (data.type.ldm.instr.class_type.type_check == LDM_TYPE) {
         cout << "ldm ";
         cout << reg(data.type.ldm.instr.ldm.rn)
         cout << "{";
         if (data.type.ldm.instr.ldm.reg_list & 1) {
            cout << "r0, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 2) {
            cout << "r1, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 4) {
            cout << "r2, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 8) {
            cout << "r3, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 16) {
            cout << "r4, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 32) {
            cout << "r5, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 64) {
            cout << "r6, ";
         }
         if (data.type.ldm.instr.ldm.reg_list & 128) {
            cout << "r7, ";
         };
         return LDM_TYPE;
      }
      else if (data.type.stm.instr.class_type.type_check == STM_TYPE) {
         //return STM_TYPE;
      }
      else if (data.type.ldrl.instr.class_type.type_check == LDRL_TYPE) {
         //return LDRL_TYPE;
      }
      else if (data.type.addsp.instr.class_type.type_check == ADD_SP_TYPE) {
         //return ADD_SP_TYPE;
      }
      else {
         // Complete the rest of these instruction classes
         if (data.type.ld_st.instr.class_type.opA == LD_ST_REG_OPA) {
         }
         else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMM_OPA) {
            if (data.type.ld_st.instr.class_type.opB == LD_ST_OPB_STR) {
            }
            else if (data.type.ld_st.instr.class_type.opB == LD_ST_OPB_LDR) {
            }
         }
         else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMB_OPA) {
         }
         else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMH_OPA) {
         }
         else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMSP_OPA) {
         }
         else {
            cout << "NOT A VALID INSTRUCTION" << endl;
         }
      }
   cout << "NO TYPE FOUND" << endl;

   return -1; // Error
}

char *reg(unsigned int reg) {
   char result[10];

   if (reg == 13)
      sprintf(result, "sp");
   else if (reg == 15)
      sprintf(result, "pc");
   else
      sprintf(result, "r%d", reg);

   return result;
}
