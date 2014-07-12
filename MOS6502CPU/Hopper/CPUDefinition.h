//
// Hopper Disassembler SDK
//
// (c)2014 - Cryptic Apps SARL. All Rights Reserved.
// http://www.hopperapp.com
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//

#import <Foundation/Foundation.h>
#import "CPUContext.h"
#import "HopperPlugin.h"
#import "CommonTypes.h"

#define MAX_REGISTER_CLASS  16

typedef NS_ENUM(NSUInteger, RegClass) {
    RegClass_CPUState = 0,                // State register
    RegClass_PseudoRegisterSTACK = 1,     // Pseudo registers to simulate the stack

    RegClass_GeneralPurposeRegister = 2,

    RegClass_FirstUserClass = 3,

    // x86
    RegClass_X86_FPU = RegClass_FirstUserClass,
    RegClass_X86_MMX,
    RegClass_X86_SSE,

    // ARM
    RegClass_ARM_VFP_Single = RegClass_FirstUserClass,
    RegClass_ARM_VFP_Double,
    RegClass_ARM_VFP_Quad,
    RegClass_ARM_Media
};


@protocol CPUDefinition <NSObject,HopperPlugin>

/// Build a context for disassembling.
/// This method should be fast, because it'll be called very often.
- (NSObject<CPUContext> *)buildCPUContextForFile:(NSObject<HPDisassembledFile> *)file;

/// Returns an array of NSString of CPU families handled by the plugin.
- (NSArray *)cpuFamilies;
/// Returns an array of NSString of CPU subfamilies handled by the plugin for a given CPU family.
- (NSArray *)cpuSubFamiliesForFamily:(NSString *)family;
/// Returns 32 or 64, according to the family and subFamily arguments.
- (int)addressSpaceWidthInBitsForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily;

/// Default endianess of the CPU.
- (CPUEndianess)endianess;
/// Usually, returns 1, but for the Intel processor, it'll return 2 because we have the Intel and the AT&T syntaxes.
- (NSUInteger)syntaxVariantCount;
/// The number of CPU modes. For instance, 2 for the ARM CPU family: ARM and Thumb modes.
- (NSUInteger)cpuModeCount;

- (NSArray *)syntaxVariantNames;
- (NSArray *)cpuModeNames;

- (NSUInteger)registerClassCount;
- (NSUInteger)registerCountForClass:(NSUInteger)reg_class;
- (NSString *)registerIndexToString:(int)reg ofClass:(RegClass)reg_class withBitSize:(int)size andPosition:(DisasmPosition)position;

/// A weirdness of the Hopper internals. You'll usually simply need to return the "index" argument.
/// This is used by Hopper to handle the fact that operands in Intel and AT&T syntaxes are inverted.
- (NSUInteger)translateOperandIndex:(NSUInteger)index operandCount:(NSUInteger)count accordingToSyntax:(uint8_t)syntaxIndex;

/// Returns a colorized string to be displayed.
/// HPHopperServices protocol provides a very simple colorizer, based on predicates.
- (NSAttributedString *)colorizeInstructionString:(NSAttributedString *)string;

/// Returns a array of bytes that represents a NOP instruction of a given size.
- (NSData *)nopWithSize:(NSUInteger)size andMode:(NSUInteger)cpuMode forFile:(NSObject<HPDisassembledFile> *)file;

/// Return YES if the plugin embed an assembler.
- (BOOL)canAssembleInstructionsForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily;

/// Return YES if the plugin embed a decompiler.
/// Note: you cannot create a decompiler yet, because the main class (ASTNode) is not
/// publicly exposed yet.
- (BOOL)canDecompileProceduresForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily;

@end
