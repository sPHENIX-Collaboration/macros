#!/usr/bin/env python3

from ROOT import TCanvas, TFile, TH1F,TH2F, TF1, TGraph, gROOT
import math
import numpy as np
from array import array
import random

gROOT.SetBatch(True)


X = array('d',    np.arange(0.5, 20,1)  )
Y = array('d',    np.arange(0.5, 20,1)  )
#print(X)
anode_iroc_38 = [
    [-1,23,28,31,32,33,31,29,27,-1,26,26,25,25,25,26,23,20,-1,-1],
    [-1,21,30,38,40,38,35,33,29,-1,27,28,30,29,28,29,27,17,-1,-1],
    [-1,-1,30,41,45,43,37,33,28,-1,25,27,27,27,26,26,24,-1,-1,-1],
    [-1,-1,29,40,44,41,36,32,26,-1,21,19,20,22,26,27,26,-1,-1,-1],
    [-1,-1,29,39,42,39,35,31,25,-1,20,20,20,22,25,26,24,-1,-1,-1],
    [-1,-1,29,36,37,35,33,30,26,-1,21,19,20,22,26,27,26,-1,-1,-1],
    [-1,-1,25,28,27,26,26,26,23,-1,18,18,18,20,22,22,22,-1,-1,-1],
    [-1,-1,-1,25,25,25,25,24,21,-1,16,15,15,17,18,20,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,27,26,26,24,22,19,-1,15,14,14,15,17,20,-1,-1,-1,-1],
    [-1,-1,-1,29,28,26,24,21,17,-1,15,15,16,19,22,24,-1,-1,-1,-1],
    [-1,-1,-1,25,25,23,20,17,14,-1,14,15,16,18,20,22,-1,-1,-1,-1],
    [-1,-1,-1,23,22,21,18,16,15,-1,15,16,17,19,21,17,-1,-1,-1,-1],
    [-1,-1,-1,-1,24,21,19,18,16,-1,18,21,22,24,25,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,25,24,23,21,20,-1,24,27,28,29,27,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,26,27,28,26,25,-1,27,30,33,32,28,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,26,30,32,31,28,-1,31,33,36,37,30,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,25,28,30,29,27,-1,29,30,31,33,28,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
]
measuredibf_iroc_38 = [
    [-1,90,70,61,62,63,65,66,69,-1,77,75,76,87,92,94,97,92,-1,-1],
    [-1,84,62,47,47,52,55,54,62,-1,67,63,67,79,87,79,73,81,-1,-1],
    [-1,-1,60,40,39,44,47,50,63,-1,66,63,73,82,82,66,67,-1,-1,-1],
    [-1,-1,66,52,42,41,48,55,62,-1,91,79,72,75,81,86,78,-1,-1,-1],
    [-1,-1,64,53,41,41,49,55,59,-1,80,79,78,79,78,75,66,-1,-1,-1],
    [-1,-1,69,53,44,47,57,58,60,-1,78,89,83,72,59,62,69,-1,-1,-1],
    [-1,-1,70,59,56,65,74,72,68,-1,87,91,90,78,72,76,73,-1,-1,-1],
    [-1,-1,-1,63,65,77,87,77,71,-1,102,106,104,95,85,72,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,60,59,56,62,73,92,-1,105,111,109,98,82,70,-1,-1,-1,-1],
    [-1,-1,-1,60,54,55,68,87,105,-1,111,104,101,78,68,61,-1,-1,-1,-1],
    [-1,-1,-1,67,81,80,77,79,102,-1,130,119,94,84,77,58,-1,-1,-1,-1],
    [-1,-1,-1,74,89,81,76,88,115,-1,131,109,80,69,62,47,-1,-1,-1,-1],
    [-1,-1,-1,-1,75,66,69,92,121,-1,77,78,82,64,53,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,65,57,64,88,106,-1,63,70,74,60,48,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,65,64,61,60,68,-1,54,62,65,52,41,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,65,55,46,43,59,-1,52,61,62,51,38,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,47,40,38,39,58,-1,58,61,61,56,38,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
]

anode_iroc_35 = [
    [-1,22,25,27,27,26,24,24,20,-1,21,22,23,25,26,25,25,20,-1,-1],
    [-1,18,32,37,36,32,29,27,22,-1,22,25,27,31,35,34,29,19,-1,-1],
    [-1,-1,36,44,41,35,33,29,23,-1,22,25,29,34,40,40,37,-1,-1,-1],
    [-1,-1,37,43,41,36,33,29,24,-1,22,25,28,33,39,41,37,-1,-1,-1],
    [-1,-1,37,42,40,37,34,30,25,-1,24,25,27,32,39,39,33,-1,-1,-1],
    [-1,-1,36,42,42,38,34,30,25,-1,24,25,27,32,37,39,32,-1,-1,-1],
    [-1,-1,29,39,39,35,30,27,23,-1,23,23,24,27,31,33,24,-1,-1,-1],
    [-1,-1,-1,34,34,31,29,27,24,-1,24,24,25,27,29,31,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,31,30,28,26,25,23,-1,25,25,26,28,29,29,-1,-1,-1,-1],
    [-1,-1,-1,30,31,29,26,25,23,-1,25,26,27,29,31,30,-1,-1,-1,-1],
    [-1,-1,-1,31,34,33,30,27,23,-1,26,27,29,32,33,28,-1,-1,-1,-1],
    [-1,-1,-1,24,37,37,33,29,24,-1,25,27,29,32,32,17,-1,-1,-1,-1],
    [-1,-1,-1,-1,39,42,38,32,26,-1,26,27,30,34,31,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,40,46,41,34,28,-1,28,29,33,36,32,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,34,42,39,33,27,-1,28,29,33,34,28,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,31,36,36,32,28,-1,30,31,34,34,28,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,25,27,29,25,24,-1,25,25,26,26,21,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
]
measuredibf_iroc_35 = [
    [-1,42,43,38,40,44,53,52,50,-1,76,67,55,47,42,45,48,46,-1,-1],
    [-1,36,37,34,34,36,43,49,51,-1,66,67,57,44,35,35,42,33,-1,-1],
    [-1,-1,47,36,36,34,36,44,56,-1,56,65,60,47,36,32,33,-1,-1,-1],
    [-1,-1,31,30,39,49,53,52,51,-1,56,51,44,39,39,38,41,-1,-1,-1],
    [-1,-1,31,29,35,43,50,52,52,-1,58,56,47,40,37,39,44,-1,-1,-1],
    [-1,-1,32,30,31,36,44,50,50,-1,48,55,63,65,55,46,40,-1,-1,-1],
    [-1,-1,31,33,33,36,43,48,53,-1,57,58,63,62,56,49,38,-1,-1,-1],
    [-1,-1,-1,35,35,37,41,47,52,-1,68,74,69,59,50,46,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,54,63,72,71,61,55,-1,62,68,67,64,66,67,-1,-1,-1,-1],
    [-1,-1,-1,54,60,63,62,55,53,-1,60,65,66,63,61,56,-1,-1,-1,-1],
    [-1,-1,-1,42,39,37,39,43,52,-1,57,50,43,40,41,42,-1,-1,-1,-1],
    [-1,-1,-1,35,35,33,35,41,50,-1,64,53,43,39,41,36,-1,-1,-1,-1],
    [-1,-1,-1,-1,33,30,33,40,51,-1,52,57,61,59,57,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,31,27,32,42,52,-1,47,49,53,55,59,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,39,32,33,39,51,-1,46,44,48,53,57,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,40,34,34,37,46,-1,45,41,43,50,54,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,42,43,43,42,46,-1,50,46,47,51,49,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],
    [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
]

def makeLocalList(measuredibf_iroc_38, i, j):
    #print(measuredibf_iroc_38[i][j])
    nonZeroList = []
    len_list = len(measuredibf_iroc_38[i])
    #print(len_list)
    #for l in range(0,8):
    j_new = j-1
    if j_new>=0:
        if measuredibf_iroc_38[i][j_new]>0 :
            nonZeroList.append(measuredibf_iroc_38[i][j_new])
    j_new = j+1
    if j_new<len_list:
        if measuredibf_iroc_38[i][j_new]>0:
            nonZeroList.append(measuredibf_iroc_38[i][j_new])
    if not len(nonZeroList):
        len_list_y = len(measuredibf_iroc_38)
        i_new = i-1
        if i_new>=0:
            if measuredibf_iroc_38[i_new][j]>0 :
                nonZeroList.append(measuredibf_iroc_38[i_new][j])
        i_new = i+1
        if i_new<len_list_y:
            if measuredibf_iroc_38[i_new][j]>0:
                nonZeroList.append(measuredibf_iroc_38[i_new][j])




    return np.array(nonZeroList)


def fill_hist(h_measuredibf_iroc_38,measuredibf_iroc_38,measuredibf_iroc_38_new,f_middle=0):
    #measuredibf_iroc_38_new = [[0 for i in range(len(Y))] for j in range(len(X))]
    len_X = len(measuredibf_iroc_38[0])
    len_Y = len(measuredibf_iroc_38)
    all_values = []
    for i in range(0,len_X):
        for j in range(len_Y-1,-1,-1):

            ibf = measuredibf_iroc_38[j][i]
            if ibf<=0:
                ibf_list = makeLocalList(measuredibf_iroc_38,j,i)
                if len(ibf_list)>1:
                   ibf = ibf_list.mean()

            if ibf>0:
                #print('X = {} Y = {}'.format(0.5+i,0.5+(len_Y-1-j)))
                h_measuredibf_iroc_38.Fill(0.5+i,0.5+(len_Y-1-j),ibf)
                measuredibf_iroc_38_new[j][i] = ibf
                all_values.append(ibf)
                #h_anode_iroc_38.Fill(X[i],Y[19-j],anode_iroc_38[j][i])
            
    if f_middle==1:
        i = 9 
        j = 8
        ibf_list = makeLocalList(measuredibf_iroc_38_new,j,i)
        #print(measuredibf_iroc_38_new[j][i])
        if len(ibf_list)>1:
            h_measuredibf_iroc_38.SetBinContent(i+1,19-j+1,ibf_list.mean())
            measuredibf_iroc_38_new[j][i] = ibf_list.mean()
            all_values.append(ibf_list.mean())
        mean_all_values = np.mean(all_values)
        h_measuredibf_iroc_38.Scale(1./mean_all_values)
        for k in range(len_X) :
            for l in range(len_Y) :
                if measuredibf_iroc_38_new[k][l]>0:
                    measuredibf_iroc_38_new[k][l] = measuredibf_iroc_38_new[k][l]/mean_all_values


def fill_hist_100_100(h_tot, measuredibf_tot, measuredibf_1, measuredibf_2, size_XY):
    j_bias = len(measuredibf_2)-1 
    fill_X_N_times = len(measuredibf_tot)//(j_bias+1)
    fill_Y_N_times = len(measuredibf_tot)//(j_bias+1)
    for i in range(0,len(measuredibf_2[0])):
        for j in range(len(measuredibf_2)-1,-1,-1):
            for k in range(fill_Y_N_times):
                ibf_1 = measuredibf_1[j][i]
                ibf_2 = measuredibf_2[j_bias-j][i]
                if k % 2:
                    ibf_1 = measuredibf_2[j][i]
                    ibf_2 = measuredibf_1[j_bias-j][i]

                for n in range(fill_X_N_times):
                    #print(n)
                    if ibf_1>0:
                        h_tot.Fill(X[i]+14*2*n,Y[j_bias-j]+j_bias*k-2,ibf_1)
                    if ibf_2>0:
                        h_tot.Fill(X[i]+14*(2*n+1),Y[j_bias-j]+j_bias*k,ibf_2)
                    #print('{0} & {1}'.format(20*2*n,20*(2*n+1)))
    x_bins = len(measuredibf_tot[0])
    y_bins = len(measuredibf_tot)
    len_tot = len(measuredibf_tot)
    for i in range(0,x_bins):
        for j in range(0,y_bins):
            measuredibf_tot[i][j] = h_tot.GetBinContent(j+1,len_tot-i)



def IsOverFrame(r, phi):
    #these parameters are taken from Feb 12 drawings of frames.
    
    tpc_frame_side_gap=0.8 #mm  #space between radial line and start of frame
    tpc_frame_side_width=2.6 #mm  #thickness of frame
    tpc_margin=0.0 #mm  # extra gap between edge of frame and start of GEM holes

    tpc_frame_r3_outer=758.4 #mm inner edge of larger-r frame of r3
    tpc_frame_r3_inner=583.5 #mm outer edge of smaller-r frame of r3

    tpc_frame_r2_outer=574.9 #mm inner edge of larger-r frame of r2
    tpc_frame_r2_inner=411.4 #mm outer edge of smaller-r frame of r2

    tpc_frame_r1_outer=402.6 #mm inner edge of larger-r frame of r1
    tpc_frame_r1_inner=221.0 #mm outer edge of smaller-r frame of r1

    tpc_sec0_phi=0.0 #get_double_param("tpc_sec0_phi")
    #if the coordinate is in the radial spaces of the frames, return True:
    if r<tpc_frame_r1_inner+tpc_margin:
        return True
    if r>tpc_frame_r1_outer-tpc_margin  and r<tpc_frame_r2_inner+tpc_margin:
        return True
    if r>tpc_frame_r2_outer-tpc_margin  and r<tpc_frame_r3_inner+tpc_margin:
        return True
    if r>tpc_frame_r3_outer-tpc_margin:
        return True
    #if the coordinate is within gap+width of a sector boundary, return True:
    #note that this is not a line of constant radius, but a linear distance from a radius.

    #find the two spokes we're between:
    sectorangle=(math.pi/6)
    nsectors=phi/sectorangle
    nsec=math.floor(nsectors)
    reduced_phi=phi-nsec*sectorangle  #between zero and sixty degrees.
    dist_to_previous=r*math.sin(reduced_phi)
    dist_to_next=r*math.sin(sectorangle-reduced_phi)
    if dist_to_previous<tpc_frame_side_gap+tpc_frame_side_width+tpc_margin :
        return True
    if dist_to_next<tpc_frame_side_gap+tpc_frame_side_width+tpc_margin :
        return True

    return False

def nSection(r, phi):
    #these parameters are taken from Feb 12 drawings of frames.
    
    tpc_frame_side_gap=0.8 #mm  #space between radial line and start of frame
    tpc_frame_side_width=2.6 #mm  #thickness of frame
    tpc_margin=0.0 #mm  # extra gap between edge of frame and start of GEM holes

    tpc_frame_r3_outer=758.4 #mm inner edge of larger-r frame of r3
    tpc_frame_r3_inner=583.5 #mm outer edge of smaller-r frame of r3

    tpc_frame_r2_outer=574.9 #mm inner edge of larger-r frame of r2
    tpc_frame_r2_inner=411.4 #mm outer edge of smaller-r frame of r2

    tpc_frame_r1_outer=402.6 #mm inner edge of larger-r frame of r1
    tpc_frame_r1_inner=221.0 #mm outer edge of smaller-r frame of r1

    tpc_sec0_phi=0.0 #get_double_param("tpc_sec0_phi")
    #if the coordinate is in the radial spaces of the frames, return True:
    rsec=0
    rin=0
    if r>=tpc_frame_r1_inner+tpc_margin and r<=tpc_frame_r1_outer+tpc_margin:
        rsec = 1
        rin = tpc_frame_r1_inner
    if r>=tpc_frame_r2_inner+tpc_margin and r<=tpc_frame_r2_outer+tpc_margin:
        rsec = 2
        rin = tpc_frame_r2_inner
    if r>=tpc_frame_r3_inner+tpc_margin and r<=tpc_frame_r3_outer+tpc_margin:
        rsec = 3
        rin = tpc_frame_r3_inner
    #if the coordinate is within gap+width of a sector boundary, return True:
    #note that this is not a line of constant radius, but a linear distance from a radius.

    #find the two spokes we're between:
    sectorangle=(math.pi/6)
    nsectors=phi/sectorangle
    nsec=math.floor(nsectors)

    return nsec+1 , rsec, rin

def fillMap(h_modules_result,h_modules_measuredibf,h_modules_anode,h_measuredibf_100_100_new,h_anode_100_100_new):
    moduleCodeXY = {}
    ibf_list = []
    anode_list = []
    NbinsX = h_modules_measuredibf.GetNbinsX()
    NbinsY = h_modules_measuredibf.GetNbinsY()
    for i in range(0,NbinsX):
        x = h_modules_measuredibf.GetXaxis().GetBinCenter(i)
        for j in range(0,NbinsY):
            y = h_modules_measuredibf.GetXaxis().GetBinCenter(j)
            r = math.sqrt(x*x+y*y)
            phi = math.atan2(x,y)
            if phi<0:
                phi+=2*math.pi

            if not IsOverFrame(r,phi):
                nsec, rsec, rin = nSection(r, phi)
                #print(nSection(r, phi))
                mc = 100*nsec + rsec
                if mc not in moduleCodeXY.keys():
                    #moduleCode.append(mc)
                    x_mc = random.uniform(60, 145)
                    y_mc = random.uniform(0, 75)
                    moduleCodeXY[mc] = [x_mc, y_mc]
                deg60 = math.pi/6
                #print((phi-nsec*deg60)/deg60)
                moduleX = moduleCodeXY[mc][0]+(phi-nsec*deg60)*r/10#((phi-nsec*deg60)/deg60+1)*rin/221
                moduleY = moduleCodeXY[mc][1]+(r-rin)/10
                h_x = h_measuredibf_100_100_new.GetXaxis().FindBin(moduleX)
                h_y = h_measuredibf_100_100_new.GetXaxis().FindBin(moduleY)
                w = h_measuredibf_100_100_new.GetBinContent(h_x,h_y)
                w_a = h_anode_100_100_new.GetBinContent(h_x,h_y)
                if w>0:
                    h_modules_measuredibf.Fill(x,y,w) 
                    h_modules_anode.Fill(x,y,w_a) 
                    h_modules_result.Fill(x,y,w*w_a) 
                    ibf_list.append(w)
                    anode_list.append(w_a)
    #h_modules_measuredibf.Scale(1./np.mean(ibf_list))
    #h_modules_anode.Scale(1./np.mean(anode_list))



h_measuredibf_iroc_38 = TH2F("h_measuredibf_iroc_38","h_measuredibf_iroc_38",20,0,20,20,0,20)#100,0,100,100,0,100)
h_anode_iroc_38 = TH2F("h_anode_iroc_38","h_anode_iroc_38",20,0,20,20,0,20)

h_measuredibf_iroc_35 = TH2F("h_measuredibf_iroc_35","h_measuredibf_iroc_35",20,0,20,20,0,20)#100,0,100,100,0,100)
h_anode_iroc_35 = TH2F("h_anode_iroc_35","h_anode_iroc_35",20,0,20,20,0,20)

measuredibf_iroc_38_new = [[0 for i in range(len(Y))] for j in range(len(X))]
anode_iroc_38_new = [[0 for i in range(len(Y))] for j in range(len(X))]
measuredibf_iroc_35_new = [[0 for i in range(len(Y))] for j in range(len(X))]
anode_iroc_35_new = [[0 for i in range(len(Y))] for j in range(len(X))]

fill_hist(h_measuredibf_iroc_38,measuredibf_iroc_38,measuredibf_iroc_38_new,1)
fill_hist(h_anode_iroc_38,anode_iroc_38,anode_iroc_38_new,1)
fill_hist(h_measuredibf_iroc_35,measuredibf_iroc_35,measuredibf_iroc_35_new,1)
fill_hist(h_anode_iroc_35,anode_iroc_35,anode_iroc_35_new,1)

#divede_by_mean(measuredibf_iroc_38_new)
#divede_by_mean(anode_iroc_38_new)
#divede_by_mean(measuredibf_iroc_35_new)
#divede_by_mean(anode_iroc_35_new)

size_XY = 200
h_measuredibf_100_100 = TH2F("h_measuredibf_100_100","h_measuredibf_100_100",size_XY,0,size_XY,size_XY,0,size_XY)
h_measuredibf_100_100_new = TH2F("h_measuredibf_100_100_new","h_measuredibf_100_100_new",size_XY,0,size_XY,size_XY,0,size_XY)
h_anode_100_100 = TH2F("h_anode_100_100","h_anode_100_100",size_XY,0,size_XY,size_XY,0,size_XY)
h_anode_100_100_new = TH2F("h_anode_100_100_new","h_anode_100_100_new",size_XY,0,size_XY,size_XY,0,size_XY)

measuredibf_100_100 = [[0 for i in range(size_XY)] for j in range(size_XY)]
measuredibf_100_100_new = [[0 for i in range(size_XY)] for j in range(size_XY)]
anode_100_100 = [[0 for i in range(size_XY)] for j in range(size_XY)]
anode_100_100_new = [[0 for i in range(size_XY)] for j in range(size_XY)]

fill_hist_100_100(h_measuredibf_100_100,measuredibf_100_100, measuredibf_iroc_38_new, measuredibf_iroc_35_new, size_XY)
fill_hist(h_measuredibf_100_100_new,measuredibf_100_100,measuredibf_100_100_new)

fill_hist_100_100(h_anode_100_100,anode_100_100, anode_iroc_38_new, anode_iroc_35_new, size_XY)
fill_hist(h_anode_100_100_new,anode_100_100,anode_100_100_new)

#h_modules = TH2F('h_modules','h_modules',1600,-800,800,1600,-800,800)
h_modules_measuredibf = TH2F('h_modules_measuredibf','h_modules_measuredibf',1600,-800,800,1600,-800,800)
h_modules_anode = TH2F('h_modules_anode','h_modules_anode',1600,-800,800,1600,-800,800)
h_modules_result = TH2F('h_modules_result','h_modules_result',1600,-800,800,1600,-800,800)
#moduleCode = []
#section_numbers = [[0 for i in range(1600)] for j in range(1600)]


fillMap(h_modules_result,h_modules_measuredibf,h_modules_anode,h_measuredibf_100_100_new,h_anode_100_100_new)
        
#print(moduleCodeXY)
outfile = TFile("./Files/IBF_Map.root", "RECREATE");

h_modules_anode.Write()
h_modules_measuredibf.Write()

outfile.Write()
outfile.Close()

# DRAWING
c_modules_measuredibf = TCanvas('c_modules_measuredibf','c_modules_measuredibf',1600,1600)
c_modules_measuredibf.SetRightMargin(0.11)

h_modules_measuredibf.GetYaxis().SetTitle('Y, [mm]')
h_modules_measuredibf.GetXaxis().SetTitle('X, [mm]')
h_modules_measuredibf.Draw('colz')
c_modules_measuredibf.Print('./Plots/h_map_modules_measuredibf.png')

c_modules_anode = TCanvas('c_modules_anode','c_modules_anode',1600,1600)
c_modules_anode.SetRightMargin(0.11)

h_modules_anode.GetYaxis().SetTitle('Y, [mm]')
h_modules_anode.GetXaxis().SetTitle('X, [mm]')
h_modules_anode.Draw('colz')
c_modules_anode.Print('./Plots/h_map_modules_anode.png')

c_modules_result = TCanvas('c_modules_result','c_modules_result',1600,1600)
c_modules_result.SetRightMargin(0.11)

h_modules_result.Scale(20*0.44)

h_modules_result.GetYaxis().SetTitle('Y, [mm]')
h_modules_result.GetXaxis().SetTitle('X, [mm]')
h_modules_result.Draw('colz')
c_modules_result.Print('./Plots/h_map_modules_result.png')


c_measuredibf_100_100 = TCanvas('c_measuredibf_100_100','c_measuredibf_100_100',1600,1600)
c_measuredibf_100_100.SetRightMargin(0.11)

h_measuredibf_100_100_new.GetYaxis().SetTitle('Y, [mm]')
h_measuredibf_100_100_new.GetXaxis().SetTitle('X, [mm]')
h_measuredibf_100_100_new.Draw('colz')
c_measuredibf_100_100.Print('./Plots/h_map_measuredibf_100_100.png')

c_measuredibf_iroc_38 = TCanvas()
c_measuredibf_iroc_38.SetRightMargin(0.11)

h_measuredibf_iroc_38.GetYaxis().SetTitle('Y, [cm]')
h_measuredibf_iroc_38.GetXaxis().SetTitle('X, [cm]')
h_measuredibf_iroc_38.Draw('colz')
c_measuredibf_iroc_38.Print('./Plots/h_map_measuredibf_iroc_38.png')

c_anode_iroc_38 = TCanvas()
c_anode_iroc_38.SetRightMargin(0.11)

h_anode_iroc_38.GetYaxis().SetTitle('Y, [cm]')
h_anode_iroc_38.GetXaxis().SetTitle('X, [cm]')
h_anode_iroc_38.Draw('colz')
c_anode_iroc_38.Print('./Plots/h_map_anode_iroc_38.png')

c_measuredibf_iroc_35 = TCanvas()
c_measuredibf_iroc_35.SetRightMargin(0.11)

h_measuredibf_iroc_35.GetYaxis().SetTitle('Y, [cm]')
h_measuredibf_iroc_35.GetXaxis().SetTitle('X, [cm]')
h_measuredibf_iroc_35.Draw('colz')
c_measuredibf_iroc_35.Print('./Plots/h_map_measuredibf_iroc_35.png')

c_anode_iroc_35 = TCanvas()
c_anode_iroc_35.SetRightMargin(0.11)

h_anode_iroc_35.GetYaxis().SetTitle('Y, [cm]')
h_anode_iroc_35.GetXaxis().SetTitle('X, [cm]')

h_anode_iroc_35.Draw('colz')
c_anode_iroc_35.Print('./Plots/h_map_anode_iroc_35.png')
