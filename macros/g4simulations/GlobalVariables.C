#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
static bool overlapcheck = false;
static double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
#endif
