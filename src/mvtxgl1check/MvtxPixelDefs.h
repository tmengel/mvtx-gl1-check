#ifndef MVTXPIXELDEFS_H
#define MVTXPIXELDEFS_H


#include <cstdint>
#include <climits>
#include <memory>

#include <ffarawobjects/MvtxRawHit.h>

namespace MvtxPixelDefs 
{

    typedef uint64_t pixelkey; // (hitsetkey << 32) | hitkey
    static MvtxPixelDefs::pixelkey VOID_PIXEL __attribute__((unused)) = UINT64_MAX;
    static const unsigned int kBitShiftLadder __attribute__((unused)) = 32;

    MvtxPixelDefs::pixelkey gen_pixelkey(const uint32_t hitset_key, const uint32_t hitkey);
    uint32_t get_hitsetkey(const MvtxPixelDefs::pixelkey key);
    uint32_t get_hitkey(const MvtxPixelDefs::pixelkey key);
    MvtxPixelDefs::pixelkey gen_pixelkey_from_coors(const uint8_t layer, const uint8_t stave, const uint8_t chip, const uint16_t row, const uint16_t col);
    MvtxPixelDefs::pixelkey gen_pixelkey(MvtxRawHit *hit);
}

#endif // MVTXPIXELDEFS_H
