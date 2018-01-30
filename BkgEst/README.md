# Background estimation - emu method
   * Analysis codes for Drell-Yan differential cross section measurements with 2016 dataset
   * Ntuple-maker: https://github.com/KyeongPil-Lee/NtupleMaker/tree/80X

## Event selection
	# -- Dimuon event selection -- #
	root -l -b -q 'MuMuSelection.C++(type)' >&log&

	# -- Dielectron event selection -- #
	root -l -b -q 'EESelection.C++(type)' >&log&

	# -- e-mu event selection -- #
	root -l -b -q 'EMuSelection.C++(type)' >&log&

   * For type you can choose in each event selection, please see:
      * Dimuon case     : https://github.com/DalminPai/DrellYan2016/blob/master/BkgEst/EventSelection/MuMuSelection.C#L48-L92
      * Dielectron case : https://github.com/DalminPai/DrellYan2016/blob/master/BkgEst/EventSelection/EESelection.C#L42-L86
      * e-mu case       : https://github.com/DalminPai/DrellYan2016/blob/master/BkgEst/EventSelection/EMuSelection.C#L48-L85

## Background estimation using emu method
	# -- Check e-mu distributions -- #
	root -l -b -q 'emuCheck.cc'

	# -- Estimate non QCD background -- #
	root -l -b -q 'estimateBkg.cc'

	# -- Switch to data-driven background -- #
	root -l -b -q 'signal.cc'

   * You should use proper input root file for each macro
