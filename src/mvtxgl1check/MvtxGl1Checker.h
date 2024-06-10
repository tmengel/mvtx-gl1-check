#ifndef MVTXGL1CHECKER_H
#define MVTXGL1CHECKER_H

#include "MvtxPixelMasker.h"

#include <fun4all/SubsysReco.h>

#include <trackbase/TrkrDefs.h>

#include <ffarawobjects/MvtxRawHit.h>
#include <ffarawobjects/Gl1RawHit.h>
#include <ffarawobjects/Gl1RawHitv2.h>
#include <ffarawobjects/MvtxRawEvtHeader.h>
#include <ffarawobjects/MvtxRawEvtHeaderv1.h>
#include <ffarawobjects/MvtxRawHit.h>
#include <ffarawobjects/MvtxRawHitv1.h>
#include <ffarawobjects/MvtxRawHitContainer.h>
#include <ffarawobjects/MvtxRawHitContainerv1.h>

#include <TTree.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <climits>
#include <memory>

class PHCompositeNode;

class MvtxGl1Checker : public SubsysReco
{
    public:

        MvtxGl1Checker(const std::string& name = "MvtxGl1Checker") : SubsysReco(name) {}
        ~MvtxGl1Checker() override;

        // standard Fun4All functions
        int InitRun(PHCompositeNode*) override;
        int process_event(PHCompositeNode*) override;
        int End(PHCompositeNode*) override;

        void SetStrobeLength(double val) { m_strobe_length = val;}
        void SetBcoRange(int val) { m_bco_range = val;}
        
        void SetOutputfile(const std::string& name) { m_write_output = true; m_outputfile = name;}
        
        void SkipHits(bool val) { m_skip_hits = val; }

    private:

        const double GTM_FREQUENCY{9.4e6}; // 9.4 MHz
        const int m_update_interval{10000};

        // configuration
        double m_strobe_length{1.0/101000.0}; // 101kHz
        int m_bco_range{1000};

        // optional output
        bool m_write_output{false};
        std::string m_outputfile{""};

        // skip hits
        bool m_skip_hits{false};

        // mask hot pixels
        MvtxPixelMasker * m_hot_pixel_masker{nullptr};

        MvtxRawEvtHeaderv1 * m_mvtx_raw_event_header{nullptr};
        MvtxRawHitContainerv1 * m_mvtx_raw_hit_container{nullptr};
        Gl1RawHitv2 * m_gl1_raw_hit{nullptr};

        // private methods
        int setup_new_event(PHCompositeNode* topNode);
        double delta_t(const uint64_t a, const uint64_t b);


        TTree * m_tree{nullptr};
        // main tree variables
        int m_gl1_sequence{0};
        std::vector<uint64_t> m_gl1_bco{}; // really is only one value but we need a vector for the TTree
        int m_gl1_in_hb{0};
        
        int m_num_mvtx_l1s{0};
        std::vector<uint64_t> m_mvtx_lls{};

        int m_num_mvtx_hits{0};
        std::vector<uint64_t> m_mvtx_hit_pixelkeys{};
        std::vector<int> m_mvtx_hit_strobe_idxs{};
        

        TTree * m_stats_tree{nullptr};
        uint64_t m_first_gl1{0};
        uint64_t m_prev_gl1{0};
        double m_stats_total_time{0.0};

        int m_stats_num_gl1s_processed{0};
        int m_stats_num_mvtx_lls_processed{0};
        int m_stats_num_mvtx_strobes_processed{0};

        int m_stats_num_l1s_out_of_range{0};
        int m_stats_num_hb_without_hits{0};
        int m_stats_num_dropped_gl1s{0};
};

#endif
