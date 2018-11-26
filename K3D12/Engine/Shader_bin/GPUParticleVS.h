#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer cb
// {
//
//   float4x4 wvpMat;                   // Offset:    0 Size:    64
//
// }
//
// Resource bind info for instanceDrawingData
// {
//
//   struct InstanceData
//   {
//       
//       float4 pos[2];                 // Offset:    0
//       float2 colorSampleCoord;       // Offset:   32
//       float2 pad;                    // Offset:   40
//
//   } $Element;                        // Offset:    0 Size:    48
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// instanceDrawingData               texture  struct         r/o             t0      1 
// cb                                cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_VertexID              0   x           0   VERTID    uint   x   
// SV_InstanceID            0   x           1   INSTID    uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[4], immediateIndexed
dcl_resource_structured t0, 48
dcl_input_sgv v0.x, vertex_id
dcl_input_sgv v1.x, instance_id
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dcl_temps 2
ishl r0.x, v0.x, l(4)
ld_structured_indexable(structured_buffer, stride=48)(mixed,mixed,mixed,mixed) r0.xyzw, v1.x, r0.x, t0.xyzw
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, cb0[0].xyzw, r0.xxxx, r1.xyzw
mad r1.xyzw, cb0[2].xyzw, r0.zzzz, r1.xyzw
mad o0.xyzw, cb0[3].xyzw, r0.wwww, r1.xyzw
bfi r0.x, l(28), l(4), v0.x, l(12)
ld_structured_indexable(structured_buffer, stride=48)(mixed,mixed,mixed,mixed) r0.x, v1.x, r0.x, t0.xxxx
ld_structured_indexable(structured_buffer, stride=48)(mixed,mixed,mixed,mixed) r0.yz, v1.x, l(32), t0.xxyx
add r0.yz, -r0.yyzy, l(0.000000, 1.000000, 1.000000, 0.000000)
mul o1.w, r0.x, r0.y
mov o1.x, r0.z
ld_structured_indexable(structured_buffer, stride=48)(mixed,mixed,mixed,mixed) o1.z, v1.x, l(36), t0.xxxx
mov o1.y, l(1.000000)
utof r0.x, v0.x
add r0.x, -r0.x, l(1.000000)
mul o2.xy, r0.xxxx, l(0.500000, 0.500000, 0.000000, 0.000000)
ret 
// Approximately 18 instruction slots used
#endif

const BYTE _GPUParticleVS_bin[] =
{
     68,  88,  66,  67, 105, 153, 
     12, 177, 226, 250, 100,  11, 
    194, 253,  33,  22, 184,  16, 
     84, 218,   1,   0,   0,   0, 
    204,   6,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    124,   2,   0,   0, 216,   2, 
      0,   0,  76,   3,   0,   0, 
     48,   6,   0,   0,  82,  68, 
     69,  70,  64,   2,   0,   0, 
      2,   0,   0,   0, 148,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
     24,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   5,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,  48,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 144,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 105, 110, 
    115, 116,  97, 110,  99, 101, 
     68, 114,  97, 119, 105, 110, 
    103,  68,  97, 116,  97,   0, 
     99,  98,   0, 171, 144,   0, 
      0,   0,   1,   0,   0,   0, 
    196,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 124,   0, 
      0,   0,   1,   0,   0,   0, 
     32,   1,   0,   0,  48,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0, 236,   0, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0, 252,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 119, 118, 112,  77, 
     97, 116,   0, 102, 108, 111, 
     97, 116,  52, 120,  52,   0, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 243,   0,   0,   0, 
     72,   1,   0,   0,   0,   0, 
      0,   0,  48,   0,   0,   0, 
      2,   0,   0,   0, 244,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  36,  69, 
    108, 101, 109, 101, 110, 116, 
      0,  73, 110, 115, 116,  97, 
    110,  99, 101,  68,  97, 116, 
     97,   0, 112, 111, 115,   0, 
    102, 108, 111,  97, 116,  52, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     98,   1,   0,   0,  99, 111, 
    108, 111, 114,  83,  97, 109, 
    112, 108, 101,  67, 111, 111, 
    114, 100,   0, 102, 108, 111, 
     97, 116,  50,   0,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    161,   1,   0,   0, 112,  97, 
    100,   0,  94,   1,   0,   0, 
    108,   1,   0,   0,   0,   0, 
      0,   0, 144,   1,   0,   0, 
    168,   1,   0,   0,  32,   0, 
      0,   0, 204,   1,   0,   0, 
    168,   1,   0,   0,  40,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  12,   0,   0,   0, 
      3,   0, 208,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  81,   1, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     73,  83,  71,  78,  84,   0, 
      0,   0,   2,   0,   0,   0, 
      8,   0,   0,   0,  56,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   1,   0,   0,  68,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   1,   0,   0,  83,  86, 
     95,  86, 101, 114, 116, 101, 
    120,  73,  68,   0,  83,  86, 
     95,  73, 110, 115, 116,  97, 
    110,  99, 101,  73,  68,   0, 
    171, 171,  79,  83,  71,  78, 
    108,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,   0,   0,   0, 
     98,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,  12,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     67,  79,  76,  79,  82,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171,  83,  72, 
     69,  88, 220,   2,   0,   0, 
     80,   0,   1,   0, 183,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0, 162,   0, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   0,  96,   0,   0,   4, 
     18,  16,  16,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     96,   0,   0,   4,  18,  16, 
     16,   0,   1,   0,   0,   0, 
      8,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   2,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  41,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,  16, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   4,   0, 
      0,   0, 167,   0,   0, 139, 
      2, 131,   1, 128, 131, 153, 
     25,   0, 242,   0,  16,   0, 
      0,   0,   0,   0,  10,  16, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
    140,   0,   0,  11,  18,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  28,   0, 
      0,   0,   1,  64,   0,   0, 
      4,   0,   0,   0,  10,  16, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  12,   0, 
      0,   0, 167,   0,   0, 139, 
      2, 131,   1, 128, 131, 153, 
     25,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,  16, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   6, 112,  16,   0, 
      0,   0,   0,   0, 167,   0, 
      0, 139,   2, 131,   1, 128, 
    131, 153,  25,   0,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
     32,   0,   0,   0,   6, 113, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,  11,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   6,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    130,  32,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  18,  32, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0, 167,   0,   0, 139, 
      2, 131,   1, 128, 131, 153, 
     25,   0,  66,  32,  16,   0, 
      1,   0,   0,   0,  10,  16, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,  36,   0, 
      0,   0,   6, 112,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  34,  32,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     86,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  56,   0, 
      0,  10,  50,  32,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,  63,   0,   0,   0,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  18,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
