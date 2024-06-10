#ifndef MVTXPIXELMASKER_H
#define MVTXPIXELMASKER_H

#include "MvtxPixelDefs.h"

#include <ffarawobjects/MvtxRawHit.h>

#include <vector>
#include <climits>
#include <memory>


class MvtxPixelMasker
{
    public:

        MvtxPixelMasker(){}
        ~MvtxPixelMasker(){ clear();}

        typedef std::vector<MvtxPixelDefs::pixelkey> hot_pixel_map_t;

        void load_from_CDB();

        void add_pixel(MvtxPixelDefs::pixelkey key);
        void remove_pixel(MvtxPixelDefs::pixelkey key);
        
        void clear();

        bool is_masked(MvtxRawHit* hit) const;

    private:

        hot_pixel_map_t m_hot_pixel_map {};

};

#endif // MVTXPIXELMASKER_H
