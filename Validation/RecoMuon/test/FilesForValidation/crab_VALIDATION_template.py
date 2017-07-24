import sys
print sys.argv

if len(sys.argv) > 0:
    last = sys.argv.pop()
    sys.argv.extend(last.split(","))
    print sys.argv

if hasattr(sys, "argv") == True:
    options.parseArguments()
requestname = options.requestname
workarea = options.workarea
inputdataset = options.inputdataset
psetname = options.psetname
print 'Using channel: %s' % requestname

from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
config.General.requestName = requestname
config.General.workArea = workarea
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = psetname
config.Data.inputDataset= inputdataset

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
config.Data.allowNonValidInputDataset = True
