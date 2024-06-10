#include "MvtxL1Fast.h"

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

#include <trackbase/MvtxDefs.h>
#include <trackbase/TrkrDefs.h>
#include <trackbase/MvtxEventInfov2.h>
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHitSetContainerv1.h>
#include <trackbase/TrkrHitv2.h>

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

// MvtxL1Fast class
//==============================================================================
MvtxL1Fast::~MvtxL1Fast()
{

}

int MvtxL1Fast::InitRun(PHCompositeNode* /*topNode*/)
{
    std::cout << "MvtxL1Fast::InitRun - Writing output to " << m_outputfile << std::endl;
    
    // Create the output file and trees
    PHTFileServer::get().open(m_outputfile, "RECREATE");
    // main tree
    m_tree = new TTree("T", "T");
    m_tree->OptimizeBaskets();
    m_tree->SetAutoSave(-5e6);
    m_tree->Branch("gl1_sequence", &m_gl1_sequence, "gl1_sequence/I");
    m_tree->Branch("gl1_bco", &m_gl1_bco);
    m_tree->Branch("n_l1s", &m_n_l1s, "n_l1s/I");
    m_tree->Branch("mvtx_lls", &m_mvtx_lls);
    // m_tree->Branch("n_strobe_bcos", &m_n_strobe_bcos, "n_strobe_bcos/I");
    // m_tree->Branch("strobe_bcos", &m_strobe_bcos);
    // m_tree->Branch("n_l1_bcos", &m_n_l1_bcos, "n_l1_bcos/I");
    // m_tree->Branch("l1_bcos", &m_l1_bcos);


    return Fun4AllReturnCodes::EVENT_OK;

}



int MvtxL1Fast::process_event(PHCompositeNode *topNode)
{
    // clear event variables
    m_mvtx_lls.clear();
    m_gl1_bco.clear();
    m_strobe_bcos.clear();
    m_l1_bcos.clear();
    m_n_strobe_bcos = 0;
    m_n_l1_bcos = 0;
    m_n_l1s = 0;



    Gl1RawHitv2 * m_gl1_raw_hit = findNode::getClass<Gl1RawHitv2>(topNode, "GL1RAWHIT");
    if (!m_gl1_raw_hit){ std::cout << PHWHERE << "::" << __func__ << ": Could not get GL1RAWHIT from Node Tree" << std::endl; exit(1); }

    uint64_t gl1_bco = m_gl1_raw_hit->get_bco();
    m_gl1_sequence = m_gl1_raw_hit->getEvtSequence();
    m_gl1_bco.push_back(gl1_bco);

    // get dst nodes
    MvtxRawEvtHeaderv1 * m_mvtx_raw_event_header = findNode::getClass<MvtxRawEvtHeaderv1>(topNode, "MVTXRAWEVTHEADER");
    if(!m_mvtx_raw_event_header){ std::cout << PHWHERE << "::" << __func__ << ": Could not get MVTXRAWEVTHEADER from Node Tree" << std::endl; exit(1); }

    // MvtxEventInfov2* mvtx_event_header = findNode::getClass<MvtxEventInfov2>(topNode, "MVTXEVENTHEADER");
    // if (!mvtx_event_header)
    // {
    //   std::cout << __FILE__ << "::" << __func__ << " - MVTXEVENTHEADER missing, doing nothing." << std::endl;
    //   exit(1);
    // }

    // std::set<uint64_t> strobeList = mvtx_event_header->get_strobe_BCOs();
    // for (auto iterStrobe = strobeList.begin(); iterStrobe != strobeList.end(); ++iterStrobe)
    // {
    //   m_strobe_bcos.push_back(*iterStrobe);
    //   m_n_strobe_bcos++;
    //   std::set<uint64_t> l1List = mvtx_event_header->get_L1_BCO_from_strobe_BCO(*iterStrobe);
    //   for (auto iterL1 = l1List.begin(); iterL1 != l1List.end(); ++iterL1)
    //   {
    //     m_l1_bcos.push_back(static_cast<double>(*iterL1));
    //     m_n_l1_bcos++;
    //   }
    // }


    std::set<uint64_t> mvtx_l1s = m_mvtx_raw_event_header->getMvtxLvL1BCO();
    m_n_l1s = mvtx_l1s.size();
    for(auto iter = mvtx_l1s.begin(); iter != mvtx_l1s.end(); ++iter)
    {
        m_mvtx_lls.push_back(*iter);
    }
    m_tree->Fill();

    m_total_n_l1s+=m_n_l1s;
    // m_total_n_strobe_bcos+=m_n_strobe_bcos;
    // m_total_n_l1_bcos+=m_n_l1_bcos;

    return Fun4AllReturnCodes::EVENT_OK;
}

int MvtxL1Fast::End(PHCompositeNode * /*topNode*/)
{

    
    std::cout << "MvtxL1Fast::End - Writing output to " << m_outputfile << std::endl;
    std::cout << "MvtxL1Fast::End - Total number of L1s: " << m_total_n_l1s << std::endl;
    // std::cout << "MvtxL1Fast::End - Total number of Strobe BCOs: " << m_total_n_strobe_bcos << std::endl;
    // std::cout << "MvtxL1Fast::End - Total number of L1 BCOs: " << m_total_n_l1_bcos << std::endl;
    PHTFileServer::get().cd(m_outputfile);
    PHTFileServer::get().write(m_outputfile);

    return Fun4AllReturnCodes::EVENT_OK;

}
