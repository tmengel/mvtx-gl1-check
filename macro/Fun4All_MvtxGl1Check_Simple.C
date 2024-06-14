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

#include <rawdatatools/RawDataManager.h>
#include <rawdatatools/RawDataDefs.h>
#include <rawdatatools/RawFileUtils.h>

#include <mvtxgl1check/MvtxL1Fast.h>

#include <mvtx/MvtxCombinedRawDataDecoder.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(librawdatatools.so)
R__LOAD_LIBRARY(libMvtxGl1Check.so)

void Fun4All_MvtxGl1Check_Simple(const int iflx,
                                const int nEvents = 10000
                                )
{  
    
    const std::string output_name = "/sphenix/user/tmengel/MVTX/mvtx-gl1-check/rootfiles/00045393_physics/gl1_check_dbco_100_mvtx" + std::to_string(iflx) + "_" + std::to_string(nEvents) + ".root";
    const int run_number=45393;
    const std::string run_type="physics";

    // raw data manager
    RawDataManager * rdm = new RawDataManager();
    rdm->SetDataPath(RawDataDefs::SPHNXPRO_PHYS);
    rdm->SetRunNumber(run_number);
    rdm->SetRunType(run_type);
    rdm->SelectSubsystems({RawDataDefs::MVTX, RawDataDefs::GL1});
    rdm->GetAvailableSubsystems();
    rdm->Verbosity(0);

    // Initialize fun4all
    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(0);

    // Input manager
    Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
    in->Verbosity(0);

    // GL1 input
    int igl1 = 0;
    std::vector<std::string> gl1_files = rdm->GetFiles(RawDataDefs::GL1);
    std::cout<<"Found " << gl1_files.size() << " GL1 files\n ";
    for (const auto &infile : gl1_files)
    {
        SingleGl1PoolInput *sgpi = new SingleGl1PoolInput("GL1_" + to_string(igl1));
        if(!RawFileUtils::isGood(infile)){  continue; }
        std::cout << "Adding file: " << infile << std::endl;
        sgpi->AddFile(infile);
        in->registerStreamingInput(sgpi, InputManagerType::GL1);
        igl1++;
    }

    SingleMvtxPoolInput  * mvtx_sngl = new SingleMvtxPoolInput("MVTX_FLX" + to_string(iflx));
    mvtx_sngl->SetBcoRange(100);
    mvtx_sngl->SetNegativeBco(100);
    std::vector<std::string> mvtx_files = rdm->GetFiles(RawDataDefs::MVTX, RawDataDefs::get_channel_name(RawDataDefs::MVTX, iflx));
    std::cout << "Adding files for flx " << iflx << std::endl;
    for (const auto &infile : mvtx_files)
    {
        if(!RawFileUtils::isGood(infile)){  continue; }
        std::cout << "\tAdding file: " << infile << std::endl;
        mvtx_sngl->AddFile(infile);
    }
    in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
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
