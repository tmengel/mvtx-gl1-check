#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// main function
int CombineFelixFiles()
{
    
    const std::string filename0 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/gl1_check_dbco_100_mvtx0_-1.root";
    const std::string filename1 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/gl1_check_dbco_100_mvtx1_-1.root";
    const std::string filename2 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/gl1_check_dbco_100_mvtx2_-1.root";
    const std::string filename3 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/gl1_check_dbco_100_mvtx3_-1.root";
    const std::string filename4 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/gl1_check_dbco_100_mvtx4_-1.root";
    const std::string filename5 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/gl1_check_dbco_100_mvtx5_-1.root";

    const std::string treename = "T";

    TFile *f0 = new TFile(filename0.c_str() , "READ");
    TFile *f1 = new TFile(filename1.c_str() , "READ");
    TFile *f2 = new TFile(filename2.c_str() , "READ");
    TFile *f3 = new TFile(filename3.c_str() , "READ");
    TFile *f4 = new TFile(filename4.c_str() , "READ");
    TFile *f5 = new TFile(filename5.c_str() , "READ");

    TTree *t0 = (TTree*)f0->Get(treename.c_str());
    TTree *t1 = (TTree*)f1->Get(treename.c_str());
    TTree *t2 = (TTree*)f2->Get(treename.c_str());
    TTree *t3 = (TTree*)f3->Get(treename.c_str());
    TTree *t4 = (TTree*)f4->Get(treename.c_str());
    TTree *t5 = (TTree*)f5->Get(treename.c_str());

    if(!f0->IsOpen() || !t0 || !f1->IsOpen() || !t1 || !f2->IsOpen() || !t2 || !f3->IsOpen() || !t3 || !f4->IsOpen() || !t4 || !f5->IsOpen() || !t5)
    {
        std::cerr << "Error: could not open file " << filename0 << std::endl;
        return 1;
    }


    std::vector<uint64_t> * gl1_bco_0  = 0;
    std::vector<uint64_t> * gl1_bco_1  = 0;
    std::vector<uint64_t> * gl1_bco_2  = 0;
    std::vector<uint64_t> * gl1_bco_3  = 0;
    std::vector<uint64_t> * gl1_bco_4  = 0;
    std::vector<uint64_t> * gl1_bco_5  = 0;
    std::vector<uint64_t> * mvtx_bco_0  = 0;
    std::vector<uint64_t> * mvtx_bco_1  = 0;
    std::vector<uint64_t> * mvtx_bco_2  = 0;
    std::vector<uint64_t> * mvtx_bco_3  = 0;
    std::vector<uint64_t> * mvtx_bco_4  = 0;
    std::vector<uint64_t> * mvtx_bco_5  = 0;

    t0->SetBranchAddress("gl1_bco", &gl1_bco_0);
    t0->SetBranchAddress("mvtx_lls", &mvtx_bco_0);

    t1->SetBranchAddress("gl1_bco", &gl1_bco_1);
    t1->SetBranchAddress("mvtx_lls", &mvtx_bco_1);
    
    t2->SetBranchAddress("gl1_bco", &gl1_bco_2);
    t2->SetBranchAddress("mvtx_lls", &mvtx_bco_2);
    
    t3->SetBranchAddress("gl1_bco", &gl1_bco_3);
    t3->SetBranchAddress("mvtx_lls", &mvtx_bco_3);
    
    t4->SetBranchAddress("gl1_bco", &gl1_bco_4);
    t4->SetBranchAddress("mvtx_lls", &mvtx_bco_4);
    
    t5->SetBranchAddress("gl1_bco", &gl1_bco_5);
    t5->SetBranchAddress("mvtx_lls", &mvtx_bco_5);

    int n_events0 = t0->GetEntries();
    int n_events1 = t1->GetEntries();
    int n_events2 = t2->GetEntries();
    int n_events3 = t3->GetEntries();
    int n_events4 = t4->GetEntries();
    int n_events5 = t5->GetEntries();

    // get min number of events
    int n_events = std::max({n_events0, n_events1, n_events2, n_events3, n_events4, n_events5});
    std::cout << "Number of events: " << n_events << std::endl;
    // const int n_bins = 7; // 7 for inclusive, 6 for exclusive
    // TH1D * h1_stave_avg_gl1_efficiency = new TH1D("h1_stave_avg_gl1_efficiency", "FELIX GL1 Efficiency", n_bins, -0.5, 6.5);

    std::string output_filename = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/all_felixs.root";
    TFile *f = new TFile(output_filename.c_str(), "RECREATE");
    std::vector<uint64_t> gl1_bco_all{};
    std::vector<uint64_t> mvtx_l1s_0{};
    std::vector<uint64_t> mvtx_l1s_1{};
    std::vector<uint64_t> mvtx_l1s_2{};
    std::vector<uint64_t> mvtx_l1s_3{};
    std::vector<uint64_t> mvtx_l1s_4{};
    std::vector<uint64_t> mvtx_l1s_5{};
    TTree * t = new TTree("T", "T");
    t->Branch("gl1_bco", &gl1_bco_all);
    t->Branch("mvtx_l1s_0", &mvtx_l1s_0);
    t->Branch("mvtx_l1s_1", &mvtx_l1s_1);
    t->Branch("mvtx_l1s_2", &mvtx_l1s_2);
    t->Branch("mvtx_l1s_3", &mvtx_l1s_3);
    t->Branch("mvtx_l1s_4", &mvtx_l1s_4);
    t->Branch("mvtx_l1s_5", &mvtx_l1s_5);


    int n_not_matching = 0;
    for (int i = 0; i < n_events; i++)
    {
        bool have0 = false;
        if(i < n_events0){
            t0->GetEntry(i);
            have0 = true;
        }
        bool have1 = false;
        if(i < n_events1){
            t1->GetEntry(i);
            have1 = true;
        }

        bool have2 = false;
        if(i < n_events2){
            t2->GetEntry(i);
            have2 = true;
        }

        bool have3 = false;
        if(i < n_events3){
            t3->GetEntry(i);
            have3 = true;
        }

        bool have4 = false;
        if(i < n_events4){
            t4->GetEntry(i);
            have4 = true;
        }

        bool have5 = false;
        if(i < n_events5){
            t5->GetEntry(i);
            have5 = true;
        }

        std::vector<uint64_t> non_zero_gl1_bcos{};
        if(gl1_bco_0->size() > 0 && have0)
        {
            non_zero_gl1_bcos.push_back(gl1_bco_0->at(0));
        }
        if(gl1_bco_1->size() > 0 && have1)
        {
            non_zero_gl1_bcos.push_back(gl1_bco_1->at(0));
        }
        if(gl1_bco_2->size() > 0 && have2)
        {
            non_zero_gl1_bcos.push_back(gl1_bco_2->at(0));
        }
        if(gl1_bco_3->size() > 0 && have3)
        {
            non_zero_gl1_bcos.push_back(gl1_bco_3->at(0));
        }
        if(gl1_bco_4->size() > 0 && have4)
        {
            non_zero_gl1_bcos.push_back(gl1_bco_4->at(0));
        }
        if(gl1_bco_5->size() > 0 && have5)
        {
            non_zero_gl1_bcos.push_back(gl1_bco_5->at(0));
        }

       

        bool all_match = true;
        for (int j = 0; j < non_zero_gl1_bcos.size(); j++)
        {
            if(non_zero_gl1_bcos.at(j) != non_zero_gl1_bcos.at(0))
            {
                all_match = false;
                break;
            }
        }

        if(!all_match)
        {
            std::cerr << "Error: GL1 BCOs do not match in event " << i << std::endl;
            n_not_matching++;
            continue;
        }

        gl1_bco_all.clear();
        mvtx_l1s_0.clear();
        mvtx_l1s_1.clear();
        mvtx_l1s_2.clear();
        mvtx_l1s_3.clear();
        mvtx_l1s_4.clear();
        mvtx_l1s_5.clear();


        uint64_t gl1_bco_all_val = non_zero_gl1_bcos.at(0);
        gl1_bco_all.push_back(gl1_bco_all_val);

        for (int il1 = 0; il1 < mvtx_bco_0->size(); il1++)
        {
            if(have0)
            {
                mvtx_l1s_0.push_back(mvtx_bco_0->at(il1));
            }
        }
        for (int il1 = 0; il1 < mvtx_bco_1->size(); il1++)
        {
            if(have1)
            {
                mvtx_l1s_1.push_back(mvtx_bco_1->at(il1));
            }
        }
        for (int il1 = 0; il1 < mvtx_bco_2->size(); il1++)
        {
            if(have2)
            {
                mvtx_l1s_2.push_back(mvtx_bco_2->at(il1));
            }
        }
        for (int il1 = 0; il1 < mvtx_bco_3->size(); il1++)
        {
            if(have3)
            {
                mvtx_l1s_3.push_back(mvtx_bco_3->at(il1));
            }
        }
        for (int il1 = 0; il1 < mvtx_bco_4->size(); il1++)
        {
            if(have4)
            {
                mvtx_l1s_4.push_back(mvtx_bco_4->at(il1));
            }
        }
        for (int il1 = 0; il1 < mvtx_bco_5->size(); il1++)
        {
            if(have5)
            {
                mvtx_l1s_5.push_back(mvtx_bco_5->at(il1));
            }
        }

        if (mvtx_l1s_0.size() ==0){
            mvtx_l1s_0.push_back(0);
        }
        if (mvtx_l1s_1.size() ==0){
            mvtx_l1s_1.push_back(0);
        }
        if (mvtx_l1s_2.size() ==0){
            mvtx_l1s_2.push_back(0);
        }
        if (mvtx_l1s_3.size() ==0){
            mvtx_l1s_3.push_back(0);
        }
        if (mvtx_l1s_4.size() ==0){
            mvtx_l1s_4.push_back(0);
        }
        if (mvtx_l1s_5.size() ==0){
            mvtx_l1s_5.push_back(0);
        }

        t->Fill();

    }

    f->cd();
    f->Write();
    f->Close();


    std::cout <<"All done" << std::endl;
    std::cout << "Number of events with non-matching GL1 BCOs: " << n_not_matching << std::endl;

    // cleanup
    f0->Close();
    f1->Close();
    f2->Close();
    f3->Close();
    f4->Close();
    f5->Close();
    return 0;
}


int SyncFelixFiles()
{
    
    const std::string filename = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/all_felixs.root";
    const std::string treename = "T";

    TFile *f = new TFile(filename.c_str() , "READ");
    TTree *t = (TTree*)f->Get(treename.c_str());
    if (!f->IsOpen() || !t)
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }
    


    std::vector<uint64_t> * gl1_bco  = 0;
    std::vector<uint64_t> * mvtx_l1s_0  = 0;
    std::vector<uint64_t> * mvtx_l1s_1  = 0;
    std::vector<uint64_t> * mvtx_l1s_2  = 0;
    std::vector<uint64_t> * mvtx_l1s_3  = 0;
    std::vector<uint64_t> * mvtx_l1s_4  = 0;
    std::vector<uint64_t> * mvtx_l1s_5  = 0;

    t->SetBranchAddress("gl1_bco", &gl1_bco);
    t->SetBranchAddress("mvtx_l1s_0", &mvtx_l1s_0);
    t->SetBranchAddress("mvtx_l1s_1", &mvtx_l1s_1);
    t->SetBranchAddress("mvtx_l1s_2", &mvtx_l1s_2);
    t->SetBranchAddress("mvtx_l1s_3", &mvtx_l1s_3);
    t->SetBranchAddress("mvtx_l1s_4", &mvtx_l1s_4);
    t->SetBranchAddress("mvtx_l1s_5", &mvtx_l1s_5);

    int n_events = t->GetEntries();
    std::cout << "Number of events: " << n_events << std::endl;

    std::vector<uint64_t> gl1_missed_0{};
    std::vector<uint64_t> gl1_missed_1{};
    std::vector<uint64_t> gl1_missed_2{};
    std::vector<uint64_t> gl1_missed_3{};
    std::vector<uint64_t> gl1_missed_4{};
    std::vector<uint64_t> gl1_missed_5{};

    std::vector<uint64_t> mvtx_l1_extra_0{};
    std::vector<uint64_t> mvtx_l1_extra_1{};
    std::vector<uint64_t> mvtx_l1_extra_2{};
    std::vector<uint64_t> mvtx_l1_extra_3{};
    std::vector<uint64_t> mvtx_l1_extra_4{};
    std::vector<uint64_t> mvtx_l1_extra_5{};

    for (int i = 0; i < n_events; i++)
    {
        t->GetEntry(i);

        if(mvtx_l1s_0->at(0) == 0){
            uint64_t gl1_bco_val = gl1_bco->at(0) & 0xFFFFFFFFFF;
            gl1_missed_0.push_back(gl1_bco_val);
        }
        if(mvtx_l1s_1->at(0) == 0){
            uint64_t gl1_bco_val = gl1_bco->at(0) & 0xFFFFFFFFFF;
            gl1_missed_1.push_back(gl1_bco_val);
        }
        if(mvtx_l1s_2->at(0) == 0){
            uint64_t gl1_bco_val = gl1_bco->at(0) & 0xFFFFFFFFFF;
            gl1_missed_2.push_back(gl1_bco_val);
        }
        if(mvtx_l1s_3->at(0) == 0){
            uint64_t gl1_bco_val = gl1_bco->at(0) & 0xFFFFFFFFFF;
            gl1_missed_3.push_back(gl1_bco_val);
        }
        if(mvtx_l1s_4->at(0) == 0){
            uint64_t gl1_bco_val = gl1_bco->at(0) & 0xFFFFFFFFFF;
            gl1_missed_4.push_back(gl1_bco_val);    
        }
        if(mvtx_l1s_5->at(0) == 0){
            uint64_t gl1_bco_val = gl1_bco->at(0) & 0xFFFFFFFFFF;
            gl1_missed_5.push_back(gl1_bco_val);
        }

        if(mvtx_l1s_0->size() > 1){
            for (int j = 1; j < mvtx_l1s_0->size(); j++)
            {
                mvtx_l1_extra_0.push_back(mvtx_l1s_0->at(j));
            }
        }
        if(mvtx_l1s_1->size() > 1){
            for (int j = 1; j < mvtx_l1s_1->size(); j++)
            {
                mvtx_l1_extra_1.push_back(mvtx_l1s_1->at(j));
            }
        }
        if(mvtx_l1s_2->size() > 1){
            for (int j = 1; j < mvtx_l1s_2->size(); j++)
            {
                mvtx_l1_extra_2.push_back(mvtx_l1s_2->at(j));
            }
        }
        if(mvtx_l1s_3->size() > 1){
            for (int j = 1; j < mvtx_l1s_3->size(); j++)
            {
                mvtx_l1_extra_3.push_back(mvtx_l1s_3->at(j));
            }
        }
        if(mvtx_l1s_4->size() > 1){
            for (int j = 1; j < mvtx_l1s_4->size(); j++)
            {
                mvtx_l1_extra_4.push_back(mvtx_l1s_4->at(j));
            }
        }
        if(mvtx_l1s_5->size() > 1){
            for (int j = 1; j < mvtx_l1s_5->size(); j++)
            {
                mvtx_l1_extra_5.push_back(mvtx_l1s_5->at(j));
            }
        }
    }

   


    std::vector<uint64_t> gl1_missed_0_after_check{};
    std::vector<uint64_t> gl1_missed_1_after_check{};
    std::vector<uint64_t> gl1_missed_2_after_check{};
    std::vector<uint64_t> gl1_missed_3_after_check{};
    std::vector<uint64_t> gl1_missed_4_after_check{};
    std::vector<uint64_t> gl1_missed_5_after_check{};
    double accepted_diff = 10;



    for (int i = 0; i < gl1_missed_0.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < mvtx_l1_extra_0.size(); j++)
        {
            double diff = (double)gl1_missed_0.at(i) - (double)mvtx_l1_extra_0.at(j);
            if(diff==accepted_diff)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            gl1_missed_0_after_check.push_back(gl1_missed_0.at(i));
        }
    }

    for (int i = 0; i < gl1_missed_1.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < mvtx_l1_extra_1.size(); j++)
        {
            double diff = (double)gl1_missed_1.at(i) - (double)mvtx_l1_extra_1.at(j);
            if(diff==accepted_diff)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            gl1_missed_1_after_check.push_back(gl1_missed_1.at(i));
        }
    }

    for (int i = 0; i < gl1_missed_2.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < mvtx_l1_extra_2.size(); j++)
        {
            double diff = (double)gl1_missed_2.at(i) - (double)mvtx_l1_extra_2.at(j);
            if(diff==accepted_diff)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            gl1_missed_2_after_check.push_back(gl1_missed_2.at(i));
        }
    }

    for (int i = 0; i < gl1_missed_3.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < mvtx_l1_extra_3.size(); j++)
        {
            double diff = (double)gl1_missed_3.at(i) - (double)mvtx_l1_extra_3.at(j);
            if(diff==accepted_diff)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            gl1_missed_3_after_check.push_back(gl1_missed_3.at(i));
        }
    }

    for (int i = 0; i < gl1_missed_4.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < mvtx_l1_extra_4.size(); j++)
        {
            double diff = (double)gl1_missed_4.at(i) - (double)mvtx_l1_extra_4.at(j);
            if(diff==accepted_diff)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            gl1_missed_4_after_check.push_back(gl1_missed_4.at(i));
        }
    }

    for (int i = 0; i < gl1_missed_5.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < mvtx_l1_extra_5.size(); j++)
        {
            double diff = (double)gl1_missed_5.at(i) - (double)mvtx_l1_extra_5.at(j);
            if(diff==accepted_diff)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            gl1_missed_5_after_check.push_back(gl1_missed_5.at(i));
        }
    }


    

    int n_missed_0 = gl1_missed_0_after_check.size();
    int n_missed_1 = gl1_missed_1_after_check.size();
    int n_missed_2 = gl1_missed_2_after_check.size();
    int n_missed_3 = gl1_missed_3_after_check.size();
    int n_missed_4 = gl1_missed_4_after_check.size();
    int n_missed_5 = gl1_missed_5_after_check.size();

    std::cout << "Number of missed L1s for stave 0: " << n_missed_0 << std::endl;
    std::cout << "Number of missed L1s for stave 1: " << n_missed_1 << std::endl;
    std::cout << "Number of missed L1s for stave 2: " << n_missed_2 << std::endl;   
    std::cout << "Number of missed L1s for stave 3: " << n_missed_3 << std::endl;
    std::cout << "Number of missed L1s for stave 4: " << n_missed_4 << std::endl;
    std::cout << "Number of missed L1s for stave 5: " << n_missed_5 << std::endl;

    const int n_bins = 7; // 7 for inclusive, 6 for exclusive
    TH1D * h1_felix_avg_gl1_efficiency = new TH1D("h1_stave_avg_gl1_efficiency", "FELIX GL1 Efficiency", n_bins, -0.5, 6.5);

    

    std::set<uint64_t> gl1_missed_all_set{};
    for (int i = 0; i < gl1_missed_0_after_check.size(); i++)
    {
        gl1_missed_all_set.insert(gl1_missed_0_after_check.at(i));
    }
    for (int i = 0; i < gl1_missed_1_after_check.size(); i++)
    {
        gl1_missed_all_set.insert(gl1_missed_1_after_check.at(i));
    }
    for (int i = 0; i < gl1_missed_2_after_check.size(); i++)
    {
        gl1_missed_all_set.insert(gl1_missed_2_after_check.at(i));
    }
    for (int i = 0; i < gl1_missed_3_after_check.size(); i++)
    {
        gl1_missed_all_set.insert(gl1_missed_3_after_check.at(i));
    }
    for (int i = 0; i < gl1_missed_4_after_check.size(); i++)
    {
        gl1_missed_all_set.insert(gl1_missed_4_after_check.at(i));
    }
    for (int i = 0; i < gl1_missed_5_after_check.size(); i++)
    {
        gl1_missed_all_set.insert(gl1_missed_5_after_check.at(i));
    }

    int n_missed_all = gl1_missed_all_set.size();

    double felix0_efficiency = 1.0 - (double)n_missed_0 / (double)n_events;
    double felix1_efficiency = 1.0 - (double)n_missed_1 / (double)n_events;
    double felix2_efficiency = 1.0 - (double)n_missed_2 / (double)n_events;
    double felix3_efficiency = 1.0 - (double)n_missed_3 / (double)n_events;
    double felix4_efficiency = 1.0 - (double)n_missed_4 / (double)n_events;
    double felix5_efficiency = 1.0 - (double)n_missed_5 / (double)n_events;
    double felix_avg_efficiency = 1.0 - (double)n_missed_all / (double)n_events;
    std::cout << "Felix 0 Efficiency: " << felix0_efficiency << std::endl;
    std::cout << "Felix 1 Efficiency: " << felix1_efficiency << std::endl;
    std::cout << "Felix 2 Efficiency: " << felix2_efficiency << std::endl;
    std::cout << "Felix 3 Efficiency: " << felix3_efficiency << std::endl;
    std::cout << "Felix 4 Efficiency: " << felix4_efficiency << std::endl;
    std::cout << "Felix 5 Efficiency: " << felix5_efficiency << std::endl;
    std::cout << "Felix All Efficiency: " << felix_avg_efficiency << std::endl;


    h1_felix_avg_gl1_efficiency->SetBinContent(0, felix0_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinContent(1, felix1_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinContent(2, felix2_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinContent(3, felix3_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinContent(4, felix4_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinContent(5, felix5_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinContent(6, felix_avg_efficiency*100.0);
    h1_felix_avg_gl1_efficiency->SetBinError(0, 0);
    h1_felix_avg_gl1_efficiency->SetBinError(1, 0);
    h1_felix_avg_gl1_efficiency->SetBinError(2, 0);
    h1_felix_avg_gl1_efficiency->SetBinError(3, 0);
    h1_felix_avg_gl1_efficiency->SetBinError(4, 0);
    h1_felix_avg_gl1_efficiency->SetBinError(5, 0);
    h1_felix_avg_gl1_efficiency->SetBinError(6, 0);

    
    TCanvas * c1 = new TCanvas("c1", "c1", 800, 600);
    h1_felix_avg_gl1_efficiency->SetStats(0);
    h1_felix_avg_gl1_efficiency->SetMarkerStyle(20);
    h1_felix_avg_gl1_efficiency->GetXaxis()->SetTitle("FELIX Number");
    h1_felix_avg_gl1_efficiency->GetYaxis()->SetTitle("Efficiency (%)");
    h1_felix_avg_gl1_efficiency->GetXaxis()->SetRangeUser(-0.5, 6.5);
    h1_felix_avg_gl1_efficiency->GetYaxis()->SetRangeUser(0, 100);

    h1_felix_avg_gl1_efficiency->Draw("E");
    // h1_stave_avg_gl1_efficiency->Draw("E1");

    c1->SaveAs("felix_efficiency.png");

    TFile *f_out = new TFile("/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045392_physics/felix_efficiency.root", "RECREATE");
    h1_felix_avg_gl1_efficiency->Write();
    f_out->Close();



    f->Close();
    return 0;
}
