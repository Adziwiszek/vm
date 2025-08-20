#include <stdio.h>

#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include "compiler.h"

VM vm;

void debugStack() {
  printf("stack count = %d\tstack capacity = %d\n", vm.stack.count,
         vm.stack.capacity);
}

void initVM() {
  initValueArray(&vm.stack);
  upatedStackPointer();
}

void freeVM() {
  freeValueArray(&vm.stack);
  upatedStackPointer();
}

void push(Value value) {
  writeValueArray(&vm.stack, value);
  upatedStackPointer();
}

Value pop() {
  vm.stack.count--;
  vm.stackTop--;
  return *vm.stackTop;
}

InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if(!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value *slot = vm.stack.values; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = pop();                                                          \
    double a = pop();                                                          \
    push(a op b);                                                              \
  } while (false)
#define UNARY_OP(op)                                                      \
  do {                                                                         \
    vm.stack.values[vm.stack.count - 1] =                                      \
        op vm.stack.values[vm.stack.count - 1];                                  \
  } while (false)

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT:
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    case OP_RETURN:
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
      break;
    case OP_NEGATE:
      UNARY_OP(-);
      break;
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

static void upatedStackPointer() {
  vm.stackTop = &vm.stack.values[vm.stack.count];
}
