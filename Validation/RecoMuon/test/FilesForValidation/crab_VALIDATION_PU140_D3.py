from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
config.General.requestName = 'Val_900_pre1_PU140_D3_2'
config.General.workArea = 'crabProjects_VAL_PU140_D3_2'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName ='Val_2023D3.py'
config.Data.inputDataset='/RelValZMM_14/CMSSW_9_0_0_pre1-PU25ns_90X_upgrade2023_realistic_v0_2023D3PU140-v1/GEN-SIM-RECO'

config.Data.splitting = 'FileBased'
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 1
#NJOBS = 10000  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.
#config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
#config.Data.outputPrimaryDataset = 'NuGunDR'
config.Data.useParent = True

#config.Data.ignoreLocality = True
#config.Site.whitelist   = ['T2_US_UCSD', 'T2_US_Vanderbilt', 'T2_CH_CSCS','T2_DE_DESY','T2_ES_IFCA']
config.Site.storageSite = 'T2_IT_Bari'
#config.Site.whitelist = ['T2_IT_Bari']
config.JobType.maxMemoryMB=3500