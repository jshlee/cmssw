# This is a comment in the submit file
# file name : submit_echo
# universe describes an execution envirorment. Set universe to vanilla
universe = vanilla
# Path of the executable (it can be a system command, your own application, a script, etc...)
executable = /lustrehome/calabria/ValidazioneOfficial3/MuonVal2/CMSSW_9_1_1/src/Validation/RecoMuon/test/FilesForValidation/HARVESTFullGlobal_2023D17_PU200_SNB_MA.py
# The argument to pass to the executable
#arguments = "Hello World!"
# The output of the remote machine running the job will be printed on echo.out
output = echo.out
error = echo.error
log = echo.log
request_cpus = 1
rank = Memory
queue
