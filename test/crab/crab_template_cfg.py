# crab_template_cfg.py
# Variables like 'REQUEST_NAME' are replaced using a configuration .yml
# and the parseYaml.py wrapper

from WMCore.Configuration import Configuration
from CRABClient.UserUtilities import config

config = Configuration()

config.section_("General")
config.General.workArea     = 'crab_projects'
config.General.requestName  = 'REQUEST_NAME'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName   = 'CRAB_PYTHON_SCRIPT_PATH'
config.JobType.outputFiles = ['OUTPUT_FILE_NAME']

config.section_("Data")
config.Data.inputDataset = 'DAS_NAME'
config.Data.inputDBS     = 'INPUT_DBS'
config.Data.unitsPerJob  = 2
config.Data.splitting    = 'FileBased'

config.Data.outLFNDirBase    = '/store/user/skkwan/Phase2EGammaAnalyzer'
config.Data.publication      = True
config.Data.outputDatasetTag = 'OUTPUT_TAG'

# config.JobType.maxJobRuntimeMin = 300
# config.JobType.numCores = 4
# config.JobType.maxMemoryMB = 9000

config.section_("Site")
config.Site.storageSite = "T2_US_Wisconsin"