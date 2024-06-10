#include "MvtxGl1Checker.h"
#include "MvtxPixelMasker.h"
#include "MvtxPixelDefs.h"

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/getClass.h>

#include <ffarawobjects/Gl1RawHit.h>
#include <ffarawobjects/Gl1RawHitv2.h>
#include <ffarawobjects/MvtxRawEvtHeader.h>
#include <ffarawobjects/MvtxRawEvtHeaderv1.h>
#include <ffarawobjects/MvtxRawHit.h>
#include <ffarawobjects/MvtxRawHitv1.h>
#include <ffarawobjects/MvtxRawHitContainer.h>
#include <ffarawobjects/MvtxRawHitContainerv1.h>

#include <ffamodules/CDBInterface.h> 
#include <cdbobjects/CDBTTree.h>

#include <trackbase/MvtxDefs.h>
#include <trackbase/TrkrDefs.h>

#include <string>
#include <vector>
#include <cstdint>
#include <climits>
#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cmath>

// MvtxGl1Checker class
//==============================================================================
MvtxGl1Checker::~MvtxGl1Checker()
{
    if (m_hot_pixel_masker) delete m_hot_pixel_masker;
}

int MvtxGl1Checker::InitRun(PHCompositeNode* /*topNode*/)
{
    // Load the hot pixel map from the CDB
    if(!m_skip_hits)
    {
        m_hot_pixel_masker = new MvtxPixelMasker();
        m_hot_pixel_masker->load_from_CDB();
    }
    if(!m_write_output){ return Fun4AllReturnCodes::EVENT_OK; }

    std::cout << "MvtxGl1Checker::InitRun - Writing output to " << m_outputfile << std::endl;
    
    // Create the output file and trees
    PHTFileServer::get().open(m_outputfile, "RECREATE");
    // main tree
    m_tree = new TTree("hits", "hits");
    m_tree->OptimizeBaskets();
    m_tree->SetAutoSave(-5e6);
    m_tree->Branch("gl1_in_mvtx_hb", &m_gl1_in_hb, "gl1_in_mvtx_hb/I");
    // m_tree->Branch("mvtx_l1_match", &m_mvtx_l1_match, "mvtx_l1_match/I");
    m_tree->Branch("gl1_seq", &m_gl1_sequence, "gl1_seq/I");
    m_tree->Branch("gl1_bcos", &m_gl1_bco);
    m_tree->Branch("num_mvtx_lls", &m_num_mvtx_l1s, "num_mvtx_lls/I");
    m_tree->Branch("mvtx_lls", &m_mvtx_lls);
    if(!m_skip_hits)
    {
        m_tree->Branch("n_mvtx_hits", &m_num_mvtx_hits, "n_mvtx_hits/I");
        m_tree->Branch("mvtx_hit_pixel", &m_mvtx_hit_pixelkeys);
        m_tree->Branch("mvtx_hit_strobe_idx", &m_mvtx_hit_strobe_idxs);
    }

    m_stats_tree = new TTree("stats", "stats");
    m_stats_tree->OptimizeBaskets();
    m_stats_tree->SetAutoSave(-5e6);
    m_stats_tree->Branch("time_of_run", &m_stats_total_time, "time_of_run/D");
    m_stats_tree->Branch("n_gl1s_processed", &m_stats_num_gl1s_processed, "n_gl1s_processed/I");
    m_stats_tree->Branch("n_dropped_gl1s", &m_stats_num_dropped_gl1s, "n_dropped_gl1s/I");
    m_stats_tree->Branch("n_lls_processed", &m_stats_num_mvtx_lls_processed, "n_lls_processed/I");
    m_stats_tree->Branch("n_mismatched_l1s", &m_stats_num_l1s_out_of_range, "n_mismatched_l1s/I");
    if(!m_skip_hits)
    {
        m_stats_tree->Branch("mvtx_strobes_processed", &m_stats_num_mvtx_strobes_processed, "mvtx_strobes_processed/I");
    }


    return Fun4AllReturnCodes::EVENT_OK;

}

int MvtxGl1Checker::setup_new_event(PHCompositeNode* topNode)
{
    // reset event variables
    m_gl1_sequence = 0;
    m_gl1_bco.clear();
    m_gl1_in_hb = 0;
    m_num_mvtx_l1s = 0;
    m_mvtx_lls.clear();
    m_num_mvtx_hits = 0;
    m_mvtx_hit_pixelkeys.clear();
    m_mvtx_hit_strobe_idxs.clear();
    
    // get dst nodes
    m_mvtx_raw_event_header = findNode::getClass<MvtxRawEvtHeaderv1>(topNode, "MVTXRAWEVTHEADER");
    if(!m_mvtx_raw_event_header){ std::cout << PHWHERE << "::" << __func__ << ": Could not get MVTXRAWEVTHEADER from Node Tree" << std::endl; exit(1); }
    if (Verbosity() > 2) {  m_mvtx_raw_event_header->identify();  }

    m_gl1_raw_hit = findNode::getClass<Gl1RawHitv2>(topNode, "GL1RAWHIT");
    if (!m_gl1_raw_hit){ std::cout << PHWHERE << "::" << __func__ << ": Could not get GL1RAWHIT from Node Tree" << std::endl; exit(1); }
    if(Verbosity() > 2) {  m_gl1_raw_hit->identify();  }

    if (!m_skip_hits){ 
    
        m_mvtx_raw_hit_container = findNode::getClass<MvtxRawHitContainerv1>(topNode, "MVTXRAWHIT");
        if (!m_mvtx_raw_hit_container){ std::cout << PHWHERE << "::" << __func__ << ": Could not get MVTXRAWHIT from Node Tree" << std::endl; exit(1); }
        if(Verbosity() > 2) {  m_mvtx_raw_hit_container->identify();  }
    }
    return Fun4AllReturnCodes::EVENT_OK;

}



int MvtxGl1Checker::process_event(PHCompositeNode *topNode)
{
    // clear event variables
    setup_new_event(topNode);

    // get the BCO from the GL1 
    uint64_t gl1_bco = m_gl1_raw_hit->get_bco() & 0xFFFFFFFFFF;
    m_gl1_sequence = m_gl1_raw_hit->getEvtSequence();
    m_gl1_bco.push_back(gl1_bco);

    if(m_stats_num_gl1s_processed == 0)
    {
        m_first_gl1 = gl1_bco;
    }
    m_stats_num_gl1s_processed++;

    // get lls from the MVTX raw event header
    std::set<uint64_t> mvtx_l1s = m_mvtx_raw_event_header->getMvtxLvL1BCO();
    for (auto iter = mvtx_l1s.begin(); iter != mvtx_l1s.end(); ++iter)
    {
        m_mvtx_lls.push_back(*iter);
        m_stats_num_mvtx_lls_processed++;
        m_num_mvtx_l1s++;

        // calculate the time difference between the GL1 BCO and the MVTX L1 BCO
        double diff_bco = static_cast<double>(*iter) - static_cast<double>(gl1_bco);
        if (std::abs(diff_bco) < m_bco_range)
        {
            m_gl1_in_hb = 1;
        }
        else 
        {
            m_stats_num_l1s_out_of_range++;
            if(Verbosity() > 0){ std::cout << "WARNING::MVTX L1 outside of GL1 range. GL1: " << std::hex << gl1_bco << std::dec << " | MVTX L1:"  << std::hex << *iter << std::dec << " (diff: " << diff_bco << ")" << std::endl; }
        }
    }
    if(m_num_mvtx_l1s == 0)
    {
        if (Verbosity() > 0){ std::cout << "WARNING::No MVTX L1s found for GL1: " << std::hex << gl1_bco << std::dec << std::endl; }
    }

    // get strobes from the MVTX hit container
    if(!m_skip_hits)
    {
        std::set<uint64_t> strobe_bcos{};
        for (unsigned int ihit = 0; ihit < m_mvtx_raw_hit_container->get_nhits(); ihit++)
        {
            // get this hit
            auto mvtx_hit = m_mvtx_raw_hit_container->get_hit(ihit);
            
            // make sure it exists
            if (!mvtx_hit){ std::cout << PHWHERE << "::" << __func__ << ": Could not get MVTX hit from container. Hit index: " << ihit << std::endl; continue; }

            // check if it is masked
            if (m_hot_pixel_masker->is_masked(mvtx_hit)){ continue; }

            // super verbose
            if (Verbosity() > 3){  mvtx_hit->identify(); }

            // get the hit info
            uint64_t strobe = mvtx_hit->get_bco();
            strobe_bcos.insert(strobe);

            // generate pixel key
            MvtxPixelDefs::pixelkey this_pixelkey = MvtxPixelDefs::gen_pixelkey(mvtx_hit);

            uint64_t diff_bco = strobe - gl1_bco;
            double time_elapsed = diff_bco * 0.106; // 106 ns RHIC clock
            int index = std::floor(time_elapsed / m_strobe_length);

            m_num_mvtx_hits++;
            m_mvtx_hit_pixelkeys.push_back(this_pixelkey);
            m_mvtx_hit_strobe_idxs.push_back(index);
        }
        if(m_num_mvtx_hits == 0){ m_stats_num_hb_without_hits++; }
        m_stats_num_mvtx_strobes_processed+=strobe_bcos.size();
    }

    if(m_gl1_in_hb == 0){ m_stats_num_dropped_gl1s++; }

    if(m_write_output) { m_tree->Fill(); }

    if(m_stats_num_gl1s_processed % m_update_interval == 0)
    {
        std::cout << "Processed " << m_stats_num_gl1s_processed << " GL1s. Dropped " << m_stats_num_dropped_gl1s << " GL1s (" << static_cast<double>(m_stats_num_dropped_gl1s) / static_cast<double>(m_stats_num_gl1s_processed) * 100 << "%)";
        if(!m_skip_hits){ std::cout << " | Empty HB: " << m_stats_num_hb_without_hits << " (" << static_cast<double>(m_stats_num_hb_without_hits) / static_cast<double>(m_stats_num_gl1s_processed) * 100 << "%)";}
        std::cout << std::endl;
    }

    m_prev_gl1 = gl1_bco;

    return Fun4AllReturnCodes::EVENT_OK;
}

int MvtxGl1Checker::End(PHCompositeNode * /*topNode*/)
{
    if(!m_write_output){ return Fun4AllReturnCodes::EVENT_OK; }

    // fill the stats tree
    m_stats_total_time = static_cast<double>(m_prev_gl1 - m_first_gl1) / GTM_FREQUENCY;
    m_stats_tree->Fill();

    
    std::cout << "MvtxGl1Checker::End - Writing output to " << m_outputfile << std::endl;
    PHTFileServer::get().cd(m_outputfile);
    PHTFileServer::get().write(m_outputfile);

    return Fun4AllReturnCodes::EVENT_OK;

}
