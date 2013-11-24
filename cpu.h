#include <stdint.h>

typedef enum opcode_e {
  OPCODE_NOP = 0x00,
  OPCODE_SET = 0x01,
  OPCODE_CPY = 0x02,
  OPCODE_DRB = 0x03,
  OPCODE_DWB = 0x04,
  OPCODE_SRB = 0x05,
  OPCODE_SWB = 0x06,
  OPCODE_ADD = 0x07,
  OPCODE_MUL = 0x08,
  OPCODE_AND = 0x09,
  OPCODE_OR  = 0x0a,
  OPCODE_XOR = 0x0b,
  OPCODE_JMP = 0x0c,
  OPCODE_JNE = 0x0d,
  OPCODE_END = 0xff,
} opcode_t;

typedef struct cpu_s {
  uint32_t ticks;
  uint32_t eip;
  uint8_t running;
  uint32_t registers[8];
  uint32_t image_size;
  uint8_t* image;
} cpu_t;

uint32_t read_uint32(uint8_t* ptr);

void cpu_init(cpu_t* cpu);
int cpu_tick(cpu_t* cpu);
int cpu_run(cpu_t* cpu);
