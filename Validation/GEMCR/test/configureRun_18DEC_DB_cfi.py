#Configuration for unpacker
import csv,os

#RAWFileName="/afs/cern.ch/user/d/dorney/public/run000186_LatencyScan_TIF_2016-12-12.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000080_Cosmics_TIF_2016-12-05.dat"
#RAWFileName="/cms/home/jlee/gemcrs/src/run000006_Cosmics_TIF_2016-11-20.dat"
#RAWFileName="/afs/cern.ch/work/d/dorney/CMS_GEM/Data/QC8/run000035_Test_TIF_2016-11-28.dat"
#RAWFileName="/afs/cern.ch/work/d/dorney/CMS_GEM/Data/QC8/run000036_Test_TIF_2016-11-28.dat"
#RAWFileName="/afs/cern.ch/work/d/dorney/CMS_GEM/Data/QC8/run000037_Test_TIF_2016-11-28.dat"
#RAWFileName="run000044_Cosmics_TIF_2016-12-03.dat"
RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000080_Cosmics_TIF_2016-12-05.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000113_Cosmics_TIF_2016-12-07.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000209_Cosmics_TIF_2016-12-13.dat"
RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000192_Cosmics_TIF_2016-12-12_chunk_0.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000192_Cosmics_TIF_2016-12-12.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000193_Cosmics_TIF_2016-12-12_chunk_0.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000208_LatencyScan_TIF_2016-12-13.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000186_LatencyScan_TIF_2016-12-12.dat"
#RAWFileName="/afs/cern.ch/user/h/hyunyong/public/run000189_LatencyScan_TIF_2016-12-12.dat"
MaxEvents=-1

makeTrack = True
#makeTrack = False
#flags for tracking
minClusterSize = 1
maxClusterSize = 10
maxResidual = 5.0 # cm
trackChi2 = 10
trackResX = 3.0 #cm
trackResY = 1.5
tag = ""
########################################################################################
# 
########################################################################################
RunNumber=int(RAWFileName.split("/")[-1].split("_")[0][3:])

ratePlot = False
GEMMask = "Validation/GEMCR/data/GEMMaskVecRun%06d.dat"%RunNumber
GEMHot = "Validation/GEMCR/data/GEMHotVecRun%06d.dat"%RunNumber

if os.path.isfile(os.environ['SRT_CMSSW_BASE_SCRAMRTDEL']+"/src/"+GEMHot):
   runWithMasking = True
else : 
  runWithMasking = False
if runWithMasking :
   makeMaskList = False
else :
   makeMaskList = True
if not runWithMasking :
  GEMMask = "Validation/GEMCR/data/GEMMaskEmp.dat" 
  GEMHot = "Validation/GEMCR/data/GEMMaskEmp.dat"

runWithMasking = False
#makeMaskList = True
makeMaskList = False

if runWithMasking : print "This run is running with masking"
else : print  "This run dose not have mask lists" 

if makeTrack and runWithMasking : OutputFileName='Reco_Run%06d.root'%RunNumber
elif runWithMasking : OutputFileName='Reco_Run%06d_maskedRecHit.root'%RunNumber
else : OutputFileName='Reco_Run%06d_test.root'%RunNumber

if ratePlot : OutputFileName='Reco_Run%06d_RandomTrigger.root'%RunNumber

if not tag == "":
  OutputFileName.replace(".root", "_%s.root"%tag)

print OutputFileName, " will be generted."
sqlite_file = os.environ['SRT_CMSSW_BASE_SCRAMRTDEL']+'/src/EventFilter/GEMRawToDigi/test/GEMEMap_CosmicStand_18Dec2016_TSCOL1-3_TSROW1-5_D1-2_Long.db'

def configureRun(SLOTLIST=[], VFATLIST=[], COLUMNLIST=[], ROWLIST=[], LAYERLIST=[], chamber=[], columnStand=[], rowStand=[], layerSC=[]):

    fileVFATS=os.environ.get('CMSSW_BASE')+"/src/EventFilter/GEMRawToDigi/data/VFAT2LIST.csv"

    schamber=[]
    slot=[]
    barcode=[]

    #Configuration of the Stand: write down every VFAT
    #The ones below are a editted version with respect to what is there in the elog

    NUMBEROFDETECTORS=4
    #NUMBEROFDETECTORS=10



#    schamber.append("GE1/1-SCS03")
#    chamber.append("GE1/1-VII-S-CERN-0003")
#    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
#    barcode.append(["#146","#204","#296","#284","#263","#093","#090","#035","#027","#088","#138","#336","#098","#040","#077","#211","#082","#029","#076","#048","#066","#290","#286","#316"])
#    columnStand.append(2)
#    rowStand.append(5)
#    layerSC.append(2)
    """
    schamber.append("GE1/1-SCS03")
    chamber.append("GE1/1-VII-S-CERN-0004")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#071","#319","#294","#321","#139","#096","#258","#271","#075","#265","#251","#325","#111","#318","#295","#274","#001", "#240","#310","#288","#188","#273","#058","#070"])
    columnStand.append(2)
    rowStand.append(5)
    layerSC.append(1) #Interior?


    schamber.append("GE1/1-SCS01")
    chamber.append("GE1/1-VII-S-CERN-0005")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#339","#308","#051","#068","#322","#337","#299","#197","#056","#303","#087","#301","#266","#195","#270","#050","#252","#135","#275","#253","#061","#140","#329","#215"])
    columnStand.append(2)
    rowStand.append(4)
    layerSC.append(2)

    schamber.append("GE1/1-SCS01")
    chamber.append("GE1/1-VII-S-CERN-0006")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#242","#302","#304","#069","#281","#059","#283","#100","#338","#259","#309","#073","#089","#214","#272","#312","#145","#217","#282","#086","#279","#278","#234","#246"])
    columnStand.append(2)
    rowStand.append(4)
    layerSC.append(1) #Interior?
    """
    schamber.append("GE1/1-SCL01")
    chamber.append("GE1/1-VII-L-CERN-0002")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#155","#119","#118","#228","#205","#099","#131","#085","#129","#109","#226","#185","#223","#106","#133","#110","#125","#148","#115","#136","#220","#094","#147","#191"])
    columnStand.append(2)
    rowStand.append(3)
    layerSC.append(2)

    schamber.append("GE1/1-SCL01")
    chamber.append("GE1/1-VII-L-CERN-0004")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#121","#036","#031","#164","#045","#028","#170","#180","#184","#169","#183","#168","#127","#161","#044","#157","#172","#120","#167","#154","#039","#182","#026","#038"])
    columnStand.append(2)
    rowStand.append(3)
    layerSC.append(1) #Interior?

    schamber.append("GE1/1-SCL02")
    chamber.append("GE1/1-VII-L-CERN-0003")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#171","#186","#116","#163","#114","#174","#162","#151","#152","#175","#177","#150","#179","#210","#178","#042","#173","#003","#165","#181","#122","#153","#149","#176"])
    columnStand.append(2)
    rowStand.append(2)
    layerSC.append(2)

    schamber.append("GE1/1-SCL02")
    chamber.append("GE1/1-VII-L-CERN-0001")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#117","#212","#206","#239","#063","#196","#243","#104","#244","#202","#130","#221","#189","#193","#187","#235","#124","#222","#201","#198","#213","#105","#194","#236"])
    columnStand.append(2)
    rowStand.append(2)
    layerSC.append(1) #Interior?
    """
    schamber.append("GE1/1-SCS02")
    chamber.append("GE1/1-VII-S-CERN-0002")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#326","#331","#297","#034","#049","#046","#314","#072","#192","#142","#333","#334","#292","#268","#305","#102","#055","#280","#307","#332","#265","#324","#078","#313"])
    columnStand.append(2)
    rowStand.append(1)
    layerSC.append(2)

    schamber.append("GE1/1-SCS02")
    chamber.append("GE1/1-VII-S-CERN-0001")
    slot.append([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23])
    barcode.append(["#060","#306","#057","#289","#030","#047","#053","#315","#327","#311","#144","#108","#256","#323","#264","#328","#254","#004","#225","#277","#287","#285","#041","#317"])
    columnStand.append(2)
    rowStand.append(1)
    layerSC.append(1) #Interior?
    """
    VFATHEX=[]
    BARCODE=[]



    for i in range(0,NUMBEROFDETECTORS):
      for item in range(0,len(barcode[i])):
          with open(fileVFATS, 'rt') as f:
                    reader = csv.reader(f, delimiter=',')
                    for row in reader:
                            words=row[2].split()
                            if len(words)==2:
                              if words[1]==barcode[i][item]:
                                    VFATLIST.append(int(row[1],16))
                                    VFATHEX.append(row[1])
                                    BARCODE.append(barcode[i][item])
                                    SLOTLIST.append(slot[i][item])
                                    COLUMNLIST.append(columnStand[i])
                                    ROWLIST.append(rowStand[i])
                                    LAYERLIST.append(layerSC[i])

