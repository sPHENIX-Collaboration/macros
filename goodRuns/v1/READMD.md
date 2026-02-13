Usage:
 ./GoldenRunList_ConderFile.sh [removeRunsWithMissingMaps] [dontGenerateFileLists]

 Optional Arguments:
   removeRunsWithMissingMaps: If provided, runs that lack bad tower maps are excluded.
   dontGenerateFileLists: If provided, the final step of generating DST lists is skipped.

 The script retrieves, filters, and processes run information based on event criteria, calorimeter quality assurance
 (Calo QA), runtime, livetime, and bad tower map availability. If removeRunsWithMissingMaps is specified, runs without
 bad tower maps are removed. If dontGenerateFileLists is specified, the DST list creation step at the end is not executed.

 Logic, output, and ordering remain unchanged unless dontGenerateFileLists is specified. In that case, all steps occur as
 before, except for DST list generation.
