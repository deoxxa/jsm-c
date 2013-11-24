#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "cpu.h"

uint32_t read_uint32(uint8_t* ptr) {
  return ptr[0] * 16777216 + ptr[1] * 65536 + ptr[2] * 256 + ptr[3];
}

void cpu_init(cpu_t* cpu) {
  cpu->ticks = 0;
  cpu->eip = 0;
  cpu->running = 1;
  for (int i=0;i<8;++i) {
    cpu->registers[i] = 0;
  }
  cpu->image_size = 0;
  cpu->image = NULL;
}

int cpu_tick(cpu_t* cpu) {
  printf("[%04d] [0x%04x] 0x%02x\n", cpu->ticks++, cpu->eip, cpu->image[cpu->eip]);

  switch (cpu->image[cpu->eip]) {
    case OPCODE_NOP: {
      cpu->eip += 1;
      break;
    }
    case OPCODE_SET: {
      cpu->registers[cpu->image[cpu->eip + 1]] = read_uint32(cpu->image + cpu->eip + 2);
      cpu->eip += 6;
      break;
    }
    case OPCODE_CPY: {
      cpu->registers[cpu->image[cpu->eip + 1]] = cpu->registers[cpu->image[cpu->eip + 2]];
      cpu->eip += 3;
      break;
    }
    case OPCODE_DRB: {
      cpu->registers[cpu->image[cpu->eip + 1]] = *(uint8_t*)(cpu->image + cpu->registers[cpu->image[cpu->eip + 2]]);
      cpu->eip += 3;
      break;
    }
    case OPCODE_DWB: {
      *(uint8_t*)(cpu->image + cpu->registers[cpu->image[cpu->eip + 2]]) = cpu->registers[cpu->image[cpu->eip + 1]];
      cpu->eip += 3;
      break;
    }
    case OPCODE_SRB: {
      cpu->registers[cpu->image[cpu->eip + 1]] = *(uint8_t*)(cpu->image + cpu->image[cpu->eip + 2]);
      cpu->eip += 6;
      break;
    }
    case OPCODE_SWB: {
      *(uint8_t*)(cpu->image + cpu->image[cpu->eip + 2]) = cpu->registers[cpu->image[cpu->eip + 1]];
      cpu->eip += 6;
      break;
    }
    case OPCODE_ADD: {
      cpu->registers[cpu->image[cpu->eip + 1]] = cpu->registers[cpu->image[cpu->eip + 2]] + cpu->registers[cpu->image[cpu->eip + 3]];
      cpu->eip += 4;
      break;
    }
    case OPCODE_MUL: {
      cpu->registers[cpu->image[cpu->eip + 1]] = cpu->registers[cpu->image[cpu->eip + 2]] * cpu->registers[cpu->image[cpu->eip + 3]];
      cpu->eip += 4;
      break;
    }
    case OPCODE_AND: {
      cpu->registers[cpu->image[cpu->eip + 1]] = cpu->registers[cpu->image[cpu->eip + 2]] & cpu->registers[cpu->image[cpu->eip + 3]];
      cpu->eip += 4;
      break;
    }
    case OPCODE_OR: {
      cpu->registers[cpu->image[cpu->eip + 1]] = cpu->registers[cpu->image[cpu->eip + 2]] | cpu->registers[cpu->image[cpu->eip + 3]];
      cpu->eip += 4;
      break;
    }
    case OPCODE_XOR: {
      cpu->registers[cpu->image[cpu->eip + 1]] = cpu->registers[cpu->image[cpu->eip + 2]] ^ cpu->registers[cpu->image[cpu->eip + 3]];
      cpu->eip += 4;
      break;
    }
    case OPCODE_JMP: {
      cpu->eip = read_uint32(cpu->image + cpu->eip + 1);
      break;
    }
    case OPCODE_JNE: {
      if (cpu->registers[cpu->image[cpu->eip + 1]] != cpu->registers[cpu->image[cpu->eip + 2]]) {
        cpu->eip = read_uint32(cpu->image + cpu->eip + 3);
      } else {
        cpu->eip += 7;
      }
      break;
    }
    case OPCODE_END: {
      cpu->running = 0;
      break;
    }
    default: {
      fprintf(stderr, "unknown opcode seen: 0x%.02x\n", cpu->image[cpu->eip]);
      return -1;
    }
  }

  return 0;
}

int cpu_run(cpu_t* cpu) {
  int rc, i;

  fprintf(stderr, "> running cpu\n");

  do {
    rc = cpu_tick(cpu);

    if (rc == -1) {
      return -1;
    }
  } while (cpu->running == 1);

  fprintf(stderr, "> cpu completed running, status is %d\n", rc);
  for (i=0;i<8;++i) {
    fprintf(stderr, "> register %02x: 0x%08x\n", i, cpu->registers[i]);
  }

  return rc;
}
