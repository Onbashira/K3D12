#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// Resource bind info for gInputA
// {
//
//   struct Data
//   {
//       
//       float3 v1;                     // Offset:    0
//       float2 v2;                     // Offset:   12
//
//   } $Element;                        // Offset:    0 Size:    20
//
// }
//
// Resource bind info for gInputB
// {
//
//   struct Data
//   {
//       
//       float3 v1;                     // Offset:    0
//       float2 v2;                     // Offset:   12
//
//   } $Element;                        // Offset:    0 Size:    20
//
// }
//
// Resource bind info for gOutput
// {
//
//   struct Data
//   {
//       
//       float3 v1;                     // Offset:    0
//       float2 v2;                     // Offset:   12
//
//   } $Element;                        // Offset:    0 Size:    20
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// gInputA                           texture  struct         r/o             t0      1 
// gInputB                           texture  struct         r/o             t1      1 
// gOutput                               UAV  struct         r/w             u0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_resource_structured t0, 20
dcl_resource_structured t1, 20
dcl_uav_structured u0, 20
dcl_input vThreadID.x
dcl_temps 4
dcl_thread_group 32, 1, 1
ld_structured_indexable(structured_buffer, stride=20)(mixed,mixed,mixed,mixed) r0.y, vThreadID.x, l(16), t0.xxxx
ld_structured_indexable(structured_buffer, stride=20)(mixed,mixed,mixed,mixed) r1.y, vThreadID.x, l(16), t1.xxxx
ld_structured_indexable(structured_buffer, stride=20)(mixed,mixed,mixed,mixed) r2.xyzw, vThreadID.x, l(0), t0.xyzw
mov r0.x, r2.w
ld_structured_indexable(structured_buffer, stride=20)(mixed,mixed,mixed,mixed) r3.xyzw, vThreadID.x, l(0), t1.xyzw
mov r1.x, r3.w
add r2.xyz, r2.xyzx, r3.xyzx
add r0.xy, r0.xyxx, r1.xyxx
mov r2.w, r0.x
store_structured u0.xyzw, vThreadID.x, l(0), r2.xyzw
store_structured u0.x, vThreadID.x, l(16), r0.y
ret 
// Approximately 12 instruction slots used
#endif

const BYTE _ParticleCS_bin[] =
{
     68,  88,  66,  67,  73, 101, 
    214,  56,  17, 163,  49, 181, 
    199, 108,  41,  64, 210, 156, 
    240, 171,   1,   0,   0,   0, 
    248,   4,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    128,   2,   0,   0, 144,   2, 
      0,   0, 160,   2,   0,   0, 
     92,   4,   0,   0,  82,  68, 
     69,  70,  68,   2,   0,   0, 
      3,   0,   0,   0, 180,   0, 
      0,   0,   3,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
     28,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   0,   0,   0,   5,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,  20,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 164,   0,   0,   0, 
      5,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
     20,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 172,   0, 
      0,   0,   6,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,  20,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    103,  73, 110, 112, 117, 116, 
     65,   0, 103,  73, 110, 112, 
    117, 116,  66,   0, 103,  79, 
    117, 116, 112, 117, 116,   0, 
    156,   0,   0,   0,   1,   0, 
      0,   0, 252,   0,   0,   0, 
     20,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
    164,   0,   0,   0,   1,   0, 
      0,   0, 204,   1,   0,   0, 
     20,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
    172,   0,   0,   0,   1,   0, 
      0,   0, 244,   1,   0,   0, 
     20,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     36,   1,   0,   0,   0,   0, 
      0,   0,  20,   0,   0,   0, 
      2,   0,   0,   0, 168,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  36,  69, 
    108, 101, 109, 101, 110, 116, 
      0,  68,  97, 116,  97,   0, 
    118,  49,   0, 102, 108, 111, 
     97, 116,  51,   0,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     53,   1,   0,   0, 118,  50, 
      0, 102, 108, 111,  97, 116, 
     50,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     99,   1,   0,   0,  50,   1, 
      0,   0,  60,   1,   0,   0, 
      0,   0,   0,   0,  96,   1, 
      0,   0, 108,   1,   0,   0, 
     12,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,   5,   0, 
      0,   0,   2,   0, 144,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     45,   1,   0,   0,  36,   1, 
      0,   0,   0,   0,   0,   0, 
     20,   0,   0,   0,   2,   0, 
      0,   0, 168,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  36,   1,   0,   0, 
      0,   0,   0,   0,  20,   0, 
      0,   0,   2,   0,   0,   0, 
    168,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  79,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     83,  72,  69,  88, 180,   1, 
      0,   0,  80,   0,   5,   0, 
    109,   0,   0,   0, 106,   8, 
      0,   1, 162,   0,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  20,   0,   0,   0, 
    162,   0,   0,   4,   0, 112, 
     16,   0,   1,   0,   0,   0, 
     20,   0,   0,   0, 158,   0, 
      0,   4,   0, 224,  17,   0, 
      0,   0,   0,   0,  20,   0, 
      0,   0,  95,   0,   0,   2, 
     18,   0,   2,   0, 104,   0, 
      0,   2,   4,   0,   0,   0, 
    155,   0,   0,   4,  32,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 167,   0, 
      0, 138,   2, 163,   0, 128, 
    131, 153,  25,   0,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,   2,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
      6, 112,  16,   0,   0,   0, 
      0,   0, 167,   0,   0, 138, 
      2, 163,   0, 128, 131, 153, 
     25,   0,  34,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
      2,   0,   1,  64,   0,   0, 
     16,   0,   0,   0,   6, 112, 
     16,   0,   1,   0,   0,   0, 
    167,   0,   0, 138,   2, 163, 
      0, 128, 131, 153,  25,   0, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,   2,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
    167,   0,   0, 138,   2, 163, 
      0, 128, 131, 153,  25,   0, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,   2,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5,  18,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   3,   0,   0,   0, 
      0,   0,   0,   7, 114,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   7,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0, 168,   0, 
      0,   8, 242, 224,  17,   0, 
      0,   0,   0,   0,  10,   0, 
      2,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
    168,   0,   0,   8,  18, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,   2,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  12,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0
};