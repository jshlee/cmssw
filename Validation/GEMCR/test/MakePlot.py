#!/usr/bin/env python

"""
Copied from GEMCode/GEMValidation
"""

import numpy as np
import scipy.signal
import optparse
import os, sys, shutil, csv
from array import array
from ROOT import *
gROOT.SetBatch(1)
gStyle.SetStatStyle(0)
gStyle.SetOptStat(1110)

ablue = array("d", [0.51, 1.00, 0.12, 0.00, 0.00])
ared = array("d", [0.00, 0.00, 0.87, 1.00, 0.51])
agreen = array("d", [0.00, 0.81, 1.00, 0.20, 0.00])
astop = array("d", [0.00, 0.34, 0.61, 0.84, 1.00])
myPalette = []
fi = TColor.CreateGradientColorTable(5, astop, ared, agreen, ablue, 100 )
for x in xrange(100): myPalette.append(fi+x)
gStyle.SetPalette(100, array("i", myPalette))

import configureRun_cfi as runConfig

rootF = "DQM_V0001_R%09d__Global__CMSSW_X_Y_Z__RECO.root"%runConfig.RunNumber
run = runConfig.RunNumber
tDir = "DQMData/Run %d/MuonGEMRecHitsV/Run summary/GEMRecHitsTask"%(run)
oDir = "Run%06d_Plots/"%run

rate = []
vfatRate = []
emptyChamber = []
#maskPlot = runConfig.runWithMasking
#maskPlot = True
showRate = runConfig.ratePlot
makeMaskList = runConfig.makeMaskList

maskL = {}

SLOTLIST=[]
VFATLIST=[] 
COLUMNLIST=[] 
ROWLIST=[]
LAYERLIST=[]
chamber=[]
columnStand=[]
rowStand=[]
layerSC=[]

runConfig.configureRun(SLOTLIST, VFATLIST, COLUMNLIST, ROWLIST, LAYERLIST, chamber, columnStand, rowStand, layerSC)

chamList = {}
for i, cn in enumerate(chamber):
  chamList["{}_{}_{}".format(COLUMNLIST[i*24], ROWLIST[i*24], LAYERLIST[i*24])] = cn

chIndex = [1,3,5,7,9,11,13,15,17,19,21,23,25,27,29]
chi = 0
chamGEO = {}
for c in [1,2,3]:
  for r in [1,2,3,4,5]:
    chamGEO["chamber_{}".format(chIndex[chi])] = [c,r]
    chi+=1

tmpID = [x for x in csv.reader(open("../data/GEMRAWID.dat","r"))][0]
GEMRAWID = {}
GEMNAME = {}
for x in tmpID:
  p = x.split(" : ")
  GEMRAWID[p[0][1:]] = int(p[1])
  GEMNAME[int(p[1])] = p[0][1:]

def readMaskList():
  tmpM = open("../data/GEMMaskVecRun%06d.dat"%run, "r")
  tmpH = open("../data/GEMHotVecRun%06d.dat"%run, "r")
  for x in tmpM:
    tmpL = x.split()
    key = int(tmpL[0])
    value = int(tmpL[1])
    if not key in maskL: maskL[key] = [value]
    else :
      tmpV = maskL[key]
      tmpV.append(value)
      maskL[key] = tmpV
  for x in tmpH:
    tmpL = x.split()
    key = int(tmpL[0])
    value = int(tmpL[1])
    if not key in maskL: maskL[key] = [value]
    else :
      tmpV = maskL[key]
      tmpV.append(value)
      maskL[key] = tmpV

def findRawId(name):
  return GEMRAWID[name]

def findRollName(rollId):
  return GEMNAME[rollID]

def findName(name):
  if not name.startswith("chamber") : return name
  tname = name.split("_")
  layer = int(tname[3])
  name = tname[0]+"_"+tname[1]
  colum = chamGEO[name][0]
  row = chamGEO[name][1]
  return chamList["{}_{}_{}".format(colum, row, layer)]  

def findName2(name):
  return findName(name).replace("/","")

def findVfat(x, a, b):
  step = abs(b-a)/3.0
  if x < (min(a,b)+step) : return 1
  elif  x < (min(a,b)+2.0*step) :  return 2
  else : return 3 

def saveRoot(tob, tdir):
  outRoot.cd()
  outRoot.cd(tdir.replace("/",""))
  tob.Write()
  outRoot.cd("..")

def dump(inF, tDir, outF):
  dqm_file = TFile(inF)
  d1 = dqm_file.Get(tDir)
  tlist = d1.GetListOfKeys()
  outF.cd()
  outF.mkdir("DQMHist")
  outF.cd("DQMHist")
  for t in tlist:
    tmp = d1.Get(t.GetName())
    print t
    tmp.Write()

def setAxiNum(hist,axis,r, offSet=0):
  for x in xrange(r[0],r[1]+1):
    if axis == "x" or axis == "X":
      hist.GetXaxis().SetBinLabel(x,"{}".format(x+offSet))
    elif axis == "y" or axis == "Y":
      hist.GetYaxis().SetBinLabel(x,"{}".format(x+offSet))

def fHist(hist):
  fhist = hist.Clone()
  entries = hist.Integral()
  print entries
  fhist.Reset()
  ny = fhist.GetNbinsY()
  nx = fhist.GetNbinsX()
  for y in xrange(ny):
    for x in xrange(nx):
      tmpV = hist.GetBinContent(x+1,y+1)
      fhist.SetBinContent(x+1,ny-y,tmpV)
      fhist.GetYaxis().SetBinLabel(y+1,"{}".format(ny-y))
  fhist.SetEntries(entries)
  return fhist

def hotStripCut(l, name):
  tmp = scipy.signal.medfilt(l,3)
  tmp = scipy.signal.medfilt(tmp,3)
  tmp = scipy.signal.medfilt(tmp,3)
  tmp = scipy.signal.medfilt(tmp,5)
  tmp = scipy.signal.medfilt(tmp,7)
  tmp = scipy.signal.medfilt(tmp,15)
  tmp = scipy.signal.medfilt(tmp,31)
  tmp = scipy.signal.medfilt(tmp,63)
  m = np.mean(tmp)
  mul = TH1D("%s_%d"%(findName2(name)+"_roll_"+name.split("_")[-1]+"_multiplicity", findRawId(name)), "%s %d"%(findName(name)+" roll "+name.split("_")[-1]+" multiplicity", findRawId(name)),int(2.*m) ,0,int(2.*m))
  for x in l:
    mul.Fill(x)
  if mul.GetEntries() == 0: return 0
  mul.Fit("gaus","","",mul.GetMean()-2.0*mul.GetRMS(), mul.GetMean()+2.0*mul.GetRMS())
  fitresult = TVirtualFitter.GetFitter()
  fm = fitresult.GetParameter(1) 
  sig = fitresult.GetParameter(2) 
  saveRoot(mul, findName(name))
  cut = fm+2*sig
  return [cut, int(m), mul]
    
def writeMask(hist,clsHist):
  tName = hist.GetName().split("_")
  nX = hist.GetNbinsX()
  nY = hist.GetNbinsY()
  hotVfat = TH2D(findName(hist.GetName()).replace("/","")+"_hotVfat", findName(hist.GetName())+" hot vfat",3,1,4,8,1,9)
  mHist = hist.Clone(hist.GetName()+"_masked")
  mHist.SetTitle("masked strip")
  c = TCanvas("local_X","local_x",600,600)
  c.SetRightMargin(0.35)
  for y in xrange(nY):
    name = "chamber_%d_layer_%d_roll_%d"%(int(tName[1]), int(tName[3]), y+1)
    a = []
    for x in xrange(nX):
      v = hist.GetBinContent(x+1,y+1)
      if v == 0 : 
        mask.write("%d %d\n"%(findRawId(name), x))
      a.append(v)
    cut = hotStripCut(a, name)
    if (cut[0] < cut[1]) or (cut[0] < 10) :
      print "hot strip cut fitting was wrong!\n"*10
    if (cut[0] < cut[1]):
      cut[0] = cut[1]*3.0
    if (cut[0] < 10) :
      cut[0] = 10
    clsh = TH1D(findName2(hist.GetName())+"_iEta_%d"%(y+1)+"_CLS_gemDigi_multiplicity", findName(hist.GetName())+"roll %d cls cls cut multiplicity"%(y+1),cut[1]*2, 0, cut[1]*2 )
    for x in xrange(nX):
      v = hist.GetBinContent(x+1,y+1)
      clsh.Fill(clsHist.GetBinContent(x+1,y+1))
      if v>cut[0]: 
        hotStrip.write("%d %d\n"%(findRawId(name), x))
        hotVfat.Fill(findVfat(x,0,128*3),y+1)
        mHist.SetBinContent(x+1,y+1,0)
    saveRoot(clsh, findName2(hist.GetName()))
    if divmod(cut[1],2)[1] == 0 : rebinN = 4
    else : rebinN = 5
    c.SetLogy(1)
    digih = cut[2]
    digih.Fit("gaus","","", digih.GetMean()-2.0*digih.GetRMS(), digih.GetMean()+2.0*digih.GetRMS())
    digih.SetLineColor(kBlue-4)
    digih.SetLineWidth(2)
    digih.Rebin(rebinN)
    digih.SetMaximum(digih.GetMaximum()*100)
    digih.Draw()
    clsh.SetLineColor(kGreen+3)
    clsh.SetLineWidth(2)
    clsh.Rebin(rebinN)
    #clsh.Draw("HIST SAME")
    cutLine = TLine(cut[0], 0, cut[0], digih.GetMaximum()*1.2)
    cutLine.SetLineWidth(3)
    cutLine.SetLineStyle(7)
    cutLine.SetLineColor(kRed)
    cutLine.Draw()
    le = TLegend(0.2, 0.75, 0.6, 0.9)
    le.SetTextSize(0.035)
    le.SetFillStyle(0)
    le.SetFillColor(kWhite)
    le.SetBorderSize(0)
    le.SetHeader("cut : %.2f, reBin = %d"%(cut[0], rebinN))
    le.AddEntry(digih, "Digi collection")
    #le.AddEntry(clsh, "%d < CLS < %d"%(runConfig.minClusterSize-1,runConfig.maxClusterSize+1))
    le.Draw()

    c.SaveAs(oDir+"/"+findName2(hist.GetName())+"/"+findName2(hist.GetName())+"_iEta_%d_gemDigi_multiplicity.png"%(y+1)) 
    c.SetLogy(0)
  hotVfat.SetXTitle("vfat number")
  hotVfat.SetYTitle("roll number")
  setAxiNum(hotVfat,"y",[1,8])
  setAxiNum(hotVfat,"x",[1,3])
  hotVfatf = fHist(hotVfat)
  saveRoot(hotVfat, findName(hist.GetName()).replace("/",""))
  hotVfatf.Draw("colz text")
  c.SaveAs(oDir+"/"+findName(hist.GetName()).replace("/","")+"/"+findName(hist.GetName()).replace("/","")+"_hotStrips.png") 
  mRoll = roll1D(mHist)
  dRoll = roll1D(hist)
  for x in xrange(len(mRoll)):
    dRoll[x].SetLineColor(kRed)
    mRoll[x].SetLineColor(kBlue)
    c.SetLogy(1)
    dRoll[x].SetTitle("Roll %d"%(x+1))
    dRoll[x].Draw("HIST")
    mRoll[x].Draw("HIST SAME")
    c.SaveAs(oDir+"/"+findName2(hist.GetName())+"/"+findName2(hist.GetName())+"_iEta_%d_hot_strip_cut.png"%(x+1))
    c.SetLogy(0)
  for h in mRoll:
    draw_occ(oDir, h, ".png", "", True)
    saveRoot(h, findName2(mHist.GetName()))
  fmHist = fHist(mHist)
  draw_occ(oDir, fmHist)
  saveRoot(fmHist, findName2(fmHist.GetName()))

def maskHist(hist):
  tName = hist.GetName().split("_")
  mhist = hist.Clone(hist.GetName()+"_masked")
  mhist.SetTitle(hist.GetTitle()+" masked")
  ny = mhist.GetNbinsY()
  nx = mhist.GetNbinsX()
  for y in xrange(ny):
    key = findRawId("chamber_%d_layer_%d_roll_%d"%(int(tName[1]), int(tName[3]), y+1))
    if not key in maskL : continue
    for x in maskL[key]:
      mhist.SetBinContent(x+1,y+1,0)
  return mhist  

def makeMapHist(hist):
  h2 = TH2D(hist.GetName()+"_map", "RecHit size mean value", 3,1,4,8,1,9) 
  h2.SetXTitle("vfat number")
  h2.SetYTitle("roll number")
  ny = hist.GetNbinsY()
  nx = hist.GetNbinsX()
  for y in xrange(ny):
    ent = 0
    val = 0
    for x in xrange(nx):
      tmpV =  hist.GetBinContent(x+1,y+1)
      ent += tmpV
      val += tmpV*(x+1)
    if ent == 0 : mean = 0
    else : mean = val/ent
    h2.SetBinContent(divmod(y,8)[0]+1, 8-divmod(y,8)[1], mean)
  return h2

def myFitter(hist,fun):

  if hist.GetEntries() == 0: return 0,0
  hist.Fit(fun.GetName())
  fitresult = TVirtualFitter.GetFitter()
  m = fitresult.GetParameter(0)
  b = fitresult.GetParameter(1)
  return m,b

def effErr(h1, h2,t):
  name = h1.GetName()+"_"+t
  nX = h1.GetNbinsX()
  lX = h1.GetXaxis().GetBinLowEdge(1)
  uX = h1.GetXaxis().GetBinUpEdge(nX)
  nY = h1.GetNbinsY()
  lY = h1.GetYaxis().GetBinLowEdge(1)
  uY = h1.GetYaxis().GetBinUpEdge(nY)
  print nX, lX, uX, nY, lY, uY
  teff = TEfficiency(name, "TEFF", nX, lX, uX, nY, lY, uY) 
  for y in xrange(nY):
    for x in xrange(nX):
      passed = h1.GetBinContent(x+1,y+1)
      tFill = h2.GetBinContent(x+1,y+1)
      #if tFill == 0 : continue
      cX = h1.GetXaxis().GetBinCenter(x+1)
      cY = h1.GetYaxis().GetBinCenter(y+1)
      teff.FillWeighted(True, passed, cX, cY)
      teff.FillWeighted(False, tFill-passed, cX, cY)
  err = TH2D(name+"_err", name+" efficiency Error",nX,lX,uX,nY,lY,uY)
  for y in xrange(nY):
    for x in xrange(nX):
      errLow = teff.GetEfficiencyErrorLow(teff.GetGlobalBin(x+1,y+1))
      errUp = teff.GetEfficiencyErrorUp(teff.GetGlobalBin(x+1,y+1))
      e = (errLow+errUp)/2.0
      if e > 0 : err.SetBinContent(x+1,y+1,e)
  return err

def effErr1D(h1, h2,t):
  name = findName(h1.GetName()).replace("/","")
  print name
  nX = h1.GetNbinsX()
  lX = h1.GetXaxis().GetBinLowEdge(1)
  uX = h1.GetXaxis().GetBinUpEdge(nX)
  nY = h1.GetNbinsY()
  lY = h1.GetYaxis().GetBinLowEdge(1)
  uY = h1.GetYaxis().GetBinUpEdge(nY)
  
  teff = TEfficiency(name,name , 24, 0, 24)
  for x in xrange(3):
    for y in xrange(8):
      passed = h1.GetBinContent(x+1,y+1)
      tFill = h2.GetBinContent(x+1,y+1)
      ibin = y + x*8 
      teff.FillWeighted(True, passed, ibin)
      teff.FillWeighted(False, tFill-passed, ibin)
  return teff

def roll1D(hist):
  nY = hist.GetNbinsY()
  nX = hist.GetNbinsX()
  name = hist.GetName()
  if nY == 8:
    histL = [TH1D(name+"_iEta_%d"%(x+1), "",nX,0,nX) for x in xrange(nY)]
  if nY == 9:
    histL = [TH1D(name+"_iEta_%d"%(x), "",nX,0,nX) for x in xrange(nY)]
  for y in xrange(nY):
    for x in xrange(nX):
      histL[y].SetBinContent(x+1, hist.GetBinContent(x+1,y+1))
  return histL

def vfat2roll(hist):
  name = findName2(hist.GetName())
  rollHist = [TH1D(name+"_iEta_%d_CLS"%(x+1), "CLS roll %d"%(x+1), 50, 0, 50)  for x in xrange(8)]
  for y in xrange(hist.GetNbinsY()):
    iroll = 8-divmod(y,8)[1]
    for x in xrange(hist.GetNbinsX()):
      rollHist[iroll-1].Fill(x,hist.GetBinContent(x+1,y+1))
  return rollHist


def draw_occ(target_dir, h, ext =".png", opt = "colz", logy = False, logz = False):
  gStyle.SetStatStyle(0)
  gStyle.SetOptStat(1110)
  name = findName(h.GetName())
  tname = h.GetName().split("_")
  etc = "_"
  for x in tname[4:]:
    etc += x+"_"
  if not h.GetName().startswith("chamber") : etc=""
  h.SetName(name.replace("GE1/1", "GE11")+etc[:-1])
  c = TCanvas(h.GetName(),h.GetName(),600,600)
  c.Clear()
  c.SetRightMargin(0.35)
  if not h:
    sys.exit('h does not exist')
  h.SetLineWidth(2)
  h.SetLineColor(kBlue)
  if logy:
    c.SetLogy(1)
    ext = "_log"+ext
  if logz:
    c.SetLogz(1)
    ext = "_log"+ext
  h.Draw(opt)
  if name.startswith("GE1"): c.SaveAs(target_dir + name.split("_")[0].replace("GE1/1", "GE11")+"/"+ h.GetName() + ext)
  else : c.SaveAs(target_dir + h.GetName() + ext)
  if logy:
    c.SetLogy(0)
  if logz:
    c.SetLogz(0)

def draw_plot( file, tDir,oDir ) :
  c = TCanvas("c","c",600,600)
  dqm_file = TFile( file)
  d1 = dqm_file.Get(tDir)
  key_list =[]

  try :
    tlist = d1.GetListOfKeys()
  except :
    print oDir
    if ( oDir.find("Digi") != -1 ):
      tDir = "DQMData/Run 1/MuonGEMDigisV/Run summary/GEMDigiTask"
      d1 = dqm_file.Get(tDir)
      tlist = d1.GetListOfKeys()
    elif ( oDir.find("RecHit") != -1 ):
      tDir = "DQMData/Run 1/MuonGEMRecHitsV/Run summary/GEMRecHitTask"
      d1 = dqm_file.Get(tDir)
      tlist = d1.GetListOfKeys()
    else :
      print "error"
      exit(-1)
  for x in tlist :
    key_list.append(x.GetName())

  for hist in key_list :
    if ( hist.startswith("chamber") and hist.endswith("recHit_efficiency")):
      if not runConfig.makeTrack : continue
      tmph = d1.Get(hist)
      thEff = d1.Get(hist.replace("recHit_efficiency", "th2D_eff"))
      tmp1 = effErr1D(tmph,thEff, "1D")
      c1 = TCanvas("1D", "1D",600,600) 
      tmp1.Draw()
      c1.SaveAs(oDir+tmp1.GetName()+"/%s_TEfficiency1D.png"%tmp1.GetName())
      saveRoot(tmp1, findName(tmp1.GetName()).replace("GE1/1", "GE11"))
      tmpe = effErr(tmph, thEff,"vfat")
      tmpe.SetXTitle("vfat number")
      tmpe.SetYTitle("roll number")
      setAxiNum(tmpe,"x",[1,3])
      setAxiNum(tmpe,"y",[1,8])    
      tmpef = fHist(tmpe)   
      tmpef.SetEntries(thEff.Integral())
      draw_occ(oDir, tmpef,".png", "colz text")
      saveRoot(tmpef, findName(tmp1.GetName()).replace("GE1/1", "GE11"))
      tmph.Divide(thEff)
      tmph.SetXTitle("vfat number")
      tmph.SetYTitle("roll number")
      setAxiNum(tmph,"x",[1,3])
      setAxiNum(tmph,"y",[1,8])

      tmpf = fHist(tmph)
      tmpf.SetEntries(thEff.Integral())
      tmpf.GetZaxis().SetRangeUser(0.0,1.0)
      draw_occ(oDir, tmpf, ".png", "colz text")
      saveRoot(tmpf, findName(tmp1.GetName()).replace("GE1/1", "GE11"))
 

    elif ( hist.startswith("chamber") and hist.endswith("trxy_eff")):
      if not runConfig.makeTrack : continue
      tmph = d1.Get(hist)
      thEff = d1.Get(hist.replace("trxy_eff", "thxy_eff"))
      tmpe = effErr(tmph,thEff,"x")
      tmpe.SetXTitle("x [cm]")
      tmpe.SetYTitle("y [cm]")
      tmpef = fHist(tmpe)
      tmpef.SetEntries(thEff.Integral())
      draw_occ(oDir, tmpef,".png", "colz text")
      draw_occ(oDir, tmpef)
      tmph.Divide(thEff)
      tmph.SetXTitle("x [cm]")
      tmph.SetYTitle("y [cm]")
      #setAxiNum(tmph,"y",[1,8])
      tmpf = fHist(tmph)
      tmpf.GetZaxis().SetRangeUser(0.0,1.0)
      draw_occ(oDir, tmpf)
      saveRoot(tmpf, findName(tmp1.GetName()))

    elif ( hist.endswith("bestChi2")):
      if not runConfig.makeTrack : continue
      tmph = d1.Get(hist)
      tmph.SetXTitle("#chi^{2}")
      draw_occ(oDir, tmph)

    elif ( hist.startswith("chamber") and hist.endswith("gemDigi")):
      tmph = d1.Get(hist)
      tmph.SetXTitle("Strip")
      tmph.SetYTitle("Roll Number (iEta)") 
      if makeMaskList and hist.endswith("digi_gemDigi"):
        if tmph.Integral() == 0: continue
        clsHist = d1.Get(hist.replace("digi_gemDigi", "CLS_gemDigi"))
        writeMask(tmph, clsHist)
      histL = roll1D(tmph)
      ent = d1.Get("firedMul").Integral()
      for h in histL:
        draw_occ(oDir, h) 
        saveRoot(h, findName(tmph.GetName()))
        tmpR = h.Clone(h.GetName()+"_rates")
        tmpR.Scale(40000.0/(4092.5/8.0/384.0)/ent)
        tmpR.SetYTitle("rate [KHz/cm^{2}]")
        draw_occ(oDir,tmpR)
        draw_occ(oDir,tmpR,".png","",True)
      vfatRate = TH2D(findName(tmph.GetName()).replace("/","")+"_vfatRate", findName(tmph.GetName())+"vfat rate [KHz/cm^{2}]",3,1,4,8,1,9)
      for y in xrange(tmph.GetNbinsY()):
        for x in xrange(tmph.GetNbinsX()):
          v = tmph.GetBinContent(x+1,y+1)
          vfatRate.Fill(findVfat(x,0,128*3), y+1,v)
      
      vfatRate.SetXTitle("vfat number")
      vfatRate.SetYTitle("roll number")
      setAxiNum(vfatRate,"y",[1,8])
      setAxiNum(vfatRate,"x",[1,3])
      vfatRate.Scale(40000.0/(4092.5/8.0/3.0)/ent)
      vfatRatef = fHist(vfatRate)
      draw_occ(oDir,vfatRatef,".png", "colz text")  
      tmpf = fHist(tmph)
      draw_occ(oDir, tmpf)
      saveRoot(tmpf, findName(tmph.GetName()).replace("GE1/1", "GE11"))

    elif ( hist.startswith("chamber") and hist.endswith("recHit")):
      tmph = d1.Get(hist)
      tmph.SetXTitle("x [cm]")
      tmph.SetYTitle("Roll Number (iEta)") 
      tmpf = fHist(tmph)
      draw_occ(oDir, tmpf)
      saveRoot(tmpf, findName(tmph.GetName()))

    elif ( hist.startswith("chamber") and hist.endswith("hit_mul")):
      tmph = d1.Get(hist)
      tmph.SetXTitle("hit")
      tmph.SetYTitle("count")
      draw_occ(oDir, tmph,".png","",1,0)

    elif ( hist.startswith("chamber") and hist.endswith("vfatHit_mul")):
      tmph = d1.Get(hist)
      tmph.SetXTitle("hit")
      tmph.SetYTitle("vfat number")
      setAxiNum(tmph,"y",[1,24],-1)
      draw_occ(oDir, tmph,".png", "colz", 0,1)

    elif ( hist.startswith("chamber") and hist.endswith("stripHit_mul")):
      tmph = d1.Get(hist)
      for h in roll1D(tmph):
        if h.GetName().endswith("_iEta_0") : h.SetTitle("strip multiplicity")
        else:  h.SetTitle("roll %d strip multiplicity"%int(h.GetName()[-1]))
        h.SetXTitle("hit")
        h.SetYTitle("count")
        draw_occ(oDir, h, ".png", "",1,0)
        saveRoot(h, findName2(tmph.GetName()))

    elif ( hist.startswith("chamber") and hist.endswith("recHit_size")):
      tmph = d1.Get(hist)
      dName = tmph.GetName()
      for h in vfat2roll(tmph):
        h.SetXTitle("recHit size")
        h.SetYTitle("count")
        saveRoot(h, findName(dName))
        draw_occ(oDir, h,".png","HIST",True)
      h2 = makeMapHist(tmph)
      tmph.SetXTitle("recHit size")
      tmph.SetYTitle("vfat number")
      setAxiNum(tmph,"y",[1,24],-1)
      draw_occ(oDir, tmph)
      setAxiNum(h2,"x",[1,3])
      tmpf = fHist(h2)
      draw_occ(oDir, tmpf, ".png", "colz text")
      saveRoot(tmpf, findName(dName))

    elif (hist == "rh1_chamber"):
      tmph = d1.Get(hist)
      tmph.SetYTitle("count")
      tmph.SetMaximum(tmph.GetMaximum()*1.5)
      tmp2 = d1.Get("rh2_chamber")
      tmp3 = d1.Get("rh3_chamber")
      tmph.SetLineColor(kBlue-4)
      tmp2.SetLineColor(kMagenta-4)
      tmp3.SetLineColor(kOrange+7)
      tmph.SetLineWidth(2)
      tmp2.SetLineWidth(2)
      tmp3.SetLineWidth(2)
      for x in xrange(tmph.GetNbinsX()):
        cName = tmph.GetXaxis().GetBinLabel(x+1)
        tmph.GetXaxis().SetBinLabel(x+1, findName(cName)) 
        v = tmph.GetBinContent(x+1)
        if v == 0:
          emptyChamber.append(findName(cName))
      tmph.SetStats(0) 
      c = TCanvas("ent","ent",600,600) 
      c.SetBottomMargin(0.18)
      c.SetRightMargin(0.18)
      tmph.Draw()
      tmp2.Draw("same")
      tmp3.Draw("same")
      le = TLegend(0.4, 0.75, 0.99, 0.9)
      le.SetTextSize(0.035)
      le.SetFillStyle(0)
      le.SetFillColor(kWhite)
      le.SetBorderSize(0)
      le.SetHeader("recHit Entries")
      le.AddEntry(tmph, "all recHits")
      le.AddEntry(tmp2, "%d < CLS < %d"%(runConfig.minClusterSize-1,runConfig.maxClusterSize+1))
      le.AddEntry(tmp3, "tracking recHits")
      le.Draw()
      c.SaveAs(oDir+"recHits.png")
      hist = "firedMul"
      tmph = d1.Get(hist)
      tmph.SetYTitle("count")
      tmph.SetLineColor(kBlue-4)
      tmph.SetLineWidth(2)
      tmph.Draw()
      ent = d1.Get("firedMul").Integral()
      c.SaveAs(oDir+"firedMul.png")
      hist = "firedChamber"
      tmph = d1.Get(hist)
      tmph.SetYTitle("count")
      tmph.SetLineColor(kBlue-4)
      tmph.SetLineWidth(2)
      c.SaveAs(oDir+"firedMul.png")
      for x in xrange(tmph.GetNbinsX()):
        cName = tmph.GetXaxis().GetBinLabel(x+1)
        tmph.GetXaxis().SetBinLabel(x+1, findName(cName))
        tmpEnt = tmph.GetBinContent(x+1)
        rate.append([findName(cName),tmpEnt,ent])
      c.SetBottomMargin(0.18)
      c.SetRightMargin(0.18)
      tmph.Draw()
      c.SaveAs(oDir+"firedChamber.png")

    elif (hist.startswith("chamber") and hist.endswith("local_x")):
      if not runConfig.makeTrack : continue
      c = TCanvas("local_X","local_x",600,600)
      tmph = d1.Get(hist)
      tmph.Draw()
      fun = TF1("localx", "[0]*x + [1]")
      fun.SetParameter(0,1)
      fun.SetParameter(1,0)
      fitR = myFitter(tmph, fun) 
      tmph.SetXTitle("Track hit [cm]") 
      tmph.SetYTitle("recHit [cm]")     
      name = findName(hist)
      tname = hist.split("_")
      etc = "_"
      for x in tname[4:]:
        etc += x+"_"
      title = name+" "+tmph.GetTitle()
      tmph.SetTitle(title)
      tmph.SetName(name.replace("GE1/1", "GE11")+etc[:-1])
      extraText = TLatex()
      extraText.SetNDC()
      extraText.SetTextFont(52)
      extraText.SetTextSize(0.03)
      extraText.DrawLatex(0.1,0.9,"fit result : y = mx + b (m = %1.2f, b = %1.2f)"%(fitR[0], fitR[1]))
      dName = name.replace("GE1/1", "GE11")+"/"
      outRoot.cd(dName)
      tmph.Write()
      outRoot.cd("..")
      c.SaveAs(oDir+dName+tmph.GetName()+".png")   
    
      hist =hist.replace("local_x", "residual")
      tmph = d1.Get(hist)
      fun2 = TF1("residual", "[0]*x + [1]")
      fun2.SetParameter(0,0)
      fun2.SetParameter(1,0)
      tmph.Fit(fun2.GetName())
      fitR = myFitter(tmph, fun2)
      tmph.SetXTitle("Track hit [cm]")
      tmph.SetYTitle("(recHit - Track hit) [cm]")
      name = findName(hist)
      tname = hist.split("_")
      etc = "_"
      for x in tname[4:]:
        etc += x+"_"
      title = name+" "+tmph.GetTitle()
      tmph.SetTitle(title)
      tmph.SetName(name.replace("GE1/1", "GE11")+etc[:-1])
      extraText = TLatex()
      extraText.SetNDC()
      extraText.SetTextFont(52)
      extraText.SetTextSize(0.03)
      extraText.DrawLatex(0.1,0.9,"fit result : y = mx + b (m = %1.2f, b = %1.2f)"%(fitR[0], fitR[1]))
      dName = name.replace("GE1/1", "GE11")+"/"
      outRoot.cd(dName)
      tmph.Write()
      outRoot.cd("..")
      c.SaveAs(oDir+dName+tmph.GetName()+".png")

    else : continue

def makeSummary():

  head = """
\documentclass{beamer}
\usefonttheme[onlylarge]{structurebold}
\setbeamerfont*{frametitle}{size=\\normalsize,series=\\bfseries}
\setbeamertemplate{navigation symbols}{}
\setbeamertemplate{itemize items}[circle]
\setbeamerfont{page number in head/foot}{size=\small}
\setbeamertemplate{footline}[frame number]
\usepackage[english]{babel}
\usepackage[latin1]{inputenc}
\usepackage{times}
\usepackage{graphicx}
\usepackage[T1]{fontenc}
\usepackage{alltt}

\\newcommand{\\baseLoc}{./}

\\newcommand{\imageOne}[1]{
\scalebox{0.3}{
\includegraphics{\\baseLoc#1}
}
}

\\newcommand{\imageTwo}[2]{
\scalebox{0.26}{
\includegraphics{\\baseLoc#1}
\includegraphics{\\baseLoc#2}
}
}
\\newcommand{\imageThree}[3]{
\scalebox{0.18}{
\includegraphics{\\baseLoc#1}
\includegraphics{\\baseLoc#2}
\includegraphics{\\baseLoc#3}
}
}

\\newcommand{\imageFour}[4]{
\scalebox{0.18}{
\includegraphics{\\baseLoc#1}
\includegraphics{\\baseLoc#2}
}
\\
\scalebox{0.18}{
\includegraphics{\\baseLoc#3}
\includegraphics{\\baseLoc#4}
}
}

\\newcommand{\imageFive}[5]{
\scalebox{0.18}{
\includegraphics{\\baseLoc#1}
\includegraphics{\\baseLoc#2}
\includegraphics{\\baseLoc#3}
}
\\
\scalebox{0.18}{
\includegraphics{\\baseLoc#4}
\includegraphics{\\baseLoc#5}
}
}


\\newcommand{\imageSix}[6]{
\scalebox{0.18}{
\includegraphics{\\baseLoc#1}
\includegraphics{\\baseLoc#2}
\includegraphics{\\baseLoc#3}
}
\\
\scalebox{0.18}{
\includegraphics{\\baseLoc#4}
\includegraphics{\\baseLoc#5}
\includegraphics{\\baseLoc#6}
}
}

\\newcommand{\imageEight}[8]{
\scalebox{0.14}{
\includegraphics{\\baseLoc#1}
\includegraphics{\\baseLoc#2}
\includegraphics{\\baseLoc#3}
\includegraphics{\\baseLoc#4}
}
\\
\scalebox{0.14}{
\includegraphics{\\baseLoc#5}
\includegraphics{\\baseLoc#6}
\includegraphics{\\baseLoc#7}
\includegraphics{\\baseLoc#8}
}
}


\\begin{document}
"""
  t_recHit = """
\\begin{frame}[plain]{%s recHit}
\imageSix{%s_digi_gemDigi.png}{%s_recHit_gemDigi.png}{%s_CLS_gemDigi.png}{%s_recHit.png}{%s_recHit_size.png}{%s_recHit_size_map.png}
\end{frame}
"""
  t_recHitMask = """
\\begin{frame}[plain]{%s recHit}
\imageSix{%s_digi_gemDigi.png}{%s_digi_gemDigi_masked.png}{%s_hotStrips.png}{%s_recHit.png}{%s_recHit_size.png}{%s_recHit_size_map.png}
\end{frame}
"""

  t_track = """
\\begin{frame}[plain]{%s Track}
\imageFive{%s_recHit_efficiency.png}{%s_recHit_efficiency_vfat_err.png}{%s_TEfficiency1D.png}{%s_trxy_eff.png}{%s_trxy_eff_x_err.png}
\end{frame}
"""
  t_localx = """
\\begin{frame}[plain]{%s Alignment}
\imageTwo{%s_local_x.png}{%s_residual.png}
\end{frame}

"""
  t_info = """
\\begin{frame}[plain]{Run Info.}
\\begin{itemize}
  \item RAWFileName : %s
  \item OutPutFile : %s
  \item MaxEventSet : %d (%d events were processed)
  \item minClusterSize : %d
  \item maxClusterSize : %d
  \item maxResidual : %1.2f cm
  \item Track : %s
  \\begin{itemize}
    \item trackChi2 : %1.2f
    \item trackResX : %1.2f
    \item trackResY : %1.2f
  \end{itemize}
\end{itemize}
\end{frame}

\\begin{frame}[plain]{recHits entries for chambers}
\imageOne{recHits.png}
\end{frame}

\\begin{frame}[plain]{fired chambers}
\imageTwo{firedMul.png}{firedChamber.png}
\end{frame}

"""

  rate_t = """
\\begin{frame}[plain]{%s roll %d}
\imageTwo{%s/%s_digi_gemDigi_iEta_%d_rates.png}{%s/%s_digi_gemDigi_iEta_%d_rates_log.png}
\end{frame}
"""
  ratevfat_t = """
\\begin{frame}[plain]{%s vfat rate}
\imageOne{%s/%s_vfatRate.png}
\end{frame}
"""
  hotCut_t = """
\\begin{frame}[plain]{%s vfat rate}
\imageTwo{%s/%s_vfatRate.png}{%s/%s_masked_vfatRate.png}
\end{frame}
"""
  CLS_t = """
\\begin{frame}[plain]{%s CLS}
\imageEight{%s_iEta_%d_CLS_log.png}{%s_iEta_%d_CLS_log.png}{%s_iEta_%d_CLS_log.png}{%s_iEta_%d_CLS_log.png}
{%s_iEta_%d_CLS_log.png}{%s_iEta_%d_CLS_log.png}{%s_iEta_%d_CLS_log.png}{%s_iEta_%d_CLS_log.png}
\end{frame}
"""
  digimul_t = """
\\begin{frame}[plain]{%s gemDigi multiplicity}
\imageEight{%s_iEta_%d_gemDigi_multiplicity.png}{%s_iEta_%d_gemDigi_multiplicity.png}{%s_iEta_%d_gemDigi_multiplicity.png}{%s_iEta_%d_gemDigi_multiplicity.png}
{%s_iEta_%d_gemDigi_multiplicity.png}{%s_iEta_%d_gemDigi_multiplicity.png}{%s_iEta_%d_gemDigi_multiplicity.png}{%s_iEta_%d_gemDigi_multiplicity.png}
\end{frame}
"""
  rollCut_t = """
\\begin{frame}[plain]{%s hot stipt cut}
\imageEight{%s_iEta_%d_hot_strip_cut.png}{%s_iEta_%d_hot_strip_cut.png}{%s_iEta_%d_hot_strip_cut.png}{%s_iEta_%d_hot_strip_cut.png}
{%s_iEta_%d_hot_strip_cut.png}{%s_iEta_%d_hot_strip_cut.png}{%s_iEta_%d_hot_strip_cut.png}{%s_iEta_%d_hot_strip_cut.png}
\end{frame}
"""

  hitMul_t = """
\\begin{frame}[plain]{%s hit multiplicity}
\imageThree{%s_hit_mul_log.png}{%s_vfatHit_mul_log.png}{%s_stripHit_mul_iEta_0_log.png}
\end{frame}
"""
  hitRoll_t = """
\\begin{frame}[plain]{%s strip multiplicity}
\imageEight{%s_stripHit_mul_iEta_%d_log.png}{%s_stripHit_mul_iEta_%d_log.png}{%s_stripHit_mul_iEta_%d_log.png}{%s_stripHit_mul_iEta_%d_log.png}
{%s_stripHit_mul_iEta_%d_log.png}{%s_stripHit_mul_iEta_%d_log.png}{%s_stripHit_mul_iEta_%d_log.png}{%s_stripHit_mul_iEta_%d_log.png}
\end{frame}
"""
  chi2_t = """
\\begin{frame}[plain]{%s $\chi^{2}$ distribution}
\imageOne{%s_bestChi2.png}
\end{frame}
"""

  chamber.sort()
  for c in emptyChamber:
    chamber.remove(c)

  os.chdir(oDir)
  outF = open(runConfig.OutputFileName.replace(".root", ".tex"), "w")
  outF.write(head)
  if runConfig.makeTrack: trackCheck = "True"
  else : trackCheck = "False"

  outF.write(t_info%(runConfig.RAWFileName.split("/")[-1].replace("_","\_"), runConfig.OutputFileName.replace("_","\_"),runConfig.MaxEvents, int(rate[0][2]),runConfig.minClusterSize, runConfig.maxClusterSize, runConfig.maxResidual, trackCheck, runConfig.trackChi2, runConfig.trackResX, runConfig.trackResY ))
  if showRate:
    outF.write("\\begin{frame}[plain]{noise rates}\n\\begin{itemize}")
    for x in rate:
      outF.write("\item %s : %3.3f MHz (%d times fired)"%(x[0], x[1]/x[2]*40.0, x[1]))
    outF.write("\end{itemize}\n\end{frame}")
    for x in chamber:
      d = x.replace("/","")
      outF.write(ratevfat_t%(x,d,d))
      for r in xrange(1,9):
        outF.write(rate_t%(x,r,d,d,r,d,d,r))
  for x in chamber:
    t = x.replace("GE1/1", "GE11")
    x = t+"/"+t
    t = t.replace("GE11", "GE1/1")
    outF.write(hitMul_t%(t,x,x,x))
    outF.write(hitRoll_t%(t, x,1, x,2, x,3, x,4, x,5, x,6, x,7, x,8))
    #if maskPlot : outF.write(t_recHitMask%(t,x,x,x,x,x,x))
    #else : outF.write(t_recHit%(t,x,x,x,x,x,x))
    if makeMaskList: outF.write(t_recHitMask%(t,x,x,x,x,x,x))
    else : outF.write(t_recHit%(t,x,x,x,x,x,x))
    outF.write(CLS_t%(t, x,1, x,2, x,3, x,4, x,5, x,6, x,7, x,8))
    if makeMaskList: 
      outF.write(digimul_t%(t, x,1, x,2, x,3, x,4, x,5, x,6, x,7, x,8))
      outF.write(rollCut_t%(t, x,1, x,2, x,3, x,4, x,5, x,6, x,7, x,8))
    if runConfig.makeTrack :
      outF.write(t_track%(t,x,x,x,x,x))
      outF.write(t_localx%(t,x,x))
      outF.write(chi2_t%(t,x))
  outF.write("\end{document}")
  outF.close() 
  os.system("latex --output-format=pdf "+runConfig.OutputFileName.replace(".root", ".tex"))
  shutil.copy2("./"+runConfig.OutputFileName.replace(".root", ".pdf"), "..")


if __name__ == '__main__' :
  
  os.system("mkdir -p "+oDir )
  outRoot = TFile(oDir+oDir[:-1]+".root", "recreate")
  if makeMaskList :
    fMaskList = "../data/GEMMaskVecRun%06d.dat"%run
    fHotStripList = "../data/GEMHotVecRun%06d.dat"%run
    if os.path.isfile(fMaskList) : 
      print "MaskList file exist \n"*10
      sys.exit()
    if os.path.isfile(fHotStripList) : 
      print "HotStripList file exist \n"*10
      sys.exit()
    mask = open(fMaskList, "w")
    hotStrip = open(fHotStripList, "w")
  for c in chamber:
    outRoot.mkdir(c.replace("/",""))
    os.system("mkdir -p "+oDir+"/"+c.replace("/",""))  
  draw_plot(rootF,tDir,oDir)  
  dump(rootF, tDir, outRoot) 
  outRoot.Write()
  outRoot.Close() 
  makeSummary()
