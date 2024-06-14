#ifndef MVTXL1FAST_H
#define MVTXL1FAST_H

#include <fun4all/SubsysReco.h>

#include <TTree.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <climits>
#include <memory>

class PHCompositeNode;

class MvtxL1Fast : public SubsysReco
{
    public:

        MvtxL1Fast(const std::string& name = "MvtxL1Fast") : SubsysReco(name) {}
        ~MvtxL1Fast() override;

        // standard Fun4All functions
        int InitRun(PHCompositeNode*) override;
        int process_event(PHCompositeNode*) override;
        int End(PHCompositeNode*) override;
        void SetOutputfile(const std::string& name) { m_outputfile = name;}

    private:

        // optional output
        std::string m_outputfile{""};


        TTree * m_tree{nullptr};
        int m_gl1_sequence{0};
        std::vector<uint64_t> m_gl1_bco{};

        int m_n_l1s{0};
        std::vector<uint64_t> m_mvtx_lls{};

        int m_n_strobe_bcos{0};
        std::vector<uint64_t> m_strobe_bcos{};

        int m_n_l1_bcos{0};
        std::vector<uint64_t> m_l1_bcos{};

        int m_n_no_hits{0};

        int m_total_n_l1s{0};
        int m_total_n_strobe_bcos{0};
        int m_total_n_l1_bcos{0};

};

#endif
