# parseYaml.py
import os
import yaml

config_file = open("datasetConfig.yml")

config = yaml.safe_load(config_file)

CMSSW_BASE = os.getenv('CMSSW_BASE') + "/"

print("Found CMSSW_BASE as", CMSSW_BASE)

for eraType in ["signal"]:
   for d in config[eraType]["datasets"]:

      dir = "crabJobConfigs/" + str(config[eraType]["year"]) + "/" 

      newCRABConfFile  = dir + "crabTest_" + d + "_" + config[eraType]["prodtag"] + "_cfg.py"  
      print(newCRABConfFile)

      requestName      = "test_" + config["requestname"] + "_" + d + "_" + config[eraType]["prodtag"]  
      crabPythonScript = CMSSW_BASE + config[eraType]["script"]      # crab python script: one per era (will say which jmeCorrection to use)
      dasName          = config[eraType]["datasets"][d] # DAS name
      inputDBS         = config[eraType]["inputDBS"]    # inputDBS
      outputTag        = 'Analyzer_' + d + "_" + config[eraType]["prodtag"]  

      # Make one new template file per dataset
      with open('crab_template_cfg.py', 'r') as templatefile:
         t1 = templatefile.read()
         with open(newCRABConfFile, 'w+') as writefile:

            t2 = t1.replace('REQUEST_NAME',            requestName)
            t3 = t2.replace('CRAB_PYTHON_SCRIPT_PATH', crabPythonScript)
            t4 = t3.replace('DAS_NAME',                dasName)
            t5 = t4.replace('INPUT_DBS',               inputDBS)
            t6 = t5.replace('OUTPUT_TAG',              outputTag)

            writefile.write(t6)

#-------------------------------------------------------#