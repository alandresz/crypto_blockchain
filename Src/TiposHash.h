
#ifndef _TiposHash_INCLUDED_
#define _TiposHash_INCLUDED_

enum class TiposHash {
	clavehash256,
	clavefirma,
};

enum class StatusBlock {
	BlockSinDatos,
	BlockBadAlloc,
	BlockCambiosPendientes,
	BlockPendienteCadena_prehash,
	BlockCalculadoHash,
	BlockCalculadoCadena_prehash,
	BlockEstadoError,
	BlockEstadoOk,
};

enum class LargoHash : unsigned int { LargoHashFirma = 40, LargoHashEstandar = 64 };
// Estudiar para que no sean int sino size_t

/* Pasarlo a enum Class
const size_t LargoHashEstandar = 64;
const size_t LargoHashFirma    = 40;	// Hash Pública de la Cuenta// https://stackoverflow.com/questions/2268749/defining-global-constant-in-c
*/
// Análisis de Pro vs Contras contra #define y otras formas

#endif
