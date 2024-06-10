#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleMvtxPoolInput.h>

#include <phool/recoConsts.h>

#include <ffarawmodules/StreamingCheck.h>

#include <ffamodules/HeadReco.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/SyncReco.h>

#include <mvtxgl1check/MvtxL1Fast.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libMvtxGl1Check.so)


void Fun4All_MvtxGl1Check(const int nEvents = 1000,
                            const std::string &input_gl1file = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/gl1.list",
                             const std::string &input_mvtxfile00 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/mvtx0.list",
                             const std::string &input_mvtxfile01 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/mvtx1.list",
                             const std::string &input_mvtxfile02 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/mvtx2.list",
                             const std::string &input_mvtxfile03 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/mvtx3.list",
                             const std::string &input_mvtxfile04 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/mvtx4.list",
                             const std::string &input_mvtxfile05 = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/condor/filelist/45252_physics/mvtx5.list")
{
    const std::string output_name = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045252_physics/gl1_check_mvtx_all_" + std::to_string(nEvents) + ".root";
    
    
    // Initialize fun4all
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(1);

    // Input manager
    Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
    // in->Verbosity(0);


    // GL1 input
    int igl1 = 0;
    std::vector<std::string> gl1_files = {input_gl1file};
    for ( auto infile : gl1_files)
    {
        SingleGl1PoolInput *sgpi = new SingleGl1PoolInput("GL1_" + to_string(igl1));
        sgpi->AddListFile(infile);
        in->registerStreamingInput(sgpi, InputManagerType::GL1);
        igl1++;
    }

    // MVTX input
    std::vector<std::string> mvtx_files = {input_mvtxfile00, input_mvtxfile01, input_mvtxfile02, input_mvtxfile03, input_mvtxfile04, input_mvtxfile05};
    int iflx = 0;
    for (auto infile : mvtx_files)
    {
       SingleMvtxPoolInput  * mvtx_sngl = new SingleMvtxPoolInput("MVTX_" + to_string(iflx));
        mvtx_sngl->SetBcoRange(1000);
        mvtx_sngl->SetNegativeBco(1000);
        mvtx_sngl->AddListFile(infile);

       in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
         iflx++;
    }

    
        se->registerInputManager(in);

    SyncReco *sync = new SyncReco();
    se->registerSubsystem(sync);

    HeadReco *head = new HeadReco();
    se->registerSubsystem(head);

    FlagHandler *flag = new FlagHandler();
    se->registerSubsystem(flag);

    MvtxL1Fast *mgc = new MvtxL1Fast();
    mgc->SetOutputfile(output_name);
    se->registerSubsystem(mgc);


    se->run(nEvents);
    se->End();
    delete se;
    gSystem->Exit(0);

}
