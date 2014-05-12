#ifndef _DECODE_H_
#define _DECODE_H_
#include <unistd.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>

using namespace std;

/* DEFINES FOR TYPES AND OPS */
#define ALU_TYPE 0
#define SP_TYPE 17
#define DP_TYPE 32
#define LD_ST_REG_OPA 5
#define LD_ST_OPB_STR 0
#define LD_ST_OPB_STRH 1
#define LD_ST_OPB_STRB 2
#define LD_ST_OPB_LDRSB 3
#define LD_ST_OPB_LDR 4
#define LD_ST_OPB_LDRH 5
#define LD_ST_OPB_LDRB 6
#define LD_ST_OPB_LDRSH 7
#define LD_ST_IMM_OPA 6
#define LD_ST_IMMB_OPA 7
#define LD_ST_IMMH_OPA 8
#define LD_ST_IMMSP_OPA 9
#define LD_ST_ST 0
#define LD_ST_LD 1
#define UNCOND_TYPE 28
#define ADD_SP_TYPE 21
#define ALU_LSLI_OP 0
#define ALU_LSRI_OP 1
#define ALU_ASRI_OP 2
#define ALU_ADDR_OP 12
#define ALU_SUBR_OP 13
#define ALU_ADD3I_OP 14
#define ALU_SUB3I_OP 15
#define ALU_MOV_OP 4
#define ALU_CMP_OP 5
#define ALU_ADD8I_OP 6
#define ALU_SUB8I_OP 7
#define MISC_TYPE 11
#define LDM_TYPE 25
#define STM_TYPE 24
#define LDRL_TYPE 9
#define COND_TYPE 13
#define MISC_PUSH_OP 2
#define MISC_SUB_OP 1
#define MISC_POP_OP 6
#define ADD_SP8I_TYPE 22


/* ALU Type Structs */
struct ALU_LSLI_Instr {
   unsigned int rd: 3;
   unsigned int rm: 3;
   unsigned int imm: 5;
   unsigned int op: 5;
   unsigned int pad: 16;
};

typedef ALU_LSLI_Instr ALU_LSRI_Instr;
typedef ALU_LSLI_Instr ALU_ASRI_Instr;

struct ALU_ADDR_Instr {
   unsigned int rd: 3;
   unsigned int rn: 3;
   unsigned int rm: 3;
   unsigned int op: 7;
   unsigned int pad: 16;
};

typedef ALU_ADDR_Instr ALU_SUBR_Instr;

struct ALU_ADD3I_Instr {
   unsigned int rd: 3;
   unsigned int rn: 3;
   unsigned int imm: 3;
   unsigned int op: 7;
   unsigned int pad: 16;
};

typedef ALU_ADD3I_Instr ALU_SUB3I_Instr;

struct ALU_ADD8I_Instr {
   unsigned int imm: 8;
   unsigned int rdn: 3;
   unsigned int op: 5;
   unsigned int pad: 16;
};

typedef ALU_ADD8I_Instr ALU_SUB8I_Instr;
typedef ALU_ADD8I_Instr ALU_CMP8I_Instr;
typedef ALU_ADD8I_Instr ALU_MOV8I_Instr;


struct ALU_Type {
   union {
      unsigned int mem;
      struct {
         unsigned int data: 14;
         unsigned int type_check: 2;
         unsigned int pad: 16;
      } class_type;
      ALU_LSLI_Instr lsli;
      ALU_LSRI_Instr lsri;
      ALU_ASRI_Instr asri;
      ALU_ADDR_Instr addr;
      ALU_SUBR_Instr subr;
      ALU_ADD3I_Instr add3i;
      ALU_SUB3I_Instr sub3i;
      ALU_ADD8I_Instr add8i;
      ALU_SUB8I_Instr sub8i;
      ALU_CMP8I_Instr cmp;
      ALU_MOV8I_Instr mov;
   } instr;
};

struct DP_Type {
   union {
      unsigned int mem;
      struct {
         unsigned int data: 10;
         unsigned int type_check: 6;
         unsigned int pad: 16;
      } class_type;
      /* All Data Processing Types follow this format */
      struct {
         unsigned int rdn: 3;
         unsigned int rm: 3;
         unsigned int op: 4;
         unsigned int type: 6;
         unsigned int pad: 16;
      } DP_Instr;
   } instr; 
};

/* Special Types */
struct SP_ADD_Instr {
   unsigned int rn: 3;
   unsigned int rm: 4;
   unsigned int N: 1;
   unsigned int op: 2;
   unsigned int type: 6;
   unsigned int pad: 16;  
};

typedef SP_ADD_Instr SP_CMP_Instr;
typedef SP_ADD_Instr SP_MOV_Instr;

struct SP_BX_Instr {
   unsigned int unused: 3;
   unsigned int rm: 4;
   unsigned int N: 1;
   unsigned int op: 2;
   unsigned int type: 6;
   unsigned int pad: 16; 
};

typedef SP_BX_Instr SP_BLX_Instr;

struct SP_Type {
   union {
      struct {
         unsigned int data: 10;
         unsigned int type_check: 6;
         unsigned int pad: 16;
      } class_type;
      SP_ADD_Instr add;
      SP_CMP_Instr cmp;
      SP_MOV_Instr mov;
      SP_BX_Instr bx;
      SP_BLX_Instr blx;
   } instr;
};

struct LD_ST_Type {
   union {
      struct {
         unsigned int data: 9;
         unsigned int opB: 3;
         unsigned int opA: 4;
         unsigned int pad: 16;
      } class_type;
      /* Register struct for Load Store Types */
      struct {
         unsigned int rt: 3;
         unsigned int rn: 3;
         unsigned int rm: 3;
         unsigned int op: 7;
         unsigned int pad: 16;
      } ld_st_reg;
      /* Immediate struct for Load Store Types */
      struct {
         unsigned int rt: 3;
         unsigned int rn: 3;
         unsigned int imm: 5;
         unsigned int op: 5;
         unsigned int pad: 16;
      } ld_st_imm;
   } instr;
};

/* Misc. Types */

struct MISC_ADD_Instr {
   unsigned int imm: 7;
   unsigned int op: 5;
   unsigned int type: 4;
   unsigned int pad: 16;
};

typedef MISC_ADD_Instr MISC_SUB_Instr;

struct MISC_SXTH_Instr {
   unsigned int rd: 3;
   unsigned int rm: 3;
   unsigned int op: 6;
   unsigned int type: 4;
   unsigned int pad: 16;
};

typedef MISC_SXTH_Instr MISC_SXTB_Instr;
typedef MISC_SXTH_Instr MISC_UXTH_Instr;
typedef MISC_SXTH_Instr MISC_UXTB_Instr;
typedef MISC_SXTH_Instr MISC_REV_Instr;
typedef MISC_SXTH_Instr MISC_REV16_Instr;
typedef MISC_SXTH_Instr MISC_REVSH_Instr;

struct MISC_PUSH_Instr {
   unsigned int reg_list: 8;
   unsigned int m: 1;
   unsigned int op: 3;
   unsigned int type: 4;
   unsigned int pad: 16;
};

typedef MISC_PUSH_Instr MISC_POP_Instr;

struct MISC_BKPT_Instr {
   unsigned int imm: 8;
   unsigned int op: 4;
   unsigned int type: 4;
   unsigned int pad: 16;
};

struct MISC_Type {
   union {
      struct {
         unsigned int data: 12;
         unsigned int type_check: 4;
         unsigned int pad: 16;
      } class_type;
      MISC_ADD_Instr add;
      MISC_SUB_Instr sub;
      MISC_SXTH_Instr sxth;
      MISC_SXTB_Instr sxtb;
      MISC_UXTH_Instr uxth;
      MISC_UXTB_Instr uxtb;
      MISC_REV_Instr rev;
      MISC_REV16_Instr rev16;
      MISC_REVSH_Instr revsh;
      MISC_PUSH_Instr push;
      MISC_POP_Instr pop;
      MISC_BKPT_Instr bkpt;
   } instr;
};
/* Conditional Type */
struct COND_B_Instr {
   unsigned int imm: 8;
   unsigned int cond: 4;
   unsigned int type: 4;
   unsigned int pad: 16;
}; 

struct COND_Type {
   union {
      struct {
         unsigned int data: 12;
         unsigned int type_check: 4;
         unsigned int pad: 16;
      } class_type;
      COND_B_Instr b;
   } instr;
};

/* Literal Type */
struct LDRL_Instr {
   
};

struct LDRL_Type {
   union {
      struct {
         unsigned int data: 11;
         unsigned int type_check: 5;
         unsigned int pad: 16;
      } class_type;
      LDRL_Instr ldrl;
   } instr;
};

/* ST Multiple Type*/
struct STM_Instr {
  // TODO
};

struct STM_Type {
   union {
      struct {
         unsigned int data: 11;
         unsigned int type_check: 5;
         unsigned int pad: 16;
      } class_type;
      STM_Instr stm;
   } instr;
};

/* LD Multiple Type*/
struct LDM_Instr {
   unsigned int reg_list: 8;
   unsigned int rn: 3;
   unsigned int type: 5;
   unsigned int pad: 16;
};

struct LDM_Type {
   union {
      struct {
         unsigned int data: 11;
         unsigned int type_check: 5;
         unsigned int pad: 16;
      } class_type;
      LDM_Instr ldm;
   } instr;
};

/* Unconditional Type */
struct UNCOND_B_Instr {
   unsigned int imm: 11;
   unsigned int op: 5;
   unsigned int pad: 16;
};

struct UNCOND_Type {
   union {
      struct {
         unsigned int data: 11;
         unsigned int type_check: 5;
         unsigned int pad: 16;
      } class_type;
      UNCOND_B_Instr b;
   } instr;
};


struct ADD_SP_Instr {
  // TODO
};

struct ADD_SP_Type {
   union {
      struct {
         unsigned int data: 11;
         unsigned int type_check: 5;
         unsigned int pad: 16;
      } class_type;
      ADD_SP_Instr add;
   } instr; 
};

class ALL_Types{ 
   public:
      union {
         unsigned int mem;
         ALU_Type alu;
         DP_Type dp;
         SP_Type sp;
         LD_ST_Type ld_st;
         MISC_Type misc;
         COND_Type cond;
         UNCOND_Type uncond;
         LDM_Type ldm;
         STM_Type stm;
         LDRL_Type ldrl;
         ADD_SP_Type addsp;
      } type;
      ALL_Types() {}
      ALL_Types(const unsigned int & _type) {type.mem = _type;}
      /*
       * @param data An ALL_Types that uses data
       * @return returns the Type of instruction it is.
       *
       */
      static void printCond(char byte) {
         switch (byte) {
            case 10:
               cout << "ge";
               break;
            case 11:
               cout << "lt";
               break;
            case 12:
               cout << "gt";
               break;
            case 13:
               cout << "le";
               break;
            default:
               cout << "";
         }
      }
};
      
class Data32 {
private:
  union {
    unsigned int _uint;
    int _int;
    unsigned char _ubyte4[4];
  } d;
public:
  // Data32() {}
  Data32(const unsigned int & _d) {d._uint = _d; }
  Data32(const unsigned char & d0, 
         const unsigned char & d1, 
         const unsigned char & d2, 
         const unsigned char & d3) { 
    set_data_ubyte4(0, d0);
    set_data_ubyte4(1, d1);
    set_data_ubyte4(2, d2);
    set_data_ubyte4(3, d3);
  }
  // Data32 & operator=(const Data32 & _d) { set_data_uint(_d.data_uint()); }
  operator unsigned int() const { return data_uint(); }
  bool operator==(const Data32 & dd) const { return (data_uint() == dd.data_uint()); }
  bool operator!=(const Data32 & dd) const { return (data_uint() != dd.data_uint()); }
  inline void set_data_int(int arg) { 
    d._int = arg; 
  }
  inline int data_int(void) const {
    return d._int; 
  }
  inline void set_data_uint(unsigned int arg) {
    d._uint = arg;
  }
  inline unsigned int data_uint(void) const {
    return d._uint; 
  }
  inline void set_data_ubyte4(int i, unsigned char arg) {
    d._ubyte4[3-i] = arg;
  }
  inline unsigned char data_ubyte4(int i) const {
    return d._ubyte4[3-i]; 
  }
  static void printD(const Data32 d) {
    cout << hex << d.data_uint() << endl;
  }
};

class Register {
private:
  Data32 d;
public:
  Register() : d(0) {}
  Register(Data32 _d) : d(_d) {}
  void write(unsigned int val) {
    d = Data32(val);
  }
  operator Data32() const { return d; }
  operator unsigned int() const { return d.data_uint(); }
  Register & operator=(unsigned int val) {
    write(val);
  }
};

class Options {
public:
  Options() : program(false), dump(false), instrs(false), writes(false),
              stats(false) {}
  bool program;
  bool dump;
  bool instrs;
  bool writes;
  bool stats;
};

extern Register pc;
extern Options opts;

void parse(const char * file);
void execute();  
#endif
