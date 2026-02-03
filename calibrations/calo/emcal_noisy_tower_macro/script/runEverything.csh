#!/bin/tcsh

setupDirs.csh

generateFileLists.csh 403 2023 011

runHotTowerMacroMultiStage.csh fileLists/listOfLists.list
