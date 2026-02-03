<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.1">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin">
<packages>
<package name="SEAM-30-02.0-L-06-1-A-K-TR">
<smd name="P$1" x="-3.6449" y="-18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$2" x="-3.6449" y="-17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$3" x="-3.6449" y="-15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$4" x="-3.6449" y="-14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$5" x="-3.6449" y="-13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$6" x="-3.6449" y="-12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$7" x="-3.6449" y="-10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$8" x="-3.6449" y="-9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$9" x="-3.6449" y="-8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$10" x="-3.6449" y="-6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$11" x="-3.6449" y="-5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$12" x="-3.6449" y="-4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$13" x="-3.6449" y="-3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$14" x="-3.6449" y="-1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$15" x="-3.6449" y="-0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$16" x="-3.6449" y="0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$17" x="-3.6449" y="1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$18" x="-3.6449" y="3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$19" x="-3.6449" y="4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$20" x="-3.6449" y="5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$21" x="-3.6449" y="6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$22" x="-3.6449" y="8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$23" x="-3.6449" y="9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$24" x="-3.6449" y="10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$25" x="-3.6449" y="12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$26" x="-3.6449" y="13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$27" x="-3.6449" y="14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$28" x="-3.6449" y="15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$29" x="-3.6449" y="17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$30" x="-3.6449" y="18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$31" x="-4.9149" y="-18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$32" x="-4.9149" y="-17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$33" x="-4.9149" y="-15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$34" x="-4.9149" y="-14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$35" x="-4.9149" y="-13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$36" x="-4.9149" y="-12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$37" x="-4.9149" y="-10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$38" x="-4.9149" y="-9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$39" x="-4.9149" y="-8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$40" x="-4.9149" y="-6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$41" x="-4.9149" y="-5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$42" x="-4.9149" y="-4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$43" x="-4.9149" y="-3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$44" x="-4.9149" y="-1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$45" x="-4.9149" y="-0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$46" x="-4.9149" y="0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$47" x="-4.9149" y="1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$48" x="-4.9149" y="3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$49" x="-4.9149" y="4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$50" x="-4.9149" y="5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$51" x="-4.9149" y="6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$52" x="-4.9149" y="8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$53" x="-4.9149" y="9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$54" x="-4.9149" y="10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$55" x="-4.9149" y="12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$56" x="-4.9149" y="13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$57" x="-4.9149" y="14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$58" x="-4.9149" y="15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$59" x="-4.9149" y="17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$60" x="-4.9149" y="18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$61" x="-6.1849" y="-18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$62" x="-6.1849" y="-17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$63" x="-6.1849" y="-15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$64" x="-6.1849" y="-14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$65" x="-6.1849" y="-13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$66" x="-6.1849" y="-12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$67" x="-6.1849" y="-10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$68" x="-6.1849" y="-9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$69" x="-6.1849" y="-8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$70" x="-6.1849" y="-6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$71" x="-6.1849" y="-5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$72" x="-6.1849" y="-4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$73" x="-6.1849" y="-3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$74" x="-6.1849" y="-1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$75" x="-6.1849" y="-0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$76" x="-6.1849" y="0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$77" x="-6.1849" y="1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$78" x="-6.1849" y="3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$79" x="-6.1849" y="4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$80" x="-6.1849" y="5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$81" x="-6.1849" y="6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$82" x="-6.1849" y="8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$83" x="-6.1849" y="9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$84" x="-6.1849" y="10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$85" x="-6.1849" y="12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$86" x="-6.1849" y="13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$87" x="-6.1849" y="14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$88" x="-6.1849" y="15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$89" x="-6.1849" y="17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$90" x="-6.1849" y="18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$91" x="-7.4549" y="-18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$92" x="-7.4549" y="-17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$93" x="-7.4549" y="-15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$94" x="-7.4549" y="-14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$95" x="-7.4549" y="-13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$96" x="-7.4549" y="-12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$97" x="-7.4549" y="-10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$98" x="-7.4549" y="-9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$99" x="-7.4549" y="-8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$100" x="-7.4549" y="-6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$101" x="-7.4549" y="-5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$102" x="-7.4549" y="-4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$103" x="-7.4549" y="-3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$104" x="-7.4549" y="-1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$105" x="-7.4549" y="-0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$106" x="-7.4549" y="0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$107" x="-7.4549" y="1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$108" x="-7.4549" y="3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$109" x="-7.4549" y="4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$110" x="-7.4549" y="5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$111" x="-7.4549" y="6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$112" x="-7.4549" y="8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$113" x="-7.4549" y="9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$114" x="-7.4549" y="10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$115" x="-7.4549" y="12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$116" x="-7.4549" y="13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$117" x="-7.4549" y="14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$118" x="-7.4549" y="15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$119" x="-7.4549" y="17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$120" x="-7.4549" y="18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$121" x="-8.7249" y="-18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$122" x="-8.7249" y="-17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$123" x="-8.7249" y="-15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$124" x="-8.7249" y="-14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$125" x="-8.7249" y="-13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$126" x="-8.7249" y="-12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$127" x="-8.7249" y="-10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$128" x="-8.7249" y="-9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$129" x="-8.7249" y="-8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$130" x="-8.7249" y="-6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$131" x="-8.7249" y="-5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$132" x="-8.7249" y="-4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$133" x="-8.7249" y="-3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$134" x="-8.7249" y="-1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$135" x="-8.7249" y="-0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$136" x="-8.7249" y="0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$137" x="-8.7249" y="1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$138" x="-8.7249" y="3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$139" x="-8.7249" y="4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$140" x="-8.7249" y="5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$141" x="-8.7249" y="6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$142" x="-8.7249" y="8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$143" x="-8.7249" y="9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$144" x="-8.7249" y="10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$145" x="-8.7249" y="12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$146" x="-8.7249" y="13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$147" x="-8.7249" y="14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$148" x="-8.7249" y="15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$149" x="-8.7249" y="17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$150" x="-8.7249" y="18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$151" x="-9.9949" y="-18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$152" x="-9.9949" y="-17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$153" x="-9.9949" y="-15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$154" x="-9.9949" y="-14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$155" x="-9.9949" y="-13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$156" x="-9.9949" y="-12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$157" x="-9.9949" y="-10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$158" x="-9.9949" y="-9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$159" x="-9.9949" y="-8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$160" x="-9.9949" y="-6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$161" x="-9.9949" y="-5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$162" x="-9.9949" y="-4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$163" x="-9.9949" y="-3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$164" x="-9.9949" y="-1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$165" x="-9.9949" y="-0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$166" x="-9.9949" y="0.635" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$167" x="-9.9949" y="1.905" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$168" x="-9.9949" y="3.175" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$169" x="-9.9949" y="4.445" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$170" x="-9.9949" y="5.715" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$171" x="-9.9949" y="6.985" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$172" x="-9.9949" y="8.255" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$173" x="-9.9949" y="9.525" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$174" x="-9.9949" y="10.795" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$175" x="-9.9949" y="12.065" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$176" x="-9.9949" y="13.335" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$177" x="-9.9949" y="14.605" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$178" x="-9.9949" y="15.875" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$179" x="-9.9949" y="17.145" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<smd name="P$180" x="-9.9949" y="18.415" dx="0.635" dy="0.635" layer="1" roundness="100"/>
<wire x1="-11.6205" y1="21.5392" x2="-2.0193" y2="21.5392" width="0.127" layer="51"/>
<wire x1="-2.0193" y1="21.5392" x2="-2.0193" y2="-21.5392" width="0.127" layer="51"/>
<wire x1="-2.0193" y1="-21.5392" x2="-6.0579" y2="-21.5392" width="0.127" layer="51"/>
<wire x1="-6.0579" y1="-21.5392" x2="-6.0579" y2="-22.4282" width="0.127" layer="51"/>
<wire x1="-6.0579" y1="-22.4282" x2="-7.5819" y2="-22.4282" width="0.127" layer="51"/>
<wire x1="-7.5819" y1="-22.4282" x2="-7.5819" y2="-21.5392" width="0.127" layer="51"/>
<wire x1="-7.5819" y1="-21.5392" x2="-11.6205" y2="-21.5392" width="0.127" layer="51"/>
<wire x1="-11.6205" y1="-21.5392" x2="-11.6205" y2="21.5392" width="0.127" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="SEAM-30-02.0-L-06-1-A-K-TR">
<pin name="A1" x="-43.18" y="27.94" length="middle"/>
<pin name="A2" x="-43.18" y="25.4" length="middle"/>
<pin name="A3" x="-43.18" y="22.86" length="middle"/>
<pin name="A4" x="-43.18" y="20.32" length="middle"/>
<pin name="A5" x="-43.18" y="17.78" length="middle"/>
<pin name="A6" x="-43.18" y="15.24" length="middle"/>
<pin name="A7" x="-43.18" y="12.7" length="middle"/>
<pin name="A8" x="-43.18" y="10.16" length="middle"/>
<pin name="A9" x="-43.18" y="7.62" length="middle"/>
<pin name="A10" x="-43.18" y="5.08" length="middle"/>
<pin name="A11" x="-43.18" y="2.54" length="middle"/>
<pin name="A12" x="-43.18" y="0" length="middle"/>
<pin name="A13" x="-43.18" y="-2.54" length="middle"/>
<pin name="A14" x="-43.18" y="-5.08" length="middle"/>
<pin name="A15" x="-43.18" y="-7.62" length="middle"/>
<pin name="A16" x="-43.18" y="-10.16" length="middle"/>
<pin name="A17" x="-43.18" y="-12.7" length="middle"/>
<pin name="A18" x="-43.18" y="-15.24" length="middle"/>
<pin name="A19" x="-43.18" y="-17.78" length="middle"/>
<pin name="A20" x="-43.18" y="-20.32" length="middle"/>
<pin name="A21" x="-43.18" y="-22.86" length="middle"/>
<pin name="A22" x="-43.18" y="-25.4" length="middle"/>
<pin name="A23" x="-43.18" y="-27.94" length="middle"/>
<pin name="A24" x="-43.18" y="-30.48" length="middle"/>
<pin name="A25" x="-43.18" y="-33.02" length="middle"/>
<pin name="A26" x="-43.18" y="-35.56" length="middle"/>
<pin name="A27" x="-43.18" y="-38.1" length="middle"/>
<pin name="A28" x="-43.18" y="-40.64" length="middle"/>
<pin name="A29" x="-43.18" y="-43.18" length="middle"/>
<pin name="A30" x="-43.18" y="-45.72" length="middle"/>
<pin name="B1" x="-17.78" y="27.94" length="middle"/>
<pin name="B2" x="-17.78" y="25.4" length="middle"/>
<pin name="B3" x="-17.78" y="22.86" length="middle"/>
<pin name="B4" x="-17.78" y="20.32" length="middle"/>
<pin name="B5" x="-17.78" y="17.78" length="middle"/>
<pin name="B6" x="-17.78" y="15.24" length="middle"/>
<pin name="B7" x="-17.78" y="12.7" length="middle"/>
<pin name="B8" x="-17.78" y="10.16" length="middle"/>
<pin name="B9" x="-17.78" y="7.62" length="middle"/>
<pin name="B10" x="-17.78" y="5.08" length="middle"/>
<pin name="B11" x="-17.78" y="2.54" length="middle"/>
<pin name="B12" x="-17.78" y="0" length="middle"/>
<pin name="B13" x="-17.78" y="-2.54" length="middle"/>
<pin name="B14" x="-17.78" y="-5.08" length="middle"/>
<pin name="B15" x="-17.78" y="-7.62" length="middle"/>
<pin name="B16" x="-17.78" y="-10.16" length="middle"/>
<pin name="B17" x="-17.78" y="-12.7" length="middle"/>
<pin name="B18" x="-17.78" y="-15.24" length="middle"/>
<pin name="B19" x="-17.78" y="-17.78" length="middle"/>
<pin name="B20" x="-17.78" y="-20.32" length="middle"/>
<pin name="B21" x="-17.78" y="-22.86" length="middle"/>
<pin name="B22" x="-17.78" y="-25.4" length="middle"/>
<pin name="B23" x="-17.78" y="-27.94" length="middle"/>
<pin name="B24" x="-17.78" y="-30.48" length="middle"/>
<pin name="B25" x="-17.78" y="-33.02" length="middle"/>
<pin name="B26" x="-17.78" y="-35.56" length="middle"/>
<pin name="B27" x="-17.78" y="-38.1" length="middle"/>
<pin name="B28" x="-17.78" y="-40.64" length="middle"/>
<pin name="B29" x="-17.78" y="-43.18" length="middle"/>
<pin name="B30" x="-17.78" y="-45.72" length="middle"/>
<pin name="C1" x="7.62" y="27.94" length="middle"/>
<pin name="C2" x="7.62" y="25.4" length="middle"/>
<pin name="C3" x="7.62" y="22.86" length="middle"/>
<pin name="C4" x="7.62" y="20.32" length="middle"/>
<pin name="C5" x="7.62" y="17.78" length="middle"/>
<pin name="C6" x="7.62" y="15.24" length="middle"/>
<pin name="C7" x="7.62" y="12.7" length="middle"/>
<pin name="C8" x="7.62" y="10.16" length="middle"/>
<pin name="C9" x="7.62" y="7.62" length="middle"/>
<pin name="C10" x="7.62" y="5.08" length="middle"/>
<pin name="C11" x="7.62" y="2.54" length="middle"/>
<pin name="C12" x="7.62" y="0" length="middle"/>
<pin name="C13" x="7.62" y="-2.54" length="middle"/>
<pin name="C14" x="7.62" y="-5.08" length="middle"/>
<pin name="C15" x="7.62" y="-7.62" length="middle"/>
<pin name="C16" x="7.62" y="-10.16" length="middle"/>
<pin name="C17" x="7.62" y="-12.7" length="middle"/>
<pin name="C18" x="7.62" y="-15.24" length="middle"/>
<pin name="C19" x="7.62" y="-17.78" length="middle"/>
<pin name="C20" x="7.62" y="-20.32" length="middle"/>
<pin name="C21" x="7.62" y="-22.86" length="middle"/>
<pin name="C22" x="7.62" y="-25.4" length="middle"/>
<pin name="C23" x="7.62" y="-27.94" length="middle"/>
<pin name="C24" x="7.62" y="-30.48" length="middle"/>
<pin name="C25" x="7.62" y="-33.02" length="middle"/>
<pin name="C26" x="7.62" y="-35.56" length="middle"/>
<pin name="C27" x="7.62" y="-38.1" length="middle"/>
<pin name="C28" x="7.62" y="-40.64" length="middle"/>
<pin name="C29" x="7.62" y="-43.18" length="middle"/>
<pin name="C30" x="7.62" y="-45.72" length="middle"/>
<pin name="D1" x="33.02" y="27.94" length="middle"/>
<pin name="D2" x="33.02" y="25.4" length="middle"/>
<pin name="D3" x="33.02" y="22.86" length="middle"/>
<pin name="D4" x="33.02" y="20.32" length="middle"/>
<pin name="D5" x="33.02" y="17.78" length="middle"/>
<pin name="D6" x="33.02" y="15.24" length="middle"/>
<pin name="D7" x="33.02" y="12.7" length="middle"/>
<pin name="D8" x="33.02" y="10.16" length="middle"/>
<pin name="D9" x="33.02" y="7.62" length="middle"/>
<pin name="D10" x="33.02" y="5.08" length="middle"/>
<pin name="D11" x="33.02" y="2.54" length="middle"/>
<pin name="D12" x="33.02" y="0" length="middle"/>
<pin name="D13" x="33.02" y="-2.54" length="middle"/>
<pin name="D14" x="33.02" y="-5.08" length="middle"/>
<pin name="D15" x="33.02" y="-7.62" length="middle"/>
<pin name="D16" x="33.02" y="-10.16" length="middle"/>
<pin name="D17" x="33.02" y="-12.7" length="middle"/>
<pin name="D18" x="33.02" y="-15.24" length="middle"/>
<pin name="D19" x="33.02" y="-17.78" length="middle"/>
<pin name="D20" x="33.02" y="-20.32" length="middle"/>
<pin name="D21" x="33.02" y="-22.86" length="middle"/>
<pin name="D22" x="33.02" y="-25.4" length="middle"/>
<pin name="D23" x="33.02" y="-27.94" length="middle"/>
<pin name="D24" x="33.02" y="-30.48" length="middle"/>
<pin name="D25" x="33.02" y="-33.02" length="middle"/>
<pin name="D26" x="33.02" y="-35.56" length="middle"/>
<pin name="D27" x="33.02" y="-38.1" length="middle"/>
<pin name="D28" x="33.02" y="-40.64" length="middle"/>
<pin name="D29" x="33.02" y="-43.18" length="middle"/>
<pin name="D30" x="33.02" y="-45.72" length="middle"/>
<pin name="E1" x="58.42" y="27.94" length="middle"/>
<pin name="E2" x="58.42" y="25.4" length="middle"/>
<pin name="E3" x="58.42" y="22.86" length="middle"/>
<pin name="E4" x="58.42" y="20.32" length="middle"/>
<pin name="E5" x="58.42" y="17.78" length="middle"/>
<pin name="E6" x="58.42" y="15.24" length="middle"/>
<pin name="E7" x="58.42" y="12.7" length="middle"/>
<pin name="E8" x="58.42" y="10.16" length="middle"/>
<pin name="E9" x="58.42" y="7.62" length="middle"/>
<pin name="E10" x="58.42" y="5.08" length="middle"/>
<pin name="E11" x="58.42" y="2.54" length="middle"/>
<pin name="E12" x="58.42" y="0" length="middle"/>
<pin name="E13" x="58.42" y="-2.54" length="middle"/>
<pin name="E14" x="58.42" y="-5.08" length="middle"/>
<pin name="E15" x="58.42" y="-7.62" length="middle"/>
<pin name="E16" x="58.42" y="-10.16" length="middle"/>
<pin name="E17" x="58.42" y="-12.7" length="middle"/>
<pin name="E18" x="58.42" y="-15.24" length="middle"/>
<pin name="E19" x="58.42" y="-17.78" length="middle"/>
<pin name="E20" x="58.42" y="-20.32" length="middle"/>
<pin name="E21" x="58.42" y="-22.86" length="middle"/>
<pin name="E22" x="58.42" y="-25.4" length="middle"/>
<pin name="E23" x="58.42" y="-27.94" length="middle"/>
<pin name="E24" x="58.42" y="-30.48" length="middle"/>
<pin name="E25" x="58.42" y="-33.02" length="middle"/>
<pin name="E26" x="58.42" y="-35.56" length="middle"/>
<pin name="E27" x="58.42" y="-38.1" length="middle"/>
<pin name="E28" x="58.42" y="-40.64" length="middle"/>
<pin name="E29" x="58.42" y="-43.18" length="middle"/>
<pin name="E30" x="58.42" y="-45.72" length="middle"/>
<pin name="F1" x="83.82" y="27.94" length="middle"/>
<pin name="F2" x="83.82" y="25.4" length="middle"/>
<pin name="F3" x="83.82" y="22.86" length="middle"/>
<pin name="F4" x="83.82" y="20.32" length="middle"/>
<pin name="F5" x="83.82" y="17.78" length="middle"/>
<pin name="F6" x="83.82" y="15.24" length="middle"/>
<pin name="F7" x="83.82" y="12.7" length="middle"/>
<pin name="F8" x="83.82" y="10.16" length="middle"/>
<pin name="F9" x="83.82" y="7.62" length="middle"/>
<pin name="F10" x="83.82" y="5.08" length="middle"/>
<pin name="F11" x="83.82" y="2.54" length="middle"/>
<pin name="F12" x="83.82" y="0" length="middle"/>
<pin name="F13" x="83.82" y="-2.54" length="middle"/>
<pin name="F14" x="83.82" y="-5.08" length="middle"/>
<pin name="F15" x="83.82" y="-7.62" length="middle"/>
<pin name="F16" x="83.82" y="-10.16" length="middle"/>
<pin name="F17" x="83.82" y="-12.7" length="middle"/>
<pin name="F18" x="83.82" y="-15.24" length="middle"/>
<pin name="F19" x="83.82" y="-17.78" length="middle"/>
<pin name="F20" x="83.82" y="-20.32" length="middle"/>
<pin name="F21" x="83.82" y="-22.86" length="middle"/>
<pin name="F22" x="83.82" y="-25.4" length="middle"/>
<pin name="F23" x="83.82" y="-27.94" length="middle"/>
<pin name="F24" x="83.82" y="-30.48" length="middle"/>
<pin name="F25" x="83.82" y="-33.02" length="middle"/>
<pin name="F26" x="83.82" y="-35.56" length="middle"/>
<pin name="F27" x="83.82" y="-38.1" length="middle"/>
<pin name="F28" x="83.82" y="-40.64" length="middle"/>
<pin name="F29" x="83.82" y="-43.18" length="middle"/>
<pin name="F30" x="83.82" y="-45.72" length="middle"/>
<wire x1="-48.26" y1="30.48" x2="-40.64" y2="30.48" width="0.254" layer="94"/>
<wire x1="-40.64" y1="30.48" x2="-40.64" y2="-48.26" width="0.254" layer="94"/>
<wire x1="-40.64" y1="-48.26" x2="-48.26" y2="-48.26" width="0.254" layer="94"/>
<wire x1="-48.26" y1="-48.26" x2="-48.26" y2="30.48" width="0.254" layer="94"/>
<wire x1="-22.86" y1="30.48" x2="-15.24" y2="30.48" width="0.254" layer="94"/>
<wire x1="-15.24" y1="30.48" x2="-15.24" y2="-48.26" width="0.254" layer="94"/>
<wire x1="-15.24" y1="-48.26" x2="-22.86" y2="-48.26" width="0.254" layer="94"/>
<wire x1="-22.86" y1="-48.26" x2="-22.86" y2="30.48" width="0.254" layer="94"/>
<wire x1="2.54" y1="30.48" x2="10.16" y2="30.48" width="0.254" layer="94"/>
<wire x1="10.16" y1="30.48" x2="10.16" y2="-48.26" width="0.254" layer="94"/>
<wire x1="10.16" y1="-48.26" x2="2.54" y2="-48.26" width="0.254" layer="94"/>
<wire x1="2.54" y1="-48.26" x2="2.54" y2="30.48" width="0.254" layer="94"/>
<wire x1="27.94" y1="30.48" x2="35.56" y2="30.48" width="0.254" layer="94"/>
<wire x1="35.56" y1="30.48" x2="35.56" y2="-48.26" width="0.254" layer="94"/>
<wire x1="35.56" y1="-48.26" x2="27.94" y2="-48.26" width="0.254" layer="94"/>
<wire x1="27.94" y1="-48.26" x2="27.94" y2="30.48" width="0.254" layer="94"/>
<wire x1="53.34" y1="30.48" x2="60.96" y2="30.48" width="0.254" layer="94"/>
<wire x1="60.96" y1="30.48" x2="60.96" y2="-48.26" width="0.254" layer="94"/>
<wire x1="60.96" y1="-48.26" x2="53.34" y2="-48.26" width="0.254" layer="94"/>
<wire x1="53.34" y1="-48.26" x2="53.34" y2="30.48" width="0.254" layer="94"/>
<wire x1="78.74" y1="30.48" x2="86.36" y2="30.48" width="0.254" layer="94"/>
<wire x1="86.36" y1="30.48" x2="86.36" y2="-48.26" width="0.254" layer="94"/>
<wire x1="86.36" y1="-48.26" x2="78.74" y2="-48.26" width="0.254" layer="94"/>
<wire x1="78.74" y1="-48.26" x2="78.74" y2="30.48" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SEAM-30-02.0-L-06-1-A-K-TR">
<gates>
<gate name="G$1" symbol="SEAM-30-02.0-L-06-1-A-K-TR" x="-78.74" y="10.16"/>
</gates>
<devices>
<device name="" package="SEAM-30-02.0-L-06-1-A-K-TR">
<connects>
<connect gate="G$1" pin="A1" pad="P$1"/>
<connect gate="G$1" pin="A10" pad="P$10"/>
<connect gate="G$1" pin="A11" pad="P$11"/>
<connect gate="G$1" pin="A12" pad="P$12"/>
<connect gate="G$1" pin="A13" pad="P$13"/>
<connect gate="G$1" pin="A14" pad="P$14"/>
<connect gate="G$1" pin="A15" pad="P$15"/>
<connect gate="G$1" pin="A16" pad="P$16"/>
<connect gate="G$1" pin="A17" pad="P$17"/>
<connect gate="G$1" pin="A18" pad="P$18"/>
<connect gate="G$1" pin="A19" pad="P$19"/>
<connect gate="G$1" pin="A2" pad="P$2"/>
<connect gate="G$1" pin="A20" pad="P$20"/>
<connect gate="G$1" pin="A21" pad="P$21"/>
<connect gate="G$1" pin="A22" pad="P$22"/>
<connect gate="G$1" pin="A23" pad="P$23"/>
<connect gate="G$1" pin="A24" pad="P$24"/>
<connect gate="G$1" pin="A25" pad="P$25"/>
<connect gate="G$1" pin="A26" pad="P$26"/>
<connect gate="G$1" pin="A27" pad="P$27"/>
<connect gate="G$1" pin="A28" pad="P$28"/>
<connect gate="G$1" pin="A29" pad="P$29"/>
<connect gate="G$1" pin="A3" pad="P$3"/>
<connect gate="G$1" pin="A30" pad="P$30"/>
<connect gate="G$1" pin="A4" pad="P$4"/>
<connect gate="G$1" pin="A5" pad="P$5"/>
<connect gate="G$1" pin="A6" pad="P$6"/>
<connect gate="G$1" pin="A7" pad="P$7"/>
<connect gate="G$1" pin="A8" pad="P$8"/>
<connect gate="G$1" pin="A9" pad="P$9"/>
<connect gate="G$1" pin="B1" pad="P$31"/>
<connect gate="G$1" pin="B10" pad="P$40"/>
<connect gate="G$1" pin="B11" pad="P$41"/>
<connect gate="G$1" pin="B12" pad="P$42"/>
<connect gate="G$1" pin="B13" pad="P$43"/>
<connect gate="G$1" pin="B14" pad="P$44"/>
<connect gate="G$1" pin="B15" pad="P$45"/>
<connect gate="G$1" pin="B16" pad="P$46"/>
<connect gate="G$1" pin="B17" pad="P$47"/>
<connect gate="G$1" pin="B18" pad="P$48"/>
<connect gate="G$1" pin="B19" pad="P$49"/>
<connect gate="G$1" pin="B2" pad="P$32"/>
<connect gate="G$1" pin="B20" pad="P$50"/>
<connect gate="G$1" pin="B21" pad="P$51"/>
<connect gate="G$1" pin="B22" pad="P$52"/>
<connect gate="G$1" pin="B23" pad="P$53"/>
<connect gate="G$1" pin="B24" pad="P$54"/>
<connect gate="G$1" pin="B25" pad="P$55"/>
<connect gate="G$1" pin="B26" pad="P$56"/>
<connect gate="G$1" pin="B27" pad="P$57"/>
<connect gate="G$1" pin="B28" pad="P$58"/>
<connect gate="G$1" pin="B29" pad="P$59"/>
<connect gate="G$1" pin="B3" pad="P$33"/>
<connect gate="G$1" pin="B30" pad="P$60"/>
<connect gate="G$1" pin="B4" pad="P$34"/>
<connect gate="G$1" pin="B5" pad="P$35"/>
<connect gate="G$1" pin="B6" pad="P$36"/>
<connect gate="G$1" pin="B7" pad="P$37"/>
<connect gate="G$1" pin="B8" pad="P$38"/>
<connect gate="G$1" pin="B9" pad="P$39"/>
<connect gate="G$1" pin="C1" pad="P$61"/>
<connect gate="G$1" pin="C10" pad="P$70"/>
<connect gate="G$1" pin="C11" pad="P$71"/>
<connect gate="G$1" pin="C12" pad="P$72"/>
<connect gate="G$1" pin="C13" pad="P$73"/>
<connect gate="G$1" pin="C14" pad="P$74"/>
<connect gate="G$1" pin="C15" pad="P$75"/>
<connect gate="G$1" pin="C16" pad="P$76"/>
<connect gate="G$1" pin="C17" pad="P$77"/>
<connect gate="G$1" pin="C18" pad="P$78"/>
<connect gate="G$1" pin="C19" pad="P$79"/>
<connect gate="G$1" pin="C2" pad="P$62"/>
<connect gate="G$1" pin="C20" pad="P$80"/>
<connect gate="G$1" pin="C21" pad="P$81"/>
<connect gate="G$1" pin="C22" pad="P$82"/>
<connect gate="G$1" pin="C23" pad="P$83"/>
<connect gate="G$1" pin="C24" pad="P$84"/>
<connect gate="G$1" pin="C25" pad="P$85"/>
<connect gate="G$1" pin="C26" pad="P$86"/>
<connect gate="G$1" pin="C27" pad="P$87"/>
<connect gate="G$1" pin="C28" pad="P$88"/>
<connect gate="G$1" pin="C29" pad="P$89"/>
<connect gate="G$1" pin="C3" pad="P$63"/>
<connect gate="G$1" pin="C30" pad="P$90"/>
<connect gate="G$1" pin="C4" pad="P$64"/>
<connect gate="G$1" pin="C5" pad="P$65"/>
<connect gate="G$1" pin="C6" pad="P$66"/>
<connect gate="G$1" pin="C7" pad="P$67"/>
<connect gate="G$1" pin="C8" pad="P$68"/>
<connect gate="G$1" pin="C9" pad="P$69"/>
<connect gate="G$1" pin="D1" pad="P$91"/>
<connect gate="G$1" pin="D10" pad="P$100"/>
<connect gate="G$1" pin="D11" pad="P$101"/>
<connect gate="G$1" pin="D12" pad="P$102"/>
<connect gate="G$1" pin="D13" pad="P$103"/>
<connect gate="G$1" pin="D14" pad="P$104"/>
<connect gate="G$1" pin="D15" pad="P$105"/>
<connect gate="G$1" pin="D16" pad="P$106"/>
<connect gate="G$1" pin="D17" pad="P$107"/>
<connect gate="G$1" pin="D18" pad="P$108"/>
<connect gate="G$1" pin="D19" pad="P$109"/>
<connect gate="G$1" pin="D2" pad="P$92"/>
<connect gate="G$1" pin="D20" pad="P$110"/>
<connect gate="G$1" pin="D21" pad="P$111"/>
<connect gate="G$1" pin="D22" pad="P$112"/>
<connect gate="G$1" pin="D23" pad="P$113"/>
<connect gate="G$1" pin="D24" pad="P$114"/>
<connect gate="G$1" pin="D25" pad="P$115"/>
<connect gate="G$1" pin="D26" pad="P$116"/>
<connect gate="G$1" pin="D27" pad="P$117"/>
<connect gate="G$1" pin="D28" pad="P$118"/>
<connect gate="G$1" pin="D29" pad="P$119"/>
<connect gate="G$1" pin="D3" pad="P$93"/>
<connect gate="G$1" pin="D30" pad="P$120"/>
<connect gate="G$1" pin="D4" pad="P$94"/>
<connect gate="G$1" pin="D5" pad="P$95"/>
<connect gate="G$1" pin="D6" pad="P$96"/>
<connect gate="G$1" pin="D7" pad="P$97"/>
<connect gate="G$1" pin="D8" pad="P$98"/>
<connect gate="G$1" pin="D9" pad="P$99"/>
<connect gate="G$1" pin="E1" pad="P$121"/>
<connect gate="G$1" pin="E10" pad="P$130"/>
<connect gate="G$1" pin="E11" pad="P$131"/>
<connect gate="G$1" pin="E12" pad="P$132"/>
<connect gate="G$1" pin="E13" pad="P$133"/>
<connect gate="G$1" pin="E14" pad="P$134"/>
<connect gate="G$1" pin="E15" pad="P$135"/>
<connect gate="G$1" pin="E16" pad="P$136"/>
<connect gate="G$1" pin="E17" pad="P$137"/>
<connect gate="G$1" pin="E18" pad="P$138"/>
<connect gate="G$1" pin="E19" pad="P$139"/>
<connect gate="G$1" pin="E2" pad="P$122"/>
<connect gate="G$1" pin="E20" pad="P$140"/>
<connect gate="G$1" pin="E21" pad="P$141"/>
<connect gate="G$1" pin="E22" pad="P$142"/>
<connect gate="G$1" pin="E23" pad="P$143"/>
<connect gate="G$1" pin="E24" pad="P$144"/>
<connect gate="G$1" pin="E25" pad="P$145"/>
<connect gate="G$1" pin="E26" pad="P$146"/>
<connect gate="G$1" pin="E27" pad="P$147"/>
<connect gate="G$1" pin="E28" pad="P$148"/>
<connect gate="G$1" pin="E29" pad="P$149"/>
<connect gate="G$1" pin="E3" pad="P$123"/>
<connect gate="G$1" pin="E30" pad="P$150"/>
<connect gate="G$1" pin="E4" pad="P$124"/>
<connect gate="G$1" pin="E5" pad="P$125"/>
<connect gate="G$1" pin="E6" pad="P$126"/>
<connect gate="G$1" pin="E7" pad="P$127"/>
<connect gate="G$1" pin="E8" pad="P$128"/>
<connect gate="G$1" pin="E9" pad="P$129"/>
<connect gate="G$1" pin="F1" pad="P$151"/>
<connect gate="G$1" pin="F10" pad="P$160"/>
<connect gate="G$1" pin="F11" pad="P$161"/>
<connect gate="G$1" pin="F12" pad="P$162"/>
<connect gate="G$1" pin="F13" pad="P$163"/>
<connect gate="G$1" pin="F14" pad="P$164"/>
<connect gate="G$1" pin="F15" pad="P$165"/>
<connect gate="G$1" pin="F16" pad="P$166"/>
<connect gate="G$1" pin="F17" pad="P$167"/>
<connect gate="G$1" pin="F18" pad="P$168"/>
<connect gate="G$1" pin="F19" pad="P$169"/>
<connect gate="G$1" pin="F2" pad="P$152"/>
<connect gate="G$1" pin="F20" pad="P$170"/>
<connect gate="G$1" pin="F21" pad="P$171"/>
<connect gate="G$1" pin="F22" pad="P$172"/>
<connect gate="G$1" pin="F23" pad="P$173"/>
<connect gate="G$1" pin="F24" pad="P$174"/>
<connect gate="G$1" pin="F25" pad="P$175"/>
<connect gate="G$1" pin="F26" pad="P$176"/>
<connect gate="G$1" pin="F27" pad="P$177"/>
<connect gate="G$1" pin="F28" pad="P$178"/>
<connect gate="G$1" pin="F29" pad="P$179"/>
<connect gate="G$1" pin="F3" pad="P$153"/>
<connect gate="G$1" pin="F30" pad="P$180"/>
<connect gate="G$1" pin="F4" pad="P$154"/>
<connect gate="G$1" pin="F5" pad="P$155"/>
<connect gate="G$1" pin="F6" pad="P$156"/>
<connect gate="G$1" pin="F7" pad="P$157"/>
<connect gate="G$1" pin="F8" pad="P$158"/>
<connect gate="G$1" pin="F9" pad="P$159"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply2" urn="urn:adsk.eagle:library:372">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
Please keep in mind, that these devices are necessary for the
automatic wiring of the supply signals.&lt;p&gt;
The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND" urn="urn:adsk.eagle:symbol:26990/1" library_version="2">
<wire x1="-1.27" y1="0" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="0" y2="-1.27" width="0.254" layer="94"/>
<wire x1="0" y1="-1.27" x2="-1.27" y2="0" width="0.254" layer="94"/>
<text x="-1.905" y="-3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" urn="urn:adsk.eagle:component:27037/1" prefix="SUPPLY" library_version="2">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="GND" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="DS2431-Hemmick">
<packages>
<package name="SD2431">
<smd name="P$1" x="-1.524" y="1.27" dx="1.3208" dy="0.5334" layer="1"/>
<smd name="P$2" x="-1.524" y="0" dx="1.3208" dy="0.5334" layer="1"/>
<smd name="P$3" x="-1.524" y="-1.27" dx="1.3208" dy="0.5334" layer="1"/>
<smd name="P$4" x="1.524" y="-1.27" dx="1.3208" dy="0.5334" layer="1"/>
<smd name="P$5" x="1.524" y="0" dx="1.3208" dy="0.5334" layer="1"/>
<smd name="P$6" x="1.524" y="1.27" dx="1.3208" dy="0.5334" layer="1"/>
<wire x1="-2.54" y1="2.54" x2="2.54" y2="2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="2.54" x2="2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-2.54" x2="-2.54" y2="2.54" width="0.1524" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="DS2431">
<pin name="GND" x="0" y="0" length="middle" rot="R180"/>
<pin name="IO" x="0" y="2.54" length="middle" rot="R180"/>
<wire x1="-2.54" y1="5.08" x2="2.54" y2="5.08" width="0.1524" layer="94"/>
<wire x1="2.54" y1="5.08" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="-2.54" x2="-2.54" y2="5.08" width="0.1524" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="DS2431">
<gates>
<gate name="G$1" symbol="DS2431" x="0" y="-2.54"/>
</gates>
<devices>
<device name="" package="SD2431">
<connects>
<connect gate="G$1" pin="GND" pad="P$1"/>
<connect gate="G$1" pin="IO" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$2" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$3" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$4" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$5" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$6" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$7" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$8" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$9" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$10" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$11" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$12" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$13" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$14" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$15" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="U$16" library="SEAM-30-02.0-L-06-1-A-K-TR-offset-origin" deviceset="SEAM-30-02.0-L-06-1-A-K-TR" device=""/>
<part name="SUPPLY1" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY2" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY3" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY4" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY5" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY6" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY7" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY8" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY9" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY10" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY11" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY12" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY13" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY14" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY15" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY16" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY17" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY18" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY19" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY20" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY21" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY22" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY23" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY24" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY25" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY26" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY27" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY28" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY29" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY30" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY31" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY32" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY33" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY34" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY35" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY36" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY37" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY38" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY39" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY40" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY41" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY42" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY43" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY44" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY45" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY46" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY47" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY48" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY49" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY50" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY51" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY52" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY53" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY54" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY55" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY56" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY57" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY58" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY59" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY60" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY61" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY62" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY63" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY64" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY65" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY66" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY67" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY68" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY69" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY70" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY71" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY72" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY73" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY74" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY75" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY76" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY77" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY78" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY79" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY80" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY81" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY82" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY83" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY84" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY85" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY86" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY87" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY88" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY89" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY90" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY91" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY92" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY93" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY94" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY95" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="SUPPLY96" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="GND" device=""/>
<part name="U$17" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$18" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$19" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$20" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$21" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$22" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$23" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
<part name="U$24" library="DS2431-Hemmick" deviceset="DS2431" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="55.88" y="68.58" smashed="yes"/>
<instance part="U$2" gate="G$1" x="55.88" y="-20.32" smashed="yes"/>
<instance part="SUPPLY1" gate="GND" x="20.32" y="17.78" smashed="yes">
<attribute name="VALUE" x="18.415" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY2" gate="GND" x="45.72" y="17.78" smashed="yes">
<attribute name="VALUE" x="43.815" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY3" gate="GND" x="71.12" y="17.78" smashed="yes">
<attribute name="VALUE" x="69.215" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY4" gate="GND" x="96.52" y="17.78" smashed="yes">
<attribute name="VALUE" x="94.615" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY5" gate="GND" x="119.38" y="17.78" smashed="yes">
<attribute name="VALUE" x="117.475" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY6" gate="GND" x="147.32" y="17.78" smashed="yes">
<attribute name="VALUE" x="145.415" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY7" gate="GND" x="17.78" y="-71.12" smashed="yes">
<attribute name="VALUE" x="15.875" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY8" gate="GND" x="45.72" y="-71.12" smashed="yes">
<attribute name="VALUE" x="43.815" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY9" gate="GND" x="68.58" y="-71.12" smashed="yes">
<attribute name="VALUE" x="66.675" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY10" gate="GND" x="96.52" y="-71.12" smashed="yes">
<attribute name="VALUE" x="94.615" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY11" gate="GND" x="121.92" y="-71.12" smashed="yes">
<attribute name="VALUE" x="120.015" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY12" gate="GND" x="144.78" y="-71.12" smashed="yes">
<attribute name="VALUE" x="142.875" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="U$17" gate="G$1" x="35.56" y="15.24" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="A1"/>
<pinref part="SUPPLY1" gate="GND" pin="GND"/>
<wire x1="12.7" y1="96.52" x2="20.32" y2="96.52" width="0.1524" layer="91"/>
<wire x1="20.32" y1="96.52" x2="20.32" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="A29"/>
<wire x1="20.32" y1="93.98" x2="20.32" y2="91.44" width="0.1524" layer="91"/>
<wire x1="20.32" y1="91.44" x2="20.32" y2="88.9" width="0.1524" layer="91"/>
<wire x1="20.32" y1="88.9" x2="20.32" y2="86.36" width="0.1524" layer="91"/>
<wire x1="20.32" y1="86.36" x2="20.32" y2="81.28" width="0.1524" layer="91"/>
<wire x1="20.32" y1="81.28" x2="20.32" y2="78.74" width="0.1524" layer="91"/>
<wire x1="20.32" y1="78.74" x2="20.32" y2="76.2" width="0.1524" layer="91"/>
<wire x1="20.32" y1="76.2" x2="20.32" y2="73.66" width="0.1524" layer="91"/>
<wire x1="20.32" y1="73.66" x2="20.32" y2="71.12" width="0.1524" layer="91"/>
<wire x1="20.32" y1="71.12" x2="20.32" y2="68.58" width="0.1524" layer="91"/>
<wire x1="20.32" y1="68.58" x2="20.32" y2="66.04" width="0.1524" layer="91"/>
<wire x1="20.32" y1="66.04" x2="20.32" y2="63.5" width="0.1524" layer="91"/>
<wire x1="20.32" y1="63.5" x2="20.32" y2="60.96" width="0.1524" layer="91"/>
<wire x1="20.32" y1="60.96" x2="20.32" y2="58.42" width="0.1524" layer="91"/>
<wire x1="20.32" y1="58.42" x2="20.32" y2="55.88" width="0.1524" layer="91"/>
<wire x1="20.32" y1="55.88" x2="20.32" y2="53.34" width="0.1524" layer="91"/>
<wire x1="20.32" y1="53.34" x2="20.32" y2="50.8" width="0.1524" layer="91"/>
<wire x1="20.32" y1="50.8" x2="20.32" y2="48.26" width="0.1524" layer="91"/>
<wire x1="20.32" y1="48.26" x2="20.32" y2="45.72" width="0.1524" layer="91"/>
<wire x1="20.32" y1="45.72" x2="20.32" y2="43.18" width="0.1524" layer="91"/>
<wire x1="20.32" y1="43.18" x2="20.32" y2="40.64" width="0.1524" layer="91"/>
<wire x1="20.32" y1="40.64" x2="20.32" y2="38.1" width="0.1524" layer="91"/>
<wire x1="20.32" y1="38.1" x2="20.32" y2="35.56" width="0.1524" layer="91"/>
<wire x1="20.32" y1="35.56" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
<wire x1="20.32" y1="33.02" x2="20.32" y2="30.48" width="0.1524" layer="91"/>
<wire x1="20.32" y1="30.48" x2="20.32" y2="27.94" width="0.1524" layer="91"/>
<wire x1="20.32" y1="27.94" x2="20.32" y2="25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="25.4" x2="20.32" y2="20.32" width="0.1524" layer="91"/>
<wire x1="12.7" y1="25.4" x2="20.32" y2="25.4" width="0.1524" layer="91"/>
<junction x="20.32" y="25.4"/>
<pinref part="U$1" gate="G$1" pin="A28"/>
<wire x1="12.7" y1="27.94" x2="20.32" y2="27.94" width="0.1524" layer="91"/>
<junction x="20.32" y="27.94"/>
<pinref part="U$1" gate="G$1" pin="A27"/>
<wire x1="12.7" y1="30.48" x2="20.32" y2="30.48" width="0.1524" layer="91"/>
<junction x="20.32" y="30.48"/>
<pinref part="U$1" gate="G$1" pin="A26"/>
<wire x1="12.7" y1="33.02" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
<junction x="20.32" y="33.02"/>
<pinref part="U$1" gate="G$1" pin="A25"/>
<wire x1="12.7" y1="35.56" x2="20.32" y2="35.56" width="0.1524" layer="91"/>
<junction x="20.32" y="35.56"/>
<pinref part="U$1" gate="G$1" pin="A24"/>
<wire x1="12.7" y1="38.1" x2="20.32" y2="38.1" width="0.1524" layer="91"/>
<junction x="20.32" y="38.1"/>
<pinref part="U$1" gate="G$1" pin="A23"/>
<wire x1="12.7" y1="40.64" x2="20.32" y2="40.64" width="0.1524" layer="91"/>
<junction x="20.32" y="40.64"/>
<pinref part="U$1" gate="G$1" pin="A22"/>
<wire x1="12.7" y1="43.18" x2="20.32" y2="43.18" width="0.1524" layer="91"/>
<junction x="20.32" y="43.18"/>
<pinref part="U$1" gate="G$1" pin="A21"/>
<wire x1="12.7" y1="45.72" x2="20.32" y2="45.72" width="0.1524" layer="91"/>
<junction x="20.32" y="45.72"/>
<pinref part="U$1" gate="G$1" pin="A20"/>
<wire x1="12.7" y1="48.26" x2="20.32" y2="48.26" width="0.1524" layer="91"/>
<junction x="20.32" y="48.26"/>
<pinref part="U$1" gate="G$1" pin="A19"/>
<wire x1="12.7" y1="50.8" x2="20.32" y2="50.8" width="0.1524" layer="91"/>
<junction x="20.32" y="50.8"/>
<pinref part="U$1" gate="G$1" pin="A18"/>
<wire x1="12.7" y1="53.34" x2="20.32" y2="53.34" width="0.1524" layer="91"/>
<junction x="20.32" y="53.34"/>
<pinref part="U$1" gate="G$1" pin="A17"/>
<wire x1="12.7" y1="55.88" x2="20.32" y2="55.88" width="0.1524" layer="91"/>
<junction x="20.32" y="55.88"/>
<pinref part="U$1" gate="G$1" pin="A16"/>
<wire x1="12.7" y1="58.42" x2="20.32" y2="58.42" width="0.1524" layer="91"/>
<junction x="20.32" y="58.42"/>
<pinref part="U$1" gate="G$1" pin="A15"/>
<wire x1="12.7" y1="60.96" x2="20.32" y2="60.96" width="0.1524" layer="91"/>
<junction x="20.32" y="60.96"/>
<pinref part="U$1" gate="G$1" pin="A14"/>
<wire x1="12.7" y1="63.5" x2="20.32" y2="63.5" width="0.1524" layer="91"/>
<junction x="20.32" y="63.5"/>
<pinref part="U$1" gate="G$1" pin="A13"/>
<wire x1="12.7" y1="66.04" x2="20.32" y2="66.04" width="0.1524" layer="91"/>
<junction x="20.32" y="66.04"/>
<pinref part="U$1" gate="G$1" pin="A12"/>
<wire x1="12.7" y1="68.58" x2="20.32" y2="68.58" width="0.1524" layer="91"/>
<junction x="20.32" y="68.58"/>
<pinref part="U$1" gate="G$1" pin="A11"/>
<wire x1="12.7" y1="71.12" x2="20.32" y2="71.12" width="0.1524" layer="91"/>
<junction x="20.32" y="71.12"/>
<pinref part="U$1" gate="G$1" pin="A10"/>
<wire x1="12.7" y1="73.66" x2="20.32" y2="73.66" width="0.1524" layer="91"/>
<junction x="20.32" y="73.66"/>
<pinref part="U$1" gate="G$1" pin="A9"/>
<wire x1="12.7" y1="76.2" x2="20.32" y2="76.2" width="0.1524" layer="91"/>
<junction x="20.32" y="76.2"/>
<pinref part="U$1" gate="G$1" pin="A8"/>
<wire x1="12.7" y1="78.74" x2="20.32" y2="78.74" width="0.1524" layer="91"/>
<junction x="20.32" y="78.74"/>
<pinref part="U$1" gate="G$1" pin="A7"/>
<wire x1="12.7" y1="81.28" x2="20.32" y2="81.28" width="0.1524" layer="91"/>
<junction x="20.32" y="81.28"/>
<pinref part="U$1" gate="G$1" pin="A6"/>
<wire x1="12.7" y1="83.82" x2="20.32" y2="83.82" width="0.1524" layer="91"/>
<wire x1="20.32" y1="83.82" x2="20.32" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="A5"/>
<wire x1="12.7" y1="86.36" x2="20.32" y2="86.36" width="0.1524" layer="91"/>
<junction x="20.32" y="86.36"/>
<pinref part="U$1" gate="G$1" pin="A4"/>
<wire x1="12.7" y1="88.9" x2="20.32" y2="88.9" width="0.1524" layer="91"/>
<junction x="20.32" y="88.9"/>
<pinref part="U$1" gate="G$1" pin="A3"/>
<wire x1="12.7" y1="91.44" x2="20.32" y2="91.44" width="0.1524" layer="91"/>
<junction x="20.32" y="91.44"/>
<pinref part="U$1" gate="G$1" pin="A2"/>
<wire x1="12.7" y1="93.98" x2="20.32" y2="93.98" width="0.1524" layer="91"/>
<junction x="20.32" y="93.98"/>
<wire x1="20.32" y1="20.32" x2="27.94" y2="20.32" width="0.1524" layer="91"/>
<junction x="20.32" y="20.32"/>
<wire x1="27.94" y1="20.32" x2="27.94" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$17" gate="G$1" pin="GND"/>
<wire x1="27.94" y1="15.24" x2="35.56" y2="15.24" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="B1"/>
<pinref part="SUPPLY2" gate="GND" pin="GND"/>
<wire x1="38.1" y1="96.52" x2="45.72" y2="96.52" width="0.1524" layer="91"/>
<wire x1="45.72" y1="96.52" x2="45.72" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="B2"/>
<wire x1="45.72" y1="93.98" x2="45.72" y2="76.2" width="0.1524" layer="91"/>
<wire x1="45.72" y1="76.2" x2="45.72" y2="58.42" width="0.1524" layer="91"/>
<wire x1="45.72" y1="58.42" x2="45.72" y2="40.64" width="0.1524" layer="91"/>
<wire x1="45.72" y1="40.64" x2="45.72" y2="22.86" width="0.1524" layer="91"/>
<wire x1="45.72" y1="22.86" x2="45.72" y2="20.32" width="0.1524" layer="91"/>
<wire x1="38.1" y1="93.98" x2="45.72" y2="93.98" width="0.1524" layer="91"/>
<junction x="45.72" y="93.98"/>
<pinref part="U$1" gate="G$1" pin="B9"/>
<wire x1="38.1" y1="76.2" x2="45.72" y2="76.2" width="0.1524" layer="91"/>
<junction x="45.72" y="76.2"/>
<pinref part="U$1" gate="G$1" pin="B16"/>
<wire x1="38.1" y1="58.42" x2="45.72" y2="58.42" width="0.1524" layer="91"/>
<junction x="45.72" y="58.42"/>
<pinref part="U$1" gate="G$1" pin="B23"/>
<wire x1="38.1" y1="40.64" x2="45.72" y2="40.64" width="0.1524" layer="91"/>
<junction x="45.72" y="40.64"/>
<pinref part="U$1" gate="G$1" pin="B30"/>
<wire x1="38.1" y1="22.86" x2="45.72" y2="22.86" width="0.1524" layer="91"/>
<junction x="45.72" y="22.86"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="C1"/>
<pinref part="SUPPLY3" gate="GND" pin="GND"/>
<wire x1="63.5" y1="96.52" x2="71.12" y2="96.52" width="0.1524" layer="91"/>
<wire x1="71.12" y1="96.52" x2="71.12" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="C2"/>
<wire x1="71.12" y1="93.98" x2="71.12" y2="76.2" width="0.1524" layer="91"/>
<wire x1="71.12" y1="76.2" x2="71.12" y2="58.42" width="0.1524" layer="91"/>
<wire x1="71.12" y1="58.42" x2="71.12" y2="40.64" width="0.1524" layer="91"/>
<wire x1="71.12" y1="40.64" x2="71.12" y2="22.86" width="0.1524" layer="91"/>
<wire x1="71.12" y1="22.86" x2="71.12" y2="20.32" width="0.1524" layer="91"/>
<wire x1="63.5" y1="93.98" x2="71.12" y2="93.98" width="0.1524" layer="91"/>
<junction x="71.12" y="93.98"/>
<pinref part="U$1" gate="G$1" pin="C9"/>
<wire x1="63.5" y1="76.2" x2="71.12" y2="76.2" width="0.1524" layer="91"/>
<junction x="71.12" y="76.2"/>
<pinref part="U$1" gate="G$1" pin="C16"/>
<wire x1="63.5" y1="58.42" x2="71.12" y2="58.42" width="0.1524" layer="91"/>
<junction x="71.12" y="58.42"/>
<pinref part="U$1" gate="G$1" pin="C23"/>
<wire x1="63.5" y1="40.64" x2="71.12" y2="40.64" width="0.1524" layer="91"/>
<junction x="71.12" y="40.64"/>
<pinref part="U$1" gate="G$1" pin="C30"/>
<wire x1="63.5" y1="22.86" x2="71.12" y2="22.86" width="0.1524" layer="91"/>
<junction x="71.12" y="22.86"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="D1"/>
<pinref part="SUPPLY4" gate="GND" pin="GND"/>
<wire x1="88.9" y1="96.52" x2="96.52" y2="96.52" width="0.1524" layer="91"/>
<wire x1="96.52" y1="96.52" x2="96.52" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="D2"/>
<wire x1="96.52" y1="93.98" x2="96.52" y2="76.2" width="0.1524" layer="91"/>
<wire x1="96.52" y1="76.2" x2="96.52" y2="58.42" width="0.1524" layer="91"/>
<wire x1="96.52" y1="58.42" x2="96.52" y2="40.64" width="0.1524" layer="91"/>
<wire x1="96.52" y1="40.64" x2="96.52" y2="22.86" width="0.1524" layer="91"/>
<wire x1="96.52" y1="22.86" x2="96.52" y2="20.32" width="0.1524" layer="91"/>
<wire x1="88.9" y1="93.98" x2="96.52" y2="93.98" width="0.1524" layer="91"/>
<junction x="96.52" y="93.98"/>
<pinref part="U$1" gate="G$1" pin="D9"/>
<wire x1="88.9" y1="76.2" x2="96.52" y2="76.2" width="0.1524" layer="91"/>
<junction x="96.52" y="76.2"/>
<pinref part="U$1" gate="G$1" pin="D16"/>
<wire x1="88.9" y1="58.42" x2="96.52" y2="58.42" width="0.1524" layer="91"/>
<junction x="96.52" y="58.42"/>
<pinref part="U$1" gate="G$1" pin="D23"/>
<wire x1="88.9" y1="40.64" x2="96.52" y2="40.64" width="0.1524" layer="91"/>
<junction x="96.52" y="40.64"/>
<pinref part="U$1" gate="G$1" pin="D30"/>
<wire x1="88.9" y1="22.86" x2="96.52" y2="22.86" width="0.1524" layer="91"/>
<junction x="96.52" y="22.86"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="E1"/>
<wire x1="114.3" y1="96.52" x2="121.92" y2="96.52" width="0.1524" layer="91"/>
<wire x1="121.92" y1="96.52" x2="121.92" y2="22.86" width="0.1524" layer="91"/>
<pinref part="SUPPLY5" gate="GND" pin="GND"/>
<wire x1="121.92" y1="22.86" x2="121.92" y2="20.32" width="0.1524" layer="91"/>
<wire x1="121.92" y1="20.32" x2="119.38" y2="20.32" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="E30"/>
<wire x1="114.3" y1="22.86" x2="121.92" y2="22.86" width="0.1524" layer="91"/>
<junction x="121.92" y="22.86"/>
</segment>
<segment>
<pinref part="U$1" gate="G$1" pin="F1"/>
<pinref part="SUPPLY6" gate="GND" pin="GND"/>
<wire x1="139.7" y1="96.52" x2="147.32" y2="96.52" width="0.1524" layer="91"/>
<wire x1="147.32" y1="96.52" x2="147.32" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="F30"/>
<wire x1="147.32" y1="22.86" x2="147.32" y2="20.32" width="0.1524" layer="91"/>
<wire x1="139.7" y1="22.86" x2="147.32" y2="22.86" width="0.1524" layer="91"/>
<junction x="147.32" y="22.86"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="A1"/>
<wire x1="12.7" y1="7.62" x2="20.32" y2="7.62" width="0.1524" layer="91"/>
<wire x1="20.32" y1="7.62" x2="20.32" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY7" gate="GND" pin="GND"/>
<wire x1="20.32" y1="5.08" x2="20.32" y2="2.54" width="0.1524" layer="91"/>
<wire x1="20.32" y1="2.54" x2="20.32" y2="0" width="0.1524" layer="91"/>
<wire x1="20.32" y1="0" x2="20.32" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-2.54" x2="20.32" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-5.08" x2="20.32" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-7.62" x2="20.32" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-10.16" x2="20.32" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-12.7" x2="20.32" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-15.24" x2="20.32" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-17.78" x2="20.32" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-20.32" x2="20.32" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-22.86" x2="20.32" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-25.4" x2="20.32" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-27.94" x2="20.32" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-30.48" x2="20.32" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-33.02" x2="20.32" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-35.56" x2="20.32" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-38.1" x2="20.32" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-40.64" x2="20.32" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-43.18" x2="20.32" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-45.72" x2="20.32" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-48.26" x2="20.32" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-50.8" x2="20.32" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-53.34" x2="20.32" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-55.88" x2="20.32" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-58.42" x2="20.32" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-60.96" x2="20.32" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-63.5" x2="20.32" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-68.58" x2="17.78" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="A29"/>
<wire x1="12.7" y1="-63.5" x2="20.32" y2="-63.5" width="0.1524" layer="91"/>
<junction x="20.32" y="-63.5"/>
<pinref part="U$2" gate="G$1" pin="A28"/>
<wire x1="12.7" y1="-60.96" x2="20.32" y2="-60.96" width="0.1524" layer="91"/>
<junction x="20.32" y="-60.96"/>
<pinref part="U$2" gate="G$1" pin="A27"/>
<wire x1="12.7" y1="-58.42" x2="20.32" y2="-58.42" width="0.1524" layer="91"/>
<junction x="20.32" y="-58.42"/>
<pinref part="U$2" gate="G$1" pin="A26"/>
<wire x1="12.7" y1="-55.88" x2="20.32" y2="-55.88" width="0.1524" layer="91"/>
<junction x="20.32" y="-55.88"/>
<pinref part="U$2" gate="G$1" pin="A25"/>
<wire x1="12.7" y1="-53.34" x2="20.32" y2="-53.34" width="0.1524" layer="91"/>
<junction x="20.32" y="-53.34"/>
<pinref part="U$2" gate="G$1" pin="A24"/>
<wire x1="12.7" y1="-50.8" x2="20.32" y2="-50.8" width="0.1524" layer="91"/>
<junction x="20.32" y="-50.8"/>
<pinref part="U$2" gate="G$1" pin="A23"/>
<wire x1="12.7" y1="-48.26" x2="20.32" y2="-48.26" width="0.1524" layer="91"/>
<junction x="20.32" y="-48.26"/>
<pinref part="U$2" gate="G$1" pin="A22"/>
<wire x1="12.7" y1="-45.72" x2="20.32" y2="-45.72" width="0.1524" layer="91"/>
<junction x="20.32" y="-45.72"/>
<pinref part="U$2" gate="G$1" pin="A21"/>
<wire x1="12.7" y1="-43.18" x2="20.32" y2="-43.18" width="0.1524" layer="91"/>
<junction x="20.32" y="-43.18"/>
<pinref part="U$2" gate="G$1" pin="A20"/>
<wire x1="12.7" y1="-40.64" x2="20.32" y2="-40.64" width="0.1524" layer="91"/>
<junction x="20.32" y="-40.64"/>
<pinref part="U$2" gate="G$1" pin="A19"/>
<wire x1="12.7" y1="-38.1" x2="20.32" y2="-38.1" width="0.1524" layer="91"/>
<junction x="20.32" y="-38.1"/>
<pinref part="U$2" gate="G$1" pin="A18"/>
<wire x1="12.7" y1="-35.56" x2="20.32" y2="-35.56" width="0.1524" layer="91"/>
<junction x="20.32" y="-35.56"/>
<pinref part="U$2" gate="G$1" pin="A17"/>
<wire x1="12.7" y1="-33.02" x2="20.32" y2="-33.02" width="0.1524" layer="91"/>
<junction x="20.32" y="-33.02"/>
<pinref part="U$2" gate="G$1" pin="A16"/>
<wire x1="12.7" y1="-30.48" x2="20.32" y2="-30.48" width="0.1524" layer="91"/>
<junction x="20.32" y="-30.48"/>
<pinref part="U$2" gate="G$1" pin="A15"/>
<wire x1="12.7" y1="-27.94" x2="20.32" y2="-27.94" width="0.1524" layer="91"/>
<junction x="20.32" y="-27.94"/>
<pinref part="U$2" gate="G$1" pin="A14"/>
<wire x1="12.7" y1="-25.4" x2="20.32" y2="-25.4" width="0.1524" layer="91"/>
<junction x="20.32" y="-25.4"/>
<pinref part="U$2" gate="G$1" pin="A13"/>
<wire x1="12.7" y1="-22.86" x2="20.32" y2="-22.86" width="0.1524" layer="91"/>
<junction x="20.32" y="-22.86"/>
<pinref part="U$2" gate="G$1" pin="A12"/>
<wire x1="12.7" y1="-20.32" x2="20.32" y2="-20.32" width="0.1524" layer="91"/>
<junction x="20.32" y="-20.32"/>
<pinref part="U$2" gate="G$1" pin="A11"/>
<wire x1="12.7" y1="-17.78" x2="20.32" y2="-17.78" width="0.1524" layer="91"/>
<junction x="20.32" y="-17.78"/>
<pinref part="U$2" gate="G$1" pin="A10"/>
<wire x1="12.7" y1="-15.24" x2="20.32" y2="-15.24" width="0.1524" layer="91"/>
<junction x="20.32" y="-15.24"/>
<pinref part="U$2" gate="G$1" pin="A9"/>
<wire x1="12.7" y1="-12.7" x2="20.32" y2="-12.7" width="0.1524" layer="91"/>
<junction x="20.32" y="-12.7"/>
<pinref part="U$2" gate="G$1" pin="A8"/>
<wire x1="12.7" y1="-10.16" x2="20.32" y2="-10.16" width="0.1524" layer="91"/>
<junction x="20.32" y="-10.16"/>
<pinref part="U$2" gate="G$1" pin="A7"/>
<wire x1="12.7" y1="-7.62" x2="20.32" y2="-7.62" width="0.1524" layer="91"/>
<junction x="20.32" y="-7.62"/>
<pinref part="U$2" gate="G$1" pin="A6"/>
<wire x1="12.7" y1="-5.08" x2="20.32" y2="-5.08" width="0.1524" layer="91"/>
<junction x="20.32" y="-5.08"/>
<pinref part="U$2" gate="G$1" pin="A5"/>
<wire x1="12.7" y1="-2.54" x2="20.32" y2="-2.54" width="0.1524" layer="91"/>
<junction x="20.32" y="-2.54"/>
<pinref part="U$2" gate="G$1" pin="A4"/>
<wire x1="12.7" y1="0" x2="20.32" y2="0" width="0.1524" layer="91"/>
<junction x="20.32" y="0"/>
<pinref part="U$2" gate="G$1" pin="A3"/>
<wire x1="12.7" y1="2.54" x2="20.32" y2="2.54" width="0.1524" layer="91"/>
<junction x="20.32" y="2.54"/>
<pinref part="U$2" gate="G$1" pin="A2"/>
<wire x1="12.7" y1="5.08" x2="20.32" y2="5.08" width="0.1524" layer="91"/>
<junction x="20.32" y="5.08"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="B1"/>
<wire x1="38.1" y1="7.62" x2="45.72" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY8" gate="GND" pin="GND"/>
<wire x1="45.72" y1="7.62" x2="45.72" y2="5.08" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="B30"/>
<wire x1="45.72" y1="5.08" x2="45.72" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-12.7" x2="45.72" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-30.48" x2="45.72" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-48.26" x2="45.72" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-66.04" x2="45.72" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="38.1" y1="-66.04" x2="45.72" y2="-66.04" width="0.1524" layer="91"/>
<junction x="45.72" y="-66.04"/>
<pinref part="U$2" gate="G$1" pin="B23"/>
<wire x1="38.1" y1="-48.26" x2="45.72" y2="-48.26" width="0.1524" layer="91"/>
<junction x="45.72" y="-48.26"/>
<pinref part="U$2" gate="G$1" pin="B16"/>
<wire x1="38.1" y1="-30.48" x2="45.72" y2="-30.48" width="0.1524" layer="91"/>
<junction x="45.72" y="-30.48"/>
<pinref part="U$2" gate="G$1" pin="B9"/>
<wire x1="38.1" y1="-12.7" x2="45.72" y2="-12.7" width="0.1524" layer="91"/>
<junction x="45.72" y="-12.7"/>
<pinref part="U$2" gate="G$1" pin="B2"/>
<wire x1="38.1" y1="5.08" x2="45.72" y2="5.08" width="0.1524" layer="91"/>
<junction x="45.72" y="5.08"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="C1"/>
<wire x1="63.5" y1="7.62" x2="71.12" y2="7.62" width="0.1524" layer="91"/>
<wire x1="71.12" y1="7.62" x2="71.12" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY9" gate="GND" pin="GND"/>
<wire x1="71.12" y1="5.08" x2="71.12" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-12.7" x2="71.12" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-30.48" x2="71.12" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-48.26" x2="71.12" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-66.04" x2="71.12" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-68.58" x2="68.58" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="C30"/>
<wire x1="71.12" y1="-66.04" x2="63.5" y2="-66.04" width="0.1524" layer="91"/>
<junction x="71.12" y="-66.04"/>
<pinref part="U$2" gate="G$1" pin="C23"/>
<wire x1="63.5" y1="-48.26" x2="71.12" y2="-48.26" width="0.1524" layer="91"/>
<junction x="71.12" y="-48.26"/>
<pinref part="U$2" gate="G$1" pin="C16"/>
<wire x1="63.5" y1="-30.48" x2="71.12" y2="-30.48" width="0.1524" layer="91"/>
<junction x="71.12" y="-30.48"/>
<pinref part="U$2" gate="G$1" pin="C9"/>
<wire x1="63.5" y1="-12.7" x2="71.12" y2="-12.7" width="0.1524" layer="91"/>
<junction x="71.12" y="-12.7"/>
<pinref part="U$2" gate="G$1" pin="C2"/>
<wire x1="63.5" y1="5.08" x2="71.12" y2="5.08" width="0.1524" layer="91"/>
<junction x="71.12" y="5.08"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="D1"/>
<wire x1="88.9" y1="7.62" x2="96.52" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY10" gate="GND" pin="GND"/>
<wire x1="96.52" y1="7.62" x2="96.52" y2="5.08" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="D30"/>
<wire x1="96.52" y1="5.08" x2="96.52" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-12.7" x2="96.52" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-30.48" x2="96.52" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-48.26" x2="96.52" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-66.04" x2="96.52" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="88.9" y1="-66.04" x2="96.52" y2="-66.04" width="0.1524" layer="91"/>
<junction x="96.52" y="-66.04"/>
<pinref part="U$2" gate="G$1" pin="D23"/>
<wire x1="88.9" y1="-48.26" x2="96.52" y2="-48.26" width="0.1524" layer="91"/>
<junction x="96.52" y="-48.26"/>
<pinref part="U$2" gate="G$1" pin="D16"/>
<wire x1="88.9" y1="-30.48" x2="96.52" y2="-30.48" width="0.1524" layer="91"/>
<junction x="96.52" y="-30.48"/>
<pinref part="U$2" gate="G$1" pin="D9"/>
<wire x1="88.9" y1="-12.7" x2="96.52" y2="-12.7" width="0.1524" layer="91"/>
<junction x="96.52" y="-12.7"/>
<pinref part="U$2" gate="G$1" pin="D2"/>
<wire x1="88.9" y1="5.08" x2="96.52" y2="5.08" width="0.1524" layer="91"/>
<junction x="96.52" y="5.08"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="E1"/>
<wire x1="114.3" y1="7.62" x2="121.92" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY11" gate="GND" pin="GND"/>
<wire x1="121.92" y1="7.62" x2="121.92" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="E30"/>
<wire x1="121.92" y1="-66.04" x2="121.92" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="114.3" y1="-66.04" x2="121.92" y2="-66.04" width="0.1524" layer="91"/>
<junction x="121.92" y="-66.04"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="F1"/>
<wire x1="139.7" y1="7.62" x2="147.32" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY12" gate="GND" pin="GND"/>
<wire x1="147.32" y1="7.62" x2="147.32" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="147.32" y1="-66.04" x2="147.32" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="147.32" y1="-68.58" x2="144.78" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="F30"/>
<wire x1="139.7" y1="-66.04" x2="147.32" y2="-66.04" width="0.1524" layer="91"/>
<junction x="147.32" y="-66.04"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="A30"/>
<wire x1="12.7" y1="22.86" x2="30.48" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$17" gate="G$1" pin="IO"/>
<wire x1="30.48" y1="22.86" x2="30.48" y2="17.78" width="0.1524" layer="91"/>
<wire x1="30.48" y1="17.78" x2="35.56" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.09.012" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B3"/>
<wire x1="38.1" y1="91.44" x2="48.26" y2="91.44" width="0.1016" layer="91"/>
<label x="48.26" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.012" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B4"/>
<wire x1="38.1" y1="88.9" x2="48.26" y2="88.9" width="0.1016" layer="91"/>
<label x="48.26" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B5"/>
<wire x1="38.1" y1="86.36" x2="48.26" y2="86.36" width="0.1016" layer="91"/>
<label x="48.26" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B6"/>
<wire x1="38.1" y1="83.82" x2="48.26" y2="83.82" width="0.1016" layer="91"/>
<label x="48.26" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B7"/>
<wire x1="38.1" y1="81.28" x2="48.26" y2="81.28" width="0.1016" layer="91"/>
<label x="48.26" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B8"/>
<wire x1="38.1" y1="78.74" x2="48.26" y2="78.74" width="0.1016" layer="91"/>
<label x="48.26" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B10"/>
<wire x1="38.1" y1="73.66" x2="48.26" y2="73.66" width="0.1016" layer="91"/>
<label x="48.26" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B11"/>
<wire x1="38.1" y1="71.12" x2="48.26" y2="71.12" width="0.1016" layer="91"/>
<label x="48.26" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B12"/>
<wire x1="38.1" y1="68.58" x2="48.26" y2="68.58" width="0.1016" layer="91"/>
<label x="48.26" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B13"/>
<wire x1="38.1" y1="66.04" x2="48.26" y2="66.04" width="0.1016" layer="91"/>
<label x="48.26" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B14"/>
<wire x1="38.1" y1="63.5" x2="48.26" y2="63.5" width="0.1016" layer="91"/>
<label x="48.26" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.011" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B15"/>
<wire x1="38.1" y1="60.96" x2="48.26" y2="60.96" width="0.1016" layer="91"/>
<label x="48.26" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B17"/>
<wire x1="38.1" y1="55.88" x2="48.26" y2="55.88" width="0.1016" layer="91"/>
<label x="48.26" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B18"/>
<wire x1="38.1" y1="53.34" x2="48.26" y2="53.34" width="0.1016" layer="91"/>
<label x="48.26" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B19"/>
<wire x1="38.1" y1="50.8" x2="48.26" y2="50.8" width="0.1016" layer="91"/>
<label x="48.26" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B20"/>
<wire x1="38.1" y1="48.26" x2="48.26" y2="48.26" width="0.1016" layer="91"/>
<label x="48.26" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B21"/>
<wire x1="38.1" y1="45.72" x2="48.26" y2="45.72" width="0.1016" layer="91"/>
<label x="48.26" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B22"/>
<wire x1="38.1" y1="43.18" x2="48.26" y2="43.18" width="0.1016" layer="91"/>
<label x="48.26" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B24"/>
<wire x1="38.1" y1="38.1" x2="48.26" y2="38.1" width="0.1016" layer="91"/>
<label x="48.26" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B25"/>
<wire x1="38.1" y1="35.56" x2="48.26" y2="35.56" width="0.1016" layer="91"/>
<label x="48.26" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B26"/>
<wire x1="38.1" y1="33.02" x2="48.26" y2="33.02" width="0.1016" layer="91"/>
<label x="48.26" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B27"/>
<wire x1="38.1" y1="30.48" x2="48.26" y2="30.48" width="0.1016" layer="91"/>
<label x="48.26" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B28"/>
<wire x1="38.1" y1="27.94" x2="48.26" y2="27.94" width="0.1016" layer="91"/>
<label x="48.26" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="B29"/>
<wire x1="38.1" y1="25.4" x2="48.26" y2="25.4" width="0.1016" layer="91"/>
<label x="48.26" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.013" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C3"/>
<wire x1="63.5" y1="91.44" x2="73.66" y2="91.44" width="0.1016" layer="91"/>
<label x="73.66" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.013" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C4"/>
<wire x1="63.5" y1="88.9" x2="73.66" y2="88.9" width="0.1016" layer="91"/>
<label x="73.66" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C5"/>
<wire x1="63.5" y1="86.36" x2="73.66" y2="86.36" width="0.1016" layer="91"/>
<label x="73.66" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C6"/>
<wire x1="63.5" y1="83.82" x2="73.66" y2="83.82" width="0.1016" layer="91"/>
<label x="73.66" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C7"/>
<wire x1="63.5" y1="81.28" x2="73.66" y2="81.28" width="0.1016" layer="91"/>
<label x="73.66" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C8"/>
<wire x1="63.5" y1="78.74" x2="73.66" y2="78.74" width="0.1016" layer="91"/>
<label x="73.66" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.011" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C10"/>
<wire x1="63.5" y1="73.66" x2="73.66" y2="73.66" width="0.1016" layer="91"/>
<label x="73.66" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C11"/>
<wire x1="63.5" y1="71.12" x2="73.66" y2="71.12" width="0.1016" layer="91"/>
<label x="73.66" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C12"/>
<wire x1="63.5" y1="68.58" x2="73.66" y2="68.58" width="0.1016" layer="91"/>
<label x="73.66" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C13"/>
<wire x1="63.5" y1="66.04" x2="73.66" y2="66.04" width="0.1016" layer="91"/>
<label x="73.66" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C14"/>
<wire x1="63.5" y1="63.5" x2="73.66" y2="63.5" width="0.1016" layer="91"/>
<label x="73.66" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.012" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C15"/>
<wire x1="63.5" y1="60.96" x2="73.66" y2="60.96" width="0.1016" layer="91"/>
<label x="73.66" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C17"/>
<wire x1="63.5" y1="55.88" x2="73.66" y2="55.88" width="0.1016" layer="91"/>
<label x="73.66" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C18"/>
<wire x1="63.5" y1="53.34" x2="73.66" y2="53.34" width="0.1016" layer="91"/>
<label x="73.66" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C19"/>
<wire x1="63.5" y1="50.8" x2="73.66" y2="50.8" width="0.1016" layer="91"/>
<label x="73.66" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C20"/>
<wire x1="63.5" y1="48.26" x2="73.66" y2="48.26" width="0.1016" layer="91"/>
<label x="73.66" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C21"/>
<wire x1="63.5" y1="45.72" x2="73.66" y2="45.72" width="0.1016" layer="91"/>
<label x="73.66" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C22"/>
<wire x1="63.5" y1="43.18" x2="73.66" y2="43.18" width="0.1016" layer="91"/>
<label x="73.66" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.011" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C24"/>
<wire x1="63.5" y1="38.1" x2="73.66" y2="38.1" width="0.1016" layer="91"/>
<label x="73.66" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C25"/>
<wire x1="63.5" y1="35.56" x2="73.66" y2="35.56" width="0.1016" layer="91"/>
<label x="73.66" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C26"/>
<wire x1="63.5" y1="33.02" x2="73.66" y2="33.02" width="0.1016" layer="91"/>
<label x="73.66" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C27"/>
<wire x1="63.5" y1="30.48" x2="73.66" y2="30.48" width="0.1016" layer="91"/>
<label x="73.66" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C28"/>
<wire x1="63.5" y1="27.94" x2="73.66" y2="27.94" width="0.1016" layer="91"/>
<label x="73.66" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.011" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="C29"/>
<wire x1="63.5" y1="25.4" x2="73.66" y2="25.4" width="0.1016" layer="91"/>
<label x="73.66" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D3"/>
<wire x1="88.9" y1="91.44" x2="99.06" y2="91.44" width="0.1016" layer="91"/>
<label x="99.06" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D4"/>
<wire x1="88.9" y1="88.9" x2="99.06" y2="88.9" width="0.1016" layer="91"/>
<label x="99.06" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D5"/>
<wire x1="88.9" y1="86.36" x2="99.06" y2="86.36" width="0.1016" layer="91"/>
<label x="99.06" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D6"/>
<wire x1="88.9" y1="83.82" x2="99.06" y2="83.82" width="0.1016" layer="91"/>
<label x="99.06" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D7"/>
<wire x1="88.9" y1="81.28" x2="99.06" y2="81.28" width="0.1016" layer="91"/>
<label x="99.06" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D8"/>
<wire x1="88.9" y1="78.74" x2="99.06" y2="78.74" width="0.1016" layer="91"/>
<label x="99.06" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.012" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D10"/>
<wire x1="88.9" y1="73.66" x2="99.06" y2="73.66" width="0.1016" layer="91"/>
<label x="99.06" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D11"/>
<wire x1="88.9" y1="71.12" x2="99.06" y2="71.12" width="0.1016" layer="91"/>
<label x="99.06" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D12"/>
<wire x1="88.9" y1="68.58" x2="99.06" y2="68.58" width="0.1016" layer="91"/>
<label x="99.06" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D13"/>
<wire x1="88.9" y1="66.04" x2="99.06" y2="66.04" width="0.1016" layer="91"/>
<label x="99.06" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D14"/>
<wire x1="88.9" y1="63.5" x2="99.06" y2="63.5" width="0.1016" layer="91"/>
<label x="99.06" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.013" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D15"/>
<wire x1="88.9" y1="60.96" x2="99.06" y2="60.96" width="0.1016" layer="91"/>
<label x="99.06" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.000" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D17"/>
<wire x1="88.9" y1="55.88" x2="99.06" y2="55.88" width="0.1016" layer="91"/>
<label x="99.06" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D18"/>
<wire x1="88.9" y1="53.34" x2="99.06" y2="53.34" width="0.1016" layer="91"/>
<label x="99.06" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D19"/>
<wire x1="88.9" y1="50.8" x2="99.06" y2="50.8" width="0.1016" layer="91"/>
<label x="99.06" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D20"/>
<wire x1="88.9" y1="48.26" x2="99.06" y2="48.26" width="0.1016" layer="91"/>
<label x="99.06" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D21"/>
<wire x1="88.9" y1="45.72" x2="99.06" y2="45.72" width="0.1016" layer="91"/>
<label x="99.06" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D22"/>
<wire x1="88.9" y1="43.18" x2="99.06" y2="43.18" width="0.1016" layer="91"/>
<label x="99.06" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.012" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D24"/>
<wire x1="88.9" y1="38.1" x2="99.06" y2="38.1" width="0.1016" layer="91"/>
<label x="99.06" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D25"/>
<wire x1="88.9" y1="35.56" x2="99.06" y2="35.56" width="0.1016" layer="91"/>
<label x="99.06" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D26"/>
<wire x1="88.9" y1="33.02" x2="99.06" y2="33.02" width="0.1016" layer="91"/>
<label x="99.06" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D27"/>
<wire x1="88.9" y1="30.48" x2="99.06" y2="30.48" width="0.1016" layer="91"/>
<label x="99.06" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D28"/>
<wire x1="88.9" y1="27.94" x2="99.06" y2="27.94" width="0.1016" layer="91"/>
<label x="99.06" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.012" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D29"/>
<wire x1="88.9" y1="25.4" x2="99.06" y2="25.4" width="0.1016" layer="91"/>
<label x="99.06" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E3"/>
<wire x1="114.3" y1="91.44" x2="124.46" y2="91.44" width="0.1016" layer="91"/>
<label x="124.46" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E4"/>
<wire x1="114.3" y1="88.9" x2="124.46" y2="88.9" width="0.1016" layer="91"/>
<label x="124.46" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E5"/>
<wire x1="114.3" y1="86.36" x2="124.46" y2="86.36" width="0.1016" layer="91"/>
<label x="124.46" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.006" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E6"/>
<wire x1="114.3" y1="83.82" x2="124.46" y2="83.82" width="0.1016" layer="91"/>
<label x="124.46" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E7"/>
<wire x1="114.3" y1="81.28" x2="124.46" y2="81.28" width="0.1016" layer="91"/>
<label x="124.46" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.001" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E8"/>
<wire x1="114.3" y1="78.74" x2="124.46" y2="78.74" width="0.1016" layer="91"/>
<label x="124.46" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.013" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E10"/>
<wire x1="114.3" y1="73.66" x2="124.46" y2="73.66" width="0.1016" layer="91"/>
<label x="124.46" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E11"/>
<wire x1="114.3" y1="71.12" x2="124.46" y2="71.12" width="0.1016" layer="91"/>
<label x="124.46" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E12"/>
<wire x1="114.3" y1="68.58" x2="124.46" y2="68.58" width="0.1016" layer="91"/>
<label x="124.46" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E13"/>
<wire x1="114.3" y1="66.04" x2="124.46" y2="66.04" width="0.1016" layer="91"/>
<label x="124.46" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E14"/>
<wire x1="114.3" y1="63.5" x2="124.46" y2="63.5" width="0.1016" layer="91"/>
<label x="124.46" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E15"/>
<wire x1="114.3" y1="60.96" x2="124.46" y2="60.96" width="0.1016" layer="91"/>
<label x="124.46" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E17"/>
<wire x1="114.3" y1="55.88" x2="124.46" y2="55.88" width="0.1016" layer="91"/>
<label x="124.46" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E18"/>
<wire x1="114.3" y1="53.34" x2="124.46" y2="53.34" width="0.1016" layer="91"/>
<label x="124.46" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E19"/>
<wire x1="114.3" y1="50.8" x2="124.46" y2="50.8" width="0.1016" layer="91"/>
<label x="124.46" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.019" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E20"/>
<wire x1="114.3" y1="48.26" x2="124.46" y2="48.26" width="0.1016" layer="91"/>
<label x="124.46" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E21"/>
<wire x1="114.3" y1="45.72" x2="124.46" y2="45.72" width="0.1016" layer="91"/>
<label x="124.46" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E22"/>
<wire x1="114.3" y1="43.18" x2="124.46" y2="43.18" width="0.1016" layer="91"/>
<label x="124.46" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E24"/>
<wire x1="114.3" y1="38.1" x2="124.46" y2="38.1" width="0.1016" layer="91"/>
<label x="124.46" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E25"/>
<wire x1="114.3" y1="35.56" x2="124.46" y2="35.56" width="0.1016" layer="91"/>
<label x="124.46" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E26"/>
<wire x1="114.3" y1="33.02" x2="124.46" y2="33.02" width="0.1016" layer="91"/>
<label x="124.46" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.013" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E27"/>
<wire x1="114.3" y1="30.48" x2="124.46" y2="30.48" width="0.1016" layer="91"/>
<label x="124.46" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E28"/>
<wire x1="114.3" y1="27.94" x2="124.46" y2="27.94" width="0.1016" layer="91"/>
<label x="124.46" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E29"/>
<wire x1="114.3" y1="25.4" x2="124.46" y2="25.4" width="0.1016" layer="91"/>
<label x="124.46" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.011" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F3"/>
<wire x1="139.7" y1="91.44" x2="149.86" y2="91.44" width="0.1016" layer="91"/>
<label x="149.86" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F4"/>
<wire x1="139.7" y1="88.9" x2="149.86" y2="88.9" width="0.1016" layer="91"/>
<label x="149.86" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F5"/>
<wire x1="139.7" y1="86.36" x2="149.86" y2="86.36" width="0.1016" layer="91"/>
<label x="149.86" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F6"/>
<wire x1="139.7" y1="83.82" x2="149.86" y2="83.82" width="0.1016" layer="91"/>
<label x="149.86" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F7"/>
<wire x1="139.7" y1="81.28" x2="149.86" y2="81.28" width="0.1016" layer="91"/>
<label x="149.86" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.002" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F8"/>
<wire x1="139.7" y1="78.74" x2="149.86" y2="78.74" width="0.1016" layer="91"/>
<label x="149.86" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F10"/>
<wire x1="139.7" y1="73.66" x2="149.86" y2="73.66" width="0.1016" layer="91"/>
<label x="149.86" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F11"/>
<wire x1="139.7" y1="71.12" x2="149.86" y2="71.12" width="0.1016" layer="91"/>
<label x="149.86" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F12"/>
<wire x1="139.7" y1="68.58" x2="149.86" y2="68.58" width="0.1016" layer="91"/>
<label x="149.86" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F13"/>
<wire x1="139.7" y1="66.04" x2="149.86" y2="66.04" width="0.1016" layer="91"/>
<label x="149.86" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F14"/>
<wire x1="139.7" y1="63.5" x2="149.86" y2="63.5" width="0.1016" layer="91"/>
<label x="149.86" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F15"/>
<wire x1="139.7" y1="60.96" x2="149.86" y2="60.96" width="0.1016" layer="91"/>
<label x="149.86" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.021" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F17"/>
<wire x1="139.7" y1="55.88" x2="149.86" y2="55.88" width="0.1016" layer="91"/>
<label x="149.86" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.010" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F18"/>
<wire x1="139.7" y1="53.34" x2="149.86" y2="53.34" width="0.1016" layer="91"/>
<label x="149.86" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.020" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F19"/>
<wire x1="139.7" y1="50.8" x2="149.86" y2="50.8" width="0.1016" layer="91"/>
<label x="149.86" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F20"/>
<wire x1="139.7" y1="48.26" x2="149.86" y2="48.26" width="0.1016" layer="91"/>
<label x="149.86" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.009" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F21"/>
<wire x1="139.7" y1="45.72" x2="149.86" y2="45.72" width="0.1016" layer="91"/>
<label x="149.86" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.019" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F22"/>
<wire x1="139.7" y1="43.18" x2="149.86" y2="43.18" width="0.1016" layer="91"/>
<label x="149.86" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F24"/>
<wire x1="139.7" y1="38.1" x2="149.86" y2="38.1" width="0.1016" layer="91"/>
<label x="149.86" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.004" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F25"/>
<wire x1="139.7" y1="35.56" x2="149.86" y2="35.56" width="0.1016" layer="91"/>
<label x="149.86" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F26"/>
<wire x1="139.7" y1="33.02" x2="149.86" y2="33.02" width="0.1016" layer="91"/>
<label x="149.86" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.008" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F27"/>
<wire x1="139.7" y1="30.48" x2="149.86" y2="30.48" width="0.1016" layer="91"/>
<label x="149.86" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.003" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F28"/>
<wire x1="139.7" y1="27.94" x2="149.86" y2="27.94" width="0.1016" layer="91"/>
<label x="149.86" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.013" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F29"/>
<wire x1="139.7" y1="25.4" x2="149.86" y2="25.4" width="0.1016" layer="91"/>
<label x="149.86" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.011" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E2"/>
<wire x1="114.3" y1="93.98" x2="124.46" y2="93.98" width="0.1016" layer="91"/>
<label x="124.46" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.007" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F2"/>
<wire x1="139.7" y1="93.98" x2="149.86" y2="93.98" width="0.1016" layer="91"/>
<label x="149.86" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E9"/>
<wire x1="114.3" y1="76.2" x2="124.46" y2="76.2" width="0.1016" layer="91"/>
<label x="124.46" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.005" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F9"/>
<wire x1="139.7" y1="76.2" x2="149.86" y2="76.2" width="0.1016" layer="91"/>
<label x="149.86" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.021" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E16"/>
<wire x1="114.3" y1="58.42" x2="124.46" y2="58.42" width="0.1016" layer="91"/>
<label x="124.46" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.020" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F16"/>
<wire x1="139.7" y1="58.42" x2="149.86" y2="58.42" width="0.1016" layer="91"/>
<label x="149.86" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.015" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="E23"/>
<wire x1="114.3" y1="40.64" x2="124.46" y2="40.64" width="0.1016" layer="91"/>
<label x="124.46" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.014" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="F23"/>
<wire x1="139.7" y1="40.64" x2="149.86" y2="40.64" width="0.1016" layer="91"/>
<label x="149.86" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B3"/>
<wire x1="38.1" y1="2.54" x2="48.26" y2="2.54" width="0.1016" layer="91"/>
<label x="48.26" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B4"/>
<wire x1="38.1" y1="0" x2="48.26" y2="0" width="0.1016" layer="91"/>
<label x="48.26" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B5"/>
<wire x1="38.1" y1="-2.54" x2="48.26" y2="-2.54" width="0.1016" layer="91"/>
<label x="48.26" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B6"/>
<wire x1="38.1" y1="-5.08" x2="48.26" y2="-5.08" width="0.1016" layer="91"/>
<label x="48.26" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B7"/>
<wire x1="38.1" y1="-7.62" x2="48.26" y2="-7.62" width="0.1016" layer="91"/>
<label x="48.26" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B8"/>
<wire x1="38.1" y1="-10.16" x2="48.26" y2="-10.16" width="0.1016" layer="91"/>
<label x="48.26" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B10"/>
<wire x1="38.1" y1="-15.24" x2="48.26" y2="-15.24" width="0.1016" layer="91"/>
<label x="48.26" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B11"/>
<wire x1="38.1" y1="-17.78" x2="48.26" y2="-17.78" width="0.1016" layer="91"/>
<label x="48.26" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B12"/>
<wire x1="38.1" y1="-20.32" x2="48.26" y2="-20.32" width="0.1016" layer="91"/>
<label x="48.26" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B13"/>
<wire x1="38.1" y1="-22.86" x2="48.26" y2="-22.86" width="0.1016" layer="91"/>
<label x="48.26" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B14"/>
<wire x1="38.1" y1="-25.4" x2="48.26" y2="-25.4" width="0.1016" layer="91"/>
<label x="48.26" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B15"/>
<wire x1="38.1" y1="-27.94" x2="48.26" y2="-27.94" width="0.1016" layer="91"/>
<label x="48.26" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B17"/>
<wire x1="38.1" y1="-33.02" x2="48.26" y2="-33.02" width="0.1016" layer="91"/>
<label x="48.26" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B18"/>
<wire x1="38.1" y1="-35.56" x2="48.26" y2="-35.56" width="0.1016" layer="91"/>
<label x="48.26" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B19"/>
<wire x1="38.1" y1="-38.1" x2="48.26" y2="-38.1" width="0.1016" layer="91"/>
<label x="48.26" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B20"/>
<wire x1="38.1" y1="-40.64" x2="48.26" y2="-40.64" width="0.1016" layer="91"/>
<label x="48.26" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B21"/>
<wire x1="38.1" y1="-43.18" x2="48.26" y2="-43.18" width="0.1016" layer="91"/>
<label x="48.26" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B22"/>
<wire x1="38.1" y1="-45.72" x2="48.26" y2="-45.72" width="0.1016" layer="91"/>
<label x="48.26" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B24"/>
<wire x1="38.1" y1="-50.8" x2="48.26" y2="-50.8" width="0.1016" layer="91"/>
<label x="48.26" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B25"/>
<wire x1="38.1" y1="-53.34" x2="48.26" y2="-53.34" width="0.1016" layer="91"/>
<label x="48.26" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B26"/>
<wire x1="38.1" y1="-55.88" x2="48.26" y2="-55.88" width="0.1016" layer="91"/>
<label x="48.26" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B27"/>
<wire x1="38.1" y1="-58.42" x2="48.26" y2="-58.42" width="0.1016" layer="91"/>
<label x="48.26" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B28"/>
<wire x1="38.1" y1="-60.96" x2="48.26" y2="-60.96" width="0.1016" layer="91"/>
<label x="48.26" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="B29"/>
<wire x1="38.1" y1="-63.5" x2="48.26" y2="-63.5" width="0.1016" layer="91"/>
<label x="48.26" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C3"/>
<wire x1="63.5" y1="2.54" x2="73.66" y2="2.54" width="0.1016" layer="91"/>
<label x="73.66" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C4"/>
<wire x1="63.5" y1="0" x2="73.66" y2="0" width="0.1016" layer="91"/>
<label x="73.66" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C5"/>
<wire x1="63.5" y1="-2.54" x2="73.66" y2="-2.54" width="0.1016" layer="91"/>
<label x="73.66" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C6"/>
<wire x1="63.5" y1="-5.08" x2="73.66" y2="-5.08" width="0.1016" layer="91"/>
<label x="73.66" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C7"/>
<wire x1="63.5" y1="-7.62" x2="73.66" y2="-7.62" width="0.1016" layer="91"/>
<label x="73.66" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C8"/>
<wire x1="63.5" y1="-10.16" x2="73.66" y2="-10.16" width="0.1016" layer="91"/>
<label x="73.66" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C10"/>
<wire x1="63.5" y1="-15.24" x2="73.66" y2="-15.24" width="0.1016" layer="91"/>
<label x="73.66" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C11"/>
<wire x1="63.5" y1="-17.78" x2="73.66" y2="-17.78" width="0.1016" layer="91"/>
<label x="73.66" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C12"/>
<wire x1="63.5" y1="-20.32" x2="73.66" y2="-20.32" width="0.1016" layer="91"/>
<label x="73.66" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C13"/>
<wire x1="63.5" y1="-22.86" x2="73.66" y2="-22.86" width="0.1016" layer="91"/>
<label x="73.66" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C14"/>
<wire x1="63.5" y1="-25.4" x2="73.66" y2="-25.4" width="0.1016" layer="91"/>
<label x="73.66" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.000" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C15"/>
<wire x1="63.5" y1="-27.94" x2="73.66" y2="-27.94" width="0.1016" layer="91"/>
<label x="73.66" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C17"/>
<wire x1="63.5" y1="-33.02" x2="73.66" y2="-33.02" width="0.1016" layer="91"/>
<label x="73.66" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C18"/>
<wire x1="63.5" y1="-35.56" x2="73.66" y2="-35.56" width="0.1016" layer="91"/>
<label x="73.66" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C19"/>
<wire x1="63.5" y1="-38.1" x2="73.66" y2="-38.1" width="0.1016" layer="91"/>
<label x="73.66" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C20"/>
<wire x1="63.5" y1="-40.64" x2="73.66" y2="-40.64" width="0.1016" layer="91"/>
<label x="73.66" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C21"/>
<wire x1="63.5" y1="-43.18" x2="73.66" y2="-43.18" width="0.1016" layer="91"/>
<label x="73.66" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C22"/>
<wire x1="63.5" y1="-45.72" x2="73.66" y2="-45.72" width="0.1016" layer="91"/>
<label x="73.66" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C24"/>
<wire x1="63.5" y1="-50.8" x2="73.66" y2="-50.8" width="0.1016" layer="91"/>
<label x="73.66" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C25"/>
<wire x1="63.5" y1="-53.34" x2="73.66" y2="-53.34" width="0.1016" layer="91"/>
<label x="73.66" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C26"/>
<wire x1="63.5" y1="-55.88" x2="73.66" y2="-55.88" width="0.1016" layer="91"/>
<label x="73.66" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C27"/>
<wire x1="63.5" y1="-58.42" x2="73.66" y2="-58.42" width="0.1016" layer="91"/>
<label x="73.66" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C28"/>
<wire x1="63.5" y1="-60.96" x2="73.66" y2="-60.96" width="0.1016" layer="91"/>
<label x="73.66" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="C29"/>
<wire x1="63.5" y1="-63.5" x2="73.66" y2="-63.5" width="0.1016" layer="91"/>
<label x="73.66" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D3"/>
<wire x1="88.9" y1="2.54" x2="99.06" y2="2.54" width="0.1016" layer="91"/>
<label x="99.06" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D4"/>
<wire x1="88.9" y1="0" x2="99.06" y2="0" width="0.1016" layer="91"/>
<label x="99.06" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D5"/>
<wire x1="88.9" y1="-2.54" x2="99.06" y2="-2.54" width="0.1016" layer="91"/>
<label x="99.06" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D6"/>
<wire x1="88.9" y1="-5.08" x2="99.06" y2="-5.08" width="0.1016" layer="91"/>
<label x="99.06" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D7"/>
<wire x1="88.9" y1="-7.62" x2="99.06" y2="-7.62" width="0.1016" layer="91"/>
<label x="99.06" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D8"/>
<wire x1="88.9" y1="-10.16" x2="99.06" y2="-10.16" width="0.1016" layer="91"/>
<label x="99.06" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D10"/>
<wire x1="88.9" y1="-15.24" x2="99.06" y2="-15.24" width="0.1016" layer="91"/>
<label x="99.06" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D11"/>
<wire x1="88.9" y1="-17.78" x2="99.06" y2="-17.78" width="0.1016" layer="91"/>
<label x="99.06" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D12"/>
<wire x1="88.9" y1="-20.32" x2="99.06" y2="-20.32" width="0.1016" layer="91"/>
<label x="99.06" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D13"/>
<wire x1="88.9" y1="-22.86" x2="99.06" y2="-22.86" width="0.1016" layer="91"/>
<label x="99.06" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D14"/>
<wire x1="88.9" y1="-25.4" x2="99.06" y2="-25.4" width="0.1016" layer="91"/>
<label x="99.06" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D15"/>
<wire x1="88.9" y1="-27.94" x2="99.06" y2="-27.94" width="0.1016" layer="91"/>
<label x="99.06" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D17"/>
<wire x1="88.9" y1="-33.02" x2="99.06" y2="-33.02" width="0.1016" layer="91"/>
<label x="99.06" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D18"/>
<wire x1="88.9" y1="-35.56" x2="99.06" y2="-35.56" width="0.1016" layer="91"/>
<label x="99.06" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D19"/>
<wire x1="88.9" y1="-38.1" x2="99.06" y2="-38.1" width="0.1016" layer="91"/>
<label x="99.06" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D20"/>
<wire x1="88.9" y1="-40.64" x2="99.06" y2="-40.64" width="0.1016" layer="91"/>
<label x="99.06" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D21"/>
<wire x1="88.9" y1="-43.18" x2="99.06" y2="-43.18" width="0.1016" layer="91"/>
<label x="99.06" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D22"/>
<wire x1="88.9" y1="-45.72" x2="99.06" y2="-45.72" width="0.1016" layer="91"/>
<label x="99.06" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D24"/>
<wire x1="88.9" y1="-50.8" x2="99.06" y2="-50.8" width="0.1016" layer="91"/>
<label x="99.06" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D25"/>
<wire x1="88.9" y1="-53.34" x2="99.06" y2="-53.34" width="0.1016" layer="91"/>
<label x="99.06" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D26"/>
<wire x1="88.9" y1="-55.88" x2="99.06" y2="-55.88" width="0.1016" layer="91"/>
<label x="99.06" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D27"/>
<wire x1="88.9" y1="-58.42" x2="99.06" y2="-58.42" width="0.1016" layer="91"/>
<label x="99.06" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D28"/>
<wire x1="88.9" y1="-60.96" x2="99.06" y2="-60.96" width="0.1016" layer="91"/>
<label x="99.06" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="D29"/>
<wire x1="88.9" y1="-63.5" x2="99.06" y2="-63.5" width="0.1016" layer="91"/>
<label x="99.06" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E3"/>
<wire x1="114.3" y1="2.54" x2="124.46" y2="2.54" width="0.1016" layer="91"/>
<label x="124.46" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E4"/>
<wire x1="114.3" y1="0" x2="124.46" y2="0" width="0.1016" layer="91"/>
<label x="124.46" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E5"/>
<wire x1="114.3" y1="-2.54" x2="124.46" y2="-2.54" width="0.1016" layer="91"/>
<label x="124.46" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E6"/>
<wire x1="114.3" y1="-5.08" x2="124.46" y2="-5.08" width="0.1016" layer="91"/>
<label x="124.46" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E7"/>
<wire x1="114.3" y1="-7.62" x2="124.46" y2="-7.62" width="0.1016" layer="91"/>
<label x="124.46" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E8"/>
<wire x1="114.3" y1="-10.16" x2="124.46" y2="-10.16" width="0.1016" layer="91"/>
<label x="124.46" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E10"/>
<wire x1="114.3" y1="-15.24" x2="124.46" y2="-15.24" width="0.1016" layer="91"/>
<label x="124.46" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E11"/>
<wire x1="114.3" y1="-17.78" x2="124.46" y2="-17.78" width="0.1016" layer="91"/>
<label x="124.46" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E12"/>
<wire x1="114.3" y1="-20.32" x2="124.46" y2="-20.32" width="0.1016" layer="91"/>
<label x="124.46" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.006" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E13"/>
<wire x1="114.3" y1="-22.86" x2="124.46" y2="-22.86" width="0.1016" layer="91"/>
<label x="124.46" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E14"/>
<wire x1="114.3" y1="-25.4" x2="124.46" y2="-25.4" width="0.1016" layer="91"/>
<label x="124.46" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.001" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E15"/>
<wire x1="114.3" y1="-27.94" x2="124.46" y2="-27.94" width="0.1016" layer="91"/>
<label x="124.46" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E17"/>
<wire x1="114.3" y1="-33.02" x2="124.46" y2="-33.02" width="0.1016" layer="91"/>
<label x="124.46" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E18"/>
<wire x1="114.3" y1="-35.56" x2="124.46" y2="-35.56" width="0.1016" layer="91"/>
<label x="124.46" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E19"/>
<wire x1="114.3" y1="-38.1" x2="124.46" y2="-38.1" width="0.1016" layer="91"/>
<label x="124.46" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E20"/>
<wire x1="114.3" y1="-40.64" x2="124.46" y2="-40.64" width="0.1016" layer="91"/>
<label x="124.46" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E21"/>
<wire x1="114.3" y1="-43.18" x2="124.46" y2="-43.18" width="0.1016" layer="91"/>
<label x="124.46" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E22"/>
<wire x1="114.3" y1="-45.72" x2="124.46" y2="-45.72" width="0.1016" layer="91"/>
<label x="124.46" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E24"/>
<wire x1="114.3" y1="-50.8" x2="124.46" y2="-50.8" width="0.1016" layer="91"/>
<label x="124.46" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E25"/>
<wire x1="114.3" y1="-53.34" x2="124.46" y2="-53.34" width="0.1016" layer="91"/>
<label x="124.46" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E26"/>
<wire x1="114.3" y1="-55.88" x2="124.46" y2="-55.88" width="0.1016" layer="91"/>
<label x="124.46" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E27"/>
<wire x1="114.3" y1="-58.42" x2="124.46" y2="-58.42" width="0.1016" layer="91"/>
<label x="124.46" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E28"/>
<wire x1="114.3" y1="-60.96" x2="124.46" y2="-60.96" width="0.1016" layer="91"/>
<label x="124.46" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.008" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E29"/>
<wire x1="114.3" y1="-63.5" x2="124.46" y2="-63.5" width="0.1016" layer="91"/>
<label x="124.46" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F3"/>
<wire x1="139.7" y1="2.54" x2="149.86" y2="2.54" width="0.1016" layer="91"/>
<label x="149.86" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F4"/>
<wire x1="139.7" y1="0" x2="149.86" y2="0" width="0.1016" layer="91"/>
<label x="149.86" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F5"/>
<wire x1="139.7" y1="-2.54" x2="149.86" y2="-2.54" width="0.1016" layer="91"/>
<label x="149.86" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F6"/>
<wire x1="139.7" y1="-5.08" x2="149.86" y2="-5.08" width="0.1016" layer="91"/>
<label x="149.86" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F7"/>
<wire x1="139.7" y1="-7.62" x2="149.86" y2="-7.62" width="0.1016" layer="91"/>
<label x="149.86" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F8"/>
<wire x1="139.7" y1="-10.16" x2="149.86" y2="-10.16" width="0.1016" layer="91"/>
<label x="149.86" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F10"/>
<wire x1="139.7" y1="-15.24" x2="149.86" y2="-15.24" width="0.1016" layer="91"/>
<label x="149.86" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.011" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F11"/>
<wire x1="139.7" y1="-17.78" x2="149.86" y2="-17.78" width="0.1016" layer="91"/>
<label x="149.86" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F12"/>
<wire x1="139.7" y1="-20.32" x2="149.86" y2="-20.32" width="0.1016" layer="91"/>
<label x="149.86" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F13"/>
<wire x1="139.7" y1="-22.86" x2="149.86" y2="-22.86" width="0.1016" layer="91"/>
<label x="149.86" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F14"/>
<wire x1="139.7" y1="-25.4" x2="149.86" y2="-25.4" width="0.1016" layer="91"/>
<label x="149.86" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.002" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F15"/>
<wire x1="139.7" y1="-27.94" x2="149.86" y2="-27.94" width="0.1016" layer="91"/>
<label x="149.86" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F17"/>
<wire x1="139.7" y1="-33.02" x2="149.86" y2="-33.02" width="0.1016" layer="91"/>
<label x="149.86" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F18"/>
<wire x1="139.7" y1="-35.56" x2="149.86" y2="-35.56" width="0.1016" layer="91"/>
<label x="149.86" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F19"/>
<wire x1="139.7" y1="-38.1" x2="149.86" y2="-38.1" width="0.1016" layer="91"/>
<label x="149.86" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F20"/>
<wire x1="139.7" y1="-40.64" x2="149.86" y2="-40.64" width="0.1016" layer="91"/>
<label x="149.86" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.010" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F21"/>
<wire x1="139.7" y1="-43.18" x2="149.86" y2="-43.18" width="0.1016" layer="91"/>
<label x="149.86" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F22"/>
<wire x1="139.7" y1="-45.72" x2="149.86" y2="-45.72" width="0.1016" layer="91"/>
<label x="149.86" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F24"/>
<wire x1="139.7" y1="-50.8" x2="149.86" y2="-50.8" width="0.1016" layer="91"/>
<label x="149.86" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F25"/>
<wire x1="139.7" y1="-53.34" x2="149.86" y2="-53.34" width="0.1016" layer="91"/>
<label x="149.86" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.004" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F26"/>
<wire x1="139.7" y1="-55.88" x2="149.86" y2="-55.88" width="0.1016" layer="91"/>
<label x="149.86" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.009" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F27"/>
<wire x1="139.7" y1="-58.42" x2="149.86" y2="-58.42" width="0.1016" layer="91"/>
<label x="149.86" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.003" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F28"/>
<wire x1="139.7" y1="-60.96" x2="149.86" y2="-60.96" width="0.1016" layer="91"/>
<label x="149.86" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.013" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F29"/>
<wire x1="139.7" y1="-63.5" x2="149.86" y2="-63.5" width="0.1016" layer="91"/>
<label x="149.86" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E2"/>
<wire x1="114.3" y1="5.08" x2="124.46" y2="5.08" width="0.1016" layer="91"/>
<label x="124.46" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F2"/>
<wire x1="139.7" y1="5.08" x2="149.86" y2="5.08" width="0.1016" layer="91"/>
<label x="149.86" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.012" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E9"/>
<wire x1="114.3" y1="-12.7" x2="124.46" y2="-12.7" width="0.1016" layer="91"/>
<label x="124.46" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.007" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F9"/>
<wire x1="139.7" y1="-12.7" x2="149.86" y2="-12.7" width="0.1016" layer="91"/>
<label x="149.86" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E16"/>
<wire x1="114.3" y1="-30.48" x2="124.46" y2="-30.48" width="0.1016" layer="91"/>
<label x="124.46" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.005" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F16"/>
<wire x1="139.7" y1="-30.48" x2="149.86" y2="-30.48" width="0.1016" layer="91"/>
<label x="149.86" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.015" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="E23"/>
<wire x1="114.3" y1="-48.26" x2="124.46" y2="-48.26" width="0.1016" layer="91"/>
<label x="124.46" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.014" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="F23"/>
<wire x1="139.7" y1="-48.26" x2="149.86" y2="-48.26" width="0.1016" layer="91"/>
<label x="149.86" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$3" gate="G$1" x="60.96" y="66.04" smashed="yes"/>
<instance part="U$4" gate="G$1" x="60.96" y="-17.78" smashed="yes"/>
<instance part="SUPPLY13" gate="GND" x="22.86" y="15.24" smashed="yes">
<attribute name="VALUE" x="20.955" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY14" gate="GND" x="48.26" y="15.24" smashed="yes">
<attribute name="VALUE" x="46.355" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY15" gate="GND" x="73.66" y="15.24" smashed="yes">
<attribute name="VALUE" x="71.755" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY16" gate="GND" x="99.06" y="15.24" smashed="yes">
<attribute name="VALUE" x="97.155" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY17" gate="GND" x="124.46" y="12.7" smashed="yes">
<attribute name="VALUE" x="122.555" y="9.525" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY18" gate="GND" x="149.86" y="15.24" smashed="yes">
<attribute name="VALUE" x="147.955" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY19" gate="GND" x="22.86" y="-68.58" smashed="yes">
<attribute name="VALUE" x="20.955" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY20" gate="GND" x="50.8" y="-68.58" smashed="yes">
<attribute name="VALUE" x="48.895" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY21" gate="GND" x="73.66" y="-68.58" smashed="yes">
<attribute name="VALUE" x="71.755" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY22" gate="GND" x="99.06" y="-68.58" smashed="yes">
<attribute name="VALUE" x="97.155" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY23" gate="GND" x="127" y="-68.58" smashed="yes">
<attribute name="VALUE" x="125.095" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY24" gate="GND" x="157.48" y="-68.58" smashed="yes">
<attribute name="VALUE" x="155.575" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="U$18" gate="G$1" x="35.56" y="15.24" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="A1"/>
<wire x1="17.78" y1="93.98" x2="25.4" y2="93.98" width="0.1524" layer="91"/>
<wire x1="25.4" y1="93.98" x2="25.4" y2="91.44" width="0.1524" layer="91"/>
<pinref part="SUPPLY13" gate="GND" pin="GND"/>
<wire x1="25.4" y1="91.44" x2="25.4" y2="88.9" width="0.1524" layer="91"/>
<wire x1="25.4" y1="88.9" x2="25.4" y2="86.36" width="0.1524" layer="91"/>
<wire x1="25.4" y1="86.36" x2="25.4" y2="83.82" width="0.1524" layer="91"/>
<wire x1="25.4" y1="83.82" x2="25.4" y2="81.28" width="0.1524" layer="91"/>
<wire x1="25.4" y1="81.28" x2="25.4" y2="78.74" width="0.1524" layer="91"/>
<wire x1="25.4" y1="78.74" x2="25.4" y2="76.2" width="0.1524" layer="91"/>
<wire x1="25.4" y1="76.2" x2="25.4" y2="73.66" width="0.1524" layer="91"/>
<wire x1="25.4" y1="73.66" x2="25.4" y2="71.12" width="0.1524" layer="91"/>
<wire x1="25.4" y1="71.12" x2="25.4" y2="68.58" width="0.1524" layer="91"/>
<wire x1="25.4" y1="68.58" x2="25.4" y2="66.04" width="0.1524" layer="91"/>
<wire x1="25.4" y1="66.04" x2="25.4" y2="63.5" width="0.1524" layer="91"/>
<wire x1="25.4" y1="63.5" x2="25.4" y2="60.96" width="0.1524" layer="91"/>
<wire x1="25.4" y1="60.96" x2="25.4" y2="58.42" width="0.1524" layer="91"/>
<wire x1="25.4" y1="58.42" x2="25.4" y2="55.88" width="0.1524" layer="91"/>
<wire x1="25.4" y1="55.88" x2="25.4" y2="53.34" width="0.1524" layer="91"/>
<wire x1="25.4" y1="53.34" x2="25.4" y2="50.8" width="0.1524" layer="91"/>
<wire x1="25.4" y1="50.8" x2="25.4" y2="48.26" width="0.1524" layer="91"/>
<wire x1="25.4" y1="48.26" x2="25.4" y2="45.72" width="0.1524" layer="91"/>
<wire x1="25.4" y1="45.72" x2="25.4" y2="43.18" width="0.1524" layer="91"/>
<wire x1="25.4" y1="43.18" x2="25.4" y2="40.64" width="0.1524" layer="91"/>
<wire x1="25.4" y1="40.64" x2="25.4" y2="38.1" width="0.1524" layer="91"/>
<wire x1="25.4" y1="38.1" x2="25.4" y2="35.56" width="0.1524" layer="91"/>
<wire x1="25.4" y1="35.56" x2="25.4" y2="33.02" width="0.1524" layer="91"/>
<wire x1="25.4" y1="33.02" x2="25.4" y2="30.48" width="0.1524" layer="91"/>
<wire x1="25.4" y1="30.48" x2="25.4" y2="27.94" width="0.1524" layer="91"/>
<wire x1="25.4" y1="27.94" x2="25.4" y2="25.4" width="0.1524" layer="91"/>
<wire x1="25.4" y1="25.4" x2="25.4" y2="22.86" width="0.1524" layer="91"/>
<wire x1="25.4" y1="22.86" x2="25.4" y2="17.78" width="0.1524" layer="91"/>
<wire x1="25.4" y1="17.78" x2="22.86" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="A29"/>
<wire x1="17.78" y1="22.86" x2="25.4" y2="22.86" width="0.1524" layer="91"/>
<junction x="25.4" y="22.86"/>
<pinref part="U$3" gate="G$1" pin="A28"/>
<wire x1="17.78" y1="25.4" x2="25.4" y2="25.4" width="0.1524" layer="91"/>
<junction x="25.4" y="25.4"/>
<pinref part="U$3" gate="G$1" pin="A27"/>
<wire x1="17.78" y1="27.94" x2="25.4" y2="27.94" width="0.1524" layer="91"/>
<junction x="25.4" y="27.94"/>
<pinref part="U$3" gate="G$1" pin="A26"/>
<wire x1="17.78" y1="30.48" x2="25.4" y2="30.48" width="0.1524" layer="91"/>
<junction x="25.4" y="30.48"/>
<pinref part="U$3" gate="G$1" pin="A25"/>
<wire x1="17.78" y1="33.02" x2="25.4" y2="33.02" width="0.1524" layer="91"/>
<junction x="25.4" y="33.02"/>
<pinref part="U$3" gate="G$1" pin="A24"/>
<wire x1="17.78" y1="35.56" x2="25.4" y2="35.56" width="0.1524" layer="91"/>
<junction x="25.4" y="35.56"/>
<pinref part="U$3" gate="G$1" pin="A23"/>
<wire x1="17.78" y1="38.1" x2="25.4" y2="38.1" width="0.1524" layer="91"/>
<junction x="25.4" y="38.1"/>
<pinref part="U$3" gate="G$1" pin="A22"/>
<wire x1="25.4" y1="40.64" x2="17.78" y2="40.64" width="0.1524" layer="91"/>
<junction x="25.4" y="40.64"/>
<pinref part="U$3" gate="G$1" pin="A21"/>
<wire x1="17.78" y1="43.18" x2="25.4" y2="43.18" width="0.1524" layer="91"/>
<junction x="25.4" y="43.18"/>
<pinref part="U$3" gate="G$1" pin="A20"/>
<wire x1="17.78" y1="45.72" x2="25.4" y2="45.72" width="0.1524" layer="91"/>
<junction x="25.4" y="45.72"/>
<pinref part="U$3" gate="G$1" pin="A19"/>
<wire x1="17.78" y1="48.26" x2="25.4" y2="48.26" width="0.1524" layer="91"/>
<junction x="25.4" y="48.26"/>
<pinref part="U$3" gate="G$1" pin="A18"/>
<wire x1="17.78" y1="50.8" x2="25.4" y2="50.8" width="0.1524" layer="91"/>
<junction x="25.4" y="50.8"/>
<pinref part="U$3" gate="G$1" pin="A17"/>
<wire x1="17.78" y1="53.34" x2="25.4" y2="53.34" width="0.1524" layer="91"/>
<junction x="25.4" y="53.34"/>
<pinref part="U$3" gate="G$1" pin="A16"/>
<wire x1="17.78" y1="55.88" x2="25.4" y2="55.88" width="0.1524" layer="91"/>
<junction x="25.4" y="55.88"/>
<pinref part="U$3" gate="G$1" pin="A15"/>
<wire x1="17.78" y1="58.42" x2="25.4" y2="58.42" width="0.1524" layer="91"/>
<junction x="25.4" y="58.42"/>
<pinref part="U$3" gate="G$1" pin="A14"/>
<wire x1="17.78" y1="60.96" x2="25.4" y2="60.96" width="0.1524" layer="91"/>
<junction x="25.4" y="60.96"/>
<pinref part="U$3" gate="G$1" pin="A13"/>
<wire x1="17.78" y1="63.5" x2="25.4" y2="63.5" width="0.1524" layer="91"/>
<junction x="25.4" y="63.5"/>
<pinref part="U$3" gate="G$1" pin="A12"/>
<wire x1="17.78" y1="66.04" x2="25.4" y2="66.04" width="0.1524" layer="91"/>
<junction x="25.4" y="66.04"/>
<pinref part="U$3" gate="G$1" pin="A11"/>
<wire x1="17.78" y1="68.58" x2="25.4" y2="68.58" width="0.1524" layer="91"/>
<junction x="25.4" y="68.58"/>
<pinref part="U$3" gate="G$1" pin="A10"/>
<wire x1="17.78" y1="71.12" x2="25.4" y2="71.12" width="0.1524" layer="91"/>
<junction x="25.4" y="71.12"/>
<pinref part="U$3" gate="G$1" pin="A9"/>
<wire x1="17.78" y1="73.66" x2="25.4" y2="73.66" width="0.1524" layer="91"/>
<junction x="25.4" y="73.66"/>
<pinref part="U$3" gate="G$1" pin="A8"/>
<wire x1="17.78" y1="76.2" x2="25.4" y2="76.2" width="0.1524" layer="91"/>
<junction x="25.4" y="76.2"/>
<pinref part="U$3" gate="G$1" pin="A7"/>
<wire x1="25.4" y1="78.74" x2="17.78" y2="78.74" width="0.1524" layer="91"/>
<junction x="25.4" y="78.74"/>
<pinref part="U$3" gate="G$1" pin="A6"/>
<wire x1="17.78" y1="81.28" x2="25.4" y2="81.28" width="0.1524" layer="91"/>
<junction x="25.4" y="81.28"/>
<pinref part="U$3" gate="G$1" pin="A5"/>
<wire x1="17.78" y1="83.82" x2="25.4" y2="83.82" width="0.1524" layer="91"/>
<junction x="25.4" y="83.82"/>
<pinref part="U$3" gate="G$1" pin="A4"/>
<wire x1="17.78" y1="86.36" x2="25.4" y2="86.36" width="0.1524" layer="91"/>
<junction x="25.4" y="86.36"/>
<pinref part="U$3" gate="G$1" pin="A3"/>
<wire x1="25.4" y1="88.9" x2="17.78" y2="88.9" width="0.1524" layer="91"/>
<junction x="25.4" y="88.9"/>
<pinref part="U$3" gate="G$1" pin="A2"/>
<wire x1="17.78" y1="91.44" x2="25.4" y2="91.44" width="0.1524" layer="91"/>
<junction x="25.4" y="91.44"/>
<wire x1="22.86" y1="17.78" x2="27.94" y2="17.78" width="0.1524" layer="91"/>
<junction x="22.86" y="17.78"/>
<pinref part="U$18" gate="G$1" pin="GND"/>
<wire x1="27.94" y1="17.78" x2="27.94" y2="15.24" width="0.1524" layer="91"/>
<wire x1="27.94" y1="15.24" x2="35.56" y2="15.24" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="B1"/>
<wire x1="43.18" y1="93.98" x2="50.8" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY14" gate="GND" pin="GND"/>
<wire x1="50.8" y1="93.98" x2="50.8" y2="91.44" width="0.1524" layer="91"/>
<wire x1="50.8" y1="91.44" x2="50.8" y2="73.66" width="0.1524" layer="91"/>
<wire x1="50.8" y1="73.66" x2="50.8" y2="55.88" width="0.1524" layer="91"/>
<wire x1="50.8" y1="55.88" x2="50.8" y2="38.1" width="0.1524" layer="91"/>
<wire x1="50.8" y1="38.1" x2="50.8" y2="20.32" width="0.1524" layer="91"/>
<wire x1="50.8" y1="20.32" x2="50.8" y2="17.78" width="0.1524" layer="91"/>
<wire x1="50.8" y1="17.78" x2="48.26" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="B2"/>
<wire x1="43.18" y1="91.44" x2="50.8" y2="91.44" width="0.1524" layer="91"/>
<junction x="50.8" y="91.44"/>
<pinref part="U$3" gate="G$1" pin="B9"/>
<wire x1="43.18" y1="73.66" x2="50.8" y2="73.66" width="0.1524" layer="91"/>
<junction x="50.8" y="73.66"/>
<pinref part="U$3" gate="G$1" pin="B16"/>
<wire x1="43.18" y1="55.88" x2="50.8" y2="55.88" width="0.1524" layer="91"/>
<junction x="50.8" y="55.88"/>
<pinref part="U$3" gate="G$1" pin="B23"/>
<wire x1="43.18" y1="38.1" x2="50.8" y2="38.1" width="0.1524" layer="91"/>
<junction x="50.8" y="38.1"/>
<pinref part="U$3" gate="G$1" pin="B30"/>
<wire x1="43.18" y1="20.32" x2="50.8" y2="20.32" width="0.1524" layer="91"/>
<junction x="50.8" y="20.32"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="C1"/>
<wire x1="68.58" y1="93.98" x2="76.2" y2="93.98" width="0.1524" layer="91"/>
<wire x1="76.2" y1="93.98" x2="76.2" y2="91.44" width="0.1524" layer="91"/>
<pinref part="SUPPLY15" gate="GND" pin="GND"/>
<wire x1="76.2" y1="91.44" x2="76.2" y2="73.66" width="0.1524" layer="91"/>
<wire x1="76.2" y1="73.66" x2="76.2" y2="55.88" width="0.1524" layer="91"/>
<wire x1="76.2" y1="55.88" x2="76.2" y2="38.1" width="0.1524" layer="91"/>
<wire x1="76.2" y1="38.1" x2="76.2" y2="20.32" width="0.1524" layer="91"/>
<wire x1="76.2" y1="20.32" x2="76.2" y2="17.78" width="0.1524" layer="91"/>
<wire x1="76.2" y1="17.78" x2="73.66" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="C2"/>
<wire x1="68.58" y1="91.44" x2="76.2" y2="91.44" width="0.1524" layer="91"/>
<junction x="76.2" y="91.44"/>
<pinref part="U$3" gate="G$1" pin="C9"/>
<wire x1="68.58" y1="73.66" x2="76.2" y2="73.66" width="0.1524" layer="91"/>
<junction x="76.2" y="73.66"/>
<pinref part="U$3" gate="G$1" pin="C16"/>
<wire x1="68.58" y1="55.88" x2="76.2" y2="55.88" width="0.1524" layer="91"/>
<junction x="76.2" y="55.88"/>
<pinref part="U$3" gate="G$1" pin="C23"/>
<wire x1="68.58" y1="38.1" x2="76.2" y2="38.1" width="0.1524" layer="91"/>
<junction x="76.2" y="38.1"/>
<pinref part="U$3" gate="G$1" pin="C30"/>
<wire x1="68.58" y1="20.32" x2="76.2" y2="20.32" width="0.1524" layer="91"/>
<junction x="76.2" y="20.32"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="D1"/>
<wire x1="93.98" y1="93.98" x2="101.6" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY16" gate="GND" pin="GND"/>
<wire x1="101.6" y1="93.98" x2="101.6" y2="91.44" width="0.1524" layer="91"/>
<wire x1="101.6" y1="91.44" x2="101.6" y2="73.66" width="0.1524" layer="91"/>
<wire x1="101.6" y1="73.66" x2="101.6" y2="55.88" width="0.1524" layer="91"/>
<wire x1="101.6" y1="55.88" x2="101.6" y2="38.1" width="0.1524" layer="91"/>
<wire x1="101.6" y1="38.1" x2="101.6" y2="20.32" width="0.1524" layer="91"/>
<wire x1="101.6" y1="20.32" x2="101.6" y2="17.78" width="0.1524" layer="91"/>
<wire x1="101.6" y1="17.78" x2="99.06" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="D2"/>
<wire x1="93.98" y1="91.44" x2="101.6" y2="91.44" width="0.1524" layer="91"/>
<junction x="101.6" y="91.44"/>
<pinref part="U$3" gate="G$1" pin="D9"/>
<wire x1="93.98" y1="73.66" x2="101.6" y2="73.66" width="0.1524" layer="91"/>
<junction x="101.6" y="73.66"/>
<pinref part="U$3" gate="G$1" pin="D16"/>
<wire x1="93.98" y1="55.88" x2="101.6" y2="55.88" width="0.1524" layer="91"/>
<junction x="101.6" y="55.88"/>
<pinref part="U$3" gate="G$1" pin="D30"/>
<wire x1="93.98" y1="20.32" x2="101.6" y2="20.32" width="0.1524" layer="91"/>
<junction x="101.6" y="20.32"/>
<pinref part="U$3" gate="G$1" pin="D23"/>
<wire x1="93.98" y1="38.1" x2="101.6" y2="38.1" width="0.1524" layer="91"/>
<junction x="101.6" y="38.1"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="E1"/>
<wire x1="119.38" y1="93.98" x2="127" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY17" gate="GND" pin="GND"/>
<wire x1="127" y1="93.98" x2="127" y2="20.32" width="0.1524" layer="91"/>
<wire x1="127" y1="20.32" x2="127" y2="15.24" width="0.1524" layer="91"/>
<wire x1="127" y1="15.24" x2="124.46" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="E30"/>
<wire x1="119.38" y1="20.32" x2="127" y2="20.32" width="0.1524" layer="91"/>
<junction x="127" y="20.32"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="F1"/>
<wire x1="144.78" y1="93.98" x2="152.4" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY18" gate="GND" pin="GND"/>
<wire x1="152.4" y1="93.98" x2="152.4" y2="20.32" width="0.1524" layer="91"/>
<wire x1="152.4" y1="20.32" x2="152.4" y2="17.78" width="0.1524" layer="91"/>
<wire x1="152.4" y1="17.78" x2="149.86" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$3" gate="G$1" pin="F30"/>
<wire x1="144.78" y1="20.32" x2="152.4" y2="20.32" width="0.1524" layer="91"/>
<junction x="152.4" y="20.32"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="A1"/>
<wire x1="17.78" y1="10.16" x2="25.4" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY19" gate="GND" pin="GND"/>
<wire x1="25.4" y1="10.16" x2="25.4" y2="7.62" width="0.1524" layer="91"/>
<wire x1="25.4" y1="7.62" x2="25.4" y2="2.54" width="0.1524" layer="91"/>
<wire x1="25.4" y1="2.54" x2="25.4" y2="0" width="0.1524" layer="91"/>
<wire x1="25.4" y1="0" x2="25.4" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-2.54" x2="25.4" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-5.08" x2="25.4" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-7.62" x2="25.4" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-10.16" x2="25.4" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-12.7" x2="25.4" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-15.24" x2="25.4" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-17.78" x2="25.4" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-20.32" x2="25.4" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-22.86" x2="25.4" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-25.4" x2="25.4" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-27.94" x2="25.4" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-30.48" x2="25.4" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-33.02" x2="25.4" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-35.56" x2="25.4" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-38.1" x2="25.4" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-40.64" x2="25.4" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-43.18" x2="25.4" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-45.72" x2="25.4" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-48.26" x2="25.4" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-50.8" x2="25.4" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-53.34" x2="25.4" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-55.88" x2="25.4" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-58.42" x2="25.4" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-60.96" x2="25.4" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-66.04" x2="22.86" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="A29"/>
<wire x1="17.78" y1="-60.96" x2="25.4" y2="-60.96" width="0.1524" layer="91"/>
<junction x="25.4" y="-60.96"/>
<pinref part="U$4" gate="G$1" pin="A28"/>
<wire x1="17.78" y1="-58.42" x2="25.4" y2="-58.42" width="0.1524" layer="91"/>
<junction x="25.4" y="-58.42"/>
<pinref part="U$4" gate="G$1" pin="A27"/>
<wire x1="17.78" y1="-55.88" x2="25.4" y2="-55.88" width="0.1524" layer="91"/>
<junction x="25.4" y="-55.88"/>
<pinref part="U$4" gate="G$1" pin="A26"/>
<wire x1="17.78" y1="-53.34" x2="25.4" y2="-53.34" width="0.1524" layer="91"/>
<junction x="25.4" y="-53.34"/>
<pinref part="U$4" gate="G$1" pin="A25"/>
<wire x1="17.78" y1="-50.8" x2="25.4" y2="-50.8" width="0.1524" layer="91"/>
<junction x="25.4" y="-50.8"/>
<pinref part="U$4" gate="G$1" pin="A24"/>
<wire x1="17.78" y1="-48.26" x2="25.4" y2="-48.26" width="0.1524" layer="91"/>
<junction x="25.4" y="-48.26"/>
<pinref part="U$4" gate="G$1" pin="A23"/>
<wire x1="17.78" y1="-45.72" x2="25.4" y2="-45.72" width="0.1524" layer="91"/>
<junction x="25.4" y="-45.72"/>
<pinref part="U$4" gate="G$1" pin="A22"/>
<wire x1="17.78" y1="-43.18" x2="25.4" y2="-43.18" width="0.1524" layer="91"/>
<junction x="25.4" y="-43.18"/>
<pinref part="U$4" gate="G$1" pin="A21"/>
<wire x1="17.78" y1="-40.64" x2="25.4" y2="-40.64" width="0.1524" layer="91"/>
<junction x="25.4" y="-40.64"/>
<pinref part="U$4" gate="G$1" pin="A20"/>
<wire x1="17.78" y1="-38.1" x2="25.4" y2="-38.1" width="0.1524" layer="91"/>
<junction x="25.4" y="-38.1"/>
<pinref part="U$4" gate="G$1" pin="A19"/>
<wire x1="17.78" y1="-35.56" x2="25.4" y2="-35.56" width="0.1524" layer="91"/>
<junction x="25.4" y="-35.56"/>
<pinref part="U$4" gate="G$1" pin="A18"/>
<wire x1="17.78" y1="-33.02" x2="25.4" y2="-33.02" width="0.1524" layer="91"/>
<junction x="25.4" y="-33.02"/>
<pinref part="U$4" gate="G$1" pin="A17"/>
<wire x1="17.78" y1="-30.48" x2="25.4" y2="-30.48" width="0.1524" layer="91"/>
<junction x="25.4" y="-30.48"/>
<pinref part="U$4" gate="G$1" pin="A16"/>
<wire x1="17.78" y1="-27.94" x2="25.4" y2="-27.94" width="0.1524" layer="91"/>
<junction x="25.4" y="-27.94"/>
<pinref part="U$4" gate="G$1" pin="A15"/>
<wire x1="17.78" y1="-25.4" x2="25.4" y2="-25.4" width="0.1524" layer="91"/>
<junction x="25.4" y="-25.4"/>
<pinref part="U$4" gate="G$1" pin="A14"/>
<wire x1="17.78" y1="-22.86" x2="25.4" y2="-22.86" width="0.1524" layer="91"/>
<junction x="25.4" y="-22.86"/>
<pinref part="U$4" gate="G$1" pin="A13"/>
<wire x1="17.78" y1="-20.32" x2="25.4" y2="-20.32" width="0.1524" layer="91"/>
<junction x="25.4" y="-20.32"/>
<pinref part="U$4" gate="G$1" pin="A12"/>
<wire x1="17.78" y1="-17.78" x2="25.4" y2="-17.78" width="0.1524" layer="91"/>
<junction x="25.4" y="-17.78"/>
<pinref part="U$4" gate="G$1" pin="A11"/>
<wire x1="17.78" y1="-15.24" x2="25.4" y2="-15.24" width="0.1524" layer="91"/>
<junction x="25.4" y="-15.24"/>
<pinref part="U$4" gate="G$1" pin="A10"/>
<wire x1="17.78" y1="-12.7" x2="25.4" y2="-12.7" width="0.1524" layer="91"/>
<junction x="25.4" y="-12.7"/>
<pinref part="U$4" gate="G$1" pin="A9"/>
<wire x1="17.78" y1="-10.16" x2="25.4" y2="-10.16" width="0.1524" layer="91"/>
<junction x="25.4" y="-10.16"/>
<pinref part="U$4" gate="G$1" pin="A8"/>
<wire x1="17.78" y1="-7.62" x2="25.4" y2="-7.62" width="0.1524" layer="91"/>
<junction x="25.4" y="-7.62"/>
<pinref part="U$4" gate="G$1" pin="A7"/>
<wire x1="17.78" y1="-5.08" x2="25.4" y2="-5.08" width="0.1524" layer="91"/>
<junction x="25.4" y="-5.08"/>
<pinref part="U$4" gate="G$1" pin="A6"/>
<wire x1="17.78" y1="-2.54" x2="25.4" y2="-2.54" width="0.1524" layer="91"/>
<junction x="25.4" y="-2.54"/>
<pinref part="U$4" gate="G$1" pin="A5"/>
<wire x1="17.78" y1="0" x2="25.4" y2="0" width="0.1524" layer="91"/>
<junction x="25.4" y="0"/>
<pinref part="U$4" gate="G$1" pin="A4"/>
<wire x1="17.78" y1="2.54" x2="25.4" y2="2.54" width="0.1524" layer="91"/>
<junction x="25.4" y="2.54"/>
<pinref part="U$4" gate="G$1" pin="A3"/>
<wire x1="17.78" y1="5.08" x2="25.4" y2="5.08" width="0.1524" layer="91"/>
<wire x1="25.4" y1="5.08" x2="25.4" y2="2.54" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="A2"/>
<wire x1="17.78" y1="7.62" x2="25.4" y2="7.62" width="0.1524" layer="91"/>
<junction x="25.4" y="7.62"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="B1"/>
<wire x1="43.18" y1="10.16" x2="50.8" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY20" gate="GND" pin="GND"/>
<wire x1="50.8" y1="10.16" x2="50.8" y2="7.62" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="B23"/>
<wire x1="50.8" y1="7.62" x2="50.8" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-10.16" x2="50.8" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-27.94" x2="50.8" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-45.72" x2="50.8" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-63.5" x2="50.8" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="43.18" y1="-45.72" x2="50.8" y2="-45.72" width="0.1524" layer="91"/>
<junction x="50.8" y="-45.72"/>
<pinref part="U$4" gate="G$1" pin="B2"/>
<wire x1="43.18" y1="7.62" x2="50.8" y2="7.62" width="0.1524" layer="91"/>
<junction x="50.8" y="7.62"/>
<pinref part="U$4" gate="G$1" pin="B30"/>
<wire x1="43.18" y1="-63.5" x2="50.8" y2="-63.5" width="0.1524" layer="91"/>
<junction x="50.8" y="-63.5"/>
<pinref part="U$4" gate="G$1" pin="B16"/>
<wire x1="43.18" y1="-27.94" x2="50.8" y2="-27.94" width="0.1524" layer="91"/>
<junction x="50.8" y="-27.94"/>
<pinref part="U$4" gate="G$1" pin="B9"/>
<wire x1="43.18" y1="-10.16" x2="50.8" y2="-10.16" width="0.1524" layer="91"/>
<junction x="50.8" y="-10.16"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="C1"/>
<wire x1="68.58" y1="10.16" x2="76.2" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY21" gate="GND" pin="GND"/>
<wire x1="76.2" y1="10.16" x2="76.2" y2="7.62" width="0.1524" layer="91"/>
<wire x1="76.2" y1="7.62" x2="76.2" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-10.16" x2="76.2" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-27.94" x2="76.2" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-45.72" x2="76.2" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-63.5" x2="76.2" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-66.04" x2="73.66" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="C23"/>
<wire x1="68.58" y1="-45.72" x2="76.2" y2="-45.72" width="0.1524" layer="91"/>
<junction x="76.2" y="-45.72"/>
<pinref part="U$4" gate="G$1" pin="C2"/>
<wire x1="68.58" y1="7.62" x2="76.2" y2="7.62" width="0.1524" layer="91"/>
<junction x="76.2" y="7.62"/>
<pinref part="U$4" gate="G$1" pin="C30"/>
<wire x1="68.58" y1="-63.5" x2="76.2" y2="-63.5" width="0.1524" layer="91"/>
<junction x="76.2" y="-63.5"/>
<pinref part="U$4" gate="G$1" pin="C16"/>
<wire x1="68.58" y1="-27.94" x2="76.2" y2="-27.94" width="0.1524" layer="91"/>
<junction x="76.2" y="-27.94"/>
<pinref part="U$4" gate="G$1" pin="C9"/>
<wire x1="68.58" y1="-10.16" x2="76.2" y2="-10.16" width="0.1524" layer="91"/>
<junction x="76.2" y="-10.16"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="D1"/>
<wire x1="93.98" y1="10.16" x2="101.6" y2="10.16" width="0.1524" layer="91"/>
<wire x1="101.6" y1="10.16" x2="101.6" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY22" gate="GND" pin="GND"/>
<wire x1="101.6" y1="7.62" x2="101.6" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-10.16" x2="101.6" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-27.94" x2="101.6" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-45.72" x2="101.6" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-63.5" x2="101.6" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-66.04" x2="99.06" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="D23"/>
<wire x1="93.98" y1="-45.72" x2="101.6" y2="-45.72" width="0.1524" layer="91"/>
<junction x="101.6" y="-45.72"/>
<pinref part="U$4" gate="G$1" pin="D9"/>
<pinref part="U$4" gate="G$1" pin="D16"/>
<wire x1="93.98" y1="-27.94" x2="101.6" y2="-27.94" width="0.1524" layer="91"/>
<junction x="101.6" y="-27.94"/>
<pinref part="U$4" gate="G$1" pin="D2"/>
<wire x1="93.98" y1="7.62" x2="101.6" y2="7.62" width="0.1524" layer="91"/>
<junction x="101.6" y="7.62"/>
<pinref part="U$4" gate="G$1" pin="D30"/>
<wire x1="93.98" y1="-63.5" x2="101.6" y2="-63.5" width="0.1524" layer="91"/>
<junction x="101.6" y="-63.5"/>
<wire x1="93.98" y1="-10.16" x2="101.6" y2="-10.16" width="0.1524" layer="91"/>
<junction x="101.6" y="-10.16"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="E1"/>
<wire x1="119.38" y1="10.16" x2="127" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY23" gate="GND" pin="GND"/>
<wire x1="127" y1="10.16" x2="127" y2="-63.5" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="E30"/>
<wire x1="127" y1="-63.5" x2="127" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="119.38" y1="-63.5" x2="127" y2="-63.5" width="0.1524" layer="91"/>
<junction x="127" y="-63.5"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="F1"/>
<wire x1="144.78" y1="10.16" x2="152.4" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY24" gate="GND" pin="GND"/>
<wire x1="152.4" y1="10.16" x2="152.4" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="152.4" y1="-63.5" x2="152.4" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="152.4" y1="-66.04" x2="157.48" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$4" gate="G$1" pin="F30"/>
<wire x1="144.78" y1="-63.5" x2="152.4" y2="-63.5" width="0.1524" layer="91"/>
<junction x="152.4" y="-63.5"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="A30"/>
<wire x1="17.78" y1="20.32" x2="35.56" y2="20.32" width="0.1524" layer="91"/>
<pinref part="U$18" gate="G$1" pin="IO"/>
<wire x1="35.56" y1="20.32" x2="35.56" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.09.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B3"/>
<wire x1="43.18" y1="88.9" x2="53.34" y2="88.9" width="0.1016" layer="91"/>
<label x="53.34" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B4"/>
<wire x1="43.18" y1="86.36" x2="53.34" y2="86.36" width="0.1016" layer="91"/>
<label x="53.34" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B5"/>
<wire x1="43.18" y1="83.82" x2="53.34" y2="83.82" width="0.1016" layer="91"/>
<label x="53.34" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.021" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B6"/>
<wire x1="43.18" y1="81.28" x2="53.34" y2="81.28" width="0.1016" layer="91"/>
<label x="53.34" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B7"/>
<wire x1="43.18" y1="78.74" x2="53.34" y2="78.74" width="0.1016" layer="91"/>
<label x="53.34" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B8"/>
<wire x1="43.18" y1="76.2" x2="53.34" y2="76.2" width="0.1016" layer="91"/>
<label x="53.34" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B10"/>
<wire x1="43.18" y1="71.12" x2="53.34" y2="71.12" width="0.1016" layer="91"/>
<label x="53.34" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.021" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B11"/>
<wire x1="43.18" y1="68.58" x2="53.34" y2="68.58" width="0.1016" layer="91"/>
<label x="53.34" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B12"/>
<wire x1="43.18" y1="66.04" x2="53.34" y2="66.04" width="0.1016" layer="91"/>
<label x="53.34" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B13"/>
<wire x1="43.18" y1="63.5" x2="53.34" y2="63.5" width="0.1016" layer="91"/>
<label x="53.34" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B14"/>
<wire x1="43.18" y1="60.96" x2="53.34" y2="60.96" width="0.1016" layer="91"/>
<label x="53.34" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B15"/>
<wire x1="43.18" y1="58.42" x2="53.34" y2="58.42" width="0.1016" layer="91"/>
<label x="53.34" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B17"/>
<wire x1="43.18" y1="53.34" x2="53.34" y2="53.34" width="0.1016" layer="91"/>
<label x="53.34" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.011" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B18"/>
<wire x1="43.18" y1="50.8" x2="53.34" y2="50.8" width="0.1016" layer="91"/>
<label x="53.34" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B19"/>
<wire x1="43.18" y1="48.26" x2="53.34" y2="48.26" width="0.1016" layer="91"/>
<label x="53.34" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.012" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B20"/>
<wire x1="43.18" y1="45.72" x2="53.34" y2="45.72" width="0.1016" layer="91"/>
<label x="53.34" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B21"/>
<wire x1="43.18" y1="43.18" x2="53.34" y2="43.18" width="0.1016" layer="91"/>
<label x="53.34" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.013" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B22"/>
<wire x1="43.18" y1="40.64" x2="53.34" y2="40.64" width="0.1016" layer="91"/>
<label x="53.34" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B24"/>
<wire x1="43.18" y1="35.56" x2="53.34" y2="35.56" width="0.1016" layer="91"/>
<label x="53.34" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.021" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B25"/>
<wire x1="43.18" y1="33.02" x2="53.34" y2="33.02" width="0.1016" layer="91"/>
<label x="53.34" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B26"/>
<wire x1="43.18" y1="30.48" x2="53.34" y2="30.48" width="0.1016" layer="91"/>
<label x="53.34" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B27"/>
<wire x1="43.18" y1="27.94" x2="53.34" y2="27.94" width="0.1016" layer="91"/>
<label x="53.34" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.021" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B28"/>
<wire x1="43.18" y1="25.4" x2="53.34" y2="25.4" width="0.1016" layer="91"/>
<label x="53.34" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="B29"/>
<wire x1="43.18" y1="22.86" x2="53.34" y2="22.86" width="0.1016" layer="91"/>
<label x="53.34" y="22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C3"/>
<wire x1="68.58" y1="88.9" x2="78.74" y2="88.9" width="0.1016" layer="91"/>
<label x="78.74" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C4"/>
<wire x1="68.58" y1="86.36" x2="78.74" y2="86.36" width="0.1016" layer="91"/>
<label x="78.74" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C5"/>
<wire x1="68.58" y1="83.82" x2="78.74" y2="83.82" width="0.1016" layer="91"/>
<label x="78.74" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.021" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C6"/>
<wire x1="68.58" y1="81.28" x2="78.74" y2="81.28" width="0.1016" layer="91"/>
<label x="78.74" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.019" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C7"/>
<wire x1="68.58" y1="78.74" x2="78.74" y2="78.74" width="0.1016" layer="91"/>
<label x="78.74" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C8"/>
<wire x1="68.58" y1="76.2" x2="78.74" y2="76.2" width="0.1016" layer="91"/>
<label x="78.74" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C10"/>
<wire x1="68.58" y1="71.12" x2="78.74" y2="71.12" width="0.1016" layer="91"/>
<label x="78.74" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C11"/>
<wire x1="68.58" y1="68.58" x2="78.74" y2="68.58" width="0.1016" layer="91"/>
<label x="78.74" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C12"/>
<wire x1="68.58" y1="66.04" x2="78.74" y2="66.04" width="0.1016" layer="91"/>
<label x="78.74" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C13"/>
<wire x1="68.58" y1="63.5" x2="78.74" y2="63.5" width="0.1016" layer="91"/>
<label x="78.74" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C14"/>
<wire x1="68.58" y1="60.96" x2="78.74" y2="60.96" width="0.1016" layer="91"/>
<label x="78.74" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C15"/>
<wire x1="68.58" y1="58.42" x2="78.74" y2="58.42" width="0.1016" layer="91"/>
<label x="78.74" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C17"/>
<wire x1="68.58" y1="53.34" x2="78.74" y2="53.34" width="0.1016" layer="91"/>
<label x="78.74" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.016" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C18"/>
<wire x1="68.58" y1="50.8" x2="78.74" y2="50.8" width="0.1016" layer="91"/>
<label x="78.74" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C19"/>
<wire x1="68.58" y1="48.26" x2="78.74" y2="48.26" width="0.1016" layer="91"/>
<label x="78.74" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C20"/>
<wire x1="68.58" y1="45.72" x2="78.74" y2="45.72" width="0.1016" layer="91"/>
<label x="78.74" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C21"/>
<wire x1="68.58" y1="43.18" x2="78.74" y2="43.18" width="0.1016" layer="91"/>
<label x="78.74" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C22"/>
<wire x1="68.58" y1="40.64" x2="78.74" y2="40.64" width="0.1016" layer="91"/>
<label x="78.74" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C24"/>
<wire x1="68.58" y1="35.56" x2="78.74" y2="35.56" width="0.1016" layer="91"/>
<label x="78.74" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C25"/>
<wire x1="68.58" y1="33.02" x2="78.74" y2="33.02" width="0.1016" layer="91"/>
<label x="78.74" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C26"/>
<wire x1="68.58" y1="30.48" x2="78.74" y2="30.48" width="0.1016" layer="91"/>
<label x="78.74" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C27"/>
<wire x1="68.58" y1="27.94" x2="78.74" y2="27.94" width="0.1016" layer="91"/>
<label x="78.74" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C28"/>
<wire x1="68.58" y1="25.4" x2="78.74" y2="25.4" width="0.1016" layer="91"/>
<label x="78.74" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="C29"/>
<wire x1="68.58" y1="22.86" x2="78.74" y2="22.86" width="0.1016" layer="91"/>
<label x="78.74" y="22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.027" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D3"/>
<wire x1="93.98" y1="88.9" x2="104.14" y2="88.9" width="0.1016" layer="91"/>
<label x="104.14" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.027" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D4"/>
<wire x1="93.98" y1="86.36" x2="104.14" y2="86.36" width="0.1016" layer="91"/>
<label x="104.14" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D5"/>
<wire x1="93.98" y1="83.82" x2="104.14" y2="83.82" width="0.1016" layer="91"/>
<label x="104.14" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D6"/>
<wire x1="93.98" y1="81.28" x2="104.14" y2="81.28" width="0.1016" layer="91"/>
<label x="104.14" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.019" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D7"/>
<wire x1="93.98" y1="78.74" x2="104.14" y2="78.74" width="0.1016" layer="91"/>
<label x="104.14" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D8"/>
<wire x1="93.98" y1="76.2" x2="104.14" y2="76.2" width="0.1016" layer="91"/>
<label x="104.14" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D10"/>
<wire x1="93.98" y1="71.12" x2="104.14" y2="71.12" width="0.1016" layer="91"/>
<label x="104.14" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D11"/>
<wire x1="93.98" y1="68.58" x2="104.14" y2="68.58" width="0.1016" layer="91"/>
<label x="104.14" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D12"/>
<wire x1="93.98" y1="66.04" x2="104.14" y2="66.04" width="0.1016" layer="91"/>
<label x="104.14" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D13"/>
<wire x1="93.98" y1="63.5" x2="104.14" y2="63.5" width="0.1016" layer="91"/>
<label x="104.14" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D14"/>
<wire x1="93.98" y1="60.96" x2="104.14" y2="60.96" width="0.1016" layer="91"/>
<label x="104.14" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.027" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D15"/>
<wire x1="93.98" y1="58.42" x2="104.14" y2="58.42" width="0.1016" layer="91"/>
<label x="104.14" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.011" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D17"/>
<wire x1="93.98" y1="53.34" x2="104.14" y2="53.34" width="0.1016" layer="91"/>
<label x="104.14" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D18"/>
<wire x1="93.98" y1="50.8" x2="104.14" y2="50.8" width="0.1016" layer="91"/>
<label x="104.14" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.012" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D19"/>
<wire x1="93.98" y1="48.26" x2="104.14" y2="48.26" width="0.1016" layer="91"/>
<label x="104.14" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D20"/>
<wire x1="93.98" y1="45.72" x2="104.14" y2="45.72" width="0.1016" layer="91"/>
<label x="104.14" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.013" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D21"/>
<wire x1="93.98" y1="43.18" x2="104.14" y2="43.18" width="0.1016" layer="91"/>
<label x="104.14" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.024" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D22"/>
<wire x1="93.98" y1="40.64" x2="104.14" y2="40.64" width="0.1016" layer="91"/>
<label x="104.14" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D24"/>
<wire x1="93.98" y1="35.56" x2="104.14" y2="35.56" width="0.1016" layer="91"/>
<label x="104.14" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D25"/>
<wire x1="93.98" y1="33.02" x2="104.14" y2="33.02" width="0.1016" layer="91"/>
<label x="104.14" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D26"/>
<wire x1="93.98" y1="30.48" x2="104.14" y2="30.48" width="0.1016" layer="91"/>
<label x="104.14" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D27"/>
<wire x1="93.98" y1="27.94" x2="104.14" y2="27.94" width="0.1016" layer="91"/>
<label x="104.14" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D28"/>
<wire x1="93.98" y1="25.4" x2="104.14" y2="25.4" width="0.1016" layer="91"/>
<label x="104.14" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="D29"/>
<wire x1="93.98" y1="22.86" x2="104.14" y2="22.86" width="0.1016" layer="91"/>
<label x="104.14" y="22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.028" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E3"/>
<wire x1="119.38" y1="88.9" x2="129.54" y2="88.9" width="0.1016" layer="91"/>
<label x="129.54" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.028" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E4"/>
<wire x1="119.38" y1="86.36" x2="129.54" y2="86.36" width="0.1016" layer="91"/>
<label x="129.54" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E5"/>
<wire x1="119.38" y1="83.82" x2="129.54" y2="83.82" width="0.1016" layer="91"/>
<label x="129.54" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.022" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E6"/>
<wire x1="119.38" y1="81.28" x2="129.54" y2="81.28" width="0.1016" layer="91"/>
<label x="129.54" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.020" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E7"/>
<wire x1="119.38" y1="78.74" x2="129.54" y2="78.74" width="0.1016" layer="91"/>
<label x="129.54" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.017" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E8"/>
<wire x1="119.38" y1="76.2" x2="129.54" y2="76.2" width="0.1016" layer="91"/>
<label x="129.54" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.027" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E10"/>
<wire x1="119.38" y1="71.12" x2="129.54" y2="71.12" width="0.1016" layer="91"/>
<label x="129.54" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E11"/>
<wire x1="119.38" y1="68.58" x2="129.54" y2="68.58" width="0.1016" layer="91"/>
<label x="129.54" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.019" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E12"/>
<wire x1="119.38" y1="66.04" x2="129.54" y2="66.04" width="0.1016" layer="91"/>
<label x="129.54" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.019" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E13"/>
<wire x1="119.38" y1="63.5" x2="129.54" y2="63.5" width="0.1016" layer="91"/>
<label x="129.54" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E14"/>
<wire x1="119.38" y1="60.96" x2="129.54" y2="60.96" width="0.1016" layer="91"/>
<label x="129.54" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.028" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E15"/>
<wire x1="119.38" y1="58.42" x2="129.54" y2="58.42" width="0.1016" layer="91"/>
<label x="129.54" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E17"/>
<wire x1="119.38" y1="53.34" x2="129.54" y2="53.34" width="0.1016" layer="91"/>
<label x="129.54" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E18"/>
<wire x1="119.38" y1="50.8" x2="129.54" y2="50.8" width="0.1016" layer="91"/>
<label x="129.54" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E19"/>
<wire x1="119.38" y1="48.26" x2="129.54" y2="48.26" width="0.1016" layer="91"/>
<label x="129.54" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.032" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E20"/>
<wire x1="119.38" y1="45.72" x2="129.54" y2="45.72" width="0.1016" layer="91"/>
<label x="129.54" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E21"/>
<wire x1="119.38" y1="43.18" x2="129.54" y2="43.18" width="0.1016" layer="91"/>
<label x="129.54" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E22"/>
<wire x1="119.38" y1="40.64" x2="129.54" y2="40.64" width="0.1016" layer="91"/>
<label x="129.54" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E24"/>
<wire x1="119.38" y1="35.56" x2="129.54" y2="35.56" width="0.1016" layer="91"/>
<label x="129.54" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.020" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E25"/>
<wire x1="119.38" y1="33.02" x2="129.54" y2="33.02" width="0.1016" layer="91"/>
<label x="129.54" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E26"/>
<wire x1="119.38" y1="30.48" x2="129.54" y2="30.48" width="0.1016" layer="91"/>
<label x="129.54" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.027" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E27"/>
<wire x1="119.38" y1="27.94" x2="129.54" y2="27.94" width="0.1016" layer="91"/>
<label x="129.54" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.019" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E28"/>
<wire x1="119.38" y1="25.4" x2="129.54" y2="25.4" width="0.1016" layer="91"/>
<label x="129.54" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E29"/>
<wire x1="119.38" y1="22.86" x2="129.54" y2="22.86" width="0.1016" layer="91"/>
<label x="129.54" y="22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.029" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F3"/>
<wire x1="144.78" y1="88.9" x2="154.94" y2="88.9" width="0.1016" layer="91"/>
<label x="154.94" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.029" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F4"/>
<wire x1="144.78" y1="86.36" x2="154.94" y2="86.36" width="0.1016" layer="91"/>
<label x="154.94" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F5"/>
<wire x1="144.78" y1="83.82" x2="154.94" y2="83.82" width="0.1016" layer="91"/>
<label x="154.94" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.023" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F6"/>
<wire x1="144.78" y1="81.28" x2="154.94" y2="81.28" width="0.1016" layer="91"/>
<label x="154.94" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.020" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F7"/>
<wire x1="144.78" y1="78.74" x2="154.94" y2="78.74" width="0.1016" layer="91"/>
<label x="154.94" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.018" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F8"/>
<wire x1="144.78" y1="76.2" x2="154.94" y2="76.2" width="0.1016" layer="91"/>
<label x="154.94" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.028" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F10"/>
<wire x1="144.78" y1="71.12" x2="154.94" y2="71.12" width="0.1016" layer="91"/>
<label x="154.94" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F11"/>
<wire x1="144.78" y1="68.58" x2="154.94" y2="68.58" width="0.1016" layer="91"/>
<label x="154.94" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.020" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F12"/>
<wire x1="144.78" y1="66.04" x2="154.94" y2="66.04" width="0.1016" layer="91"/>
<label x="154.94" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.020" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F13"/>
<wire x1="144.78" y1="63.5" x2="154.94" y2="63.5" width="0.1016" layer="91"/>
<label x="154.94" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F14"/>
<wire x1="144.78" y1="60.96" x2="154.94" y2="60.96" width="0.1016" layer="91"/>
<label x="154.94" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.029" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F15"/>
<wire x1="144.78" y1="58.42" x2="154.94" y2="58.42" width="0.1016" layer="91"/>
<label x="154.94" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.034" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F17"/>
<wire x1="144.78" y1="53.34" x2="154.94" y2="53.34" width="0.1016" layer="91"/>
<label x="154.94" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.026" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F18"/>
<wire x1="144.78" y1="50.8" x2="154.94" y2="50.8" width="0.1016" layer="91"/>
<label x="154.94" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.033" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F19"/>
<wire x1="144.78" y1="48.26" x2="154.94" y2="48.26" width="0.1016" layer="91"/>
<label x="154.94" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F20"/>
<wire x1="144.78" y1="45.72" x2="154.94" y2="45.72" width="0.1016" layer="91"/>
<label x="154.94" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.025" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F21"/>
<wire x1="144.78" y1="43.18" x2="154.94" y2="43.18" width="0.1016" layer="91"/>
<label x="154.94" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.032" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F22"/>
<wire x1="144.78" y1="40.64" x2="154.94" y2="40.64" width="0.1016" layer="91"/>
<label x="154.94" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.029" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F24"/>
<wire x1="144.78" y1="35.56" x2="154.94" y2="35.56" width="0.1016" layer="91"/>
<label x="154.94" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.020" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F25"/>
<wire x1="144.78" y1="33.02" x2="154.94" y2="33.02" width="0.1016" layer="91"/>
<label x="154.94" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.028" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F26"/>
<wire x1="144.78" y1="30.48" x2="154.94" y2="30.48" width="0.1016" layer="91"/>
<label x="154.94" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F27"/>
<wire x1="144.78" y1="27.94" x2="154.94" y2="27.94" width="0.1016" layer="91"/>
<label x="154.94" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.019" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F28"/>
<wire x1="144.78" y1="25.4" x2="154.94" y2="25.4" width="0.1016" layer="91"/>
<label x="154.94" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.027" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F29"/>
<wire x1="144.78" y1="22.86" x2="154.94" y2="22.86" width="0.1016" layer="91"/>
<label x="154.94" y="22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.030" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E2"/>
<wire x1="119.38" y1="91.44" x2="129.54" y2="91.44" width="0.1016" layer="91"/>
<label x="129.54" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.031" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F2"/>
<wire x1="144.78" y1="91.44" x2="154.94" y2="91.44" width="0.1016" layer="91"/>
<label x="154.94" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.029" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E9"/>
<wire x1="119.38" y1="73.66" x2="129.54" y2="73.66" width="0.1016" layer="91"/>
<label x="129.54" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.021" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F9"/>
<wire x1="144.78" y1="73.66" x2="154.94" y2="73.66" width="0.1016" layer="91"/>
<label x="154.94" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.034" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E16"/>
<wire x1="119.38" y1="55.88" x2="129.54" y2="55.88" width="0.1016" layer="91"/>
<label x="129.54" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.033" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F16"/>
<wire x1="144.78" y1="55.88" x2="154.94" y2="55.88" width="0.1016" layer="91"/>
<label x="154.94" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.029" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="E23"/>
<wire x1="119.38" y1="38.1" x2="129.54" y2="38.1" width="0.1016" layer="91"/>
<label x="129.54" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.028" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="F23"/>
<wire x1="144.78" y1="38.1" x2="154.94" y2="38.1" width="0.1016" layer="91"/>
<label x="154.94" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B3"/>
<wire x1="43.18" y1="5.08" x2="53.34" y2="5.08" width="0.1016" layer="91"/>
<label x="53.34" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B4"/>
<wire x1="43.18" y1="2.54" x2="53.34" y2="2.54" width="0.1016" layer="91"/>
<label x="53.34" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B5"/>
<wire x1="43.18" y1="0" x2="53.34" y2="0" width="0.1016" layer="91"/>
<label x="53.34" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B6"/>
<wire x1="43.18" y1="-2.54" x2="53.34" y2="-2.54" width="0.1016" layer="91"/>
<label x="53.34" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B7"/>
<wire x1="43.18" y1="-5.08" x2="53.34" y2="-5.08" width="0.1016" layer="91"/>
<label x="53.34" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B8"/>
<wire x1="43.18" y1="-7.62" x2="53.34" y2="-7.62" width="0.1016" layer="91"/>
<label x="53.34" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B10"/>
<wire x1="43.18" y1="-12.7" x2="53.34" y2="-12.7" width="0.1016" layer="91"/>
<label x="53.34" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B11"/>
<wire x1="43.18" y1="-15.24" x2="53.34" y2="-15.24" width="0.1016" layer="91"/>
<label x="53.34" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B12"/>
<wire x1="43.18" y1="-17.78" x2="53.34" y2="-17.78" width="0.1016" layer="91"/>
<label x="53.34" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B13"/>
<wire x1="43.18" y1="-20.32" x2="53.34" y2="-20.32" width="0.1016" layer="91"/>
<label x="53.34" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B14"/>
<wire x1="43.18" y1="-22.86" x2="53.34" y2="-22.86" width="0.1016" layer="91"/>
<label x="53.34" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B15"/>
<wire x1="43.18" y1="-25.4" x2="53.34" y2="-25.4" width="0.1016" layer="91"/>
<label x="53.34" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B17"/>
<wire x1="43.18" y1="-30.48" x2="53.34" y2="-30.48" width="0.1016" layer="91"/>
<label x="53.34" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B18"/>
<wire x1="43.18" y1="-33.02" x2="53.34" y2="-33.02" width="0.1016" layer="91"/>
<label x="53.34" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B19"/>
<wire x1="43.18" y1="-35.56" x2="53.34" y2="-35.56" width="0.1016" layer="91"/>
<label x="53.34" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B20"/>
<wire x1="43.18" y1="-38.1" x2="53.34" y2="-38.1" width="0.1016" layer="91"/>
<label x="53.34" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B21"/>
<wire x1="43.18" y1="-40.64" x2="53.34" y2="-40.64" width="0.1016" layer="91"/>
<label x="53.34" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B22"/>
<wire x1="43.18" y1="-43.18" x2="53.34" y2="-43.18" width="0.1016" layer="91"/>
<label x="53.34" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B24"/>
<wire x1="43.18" y1="-48.26" x2="53.34" y2="-48.26" width="0.1016" layer="91"/>
<label x="53.34" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B25"/>
<wire x1="43.18" y1="-50.8" x2="53.34" y2="-50.8" width="0.1016" layer="91"/>
<label x="53.34" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B26"/>
<wire x1="43.18" y1="-53.34" x2="53.34" y2="-53.34" width="0.1016" layer="91"/>
<label x="53.34" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B27"/>
<wire x1="43.18" y1="-55.88" x2="53.34" y2="-55.88" width="0.1016" layer="91"/>
<label x="53.34" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B28"/>
<wire x1="43.18" y1="-58.42" x2="53.34" y2="-58.42" width="0.1016" layer="91"/>
<label x="53.34" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="B29"/>
<wire x1="43.18" y1="-60.96" x2="53.34" y2="-60.96" width="0.1016" layer="91"/>
<label x="53.34" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C3"/>
<wire x1="68.58" y1="5.08" x2="78.74" y2="5.08" width="0.1016" layer="91"/>
<label x="78.74" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C4"/>
<wire x1="68.58" y1="2.54" x2="78.74" y2="2.54" width="0.1016" layer="91"/>
<label x="78.74" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C5"/>
<wire x1="68.58" y1="0" x2="78.74" y2="0" width="0.1016" layer="91"/>
<label x="78.74" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C6"/>
<wire x1="68.58" y1="-2.54" x2="78.74" y2="-2.54" width="0.1016" layer="91"/>
<label x="78.74" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C7"/>
<wire x1="68.58" y1="-5.08" x2="78.74" y2="-5.08" width="0.1016" layer="91"/>
<label x="78.74" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C8"/>
<wire x1="68.58" y1="-7.62" x2="78.74" y2="-7.62" width="0.1016" layer="91"/>
<label x="78.74" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C10"/>
<wire x1="68.58" y1="-12.7" x2="78.74" y2="-12.7" width="0.1016" layer="91"/>
<label x="78.74" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C11"/>
<wire x1="68.58" y1="-15.24" x2="78.74" y2="-15.24" width="0.1016" layer="91"/>
<label x="78.74" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C12"/>
<wire x1="68.58" y1="-17.78" x2="78.74" y2="-17.78" width="0.1016" layer="91"/>
<label x="78.74" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C13"/>
<wire x1="68.58" y1="-20.32" x2="78.74" y2="-20.32" width="0.1016" layer="91"/>
<label x="78.74" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C14"/>
<wire x1="68.58" y1="-22.86" x2="78.74" y2="-22.86" width="0.1016" layer="91"/>
<label x="78.74" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.016" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C15"/>
<wire x1="68.58" y1="-25.4" x2="78.74" y2="-25.4" width="0.1016" layer="91"/>
<label x="78.74" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C17"/>
<wire x1="68.58" y1="-30.48" x2="78.74" y2="-30.48" width="0.1016" layer="91"/>
<label x="78.74" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C18"/>
<wire x1="68.58" y1="-33.02" x2="78.74" y2="-33.02" width="0.1016" layer="91"/>
<label x="78.74" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C19"/>
<wire x1="68.58" y1="-35.56" x2="78.74" y2="-35.56" width="0.1016" layer="91"/>
<label x="78.74" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C20"/>
<wire x1="68.58" y1="-38.1" x2="78.74" y2="-38.1" width="0.1016" layer="91"/>
<label x="78.74" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C21"/>
<wire x1="68.58" y1="-40.64" x2="78.74" y2="-40.64" width="0.1016" layer="91"/>
<label x="78.74" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C22"/>
<wire x1="68.58" y1="-43.18" x2="78.74" y2="-43.18" width="0.1016" layer="91"/>
<label x="78.74" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C24"/>
<wire x1="68.58" y1="-48.26" x2="78.74" y2="-48.26" width="0.1016" layer="91"/>
<label x="78.74" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C25"/>
<wire x1="68.58" y1="-50.8" x2="78.74" y2="-50.8" width="0.1016" layer="91"/>
<label x="78.74" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C26"/>
<wire x1="68.58" y1="-53.34" x2="78.74" y2="-53.34" width="0.1016" layer="91"/>
<label x="78.74" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C27"/>
<wire x1="68.58" y1="-55.88" x2="78.74" y2="-55.88" width="0.1016" layer="91"/>
<label x="78.74" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C28"/>
<wire x1="68.58" y1="-58.42" x2="78.74" y2="-58.42" width="0.1016" layer="91"/>
<label x="78.74" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.026" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="C29"/>
<wire x1="68.58" y1="-60.96" x2="78.74" y2="-60.96" width="0.1016" layer="91"/>
<label x="78.74" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D3"/>
<wire x1="93.98" y1="5.08" x2="104.14" y2="5.08" width="0.1016" layer="91"/>
<label x="104.14" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D4"/>
<wire x1="93.98" y1="2.54" x2="104.14" y2="2.54" width="0.1016" layer="91"/>
<label x="104.14" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D5"/>
<wire x1="93.98" y1="0" x2="104.14" y2="0" width="0.1016" layer="91"/>
<label x="104.14" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D6"/>
<wire x1="93.98" y1="-2.54" x2="104.14" y2="-2.54" width="0.1016" layer="91"/>
<label x="104.14" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D7"/>
<wire x1="93.98" y1="-5.08" x2="104.14" y2="-5.08" width="0.1016" layer="91"/>
<label x="104.14" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D8"/>
<wire x1="93.98" y1="-7.62" x2="104.14" y2="-7.62" width="0.1016" layer="91"/>
<label x="104.14" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D10"/>
<wire x1="93.98" y1="-12.7" x2="104.14" y2="-12.7" width="0.1016" layer="91"/>
<label x="104.14" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D11"/>
<wire x1="93.98" y1="-15.24" x2="104.14" y2="-15.24" width="0.1016" layer="91"/>
<label x="104.14" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D12"/>
<wire x1="93.98" y1="-17.78" x2="104.14" y2="-17.78" width="0.1016" layer="91"/>
<label x="104.14" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D13"/>
<wire x1="93.98" y1="-20.32" x2="104.14" y2="-20.32" width="0.1016" layer="91"/>
<label x="104.14" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D14"/>
<wire x1="93.98" y1="-22.86" x2="104.14" y2="-22.86" width="0.1016" layer="91"/>
<label x="104.14" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D15"/>
<wire x1="93.98" y1="-25.4" x2="104.14" y2="-25.4" width="0.1016" layer="91"/>
<label x="104.14" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D17"/>
<wire x1="93.98" y1="-30.48" x2="104.14" y2="-30.48" width="0.1016" layer="91"/>
<label x="104.14" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D18"/>
<wire x1="93.98" y1="-33.02" x2="104.14" y2="-33.02" width="0.1016" layer="91"/>
<label x="104.14" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D19"/>
<wire x1="93.98" y1="-35.56" x2="104.14" y2="-35.56" width="0.1016" layer="91"/>
<label x="104.14" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D20"/>
<wire x1="93.98" y1="-38.1" x2="104.14" y2="-38.1" width="0.1016" layer="91"/>
<label x="104.14" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D21"/>
<wire x1="93.98" y1="-40.64" x2="104.14" y2="-40.64" width="0.1016" layer="91"/>
<label x="104.14" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D22"/>
<wire x1="93.98" y1="-43.18" x2="104.14" y2="-43.18" width="0.1016" layer="91"/>
<label x="104.14" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D24"/>
<wire x1="93.98" y1="-48.26" x2="104.14" y2="-48.26" width="0.1016" layer="91"/>
<label x="104.14" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D25"/>
<wire x1="93.98" y1="-50.8" x2="104.14" y2="-50.8" width="0.1016" layer="91"/>
<label x="104.14" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D26"/>
<wire x1="93.98" y1="-53.34" x2="104.14" y2="-53.34" width="0.1016" layer="91"/>
<label x="104.14" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D27"/>
<wire x1="93.98" y1="-55.88" x2="104.14" y2="-55.88" width="0.1016" layer="91"/>
<label x="104.14" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D28"/>
<wire x1="93.98" y1="-58.42" x2="104.14" y2="-58.42" width="0.1016" layer="91"/>
<label x="104.14" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.027" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="D29"/>
<wire x1="93.98" y1="-60.96" x2="104.14" y2="-60.96" width="0.1016" layer="91"/>
<label x="104.14" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E3"/>
<wire x1="119.38" y1="5.08" x2="129.54" y2="5.08" width="0.1016" layer="91"/>
<label x="129.54" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E4"/>
<wire x1="119.38" y1="2.54" x2="129.54" y2="2.54" width="0.1016" layer="91"/>
<label x="129.54" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E5"/>
<wire x1="119.38" y1="0" x2="129.54" y2="0" width="0.1016" layer="91"/>
<label x="129.54" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E6"/>
<wire x1="119.38" y1="-2.54" x2="129.54" y2="-2.54" width="0.1016" layer="91"/>
<label x="129.54" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E7"/>
<wire x1="119.38" y1="-5.08" x2="129.54" y2="-5.08" width="0.1016" layer="91"/>
<label x="129.54" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E8"/>
<wire x1="119.38" y1="-7.62" x2="129.54" y2="-7.62" width="0.1016" layer="91"/>
<label x="129.54" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E10"/>
<wire x1="119.38" y1="-12.7" x2="129.54" y2="-12.7" width="0.1016" layer="91"/>
<label x="129.54" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E11"/>
<wire x1="119.38" y1="-15.24" x2="129.54" y2="-15.24" width="0.1016" layer="91"/>
<label x="129.54" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E12"/>
<wire x1="119.38" y1="-17.78" x2="129.54" y2="-17.78" width="0.1016" layer="91"/>
<label x="129.54" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.022" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E13"/>
<wire x1="119.38" y1="-20.32" x2="129.54" y2="-20.32" width="0.1016" layer="91"/>
<label x="129.54" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E14"/>
<wire x1="119.38" y1="-22.86" x2="129.54" y2="-22.86" width="0.1016" layer="91"/>
<label x="129.54" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.017" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E15"/>
<wire x1="119.38" y1="-25.4" x2="129.54" y2="-25.4" width="0.1016" layer="91"/>
<label x="129.54" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E17"/>
<wire x1="119.38" y1="-30.48" x2="129.54" y2="-30.48" width="0.1016" layer="91"/>
<label x="129.54" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E18"/>
<wire x1="119.38" y1="-33.02" x2="129.54" y2="-33.02" width="0.1016" layer="91"/>
<label x="129.54" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E19"/>
<wire x1="119.38" y1="-35.56" x2="129.54" y2="-35.56" width="0.1016" layer="91"/>
<label x="129.54" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E20"/>
<wire x1="119.38" y1="-38.1" x2="129.54" y2="-38.1" width="0.1016" layer="91"/>
<label x="129.54" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E21"/>
<wire x1="119.38" y1="-40.64" x2="129.54" y2="-40.64" width="0.1016" layer="91"/>
<label x="129.54" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E22"/>
<wire x1="119.38" y1="-43.18" x2="129.54" y2="-43.18" width="0.1016" layer="91"/>
<label x="129.54" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E24"/>
<wire x1="119.38" y1="-48.26" x2="129.54" y2="-48.26" width="0.1016" layer="91"/>
<label x="129.54" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E25"/>
<wire x1="119.38" y1="-50.8" x2="129.54" y2="-50.8" width="0.1016" layer="91"/>
<label x="129.54" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E26"/>
<wire x1="119.38" y1="-53.34" x2="129.54" y2="-53.34" width="0.1016" layer="91"/>
<label x="129.54" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E27"/>
<wire x1="119.38" y1="-55.88" x2="129.54" y2="-55.88" width="0.1016" layer="91"/>
<label x="129.54" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E28"/>
<wire x1="119.38" y1="-58.42" x2="129.54" y2="-58.42" width="0.1016" layer="91"/>
<label x="129.54" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.024" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E29"/>
<wire x1="119.38" y1="-60.96" x2="129.54" y2="-60.96" width="0.1016" layer="91"/>
<label x="129.54" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F3"/>
<wire x1="144.78" y1="5.08" x2="154.94" y2="5.08" width="0.1016" layer="91"/>
<label x="154.94" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F4"/>
<wire x1="144.78" y1="2.54" x2="154.94" y2="2.54" width="0.1016" layer="91"/>
<label x="154.94" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F5"/>
<wire x1="144.78" y1="0" x2="154.94" y2="0" width="0.1016" layer="91"/>
<label x="154.94" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F6"/>
<wire x1="144.78" y1="-2.54" x2="154.94" y2="-2.54" width="0.1016" layer="91"/>
<label x="154.94" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F7"/>
<wire x1="144.78" y1="-5.08" x2="154.94" y2="-5.08" width="0.1016" layer="91"/>
<label x="154.94" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F8"/>
<wire x1="144.78" y1="-7.62" x2="154.94" y2="-7.62" width="0.1016" layer="91"/>
<label x="154.94" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F10"/>
<wire x1="144.78" y1="-12.7" x2="154.94" y2="-12.7" width="0.1016" layer="91"/>
<label x="154.94" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F11"/>
<wire x1="144.78" y1="-15.24" x2="154.94" y2="-15.24" width="0.1016" layer="91"/>
<label x="154.94" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F12"/>
<wire x1="144.78" y1="-17.78" x2="154.94" y2="-17.78" width="0.1016" layer="91"/>
<label x="154.94" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F13"/>
<wire x1="144.78" y1="-20.32" x2="154.94" y2="-20.32" width="0.1016" layer="91"/>
<label x="154.94" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F14"/>
<wire x1="144.78" y1="-22.86" x2="154.94" y2="-22.86" width="0.1016" layer="91"/>
<label x="154.94" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.018" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F15"/>
<wire x1="144.78" y1="-25.4" x2="154.94" y2="-25.4" width="0.1016" layer="91"/>
<label x="154.94" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F17"/>
<wire x1="144.78" y1="-30.48" x2="154.94" y2="-30.48" width="0.1016" layer="91"/>
<label x="154.94" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F18"/>
<wire x1="144.78" y1="-33.02" x2="154.94" y2="-33.02" width="0.1016" layer="91"/>
<label x="154.94" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F19"/>
<wire x1="144.78" y1="-35.56" x2="154.94" y2="-35.56" width="0.1016" layer="91"/>
<label x="154.94" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F20"/>
<wire x1="144.78" y1="-38.1" x2="154.94" y2="-38.1" width="0.1016" layer="91"/>
<label x="154.94" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F21"/>
<wire x1="144.78" y1="-40.64" x2="154.94" y2="-40.64" width="0.1016" layer="91"/>
<label x="154.94" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F22"/>
<wire x1="144.78" y1="-43.18" x2="154.94" y2="-43.18" width="0.1016" layer="91"/>
<label x="154.94" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F24"/>
<wire x1="144.78" y1="-48.26" x2="154.94" y2="-48.26" width="0.1016" layer="91"/>
<label x="154.94" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F25"/>
<wire x1="144.78" y1="-50.8" x2="154.94" y2="-50.8" width="0.1016" layer="91"/>
<label x="154.94" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.020" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F26"/>
<wire x1="144.78" y1="-53.34" x2="154.94" y2="-53.34" width="0.1016" layer="91"/>
<label x="154.94" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.025" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F27"/>
<wire x1="144.78" y1="-55.88" x2="154.94" y2="-55.88" width="0.1016" layer="91"/>
<label x="154.94" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.019" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F28"/>
<wire x1="144.78" y1="-58.42" x2="154.94" y2="-58.42" width="0.1016" layer="91"/>
<label x="154.94" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.028" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F29"/>
<wire x1="144.78" y1="-60.96" x2="154.94" y2="-60.96" width="0.1016" layer="91"/>
<label x="154.94" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E2"/>
<wire x1="119.38" y1="7.62" x2="129.54" y2="7.62" width="0.1016" layer="91"/>
<label x="129.54" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F2"/>
<wire x1="144.78" y1="7.62" x2="154.94" y2="7.62" width="0.1016" layer="91"/>
<label x="154.94" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.031" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E9"/>
<wire x1="119.38" y1="-10.16" x2="129.54" y2="-10.16" width="0.1016" layer="91"/>
<label x="129.54" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.023" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F9"/>
<wire x1="144.78" y1="-10.16" x2="154.94" y2="-10.16" width="0.1016" layer="91"/>
<label x="154.94" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E16"/>
<wire x1="119.38" y1="-27.94" x2="129.54" y2="-27.94" width="0.1016" layer="91"/>
<label x="129.54" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.021" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F16"/>
<wire x1="144.78" y1="-27.94" x2="154.94" y2="-27.94" width="0.1016" layer="91"/>
<label x="154.94" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.030" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="E23"/>
<wire x1="119.38" y1="-45.72" x2="129.54" y2="-45.72" width="0.1016" layer="91"/>
<label x="129.54" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.029" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="F23"/>
<wire x1="144.78" y1="-45.72" x2="154.94" y2="-45.72" width="0.1016" layer="91"/>
<label x="154.94" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$5" gate="G$1" x="58.42" y="68.58" smashed="yes"/>
<instance part="U$6" gate="G$1" x="58.42" y="-22.86" smashed="yes"/>
<instance part="SUPPLY25" gate="GND" x="20.32" y="15.24" smashed="yes">
<attribute name="VALUE" x="18.415" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY26" gate="GND" x="48.26" y="15.24" smashed="yes">
<attribute name="VALUE" x="46.355" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY27" gate="GND" x="73.66" y="15.24" smashed="yes">
<attribute name="VALUE" x="71.755" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY28" gate="GND" x="96.52" y="17.78" smashed="yes">
<attribute name="VALUE" x="94.615" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY29" gate="GND" x="119.38" y="15.24" smashed="yes">
<attribute name="VALUE" x="117.475" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY30" gate="GND" x="147.32" y="15.24" smashed="yes">
<attribute name="VALUE" x="145.415" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY31" gate="GND" x="20.32" y="-73.66" smashed="yes">
<attribute name="VALUE" x="18.415" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY32" gate="GND" x="45.72" y="-73.66" smashed="yes">
<attribute name="VALUE" x="43.815" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY33" gate="GND" x="71.12" y="-73.66" smashed="yes">
<attribute name="VALUE" x="69.215" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY34" gate="GND" x="93.98" y="-73.66" smashed="yes">
<attribute name="VALUE" x="92.075" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY35" gate="GND" x="124.46" y="-73.66" smashed="yes">
<attribute name="VALUE" x="122.555" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY36" gate="GND" x="149.86" y="-73.66" smashed="yes">
<attribute name="VALUE" x="147.955" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="U$19" gate="G$1" x="38.1" y="12.7" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="96.52" x2="22.86" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY25" gate="GND" pin="GND"/>
<wire x1="22.86" y1="96.52" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<wire x1="22.86" y1="93.98" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<wire x1="22.86" y1="91.44" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<wire x1="22.86" y1="88.9" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<wire x1="22.86" y1="86.36" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<wire x1="22.86" y1="78.74" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<wire x1="22.86" y1="76.2" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<wire x1="22.86" y1="73.66" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<wire x1="22.86" y1="71.12" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<wire x1="22.86" y1="68.58" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<wire x1="22.86" y1="66.04" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="63.5" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="60.96" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="58.42" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="55.88" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="53.34" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="50.8" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="48.26" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="45.72" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="43.18" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="40.64" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="38.1" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="35.56" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="33.02" x2="22.86" y2="30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="30.48" x2="22.86" y2="27.94" width="0.1524" layer="91"/>
<wire x1="22.86" y1="27.94" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="25.4" x2="22.86" y2="17.78" width="0.1524" layer="91"/>
<wire x1="22.86" y1="17.78" x2="20.32" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="25.4" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="25.4"/>
<pinref part="U$5" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="27.94" x2="22.86" y2="27.94" width="0.1524" layer="91"/>
<junction x="22.86" y="27.94"/>
<pinref part="U$5" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="30.48" x2="22.86" y2="30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="30.48"/>
<pinref part="U$5" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="33.02" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="33.02"/>
<pinref part="U$5" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="35.56" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="35.56"/>
<pinref part="U$5" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="38.1" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="38.1"/>
<pinref part="U$5" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="40.64" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="40.64"/>
<pinref part="U$5" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="43.18" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="43.18"/>
<pinref part="U$5" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="45.72" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="45.72"/>
<pinref part="U$5" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="48.26" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="48.26"/>
<pinref part="U$5" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="50.8" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="50.8"/>
<pinref part="U$5" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="53.34" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="53.34"/>
<pinref part="U$5" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="55.88" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="55.88"/>
<pinref part="U$5" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="58.42" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="58.42"/>
<pinref part="U$5" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="60.96" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="60.96"/>
<pinref part="U$5" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="63.5" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<junction x="22.86" y="63.5"/>
<pinref part="U$5" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="66.04" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<junction x="22.86" y="66.04"/>
<pinref part="U$5" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="68.58" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<junction x="22.86" y="68.58"/>
<pinref part="U$5" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="71.12" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<junction x="22.86" y="71.12"/>
<pinref part="U$5" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="73.66" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<junction x="22.86" y="73.66"/>
<pinref part="U$5" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="76.2" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<junction x="22.86" y="76.2"/>
<pinref part="U$5" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="78.74" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<junction x="22.86" y="78.74"/>
<pinref part="U$5" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="81.28" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<wire x1="22.86" y1="81.28" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="83.82" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<wire x1="22.86" y1="83.82" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<junction x="22.86" y="81.28"/>
<pinref part="U$5" gate="G$1" pin="A5"/>
<junction x="22.86" y="86.36"/>
<wire x1="15.24" y1="86.36" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="88.9" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<junction x="22.86" y="88.9"/>
<pinref part="U$5" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="91.44" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<junction x="22.86" y="91.44"/>
<pinref part="U$5" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="93.98" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<junction x="22.86" y="93.98"/>
<wire x1="20.32" y1="17.78" x2="27.94" y2="17.78" width="0.1524" layer="91"/>
<junction x="20.32" y="17.78"/>
<wire x1="27.94" y1="17.78" x2="27.94" y2="12.7" width="0.1524" layer="91"/>
<pinref part="U$19" gate="G$1" pin="GND"/>
<wire x1="27.94" y1="12.7" x2="38.1" y2="12.7" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="96.52" x2="48.26" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY26" gate="GND" pin="GND"/>
<wire x1="48.26" y1="96.52" x2="48.26" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="B2"/>
<wire x1="48.26" y1="93.98" x2="48.26" y2="76.2" width="0.1524" layer="91"/>
<wire x1="48.26" y1="76.2" x2="48.26" y2="58.42" width="0.1524" layer="91"/>
<wire x1="48.26" y1="58.42" x2="48.26" y2="40.64" width="0.1524" layer="91"/>
<wire x1="48.26" y1="40.64" x2="48.26" y2="22.86" width="0.1524" layer="91"/>
<wire x1="48.26" y1="22.86" x2="48.26" y2="17.78" width="0.1524" layer="91"/>
<wire x1="40.64" y1="93.98" x2="48.26" y2="93.98" width="0.1524" layer="91"/>
<junction x="48.26" y="93.98"/>
<pinref part="U$5" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="76.2" x2="48.26" y2="76.2" width="0.1524" layer="91"/>
<junction x="48.26" y="76.2"/>
<pinref part="U$5" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="58.42" x2="48.26" y2="58.42" width="0.1524" layer="91"/>
<junction x="48.26" y="58.42"/>
<pinref part="U$5" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="40.64" x2="48.26" y2="40.64" width="0.1524" layer="91"/>
<junction x="48.26" y="40.64"/>
<pinref part="U$5" gate="G$1" pin="B30"/>
<wire x1="40.64" y1="22.86" x2="48.26" y2="22.86" width="0.1524" layer="91"/>
<junction x="48.26" y="22.86"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="96.52" x2="73.66" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY27" gate="GND" pin="GND"/>
<wire x1="73.66" y1="96.52" x2="73.66" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="C2"/>
<wire x1="73.66" y1="93.98" x2="73.66" y2="76.2" width="0.1524" layer="91"/>
<wire x1="73.66" y1="76.2" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<wire x1="73.66" y1="58.42" x2="73.66" y2="40.64" width="0.1524" layer="91"/>
<wire x1="73.66" y1="40.64" x2="73.66" y2="22.86" width="0.1524" layer="91"/>
<wire x1="73.66" y1="22.86" x2="73.66" y2="17.78" width="0.1524" layer="91"/>
<wire x1="66.04" y1="93.98" x2="73.66" y2="93.98" width="0.1524" layer="91"/>
<junction x="73.66" y="93.98"/>
<pinref part="U$5" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="76.2" x2="73.66" y2="76.2" width="0.1524" layer="91"/>
<junction x="73.66" y="76.2"/>
<pinref part="U$5" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="58.42" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<junction x="73.66" y="58.42"/>
<pinref part="U$5" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="40.64" x2="73.66" y2="40.64" width="0.1524" layer="91"/>
<junction x="73.66" y="40.64"/>
<pinref part="U$5" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="22.86" x2="73.66" y2="22.86" width="0.1524" layer="91"/>
<junction x="73.66" y="22.86"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="96.52" x2="99.06" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY28" gate="GND" pin="GND"/>
<wire x1="99.06" y1="96.52" x2="99.06" y2="93.98" width="0.1524" layer="91"/>
<wire x1="99.06" y1="93.98" x2="99.06" y2="76.2" width="0.1524" layer="91"/>
<wire x1="99.06" y1="76.2" x2="99.06" y2="58.42" width="0.1524" layer="91"/>
<wire x1="99.06" y1="58.42" x2="99.06" y2="40.64" width="0.1524" layer="91"/>
<wire x1="99.06" y1="40.64" x2="99.06" y2="22.86" width="0.1524" layer="91"/>
<wire x1="99.06" y1="22.86" x2="99.06" y2="20.32" width="0.1524" layer="91"/>
<wire x1="99.06" y1="20.32" x2="96.52" y2="20.32" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="93.98" x2="99.06" y2="93.98" width="0.1524" layer="91"/>
<junction x="99.06" y="93.98"/>
<pinref part="U$5" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="76.2" x2="99.06" y2="76.2" width="0.1524" layer="91"/>
<junction x="99.06" y="76.2"/>
<pinref part="U$5" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="58.42" x2="99.06" y2="58.42" width="0.1524" layer="91"/>
<junction x="99.06" y="58.42"/>
<pinref part="U$5" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="40.64" x2="99.06" y2="40.64" width="0.1524" layer="91"/>
<junction x="99.06" y="40.64"/>
<pinref part="U$5" gate="G$1" pin="D30"/>
<wire x1="91.44" y1="22.86" x2="99.06" y2="22.86" width="0.1524" layer="91"/>
<junction x="99.06" y="22.86"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="96.52" x2="124.46" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY29" gate="GND" pin="GND"/>
<wire x1="124.46" y1="96.52" x2="124.46" y2="22.86" width="0.1524" layer="91"/>
<wire x1="124.46" y1="22.86" x2="124.46" y2="17.78" width="0.1524" layer="91"/>
<wire x1="124.46" y1="17.78" x2="119.38" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="E30"/>
<wire x1="116.84" y1="22.86" x2="124.46" y2="22.86" width="0.1524" layer="91"/>
<junction x="124.46" y="22.86"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="96.52" x2="149.86" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY30" gate="GND" pin="GND"/>
<wire x1="149.86" y1="96.52" x2="149.86" y2="22.86" width="0.1524" layer="91"/>
<wire x1="149.86" y1="22.86" x2="149.86" y2="17.78" width="0.1524" layer="91"/>
<wire x1="149.86" y1="17.78" x2="147.32" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$5" gate="G$1" pin="F30"/>
<wire x1="142.24" y1="22.86" x2="149.86" y2="22.86" width="0.1524" layer="91"/>
<junction x="149.86" y="22.86"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="5.08" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY31" gate="GND" pin="GND"/>
<wire x1="22.86" y1="5.08" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="2.54" x2="22.86" y2="0" width="0.1524" layer="91"/>
<wire x1="22.86" y1="0" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-2.54" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-5.08" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-7.62" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-10.16" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-12.7" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-15.24" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-17.78" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-20.32" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-22.86" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-25.4" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-27.94" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-30.48" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-33.02" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-35.56" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-38.1" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-40.64" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-43.18" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-45.72" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-48.26" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-50.8" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-53.34" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-55.88" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-58.42" x2="22.86" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-60.96" x2="22.86" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-63.5" x2="22.86" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-66.04" x2="22.86" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-71.12" x2="20.32" y2="-71.12" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="-66.04" x2="22.86" y2="-66.04" width="0.1524" layer="91"/>
<junction x="22.86" y="-66.04"/>
<pinref part="U$6" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="-63.5" x2="22.86" y2="-63.5" width="0.1524" layer="91"/>
<junction x="22.86" y="-63.5"/>
<pinref part="U$6" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="-60.96" x2="22.86" y2="-60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="-60.96"/>
<pinref part="U$6" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="-58.42" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="-58.42"/>
<pinref part="U$6" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="-55.88" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="-55.88"/>
<pinref part="U$6" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="-53.34" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="-53.34"/>
<pinref part="U$6" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="-50.8" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="-50.8"/>
<pinref part="U$6" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="-48.26" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="-48.26"/>
<pinref part="U$6" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="-45.72" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="-45.72"/>
<pinref part="U$6" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="-43.18" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="-43.18"/>
<pinref part="U$6" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="-40.64" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="-40.64"/>
<pinref part="U$6" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="-38.1" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="-38.1"/>
<pinref part="U$6" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="-35.56" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="-35.56"/>
<pinref part="U$6" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="-33.02" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="-33.02"/>
<pinref part="U$6" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="-30.48" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="-30.48"/>
<pinref part="U$6" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="-27.94" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<junction x="22.86" y="-27.94"/>
<pinref part="U$6" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="-25.4" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="-25.4"/>
<pinref part="U$6" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="-22.86" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<junction x="22.86" y="-22.86"/>
<pinref part="U$6" gate="G$1" pin="A11"/>
<wire x1="22.86" y1="-20.32" x2="15.24" y2="-20.32" width="0.1524" layer="91"/>
<junction x="22.86" y="-20.32"/>
<pinref part="U$6" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="-17.78" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<junction x="22.86" y="-17.78"/>
<pinref part="U$6" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="-15.24" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<junction x="22.86" y="-15.24"/>
<pinref part="U$6" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="-12.7" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<junction x="22.86" y="-12.7"/>
<pinref part="U$6" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="-10.16" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<junction x="22.86" y="-10.16"/>
<pinref part="U$6" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="-7.62" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<junction x="22.86" y="-7.62"/>
<pinref part="U$6" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="-5.08" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="-5.08"/>
<pinref part="U$6" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="-2.54" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="-2.54"/>
<pinref part="U$6" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="0" x2="22.86" y2="0" width="0.1524" layer="91"/>
<junction x="22.86" y="0"/>
<pinref part="U$6" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="2.54" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="2.54"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="5.08" x2="48.26" y2="5.08" width="0.1524" layer="91"/>
<wire x1="48.26" y1="5.08" x2="48.26" y2="2.54" width="0.1524" layer="91"/>
<pinref part="SUPPLY32" gate="GND" pin="GND"/>
<wire x1="48.26" y1="2.54" x2="48.26" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-15.24" x2="48.26" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-33.02" x2="48.26" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-50.8" x2="48.26" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-68.58" x2="48.26" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-71.12" x2="45.72" y2="-71.12" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="B30"/>
<wire x1="40.64" y1="-68.58" x2="48.26" y2="-68.58" width="0.1524" layer="91"/>
<junction x="48.26" y="-68.58"/>
<pinref part="U$6" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="-50.8" x2="48.26" y2="-50.8" width="0.1524" layer="91"/>
<junction x="48.26" y="-50.8"/>
<pinref part="U$6" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="-33.02" x2="48.26" y2="-33.02" width="0.1524" layer="91"/>
<junction x="48.26" y="-33.02"/>
<pinref part="U$6" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="-15.24" x2="48.26" y2="-15.24" width="0.1524" layer="91"/>
<junction x="48.26" y="-15.24"/>
<pinref part="U$6" gate="G$1" pin="B2"/>
<wire x1="40.64" y1="2.54" x2="48.26" y2="2.54" width="0.1524" layer="91"/>
<junction x="48.26" y="2.54"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="5.08" x2="73.66" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY33" gate="GND" pin="GND"/>
<wire x1="73.66" y1="5.08" x2="73.66" y2="2.54" width="0.1524" layer="91"/>
<wire x1="73.66" y1="2.54" x2="73.66" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-15.24" x2="73.66" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-33.02" x2="73.66" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-50.8" x2="73.66" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-68.58" x2="73.66" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-71.12" x2="71.12" y2="-71.12" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="-68.58" x2="73.66" y2="-68.58" width="0.1524" layer="91"/>
<junction x="73.66" y="-68.58"/>
<pinref part="U$6" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="-50.8" x2="73.66" y2="-50.8" width="0.1524" layer="91"/>
<junction x="73.66" y="-50.8"/>
<pinref part="U$6" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="-33.02" x2="73.66" y2="-33.02" width="0.1524" layer="91"/>
<junction x="73.66" y="-33.02"/>
<pinref part="U$6" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="-15.24" x2="73.66" y2="-15.24" width="0.1524" layer="91"/>
<junction x="73.66" y="-15.24"/>
<pinref part="U$6" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="2.54" x2="73.66" y2="2.54" width="0.1524" layer="91"/>
<junction x="73.66" y="2.54"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="5.08" x2="99.06" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY34" gate="GND" pin="GND"/>
<wire x1="99.06" y1="5.08" x2="99.06" y2="2.54" width="0.1524" layer="91"/>
<wire x1="99.06" y1="2.54" x2="99.06" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-15.24" x2="99.06" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-33.02" x2="99.06" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-50.8" x2="99.06" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-68.58" x2="99.06" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-71.12" x2="93.98" y2="-71.12" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="D30"/>
<wire x1="91.44" y1="-68.58" x2="99.06" y2="-68.58" width="0.1524" layer="91"/>
<junction x="99.06" y="-68.58"/>
<pinref part="U$6" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="-50.8" x2="99.06" y2="-50.8" width="0.1524" layer="91"/>
<junction x="99.06" y="-50.8"/>
<pinref part="U$6" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="-33.02" x2="99.06" y2="-33.02" width="0.1524" layer="91"/>
<junction x="99.06" y="-33.02"/>
<pinref part="U$6" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="-15.24" x2="99.06" y2="-15.24" width="0.1524" layer="91"/>
<junction x="99.06" y="-15.24"/>
<pinref part="U$6" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="2.54" x2="99.06" y2="2.54" width="0.1524" layer="91"/>
<junction x="99.06" y="2.54"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="5.08" x2="124.46" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY35" gate="GND" pin="GND"/>
<wire x1="124.46" y1="5.08" x2="124.46" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="E30"/>
<wire x1="124.46" y1="-68.58" x2="124.46" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="116.84" y1="-68.58" x2="124.46" y2="-68.58" width="0.1524" layer="91"/>
<junction x="124.46" y="-68.58"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="5.08" x2="149.86" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY36" gate="GND" pin="GND"/>
<wire x1="149.86" y1="5.08" x2="149.86" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$6" gate="G$1" pin="F30"/>
<wire x1="149.86" y1="-68.58" x2="149.86" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="142.24" y1="-68.58" x2="149.86" y2="-68.58" width="0.1524" layer="91"/>
<junction x="149.86" y="-68.58"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="A30"/>
<wire x1="15.24" y1="22.86" x2="30.48" y2="22.86" width="0.1524" layer="91"/>
<wire x1="30.48" y1="22.86" x2="30.48" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$19" gate="G$1" pin="IO"/>
<wire x1="30.48" y1="15.24" x2="38.1" y2="15.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.09.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="91.44" x2="50.8" y2="91.44" width="0.1016" layer="91"/>
<label x="50.8" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="88.9" x2="50.8" y2="88.9" width="0.1016" layer="91"/>
<label x="50.8" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="86.36" x2="50.8" y2="86.36" width="0.1016" layer="91"/>
<label x="50.8" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="83.82" x2="50.8" y2="83.82" width="0.1016" layer="91"/>
<label x="50.8" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.034" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="81.28" x2="50.8" y2="81.28" width="0.1016" layer="91"/>
<label x="50.8" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.032" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="78.74" x2="50.8" y2="78.74" width="0.1016" layer="91"/>
<label x="50.8" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="73.66" x2="50.8" y2="73.66" width="0.1016" layer="91"/>
<label x="50.8" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="71.12" x2="50.8" y2="71.12" width="0.1016" layer="91"/>
<label x="50.8" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.032" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="68.58" x2="50.8" y2="68.58" width="0.1016" layer="91"/>
<label x="50.8" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.032" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="66.04" x2="50.8" y2="66.04" width="0.1016" layer="91"/>
<label x="50.8" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="63.5" x2="50.8" y2="63.5" width="0.1016" layer="91"/>
<label x="50.8" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="60.96" x2="50.8" y2="60.96" width="0.1016" layer="91"/>
<label x="50.8" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="55.88" x2="50.8" y2="55.88" width="0.1016" layer="91"/>
<label x="50.8" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.027" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="53.34" x2="50.8" y2="53.34" width="0.1016" layer="91"/>
<label x="50.8" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="50.8" x2="50.8" y2="50.8" width="0.1016" layer="91"/>
<label x="50.8" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.028" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="48.26" x2="50.8" y2="48.26" width="0.1016" layer="91"/>
<label x="50.8" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="45.72" x2="50.8" y2="45.72" width="0.1016" layer="91"/>
<label x="50.8" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.029" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="43.18" x2="50.8" y2="43.18" width="0.1016" layer="91"/>
<label x="50.8" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="38.1" x2="50.8" y2="38.1" width="0.1016" layer="91"/>
<label x="50.8" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="35.56" x2="50.8" y2="35.56" width="0.1016" layer="91"/>
<label x="50.8" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.032" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="33.02" x2="50.8" y2="33.02" width="0.1016" layer="91"/>
<label x="50.8" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.032" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="30.48" x2="50.8" y2="30.48" width="0.1016" layer="91"/>
<label x="50.8" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="27.94" x2="50.8" y2="27.94" width="0.1016" layer="91"/>
<label x="50.8" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="25.4" x2="50.8" y2="25.4" width="0.1016" layer="91"/>
<label x="50.8" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="91.44" x2="76.2" y2="91.44" width="0.1016" layer="91"/>
<label x="76.2" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="88.9" x2="76.2" y2="88.9" width="0.1016" layer="91"/>
<label x="76.2" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="86.36" x2="76.2" y2="86.36" width="0.1016" layer="91"/>
<label x="76.2" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="83.82" x2="76.2" y2="83.82" width="0.1016" layer="91"/>
<label x="76.2" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="81.28" x2="76.2" y2="81.28" width="0.1016" layer="91"/>
<label x="76.2" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.032" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="78.74" x2="76.2" y2="78.74" width="0.1016" layer="91"/>
<label x="76.2" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="73.66" x2="76.2" y2="73.66" width="0.1016" layer="91"/>
<label x="76.2" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="71.12" x2="76.2" y2="71.12" width="0.1016" layer="91"/>
<label x="76.2" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.033" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="68.58" x2="76.2" y2="68.58" width="0.1016" layer="91"/>
<label x="76.2" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.033" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="66.04" x2="76.2" y2="66.04" width="0.1016" layer="91"/>
<label x="76.2" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="63.5" x2="76.2" y2="63.5" width="0.1016" layer="91"/>
<label x="76.2" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="60.96" x2="76.2" y2="60.96" width="0.1016" layer="91"/>
<label x="76.2" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="55.88" x2="76.2" y2="55.88" width="0.1016" layer="91"/>
<label x="76.2" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="53.34" x2="76.2" y2="53.34" width="0.1016" layer="91"/>
<label x="76.2" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="50.8" x2="76.2" y2="50.8" width="0.1016" layer="91"/>
<label x="76.2" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="48.26" x2="76.2" y2="48.26" width="0.1016" layer="91"/>
<label x="76.2" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="45.72" x2="76.2" y2="45.72" width="0.1016" layer="91"/>
<label x="76.2" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="43.18" x2="76.2" y2="43.18" width="0.1016" layer="91"/>
<label x="76.2" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="38.1" x2="76.2" y2="38.1" width="0.1016" layer="91"/>
<label x="76.2" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="35.56" x2="76.2" y2="35.56" width="0.1016" layer="91"/>
<label x="76.2" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.033" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="33.02" x2="76.2" y2="33.02" width="0.1016" layer="91"/>
<label x="76.2" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.033" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="30.48" x2="76.2" y2="30.48" width="0.1016" layer="91"/>
<label x="76.2" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="27.94" x2="76.2" y2="27.94" width="0.1016" layer="91"/>
<label x="76.2" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.039" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="25.4" x2="76.2" y2="25.4" width="0.1016" layer="91"/>
<label x="76.2" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="91.44" x2="101.6" y2="91.44" width="0.1016" layer="91"/>
<label x="101.6" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="88.9" x2="101.6" y2="88.9" width="0.1016" layer="91"/>
<label x="101.6" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="86.36" x2="101.6" y2="86.36" width="0.1016" layer="91"/>
<label x="101.6" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="83.82" x2="101.6" y2="83.82" width="0.1016" layer="91"/>
<label x="101.6" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="81.28" x2="101.6" y2="81.28" width="0.1016" layer="91"/>
<label x="101.6" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.033" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="78.74" x2="101.6" y2="78.74" width="0.1016" layer="91"/>
<label x="101.6" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="73.66" x2="101.6" y2="73.66" width="0.1016" layer="91"/>
<label x="101.6" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="71.12" x2="101.6" y2="71.12" width="0.1016" layer="91"/>
<label x="101.6" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.034" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="68.58" x2="101.6" y2="68.58" width="0.1016" layer="91"/>
<label x="101.6" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.034" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="66.04" x2="101.6" y2="66.04" width="0.1016" layer="91"/>
<label x="101.6" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.038" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="63.5" x2="101.6" y2="63.5" width="0.1016" layer="91"/>
<label x="101.6" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="60.96" x2="101.6" y2="60.96" width="0.1016" layer="91"/>
<label x="101.6" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.027" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="55.88" x2="101.6" y2="55.88" width="0.1016" layer="91"/>
<label x="101.6" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="53.34" x2="101.6" y2="53.34" width="0.1016" layer="91"/>
<label x="101.6" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.028" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="50.8" x2="101.6" y2="50.8" width="0.1016" layer="91"/>
<label x="101.6" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.036" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="48.26" x2="101.6" y2="48.26" width="0.1016" layer="91"/>
<label x="101.6" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.029" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="45.72" x2="101.6" y2="45.72" width="0.1016" layer="91"/>
<label x="101.6" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.037" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="43.18" x2="101.6" y2="43.18" width="0.1016" layer="91"/>
<label x="101.6" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="38.1" x2="101.6" y2="38.1" width="0.1016" layer="91"/>
<label x="101.6" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="35.56" x2="101.6" y2="35.56" width="0.1016" layer="91"/>
<label x="101.6" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.034" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="33.02" x2="101.6" y2="33.02" width="0.1016" layer="91"/>
<label x="101.6" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.034" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="30.48" x2="101.6" y2="30.48" width="0.1016" layer="91"/>
<label x="101.6" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="27.94" x2="101.6" y2="27.94" width="0.1016" layer="91"/>
<label x="101.6" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.040" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="25.4" x2="101.6" y2="25.4" width="0.1016" layer="91"/>
<label x="101.6" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="91.44" x2="127" y2="91.44" width="0.1016" layer="91"/>
<label x="127" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="88.9" x2="127" y2="88.9" width="0.1016" layer="91"/>
<label x="127" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="86.36" x2="127" y2="86.36" width="0.1016" layer="91"/>
<label x="127" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.045" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="83.82" x2="127" y2="83.82" width="0.1016" layer="91"/>
<label x="127" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="81.28" x2="127" y2="81.28" width="0.1016" layer="91"/>
<label x="127" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.033" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="78.74" x2="127" y2="78.74" width="0.1016" layer="91"/>
<label x="127" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="73.66" x2="127" y2="73.66" width="0.1016" layer="91"/>
<label x="127" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.045" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="71.12" x2="127" y2="71.12" width="0.1016" layer="91"/>
<label x="127" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="68.58" x2="127" y2="68.58" width="0.1016" layer="91"/>
<label x="127" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="66.04" x2="127" y2="66.04" width="0.1016" layer="91"/>
<label x="127" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="63.5" x2="127" y2="63.5" width="0.1016" layer="91"/>
<label x="127" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="60.96" x2="127" y2="60.96" width="0.1016" layer="91"/>
<label x="127" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="55.88" x2="127" y2="55.88" width="0.1016" layer="91"/>
<label x="127" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="53.34" x2="127" y2="53.34" width="0.1016" layer="91"/>
<label x="127" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="50.8" x2="127" y2="50.8" width="0.1016" layer="91"/>
<label x="127" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="48.26" x2="127" y2="48.26" width="0.1016" layer="91"/>
<label x="127" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="45.72" x2="127" y2="45.72" width="0.1016" layer="91"/>
<label x="127" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="43.18" x2="127" y2="43.18" width="0.1016" layer="91"/>
<label x="127" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="38.1" x2="127" y2="38.1" width="0.1016" layer="91"/>
<label x="127" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="35.56" x2="127" y2="35.56" width="0.1016" layer="91"/>
<label x="127" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="33.02" x2="127" y2="33.02" width="0.1016" layer="91"/>
<label x="127" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="30.48" x2="127" y2="30.48" width="0.1016" layer="91"/>
<label x="127" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="27.94" x2="127" y2="27.94" width="0.1016" layer="91"/>
<label x="127" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.045" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="25.4" x2="127" y2="25.4" width="0.1016" layer="91"/>
<label x="127" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="91.44" x2="152.4" y2="91.44" width="0.1016" layer="91"/>
<label x="152.4" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="88.9" x2="152.4" y2="88.9" width="0.1016" layer="91"/>
<label x="152.4" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.045" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="86.36" x2="152.4" y2="86.36" width="0.1016" layer="91"/>
<label x="152.4" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="83.82" x2="152.4" y2="83.82" width="0.1016" layer="91"/>
<label x="152.4" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="81.28" x2="152.4" y2="81.28" width="0.1016" layer="91"/>
<label x="152.4" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.034" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="78.74" x2="152.4" y2="78.74" width="0.1016" layer="91"/>
<label x="152.4" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="73.66" x2="152.4" y2="73.66" width="0.1016" layer="91"/>
<label x="152.4" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="71.12" x2="152.4" y2="71.12" width="0.1016" layer="91"/>
<label x="152.4" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="68.58" x2="152.4" y2="68.58" width="0.1016" layer="91"/>
<label x="152.4" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="66.04" x2="152.4" y2="66.04" width="0.1016" layer="91"/>
<label x="152.4" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.045" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="63.5" x2="152.4" y2="63.5" width="0.1016" layer="91"/>
<label x="152.4" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="60.96" x2="152.4" y2="60.96" width="0.1016" layer="91"/>
<label x="152.4" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.048" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="55.88" x2="152.4" y2="55.88" width="0.1016" layer="91"/>
<label x="152.4" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="53.34" x2="152.4" y2="53.34" width="0.1016" layer="91"/>
<label x="152.4" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="50.8" x2="152.4" y2="50.8" width="0.1016" layer="91"/>
<label x="152.4" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="48.26" x2="152.4" y2="48.26" width="0.1016" layer="91"/>
<label x="152.4" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="45.72" x2="152.4" y2="45.72" width="0.1016" layer="91"/>
<label x="152.4" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="43.18" x2="152.4" y2="43.18" width="0.1016" layer="91"/>
<label x="152.4" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="38.1" x2="152.4" y2="38.1" width="0.1016" layer="91"/>
<label x="152.4" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="35.56" x2="152.4" y2="35.56" width="0.1016" layer="91"/>
<label x="152.4" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="33.02" x2="152.4" y2="33.02" width="0.1016" layer="91"/>
<label x="152.4" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.045" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="30.48" x2="152.4" y2="30.48" width="0.1016" layer="91"/>
<label x="152.4" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.035" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="27.94" x2="152.4" y2="27.94" width="0.1016" layer="91"/>
<label x="152.4" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.041" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="25.4" x2="152.4" y2="25.4" width="0.1016" layer="91"/>
<label x="152.4" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.044" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="93.98" x2="127" y2="93.98" width="0.1016" layer="91"/>
<label x="127" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.046" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="93.98" x2="152.4" y2="93.98" width="0.1016" layer="91"/>
<label x="152.4" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="76.2" x2="127" y2="76.2" width="0.1016" layer="91"/>
<label x="127" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="76.2" x2="152.4" y2="76.2" width="0.1016" layer="91"/>
<label x="152.4" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.048" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="58.42" x2="127" y2="58.42" width="0.1016" layer="91"/>
<label x="127" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.047" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="58.42" x2="152.4" y2="58.42" width="0.1016" layer="91"/>
<label x="152.4" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.043" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="40.64" x2="127" y2="40.64" width="0.1016" layer="91"/>
<label x="127" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.042" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="40.64" x2="152.4" y2="40.64" width="0.1016" layer="91"/>
<label x="152.4" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="0" x2="50.8" y2="0" width="0.1016" layer="91"/>
<label x="50.8" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="-2.54" x2="50.8" y2="-2.54" width="0.1016" layer="91"/>
<label x="50.8" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="-5.08" x2="50.8" y2="-5.08" width="0.1016" layer="91"/>
<label x="50.8" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="-7.62" x2="50.8" y2="-7.62" width="0.1016" layer="91"/>
<label x="50.8" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="-10.16" x2="50.8" y2="-10.16" width="0.1016" layer="91"/>
<label x="50.8" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="-12.7" x2="50.8" y2="-12.7" width="0.1016" layer="91"/>
<label x="50.8" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="-17.78" x2="50.8" y2="-17.78" width="0.1016" layer="91"/>
<label x="50.8" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="-20.32" x2="50.8" y2="-20.32" width="0.1016" layer="91"/>
<label x="50.8" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="-22.86" x2="50.8" y2="-22.86" width="0.1016" layer="91"/>
<label x="50.8" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="-25.4" x2="50.8" y2="-25.4" width="0.1016" layer="91"/>
<label x="50.8" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="-27.94" x2="50.8" y2="-27.94" width="0.1016" layer="91"/>
<label x="50.8" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="-30.48" x2="50.8" y2="-30.48" width="0.1016" layer="91"/>
<label x="50.8" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="-35.56" x2="50.8" y2="-35.56" width="0.1016" layer="91"/>
<label x="50.8" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="-38.1" x2="50.8" y2="-38.1" width="0.1016" layer="91"/>
<label x="50.8" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="-40.64" x2="50.8" y2="-40.64" width="0.1016" layer="91"/>
<label x="50.8" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="-43.18" x2="50.8" y2="-43.18" width="0.1016" layer="91"/>
<label x="50.8" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="-45.72" x2="50.8" y2="-45.72" width="0.1016" layer="91"/>
<label x="50.8" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="-48.26" x2="50.8" y2="-48.26" width="0.1016" layer="91"/>
<label x="50.8" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="-53.34" x2="50.8" y2="-53.34" width="0.1016" layer="91"/>
<label x="50.8" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="-55.88" x2="50.8" y2="-55.88" width="0.1016" layer="91"/>
<label x="50.8" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="-58.42" x2="50.8" y2="-58.42" width="0.1016" layer="91"/>
<label x="50.8" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="-60.96" x2="50.8" y2="-60.96" width="0.1016" layer="91"/>
<label x="50.8" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="-63.5" x2="50.8" y2="-63.5" width="0.1016" layer="91"/>
<label x="50.8" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="-66.04" x2="50.8" y2="-66.04" width="0.1016" layer="91"/>
<label x="50.8" y="-66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="0" x2="76.2" y2="0" width="0.1016" layer="91"/>
<label x="76.2" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="-2.54" x2="76.2" y2="-2.54" width="0.1016" layer="91"/>
<label x="76.2" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="-5.08" x2="76.2" y2="-5.08" width="0.1016" layer="91"/>
<label x="76.2" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="-7.62" x2="76.2" y2="-7.62" width="0.1016" layer="91"/>
<label x="76.2" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="-10.16" x2="76.2" y2="-10.16" width="0.1016" layer="91"/>
<label x="76.2" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="-12.7" x2="76.2" y2="-12.7" width="0.1016" layer="91"/>
<label x="76.2" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="-17.78" x2="76.2" y2="-17.78" width="0.1016" layer="91"/>
<label x="76.2" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="-20.32" x2="76.2" y2="-20.32" width="0.1016" layer="91"/>
<label x="76.2" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="-22.86" x2="76.2" y2="-22.86" width="0.1016" layer="91"/>
<label x="76.2" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.036" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="-25.4" x2="76.2" y2="-25.4" width="0.1016" layer="91"/>
<label x="76.2" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="-27.94" x2="76.2" y2="-27.94" width="0.1016" layer="91"/>
<label x="76.2" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.032" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="-30.48" x2="76.2" y2="-30.48" width="0.1016" layer="91"/>
<label x="76.2" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="-35.56" x2="76.2" y2="-35.56" width="0.1016" layer="91"/>
<label x="76.2" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="-38.1" x2="76.2" y2="-38.1" width="0.1016" layer="91"/>
<label x="76.2" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="-40.64" x2="76.2" y2="-40.64" width="0.1016" layer="91"/>
<label x="76.2" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="-43.18" x2="76.2" y2="-43.18" width="0.1016" layer="91"/>
<label x="76.2" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="-45.72" x2="76.2" y2="-45.72" width="0.1016" layer="91"/>
<label x="76.2" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="-48.26" x2="76.2" y2="-48.26" width="0.1016" layer="91"/>
<label x="76.2" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="-53.34" x2="76.2" y2="-53.34" width="0.1016" layer="91"/>
<label x="76.2" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="-55.88" x2="76.2" y2="-55.88" width="0.1016" layer="91"/>
<label x="76.2" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="-58.42" x2="76.2" y2="-58.42" width="0.1016" layer="91"/>
<label x="76.2" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="-60.96" x2="76.2" y2="-60.96" width="0.1016" layer="91"/>
<label x="76.2" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="-63.5" x2="76.2" y2="-63.5" width="0.1016" layer="91"/>
<label x="76.2" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.039" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="-66.04" x2="76.2" y2="-66.04" width="0.1016" layer="91"/>
<label x="76.2" y="-66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="0" x2="101.6" y2="0" width="0.1016" layer="91"/>
<label x="101.6" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="-2.54" x2="101.6" y2="-2.54" width="0.1016" layer="91"/>
<label x="101.6" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="-5.08" x2="101.6" y2="-5.08" width="0.1016" layer="91"/>
<label x="101.6" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="-7.62" x2="101.6" y2="-7.62" width="0.1016" layer="91"/>
<label x="101.6" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="-10.16" x2="101.6" y2="-10.16" width="0.1016" layer="91"/>
<label x="101.6" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="-12.7" x2="101.6" y2="-12.7" width="0.1016" layer="91"/>
<label x="101.6" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="-17.78" x2="101.6" y2="-17.78" width="0.1016" layer="91"/>
<label x="101.6" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="-20.32" x2="101.6" y2="-20.32" width="0.1016" layer="91"/>
<label x="101.6" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="-22.86" x2="101.6" y2="-22.86" width="0.1016" layer="91"/>
<label x="101.6" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.037" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="-25.4" x2="101.6" y2="-25.4" width="0.1016" layer="91"/>
<label x="101.6" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="-27.94" x2="101.6" y2="-27.94" width="0.1016" layer="91"/>
<label x="101.6" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="-30.48" x2="101.6" y2="-30.48" width="0.1016" layer="91"/>
<label x="101.6" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="-35.56" x2="101.6" y2="-35.56" width="0.1016" layer="91"/>
<label x="101.6" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="-38.1" x2="101.6" y2="-38.1" width="0.1016" layer="91"/>
<label x="101.6" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="-40.64" x2="101.6" y2="-40.64" width="0.1016" layer="91"/>
<label x="101.6" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="-43.18" x2="101.6" y2="-43.18" width="0.1016" layer="91"/>
<label x="101.6" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="-45.72" x2="101.6" y2="-45.72" width="0.1016" layer="91"/>
<label x="101.6" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="-48.26" x2="101.6" y2="-48.26" width="0.1016" layer="91"/>
<label x="101.6" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="-53.34" x2="101.6" y2="-53.34" width="0.1016" layer="91"/>
<label x="101.6" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.038" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="-55.88" x2="101.6" y2="-55.88" width="0.1016" layer="91"/>
<label x="101.6" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="-58.42" x2="101.6" y2="-58.42" width="0.1016" layer="91"/>
<label x="101.6" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="-60.96" x2="101.6" y2="-60.96" width="0.1016" layer="91"/>
<label x="101.6" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="-63.5" x2="101.6" y2="-63.5" width="0.1016" layer="91"/>
<label x="101.6" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.040" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="-66.04" x2="101.6" y2="-66.04" width="0.1016" layer="91"/>
<label x="101.6" y="-66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="0" x2="127" y2="0" width="0.1016" layer="91"/>
<label x="127" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="-2.54" x2="127" y2="-2.54" width="0.1016" layer="91"/>
<label x="127" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="-5.08" x2="127" y2="-5.08" width="0.1016" layer="91"/>
<label x="127" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="-7.62" x2="127" y2="-7.62" width="0.1016" layer="91"/>
<label x="127" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="-10.16" x2="127" y2="-10.16" width="0.1016" layer="91"/>
<label x="127" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="-12.7" x2="127" y2="-12.7" width="0.1016" layer="91"/>
<label x="127" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="-17.78" x2="127" y2="-17.78" width="0.1016" layer="91"/>
<label x="127" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="-20.32" x2="127" y2="-20.32" width="0.1016" layer="91"/>
<label x="127" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="-22.86" x2="127" y2="-22.86" width="0.1016" layer="91"/>
<label x="127" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="-25.4" x2="127" y2="-25.4" width="0.1016" layer="91"/>
<label x="127" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="-27.94" x2="127" y2="-27.94" width="0.1016" layer="91"/>
<label x="127" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.033" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="-30.48" x2="127" y2="-30.48" width="0.1016" layer="91"/>
<label x="127" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="-35.56" x2="127" y2="-35.56" width="0.1016" layer="91"/>
<label x="127" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="-38.1" x2="127" y2="-38.1" width="0.1016" layer="91"/>
<label x="127" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="-40.64" x2="127" y2="-40.64" width="0.1016" layer="91"/>
<label x="127" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="-43.18" x2="127" y2="-43.18" width="0.1016" layer="91"/>
<label x="127" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="-45.72" x2="127" y2="-45.72" width="0.1016" layer="91"/>
<label x="127" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="-48.26" x2="127" y2="-48.26" width="0.1016" layer="91"/>
<label x="127" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="-53.34" x2="127" y2="-53.34" width="0.1016" layer="91"/>
<label x="127" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="-55.88" x2="127" y2="-55.88" width="0.1016" layer="91"/>
<label x="127" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="-58.42" x2="127" y2="-58.42" width="0.1016" layer="91"/>
<label x="127" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="-60.96" x2="127" y2="-60.96" width="0.1016" layer="91"/>
<label x="127" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="-63.5" x2="127" y2="-63.5" width="0.1016" layer="91"/>
<label x="127" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="-66.04" x2="127" y2="-66.04" width="0.1016" layer="91"/>
<label x="127" y="-66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="0" x2="152.4" y2="0" width="0.1016" layer="91"/>
<label x="152.4" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="-2.54" x2="152.4" y2="-2.54" width="0.1016" layer="91"/>
<label x="152.4" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="-5.08" x2="152.4" y2="-5.08" width="0.1016" layer="91"/>
<label x="152.4" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="-7.62" x2="152.4" y2="-7.62" width="0.1016" layer="91"/>
<label x="152.4" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="-10.16" x2="152.4" y2="-10.16" width="0.1016" layer="91"/>
<label x="152.4" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="-12.7" x2="152.4" y2="-12.7" width="0.1016" layer="91"/>
<label x="152.4" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="-17.78" x2="152.4" y2="-17.78" width="0.1016" layer="91"/>
<label x="152.4" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="-20.32" x2="152.4" y2="-20.32" width="0.1016" layer="91"/>
<label x="152.4" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="-22.86" x2="152.4" y2="-22.86" width="0.1016" layer="91"/>
<label x="152.4" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="-25.4" x2="152.4" y2="-25.4" width="0.1016" layer="91"/>
<label x="152.4" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="-27.94" x2="152.4" y2="-27.94" width="0.1016" layer="91"/>
<label x="152.4" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.034" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="-30.48" x2="152.4" y2="-30.48" width="0.1016" layer="91"/>
<label x="152.4" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="-35.56" x2="152.4" y2="-35.56" width="0.1016" layer="91"/>
<label x="152.4" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="-38.1" x2="152.4" y2="-38.1" width="0.1016" layer="91"/>
<label x="152.4" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="-40.64" x2="152.4" y2="-40.64" width="0.1016" layer="91"/>
<label x="152.4" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="-43.18" x2="152.4" y2="-43.18" width="0.1016" layer="91"/>
<label x="152.4" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.045" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="-45.72" x2="152.4" y2="-45.72" width="0.1016" layer="91"/>
<label x="152.4" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="-48.26" x2="152.4" y2="-48.26" width="0.1016" layer="91"/>
<label x="152.4" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="-53.34" x2="152.4" y2="-53.34" width="0.1016" layer="91"/>
<label x="152.4" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="-55.88" x2="152.4" y2="-55.88" width="0.1016" layer="91"/>
<label x="152.4" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="-58.42" x2="152.4" y2="-58.42" width="0.1016" layer="91"/>
<label x="152.4" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="-60.96" x2="152.4" y2="-60.96" width="0.1016" layer="91"/>
<label x="152.4" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.035" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="-63.5" x2="152.4" y2="-63.5" width="0.1016" layer="91"/>
<label x="152.4" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.041" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="-66.04" x2="152.4" y2="-66.04" width="0.1016" layer="91"/>
<label x="152.4" y="-66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="2.54" x2="127" y2="2.54" width="0.1016" layer="91"/>
<label x="127" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="2.54" x2="152.4" y2="2.54" width="0.1016" layer="91"/>
<label x="152.4" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.044" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="-15.24" x2="127" y2="-15.24" width="0.1016" layer="91"/>
<label x="127" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.046" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="-15.24" x2="152.4" y2="-15.24" width="0.1016" layer="91"/>
<label x="152.4" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="-33.02" x2="127" y2="-33.02" width="0.1016" layer="91"/>
<label x="127" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.047" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="-33.02" x2="152.4" y2="-33.02" width="0.1016" layer="91"/>
<label x="152.4" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.043" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="-50.8" x2="127" y2="-50.8" width="0.1016" layer="91"/>
<label x="127" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.042" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="-50.8" x2="152.4" y2="-50.8" width="0.1016" layer="91"/>
<label x="152.4" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$7" gate="G$1" x="66.04" y="73.66" smashed="yes"/>
<instance part="U$8" gate="G$1" x="60.96" y="-15.24" smashed="yes"/>
<instance part="SUPPLY37" gate="GND" x="27.94" y="22.86" smashed="yes">
<attribute name="VALUE" x="26.035" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY38" gate="GND" x="55.88" y="20.32" smashed="yes">
<attribute name="VALUE" x="53.975" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY39" gate="GND" x="76.2" y="20.32" smashed="yes">
<attribute name="VALUE" x="74.295" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY40" gate="GND" x="106.68" y="20.32" smashed="yes">
<attribute name="VALUE" x="104.775" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY41" gate="GND" x="127" y="20.32" smashed="yes">
<attribute name="VALUE" x="125.095" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY42" gate="GND" x="154.94" y="20.32" smashed="yes">
<attribute name="VALUE" x="153.035" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY43" gate="GND" x="22.86" y="-66.04" smashed="yes">
<attribute name="VALUE" x="20.955" y="-69.215" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY44" gate="GND" x="48.26" y="-66.04" smashed="yes">
<attribute name="VALUE" x="46.355" y="-69.215" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY45" gate="GND" x="76.2" y="-66.04" smashed="yes">
<attribute name="VALUE" x="74.295" y="-69.215" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY46" gate="GND" x="99.06" y="-66.04" smashed="yes">
<attribute name="VALUE" x="97.155" y="-69.215" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY47" gate="GND" x="127" y="-66.04" smashed="yes">
<attribute name="VALUE" x="125.095" y="-69.215" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY48" gate="GND" x="152.4" y="-66.04" smashed="yes">
<attribute name="VALUE" x="150.495" y="-69.215" size="1.778" layer="96"/>
</instance>
<instance part="U$20" gate="G$1" x="45.72" y="20.32" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="A1"/>
<wire x1="22.86" y1="101.6" x2="30.48" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY37" gate="GND" pin="GND"/>
<wire x1="30.48" y1="101.6" x2="30.48" y2="99.06" width="0.1524" layer="91"/>
<wire x1="30.48" y1="99.06" x2="30.48" y2="96.52" width="0.1524" layer="91"/>
<wire x1="30.48" y1="96.52" x2="30.48" y2="93.98" width="0.1524" layer="91"/>
<wire x1="30.48" y1="93.98" x2="30.48" y2="91.44" width="0.1524" layer="91"/>
<wire x1="30.48" y1="91.44" x2="30.48" y2="88.9" width="0.1524" layer="91"/>
<wire x1="30.48" y1="88.9" x2="30.48" y2="86.36" width="0.1524" layer="91"/>
<wire x1="30.48" y1="86.36" x2="30.48" y2="83.82" width="0.1524" layer="91"/>
<wire x1="30.48" y1="83.82" x2="30.48" y2="81.28" width="0.1524" layer="91"/>
<wire x1="30.48" y1="81.28" x2="30.48" y2="78.74" width="0.1524" layer="91"/>
<wire x1="30.48" y1="78.74" x2="30.48" y2="76.2" width="0.1524" layer="91"/>
<wire x1="30.48" y1="76.2" x2="30.48" y2="73.66" width="0.1524" layer="91"/>
<wire x1="30.48" y1="73.66" x2="30.48" y2="71.12" width="0.1524" layer="91"/>
<wire x1="30.48" y1="71.12" x2="30.48" y2="68.58" width="0.1524" layer="91"/>
<wire x1="30.48" y1="68.58" x2="30.48" y2="66.04" width="0.1524" layer="91"/>
<wire x1="30.48" y1="66.04" x2="30.48" y2="63.5" width="0.1524" layer="91"/>
<wire x1="30.48" y1="63.5" x2="30.48" y2="60.96" width="0.1524" layer="91"/>
<wire x1="30.48" y1="60.96" x2="30.48" y2="58.42" width="0.1524" layer="91"/>
<wire x1="30.48" y1="58.42" x2="30.48" y2="55.88" width="0.1524" layer="91"/>
<wire x1="30.48" y1="55.88" x2="30.48" y2="53.34" width="0.1524" layer="91"/>
<wire x1="30.48" y1="53.34" x2="30.48" y2="50.8" width="0.1524" layer="91"/>
<wire x1="30.48" y1="50.8" x2="30.48" y2="48.26" width="0.1524" layer="91"/>
<wire x1="30.48" y1="48.26" x2="30.48" y2="45.72" width="0.1524" layer="91"/>
<wire x1="30.48" y1="45.72" x2="30.48" y2="43.18" width="0.1524" layer="91"/>
<wire x1="30.48" y1="43.18" x2="30.48" y2="40.64" width="0.1524" layer="91"/>
<wire x1="30.48" y1="40.64" x2="30.48" y2="38.1" width="0.1524" layer="91"/>
<wire x1="30.48" y1="38.1" x2="30.48" y2="35.56" width="0.1524" layer="91"/>
<wire x1="30.48" y1="35.56" x2="30.48" y2="33.02" width="0.1524" layer="91"/>
<wire x1="30.48" y1="33.02" x2="30.48" y2="30.48" width="0.1524" layer="91"/>
<wire x1="30.48" y1="30.48" x2="30.48" y2="25.4" width="0.1524" layer="91"/>
<wire x1="30.48" y1="25.4" x2="27.94" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$7" gate="G$1" pin="A29"/>
<wire x1="22.86" y1="30.48" x2="30.48" y2="30.48" width="0.1524" layer="91"/>
<junction x="30.48" y="30.48"/>
<pinref part="U$7" gate="G$1" pin="A28"/>
<wire x1="22.86" y1="33.02" x2="30.48" y2="33.02" width="0.1524" layer="91"/>
<junction x="30.48" y="33.02"/>
<pinref part="U$7" gate="G$1" pin="A27"/>
<wire x1="22.86" y1="35.56" x2="30.48" y2="35.56" width="0.1524" layer="91"/>
<junction x="30.48" y="35.56"/>
<pinref part="U$7" gate="G$1" pin="A26"/>
<wire x1="22.86" y1="38.1" x2="30.48" y2="38.1" width="0.1524" layer="91"/>
<junction x="30.48" y="38.1"/>
<pinref part="U$7" gate="G$1" pin="A25"/>
<wire x1="22.86" y1="40.64" x2="30.48" y2="40.64" width="0.1524" layer="91"/>
<junction x="30.48" y="40.64"/>
<pinref part="U$7" gate="G$1" pin="A24"/>
<wire x1="22.86" y1="43.18" x2="30.48" y2="43.18" width="0.1524" layer="91"/>
<junction x="30.48" y="43.18"/>
<pinref part="U$7" gate="G$1" pin="A23"/>
<wire x1="22.86" y1="45.72" x2="30.48" y2="45.72" width="0.1524" layer="91"/>
<junction x="30.48" y="45.72"/>
<pinref part="U$7" gate="G$1" pin="A22"/>
<wire x1="22.86" y1="48.26" x2="30.48" y2="48.26" width="0.1524" layer="91"/>
<junction x="30.48" y="48.26"/>
<pinref part="U$7" gate="G$1" pin="A21"/>
<wire x1="22.86" y1="50.8" x2="30.48" y2="50.8" width="0.1524" layer="91"/>
<junction x="30.48" y="50.8"/>
<pinref part="U$7" gate="G$1" pin="A20"/>
<wire x1="22.86" y1="53.34" x2="30.48" y2="53.34" width="0.1524" layer="91"/>
<junction x="30.48" y="53.34"/>
<pinref part="U$7" gate="G$1" pin="A19"/>
<wire x1="22.86" y1="55.88" x2="30.48" y2="55.88" width="0.1524" layer="91"/>
<junction x="30.48" y="55.88"/>
<pinref part="U$7" gate="G$1" pin="A18"/>
<wire x1="22.86" y1="58.42" x2="30.48" y2="58.42" width="0.1524" layer="91"/>
<junction x="30.48" y="58.42"/>
<pinref part="U$7" gate="G$1" pin="A17"/>
<wire x1="22.86" y1="60.96" x2="30.48" y2="60.96" width="0.1524" layer="91"/>
<junction x="30.48" y="60.96"/>
<pinref part="U$7" gate="G$1" pin="A16"/>
<wire x1="22.86" y1="63.5" x2="30.48" y2="63.5" width="0.1524" layer="91"/>
<junction x="30.48" y="63.5"/>
<pinref part="U$7" gate="G$1" pin="A15"/>
<wire x1="22.86" y1="66.04" x2="30.48" y2="66.04" width="0.1524" layer="91"/>
<junction x="30.48" y="66.04"/>
<pinref part="U$7" gate="G$1" pin="A14"/>
<wire x1="22.86" y1="68.58" x2="30.48" y2="68.58" width="0.1524" layer="91"/>
<junction x="30.48" y="68.58"/>
<pinref part="U$7" gate="G$1" pin="A13"/>
<wire x1="22.86" y1="71.12" x2="30.48" y2="71.12" width="0.1524" layer="91"/>
<junction x="30.48" y="71.12"/>
<pinref part="U$7" gate="G$1" pin="A12"/>
<wire x1="22.86" y1="73.66" x2="30.48" y2="73.66" width="0.1524" layer="91"/>
<junction x="30.48" y="73.66"/>
<pinref part="U$7" gate="G$1" pin="A11"/>
<wire x1="22.86" y1="76.2" x2="30.48" y2="76.2" width="0.1524" layer="91"/>
<junction x="30.48" y="76.2"/>
<pinref part="U$7" gate="G$1" pin="A10"/>
<wire x1="22.86" y1="78.74" x2="30.48" y2="78.74" width="0.1524" layer="91"/>
<junction x="30.48" y="78.74"/>
<pinref part="U$7" gate="G$1" pin="A9"/>
<wire x1="22.86" y1="81.28" x2="30.48" y2="81.28" width="0.1524" layer="91"/>
<junction x="30.48" y="81.28"/>
<pinref part="U$7" gate="G$1" pin="A8"/>
<wire x1="22.86" y1="83.82" x2="30.48" y2="83.82" width="0.1524" layer="91"/>
<junction x="30.48" y="83.82"/>
<pinref part="U$7" gate="G$1" pin="A7"/>
<wire x1="22.86" y1="86.36" x2="30.48" y2="86.36" width="0.1524" layer="91"/>
<junction x="30.48" y="86.36"/>
<pinref part="U$7" gate="G$1" pin="A6"/>
<wire x1="22.86" y1="88.9" x2="30.48" y2="88.9" width="0.1524" layer="91"/>
<junction x="30.48" y="88.9"/>
<pinref part="U$7" gate="G$1" pin="A5"/>
<wire x1="22.86" y1="91.44" x2="30.48" y2="91.44" width="0.1524" layer="91"/>
<junction x="30.48" y="91.44"/>
<pinref part="U$7" gate="G$1" pin="A4"/>
<wire x1="22.86" y1="93.98" x2="30.48" y2="93.98" width="0.1524" layer="91"/>
<junction x="30.48" y="93.98"/>
<pinref part="U$7" gate="G$1" pin="A3"/>
<wire x1="22.86" y1="96.52" x2="30.48" y2="96.52" width="0.1524" layer="91"/>
<junction x="30.48" y="96.52"/>
<pinref part="U$7" gate="G$1" pin="A2"/>
<wire x1="22.86" y1="99.06" x2="30.48" y2="99.06" width="0.1524" layer="91"/>
<junction x="30.48" y="99.06"/>
<wire x1="27.94" y1="25.4" x2="35.56" y2="25.4" width="0.1524" layer="91"/>
<junction x="27.94" y="25.4"/>
<wire x1="35.56" y1="25.4" x2="35.56" y2="20.32" width="0.1524" layer="91"/>
<pinref part="U$20" gate="G$1" pin="GND"/>
<wire x1="35.56" y1="20.32" x2="45.72" y2="20.32" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="B1"/>
<wire x1="48.26" y1="101.6" x2="55.88" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY38" gate="GND" pin="GND"/>
<wire x1="55.88" y1="101.6" x2="55.88" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$7" gate="G$1" pin="B2"/>
<wire x1="55.88" y1="99.06" x2="55.88" y2="81.28" width="0.1524" layer="91"/>
<wire x1="55.88" y1="81.28" x2="55.88" y2="63.5" width="0.1524" layer="91"/>
<wire x1="55.88" y1="63.5" x2="55.88" y2="45.72" width="0.1524" layer="91"/>
<wire x1="55.88" y1="45.72" x2="55.88" y2="27.94" width="0.1524" layer="91"/>
<wire x1="55.88" y1="27.94" x2="55.88" y2="22.86" width="0.1524" layer="91"/>
<wire x1="48.26" y1="99.06" x2="55.88" y2="99.06" width="0.1524" layer="91"/>
<junction x="55.88" y="99.06"/>
<pinref part="U$7" gate="G$1" pin="B9"/>
<wire x1="48.26" y1="81.28" x2="55.88" y2="81.28" width="0.1524" layer="91"/>
<junction x="55.88" y="81.28"/>
<pinref part="U$7" gate="G$1" pin="B16"/>
<wire x1="48.26" y1="63.5" x2="55.88" y2="63.5" width="0.1524" layer="91"/>
<junction x="55.88" y="63.5"/>
<pinref part="U$7" gate="G$1" pin="B23"/>
<wire x1="55.88" y1="45.72" x2="48.26" y2="45.72" width="0.1524" layer="91"/>
<junction x="55.88" y="45.72"/>
<pinref part="U$7" gate="G$1" pin="B30"/>
<wire x1="48.26" y1="27.94" x2="55.88" y2="27.94" width="0.1524" layer="91"/>
<junction x="55.88" y="27.94"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="C1"/>
<wire x1="73.66" y1="101.6" x2="81.28" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY39" gate="GND" pin="GND"/>
<wire x1="81.28" y1="101.6" x2="81.28" y2="99.06" width="0.1524" layer="91"/>
<wire x1="81.28" y1="99.06" x2="81.28" y2="81.28" width="0.1524" layer="91"/>
<wire x1="81.28" y1="81.28" x2="81.28" y2="63.5" width="0.1524" layer="91"/>
<wire x1="81.28" y1="63.5" x2="81.28" y2="45.72" width="0.1524" layer="91"/>
<wire x1="81.28" y1="45.72" x2="81.28" y2="27.94" width="0.1524" layer="91"/>
<wire x1="81.28" y1="27.94" x2="81.28" y2="22.86" width="0.1524" layer="91"/>
<wire x1="81.28" y1="22.86" x2="76.2" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$7" gate="G$1" pin="C23"/>
<wire x1="73.66" y1="45.72" x2="81.28" y2="45.72" width="0.1524" layer="91"/>
<junction x="81.28" y="45.72"/>
<pinref part="U$7" gate="G$1" pin="C16"/>
<wire x1="73.66" y1="63.5" x2="81.28" y2="63.5" width="0.1524" layer="91"/>
<junction x="81.28" y="63.5"/>
<pinref part="U$7" gate="G$1" pin="C9"/>
<wire x1="73.66" y1="81.28" x2="81.28" y2="81.28" width="0.1524" layer="91"/>
<junction x="81.28" y="81.28"/>
<pinref part="U$7" gate="G$1" pin="C2"/>
<wire x1="73.66" y1="99.06" x2="81.28" y2="99.06" width="0.1524" layer="91"/>
<junction x="81.28" y="99.06"/>
<pinref part="U$7" gate="G$1" pin="C30"/>
<wire x1="73.66" y1="27.94" x2="81.28" y2="27.94" width="0.1524" layer="91"/>
<junction x="81.28" y="27.94"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="D1"/>
<wire x1="99.06" y1="101.6" x2="106.68" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY40" gate="GND" pin="GND"/>
<wire x1="106.68" y1="101.6" x2="106.68" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$7" gate="G$1" pin="D30"/>
<wire x1="106.68" y1="99.06" x2="106.68" y2="81.28" width="0.1524" layer="91"/>
<wire x1="106.68" y1="81.28" x2="106.68" y2="63.5" width="0.1524" layer="91"/>
<wire x1="106.68" y1="63.5" x2="106.68" y2="45.72" width="0.1524" layer="91"/>
<wire x1="106.68" y1="45.72" x2="106.68" y2="27.94" width="0.1524" layer="91"/>
<wire x1="106.68" y1="27.94" x2="106.68" y2="22.86" width="0.1524" layer="91"/>
<wire x1="99.06" y1="27.94" x2="106.68" y2="27.94" width="0.1524" layer="91"/>
<junction x="106.68" y="27.94"/>
<pinref part="U$7" gate="G$1" pin="D23"/>
<wire x1="99.06" y1="45.72" x2="106.68" y2="45.72" width="0.1524" layer="91"/>
<junction x="106.68" y="45.72"/>
<pinref part="U$7" gate="G$1" pin="D16"/>
<wire x1="99.06" y1="63.5" x2="106.68" y2="63.5" width="0.1524" layer="91"/>
<junction x="106.68" y="63.5"/>
<pinref part="U$7" gate="G$1" pin="D9"/>
<wire x1="99.06" y1="81.28" x2="106.68" y2="81.28" width="0.1524" layer="91"/>
<junction x="106.68" y="81.28"/>
<pinref part="U$7" gate="G$1" pin="D2"/>
<wire x1="99.06" y1="99.06" x2="106.68" y2="99.06" width="0.1524" layer="91"/>
<junction x="106.68" y="99.06"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="E1"/>
<wire x1="124.46" y1="101.6" x2="132.08" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY41" gate="GND" pin="GND"/>
<wire x1="132.08" y1="101.6" x2="132.08" y2="27.94" width="0.1524" layer="91"/>
<wire x1="132.08" y1="27.94" x2="132.08" y2="22.86" width="0.1524" layer="91"/>
<wire x1="132.08" y1="22.86" x2="127" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$7" gate="G$1" pin="E30"/>
<wire x1="124.46" y1="27.94" x2="132.08" y2="27.94" width="0.1524" layer="91"/>
<junction x="132.08" y="27.94"/>
</segment>
<segment>
<pinref part="U$7" gate="G$1" pin="F1"/>
<wire x1="149.86" y1="101.6" x2="157.48" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY42" gate="GND" pin="GND"/>
<wire x1="157.48" y1="101.6" x2="157.48" y2="27.94" width="0.1524" layer="91"/>
<wire x1="157.48" y1="27.94" x2="157.48" y2="22.86" width="0.1524" layer="91"/>
<wire x1="157.48" y1="22.86" x2="154.94" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$7" gate="G$1" pin="F30"/>
<wire x1="149.86" y1="27.94" x2="157.48" y2="27.94" width="0.1524" layer="91"/>
<junction x="157.48" y="27.94"/>
</segment>
<segment>
<pinref part="U$8" gate="G$1" pin="F1"/>
<wire x1="144.78" y1="12.7" x2="152.4" y2="12.7" width="0.1524" layer="91"/>
<pinref part="SUPPLY48" gate="GND" pin="GND"/>
<wire x1="152.4" y1="12.7" x2="152.4" y2="-60.96" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="F30"/>
<wire x1="152.4" y1="-60.96" x2="152.4" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="144.78" y1="-60.96" x2="152.4" y2="-60.96" width="0.1524" layer="91"/>
<junction x="152.4" y="-60.96"/>
</segment>
<segment>
<pinref part="U$8" gate="G$1" pin="E1"/>
<wire x1="119.38" y1="12.7" x2="127" y2="12.7" width="0.1524" layer="91"/>
<pinref part="SUPPLY47" gate="GND" pin="GND"/>
<wire x1="127" y1="12.7" x2="127" y2="-60.96" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="E30"/>
<wire x1="127" y1="-60.96" x2="127" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="119.38" y1="-60.96" x2="127" y2="-60.96" width="0.1524" layer="91"/>
<junction x="127" y="-60.96"/>
</segment>
<segment>
<pinref part="U$8" gate="G$1" pin="D1"/>
<wire x1="93.98" y1="12.7" x2="101.6" y2="12.7" width="0.1524" layer="91"/>
<wire x1="101.6" y1="12.7" x2="101.6" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY46" gate="GND" pin="GND"/>
<wire x1="101.6" y1="10.16" x2="101.6" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-7.62" x2="101.6" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-25.4" x2="101.6" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-43.18" x2="101.6" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-60.96" x2="101.6" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="101.6" y1="-63.5" x2="99.06" y2="-63.5" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="D30"/>
<wire x1="93.98" y1="-60.96" x2="101.6" y2="-60.96" width="0.1524" layer="91"/>
<junction x="101.6" y="-60.96"/>
<pinref part="U$8" gate="G$1" pin="D23"/>
<wire x1="93.98" y1="-43.18" x2="101.6" y2="-43.18" width="0.1524" layer="91"/>
<junction x="101.6" y="-43.18"/>
<pinref part="U$8" gate="G$1" pin="D16"/>
<wire x1="93.98" y1="-25.4" x2="101.6" y2="-25.4" width="0.1524" layer="91"/>
<junction x="101.6" y="-25.4"/>
<pinref part="U$8" gate="G$1" pin="D9"/>
<wire x1="93.98" y1="-7.62" x2="101.6" y2="-7.62" width="0.1524" layer="91"/>
<junction x="101.6" y="-7.62"/>
<pinref part="U$8" gate="G$1" pin="D2"/>
<wire x1="93.98" y1="10.16" x2="101.6" y2="10.16" width="0.1524" layer="91"/>
<junction x="101.6" y="10.16"/>
</segment>
<segment>
<pinref part="U$8" gate="G$1" pin="C1"/>
<wire x1="68.58" y1="12.7" x2="76.2" y2="12.7" width="0.1524" layer="91"/>
<pinref part="SUPPLY45" gate="GND" pin="GND"/>
<wire x1="76.2" y1="12.7" x2="76.2" y2="10.16" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="C30"/>
<wire x1="76.2" y1="10.16" x2="76.2" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-7.62" x2="76.2" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-25.4" x2="76.2" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-43.18" x2="76.2" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="76.2" y1="-60.96" x2="76.2" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="68.58" y1="-60.96" x2="76.2" y2="-60.96" width="0.1524" layer="91"/>
<junction x="76.2" y="-60.96"/>
<pinref part="U$8" gate="G$1" pin="C23"/>
<wire x1="68.58" y1="-43.18" x2="76.2" y2="-43.18" width="0.1524" layer="91"/>
<junction x="76.2" y="-43.18"/>
<pinref part="U$8" gate="G$1" pin="C16"/>
<wire x1="68.58" y1="-25.4" x2="76.2" y2="-25.4" width="0.1524" layer="91"/>
<junction x="76.2" y="-25.4"/>
<pinref part="U$8" gate="G$1" pin="C9"/>
<wire x1="68.58" y1="-7.62" x2="76.2" y2="-7.62" width="0.1524" layer="91"/>
<junction x="76.2" y="-7.62"/>
<pinref part="U$8" gate="G$1" pin="C2"/>
<wire x1="68.58" y1="10.16" x2="76.2" y2="10.16" width="0.1524" layer="91"/>
<junction x="76.2" y="10.16"/>
</segment>
<segment>
<pinref part="U$8" gate="G$1" pin="B1"/>
<wire x1="43.18" y1="12.7" x2="50.8" y2="12.7" width="0.1524" layer="91"/>
<pinref part="SUPPLY44" gate="GND" pin="GND"/>
<wire x1="50.8" y1="12.7" x2="50.8" y2="10.16" width="0.1524" layer="91"/>
<wire x1="50.8" y1="10.16" x2="50.8" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-7.62" x2="50.8" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-25.4" x2="50.8" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-43.18" x2="50.8" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-60.96" x2="50.8" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="50.8" y1="-63.5" x2="48.26" y2="-63.5" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="B30"/>
<wire x1="43.18" y1="-60.96" x2="50.8" y2="-60.96" width="0.1524" layer="91"/>
<junction x="50.8" y="-60.96"/>
<pinref part="U$8" gate="G$1" pin="B23"/>
<wire x1="43.18" y1="-43.18" x2="50.8" y2="-43.18" width="0.1524" layer="91"/>
<junction x="50.8" y="-43.18"/>
<pinref part="U$8" gate="G$1" pin="B16"/>
<wire x1="43.18" y1="-25.4" x2="50.8" y2="-25.4" width="0.1524" layer="91"/>
<junction x="50.8" y="-25.4"/>
<pinref part="U$8" gate="G$1" pin="B9"/>
<wire x1="43.18" y1="-7.62" x2="50.8" y2="-7.62" width="0.1524" layer="91"/>
<junction x="50.8" y="-7.62"/>
<pinref part="U$8" gate="G$1" pin="B2"/>
<wire x1="43.18" y1="10.16" x2="50.8" y2="10.16" width="0.1524" layer="91"/>
<junction x="50.8" y="10.16"/>
</segment>
<segment>
<pinref part="U$8" gate="G$1" pin="A1"/>
<wire x1="17.78" y1="12.7" x2="25.4" y2="12.7" width="0.1524" layer="91"/>
<wire x1="25.4" y1="12.7" x2="25.4" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY43" gate="GND" pin="GND"/>
<wire x1="25.4" y1="10.16" x2="25.4" y2="7.62" width="0.1524" layer="91"/>
<wire x1="25.4" y1="7.62" x2="25.4" y2="5.08" width="0.1524" layer="91"/>
<wire x1="25.4" y1="5.08" x2="25.4" y2="2.54" width="0.1524" layer="91"/>
<wire x1="25.4" y1="2.54" x2="25.4" y2="0" width="0.1524" layer="91"/>
<wire x1="25.4" y1="0" x2="25.4" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-2.54" x2="25.4" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-5.08" x2="25.4" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-7.62" x2="25.4" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-10.16" x2="25.4" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-12.7" x2="25.4" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-15.24" x2="25.4" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-17.78" x2="25.4" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-20.32" x2="25.4" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-22.86" x2="25.4" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-25.4" x2="25.4" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-27.94" x2="25.4" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-30.48" x2="25.4" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-33.02" x2="25.4" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-35.56" x2="25.4" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-38.1" x2="25.4" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-40.64" x2="25.4" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-43.18" x2="25.4" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-45.72" x2="25.4" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-48.26" x2="25.4" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-50.8" x2="25.4" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-53.34" x2="25.4" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-55.88" x2="25.4" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-58.42" x2="25.4" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="25.4" y1="-63.5" x2="22.86" y2="-63.5" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="A29"/>
<wire x1="17.78" y1="-58.42" x2="25.4" y2="-58.42" width="0.1524" layer="91"/>
<junction x="25.4" y="-58.42"/>
<pinref part="U$8" gate="G$1" pin="A28"/>
<wire x1="17.78" y1="-55.88" x2="25.4" y2="-55.88" width="0.1524" layer="91"/>
<junction x="25.4" y="-55.88"/>
<pinref part="U$8" gate="G$1" pin="A27"/>
<wire x1="17.78" y1="-53.34" x2="25.4" y2="-53.34" width="0.1524" layer="91"/>
<junction x="25.4" y="-53.34"/>
<pinref part="U$8" gate="G$1" pin="A26"/>
<wire x1="17.78" y1="-50.8" x2="25.4" y2="-50.8" width="0.1524" layer="91"/>
<junction x="25.4" y="-50.8"/>
<pinref part="U$8" gate="G$1" pin="A25"/>
<wire x1="17.78" y1="-48.26" x2="25.4" y2="-48.26" width="0.1524" layer="91"/>
<junction x="25.4" y="-48.26"/>
<pinref part="U$8" gate="G$1" pin="A24"/>
<wire x1="17.78" y1="-45.72" x2="25.4" y2="-45.72" width="0.1524" layer="91"/>
<junction x="25.4" y="-45.72"/>
<pinref part="U$8" gate="G$1" pin="A23"/>
<wire x1="17.78" y1="-43.18" x2="25.4" y2="-43.18" width="0.1524" layer="91"/>
<junction x="25.4" y="-43.18"/>
<pinref part="U$8" gate="G$1" pin="A22"/>
<wire x1="17.78" y1="-40.64" x2="25.4" y2="-40.64" width="0.1524" layer="91"/>
<junction x="25.4" y="-40.64"/>
<pinref part="U$8" gate="G$1" pin="A21"/>
<wire x1="17.78" y1="-38.1" x2="25.4" y2="-38.1" width="0.1524" layer="91"/>
<junction x="25.4" y="-38.1"/>
<pinref part="U$8" gate="G$1" pin="A20"/>
<wire x1="17.78" y1="-35.56" x2="25.4" y2="-35.56" width="0.1524" layer="91"/>
<junction x="25.4" y="-35.56"/>
<pinref part="U$8" gate="G$1" pin="A19"/>
<wire x1="17.78" y1="-33.02" x2="25.4" y2="-33.02" width="0.1524" layer="91"/>
<junction x="25.4" y="-33.02"/>
<pinref part="U$8" gate="G$1" pin="A18"/>
<wire x1="17.78" y1="-30.48" x2="25.4" y2="-30.48" width="0.1524" layer="91"/>
<junction x="25.4" y="-30.48"/>
<pinref part="U$8" gate="G$1" pin="A17"/>
<wire x1="17.78" y1="-27.94" x2="25.4" y2="-27.94" width="0.1524" layer="91"/>
<junction x="25.4" y="-27.94"/>
<pinref part="U$8" gate="G$1" pin="A16"/>
<wire x1="17.78" y1="-25.4" x2="25.4" y2="-25.4" width="0.1524" layer="91"/>
<junction x="25.4" y="-25.4"/>
<pinref part="U$8" gate="G$1" pin="A15"/>
<wire x1="17.78" y1="-22.86" x2="25.4" y2="-22.86" width="0.1524" layer="91"/>
<junction x="25.4" y="-22.86"/>
<pinref part="U$8" gate="G$1" pin="A14"/>
<wire x1="17.78" y1="-20.32" x2="25.4" y2="-20.32" width="0.1524" layer="91"/>
<junction x="25.4" y="-20.32"/>
<pinref part="U$8" gate="G$1" pin="A13"/>
<wire x1="17.78" y1="-17.78" x2="25.4" y2="-17.78" width="0.1524" layer="91"/>
<junction x="25.4" y="-17.78"/>
<pinref part="U$8" gate="G$1" pin="A12"/>
<wire x1="17.78" y1="-15.24" x2="25.4" y2="-15.24" width="0.1524" layer="91"/>
<junction x="25.4" y="-15.24"/>
<pinref part="U$8" gate="G$1" pin="A11"/>
<junction x="25.4" y="-12.7"/>
<wire x1="17.78" y1="-12.7" x2="25.4" y2="-12.7" width="0.1524" layer="91"/>
<pinref part="U$8" gate="G$1" pin="A10"/>
<wire x1="17.78" y1="-10.16" x2="25.4" y2="-10.16" width="0.1524" layer="91"/>
<junction x="25.4" y="-10.16"/>
<pinref part="U$8" gate="G$1" pin="A9"/>
<wire x1="17.78" y1="-7.62" x2="25.4" y2="-7.62" width="0.1524" layer="91"/>
<junction x="25.4" y="-7.62"/>
<pinref part="U$8" gate="G$1" pin="A8"/>
<wire x1="17.78" y1="-5.08" x2="25.4" y2="-5.08" width="0.1524" layer="91"/>
<junction x="25.4" y="-5.08"/>
<pinref part="U$8" gate="G$1" pin="A7"/>
<wire x1="17.78" y1="-2.54" x2="25.4" y2="-2.54" width="0.1524" layer="91"/>
<junction x="25.4" y="-2.54"/>
<pinref part="U$8" gate="G$1" pin="A6"/>
<wire x1="17.78" y1="0" x2="25.4" y2="0" width="0.1524" layer="91"/>
<junction x="25.4" y="0"/>
<pinref part="U$8" gate="G$1" pin="A5"/>
<wire x1="17.78" y1="2.54" x2="25.4" y2="2.54" width="0.1524" layer="91"/>
<junction x="25.4" y="2.54"/>
<pinref part="U$8" gate="G$1" pin="A4"/>
<wire x1="17.78" y1="5.08" x2="25.4" y2="5.08" width="0.1524" layer="91"/>
<junction x="25.4" y="5.08"/>
<pinref part="U$8" gate="G$1" pin="A3"/>
<wire x1="17.78" y1="7.62" x2="25.4" y2="7.62" width="0.1524" layer="91"/>
<junction x="25.4" y="7.62"/>
<pinref part="U$8" gate="G$1" pin="A2"/>
<wire x1="17.78" y1="10.16" x2="25.4" y2="10.16" width="0.1524" layer="91"/>
<junction x="25.4" y="10.16"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="A30"/>
<wire x1="22.86" y1="27.94" x2="38.1" y2="27.94" width="0.1524" layer="91"/>
<wire x1="38.1" y1="27.94" x2="38.1" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$20" gate="G$1" pin="IO"/>
<wire x1="38.1" y1="22.86" x2="45.72" y2="22.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.09.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B3"/>
<wire x1="48.26" y1="96.52" x2="58.42" y2="96.52" width="0.1016" layer="91"/>
<label x="58.42" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B4"/>
<wire x1="48.26" y1="93.98" x2="58.42" y2="93.98" width="0.1016" layer="91"/>
<label x="58.42" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B5"/>
<wire x1="48.26" y1="91.44" x2="58.42" y2="91.44" width="0.1016" layer="91"/>
<label x="58.42" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B6"/>
<wire x1="48.26" y1="88.9" x2="58.42" y2="88.9" width="0.1016" layer="91"/>
<label x="58.42" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.048" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B7"/>
<wire x1="48.26" y1="86.36" x2="58.42" y2="86.36" width="0.1016" layer="91"/>
<label x="58.42" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B8"/>
<wire x1="48.26" y1="83.82" x2="58.42" y2="83.82" width="0.1016" layer="91"/>
<label x="58.42" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B10"/>
<wire x1="48.26" y1="78.74" x2="58.42" y2="78.74" width="0.1016" layer="91"/>
<label x="58.42" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B11"/>
<wire x1="48.26" y1="76.2" x2="58.42" y2="76.2" width="0.1016" layer="91"/>
<label x="58.42" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.048" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B12"/>
<wire x1="48.26" y1="73.66" x2="58.42" y2="73.66" width="0.1016" layer="91"/>
<label x="58.42" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.048" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B13"/>
<wire x1="48.26" y1="71.12" x2="58.42" y2="71.12" width="0.1016" layer="91"/>
<label x="58.42" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B14"/>
<wire x1="48.26" y1="68.58" x2="58.42" y2="68.58" width="0.1016" layer="91"/>
<label x="58.42" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B15"/>
<wire x1="48.26" y1="66.04" x2="58.42" y2="66.04" width="0.1016" layer="91"/>
<label x="58.42" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B17"/>
<wire x1="48.26" y1="60.96" x2="58.42" y2="60.96" width="0.1016" layer="91"/>
<label x="58.42" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B18"/>
<wire x1="48.26" y1="58.42" x2="58.42" y2="58.42" width="0.1016" layer="91"/>
<label x="58.42" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B19"/>
<wire x1="48.26" y1="55.88" x2="58.42" y2="55.88" width="0.1016" layer="91"/>
<label x="58.42" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B20"/>
<wire x1="48.26" y1="53.34" x2="58.42" y2="53.34" width="0.1016" layer="91"/>
<label x="58.42" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B21"/>
<wire x1="48.26" y1="50.8" x2="58.42" y2="50.8" width="0.1016" layer="91"/>
<label x="58.42" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B22"/>
<wire x1="48.26" y1="48.26" x2="58.42" y2="48.26" width="0.1016" layer="91"/>
<label x="58.42" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B24"/>
<wire x1="48.26" y1="43.18" x2="58.42" y2="43.18" width="0.1016" layer="91"/>
<label x="58.42" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B25"/>
<wire x1="48.26" y1="40.64" x2="58.42" y2="40.64" width="0.1016" layer="91"/>
<label x="58.42" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.048" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B26"/>
<wire x1="48.26" y1="38.1" x2="58.42" y2="38.1" width="0.1016" layer="91"/>
<label x="58.42" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.048" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B27"/>
<wire x1="48.26" y1="35.56" x2="58.42" y2="35.56" width="0.1016" layer="91"/>
<label x="58.42" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B28"/>
<wire x1="48.26" y1="33.02" x2="58.42" y2="33.02" width="0.1016" layer="91"/>
<label x="58.42" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="B29"/>
<wire x1="48.26" y1="30.48" x2="58.42" y2="30.48" width="0.1016" layer="91"/>
<label x="58.42" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C3"/>
<wire x1="73.66" y1="96.52" x2="83.82" y2="96.52" width="0.1016" layer="91"/>
<label x="83.82" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C4"/>
<wire x1="73.66" y1="93.98" x2="83.82" y2="93.98" width="0.1016" layer="91"/>
<label x="83.82" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C5"/>
<wire x1="73.66" y1="91.44" x2="83.82" y2="91.44" width="0.1016" layer="91"/>
<label x="83.82" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C6"/>
<wire x1="73.66" y1="88.9" x2="83.82" y2="88.9" width="0.1016" layer="91"/>
<label x="83.82" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.048" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C7"/>
<wire x1="73.66" y1="86.36" x2="83.82" y2="86.36" width="0.1016" layer="91"/>
<label x="83.82" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.062" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C8"/>
<wire x1="73.66" y1="83.82" x2="83.82" y2="83.82" width="0.1016" layer="91"/>
<label x="83.82" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C10"/>
<wire x1="73.66" y1="78.74" x2="83.82" y2="78.74" width="0.1016" layer="91"/>
<label x="83.82" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C11"/>
<wire x1="73.66" y1="76.2" x2="83.82" y2="76.2" width="0.1016" layer="91"/>
<label x="83.82" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C12"/>
<wire x1="73.66" y1="73.66" x2="83.82" y2="73.66" width="0.1016" layer="91"/>
<label x="83.82" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C13"/>
<wire x1="73.66" y1="71.12" x2="83.82" y2="71.12" width="0.1016" layer="91"/>
<label x="83.82" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C14"/>
<wire x1="73.66" y1="68.58" x2="83.82" y2="68.58" width="0.1016" layer="91"/>
<label x="83.82" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C15"/>
<wire x1="73.66" y1="66.04" x2="83.82" y2="66.04" width="0.1016" layer="91"/>
<label x="83.82" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.045" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C17"/>
<wire x1="73.66" y1="60.96" x2="83.82" y2="60.96" width="0.1016" layer="91"/>
<label x="83.82" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.045" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C18"/>
<wire x1="73.66" y1="58.42" x2="83.82" y2="58.42" width="0.1016" layer="91"/>
<label x="83.82" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C19"/>
<wire x1="73.66" y1="55.88" x2="83.82" y2="55.88" width="0.1016" layer="91"/>
<label x="83.82" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C20"/>
<wire x1="73.66" y1="53.34" x2="83.82" y2="53.34" width="0.1016" layer="91"/>
<label x="83.82" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C21"/>
<wire x1="73.66" y1="50.8" x2="83.82" y2="50.8" width="0.1016" layer="91"/>
<label x="83.82" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C22"/>
<wire x1="73.66" y1="48.26" x2="83.82" y2="48.26" width="0.1016" layer="91"/>
<label x="83.82" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C24"/>
<wire x1="73.66" y1="43.18" x2="83.82" y2="43.18" width="0.1016" layer="91"/>
<label x="83.82" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C25"/>
<wire x1="73.66" y1="40.64" x2="83.82" y2="40.64" width="0.1016" layer="91"/>
<label x="83.82" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C26"/>
<wire x1="73.66" y1="38.1" x2="83.82" y2="38.1" width="0.1016" layer="91"/>
<label x="83.82" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C27"/>
<wire x1="73.66" y1="35.56" x2="83.82" y2="35.56" width="0.1016" layer="91"/>
<label x="83.82" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C28"/>
<wire x1="73.66" y1="33.02" x2="83.82" y2="33.02" width="0.1016" layer="91"/>
<label x="83.82" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.052" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="C29"/>
<wire x1="73.66" y1="30.48" x2="83.82" y2="30.48" width="0.1016" layer="91"/>
<label x="83.82" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D3"/>
<wire x1="99.06" y1="96.52" x2="109.22" y2="96.52" width="0.1016" layer="91"/>
<label x="109.22" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D4"/>
<wire x1="99.06" y1="93.98" x2="109.22" y2="93.98" width="0.1016" layer="91"/>
<label x="109.22" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D5"/>
<wire x1="99.06" y1="91.44" x2="109.22" y2="91.44" width="0.1016" layer="91"/>
<label x="109.22" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D6"/>
<wire x1="99.06" y1="88.9" x2="109.22" y2="88.9" width="0.1016" layer="91"/>
<label x="109.22" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D7"/>
<wire x1="99.06" y1="86.36" x2="109.22" y2="86.36" width="0.1016" layer="91"/>
<label x="109.22" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.062" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D8"/>
<wire x1="99.06" y1="83.82" x2="109.22" y2="83.82" width="0.1016" layer="91"/>
<label x="109.22" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D10"/>
<wire x1="99.06" y1="78.74" x2="109.22" y2="78.74" width="0.1016" layer="91"/>
<label x="109.22" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D11"/>
<wire x1="99.06" y1="76.2" x2="109.22" y2="76.2" width="0.1016" layer="91"/>
<label x="109.22" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D12"/>
<wire x1="99.06" y1="73.66" x2="109.22" y2="73.66" width="0.1016" layer="91"/>
<label x="109.22" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D13"/>
<wire x1="99.06" y1="71.12" x2="109.22" y2="71.12" width="0.1016" layer="91"/>
<label x="109.22" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D14"/>
<wire x1="99.06" y1="68.58" x2="109.22" y2="68.58" width="0.1016" layer="91"/>
<label x="109.22" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D15"/>
<wire x1="99.06" y1="66.04" x2="109.22" y2="66.04" width="0.1016" layer="91"/>
<label x="109.22" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D17"/>
<wire x1="99.06" y1="60.96" x2="109.22" y2="60.96" width="0.1016" layer="91"/>
<label x="109.22" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.049" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D18"/>
<wire x1="99.06" y1="58.42" x2="109.22" y2="58.42" width="0.1016" layer="91"/>
<label x="109.22" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D19"/>
<wire x1="99.06" y1="55.88" x2="109.22" y2="55.88" width="0.1016" layer="91"/>
<label x="109.22" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.050" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D20"/>
<wire x1="99.06" y1="53.34" x2="109.22" y2="53.34" width="0.1016" layer="91"/>
<label x="109.22" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D21"/>
<wire x1="99.06" y1="50.8" x2="109.22" y2="50.8" width="0.1016" layer="91"/>
<label x="109.22" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.051" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D22"/>
<wire x1="99.06" y1="48.26" x2="109.22" y2="48.26" width="0.1016" layer="91"/>
<label x="109.22" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D24"/>
<wire x1="99.06" y1="43.18" x2="109.22" y2="43.18" width="0.1016" layer="91"/>
<label x="109.22" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D25"/>
<wire x1="99.06" y1="40.64" x2="109.22" y2="40.64" width="0.1016" layer="91"/>
<label x="109.22" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D26"/>
<wire x1="99.06" y1="38.1" x2="109.22" y2="38.1" width="0.1016" layer="91"/>
<label x="109.22" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D27"/>
<wire x1="99.06" y1="35.56" x2="109.22" y2="35.56" width="0.1016" layer="91"/>
<label x="109.22" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D28"/>
<wire x1="99.06" y1="33.02" x2="109.22" y2="33.02" width="0.1016" layer="91"/>
<label x="109.22" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.053" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="D29"/>
<wire x1="99.06" y1="30.48" x2="109.22" y2="30.48" width="0.1016" layer="91"/>
<label x="109.22" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E3"/>
<wire x1="124.46" y1="96.52" x2="134.62" y2="96.52" width="0.1016" layer="91"/>
<label x="134.62" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E4"/>
<wire x1="124.46" y1="93.98" x2="134.62" y2="93.98" width="0.1016" layer="91"/>
<label x="134.62" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E5"/>
<wire x1="124.46" y1="91.44" x2="134.62" y2="91.44" width="0.1016" layer="91"/>
<label x="134.62" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E6"/>
<wire x1="124.46" y1="88.9" x2="134.62" y2="88.9" width="0.1016" layer="91"/>
<label x="134.62" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E7"/>
<wire x1="124.46" y1="86.36" x2="134.62" y2="86.36" width="0.1016" layer="91"/>
<label x="134.62" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.063" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E8"/>
<wire x1="124.46" y1="83.82" x2="134.62" y2="83.82" width="0.1016" layer="91"/>
<label x="134.62" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E10"/>
<wire x1="124.46" y1="78.74" x2="134.62" y2="78.74" width="0.1016" layer="91"/>
<label x="134.62" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E11"/>
<wire x1="124.46" y1="76.2" x2="134.62" y2="76.2" width="0.1016" layer="91"/>
<label x="134.62" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.062" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E12"/>
<wire x1="124.46" y1="73.66" x2="134.62" y2="73.66" width="0.1016" layer="91"/>
<label x="134.62" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E13"/>
<wire x1="124.46" y1="71.12" x2="134.62" y2="71.12" width="0.1016" layer="91"/>
<label x="134.62" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E14"/>
<wire x1="124.46" y1="68.58" x2="134.62" y2="68.58" width="0.1016" layer="91"/>
<label x="134.62" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E15"/>
<wire x1="124.46" y1="66.04" x2="134.62" y2="66.04" width="0.1016" layer="91"/>
<label x="134.62" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E17"/>
<wire x1="124.46" y1="60.96" x2="134.62" y2="60.96" width="0.1016" layer="91"/>
<label x="134.62" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.069" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E18"/>
<wire x1="124.46" y1="58.42" x2="134.62" y2="58.42" width="0.1016" layer="91"/>
<label x="134.62" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E19"/>
<wire x1="124.46" y1="55.88" x2="134.62" y2="55.88" width="0.1016" layer="91"/>
<label x="134.62" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E20"/>
<wire x1="124.46" y1="53.34" x2="134.62" y2="53.34" width="0.1016" layer="91"/>
<label x="134.62" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.068" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E21"/>
<wire x1="124.46" y1="50.8" x2="134.62" y2="50.8" width="0.1016" layer="91"/>
<label x="134.62" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E22"/>
<wire x1="124.46" y1="48.26" x2="134.62" y2="48.26" width="0.1016" layer="91"/>
<label x="134.62" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E24"/>
<wire x1="124.46" y1="43.18" x2="134.62" y2="43.18" width="0.1016" layer="91"/>
<label x="134.62" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.063" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E25"/>
<wire x1="124.46" y1="40.64" x2="134.62" y2="40.64" width="0.1016" layer="91"/>
<label x="134.62" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E26"/>
<wire x1="124.46" y1="38.1" x2="134.62" y2="38.1" width="0.1016" layer="91"/>
<label x="134.62" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E27"/>
<wire x1="124.46" y1="35.56" x2="134.62" y2="35.56" width="0.1016" layer="91"/>
<label x="134.62" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.062" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E28"/>
<wire x1="124.46" y1="33.02" x2="134.62" y2="33.02" width="0.1016" layer="91"/>
<label x="134.62" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E29"/>
<wire x1="124.46" y1="30.48" x2="134.62" y2="30.48" width="0.1016" layer="91"/>
<label x="134.62" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F3"/>
<wire x1="149.86" y1="96.52" x2="160.02" y2="96.52" width="0.1016" layer="91"/>
<label x="160.02" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F4"/>
<wire x1="149.86" y1="93.98" x2="160.02" y2="93.98" width="0.1016" layer="91"/>
<label x="160.02" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F5"/>
<wire x1="149.86" y1="91.44" x2="160.02" y2="91.44" width="0.1016" layer="91"/>
<label x="160.02" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F6"/>
<wire x1="149.86" y1="88.9" x2="160.02" y2="88.9" width="0.1016" layer="91"/>
<label x="160.02" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F7"/>
<wire x1="149.86" y1="86.36" x2="160.02" y2="86.36" width="0.1016" layer="91"/>
<label x="160.02" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.063" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F8"/>
<wire x1="149.86" y1="83.82" x2="160.02" y2="83.82" width="0.1016" layer="91"/>
<label x="160.02" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F10"/>
<wire x1="149.86" y1="78.74" x2="160.02" y2="78.74" width="0.1016" layer="91"/>
<label x="160.02" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F11"/>
<wire x1="149.86" y1="76.2" x2="160.02" y2="76.2" width="0.1016" layer="91"/>
<label x="160.02" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.063" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F12"/>
<wire x1="149.86" y1="73.66" x2="160.02" y2="73.66" width="0.1016" layer="91"/>
<label x="160.02" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.062" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F13"/>
<wire x1="149.86" y1="71.12" x2="160.02" y2="71.12" width="0.1016" layer="91"/>
<label x="160.02" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F14"/>
<wire x1="149.86" y1="68.58" x2="160.02" y2="68.58" width="0.1016" layer="91"/>
<label x="160.02" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F15"/>
<wire x1="149.86" y1="66.04" x2="160.02" y2="66.04" width="0.1016" layer="91"/>
<label x="160.02" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F17"/>
<wire x1="149.86" y1="60.96" x2="160.02" y2="60.96" width="0.1016" layer="91"/>
<label x="160.02" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.069" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F18"/>
<wire x1="149.86" y1="58.42" x2="160.02" y2="58.42" width="0.1016" layer="91"/>
<label x="160.02" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F19"/>
<wire x1="149.86" y1="55.88" x2="160.02" y2="55.88" width="0.1016" layer="91"/>
<label x="160.02" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F20"/>
<wire x1="149.86" y1="53.34" x2="160.02" y2="53.34" width="0.1016" layer="91"/>
<label x="160.02" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.068" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F21"/>
<wire x1="149.86" y1="50.8" x2="160.02" y2="50.8" width="0.1016" layer="91"/>
<label x="160.02" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F22"/>
<wire x1="149.86" y1="48.26" x2="160.02" y2="48.26" width="0.1016" layer="91"/>
<label x="160.02" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F24"/>
<wire x1="149.86" y1="43.18" x2="160.02" y2="43.18" width="0.1016" layer="91"/>
<label x="160.02" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.063" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F25"/>
<wire x1="149.86" y1="40.64" x2="160.02" y2="40.64" width="0.1016" layer="91"/>
<label x="160.02" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F26"/>
<wire x1="149.86" y1="38.1" x2="160.02" y2="38.1" width="0.1016" layer="91"/>
<label x="160.02" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.058" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F27"/>
<wire x1="149.86" y1="35.56" x2="160.02" y2="35.56" width="0.1016" layer="91"/>
<label x="160.02" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.062" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F28"/>
<wire x1="149.86" y1="33.02" x2="160.02" y2="33.02" width="0.1016" layer="91"/>
<label x="160.02" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.054" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F29"/>
<wire x1="149.86" y1="30.48" x2="160.02" y2="30.48" width="0.1016" layer="91"/>
<label x="160.02" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.057" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E2"/>
<wire x1="124.46" y1="99.06" x2="134.62" y2="99.06" width="0.1016" layer="91"/>
<label x="134.62" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.059" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F2"/>
<wire x1="149.86" y1="99.06" x2="160.02" y2="99.06" width="0.1016" layer="91"/>
<label x="160.02" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E9"/>
<wire x1="124.46" y1="81.28" x2="134.62" y2="81.28" width="0.1016" layer="91"/>
<label x="134.62" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.063" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F9"/>
<wire x1="149.86" y1="81.28" x2="160.02" y2="81.28" width="0.1016" layer="91"/>
<label x="160.02" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.061" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E16"/>
<wire x1="124.46" y1="63.5" x2="134.62" y2="63.5" width="0.1016" layer="91"/>
<label x="134.62" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.060" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F16"/>
<wire x1="149.86" y1="63.5" x2="160.02" y2="63.5" width="0.1016" layer="91"/>
<label x="160.02" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.056" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="E23"/>
<wire x1="124.46" y1="45.72" x2="134.62" y2="45.72" width="0.1016" layer="91"/>
<label x="134.62" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.055" class="0">
<segment>
<pinref part="U$7" gate="G$1" pin="F23"/>
<wire x1="149.86" y1="45.72" x2="160.02" y2="45.72" width="0.1016" layer="91"/>
<label x="160.02" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B3"/>
<wire x1="43.18" y1="7.62" x2="53.34" y2="7.62" width="0.1016" layer="91"/>
<label x="53.34" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B4"/>
<wire x1="43.18" y1="5.08" x2="53.34" y2="5.08" width="0.1016" layer="91"/>
<label x="53.34" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B5"/>
<wire x1="43.18" y1="2.54" x2="53.34" y2="2.54" width="0.1016" layer="91"/>
<label x="53.34" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B6"/>
<wire x1="43.18" y1="0" x2="53.34" y2="0" width="0.1016" layer="91"/>
<label x="53.34" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B7"/>
<wire x1="43.18" y1="-2.54" x2="53.34" y2="-2.54" width="0.1016" layer="91"/>
<label x="53.34" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B8"/>
<wire x1="43.18" y1="-5.08" x2="53.34" y2="-5.08" width="0.1016" layer="91"/>
<label x="53.34" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B10"/>
<wire x1="43.18" y1="-10.16" x2="53.34" y2="-10.16" width="0.1016" layer="91"/>
<label x="53.34" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B11"/>
<wire x1="43.18" y1="-12.7" x2="53.34" y2="-12.7" width="0.1016" layer="91"/>
<label x="53.34" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B12"/>
<wire x1="43.18" y1="-15.24" x2="53.34" y2="-15.24" width="0.1016" layer="91"/>
<label x="53.34" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B13"/>
<wire x1="43.18" y1="-17.78" x2="53.34" y2="-17.78" width="0.1016" layer="91"/>
<label x="53.34" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B14"/>
<wire x1="43.18" y1="-20.32" x2="53.34" y2="-20.32" width="0.1016" layer="91"/>
<label x="53.34" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B15"/>
<wire x1="43.18" y1="-22.86" x2="53.34" y2="-22.86" width="0.1016" layer="91"/>
<label x="53.34" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B17"/>
<wire x1="43.18" y1="-27.94" x2="53.34" y2="-27.94" width="0.1016" layer="91"/>
<label x="53.34" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B18"/>
<wire x1="43.18" y1="-30.48" x2="53.34" y2="-30.48" width="0.1016" layer="91"/>
<label x="53.34" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B19"/>
<wire x1="43.18" y1="-33.02" x2="53.34" y2="-33.02" width="0.1016" layer="91"/>
<label x="53.34" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B20"/>
<wire x1="43.18" y1="-35.56" x2="53.34" y2="-35.56" width="0.1016" layer="91"/>
<label x="53.34" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B21"/>
<wire x1="43.18" y1="-38.1" x2="53.34" y2="-38.1" width="0.1016" layer="91"/>
<label x="53.34" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B22"/>
<wire x1="43.18" y1="-40.64" x2="53.34" y2="-40.64" width="0.1016" layer="91"/>
<label x="53.34" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B24"/>
<wire x1="43.18" y1="-45.72" x2="53.34" y2="-45.72" width="0.1016" layer="91"/>
<label x="53.34" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B25"/>
<wire x1="43.18" y1="-48.26" x2="53.34" y2="-48.26" width="0.1016" layer="91"/>
<label x="53.34" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B26"/>
<wire x1="43.18" y1="-50.8" x2="53.34" y2="-50.8" width="0.1016" layer="91"/>
<label x="53.34" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B27"/>
<wire x1="43.18" y1="-53.34" x2="53.34" y2="-53.34" width="0.1016" layer="91"/>
<label x="53.34" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B28"/>
<wire x1="43.18" y1="-55.88" x2="53.34" y2="-55.88" width="0.1016" layer="91"/>
<label x="53.34" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="B29"/>
<wire x1="43.18" y1="-58.42" x2="53.34" y2="-58.42" width="0.1016" layer="91"/>
<label x="53.34" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C3"/>
<wire x1="68.58" y1="7.62" x2="78.74" y2="7.62" width="0.1016" layer="91"/>
<label x="78.74" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C4"/>
<wire x1="68.58" y1="5.08" x2="78.74" y2="5.08" width="0.1016" layer="91"/>
<label x="78.74" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C5"/>
<wire x1="68.58" y1="2.54" x2="78.74" y2="2.54" width="0.1016" layer="91"/>
<label x="78.74" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C6"/>
<wire x1="68.58" y1="0" x2="78.74" y2="0" width="0.1016" layer="91"/>
<label x="78.74" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C7"/>
<wire x1="68.58" y1="-2.54" x2="78.74" y2="-2.54" width="0.1016" layer="91"/>
<label x="78.74" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C8"/>
<wire x1="68.58" y1="-5.08" x2="78.74" y2="-5.08" width="0.1016" layer="91"/>
<label x="78.74" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C10"/>
<wire x1="68.58" y1="-10.16" x2="78.74" y2="-10.16" width="0.1016" layer="91"/>
<label x="78.74" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C11"/>
<wire x1="68.58" y1="-12.7" x2="78.74" y2="-12.7" width="0.1016" layer="91"/>
<label x="78.74" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C12"/>
<wire x1="68.58" y1="-15.24" x2="78.74" y2="-15.24" width="0.1016" layer="91"/>
<label x="78.74" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.049" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C13"/>
<wire x1="68.58" y1="-17.78" x2="78.74" y2="-17.78" width="0.1016" layer="91"/>
<label x="78.74" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.048" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C14"/>
<wire x1="68.58" y1="-20.32" x2="78.74" y2="-20.32" width="0.1016" layer="91"/>
<label x="78.74" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C15"/>
<wire x1="68.58" y1="-22.86" x2="78.74" y2="-22.86" width="0.1016" layer="91"/>
<label x="78.74" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C17"/>
<wire x1="68.58" y1="-27.94" x2="78.74" y2="-27.94" width="0.1016" layer="91"/>
<label x="78.74" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C18"/>
<wire x1="68.58" y1="-30.48" x2="78.74" y2="-30.48" width="0.1016" layer="91"/>
<label x="78.74" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C19"/>
<wire x1="68.58" y1="-33.02" x2="78.74" y2="-33.02" width="0.1016" layer="91"/>
<label x="78.74" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C20"/>
<wire x1="68.58" y1="-35.56" x2="78.74" y2="-35.56" width="0.1016" layer="91"/>
<label x="78.74" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C21"/>
<wire x1="68.58" y1="-38.1" x2="78.74" y2="-38.1" width="0.1016" layer="91"/>
<label x="78.74" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C22"/>
<wire x1="68.58" y1="-40.64" x2="78.74" y2="-40.64" width="0.1016" layer="91"/>
<label x="78.74" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C24"/>
<wire x1="68.58" y1="-45.72" x2="78.74" y2="-45.72" width="0.1016" layer="91"/>
<label x="78.74" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C25"/>
<wire x1="68.58" y1="-48.26" x2="78.74" y2="-48.26" width="0.1016" layer="91"/>
<label x="78.74" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C26"/>
<wire x1="68.58" y1="-50.8" x2="78.74" y2="-50.8" width="0.1016" layer="91"/>
<label x="78.74" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C27"/>
<wire x1="68.58" y1="-53.34" x2="78.74" y2="-53.34" width="0.1016" layer="91"/>
<label x="78.74" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C28"/>
<wire x1="68.58" y1="-55.88" x2="78.74" y2="-55.88" width="0.1016" layer="91"/>
<label x="78.74" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.052" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="C29"/>
<wire x1="68.58" y1="-58.42" x2="78.74" y2="-58.42" width="0.1016" layer="91"/>
<label x="78.74" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D3"/>
<wire x1="93.98" y1="7.62" x2="104.14" y2="7.62" width="0.1016" layer="91"/>
<label x="104.14" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D4"/>
<wire x1="93.98" y1="5.08" x2="104.14" y2="5.08" width="0.1016" layer="91"/>
<label x="104.14" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D5"/>
<wire x1="93.98" y1="2.54" x2="104.14" y2="2.54" width="0.1016" layer="91"/>
<label x="104.14" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D6"/>
<wire x1="93.98" y1="0" x2="104.14" y2="0" width="0.1016" layer="91"/>
<label x="104.14" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D7"/>
<wire x1="93.98" y1="-2.54" x2="104.14" y2="-2.54" width="0.1016" layer="91"/>
<label x="104.14" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D8"/>
<wire x1="93.98" y1="-5.08" x2="104.14" y2="-5.08" width="0.1016" layer="91"/>
<label x="104.14" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D10"/>
<wire x1="93.98" y1="-10.16" x2="104.14" y2="-10.16" width="0.1016" layer="91"/>
<label x="104.14" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D11"/>
<wire x1="93.98" y1="-12.7" x2="104.14" y2="-12.7" width="0.1016" layer="91"/>
<label x="104.14" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D12"/>
<wire x1="93.98" y1="-15.24" x2="104.14" y2="-15.24" width="0.1016" layer="91"/>
<label x="104.14" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.050" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D13"/>
<wire x1="93.98" y1="-17.78" x2="104.14" y2="-17.78" width="0.1016" layer="91"/>
<label x="104.14" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D14"/>
<wire x1="93.98" y1="-20.32" x2="104.14" y2="-20.32" width="0.1016" layer="91"/>
<label x="104.14" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D15"/>
<wire x1="93.98" y1="-22.86" x2="104.14" y2="-22.86" width="0.1016" layer="91"/>
<label x="104.14" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D17"/>
<wire x1="93.98" y1="-27.94" x2="104.14" y2="-27.94" width="0.1016" layer="91"/>
<label x="104.14" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D18"/>
<wire x1="93.98" y1="-30.48" x2="104.14" y2="-30.48" width="0.1016" layer="91"/>
<label x="104.14" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D19"/>
<wire x1="93.98" y1="-33.02" x2="104.14" y2="-33.02" width="0.1016" layer="91"/>
<label x="104.14" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D20"/>
<wire x1="93.98" y1="-35.56" x2="104.14" y2="-35.56" width="0.1016" layer="91"/>
<label x="104.14" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D21"/>
<wire x1="93.98" y1="-38.1" x2="104.14" y2="-38.1" width="0.1016" layer="91"/>
<label x="104.14" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D22"/>
<wire x1="93.98" y1="-40.64" x2="104.14" y2="-40.64" width="0.1016" layer="91"/>
<label x="104.14" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D24"/>
<wire x1="93.98" y1="-45.72" x2="104.14" y2="-45.72" width="0.1016" layer="91"/>
<label x="104.14" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.051" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D25"/>
<wire x1="93.98" y1="-48.26" x2="104.14" y2="-48.26" width="0.1016" layer="91"/>
<label x="104.14" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D26"/>
<wire x1="93.98" y1="-50.8" x2="104.14" y2="-50.8" width="0.1016" layer="91"/>
<label x="104.14" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D27"/>
<wire x1="93.98" y1="-53.34" x2="104.14" y2="-53.34" width="0.1016" layer="91"/>
<label x="104.14" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D28"/>
<wire x1="93.98" y1="-55.88" x2="104.14" y2="-55.88" width="0.1016" layer="91"/>
<label x="104.14" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.053" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="D29"/>
<wire x1="93.98" y1="-58.42" x2="104.14" y2="-58.42" width="0.1016" layer="91"/>
<label x="104.14" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E3"/>
<wire x1="119.38" y1="7.62" x2="129.54" y2="7.62" width="0.1016" layer="91"/>
<label x="129.54" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E4"/>
<wire x1="119.38" y1="5.08" x2="129.54" y2="5.08" width="0.1016" layer="91"/>
<label x="129.54" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E5"/>
<wire x1="119.38" y1="2.54" x2="129.54" y2="2.54" width="0.1016" layer="91"/>
<label x="129.54" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E6"/>
<wire x1="119.38" y1="0" x2="129.54" y2="0" width="0.1016" layer="91"/>
<label x="129.54" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E7"/>
<wire x1="119.38" y1="-2.54" x2="129.54" y2="-2.54" width="0.1016" layer="91"/>
<label x="129.54" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E8"/>
<wire x1="119.38" y1="-5.08" x2="129.54" y2="-5.08" width="0.1016" layer="91"/>
<label x="129.54" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E10"/>
<wire x1="119.38" y1="-10.16" x2="129.54" y2="-10.16" width="0.1016" layer="91"/>
<label x="129.54" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E11"/>
<wire x1="119.38" y1="-12.7" x2="129.54" y2="-12.7" width="0.1016" layer="91"/>
<label x="129.54" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E12"/>
<wire x1="119.38" y1="-15.24" x2="129.54" y2="-15.24" width="0.1016" layer="91"/>
<label x="129.54" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E13"/>
<wire x1="119.38" y1="-17.78" x2="129.54" y2="-17.78" width="0.1016" layer="91"/>
<label x="129.54" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.060" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E14"/>
<wire x1="119.38" y1="-20.32" x2="129.54" y2="-20.32" width="0.1016" layer="91"/>
<label x="129.54" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E15"/>
<wire x1="119.38" y1="-22.86" x2="129.54" y2="-22.86" width="0.1016" layer="91"/>
<label x="129.54" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E17"/>
<wire x1="119.38" y1="-27.94" x2="129.54" y2="-27.94" width="0.1016" layer="91"/>
<label x="129.54" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E18"/>
<wire x1="119.38" y1="-30.48" x2="129.54" y2="-30.48" width="0.1016" layer="91"/>
<label x="129.54" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E19"/>
<wire x1="119.38" y1="-33.02" x2="129.54" y2="-33.02" width="0.1016" layer="91"/>
<label x="129.54" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E20"/>
<wire x1="119.38" y1="-35.56" x2="129.54" y2="-35.56" width="0.1016" layer="91"/>
<label x="129.54" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E21"/>
<wire x1="119.38" y1="-38.1" x2="129.54" y2="-38.1" width="0.1016" layer="91"/>
<label x="129.54" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E22"/>
<wire x1="119.38" y1="-40.64" x2="129.54" y2="-40.64" width="0.1016" layer="91"/>
<label x="129.54" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E24"/>
<wire x1="119.38" y1="-45.72" x2="129.54" y2="-45.72" width="0.1016" layer="91"/>
<label x="129.54" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E25"/>
<wire x1="119.38" y1="-48.26" x2="129.54" y2="-48.26" width="0.1016" layer="91"/>
<label x="129.54" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E26"/>
<wire x1="119.38" y1="-50.8" x2="129.54" y2="-50.8" width="0.1016" layer="91"/>
<label x="129.54" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E27"/>
<wire x1="119.38" y1="-53.34" x2="129.54" y2="-53.34" width="0.1016" layer="91"/>
<label x="129.54" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E28"/>
<wire x1="119.38" y1="-55.88" x2="129.54" y2="-55.88" width="0.1016" layer="91"/>
<label x="129.54" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E29"/>
<wire x1="119.38" y1="-58.42" x2="129.54" y2="-58.42" width="0.1016" layer="91"/>
<label x="129.54" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F3"/>
<wire x1="144.78" y1="7.62" x2="154.94" y2="7.62" width="0.1016" layer="91"/>
<label x="154.94" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F4"/>
<wire x1="144.78" y1="5.08" x2="154.94" y2="5.08" width="0.1016" layer="91"/>
<label x="154.94" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F5"/>
<wire x1="144.78" y1="2.54" x2="154.94" y2="2.54" width="0.1016" layer="91"/>
<label x="154.94" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F6"/>
<wire x1="144.78" y1="0" x2="154.94" y2="0" width="0.1016" layer="91"/>
<label x="154.94" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F7"/>
<wire x1="144.78" y1="-2.54" x2="154.94" y2="-2.54" width="0.1016" layer="91"/>
<label x="154.94" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F8"/>
<wire x1="144.78" y1="-5.08" x2="154.94" y2="-5.08" width="0.1016" layer="91"/>
<label x="154.94" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F10"/>
<wire x1="144.78" y1="-10.16" x2="154.94" y2="-10.16" width="0.1016" layer="91"/>
<label x="154.94" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F11"/>
<wire x1="144.78" y1="-12.7" x2="154.94" y2="-12.7" width="0.1016" layer="91"/>
<label x="154.94" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F12"/>
<wire x1="144.78" y1="-15.24" x2="154.94" y2="-15.24" width="0.1016" layer="91"/>
<label x="154.94" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F13"/>
<wire x1="144.78" y1="-17.78" x2="154.94" y2="-17.78" width="0.1016" layer="91"/>
<label x="154.94" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.061" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F14"/>
<wire x1="144.78" y1="-20.32" x2="154.94" y2="-20.32" width="0.1016" layer="91"/>
<label x="154.94" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F15"/>
<wire x1="144.78" y1="-22.86" x2="154.94" y2="-22.86" width="0.1016" layer="91"/>
<label x="154.94" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F17"/>
<wire x1="144.78" y1="-27.94" x2="154.94" y2="-27.94" width="0.1016" layer="91"/>
<label x="154.94" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F18"/>
<wire x1="144.78" y1="-30.48" x2="154.94" y2="-30.48" width="0.1016" layer="91"/>
<label x="154.94" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F19"/>
<wire x1="144.78" y1="-33.02" x2="154.94" y2="-33.02" width="0.1016" layer="91"/>
<label x="154.94" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F20"/>
<wire x1="144.78" y1="-35.56" x2="154.94" y2="-35.56" width="0.1016" layer="91"/>
<label x="154.94" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.058" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F21"/>
<wire x1="144.78" y1="-38.1" x2="154.94" y2="-38.1" width="0.1016" layer="91"/>
<label x="154.94" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F22"/>
<wire x1="144.78" y1="-40.64" x2="154.94" y2="-40.64" width="0.1016" layer="91"/>
<label x="154.94" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F24"/>
<wire x1="144.78" y1="-45.72" x2="154.94" y2="-45.72" width="0.1016" layer="91"/>
<label x="154.94" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F25"/>
<wire x1="144.78" y1="-48.26" x2="154.94" y2="-48.26" width="0.1016" layer="91"/>
<label x="154.94" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F26"/>
<wire x1="144.78" y1="-50.8" x2="154.94" y2="-50.8" width="0.1016" layer="91"/>
<label x="154.94" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F27"/>
<wire x1="144.78" y1="-53.34" x2="154.94" y2="-53.34" width="0.1016" layer="91"/>
<label x="154.94" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.062" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F28"/>
<wire x1="144.78" y1="-55.88" x2="154.94" y2="-55.88" width="0.1016" layer="91"/>
<label x="154.94" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.054" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F29"/>
<wire x1="144.78" y1="-58.42" x2="154.94" y2="-58.42" width="0.1016" layer="91"/>
<label x="154.94" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E2"/>
<wire x1="119.38" y1="10.16" x2="129.54" y2="10.16" width="0.1016" layer="91"/>
<label x="129.54" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F2"/>
<wire x1="144.78" y1="10.16" x2="154.94" y2="10.16" width="0.1016" layer="91"/>
<label x="154.94" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.057" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E9"/>
<wire x1="119.38" y1="-7.62" x2="129.54" y2="-7.62" width="0.1016" layer="91"/>
<label x="129.54" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.059" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F9"/>
<wire x1="144.78" y1="-7.62" x2="154.94" y2="-7.62" width="0.1016" layer="91"/>
<label x="154.94" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E16"/>
<wire x1="119.38" y1="-25.4" x2="129.54" y2="-25.4" width="0.1016" layer="91"/>
<label x="129.54" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.063" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F16"/>
<wire x1="144.78" y1="-25.4" x2="154.94" y2="-25.4" width="0.1016" layer="91"/>
<label x="154.94" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.056" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="E23"/>
<wire x1="119.38" y1="-43.18" x2="129.54" y2="-43.18" width="0.1016" layer="91"/>
<label x="129.54" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.055" class="0">
<segment>
<pinref part="U$8" gate="G$1" pin="F23"/>
<wire x1="144.78" y1="-43.18" x2="154.94" y2="-43.18" width="0.1016" layer="91"/>
<label x="154.94" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$9" gate="G$1" x="58.42" y="68.58" smashed="yes"/>
<instance part="U$10" gate="G$1" x="58.42" y="-20.32" smashed="yes"/>
<instance part="SUPPLY49" gate="GND" x="20.32" y="15.24" smashed="yes">
<attribute name="VALUE" x="18.415" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY50" gate="GND" x="45.72" y="15.24" smashed="yes">
<attribute name="VALUE" x="43.815" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY51" gate="GND" x="71.12" y="15.24" smashed="yes">
<attribute name="VALUE" x="69.215" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY52" gate="GND" x="96.52" y="17.78" smashed="yes">
<attribute name="VALUE" x="94.615" y="14.605" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY53" gate="GND" x="124.46" y="15.24" smashed="yes">
<attribute name="VALUE" x="122.555" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY54" gate="GND" x="147.32" y="15.24" smashed="yes">
<attribute name="VALUE" x="145.415" y="12.065" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY55" gate="GND" x="20.32" y="-71.12" smashed="yes">
<attribute name="VALUE" x="18.415" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY56" gate="GND" x="45.72" y="-71.12" smashed="yes">
<attribute name="VALUE" x="43.815" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY57" gate="GND" x="96.52" y="-71.12" smashed="yes">
<attribute name="VALUE" x="94.615" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY58" gate="GND" x="124.46" y="-71.12" smashed="yes">
<attribute name="VALUE" x="122.555" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY59" gate="GND" x="149.86" y="-71.12" smashed="yes">
<attribute name="VALUE" x="147.955" y="-74.295" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY60" gate="GND" x="73.66" y="-73.66" smashed="yes">
<attribute name="VALUE" x="71.755" y="-76.835" size="1.778" layer="96"/>
</instance>
<instance part="U$21" gate="G$1" x="38.1" y="15.24" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="96.52" x2="22.86" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY49" gate="GND" pin="GND"/>
<wire x1="22.86" y1="96.52" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<wire x1="22.86" y1="93.98" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<wire x1="22.86" y1="91.44" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<wire x1="22.86" y1="88.9" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<wire x1="22.86" y1="86.36" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<wire x1="22.86" y1="83.82" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<wire x1="22.86" y1="81.28" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<wire x1="22.86" y1="78.74" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<wire x1="22.86" y1="76.2" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<wire x1="22.86" y1="73.66" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<wire x1="22.86" y1="71.12" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<wire x1="22.86" y1="68.58" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<wire x1="22.86" y1="66.04" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="63.5" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="60.96" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="58.42" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="55.88" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="53.34" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="50.8" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="48.26" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="45.72" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="43.18" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="40.64" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="38.1" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="35.56" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="33.02" x2="22.86" y2="30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="30.48" x2="22.86" y2="27.94" width="0.1524" layer="91"/>
<wire x1="22.86" y1="27.94" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="25.4" x2="22.86" y2="17.78" width="0.1524" layer="91"/>
<wire x1="22.86" y1="17.78" x2="20.32" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$9" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="25.4" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="25.4"/>
<pinref part="U$9" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="27.94" x2="22.86" y2="27.94" width="0.1524" layer="91"/>
<junction x="22.86" y="27.94"/>
<pinref part="U$9" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="30.48" x2="22.86" y2="30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="30.48"/>
<pinref part="U$9" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="33.02" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="33.02"/>
<pinref part="U$9" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="35.56" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="35.56"/>
<pinref part="U$9" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="38.1" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="38.1"/>
<pinref part="U$9" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="40.64" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="40.64"/>
<pinref part="U$9" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="43.18" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="43.18"/>
<pinref part="U$9" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="45.72" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="45.72"/>
<pinref part="U$9" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="48.26" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="48.26"/>
<pinref part="U$9" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="50.8" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="50.8"/>
<pinref part="U$9" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="53.34" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="53.34"/>
<pinref part="U$9" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="55.88" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="55.88"/>
<pinref part="U$9" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="58.42" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="58.42"/>
<pinref part="U$9" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="60.96" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="60.96"/>
<pinref part="U$9" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="63.5" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<junction x="22.86" y="63.5"/>
<pinref part="U$9" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="66.04" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<junction x="22.86" y="66.04"/>
<pinref part="U$9" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="68.58" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<junction x="22.86" y="68.58"/>
<pinref part="U$9" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="71.12" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<junction x="22.86" y="71.12"/>
<pinref part="U$9" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="73.66" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<junction x="22.86" y="73.66"/>
<pinref part="U$9" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="76.2" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<junction x="22.86" y="76.2"/>
<pinref part="U$9" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="78.74" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<junction x="22.86" y="78.74"/>
<pinref part="U$9" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="81.28" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<junction x="22.86" y="81.28"/>
<pinref part="U$9" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="83.82" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<junction x="22.86" y="83.82"/>
<pinref part="U$9" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="86.36" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<junction x="22.86" y="86.36"/>
<pinref part="U$9" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="88.9" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<junction x="22.86" y="88.9"/>
<pinref part="U$9" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="91.44" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<junction x="22.86" y="91.44"/>
<pinref part="U$9" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="93.98" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<junction x="22.86" y="93.98"/>
<wire x1="20.32" y1="17.78" x2="27.94" y2="17.78" width="0.1524" layer="91"/>
<junction x="20.32" y="17.78"/>
<wire x1="27.94" y1="17.78" x2="27.94" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$21" gate="G$1" pin="GND"/>
<wire x1="27.94" y1="15.24" x2="38.1" y2="15.24" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="96.52" x2="48.26" y2="96.52" width="0.1524" layer="91"/>
<wire x1="48.26" y1="96.52" x2="48.26" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY50" gate="GND" pin="GND"/>
<wire x1="48.26" y1="93.98" x2="48.26" y2="76.2" width="0.1524" layer="91"/>
<wire x1="48.26" y1="76.2" x2="48.26" y2="58.42" width="0.1524" layer="91"/>
<wire x1="48.26" y1="58.42" x2="48.26" y2="40.64" width="0.1524" layer="91"/>
<wire x1="48.26" y1="40.64" x2="48.26" y2="22.86" width="0.1524" layer="91"/>
<wire x1="48.26" y1="22.86" x2="48.26" y2="17.78" width="0.1524" layer="91"/>
<wire x1="48.26" y1="17.78" x2="45.72" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$9" gate="G$1" pin="B30"/>
<wire x1="40.64" y1="22.86" x2="48.26" y2="22.86" width="0.1524" layer="91"/>
<junction x="48.26" y="22.86"/>
<pinref part="U$9" gate="G$1" pin="B2"/>
<wire x1="40.64" y1="93.98" x2="48.26" y2="93.98" width="0.1524" layer="91"/>
<junction x="48.26" y="93.98"/>
<pinref part="U$9" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="76.2" x2="48.26" y2="76.2" width="0.1524" layer="91"/>
<junction x="48.26" y="76.2"/>
<pinref part="U$9" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="58.42" x2="48.26" y2="58.42" width="0.1524" layer="91"/>
<junction x="48.26" y="58.42"/>
<pinref part="U$9" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="40.64" x2="48.26" y2="40.64" width="0.1524" layer="91"/>
<junction x="48.26" y="40.64"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="96.52" x2="73.66" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY51" gate="GND" pin="GND"/>
<wire x1="73.66" y1="96.52" x2="73.66" y2="93.98" width="0.1524" layer="91"/>
<wire x1="73.66" y1="93.98" x2="73.66" y2="76.2" width="0.1524" layer="91"/>
<wire x1="73.66" y1="76.2" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<wire x1="73.66" y1="58.42" x2="73.66" y2="40.64" width="0.1524" layer="91"/>
<wire x1="73.66" y1="40.64" x2="73.66" y2="22.86" width="0.1524" layer="91"/>
<wire x1="73.66" y1="22.86" x2="73.66" y2="17.78" width="0.1524" layer="91"/>
<wire x1="73.66" y1="17.78" x2="71.12" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$9" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="22.86" x2="73.66" y2="22.86" width="0.1524" layer="91"/>
<junction x="73.66" y="22.86"/>
<pinref part="U$9" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="40.64" x2="73.66" y2="40.64" width="0.1524" layer="91"/>
<junction x="73.66" y="40.64"/>
<pinref part="U$9" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="58.42" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<junction x="73.66" y="58.42"/>
<pinref part="U$9" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="76.2" x2="73.66" y2="76.2" width="0.1524" layer="91"/>
<junction x="73.66" y="76.2"/>
<pinref part="U$9" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="93.98" x2="73.66" y2="93.98" width="0.1524" layer="91"/>
<junction x="73.66" y="93.98"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="96.52" x2="99.06" y2="96.52" width="0.1524" layer="91"/>
<wire x1="99.06" y1="96.52" x2="99.06" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY52" gate="GND" pin="GND"/>
<wire x1="99.06" y1="93.98" x2="99.06" y2="76.2" width="0.1524" layer="91"/>
<wire x1="99.06" y1="76.2" x2="99.06" y2="58.42" width="0.1524" layer="91"/>
<wire x1="99.06" y1="58.42" x2="99.06" y2="40.64" width="0.1524" layer="91"/>
<wire x1="99.06" y1="40.64" x2="99.06" y2="22.86" width="0.1524" layer="91"/>
<wire x1="99.06" y1="22.86" x2="99.06" y2="20.32" width="0.1524" layer="91"/>
<wire x1="99.06" y1="20.32" x2="96.52" y2="20.32" width="0.1524" layer="91"/>
<pinref part="U$9" gate="G$1" pin="D30"/>
<wire x1="91.44" y1="22.86" x2="99.06" y2="22.86" width="0.1524" layer="91"/>
<junction x="99.06" y="22.86"/>
<pinref part="U$9" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="93.98" x2="99.06" y2="93.98" width="0.1524" layer="91"/>
<junction x="99.06" y="93.98"/>
<pinref part="U$9" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="76.2" x2="99.06" y2="76.2" width="0.1524" layer="91"/>
<junction x="99.06" y="76.2"/>
<pinref part="U$9" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="58.42" x2="99.06" y2="58.42" width="0.1524" layer="91"/>
<junction x="99.06" y="58.42"/>
<pinref part="U$9" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="40.64" x2="99.06" y2="40.64" width="0.1524" layer="91"/>
<junction x="99.06" y="40.64"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="96.52" x2="124.46" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY53" gate="GND" pin="GND"/>
<wire x1="124.46" y1="96.52" x2="124.46" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$9" gate="G$1" pin="E30"/>
<wire x1="124.46" y1="22.86" x2="124.46" y2="17.78" width="0.1524" layer="91"/>
<wire x1="116.84" y1="22.86" x2="124.46" y2="22.86" width="0.1524" layer="91"/>
<junction x="124.46" y="22.86"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="96.52" x2="149.86" y2="96.52" width="0.1524" layer="91"/>
<pinref part="SUPPLY54" gate="GND" pin="GND"/>
<wire x1="149.86" y1="96.52" x2="149.86" y2="22.86" width="0.1524" layer="91"/>
<wire x1="149.86" y1="22.86" x2="149.86" y2="17.78" width="0.1524" layer="91"/>
<wire x1="149.86" y1="17.78" x2="147.32" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$9" gate="G$1" pin="F30"/>
<wire x1="142.24" y1="22.86" x2="149.86" y2="22.86" width="0.1524" layer="91"/>
<junction x="149.86" y="22.86"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="7.62" x2="22.86" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY55" gate="GND" pin="GND"/>
<wire x1="22.86" y1="7.62" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="5.08" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="2.54" x2="22.86" y2="0" width="0.1524" layer="91"/>
<wire x1="22.86" y1="0" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-2.54" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-5.08" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-7.62" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-10.16" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-15.24" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-17.78" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-20.32" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-22.86" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-25.4" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-27.94" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-30.48" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-33.02" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-35.56" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-38.1" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-40.64" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-43.18" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-45.72" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-48.26" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-50.8" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-53.34" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-55.88" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-58.42" x2="22.86" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-60.96" x2="22.86" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-63.5" x2="22.86" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-68.58" x2="20.32" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="5.08" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="5.08"/>
<pinref part="U$10" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="0" x2="22.86" y2="0" width="0.1524" layer="91"/>
<junction x="22.86" y="0"/>
<pinref part="U$10" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="2.54" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="2.54"/>
<pinref part="U$10" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="-2.54" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="-2.54"/>
<pinref part="U$10" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="-5.08" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="-5.08"/>
<pinref part="U$10" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="-7.62" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<junction x="22.86" y="-7.62"/>
<pinref part="U$10" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="-10.16" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<junction x="22.86" y="-10.16"/>
<pinref part="U$10" gate="G$1" pin="A9"/>
<wire x1="22.86" y1="-12.7" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="-15.24" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<junction x="22.86" y="-15.24"/>
<pinref part="U$10" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="-17.78" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<junction x="22.86" y="-17.78"/>
<wire x1="15.24" y1="-12.7" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<junction x="22.86" y="-12.7"/>
<pinref part="U$10" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="-20.32" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<junction x="22.86" y="-20.32"/>
<pinref part="U$10" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="-22.86" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<junction x="22.86" y="-22.86"/>
<pinref part="U$10" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="-25.4" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="-25.4"/>
<pinref part="U$10" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="-27.94" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<junction x="22.86" y="-27.94"/>
<pinref part="U$10" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="-30.48" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="-30.48"/>
<pinref part="U$10" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="-33.02" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="-33.02"/>
<pinref part="U$10" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="-35.56" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="-35.56"/>
<pinref part="U$10" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="-38.1" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="-38.1"/>
<pinref part="U$10" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="-40.64" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="-40.64"/>
<pinref part="U$10" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="-43.18" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="-43.18"/>
<pinref part="U$10" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="-45.72" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="-45.72"/>
<pinref part="U$10" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="-48.26" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="-48.26"/>
<pinref part="U$10" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="-50.8" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="-50.8"/>
<pinref part="U$10" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="-53.34" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="-53.34"/>
<pinref part="U$10" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="-55.88" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="-55.88"/>
<pinref part="U$10" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="-58.42" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="-58.42"/>
<pinref part="U$10" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="-60.96" x2="22.86" y2="-60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="-60.96"/>
<pinref part="U$10" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="-63.5" x2="22.86" y2="-63.5" width="0.1524" layer="91"/>
<junction x="22.86" y="-63.5"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="7.62" x2="48.26" y2="7.62" width="0.1524" layer="91"/>
<wire x1="48.26" y1="7.62" x2="48.26" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY56" gate="GND" pin="GND"/>
<wire x1="48.26" y1="5.08" x2="48.26" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-12.7" x2="48.26" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-30.48" x2="48.26" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-48.26" x2="48.26" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-66.04" x2="48.26" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-68.58" x2="45.72" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="B30"/>
<wire x1="40.64" y1="-66.04" x2="48.26" y2="-66.04" width="0.1524" layer="91"/>
<junction x="48.26" y="-66.04"/>
<pinref part="U$10" gate="G$1" pin="B2"/>
<wire x1="40.64" y1="5.08" x2="48.26" y2="5.08" width="0.1524" layer="91"/>
<junction x="48.26" y="5.08"/>
<pinref part="U$10" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="-12.7" x2="48.26" y2="-12.7" width="0.1524" layer="91"/>
<junction x="48.26" y="-12.7"/>
<pinref part="U$10" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="-30.48" x2="48.26" y2="-30.48" width="0.1524" layer="91"/>
<junction x="48.26" y="-30.48"/>
<pinref part="U$10" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="-48.26" x2="48.26" y2="-48.26" width="0.1524" layer="91"/>
<junction x="48.26" y="-48.26"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="7.62" x2="73.66" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY60" gate="GND" pin="GND"/>
<wire x1="73.66" y1="7.62" x2="73.66" y2="5.08" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="C30"/>
<wire x1="73.66" y1="5.08" x2="73.66" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-12.7" x2="73.66" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-30.48" x2="73.66" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-48.26" x2="73.66" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-66.04" x2="73.66" y2="-71.12" width="0.1524" layer="91"/>
<wire x1="66.04" y1="-66.04" x2="73.66" y2="-66.04" width="0.1524" layer="91"/>
<junction x="73.66" y="-66.04"/>
<pinref part="U$10" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="5.08" x2="73.66" y2="5.08" width="0.1524" layer="91"/>
<junction x="73.66" y="5.08"/>
<pinref part="U$10" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="-12.7" x2="73.66" y2="-12.7" width="0.1524" layer="91"/>
<junction x="73.66" y="-12.7"/>
<pinref part="U$10" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="-30.48" x2="73.66" y2="-30.48" width="0.1524" layer="91"/>
<junction x="73.66" y="-30.48"/>
<pinref part="U$10" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="-48.26" x2="73.66" y2="-48.26" width="0.1524" layer="91"/>
<junction x="73.66" y="-48.26"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="7.62" x2="99.06" y2="7.62" width="0.1524" layer="91"/>
<wire x1="99.06" y1="7.62" x2="99.06" y2="5.08" width="0.1524" layer="91"/>
<pinref part="SUPPLY57" gate="GND" pin="GND"/>
<wire x1="99.06" y1="5.08" x2="99.06" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-12.7" x2="99.06" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-30.48" x2="99.06" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-48.26" x2="99.06" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-66.04" x2="99.06" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-68.58" x2="96.52" y2="-68.58" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="D30"/>
<wire x1="91.44" y1="-66.04" x2="99.06" y2="-66.04" width="0.1524" layer="91"/>
<junction x="99.06" y="-66.04"/>
<pinref part="U$10" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="5.08" x2="99.06" y2="5.08" width="0.1524" layer="91"/>
<junction x="99.06" y="5.08"/>
<pinref part="U$10" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="-12.7" x2="99.06" y2="-12.7" width="0.1524" layer="91"/>
<junction x="99.06" y="-12.7"/>
<pinref part="U$10" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="-30.48" x2="99.06" y2="-30.48" width="0.1524" layer="91"/>
<junction x="99.06" y="-30.48"/>
<pinref part="U$10" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="-48.26" x2="99.06" y2="-48.26" width="0.1524" layer="91"/>
<junction x="99.06" y="-48.26"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="7.62" x2="124.46" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY58" gate="GND" pin="GND"/>
<wire x1="124.46" y1="7.62" x2="124.46" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="E30"/>
<wire x1="124.46" y1="-66.04" x2="124.46" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="116.84" y1="-66.04" x2="124.46" y2="-66.04" width="0.1524" layer="91"/>
<junction x="124.46" y="-66.04"/>
</segment>
<segment>
<pinref part="U$10" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="7.62" x2="149.86" y2="7.62" width="0.1524" layer="91"/>
<pinref part="SUPPLY59" gate="GND" pin="GND"/>
<wire x1="149.86" y1="7.62" x2="149.86" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$10" gate="G$1" pin="F30"/>
<wire x1="149.86" y1="-66.04" x2="149.86" y2="-68.58" width="0.1524" layer="91"/>
<wire x1="142.24" y1="-66.04" x2="149.86" y2="-66.04" width="0.1524" layer="91"/>
<junction x="149.86" y="-66.04"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="A30"/>
<wire x1="15.24" y1="22.86" x2="30.48" y2="22.86" width="0.1524" layer="91"/>
<wire x1="30.48" y1="22.86" x2="30.48" y2="17.78" width="0.1524" layer="91"/>
<pinref part="U$21" gate="G$1" pin="IO"/>
<wire x1="30.48" y1="17.78" x2="38.1" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.09.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="91.44" x2="50.8" y2="91.44" width="0.1016" layer="91"/>
<label x="50.8" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="88.9" x2="50.8" y2="88.9" width="0.1016" layer="91"/>
<label x="50.8" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="86.36" x2="50.8" y2="86.36" width="0.1016" layer="91"/>
<label x="50.8" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="83.82" x2="50.8" y2="83.82" width="0.1016" layer="91"/>
<label x="50.8" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="81.28" x2="50.8" y2="81.28" width="0.1016" layer="91"/>
<label x="50.8" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.077" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="78.74" x2="50.8" y2="78.74" width="0.1016" layer="91"/>
<label x="50.8" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="73.66" x2="50.8" y2="73.66" width="0.1016" layer="91"/>
<label x="50.8" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="71.12" x2="50.8" y2="71.12" width="0.1016" layer="91"/>
<label x="50.8" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="68.58" x2="50.8" y2="68.58" width="0.1016" layer="91"/>
<label x="50.8" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="66.04" x2="50.8" y2="66.04" width="0.1016" layer="91"/>
<label x="50.8" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="63.5" x2="50.8" y2="63.5" width="0.1016" layer="91"/>
<label x="50.8" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="60.96" x2="50.8" y2="60.96" width="0.1016" layer="91"/>
<label x="50.8" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.062" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="55.88" x2="50.8" y2="55.88" width="0.1016" layer="91"/>
<label x="50.8" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="53.34" x2="50.8" y2="53.34" width="0.1016" layer="91"/>
<label x="50.8" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.063" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="50.8" x2="50.8" y2="50.8" width="0.1016" layer="91"/>
<label x="50.8" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="48.26" x2="50.8" y2="48.26" width="0.1016" layer="91"/>
<label x="50.8" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="45.72" x2="50.8" y2="45.72" width="0.1016" layer="91"/>
<label x="50.8" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="43.18" x2="50.8" y2="43.18" width="0.1016" layer="91"/>
<label x="50.8" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="38.1" x2="50.8" y2="38.1" width="0.1016" layer="91"/>
<label x="50.8" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="35.56" x2="50.8" y2="35.56" width="0.1016" layer="91"/>
<label x="50.8" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="33.02" x2="50.8" y2="33.02" width="0.1016" layer="91"/>
<label x="50.8" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="30.48" x2="50.8" y2="30.48" width="0.1016" layer="91"/>
<label x="50.8" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="27.94" x2="50.8" y2="27.94" width="0.1016" layer="91"/>
<label x="50.8" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="25.4" x2="50.8" y2="25.4" width="0.1016" layer="91"/>
<label x="50.8" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="91.44" x2="76.2" y2="91.44" width="0.1016" layer="91"/>
<label x="76.2" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="88.9" x2="76.2" y2="88.9" width="0.1016" layer="91"/>
<label x="76.2" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="86.36" x2="76.2" y2="86.36" width="0.1016" layer="91"/>
<label x="76.2" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="83.82" x2="76.2" y2="83.82" width="0.1016" layer="91"/>
<label x="76.2" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="81.28" x2="76.2" y2="81.28" width="0.1016" layer="91"/>
<label x="76.2" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.078" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="78.74" x2="76.2" y2="78.74" width="0.1016" layer="91"/>
<label x="76.2" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="73.66" x2="76.2" y2="73.66" width="0.1016" layer="91"/>
<label x="76.2" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="71.12" x2="76.2" y2="71.12" width="0.1016" layer="91"/>
<label x="76.2" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.076" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="68.58" x2="76.2" y2="68.58" width="0.1016" layer="91"/>
<label x="76.2" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="66.04" x2="76.2" y2="66.04" width="0.1016" layer="91"/>
<label x="76.2" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="63.5" x2="76.2" y2="63.5" width="0.1016" layer="91"/>
<label x="76.2" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="60.96" x2="76.2" y2="60.96" width="0.1016" layer="91"/>
<label x="76.2" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="55.88" x2="76.2" y2="55.88" width="0.1016" layer="91"/>
<label x="76.2" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="53.34" x2="76.2" y2="53.34" width="0.1016" layer="91"/>
<label x="76.2" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="50.8" x2="76.2" y2="50.8" width="0.1016" layer="91"/>
<label x="76.2" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="48.26" x2="76.2" y2="48.26" width="0.1016" layer="91"/>
<label x="76.2" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="45.72" x2="76.2" y2="45.72" width="0.1016" layer="91"/>
<label x="76.2" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="43.18" x2="76.2" y2="43.18" width="0.1016" layer="91"/>
<label x="76.2" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="38.1" x2="76.2" y2="38.1" width="0.1016" layer="91"/>
<label x="76.2" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="35.56" x2="76.2" y2="35.56" width="0.1016" layer="91"/>
<label x="76.2" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.076" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="33.02" x2="76.2" y2="33.02" width="0.1016" layer="91"/>
<label x="76.2" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.076" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="30.48" x2="76.2" y2="30.48" width="0.1016" layer="91"/>
<label x="76.2" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="27.94" x2="76.2" y2="27.94" width="0.1016" layer="91"/>
<label x="76.2" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.065" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="25.4" x2="76.2" y2="25.4" width="0.1016" layer="91"/>
<label x="76.2" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="91.44" x2="101.6" y2="91.44" width="0.1016" layer="91"/>
<label x="101.6" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="88.9" x2="101.6" y2="88.9" width="0.1016" layer="91"/>
<label x="101.6" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="86.36" x2="101.6" y2="86.36" width="0.1016" layer="91"/>
<label x="101.6" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="83.82" x2="101.6" y2="83.82" width="0.1016" layer="91"/>
<label x="101.6" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.076" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="81.28" x2="101.6" y2="81.28" width="0.1016" layer="91"/>
<label x="101.6" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.078" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="78.74" x2="101.6" y2="78.74" width="0.1016" layer="91"/>
<label x="101.6" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="73.66" x2="101.6" y2="73.66" width="0.1016" layer="91"/>
<label x="101.6" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="71.12" x2="101.6" y2="71.12" width="0.1016" layer="91"/>
<label x="101.6" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.077" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="68.58" x2="101.6" y2="68.58" width="0.1016" layer="91"/>
<label x="101.6" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.076" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="66.04" x2="101.6" y2="66.04" width="0.1016" layer="91"/>
<label x="101.6" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="63.5" x2="101.6" y2="63.5" width="0.1016" layer="91"/>
<label x="101.6" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="60.96" x2="101.6" y2="60.96" width="0.1016" layer="91"/>
<label x="101.6" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="55.88" x2="101.6" y2="55.88" width="0.1016" layer="91"/>
<label x="101.6" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.062" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="53.34" x2="101.6" y2="53.34" width="0.1016" layer="91"/>
<label x="101.6" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="50.8" x2="101.6" y2="50.8" width="0.1016" layer="91"/>
<label x="101.6" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.063" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="48.26" x2="101.6" y2="48.26" width="0.1016" layer="91"/>
<label x="101.6" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="45.72" x2="101.6" y2="45.72" width="0.1016" layer="91"/>
<label x="101.6" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.064" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="43.18" x2="101.6" y2="43.18" width="0.1016" layer="91"/>
<label x="101.6" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="38.1" x2="101.6" y2="38.1" width="0.1016" layer="91"/>
<label x="101.6" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="35.56" x2="101.6" y2="35.56" width="0.1016" layer="91"/>
<label x="101.6" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.077" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="33.02" x2="101.6" y2="33.02" width="0.1016" layer="91"/>
<label x="101.6" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.077" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="30.48" x2="101.6" y2="30.48" width="0.1016" layer="91"/>
<label x="101.6" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="27.94" x2="101.6" y2="27.94" width="0.1016" layer="91"/>
<label x="101.6" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.066" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="25.4" x2="101.6" y2="25.4" width="0.1016" layer="91"/>
<label x="101.6" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.068" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="91.44" x2="127" y2="91.44" width="0.1016" layer="91"/>
<label x="127" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.068" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="88.9" x2="127" y2="88.9" width="0.1016" layer="91"/>
<label x="127" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="86.36" x2="127" y2="86.36" width="0.1016" layer="91"/>
<label x="127" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="83.82" x2="127" y2="83.82" width="0.1016" layer="91"/>
<label x="127" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.076" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="81.28" x2="127" y2="81.28" width="0.1016" layer="91"/>
<label x="127" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="78.74" x2="127" y2="78.74" width="0.1016" layer="91"/>
<label x="127" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="73.66" x2="127" y2="73.66" width="0.1016" layer="91"/>
<label x="127" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="71.12" x2="127" y2="71.12" width="0.1016" layer="91"/>
<label x="127" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.078" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="68.58" x2="127" y2="68.58" width="0.1016" layer="91"/>
<label x="127" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.077" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="66.04" x2="127" y2="66.04" width="0.1016" layer="91"/>
<label x="127" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.072" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="63.5" x2="127" y2="63.5" width="0.1016" layer="91"/>
<label x="127" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.068" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="60.96" x2="127" y2="60.96" width="0.1016" layer="91"/>
<label x="127" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.080" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="55.88" x2="127" y2="55.88" width="0.1016" layer="91"/>
<label x="127" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.085" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="53.34" x2="127" y2="53.34" width="0.1016" layer="91"/>
<label x="127" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.080" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="50.8" x2="127" y2="50.8" width="0.1016" layer="91"/>
<label x="127" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="48.26" x2="127" y2="48.26" width="0.1016" layer="91"/>
<label x="127" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.084" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="45.72" x2="127" y2="45.72" width="0.1016" layer="91"/>
<label x="127" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="43.18" x2="127" y2="43.18" width="0.1016" layer="91"/>
<label x="127" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="38.1" x2="127" y2="38.1" width="0.1016" layer="91"/>
<label x="127" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="35.56" x2="127" y2="35.56" width="0.1016" layer="91"/>
<label x="127" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="33.02" x2="127" y2="33.02" width="0.1016" layer="91"/>
<label x="127" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="30.48" x2="127" y2="30.48" width="0.1016" layer="91"/>
<label x="127" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.078" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="27.94" x2="127" y2="27.94" width="0.1016" layer="91"/>
<label x="127" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="25.4" x2="127" y2="25.4" width="0.1016" layer="91"/>
<label x="127" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.069" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="91.44" x2="152.4" y2="91.44" width="0.1016" layer="91"/>
<label x="152.4" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.069" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="88.9" x2="152.4" y2="88.9" width="0.1016" layer="91"/>
<label x="152.4" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.071" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="86.36" x2="152.4" y2="86.36" width="0.1016" layer="91"/>
<label x="152.4" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="83.82" x2="152.4" y2="83.82" width="0.1016" layer="91"/>
<label x="152.4" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.077" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="81.28" x2="152.4" y2="81.28" width="0.1016" layer="91"/>
<label x="152.4" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="78.74" x2="152.4" y2="78.74" width="0.1016" layer="91"/>
<label x="152.4" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.068" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="73.66" x2="152.4" y2="73.66" width="0.1016" layer="91"/>
<label x="152.4" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="71.12" x2="152.4" y2="71.12" width="0.1016" layer="91"/>
<label x="152.4" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="68.58" x2="152.4" y2="68.58" width="0.1016" layer="91"/>
<label x="152.4" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.078" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="66.04" x2="152.4" y2="66.04" width="0.1016" layer="91"/>
<label x="152.4" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="63.5" x2="152.4" y2="63.5" width="0.1016" layer="91"/>
<label x="152.4" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.069" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="60.96" x2="152.4" y2="60.96" width="0.1016" layer="91"/>
<label x="152.4" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="55.88" x2="152.4" y2="55.88" width="0.1016" layer="91"/>
<label x="152.4" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.085" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="53.34" x2="152.4" y2="53.34" width="0.1016" layer="91"/>
<label x="152.4" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="50.8" x2="152.4" y2="50.8" width="0.1016" layer="91"/>
<label x="152.4" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="48.26" x2="152.4" y2="48.26" width="0.1016" layer="91"/>
<label x="152.4" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.084" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="45.72" x2="152.4" y2="45.72" width="0.1016" layer="91"/>
<label x="152.4" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="43.18" x2="152.4" y2="43.18" width="0.1016" layer="91"/>
<label x="152.4" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.069" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="38.1" x2="152.4" y2="38.1" width="0.1016" layer="91"/>
<label x="152.4" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="35.56" x2="152.4" y2="35.56" width="0.1016" layer="91"/>
<label x="152.4" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.068" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="33.02" x2="152.4" y2="33.02" width="0.1016" layer="91"/>
<label x="152.4" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.073" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="30.48" x2="152.4" y2="30.48" width="0.1016" layer="91"/>
<label x="152.4" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.078" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="27.94" x2="152.4" y2="27.94" width="0.1016" layer="91"/>
<label x="152.4" y="27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.067" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="25.4" x2="152.4" y2="25.4" width="0.1016" layer="91"/>
<label x="152.4" y="25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.070" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="93.98" x2="127" y2="93.98" width="0.1016" layer="91"/>
<label x="127" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="93.98" x2="152.4" y2="93.98" width="0.1016" layer="91"/>
<label x="152.4" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.069" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="76.2" x2="127" y2="76.2" width="0.1016" layer="91"/>
<label x="127" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.079" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="76.2" x2="152.4" y2="76.2" width="0.1016" layer="91"/>
<label x="152.4" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.075" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="58.42" x2="127" y2="58.42" width="0.1016" layer="91"/>
<label x="127" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.074" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="58.42" x2="152.4" y2="58.42" width="0.1016" layer="91"/>
<label x="152.4" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.069" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="40.64" x2="127" y2="40.64" width="0.1016" layer="91"/>
<label x="127" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.068" class="0">
<segment>
<pinref part="U$9" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="40.64" x2="152.4" y2="40.64" width="0.1016" layer="91"/>
<label x="152.4" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="2.54" x2="50.8" y2="2.54" width="0.1016" layer="91"/>
<label x="50.8" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="0" x2="50.8" y2="0" width="0.1016" layer="91"/>
<label x="50.8" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="-2.54" x2="50.8" y2="-2.54" width="0.1016" layer="91"/>
<label x="50.8" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="-5.08" x2="50.8" y2="-5.08" width="0.1016" layer="91"/>
<label x="50.8" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="-7.62" x2="50.8" y2="-7.62" width="0.1016" layer="91"/>
<label x="50.8" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="-10.16" x2="50.8" y2="-10.16" width="0.1016" layer="91"/>
<label x="50.8" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="-15.24" x2="50.8" y2="-15.24" width="0.1016" layer="91"/>
<label x="50.8" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="-17.78" x2="50.8" y2="-17.78" width="0.1016" layer="91"/>
<label x="50.8" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="-20.32" x2="50.8" y2="-20.32" width="0.1016" layer="91"/>
<label x="50.8" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="-22.86" x2="50.8" y2="-22.86" width="0.1016" layer="91"/>
<label x="50.8" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="-25.4" x2="50.8" y2="-25.4" width="0.1016" layer="91"/>
<label x="50.8" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="-27.94" x2="50.8" y2="-27.94" width="0.1016" layer="91"/>
<label x="50.8" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="-33.02" x2="50.8" y2="-33.02" width="0.1016" layer="91"/>
<label x="50.8" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="-35.56" x2="50.8" y2="-35.56" width="0.1016" layer="91"/>
<label x="50.8" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="-38.1" x2="50.8" y2="-38.1" width="0.1016" layer="91"/>
<label x="50.8" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="-40.64" x2="50.8" y2="-40.64" width="0.1016" layer="91"/>
<label x="50.8" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="-43.18" x2="50.8" y2="-43.18" width="0.1016" layer="91"/>
<label x="50.8" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="-45.72" x2="50.8" y2="-45.72" width="0.1016" layer="91"/>
<label x="50.8" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="-50.8" x2="50.8" y2="-50.8" width="0.1016" layer="91"/>
<label x="50.8" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="-53.34" x2="50.8" y2="-53.34" width="0.1016" layer="91"/>
<label x="50.8" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="-55.88" x2="50.8" y2="-55.88" width="0.1016" layer="91"/>
<label x="50.8" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="-58.42" x2="50.8" y2="-58.42" width="0.1016" layer="91"/>
<label x="50.8" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="-60.96" x2="50.8" y2="-60.96" width="0.1016" layer="91"/>
<label x="50.8" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="-63.5" x2="50.8" y2="-63.5" width="0.1016" layer="91"/>
<label x="50.8" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="2.54" x2="76.2" y2="2.54" width="0.1016" layer="91"/>
<label x="76.2" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="0" x2="76.2" y2="0" width="0.1016" layer="91"/>
<label x="76.2" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="-2.54" x2="76.2" y2="-2.54" width="0.1016" layer="91"/>
<label x="76.2" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="-5.08" x2="76.2" y2="-5.08" width="0.1016" layer="91"/>
<label x="76.2" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="-7.62" x2="76.2" y2="-7.62" width="0.1016" layer="91"/>
<label x="76.2" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="-10.16" x2="76.2" y2="-10.16" width="0.1016" layer="91"/>
<label x="76.2" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="-15.24" x2="76.2" y2="-15.24" width="0.1016" layer="91"/>
<label x="76.2" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="-17.78" x2="76.2" y2="-17.78" width="0.1016" layer="91"/>
<label x="76.2" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.064" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="-20.32" x2="76.2" y2="-20.32" width="0.1016" layer="91"/>
<label x="76.2" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="-22.86" x2="76.2" y2="-22.86" width="0.1016" layer="91"/>
<label x="76.2" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="-25.4" x2="76.2" y2="-25.4" width="0.1016" layer="91"/>
<label x="76.2" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="-27.94" x2="76.2" y2="-27.94" width="0.1016" layer="91"/>
<label x="76.2" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="-33.02" x2="76.2" y2="-33.02" width="0.1016" layer="91"/>
<label x="76.2" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="-35.56" x2="76.2" y2="-35.56" width="0.1016" layer="91"/>
<label x="76.2" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="-38.1" x2="76.2" y2="-38.1" width="0.1016" layer="91"/>
<label x="76.2" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="-40.64" x2="76.2" y2="-40.64" width="0.1016" layer="91"/>
<label x="76.2" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="-43.18" x2="76.2" y2="-43.18" width="0.1016" layer="91"/>
<label x="76.2" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="-45.72" x2="76.2" y2="-45.72" width="0.1016" layer="91"/>
<label x="76.2" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.075" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="-50.8" x2="76.2" y2="-50.8" width="0.1016" layer="91"/>
<label x="76.2" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="-53.34" x2="76.2" y2="-53.34" width="0.1016" layer="91"/>
<label x="76.2" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="-55.88" x2="76.2" y2="-55.88" width="0.1016" layer="91"/>
<label x="76.2" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="-58.42" x2="76.2" y2="-58.42" width="0.1016" layer="91"/>
<label x="76.2" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="-60.96" x2="76.2" y2="-60.96" width="0.1016" layer="91"/>
<label x="76.2" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.065" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="-63.5" x2="76.2" y2="-63.5" width="0.1016" layer="91"/>
<label x="76.2" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="2.54" x2="101.6" y2="2.54" width="0.1016" layer="91"/>
<label x="101.6" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="0" x2="101.6" y2="0" width="0.1016" layer="91"/>
<label x="101.6" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="-2.54" x2="101.6" y2="-2.54" width="0.1016" layer="91"/>
<label x="101.6" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="-5.08" x2="101.6" y2="-5.08" width="0.1016" layer="91"/>
<label x="101.6" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="-7.62" x2="101.6" y2="-7.62" width="0.1016" layer="91"/>
<label x="101.6" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="-10.16" x2="101.6" y2="-10.16" width="0.1016" layer="91"/>
<label x="101.6" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="-15.24" x2="101.6" y2="-15.24" width="0.1016" layer="91"/>
<label x="101.6" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="-17.78" x2="101.6" y2="-17.78" width="0.1016" layer="91"/>
<label x="101.6" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="-20.32" x2="101.6" y2="-20.32" width="0.1016" layer="91"/>
<label x="101.6" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="-22.86" x2="101.6" y2="-22.86" width="0.1016" layer="91"/>
<label x="101.6" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="-25.4" x2="101.6" y2="-25.4" width="0.1016" layer="91"/>
<label x="101.6" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="-27.94" x2="101.6" y2="-27.94" width="0.1016" layer="91"/>
<label x="101.6" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="-33.02" x2="101.6" y2="-33.02" width="0.1016" layer="91"/>
<label x="101.6" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="-35.56" x2="101.6" y2="-35.56" width="0.1016" layer="91"/>
<label x="101.6" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="-38.1" x2="101.6" y2="-38.1" width="0.1016" layer="91"/>
<label x="101.6" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="-40.64" x2="101.6" y2="-40.64" width="0.1016" layer="91"/>
<label x="101.6" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="-43.18" x2="101.6" y2="-43.18" width="0.1016" layer="91"/>
<label x="101.6" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="-45.72" x2="101.6" y2="-45.72" width="0.1016" layer="91"/>
<label x="101.6" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="-50.8" x2="101.6" y2="-50.8" width="0.1016" layer="91"/>
<label x="101.6" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="-53.34" x2="101.6" y2="-53.34" width="0.1016" layer="91"/>
<label x="101.6" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="-55.88" x2="101.6" y2="-55.88" width="0.1016" layer="91"/>
<label x="101.6" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="-58.42" x2="101.6" y2="-58.42" width="0.1016" layer="91"/>
<label x="101.6" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="-60.96" x2="101.6" y2="-60.96" width="0.1016" layer="91"/>
<label x="101.6" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.066" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="-63.5" x2="101.6" y2="-63.5" width="0.1016" layer="91"/>
<label x="101.6" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="2.54" x2="127" y2="2.54" width="0.1016" layer="91"/>
<label x="127" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="0" x2="127" y2="0" width="0.1016" layer="91"/>
<label x="127" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="-2.54" x2="127" y2="-2.54" width="0.1016" layer="91"/>
<label x="127" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="-5.08" x2="127" y2="-5.08" width="0.1016" layer="91"/>
<label x="127" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="-7.62" x2="127" y2="-7.62" width="0.1016" layer="91"/>
<label x="127" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="-10.16" x2="127" y2="-10.16" width="0.1016" layer="91"/>
<label x="127" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="-15.24" x2="127" y2="-15.24" width="0.1016" layer="91"/>
<label x="127" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="-17.78" x2="127" y2="-17.78" width="0.1016" layer="91"/>
<label x="127" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="-20.32" x2="127" y2="-20.32" width="0.1016" layer="91"/>
<label x="127" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="-22.86" x2="127" y2="-22.86" width="0.1016" layer="91"/>
<label x="127" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.076" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="-25.4" x2="127" y2="-25.4" width="0.1016" layer="91"/>
<label x="127" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="-27.94" x2="127" y2="-27.94" width="0.1016" layer="91"/>
<label x="127" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="-33.02" x2="127" y2="-33.02" width="0.1016" layer="91"/>
<label x="127" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="-35.56" x2="127" y2="-35.56" width="0.1016" layer="91"/>
<label x="127" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="-38.1" x2="127" y2="-38.1" width="0.1016" layer="91"/>
<label x="127" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="-40.64" x2="127" y2="-40.64" width="0.1016" layer="91"/>
<label x="127" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="-43.18" x2="127" y2="-43.18" width="0.1016" layer="91"/>
<label x="127" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="-45.72" x2="127" y2="-45.72" width="0.1016" layer="91"/>
<label x="127" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="-50.8" x2="127" y2="-50.8" width="0.1016" layer="91"/>
<label x="127" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="-53.34" x2="127" y2="-53.34" width="0.1016" layer="91"/>
<label x="127" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="-55.88" x2="127" y2="-55.88" width="0.1016" layer="91"/>
<label x="127" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="-58.42" x2="127" y2="-58.42" width="0.1016" layer="91"/>
<label x="127" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="-60.96" x2="127" y2="-60.96" width="0.1016" layer="91"/>
<label x="127" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="-63.5" x2="127" y2="-63.5" width="0.1016" layer="91"/>
<label x="127" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="2.54" x2="152.4" y2="2.54" width="0.1016" layer="91"/>
<label x="152.4" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="0" x2="152.4" y2="0" width="0.1016" layer="91"/>
<label x="152.4" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="-2.54" x2="152.4" y2="-2.54" width="0.1016" layer="91"/>
<label x="152.4" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="-5.08" x2="152.4" y2="-5.08" width="0.1016" layer="91"/>
<label x="152.4" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="-7.62" x2="152.4" y2="-7.62" width="0.1016" layer="91"/>
<label x="152.4" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="-10.16" x2="152.4" y2="-10.16" width="0.1016" layer="91"/>
<label x="152.4" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="-15.24" x2="152.4" y2="-15.24" width="0.1016" layer="91"/>
<label x="152.4" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="-17.78" x2="152.4" y2="-17.78" width="0.1016" layer="91"/>
<label x="152.4" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.071" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="-20.32" x2="152.4" y2="-20.32" width="0.1016" layer="91"/>
<label x="152.4" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="-22.86" x2="152.4" y2="-22.86" width="0.1016" layer="91"/>
<label x="152.4" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.077" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="-25.4" x2="152.4" y2="-25.4" width="0.1016" layer="91"/>
<label x="152.4" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="-27.94" x2="152.4" y2="-27.94" width="0.1016" layer="91"/>
<label x="152.4" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="-33.02" x2="152.4" y2="-33.02" width="0.1016" layer="91"/>
<label x="152.4" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="-35.56" x2="152.4" y2="-35.56" width="0.1016" layer="91"/>
<label x="152.4" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="-38.1" x2="152.4" y2="-38.1" width="0.1016" layer="91"/>
<label x="152.4" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="-40.64" x2="152.4" y2="-40.64" width="0.1016" layer="91"/>
<label x="152.4" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.073" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="-43.18" x2="152.4" y2="-43.18" width="0.1016" layer="91"/>
<label x="152.4" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="-45.72" x2="152.4" y2="-45.72" width="0.1016" layer="91"/>
<label x="152.4" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="-50.8" x2="152.4" y2="-50.8" width="0.1016" layer="91"/>
<label x="152.4" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="-53.34" x2="152.4" y2="-53.34" width="0.1016" layer="91"/>
<label x="152.4" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="-55.88" x2="152.4" y2="-55.88" width="0.1016" layer="91"/>
<label x="152.4" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.072" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="-58.42" x2="152.4" y2="-58.42" width="0.1016" layer="91"/>
<label x="152.4" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.078" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="-60.96" x2="152.4" y2="-60.96" width="0.1016" layer="91"/>
<label x="152.4" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.067" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="-63.5" x2="152.4" y2="-63.5" width="0.1016" layer="91"/>
<label x="152.4" y="-63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="5.08" x2="127" y2="5.08" width="0.1016" layer="91"/>
<label x="127" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="5.08" x2="152.4" y2="5.08" width="0.1016" layer="91"/>
<label x="152.4" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.070" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="-12.7" x2="127" y2="-12.7" width="0.1016" layer="91"/>
<label x="127" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.074" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="-12.7" x2="152.4" y2="-12.7" width="0.1016" layer="91"/>
<label x="152.4" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="-30.48" x2="127" y2="-30.48" width="0.1016" layer="91"/>
<label x="127" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.079" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="-30.48" x2="152.4" y2="-30.48" width="0.1016" layer="91"/>
<label x="152.4" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.069" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="-48.26" x2="127" y2="-48.26" width="0.1016" layer="91"/>
<label x="127" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.068" class="0">
<segment>
<pinref part="U$10" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="-48.26" x2="152.4" y2="-48.26" width="0.1016" layer="91"/>
<label x="152.4" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$11" gate="G$1" x="58.42" y="76.2" smashed="yes"/>
<instance part="U$12" gate="G$1" x="58.42" y="-10.16" smashed="yes"/>
<instance part="SUPPLY61" gate="GND" x="22.86" y="22.86" smashed="yes">
<attribute name="VALUE" x="20.955" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY62" gate="GND" x="48.26" y="22.86" smashed="yes">
<attribute name="VALUE" x="46.355" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY63" gate="GND" x="71.12" y="25.4" smashed="yes">
<attribute name="VALUE" x="69.215" y="22.225" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY64" gate="GND" x="99.06" y="22.86" smashed="yes">
<attribute name="VALUE" x="97.155" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY65" gate="GND" x="121.92" y="22.86" smashed="yes">
<attribute name="VALUE" x="120.015" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY66" gate="GND" x="147.32" y="22.86" smashed="yes">
<attribute name="VALUE" x="145.415" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY67" gate="GND" x="20.32" y="-60.96" smashed="yes">
<attribute name="VALUE" x="18.415" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY68" gate="GND" x="48.26" y="-60.96" smashed="yes">
<attribute name="VALUE" x="46.355" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY69" gate="GND" x="99.06" y="-60.96" smashed="yes">
<attribute name="VALUE" x="97.155" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY70" gate="GND" x="124.46" y="-60.96" smashed="yes">
<attribute name="VALUE" x="122.555" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY71" gate="GND" x="149.86" y="-60.96" smashed="yes">
<attribute name="VALUE" x="147.955" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY72" gate="GND" x="71.12" y="-60.96" smashed="yes">
<attribute name="VALUE" x="69.215" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="U$22" gate="G$1" x="38.1" y="22.86" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="104.14" x2="22.86" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY61" gate="GND" pin="GND"/>
<wire x1="22.86" y1="104.14" x2="22.86" y2="101.6" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="A2"/>
<wire x1="22.86" y1="99.06" x2="22.86" y2="96.52" width="0.1524" layer="91"/>
<wire x1="22.86" y1="96.52" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<wire x1="22.86" y1="93.98" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<wire x1="22.86" y1="91.44" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<wire x1="22.86" y1="88.9" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<wire x1="22.86" y1="83.82" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<wire x1="22.86" y1="81.28" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<wire x1="22.86" y1="78.74" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<wire x1="22.86" y1="76.2" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<wire x1="22.86" y1="73.66" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<wire x1="22.86" y1="71.12" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<wire x1="22.86" y1="68.58" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<wire x1="22.86" y1="66.04" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="63.5" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="60.96" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="58.42" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="55.88" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="53.34" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="50.8" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="48.26" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="45.72" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="43.18" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="40.64" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="38.1" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="35.56" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="33.02" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="101.6" x2="22.86" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="99.06" x2="22.86" y2="99.06" width="0.1524" layer="91"/>
<junction x="22.86" y="99.06"/>
<pinref part="U$11" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="96.52" x2="22.86" y2="96.52" width="0.1524" layer="91"/>
<junction x="22.86" y="96.52"/>
<pinref part="U$11" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="91.44" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<junction x="22.86" y="91.44"/>
<pinref part="U$11" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="93.98" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<junction x="22.86" y="93.98"/>
<pinref part="U$11" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="88.9" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<junction x="22.86" y="88.9"/>
<pinref part="U$11" gate="G$1" pin="A8"/>
<wire x1="22.86" y1="86.36" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="83.82" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<junction x="22.86" y="83.82"/>
<wire x1="15.24" y1="86.36" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<junction x="22.86" y="86.36"/>
<pinref part="U$11" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="78.74" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<junction x="22.86" y="78.74"/>
<pinref part="U$11" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="81.28" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<junction x="22.86" y="81.28"/>
<pinref part="U$11" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="76.2" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<junction x="22.86" y="76.2"/>
<pinref part="U$11" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="73.66" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<junction x="22.86" y="73.66"/>
<pinref part="U$11" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="71.12" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<junction x="22.86" y="71.12"/>
<pinref part="U$11" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="68.58" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<junction x="22.86" y="68.58"/>
<pinref part="U$11" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="66.04" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<junction x="22.86" y="66.04"/>
<pinref part="U$11" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="63.5" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<junction x="22.86" y="63.5"/>
<pinref part="U$11" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="60.96" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="60.96"/>
<pinref part="U$11" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="58.42" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="58.42"/>
<pinref part="U$11" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="55.88" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="55.88"/>
<pinref part="U$11" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="53.34" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="53.34"/>
<pinref part="U$11" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="50.8" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="50.8"/>
<pinref part="U$11" gate="G$1" pin="A23"/>
<wire x1="22.86" y1="48.26" x2="15.24" y2="48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="48.26"/>
<pinref part="U$11" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="45.72" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="45.72"/>
<pinref part="U$11" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="43.18" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="43.18"/>
<pinref part="U$11" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="40.64" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="40.64"/>
<pinref part="U$11" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="38.1" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="38.1"/>
<pinref part="U$11" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="35.56" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="35.56"/>
<pinref part="U$11" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="33.02" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="33.02"/>
<wire x1="15.24" y1="101.6" x2="22.86" y2="101.6" width="0.1524" layer="91"/>
<junction x="22.86" y="101.6"/>
<wire x1="22.86" y1="25.4" x2="27.94" y2="25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="25.4"/>
<wire x1="27.94" y1="25.4" x2="27.94" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$22" gate="G$1" pin="GND"/>
<wire x1="27.94" y1="22.86" x2="38.1" y2="22.86" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="104.14" x2="48.26" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY62" gate="GND" pin="GND"/>
<wire x1="48.26" y1="104.14" x2="48.26" y2="101.6" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="B30"/>
<wire x1="48.26" y1="101.6" x2="48.26" y2="83.82" width="0.1524" layer="91"/>
<wire x1="48.26" y1="83.82" x2="48.26" y2="66.04" width="0.1524" layer="91"/>
<wire x1="48.26" y1="66.04" x2="48.26" y2="48.26" width="0.1524" layer="91"/>
<wire x1="48.26" y1="48.26" x2="48.26" y2="30.48" width="0.1524" layer="91"/>
<wire x1="48.26" y1="30.48" x2="48.26" y2="25.4" width="0.1524" layer="91"/>
<wire x1="40.64" y1="30.48" x2="48.26" y2="30.48" width="0.1524" layer="91"/>
<junction x="48.26" y="30.48"/>
<pinref part="U$11" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="48.26" x2="48.26" y2="48.26" width="0.1524" layer="91"/>
<junction x="48.26" y="48.26"/>
<pinref part="U$11" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="66.04" x2="48.26" y2="66.04" width="0.1524" layer="91"/>
<junction x="48.26" y="66.04"/>
<pinref part="U$11" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="83.82" x2="48.26" y2="83.82" width="0.1524" layer="91"/>
<junction x="48.26" y="83.82"/>
<pinref part="U$11" gate="G$1" pin="B2"/>
<wire x1="40.64" y1="101.6" x2="48.26" y2="101.6" width="0.1524" layer="91"/>
<junction x="48.26" y="101.6"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="104.14" x2="73.66" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY63" gate="GND" pin="GND"/>
<wire x1="73.66" y1="104.14" x2="73.66" y2="101.6" width="0.1524" layer="91"/>
<wire x1="73.66" y1="101.6" x2="73.66" y2="83.82" width="0.1524" layer="91"/>
<wire x1="73.66" y1="83.82" x2="73.66" y2="66.04" width="0.1524" layer="91"/>
<wire x1="73.66" y1="66.04" x2="73.66" y2="48.26" width="0.1524" layer="91"/>
<wire x1="73.66" y1="48.26" x2="73.66" y2="30.48" width="0.1524" layer="91"/>
<wire x1="73.66" y1="30.48" x2="73.66" y2="27.94" width="0.1524" layer="91"/>
<wire x1="73.66" y1="27.94" x2="71.12" y2="27.94" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="30.48" x2="73.66" y2="30.48" width="0.1524" layer="91"/>
<junction x="73.66" y="30.48"/>
<pinref part="U$11" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="48.26" x2="73.66" y2="48.26" width="0.1524" layer="91"/>
<junction x="73.66" y="48.26"/>
<pinref part="U$11" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="66.04" x2="73.66" y2="66.04" width="0.1524" layer="91"/>
<junction x="73.66" y="66.04"/>
<pinref part="U$11" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="83.82" x2="73.66" y2="83.82" width="0.1524" layer="91"/>
<junction x="73.66" y="83.82"/>
<pinref part="U$11" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="101.6" x2="73.66" y2="101.6" width="0.1524" layer="91"/>
<junction x="73.66" y="101.6"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="104.14" x2="99.06" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY64" gate="GND" pin="GND"/>
<wire x1="99.06" y1="104.14" x2="99.06" y2="101.6" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="D30"/>
<wire x1="99.06" y1="101.6" x2="99.06" y2="83.82" width="0.1524" layer="91"/>
<wire x1="99.06" y1="83.82" x2="99.06" y2="66.04" width="0.1524" layer="91"/>
<wire x1="99.06" y1="66.04" x2="99.06" y2="48.26" width="0.1524" layer="91"/>
<wire x1="99.06" y1="48.26" x2="99.06" y2="30.48" width="0.1524" layer="91"/>
<wire x1="99.06" y1="30.48" x2="99.06" y2="25.4" width="0.1524" layer="91"/>
<wire x1="91.44" y1="30.48" x2="99.06" y2="30.48" width="0.1524" layer="91"/>
<junction x="99.06" y="30.48"/>
<pinref part="U$11" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="48.26" x2="99.06" y2="48.26" width="0.1524" layer="91"/>
<junction x="99.06" y="48.26"/>
<pinref part="U$11" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="66.04" x2="99.06" y2="66.04" width="0.1524" layer="91"/>
<junction x="99.06" y="66.04"/>
<pinref part="U$11" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="83.82" x2="99.06" y2="83.82" width="0.1524" layer="91"/>
<junction x="99.06" y="83.82"/>
<pinref part="U$11" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="101.6" x2="99.06" y2="101.6" width="0.1524" layer="91"/>
<junction x="99.06" y="101.6"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="104.14" x2="124.46" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY65" gate="GND" pin="GND"/>
<wire x1="124.46" y1="104.14" x2="124.46" y2="30.48" width="0.1524" layer="91"/>
<wire x1="124.46" y1="30.48" x2="124.46" y2="25.4" width="0.1524" layer="91"/>
<wire x1="124.46" y1="25.4" x2="121.92" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="E30"/>
<wire x1="116.84" y1="30.48" x2="124.46" y2="30.48" width="0.1524" layer="91"/>
<junction x="124.46" y="30.48"/>
</segment>
<segment>
<pinref part="U$11" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="104.14" x2="149.86" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY66" gate="GND" pin="GND"/>
<wire x1="149.86" y1="104.14" x2="149.86" y2="30.48" width="0.1524" layer="91"/>
<wire x1="149.86" y1="30.48" x2="149.86" y2="25.4" width="0.1524" layer="91"/>
<wire x1="149.86" y1="25.4" x2="147.32" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="F30"/>
<wire x1="142.24" y1="30.48" x2="149.86" y2="30.48" width="0.1524" layer="91"/>
<junction x="149.86" y="30.48"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="17.78" x2="149.86" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY71" gate="GND" pin="GND"/>
<wire x1="149.86" y1="17.78" x2="149.86" y2="-55.88" width="0.1524" layer="91"/>
<pinref part="U$12" gate="G$1" pin="F30"/>
<wire x1="149.86" y1="-55.88" x2="149.86" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="142.24" y1="-55.88" x2="149.86" y2="-55.88" width="0.1524" layer="91"/>
<junction x="149.86" y="-55.88"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="17.78" x2="124.46" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY70" gate="GND" pin="GND"/>
<wire x1="124.46" y1="17.78" x2="124.46" y2="-55.88" width="0.1524" layer="91"/>
<pinref part="U$12" gate="G$1" pin="E30"/>
<wire x1="124.46" y1="-55.88" x2="124.46" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="116.84" y1="-55.88" x2="124.46" y2="-55.88" width="0.1524" layer="91"/>
<junction x="124.46" y="-55.88"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="17.78" x2="99.06" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY69" gate="GND" pin="GND"/>
<wire x1="99.06" y1="17.78" x2="99.06" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$12" gate="G$1" pin="D30"/>
<wire x1="99.06" y1="15.24" x2="99.06" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-2.54" x2="99.06" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-20.32" x2="99.06" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-38.1" x2="99.06" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-55.88" x2="99.06" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="91.44" y1="-55.88" x2="99.06" y2="-55.88" width="0.1524" layer="91"/>
<junction x="99.06" y="-55.88"/>
<pinref part="U$12" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="15.24" x2="99.06" y2="15.24" width="0.1524" layer="91"/>
<junction x="99.06" y="15.24"/>
<pinref part="U$12" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="-2.54" x2="99.06" y2="-2.54" width="0.1524" layer="91"/>
<junction x="99.06" y="-2.54"/>
<pinref part="U$12" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="-20.32" x2="99.06" y2="-20.32" width="0.1524" layer="91"/>
<junction x="99.06" y="-20.32"/>
<pinref part="U$12" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="-38.1" x2="99.06" y2="-38.1" width="0.1524" layer="91"/>
<junction x="99.06" y="-38.1"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="17.78" x2="73.66" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY72" gate="GND" pin="GND"/>
<wire x1="73.66" y1="17.78" x2="73.66" y2="15.24" width="0.1524" layer="91"/>
<wire x1="73.66" y1="15.24" x2="73.66" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-2.54" x2="73.66" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-20.32" x2="73.66" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-38.1" x2="73.66" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-55.88" x2="73.66" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-58.42" x2="71.12" y2="-58.42" width="0.1524" layer="91"/>
<pinref part="U$12" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="-55.88" x2="73.66" y2="-55.88" width="0.1524" layer="91"/>
<junction x="73.66" y="-55.88"/>
<pinref part="U$12" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="15.24" x2="73.66" y2="15.24" width="0.1524" layer="91"/>
<junction x="73.66" y="15.24"/>
<pinref part="U$12" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="-2.54" x2="73.66" y2="-2.54" width="0.1524" layer="91"/>
<junction x="73.66" y="-2.54"/>
<pinref part="U$12" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="-20.32" x2="73.66" y2="-20.32" width="0.1524" layer="91"/>
<junction x="73.66" y="-20.32"/>
<pinref part="U$12" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="-38.1" x2="73.66" y2="-38.1" width="0.1524" layer="91"/>
<junction x="73.66" y="-38.1"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="17.78" x2="48.26" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY68" gate="GND" pin="GND"/>
<wire x1="48.26" y1="17.78" x2="48.26" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$12" gate="G$1" pin="B30"/>
<wire x1="48.26" y1="15.24" x2="48.26" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-2.54" x2="48.26" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-20.32" x2="48.26" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-38.1" x2="48.26" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-55.88" x2="48.26" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-55.88" x2="48.26" y2="-55.88" width="0.1524" layer="91"/>
<junction x="48.26" y="-55.88"/>
<pinref part="U$12" gate="G$1" pin="B2"/>
<wire x1="40.64" y1="15.24" x2="48.26" y2="15.24" width="0.1524" layer="91"/>
<junction x="48.26" y="15.24"/>
<pinref part="U$12" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="-2.54" x2="48.26" y2="-2.54" width="0.1524" layer="91"/>
<junction x="48.26" y="-2.54"/>
<pinref part="U$12" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="-20.32" x2="48.26" y2="-20.32" width="0.1524" layer="91"/>
<junction x="48.26" y="-20.32"/>
<pinref part="U$12" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="-38.1" x2="48.26" y2="-38.1" width="0.1524" layer="91"/>
<junction x="48.26" y="-38.1"/>
</segment>
<segment>
<pinref part="U$12" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="17.78" x2="22.86" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY67" gate="GND" pin="GND"/>
<wire x1="22.86" y1="17.78" x2="22.86" y2="15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="15.24" x2="22.86" y2="12.7" width="0.1524" layer="91"/>
<wire x1="22.86" y1="12.7" x2="22.86" y2="10.16" width="0.1524" layer="91"/>
<wire x1="22.86" y1="10.16" x2="22.86" y2="7.62" width="0.1524" layer="91"/>
<wire x1="22.86" y1="7.62" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="5.08" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="2.54" x2="22.86" y2="0" width="0.1524" layer="91"/>
<wire x1="22.86" y1="0" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-2.54" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-5.08" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-7.62" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-10.16" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-12.7" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-15.24" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-17.78" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-20.32" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-22.86" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-25.4" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-27.94" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-30.48" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-33.02" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-35.56" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-38.1" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-40.64" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-43.18" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-45.72" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-48.26" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-50.8" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-53.34" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-58.42" x2="20.32" y2="-58.42" width="0.1524" layer="91"/>
<pinref part="U$12" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="15.24" x2="22.86" y2="15.24" width="0.1524" layer="91"/>
<junction x="22.86" y="15.24"/>
<pinref part="U$12" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="12.7" x2="22.86" y2="12.7" width="0.1524" layer="91"/>
<junction x="22.86" y="12.7"/>
<pinref part="U$12" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="10.16" x2="22.86" y2="10.16" width="0.1524" layer="91"/>
<junction x="22.86" y="10.16"/>
<pinref part="U$12" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="7.62" x2="22.86" y2="7.62" width="0.1524" layer="91"/>
<junction x="22.86" y="7.62"/>
<pinref part="U$12" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="5.08" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="5.08"/>
<pinref part="U$12" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="2.54" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="2.54"/>
<pinref part="U$12" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="0" x2="22.86" y2="0" width="0.1524" layer="91"/>
<junction x="22.86" y="0"/>
<pinref part="U$12" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="-2.54" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="-2.54"/>
<pinref part="U$12" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="-5.08" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="-5.08"/>
<pinref part="U$12" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="-7.62" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<junction x="22.86" y="-7.62"/>
<pinref part="U$12" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="-10.16" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<junction x="22.86" y="-10.16"/>
<pinref part="U$12" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="-12.7" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<junction x="22.86" y="-12.7"/>
<pinref part="U$12" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="-15.24" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<junction x="22.86" y="-15.24"/>
<pinref part="U$12" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="-20.32" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<junction x="22.86" y="-20.32"/>
<pinref part="U$12" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="-17.78" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<junction x="22.86" y="-17.78"/>
<pinref part="U$12" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="-22.86" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<junction x="22.86" y="-22.86"/>
<pinref part="U$12" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="-25.4" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="-25.4"/>
<pinref part="U$12" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="-27.94" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<junction x="22.86" y="-27.94"/>
<pinref part="U$12" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="-30.48" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="-30.48"/>
<pinref part="U$12" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="-33.02" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="-33.02"/>
<pinref part="U$12" gate="G$1" pin="A22"/>
<wire x1="22.86" y1="-35.56" x2="15.24" y2="-35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="-35.56"/>
<pinref part="U$12" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="-38.1" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="-38.1"/>
<pinref part="U$12" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="-40.64" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="-40.64"/>
<pinref part="U$12" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="-43.18" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="-43.18"/>
<pinref part="U$12" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="-45.72" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="-45.72"/>
<pinref part="U$12" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="-48.26" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="-48.26"/>
<pinref part="U$12" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="-50.8" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="-50.8"/>
<pinref part="U$12" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="-53.34" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="-53.34"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="U$22" gate="G$1" pin="IO"/>
<wire x1="30.48" y1="30.48" x2="30.48" y2="25.4" width="0.1524" layer="91"/>
<wire x1="30.48" y1="25.4" x2="38.1" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$11" gate="G$1" pin="A30"/>
<wire x1="15.24" y1="30.48" x2="30.48" y2="30.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.08.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="99.06" x2="50.8" y2="99.06" width="0.1016" layer="91"/>
<label x="50.8" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.084" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="96.52" x2="50.8" y2="96.52" width="0.1016" layer="91"/>
<label x="50.8" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.085" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="93.98" x2="50.8" y2="93.98" width="0.1016" layer="91"/>
<label x="50.8" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="91.44" x2="50.8" y2="91.44" width="0.1016" layer="91"/>
<label x="50.8" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="88.9" x2="50.8" y2="88.9" width="0.1016" layer="91"/>
<label x="50.8" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.093" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="86.36" x2="50.8" y2="86.36" width="0.1016" layer="91"/>
<label x="50.8" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.080" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="81.28" x2="50.8" y2="81.28" width="0.1016" layer="91"/>
<label x="50.8" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="78.74" x2="50.8" y2="78.74" width="0.1016" layer="91"/>
<label x="50.8" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="76.2" x2="50.8" y2="76.2" width="0.1016" layer="91"/>
<label x="50.8" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="73.66" x2="50.8" y2="73.66" width="0.1016" layer="91"/>
<label x="50.8" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.085" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="71.12" x2="50.8" y2="71.12" width="0.1016" layer="91"/>
<label x="50.8" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.080" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="68.58" x2="50.8" y2="68.58" width="0.1016" layer="91"/>
<label x="50.8" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.076" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="63.5" x2="50.8" y2="63.5" width="0.1016" layer="91"/>
<label x="50.8" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="60.96" x2="50.8" y2="60.96" width="0.1016" layer="91"/>
<label x="50.8" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.077" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="58.42" x2="50.8" y2="58.42" width="0.1016" layer="91"/>
<label x="50.8" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="55.88" x2="50.8" y2="55.88" width="0.1016" layer="91"/>
<label x="50.8" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.078" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="53.34" x2="50.8" y2="53.34" width="0.1016" layer="91"/>
<label x="50.8" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="50.8" x2="50.8" y2="50.8" width="0.1016" layer="91"/>
<label x="50.8" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.080" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="45.72" x2="50.8" y2="45.72" width="0.1016" layer="91"/>
<label x="50.8" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="43.18" x2="50.8" y2="43.18" width="0.1016" layer="91"/>
<label x="50.8" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="40.64" x2="50.8" y2="40.64" width="0.1016" layer="91"/>
<label x="50.8" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="38.1" x2="50.8" y2="38.1" width="0.1016" layer="91"/>
<label x="50.8" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="35.56" x2="50.8" y2="35.56" width="0.1016" layer="91"/>
<label x="50.8" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.080" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="33.02" x2="50.8" y2="33.02" width="0.1016" layer="91"/>
<label x="50.8" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.084" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="99.06" x2="76.2" y2="99.06" width="0.1016" layer="91"/>
<label x="76.2" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.085" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="96.52" x2="76.2" y2="96.52" width="0.1016" layer="91"/>
<label x="76.2" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="93.98" x2="76.2" y2="93.98" width="0.1016" layer="91"/>
<label x="76.2" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="91.44" x2="76.2" y2="91.44" width="0.1016" layer="91"/>
<label x="76.2" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="88.9" x2="76.2" y2="88.9" width="0.1016" layer="91"/>
<label x="76.2" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.094" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="86.36" x2="76.2" y2="86.36" width="0.1016" layer="91"/>
<label x="76.2" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="81.28" x2="76.2" y2="81.28" width="0.1016" layer="91"/>
<label x="76.2" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="78.74" x2="76.2" y2="78.74" width="0.1016" layer="91"/>
<label x="76.2" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.092" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="76.2" x2="76.2" y2="76.2" width="0.1016" layer="91"/>
<label x="76.2" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="73.66" x2="76.2" y2="73.66" width="0.1016" layer="91"/>
<label x="76.2" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="71.12" x2="76.2" y2="71.12" width="0.1016" layer="91"/>
<label x="76.2" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="68.58" x2="76.2" y2="68.58" width="0.1016" layer="91"/>
<label x="76.2" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="63.5" x2="76.2" y2="63.5" width="0.1016" layer="91"/>
<label x="76.2" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="60.96" x2="76.2" y2="60.96" width="0.1016" layer="91"/>
<label x="76.2" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="58.42" x2="76.2" y2="58.42" width="0.1016" layer="91"/>
<label x="76.2" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="55.88" x2="76.2" y2="55.88" width="0.1016" layer="91"/>
<label x="76.2" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="53.34" x2="76.2" y2="53.34" width="0.1016" layer="91"/>
<label x="76.2" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="50.8" x2="76.2" y2="50.8" width="0.1016" layer="91"/>
<label x="76.2" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="45.72" x2="76.2" y2="45.72" width="0.1016" layer="91"/>
<label x="76.2" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="43.18" x2="76.2" y2="43.18" width="0.1016" layer="91"/>
<label x="76.2" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.092" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="40.64" x2="76.2" y2="40.64" width="0.1016" layer="91"/>
<label x="76.2" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.092" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="38.1" x2="76.2" y2="38.1" width="0.1016" layer="91"/>
<label x="76.2" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="35.56" x2="76.2" y2="35.56" width="0.1016" layer="91"/>
<label x="76.2" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="33.02" x2="76.2" y2="33.02" width="0.1016" layer="91"/>
<label x="76.2" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.080" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="99.06" x2="101.6" y2="99.06" width="0.1016" layer="91"/>
<label x="101.6" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.080" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="96.52" x2="101.6" y2="96.52" width="0.1016" layer="91"/>
<label x="101.6" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="93.98" x2="101.6" y2="93.98" width="0.1016" layer="91"/>
<label x="101.6" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="91.44" x2="101.6" y2="91.44" width="0.1016" layer="91"/>
<label x="101.6" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.092" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="88.9" x2="101.6" y2="88.9" width="0.1016" layer="91"/>
<label x="101.6" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.094" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="86.36" x2="101.6" y2="86.36" width="0.1016" layer="91"/>
<label x="101.6" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="81.28" x2="101.6" y2="81.28" width="0.1016" layer="91"/>
<label x="101.6" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="78.74" x2="101.6" y2="78.74" width="0.1016" layer="91"/>
<label x="101.6" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.093" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="76.2" x2="101.6" y2="76.2" width="0.1016" layer="91"/>
<label x="101.6" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.092" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="73.66" x2="101.6" y2="73.66" width="0.1016" layer="91"/>
<label x="101.6" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="71.12" x2="101.6" y2="71.12" width="0.1016" layer="91"/>
<label x="101.6" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="68.58" x2="101.6" y2="68.58" width="0.1016" layer="91"/>
<label x="101.6" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.086" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="63.5" x2="101.6" y2="63.5" width="0.1016" layer="91"/>
<label x="101.6" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.076" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="60.96" x2="101.6" y2="60.96" width="0.1016" layer="91"/>
<label x="101.6" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="58.42" x2="101.6" y2="58.42" width="0.1016" layer="91"/>
<label x="101.6" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.077" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="55.88" x2="101.6" y2="55.88" width="0.1016" layer="91"/>
<label x="101.6" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="53.34" x2="101.6" y2="53.34" width="0.1016" layer="91"/>
<label x="101.6" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.078" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="50.8" x2="101.6" y2="50.8" width="0.1016" layer="91"/>
<label x="101.6" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="45.72" x2="101.6" y2="45.72" width="0.1016" layer="91"/>
<label x="101.6" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="43.18" x2="101.6" y2="43.18" width="0.1016" layer="91"/>
<label x="101.6" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.093" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="40.64" x2="101.6" y2="40.64" width="0.1016" layer="91"/>
<label x="101.6" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.093" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="38.1" x2="101.6" y2="38.1" width="0.1016" layer="91"/>
<label x="101.6" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="35.56" x2="101.6" y2="35.56" width="0.1016" layer="91"/>
<label x="101.6" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="33.02" x2="101.6" y2="33.02" width="0.1016" layer="91"/>
<label x="101.6" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="99.06" x2="127" y2="99.06" width="0.1016" layer="91"/>
<label x="127" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.081" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="96.52" x2="127" y2="96.52" width="0.1016" layer="91"/>
<label x="127" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="93.98" x2="127" y2="93.98" width="0.1016" layer="91"/>
<label x="127" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="91.44" x2="127" y2="91.44" width="0.1016" layer="91"/>
<label x="127" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.092" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="88.9" x2="127" y2="88.9" width="0.1016" layer="91"/>
<label x="127" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="86.36" x2="127" y2="86.36" width="0.1016" layer="91"/>
<label x="127" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="81.28" x2="127" y2="81.28" width="0.1016" layer="91"/>
<label x="127" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="78.74" x2="127" y2="78.74" width="0.1016" layer="91"/>
<label x="127" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.094" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="76.2" x2="127" y2="76.2" width="0.1016" layer="91"/>
<label x="127" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.093" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="73.66" x2="127" y2="73.66" width="0.1016" layer="91"/>
<label x="127" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.088" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="71.12" x2="127" y2="71.12" width="0.1016" layer="91"/>
<label x="127" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="68.58" x2="127" y2="68.58" width="0.1016" layer="91"/>
<label x="127" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.096" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="63.5" x2="127" y2="63.5" width="0.1016" layer="91"/>
<label x="127" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.101" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="60.96" x2="127" y2="60.96" width="0.1016" layer="91"/>
<label x="127" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.096" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="58.42" x2="127" y2="58.42" width="0.1016" layer="91"/>
<label x="127" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="55.88" x2="127" y2="55.88" width="0.1016" layer="91"/>
<label x="127" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.100" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="53.34" x2="127" y2="53.34" width="0.1016" layer="91"/>
<label x="127" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="50.8" x2="127" y2="50.8" width="0.1016" layer="91"/>
<label x="127" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="45.72" x2="127" y2="45.72" width="0.1016" layer="91"/>
<label x="127" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="43.18" x2="127" y2="43.18" width="0.1016" layer="91"/>
<label x="127" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="40.64" x2="127" y2="40.64" width="0.1016" layer="91"/>
<label x="127" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="38.1" x2="127" y2="38.1" width="0.1016" layer="91"/>
<label x="127" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.094" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="35.56" x2="127" y2="35.56" width="0.1016" layer="91"/>
<label x="127" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="33.02" x2="127" y2="33.02" width="0.1016" layer="91"/>
<label x="127" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="99.06" x2="152.4" y2="99.06" width="0.1016" layer="91"/>
<label x="152.4" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.082" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="96.52" x2="152.4" y2="96.52" width="0.1016" layer="91"/>
<label x="152.4" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.087" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="93.98" x2="152.4" y2="93.98" width="0.1016" layer="91"/>
<label x="152.4" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="91.44" x2="152.4" y2="91.44" width="0.1016" layer="91"/>
<label x="152.4" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.093" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="88.9" x2="152.4" y2="88.9" width="0.1016" layer="91"/>
<label x="152.4" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="86.36" x2="152.4" y2="86.36" width="0.1016" layer="91"/>
<label x="152.4" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.084" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="81.28" x2="152.4" y2="81.28" width="0.1016" layer="91"/>
<label x="152.4" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="78.74" x2="152.4" y2="78.74" width="0.1016" layer="91"/>
<label x="152.4" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="76.2" x2="152.4" y2="76.2" width="0.1016" layer="91"/>
<label x="152.4" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.094" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="73.66" x2="152.4" y2="73.66" width="0.1016" layer="91"/>
<label x="152.4" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="71.12" x2="152.4" y2="71.12" width="0.1016" layer="91"/>
<label x="152.4" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.084" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="68.58" x2="152.4" y2="68.58" width="0.1016" layer="91"/>
<label x="152.4" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="63.5" x2="152.4" y2="63.5" width="0.1016" layer="91"/>
<label x="152.4" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.101" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="60.96" x2="152.4" y2="60.96" width="0.1016" layer="91"/>
<label x="152.4" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="58.42" x2="152.4" y2="58.42" width="0.1016" layer="91"/>
<label x="152.4" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="55.88" x2="152.4" y2="55.88" width="0.1016" layer="91"/>
<label x="152.4" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.100" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="53.34" x2="152.4" y2="53.34" width="0.1016" layer="91"/>
<label x="152.4" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="50.8" x2="152.4" y2="50.8" width="0.1016" layer="91"/>
<label x="152.4" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.085" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="45.72" x2="152.4" y2="45.72" width="0.1016" layer="91"/>
<label x="152.4" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="43.18" x2="152.4" y2="43.18" width="0.1016" layer="91"/>
<label x="152.4" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.084" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="40.64" x2="152.4" y2="40.64" width="0.1016" layer="91"/>
<label x="152.4" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.089" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="38.1" x2="152.4" y2="38.1" width="0.1016" layer="91"/>
<label x="152.4" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.094" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="35.56" x2="152.4" y2="35.56" width="0.1016" layer="91"/>
<label x="152.4" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="33.02" x2="152.4" y2="33.02" width="0.1016" layer="91"/>
<label x="152.4" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.083" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="101.6" x2="127" y2="101.6" width="0.1016" layer="91"/>
<label x="127" y="101.6" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="101.6" x2="152.4" y2="101.6" width="0.1016" layer="91"/>
<label x="152.4" y="101.6" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.085" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="83.82" x2="127" y2="83.82" width="0.1016" layer="91"/>
<label x="127" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.095" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="83.82" x2="152.4" y2="83.82" width="0.1016" layer="91"/>
<label x="152.4" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.091" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="66.04" x2="127" y2="66.04" width="0.1016" layer="91"/>
<label x="127" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.090" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="66.04" x2="152.4" y2="66.04" width="0.1016" layer="91"/>
<label x="152.4" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.085" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="48.26" x2="127" y2="48.26" width="0.1016" layer="91"/>
<label x="127" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.084" class="0">
<segment>
<pinref part="U$11" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="48.26" x2="152.4" y2="48.26" width="0.1016" layer="91"/>
<label x="152.4" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="12.7" x2="50.8" y2="12.7" width="0.1016" layer="91"/>
<label x="50.8" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="10.16" x2="50.8" y2="10.16" width="0.1016" layer="91"/>
<label x="50.8" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="7.62" x2="50.8" y2="7.62" width="0.1016" layer="91"/>
<label x="50.8" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="5.08" x2="50.8" y2="5.08" width="0.1016" layer="91"/>
<label x="50.8" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="2.54" x2="50.8" y2="2.54" width="0.1016" layer="91"/>
<label x="50.8" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="0" x2="50.8" y2="0" width="0.1016" layer="91"/>
<label x="50.8" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="-5.08" x2="50.8" y2="-5.08" width="0.1016" layer="91"/>
<label x="50.8" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="-7.62" x2="50.8" y2="-7.62" width="0.1016" layer="91"/>
<label x="50.8" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="-10.16" x2="50.8" y2="-10.16" width="0.1016" layer="91"/>
<label x="50.8" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="-12.7" x2="50.8" y2="-12.7" width="0.1016" layer="91"/>
<label x="50.8" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="-15.24" x2="50.8" y2="-15.24" width="0.1016" layer="91"/>
<label x="50.8" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="-17.78" x2="50.8" y2="-17.78" width="0.1016" layer="91"/>
<label x="50.8" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="-22.86" x2="50.8" y2="-22.86" width="0.1016" layer="91"/>
<label x="50.8" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="-25.4" x2="50.8" y2="-25.4" width="0.1016" layer="91"/>
<label x="50.8" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="-27.94" x2="50.8" y2="-27.94" width="0.1016" layer="91"/>
<label x="50.8" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="-30.48" x2="50.8" y2="-30.48" width="0.1016" layer="91"/>
<label x="50.8" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="-33.02" x2="50.8" y2="-33.02" width="0.1016" layer="91"/>
<label x="50.8" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="-35.56" x2="50.8" y2="-35.56" width="0.1016" layer="91"/>
<label x="50.8" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="-40.64" x2="50.8" y2="-40.64" width="0.1016" layer="91"/>
<label x="50.8" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="-43.18" x2="50.8" y2="-43.18" width="0.1016" layer="91"/>
<label x="50.8" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="-45.72" x2="50.8" y2="-45.72" width="0.1016" layer="91"/>
<label x="50.8" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="-48.26" x2="50.8" y2="-48.26" width="0.1016" layer="91"/>
<label x="50.8" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="-50.8" x2="50.8" y2="-50.8" width="0.1016" layer="91"/>
<label x="50.8" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="-53.34" x2="50.8" y2="-53.34" width="0.1016" layer="91"/>
<label x="50.8" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="12.7" x2="76.2" y2="12.7" width="0.1016" layer="91"/>
<label x="76.2" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="10.16" x2="76.2" y2="10.16" width="0.1016" layer="91"/>
<label x="76.2" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="7.62" x2="76.2" y2="7.62" width="0.1016" layer="91"/>
<label x="76.2" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="5.08" x2="76.2" y2="5.08" width="0.1016" layer="91"/>
<label x="76.2" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="2.54" x2="76.2" y2="2.54" width="0.1016" layer="91"/>
<label x="76.2" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="0" x2="76.2" y2="0" width="0.1016" layer="91"/>
<label x="76.2" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="-5.08" x2="76.2" y2="-5.08" width="0.1016" layer="91"/>
<label x="76.2" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.080" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="-7.62" x2="76.2" y2="-7.62" width="0.1016" layer="91"/>
<label x="76.2" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="-10.16" x2="76.2" y2="-10.16" width="0.1016" layer="91"/>
<label x="76.2" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="-12.7" x2="76.2" y2="-12.7" width="0.1016" layer="91"/>
<label x="76.2" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="-15.24" x2="76.2" y2="-15.24" width="0.1016" layer="91"/>
<label x="76.2" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="-17.78" x2="76.2" y2="-17.78" width="0.1016" layer="91"/>
<label x="76.2" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="-22.86" x2="76.2" y2="-22.86" width="0.1016" layer="91"/>
<label x="76.2" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="-25.4" x2="76.2" y2="-25.4" width="0.1016" layer="91"/>
<label x="76.2" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="-27.94" x2="76.2" y2="-27.94" width="0.1016" layer="91"/>
<label x="76.2" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="-30.48" x2="76.2" y2="-30.48" width="0.1016" layer="91"/>
<label x="76.2" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="-33.02" x2="76.2" y2="-33.02" width="0.1016" layer="91"/>
<label x="76.2" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="-35.56" x2="76.2" y2="-35.56" width="0.1016" layer="91"/>
<label x="76.2" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.091" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="-40.64" x2="76.2" y2="-40.64" width="0.1016" layer="91"/>
<label x="76.2" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="-43.18" x2="76.2" y2="-43.18" width="0.1016" layer="91"/>
<label x="76.2" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="-45.72" x2="76.2" y2="-45.72" width="0.1016" layer="91"/>
<label x="76.2" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="-48.26" x2="76.2" y2="-48.26" width="0.1016" layer="91"/>
<label x="76.2" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="-50.8" x2="76.2" y2="-50.8" width="0.1016" layer="91"/>
<label x="76.2" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="-53.34" x2="76.2" y2="-53.34" width="0.1016" layer="91"/>
<label x="76.2" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="12.7" x2="101.6" y2="12.7" width="0.1016" layer="91"/>
<label x="101.6" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="10.16" x2="101.6" y2="10.16" width="0.1016" layer="91"/>
<label x="101.6" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="7.62" x2="101.6" y2="7.62" width="0.1016" layer="91"/>
<label x="101.6" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="5.08" x2="101.6" y2="5.08" width="0.1016" layer="91"/>
<label x="101.6" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="2.54" x2="101.6" y2="2.54" width="0.1016" layer="91"/>
<label x="101.6" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="0" x2="101.6" y2="0" width="0.1016" layer="91"/>
<label x="101.6" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="-5.08" x2="101.6" y2="-5.08" width="0.1016" layer="91"/>
<label x="101.6" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.081" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="-7.62" x2="101.6" y2="-7.62" width="0.1016" layer="91"/>
<label x="101.6" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.086" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="-10.16" x2="101.6" y2="-10.16" width="0.1016" layer="91"/>
<label x="101.6" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="-12.7" x2="101.6" y2="-12.7" width="0.1016" layer="91"/>
<label x="101.6" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="-15.24" x2="101.6" y2="-15.24" width="0.1016" layer="91"/>
<label x="101.6" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="-17.78" x2="101.6" y2="-17.78" width="0.1016" layer="91"/>
<label x="101.6" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="-22.86" x2="101.6" y2="-22.86" width="0.1016" layer="91"/>
<label x="101.6" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="-25.4" x2="101.6" y2="-25.4" width="0.1016" layer="91"/>
<label x="101.6" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="-27.94" x2="101.6" y2="-27.94" width="0.1016" layer="91"/>
<label x="101.6" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="-30.48" x2="101.6" y2="-30.48" width="0.1016" layer="91"/>
<label x="101.6" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="-33.02" x2="101.6" y2="-33.02" width="0.1016" layer="91"/>
<label x="101.6" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="-35.56" x2="101.6" y2="-35.56" width="0.1016" layer="91"/>
<label x="101.6" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="-40.64" x2="101.6" y2="-40.64" width="0.1016" layer="91"/>
<label x="101.6" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="-43.18" x2="101.6" y2="-43.18" width="0.1016" layer="91"/>
<label x="101.6" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="-45.72" x2="101.6" y2="-45.72" width="0.1016" layer="91"/>
<label x="101.6" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="-48.26" x2="101.6" y2="-48.26" width="0.1016" layer="91"/>
<label x="101.6" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="-50.8" x2="101.6" y2="-50.8" width="0.1016" layer="91"/>
<label x="101.6" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="-53.34" x2="101.6" y2="-53.34" width="0.1016" layer="91"/>
<label x="101.6" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="12.7" x2="127" y2="12.7" width="0.1016" layer="91"/>
<label x="127" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="10.16" x2="127" y2="10.16" width="0.1016" layer="91"/>
<label x="127" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="7.62" x2="127" y2="7.62" width="0.1016" layer="91"/>
<label x="127" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="5.08" x2="127" y2="5.08" width="0.1016" layer="91"/>
<label x="127" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="2.54" x2="127" y2="2.54" width="0.1016" layer="91"/>
<label x="127" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="0" x2="127" y2="0" width="0.1016" layer="91"/>
<label x="127" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="-5.08" x2="127" y2="-5.08" width="0.1016" layer="91"/>
<label x="127" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.082" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="-7.62" x2="127" y2="-7.62" width="0.1016" layer="91"/>
<label x="127" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="-10.16" x2="127" y2="-10.16" width="0.1016" layer="91"/>
<label x="127" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="-12.7" x2="127" y2="-12.7" width="0.1016" layer="91"/>
<label x="127" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.092" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="-15.24" x2="127" y2="-15.24" width="0.1016" layer="91"/>
<label x="127" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="-17.78" x2="127" y2="-17.78" width="0.1016" layer="91"/>
<label x="127" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="-22.86" x2="127" y2="-22.86" width="0.1016" layer="91"/>
<label x="127" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="-25.4" x2="127" y2="-25.4" width="0.1016" layer="91"/>
<label x="127" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="-27.94" x2="127" y2="-27.94" width="0.1016" layer="91"/>
<label x="127" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="-30.48" x2="127" y2="-30.48" width="0.1016" layer="91"/>
<label x="127" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="-33.02" x2="127" y2="-33.02" width="0.1016" layer="91"/>
<label x="127" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="-35.56" x2="127" y2="-35.56" width="0.1016" layer="91"/>
<label x="127" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="-40.64" x2="127" y2="-40.64" width="0.1016" layer="91"/>
<label x="127" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="-43.18" x2="127" y2="-43.18" width="0.1016" layer="91"/>
<label x="127" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="-45.72" x2="127" y2="-45.72" width="0.1016" layer="91"/>
<label x="127" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="-48.26" x2="127" y2="-48.26" width="0.1016" layer="91"/>
<label x="127" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="-50.8" x2="127" y2="-50.8" width="0.1016" layer="91"/>
<label x="127" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="-53.34" x2="127" y2="-53.34" width="0.1016" layer="91"/>
<label x="127" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="12.7" x2="152.4" y2="12.7" width="0.1016" layer="91"/>
<label x="152.4" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="10.16" x2="152.4" y2="10.16" width="0.1016" layer="91"/>
<label x="152.4" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="7.62" x2="152.4" y2="7.62" width="0.1016" layer="91"/>
<label x="152.4" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="5.08" x2="152.4" y2="5.08" width="0.1016" layer="91"/>
<label x="152.4" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="2.54" x2="152.4" y2="2.54" width="0.1016" layer="91"/>
<label x="152.4" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="0" x2="152.4" y2="0" width="0.1016" layer="91"/>
<label x="152.4" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="-5.08" x2="152.4" y2="-5.08" width="0.1016" layer="91"/>
<label x="152.4" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="-7.62" x2="152.4" y2="-7.62" width="0.1016" layer="91"/>
<label x="152.4" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.087" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="-10.16" x2="152.4" y2="-10.16" width="0.1016" layer="91"/>
<label x="152.4" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="-12.7" x2="152.4" y2="-12.7" width="0.1016" layer="91"/>
<label x="152.4" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.093" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="-15.24" x2="152.4" y2="-15.24" width="0.1016" layer="91"/>
<label x="152.4" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="-17.78" x2="152.4" y2="-17.78" width="0.1016" layer="91"/>
<label x="152.4" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="-22.86" x2="152.4" y2="-22.86" width="0.1016" layer="91"/>
<label x="152.4" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="-25.4" x2="152.4" y2="-25.4" width="0.1016" layer="91"/>
<label x="152.4" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="-27.94" x2="152.4" y2="-27.94" width="0.1016" layer="91"/>
<label x="152.4" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="-30.48" x2="152.4" y2="-30.48" width="0.1016" layer="91"/>
<label x="152.4" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.089" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="-33.02" x2="152.4" y2="-33.02" width="0.1016" layer="91"/>
<label x="152.4" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="-35.56" x2="152.4" y2="-35.56" width="0.1016" layer="91"/>
<label x="152.4" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="-40.64" x2="152.4" y2="-40.64" width="0.1016" layer="91"/>
<label x="152.4" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="-43.18" x2="152.4" y2="-43.18" width="0.1016" layer="91"/>
<label x="152.4" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="-45.72" x2="152.4" y2="-45.72" width="0.1016" layer="91"/>
<label x="152.4" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.088" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="-48.26" x2="152.4" y2="-48.26" width="0.1016" layer="91"/>
<label x="152.4" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.094" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="-50.8" x2="152.4" y2="-50.8" width="0.1016" layer="91"/>
<label x="152.4" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.083" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="-53.34" x2="152.4" y2="-53.34" width="0.1016" layer="91"/>
<label x="152.4" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="15.24" x2="127" y2="15.24" width="0.1016" layer="91"/>
<label x="127" y="15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="15.24" x2="152.4" y2="15.24" width="0.1016" layer="91"/>
<label x="152.4" y="15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="-2.54" x2="127" y2="-2.54" width="0.1016" layer="91"/>
<label x="127" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.090" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="-2.54" x2="152.4" y2="-2.54" width="0.1016" layer="91"/>
<label x="152.4" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="-20.32" x2="127" y2="-20.32" width="0.1016" layer="91"/>
<label x="127" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.095" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="-20.32" x2="152.4" y2="-20.32" width="0.1016" layer="91"/>
<label x="152.4" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.085" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="-38.1" x2="127" y2="-38.1" width="0.1016" layer="91"/>
<label x="127" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.084" class="0">
<segment>
<pinref part="U$12" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="-38.1" x2="152.4" y2="-38.1" width="0.1016" layer="91"/>
<label x="152.4" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$13" gate="G$1" x="55.88" y="76.2" smashed="yes"/>
<instance part="U$14" gate="G$1" x="55.88" y="-10.16" smashed="yes"/>
<instance part="SUPPLY73" gate="GND" x="17.78" y="22.86" smashed="yes">
<attribute name="VALUE" x="15.875" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY74" gate="GND" x="45.72" y="22.86" smashed="yes">
<attribute name="VALUE" x="43.815" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY75" gate="GND" x="71.12" y="22.86" smashed="yes">
<attribute name="VALUE" x="69.215" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY76" gate="GND" x="99.06" y="25.4" smashed="yes">
<attribute name="VALUE" x="97.155" y="22.225" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY77" gate="GND" x="121.92" y="25.4" smashed="yes">
<attribute name="VALUE" x="120.015" y="22.225" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY78" gate="GND" x="147.32" y="22.86" smashed="yes">
<attribute name="VALUE" x="145.415" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY79" gate="GND" x="17.78" y="-60.96" smashed="yes">
<attribute name="VALUE" x="15.875" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY80" gate="GND" x="45.72" y="-60.96" smashed="yes">
<attribute name="VALUE" x="43.815" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY81" gate="GND" x="71.12" y="-60.96" smashed="yes">
<attribute name="VALUE" x="69.215" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY82" gate="GND" x="96.52" y="-60.96" smashed="yes">
<attribute name="VALUE" x="94.615" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY83" gate="GND" x="121.92" y="-60.96" smashed="yes">
<attribute name="VALUE" x="120.015" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY84" gate="GND" x="144.78" y="-60.96" smashed="yes">
<attribute name="VALUE" x="142.875" y="-64.135" size="1.778" layer="96"/>
</instance>
<instance part="U$23" gate="G$1" x="35.56" y="22.86" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="A1"/>
<wire x1="12.7" y1="104.14" x2="20.32" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY73" gate="GND" pin="GND"/>
<wire x1="20.32" y1="104.14" x2="20.32" y2="101.6" width="0.1524" layer="91"/>
<wire x1="20.32" y1="101.6" x2="20.32" y2="99.06" width="0.1524" layer="91"/>
<wire x1="20.32" y1="99.06" x2="20.32" y2="96.52" width="0.1524" layer="91"/>
<wire x1="20.32" y1="96.52" x2="20.32" y2="93.98" width="0.1524" layer="91"/>
<wire x1="20.32" y1="93.98" x2="20.32" y2="91.44" width="0.1524" layer="91"/>
<wire x1="20.32" y1="91.44" x2="20.32" y2="88.9" width="0.1524" layer="91"/>
<wire x1="20.32" y1="88.9" x2="20.32" y2="86.36" width="0.1524" layer="91"/>
<wire x1="20.32" y1="86.36" x2="20.32" y2="83.82" width="0.1524" layer="91"/>
<wire x1="20.32" y1="83.82" x2="20.32" y2="81.28" width="0.1524" layer="91"/>
<wire x1="20.32" y1="81.28" x2="20.32" y2="78.74" width="0.1524" layer="91"/>
<wire x1="20.32" y1="78.74" x2="20.32" y2="76.2" width="0.1524" layer="91"/>
<wire x1="20.32" y1="76.2" x2="20.32" y2="73.66" width="0.1524" layer="91"/>
<wire x1="20.32" y1="73.66" x2="20.32" y2="71.12" width="0.1524" layer="91"/>
<wire x1="20.32" y1="71.12" x2="20.32" y2="68.58" width="0.1524" layer="91"/>
<wire x1="20.32" y1="68.58" x2="20.32" y2="66.04" width="0.1524" layer="91"/>
<wire x1="20.32" y1="66.04" x2="20.32" y2="63.5" width="0.1524" layer="91"/>
<wire x1="20.32" y1="63.5" x2="20.32" y2="60.96" width="0.1524" layer="91"/>
<wire x1="20.32" y1="60.96" x2="20.32" y2="58.42" width="0.1524" layer="91"/>
<wire x1="20.32" y1="58.42" x2="20.32" y2="55.88" width="0.1524" layer="91"/>
<wire x1="20.32" y1="55.88" x2="20.32" y2="53.34" width="0.1524" layer="91"/>
<wire x1="20.32" y1="53.34" x2="20.32" y2="50.8" width="0.1524" layer="91"/>
<wire x1="20.32" y1="50.8" x2="20.32" y2="48.26" width="0.1524" layer="91"/>
<wire x1="20.32" y1="48.26" x2="20.32" y2="45.72" width="0.1524" layer="91"/>
<wire x1="20.32" y1="45.72" x2="20.32" y2="43.18" width="0.1524" layer="91"/>
<wire x1="20.32" y1="43.18" x2="20.32" y2="40.64" width="0.1524" layer="91"/>
<wire x1="20.32" y1="40.64" x2="20.32" y2="38.1" width="0.1524" layer="91"/>
<wire x1="20.32" y1="38.1" x2="20.32" y2="35.56" width="0.1524" layer="91"/>
<wire x1="20.32" y1="35.56" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
<wire x1="20.32" y1="33.02" x2="20.32" y2="25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="25.4" x2="17.78" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$13" gate="G$1" pin="A2"/>
<wire x1="12.7" y1="101.6" x2="20.32" y2="101.6" width="0.1524" layer="91"/>
<junction x="20.32" y="101.6"/>
<pinref part="U$13" gate="G$1" pin="A3"/>
<wire x1="12.7" y1="99.06" x2="20.32" y2="99.06" width="0.1524" layer="91"/>
<junction x="20.32" y="99.06"/>
<pinref part="U$13" gate="G$1" pin="A4"/>
<wire x1="12.7" y1="96.52" x2="20.32" y2="96.52" width="0.1524" layer="91"/>
<junction x="20.32" y="96.52"/>
<pinref part="U$13" gate="G$1" pin="A5"/>
<wire x1="12.7" y1="93.98" x2="20.32" y2="93.98" width="0.1524" layer="91"/>
<junction x="20.32" y="93.98"/>
<pinref part="U$13" gate="G$1" pin="A6"/>
<wire x1="12.7" y1="91.44" x2="20.32" y2="91.44" width="0.1524" layer="91"/>
<junction x="20.32" y="91.44"/>
<pinref part="U$13" gate="G$1" pin="A7"/>
<wire x1="12.7" y1="88.9" x2="20.32" y2="88.9" width="0.1524" layer="91"/>
<junction x="20.32" y="88.9"/>
<pinref part="U$13" gate="G$1" pin="A8"/>
<wire x1="12.7" y1="86.36" x2="20.32" y2="86.36" width="0.1524" layer="91"/>
<junction x="20.32" y="86.36"/>
<pinref part="U$13" gate="G$1" pin="A9"/>
<wire x1="12.7" y1="83.82" x2="20.32" y2="83.82" width="0.1524" layer="91"/>
<junction x="20.32" y="83.82"/>
<pinref part="U$13" gate="G$1" pin="A10"/>
<wire x1="12.7" y1="81.28" x2="20.32" y2="81.28" width="0.1524" layer="91"/>
<junction x="20.32" y="81.28"/>
<pinref part="U$13" gate="G$1" pin="A11"/>
<wire x1="12.7" y1="78.74" x2="20.32" y2="78.74" width="0.1524" layer="91"/>
<junction x="20.32" y="78.74"/>
<pinref part="U$13" gate="G$1" pin="A12"/>
<wire x1="20.32" y1="76.2" x2="12.7" y2="76.2" width="0.1524" layer="91"/>
<junction x="20.32" y="76.2"/>
<pinref part="U$13" gate="G$1" pin="A13"/>
<wire x1="12.7" y1="73.66" x2="20.32" y2="73.66" width="0.1524" layer="91"/>
<junction x="20.32" y="73.66"/>
<pinref part="U$13" gate="G$1" pin="A14"/>
<wire x1="12.7" y1="71.12" x2="20.32" y2="71.12" width="0.1524" layer="91"/>
<junction x="20.32" y="71.12"/>
<pinref part="U$13" gate="G$1" pin="A15"/>
<wire x1="12.7" y1="68.58" x2="20.32" y2="68.58" width="0.1524" layer="91"/>
<junction x="20.32" y="68.58"/>
<pinref part="U$13" gate="G$1" pin="A16"/>
<wire x1="12.7" y1="66.04" x2="20.32" y2="66.04" width="0.1524" layer="91"/>
<junction x="20.32" y="66.04"/>
<pinref part="U$13" gate="G$1" pin="A17"/>
<wire x1="12.7" y1="63.5" x2="20.32" y2="63.5" width="0.1524" layer="91"/>
<junction x="20.32" y="63.5"/>
<pinref part="U$13" gate="G$1" pin="A18"/>
<wire x1="12.7" y1="60.96" x2="20.32" y2="60.96" width="0.1524" layer="91"/>
<junction x="20.32" y="60.96"/>
<pinref part="U$13" gate="G$1" pin="A19"/>
<wire x1="12.7" y1="58.42" x2="20.32" y2="58.42" width="0.1524" layer="91"/>
<junction x="20.32" y="58.42"/>
<pinref part="U$13" gate="G$1" pin="A20"/>
<wire x1="12.7" y1="55.88" x2="20.32" y2="55.88" width="0.1524" layer="91"/>
<junction x="20.32" y="55.88"/>
<pinref part="U$13" gate="G$1" pin="A21"/>
<wire x1="12.7" y1="53.34" x2="20.32" y2="53.34" width="0.1524" layer="91"/>
<junction x="20.32" y="53.34"/>
<pinref part="U$13" gate="G$1" pin="A22"/>
<wire x1="12.7" y1="50.8" x2="20.32" y2="50.8" width="0.1524" layer="91"/>
<junction x="20.32" y="50.8"/>
<pinref part="U$13" gate="G$1" pin="A23"/>
<wire x1="12.7" y1="48.26" x2="20.32" y2="48.26" width="0.1524" layer="91"/>
<junction x="20.32" y="48.26"/>
<pinref part="U$13" gate="G$1" pin="A24"/>
<wire x1="12.7" y1="45.72" x2="20.32" y2="45.72" width="0.1524" layer="91"/>
<junction x="20.32" y="45.72"/>
<pinref part="U$13" gate="G$1" pin="A25"/>
<wire x1="12.7" y1="43.18" x2="20.32" y2="43.18" width="0.1524" layer="91"/>
<junction x="20.32" y="43.18"/>
<pinref part="U$13" gate="G$1" pin="A26"/>
<wire x1="12.7" y1="40.64" x2="20.32" y2="40.64" width="0.1524" layer="91"/>
<junction x="20.32" y="40.64"/>
<pinref part="U$13" gate="G$1" pin="A27"/>
<wire x1="12.7" y1="38.1" x2="20.32" y2="38.1" width="0.1524" layer="91"/>
<junction x="20.32" y="38.1"/>
<pinref part="U$13" gate="G$1" pin="A28"/>
<wire x1="12.7" y1="35.56" x2="20.32" y2="35.56" width="0.1524" layer="91"/>
<junction x="20.32" y="35.56"/>
<pinref part="U$13" gate="G$1" pin="A29"/>
<wire x1="12.7" y1="33.02" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
<junction x="20.32" y="33.02"/>
<wire x1="17.78" y1="25.4" x2="25.4" y2="25.4" width="0.1524" layer="91"/>
<junction x="17.78" y="25.4"/>
<wire x1="25.4" y1="25.4" x2="25.4" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$23" gate="G$1" pin="GND"/>
<wire x1="25.4" y1="22.86" x2="35.56" y2="22.86" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="B1"/>
<wire x1="38.1" y1="104.14" x2="45.72" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY74" gate="GND" pin="GND"/>
<wire x1="45.72" y1="104.14" x2="45.72" y2="101.6" width="0.1524" layer="91"/>
<pinref part="U$13" gate="G$1" pin="B30"/>
<wire x1="45.72" y1="101.6" x2="45.72" y2="83.82" width="0.1524" layer="91"/>
<wire x1="45.72" y1="83.82" x2="45.72" y2="66.04" width="0.1524" layer="91"/>
<wire x1="45.72" y1="66.04" x2="45.72" y2="48.26" width="0.1524" layer="91"/>
<wire x1="45.72" y1="48.26" x2="45.72" y2="30.48" width="0.1524" layer="91"/>
<wire x1="45.72" y1="30.48" x2="45.72" y2="25.4" width="0.1524" layer="91"/>
<wire x1="38.1" y1="30.48" x2="45.72" y2="30.48" width="0.1524" layer="91"/>
<junction x="45.72" y="30.48"/>
<pinref part="U$13" gate="G$1" pin="B23"/>
<wire x1="38.1" y1="48.26" x2="45.72" y2="48.26" width="0.1524" layer="91"/>
<junction x="45.72" y="48.26"/>
<pinref part="U$13" gate="G$1" pin="B16"/>
<wire x1="38.1" y1="66.04" x2="45.72" y2="66.04" width="0.1524" layer="91"/>
<junction x="45.72" y="66.04"/>
<pinref part="U$13" gate="G$1" pin="B9"/>
<wire x1="38.1" y1="83.82" x2="45.72" y2="83.82" width="0.1524" layer="91"/>
<junction x="45.72" y="83.82"/>
<pinref part="U$13" gate="G$1" pin="B2"/>
<wire x1="38.1" y1="101.6" x2="45.72" y2="101.6" width="0.1524" layer="91"/>
<junction x="45.72" y="101.6"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="C1"/>
<wire x1="63.5" y1="104.14" x2="71.12" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY75" gate="GND" pin="GND"/>
<wire x1="71.12" y1="104.14" x2="71.12" y2="101.6" width="0.1524" layer="91"/>
<pinref part="U$13" gate="G$1" pin="C30"/>
<wire x1="71.12" y1="101.6" x2="71.12" y2="83.82" width="0.1524" layer="91"/>
<wire x1="71.12" y1="83.82" x2="71.12" y2="66.04" width="0.1524" layer="91"/>
<wire x1="71.12" y1="66.04" x2="71.12" y2="48.26" width="0.1524" layer="91"/>
<wire x1="71.12" y1="48.26" x2="71.12" y2="30.48" width="0.1524" layer="91"/>
<wire x1="71.12" y1="30.48" x2="71.12" y2="25.4" width="0.1524" layer="91"/>
<wire x1="63.5" y1="30.48" x2="71.12" y2="30.48" width="0.1524" layer="91"/>
<junction x="71.12" y="30.48"/>
<pinref part="U$13" gate="G$1" pin="C23"/>
<wire x1="63.5" y1="48.26" x2="71.12" y2="48.26" width="0.1524" layer="91"/>
<junction x="71.12" y="48.26"/>
<pinref part="U$13" gate="G$1" pin="C16"/>
<wire x1="63.5" y1="66.04" x2="71.12" y2="66.04" width="0.1524" layer="91"/>
<junction x="71.12" y="66.04"/>
<pinref part="U$13" gate="G$1" pin="C9"/>
<wire x1="63.5" y1="83.82" x2="71.12" y2="83.82" width="0.1524" layer="91"/>
<junction x="71.12" y="83.82"/>
<pinref part="U$13" gate="G$1" pin="C2"/>
<wire x1="63.5" y1="101.6" x2="71.12" y2="101.6" width="0.1524" layer="91"/>
<junction x="71.12" y="101.6"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="D1"/>
<wire x1="88.9" y1="104.14" x2="96.52" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY76" gate="GND" pin="GND"/>
<wire x1="96.52" y1="104.14" x2="96.52" y2="101.6" width="0.1524" layer="91"/>
<wire x1="96.52" y1="101.6" x2="96.52" y2="83.82" width="0.1524" layer="91"/>
<wire x1="96.52" y1="83.82" x2="96.52" y2="66.04" width="0.1524" layer="91"/>
<wire x1="96.52" y1="66.04" x2="96.52" y2="48.26" width="0.1524" layer="91"/>
<wire x1="96.52" y1="48.26" x2="96.52" y2="30.48" width="0.1524" layer="91"/>
<wire x1="96.52" y1="30.48" x2="96.52" y2="27.94" width="0.1524" layer="91"/>
<wire x1="96.52" y1="27.94" x2="99.06" y2="27.94" width="0.1524" layer="91"/>
<pinref part="U$13" gate="G$1" pin="D30"/>
<wire x1="88.9" y1="30.48" x2="96.52" y2="30.48" width="0.1524" layer="91"/>
<junction x="96.52" y="30.48"/>
<pinref part="U$13" gate="G$1" pin="D23"/>
<wire x1="88.9" y1="48.26" x2="96.52" y2="48.26" width="0.1524" layer="91"/>
<junction x="96.52" y="48.26"/>
<pinref part="U$13" gate="G$1" pin="D16"/>
<wire x1="88.9" y1="66.04" x2="96.52" y2="66.04" width="0.1524" layer="91"/>
<junction x="96.52" y="66.04"/>
<pinref part="U$13" gate="G$1" pin="D9"/>
<wire x1="88.9" y1="83.82" x2="96.52" y2="83.82" width="0.1524" layer="91"/>
<junction x="96.52" y="83.82"/>
<pinref part="U$13" gate="G$1" pin="D2"/>
<wire x1="88.9" y1="101.6" x2="96.52" y2="101.6" width="0.1524" layer="91"/>
<junction x="96.52" y="101.6"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="E1"/>
<wire x1="114.3" y1="104.14" x2="121.92" y2="104.14" width="0.1524" layer="91"/>
<pinref part="SUPPLY77" gate="GND" pin="GND"/>
<wire x1="121.92" y1="104.14" x2="121.92" y2="30.48" width="0.1524" layer="91"/>
<pinref part="U$13" gate="G$1" pin="E30"/>
<wire x1="121.92" y1="30.48" x2="121.92" y2="27.94" width="0.1524" layer="91"/>
<wire x1="114.3" y1="30.48" x2="121.92" y2="30.48" width="0.1524" layer="91"/>
<junction x="121.92" y="30.48"/>
</segment>
<segment>
<pinref part="U$13" gate="G$1" pin="F1"/>
<pinref part="SUPPLY78" gate="GND" pin="GND"/>
<wire x1="139.7" y1="104.14" x2="147.32" y2="104.14" width="0.1524" layer="91"/>
<wire x1="147.32" y1="104.14" x2="147.32" y2="30.48" width="0.1524" layer="91"/>
<pinref part="U$13" gate="G$1" pin="F30"/>
<wire x1="147.32" y1="30.48" x2="147.32" y2="25.4" width="0.1524" layer="91"/>
<wire x1="139.7" y1="30.48" x2="147.32" y2="30.48" width="0.1524" layer="91"/>
<junction x="147.32" y="30.48"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="F1"/>
<wire x1="139.7" y1="17.78" x2="147.32" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY84" gate="GND" pin="GND"/>
<wire x1="147.32" y1="17.78" x2="147.32" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="147.32" y1="-55.88" x2="147.32" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="147.32" y1="-58.42" x2="144.78" y2="-58.42" width="0.1524" layer="91"/>
<pinref part="U$14" gate="G$1" pin="F30"/>
<wire x1="139.7" y1="-55.88" x2="147.32" y2="-55.88" width="0.1524" layer="91"/>
<junction x="147.32" y="-55.88"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="E1"/>
<wire x1="114.3" y1="17.78" x2="121.92" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY83" gate="GND" pin="GND"/>
<wire x1="121.92" y1="17.78" x2="121.92" y2="-55.88" width="0.1524" layer="91"/>
<pinref part="U$14" gate="G$1" pin="E30"/>
<wire x1="121.92" y1="-55.88" x2="121.92" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="114.3" y1="-55.88" x2="121.92" y2="-55.88" width="0.1524" layer="91"/>
<junction x="121.92" y="-55.88"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="D1"/>
<wire x1="88.9" y1="17.78" x2="96.52" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY82" gate="GND" pin="GND"/>
<wire x1="96.52" y1="17.78" x2="96.52" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$14" gate="G$1" pin="D30"/>
<wire x1="96.52" y1="15.24" x2="96.52" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-2.54" x2="96.52" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-20.32" x2="96.52" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-38.1" x2="96.52" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="96.52" y1="-55.88" x2="96.52" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="88.9" y1="-55.88" x2="96.52" y2="-55.88" width="0.1524" layer="91"/>
<junction x="96.52" y="-55.88"/>
<pinref part="U$14" gate="G$1" pin="D23"/>
<wire x1="88.9" y1="-38.1" x2="96.52" y2="-38.1" width="0.1524" layer="91"/>
<junction x="96.52" y="-38.1"/>
<pinref part="U$14" gate="G$1" pin="D16"/>
<wire x1="88.9" y1="-20.32" x2="96.52" y2="-20.32" width="0.1524" layer="91"/>
<junction x="96.52" y="-20.32"/>
<pinref part="U$14" gate="G$1" pin="D9"/>
<wire x1="88.9" y1="-2.54" x2="96.52" y2="-2.54" width="0.1524" layer="91"/>
<junction x="96.52" y="-2.54"/>
<pinref part="U$14" gate="G$1" pin="D2"/>
<wire x1="88.9" y1="15.24" x2="96.52" y2="15.24" width="0.1524" layer="91"/>
<junction x="96.52" y="15.24"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="C1"/>
<wire x1="63.5" y1="17.78" x2="71.12" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY81" gate="GND" pin="GND"/>
<wire x1="71.12" y1="17.78" x2="71.12" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$14" gate="G$1" pin="C30"/>
<wire x1="71.12" y1="15.24" x2="71.12" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-2.54" x2="71.12" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-20.32" x2="71.12" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-38.1" x2="71.12" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="71.12" y1="-55.88" x2="71.12" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="63.5" y1="-55.88" x2="71.12" y2="-55.88" width="0.1524" layer="91"/>
<junction x="71.12" y="-55.88"/>
<pinref part="U$14" gate="G$1" pin="C23"/>
<wire x1="63.5" y1="-38.1" x2="71.12" y2="-38.1" width="0.1524" layer="91"/>
<junction x="71.12" y="-38.1"/>
<pinref part="U$14" gate="G$1" pin="C16"/>
<wire x1="63.5" y1="-20.32" x2="71.12" y2="-20.32" width="0.1524" layer="91"/>
<junction x="71.12" y="-20.32"/>
<pinref part="U$14" gate="G$1" pin="C9"/>
<wire x1="63.5" y1="-2.54" x2="71.12" y2="-2.54" width="0.1524" layer="91"/>
<junction x="71.12" y="-2.54"/>
<pinref part="U$14" gate="G$1" pin="C2"/>
<wire x1="63.5" y1="15.24" x2="71.12" y2="15.24" width="0.1524" layer="91"/>
<junction x="71.12" y="15.24"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="B1"/>
<wire x1="38.1" y1="17.78" x2="45.72" y2="17.78" width="0.1524" layer="91"/>
<pinref part="SUPPLY80" gate="GND" pin="GND"/>
<wire x1="45.72" y1="17.78" x2="45.72" y2="15.24" width="0.1524" layer="91"/>
<pinref part="U$14" gate="G$1" pin="B30"/>
<wire x1="45.72" y1="15.24" x2="45.72" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-2.54" x2="45.72" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-20.32" x2="45.72" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-38.1" x2="45.72" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-55.88" x2="45.72" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="38.1" y1="-55.88" x2="45.72" y2="-55.88" width="0.1524" layer="91"/>
<junction x="45.72" y="-55.88"/>
<pinref part="U$14" gate="G$1" pin="B23"/>
<wire x1="38.1" y1="-38.1" x2="45.72" y2="-38.1" width="0.1524" layer="91"/>
<junction x="45.72" y="-38.1"/>
<pinref part="U$14" gate="G$1" pin="B16"/>
<wire x1="38.1" y1="-20.32" x2="45.72" y2="-20.32" width="0.1524" layer="91"/>
<junction x="45.72" y="-20.32"/>
<pinref part="U$14" gate="G$1" pin="B9"/>
<wire x1="38.1" y1="-2.54" x2="45.72" y2="-2.54" width="0.1524" layer="91"/>
<junction x="45.72" y="-2.54"/>
<pinref part="U$14" gate="G$1" pin="B2"/>
<wire x1="38.1" y1="15.24" x2="45.72" y2="15.24" width="0.1524" layer="91"/>
<junction x="45.72" y="15.24"/>
</segment>
<segment>
<pinref part="U$14" gate="G$1" pin="A1"/>
<wire x1="12.7" y1="17.78" x2="20.32" y2="17.78" width="0.1524" layer="91"/>
<wire x1="20.32" y1="17.78" x2="20.32" y2="15.24" width="0.1524" layer="91"/>
<pinref part="SUPPLY79" gate="GND" pin="GND"/>
<wire x1="20.32" y1="15.24" x2="20.32" y2="12.7" width="0.1524" layer="91"/>
<wire x1="20.32" y1="12.7" x2="20.32" y2="10.16" width="0.1524" layer="91"/>
<wire x1="20.32" y1="10.16" x2="20.32" y2="7.62" width="0.1524" layer="91"/>
<wire x1="20.32" y1="7.62" x2="20.32" y2="5.08" width="0.1524" layer="91"/>
<wire x1="20.32" y1="5.08" x2="20.32" y2="2.54" width="0.1524" layer="91"/>
<wire x1="20.32" y1="2.54" x2="20.32" y2="0" width="0.1524" layer="91"/>
<wire x1="20.32" y1="0" x2="20.32" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-2.54" x2="20.32" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-5.08" x2="20.32" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-7.62" x2="20.32" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-10.16" x2="20.32" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-12.7" x2="20.32" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-15.24" x2="20.32" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-17.78" x2="20.32" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-20.32" x2="20.32" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-22.86" x2="20.32" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-25.4" x2="20.32" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-27.94" x2="20.32" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-30.48" x2="20.32" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-33.02" x2="20.32" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-35.56" x2="20.32" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-38.1" x2="20.32" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-40.64" x2="20.32" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-43.18" x2="20.32" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-45.72" x2="20.32" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-48.26" x2="20.32" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-50.8" x2="20.32" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-53.34" x2="20.32" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-58.42" x2="17.78" y2="-58.42" width="0.1524" layer="91"/>
<pinref part="U$14" gate="G$1" pin="A2"/>
<wire x1="12.7" y1="15.24" x2="20.32" y2="15.24" width="0.1524" layer="91"/>
<junction x="20.32" y="15.24"/>
<pinref part="U$14" gate="G$1" pin="A3"/>
<wire x1="12.7" y1="12.7" x2="20.32" y2="12.7" width="0.1524" layer="91"/>
<junction x="20.32" y="12.7"/>
<pinref part="U$14" gate="G$1" pin="A4"/>
<wire x1="12.7" y1="10.16" x2="20.32" y2="10.16" width="0.1524" layer="91"/>
<junction x="20.32" y="10.16"/>
<pinref part="U$14" gate="G$1" pin="A5"/>
<wire x1="12.7" y1="7.62" x2="20.32" y2="7.62" width="0.1524" layer="91"/>
<junction x="20.32" y="7.62"/>
<pinref part="U$14" gate="G$1" pin="A6"/>
<wire x1="12.7" y1="5.08" x2="20.32" y2="5.08" width="0.1524" layer="91"/>
<junction x="20.32" y="5.08"/>
<pinref part="U$14" gate="G$1" pin="A7"/>
<wire x1="12.7" y1="2.54" x2="20.32" y2="2.54" width="0.1524" layer="91"/>
<junction x="20.32" y="2.54"/>
<pinref part="U$14" gate="G$1" pin="A8"/>
<wire x1="12.7" y1="0" x2="20.32" y2="0" width="0.1524" layer="91"/>
<junction x="20.32" y="0"/>
<pinref part="U$14" gate="G$1" pin="A9"/>
<wire x1="12.7" y1="-2.54" x2="20.32" y2="-2.54" width="0.1524" layer="91"/>
<junction x="20.32" y="-2.54"/>
<pinref part="U$14" gate="G$1" pin="A10"/>
<wire x1="12.7" y1="-5.08" x2="20.32" y2="-5.08" width="0.1524" layer="91"/>
<junction x="20.32" y="-5.08"/>
<pinref part="U$14" gate="G$1" pin="A11"/>
<wire x1="12.7" y1="-7.62" x2="20.32" y2="-7.62" width="0.1524" layer="91"/>
<junction x="20.32" y="-7.62"/>
<pinref part="U$14" gate="G$1" pin="A12"/>
<wire x1="12.7" y1="-10.16" x2="20.32" y2="-10.16" width="0.1524" layer="91"/>
<junction x="20.32" y="-10.16"/>
<pinref part="U$14" gate="G$1" pin="A13"/>
<wire x1="12.7" y1="-12.7" x2="20.32" y2="-12.7" width="0.1524" layer="91"/>
<junction x="20.32" y="-12.7"/>
<pinref part="U$14" gate="G$1" pin="A14"/>
<wire x1="12.7" y1="-15.24" x2="20.32" y2="-15.24" width="0.1524" layer="91"/>
<junction x="20.32" y="-15.24"/>
<pinref part="U$14" gate="G$1" pin="A15"/>
<wire x1="12.7" y1="-17.78" x2="20.32" y2="-17.78" width="0.1524" layer="91"/>
<junction x="20.32" y="-17.78"/>
<pinref part="U$14" gate="G$1" pin="A16"/>
<wire x1="12.7" y1="-20.32" x2="20.32" y2="-20.32" width="0.1524" layer="91"/>
<junction x="20.32" y="-20.32"/>
<pinref part="U$14" gate="G$1" pin="A17"/>
<wire x1="12.7" y1="-22.86" x2="20.32" y2="-22.86" width="0.1524" layer="91"/>
<junction x="20.32" y="-22.86"/>
<pinref part="U$14" gate="G$1" pin="A18"/>
<wire x1="12.7" y1="-25.4" x2="20.32" y2="-25.4" width="0.1524" layer="91"/>
<junction x="20.32" y="-25.4"/>
<pinref part="U$14" gate="G$1" pin="A19"/>
<wire x1="12.7" y1="-27.94" x2="20.32" y2="-27.94" width="0.1524" layer="91"/>
<junction x="20.32" y="-27.94"/>
<pinref part="U$14" gate="G$1" pin="A20"/>
<wire x1="12.7" y1="-30.48" x2="20.32" y2="-30.48" width="0.1524" layer="91"/>
<junction x="20.32" y="-30.48"/>
<pinref part="U$14" gate="G$1" pin="A21"/>
<wire x1="12.7" y1="-33.02" x2="20.32" y2="-33.02" width="0.1524" layer="91"/>
<junction x="20.32" y="-33.02"/>
<pinref part="U$14" gate="G$1" pin="A22"/>
<wire x1="12.7" y1="-35.56" x2="20.32" y2="-35.56" width="0.1524" layer="91"/>
<junction x="20.32" y="-35.56"/>
<pinref part="U$14" gate="G$1" pin="A23"/>
<wire x1="12.7" y1="-38.1" x2="20.32" y2="-38.1" width="0.1524" layer="91"/>
<junction x="20.32" y="-38.1"/>
<pinref part="U$14" gate="G$1" pin="A24"/>
<wire x1="12.7" y1="-40.64" x2="20.32" y2="-40.64" width="0.1524" layer="91"/>
<junction x="20.32" y="-40.64"/>
<pinref part="U$14" gate="G$1" pin="A25"/>
<wire x1="12.7" y1="-43.18" x2="20.32" y2="-43.18" width="0.1524" layer="91"/>
<junction x="20.32" y="-43.18"/>
<pinref part="U$14" gate="G$1" pin="A27"/>
<wire x1="12.7" y1="-48.26" x2="20.32" y2="-48.26" width="0.1524" layer="91"/>
<junction x="20.32" y="-48.26"/>
<pinref part="U$14" gate="G$1" pin="A26"/>
<wire x1="12.7" y1="-45.72" x2="20.32" y2="-45.72" width="0.1524" layer="91"/>
<junction x="20.32" y="-45.72"/>
<pinref part="U$14" gate="G$1" pin="A28"/>
<wire x1="12.7" y1="-50.8" x2="20.32" y2="-50.8" width="0.1524" layer="91"/>
<junction x="20.32" y="-50.8"/>
<pinref part="U$14" gate="G$1" pin="A29"/>
<wire x1="12.7" y1="-53.34" x2="20.32" y2="-53.34" width="0.1524" layer="91"/>
<junction x="20.32" y="-53.34"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="A30"/>
<wire x1="12.7" y1="30.48" x2="27.94" y2="30.48" width="0.1524" layer="91"/>
<wire x1="27.94" y1="30.48" x2="27.94" y2="25.4" width="0.1524" layer="91"/>
<pinref part="U$23" gate="G$1" pin="IO"/>
<wire x1="27.94" y1="25.4" x2="35.56" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.08.096" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B3"/>
<wire x1="38.1" y1="99.06" x2="48.26" y2="99.06" width="0.1016" layer="91"/>
<label x="48.26" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B4"/>
<wire x1="38.1" y1="96.52" x2="48.26" y2="96.52" width="0.1016" layer="91"/>
<label x="48.26" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.101" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B5"/>
<wire x1="38.1" y1="93.98" x2="48.26" y2="93.98" width="0.1016" layer="91"/>
<label x="48.26" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B6"/>
<wire x1="38.1" y1="91.44" x2="48.26" y2="91.44" width="0.1016" layer="91"/>
<label x="48.26" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B7"/>
<wire x1="38.1" y1="88.9" x2="48.26" y2="88.9" width="0.1016" layer="91"/>
<label x="48.26" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.109" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B8"/>
<wire x1="38.1" y1="86.36" x2="48.26" y2="86.36" width="0.1016" layer="91"/>
<label x="48.26" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.096" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B10"/>
<wire x1="38.1" y1="81.28" x2="48.26" y2="81.28" width="0.1016" layer="91"/>
<label x="48.26" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B11"/>
<wire x1="38.1" y1="78.74" x2="48.26" y2="78.74" width="0.1016" layer="91"/>
<label x="48.26" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B12"/>
<wire x1="38.1" y1="76.2" x2="48.26" y2="76.2" width="0.1016" layer="91"/>
<label x="48.26" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B13"/>
<wire x1="38.1" y1="73.66" x2="48.26" y2="73.66" width="0.1016" layer="91"/>
<label x="48.26" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.101" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B14"/>
<wire x1="38.1" y1="71.12" x2="48.26" y2="71.12" width="0.1016" layer="91"/>
<label x="48.26" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.096" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B15"/>
<wire x1="38.1" y1="68.58" x2="48.26" y2="68.58" width="0.1016" layer="91"/>
<label x="48.26" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.092" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B17"/>
<wire x1="38.1" y1="63.5" x2="48.26" y2="63.5" width="0.1016" layer="91"/>
<label x="48.26" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B18"/>
<wire x1="38.1" y1="60.96" x2="48.26" y2="60.96" width="0.1016" layer="91"/>
<label x="48.26" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.093" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B19"/>
<wire x1="38.1" y1="58.42" x2="48.26" y2="58.42" width="0.1016" layer="91"/>
<label x="48.26" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B20"/>
<wire x1="38.1" y1="55.88" x2="48.26" y2="55.88" width="0.1016" layer="91"/>
<label x="48.26" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.094" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B21"/>
<wire x1="38.1" y1="53.34" x2="48.26" y2="53.34" width="0.1016" layer="91"/>
<label x="48.26" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B22"/>
<wire x1="38.1" y1="50.8" x2="48.26" y2="50.8" width="0.1016" layer="91"/>
<label x="48.26" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.096" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B24"/>
<wire x1="38.1" y1="45.72" x2="48.26" y2="45.72" width="0.1016" layer="91"/>
<label x="48.26" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B25"/>
<wire x1="38.1" y1="43.18" x2="48.26" y2="43.18" width="0.1016" layer="91"/>
<label x="48.26" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B26"/>
<wire x1="38.1" y1="40.64" x2="48.26" y2="40.64" width="0.1016" layer="91"/>
<label x="48.26" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B27"/>
<wire x1="38.1" y1="38.1" x2="48.26" y2="38.1" width="0.1016" layer="91"/>
<label x="48.26" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B28"/>
<wire x1="38.1" y1="35.56" x2="48.26" y2="35.56" width="0.1016" layer="91"/>
<label x="48.26" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.096" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="B29"/>
<wire x1="38.1" y1="33.02" x2="48.26" y2="33.02" width="0.1016" layer="91"/>
<label x="48.26" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C3"/>
<wire x1="63.5" y1="99.06" x2="73.66" y2="99.06" width="0.1016" layer="91"/>
<label x="73.66" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C4"/>
<wire x1="63.5" y1="96.52" x2="73.66" y2="96.52" width="0.1016" layer="91"/>
<label x="73.66" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C5"/>
<wire x1="63.5" y1="93.98" x2="73.66" y2="93.98" width="0.1016" layer="91"/>
<label x="73.66" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C6"/>
<wire x1="63.5" y1="91.44" x2="73.66" y2="91.44" width="0.1016" layer="91"/>
<label x="73.66" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C7"/>
<wire x1="63.5" y1="88.9" x2="73.66" y2="88.9" width="0.1016" layer="91"/>
<label x="73.66" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.110" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C8"/>
<wire x1="63.5" y1="86.36" x2="73.66" y2="86.36" width="0.1016" layer="91"/>
<label x="73.66" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C10"/>
<wire x1="63.5" y1="81.28" x2="73.66" y2="81.28" width="0.1016" layer="91"/>
<label x="73.66" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C11"/>
<wire x1="63.5" y1="78.74" x2="73.66" y2="78.74" width="0.1016" layer="91"/>
<label x="73.66" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.108" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C12"/>
<wire x1="63.5" y1="76.2" x2="73.66" y2="76.2" width="0.1016" layer="91"/>
<label x="73.66" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C13"/>
<wire x1="63.5" y1="73.66" x2="73.66" y2="73.66" width="0.1016" layer="91"/>
<label x="73.66" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C14"/>
<wire x1="63.5" y1="71.12" x2="73.66" y2="71.12" width="0.1016" layer="91"/>
<label x="73.66" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C15"/>
<wire x1="63.5" y1="68.58" x2="73.66" y2="68.58" width="0.1016" layer="91"/>
<label x="73.66" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C17"/>
<wire x1="63.5" y1="63.5" x2="73.66" y2="63.5" width="0.1016" layer="91"/>
<label x="73.66" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C18"/>
<wire x1="63.5" y1="60.96" x2="73.66" y2="60.96" width="0.1016" layer="91"/>
<label x="73.66" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C19"/>
<wire x1="63.5" y1="58.42" x2="73.66" y2="58.42" width="0.1016" layer="91"/>
<label x="73.66" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C20"/>
<wire x1="63.5" y1="55.88" x2="73.66" y2="55.88" width="0.1016" layer="91"/>
<label x="73.66" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C21"/>
<wire x1="63.5" y1="53.34" x2="73.66" y2="53.34" width="0.1016" layer="91"/>
<label x="73.66" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C22"/>
<wire x1="63.5" y1="50.8" x2="73.66" y2="50.8" width="0.1016" layer="91"/>
<label x="73.66" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C24"/>
<wire x1="63.5" y1="45.72" x2="73.66" y2="45.72" width="0.1016" layer="91"/>
<label x="73.66" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C25"/>
<wire x1="63.5" y1="43.18" x2="73.66" y2="43.18" width="0.1016" layer="91"/>
<label x="73.66" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.108" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C26"/>
<wire x1="63.5" y1="40.64" x2="73.66" y2="40.64" width="0.1016" layer="91"/>
<label x="73.66" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.108" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C27"/>
<wire x1="63.5" y1="38.1" x2="73.66" y2="38.1" width="0.1016" layer="91"/>
<label x="73.66" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C28"/>
<wire x1="63.5" y1="35.56" x2="73.66" y2="35.56" width="0.1016" layer="91"/>
<label x="73.66" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="C29"/>
<wire x1="63.5" y1="33.02" x2="73.66" y2="33.02" width="0.1016" layer="91"/>
<label x="73.66" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.097" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D3"/>
<wire x1="88.9" y1="99.06" x2="99.06" y2="99.06" width="0.1016" layer="91"/>
<label x="99.06" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.100" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D4"/>
<wire x1="88.9" y1="96.52" x2="99.06" y2="96.52" width="0.1016" layer="91"/>
<label x="99.06" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D5"/>
<wire x1="88.9" y1="93.98" x2="99.06" y2="93.98" width="0.1016" layer="91"/>
<label x="99.06" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D6"/>
<wire x1="88.9" y1="91.44" x2="99.06" y2="91.44" width="0.1016" layer="91"/>
<label x="99.06" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.108" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D7"/>
<wire x1="88.9" y1="88.9" x2="99.06" y2="88.9" width="0.1016" layer="91"/>
<label x="99.06" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.110" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D8"/>
<wire x1="88.9" y1="86.36" x2="99.06" y2="86.36" width="0.1016" layer="91"/>
<label x="99.06" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D10"/>
<wire x1="88.9" y1="81.28" x2="99.06" y2="81.28" width="0.1016" layer="91"/>
<label x="99.06" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D11"/>
<wire x1="88.9" y1="78.74" x2="99.06" y2="78.74" width="0.1016" layer="91"/>
<label x="99.06" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.109" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D12"/>
<wire x1="88.9" y1="76.2" x2="99.06" y2="76.2" width="0.1016" layer="91"/>
<label x="99.06" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.108" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D13"/>
<wire x1="88.9" y1="73.66" x2="99.06" y2="73.66" width="0.1016" layer="91"/>
<label x="99.06" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D14"/>
<wire x1="88.9" y1="71.12" x2="99.06" y2="71.12" width="0.1016" layer="91"/>
<label x="99.06" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D15"/>
<wire x1="88.9" y1="68.58" x2="99.06" y2="68.58" width="0.1016" layer="91"/>
<label x="99.06" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.102" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D17"/>
<wire x1="88.9" y1="63.5" x2="99.06" y2="63.5" width="0.1016" layer="91"/>
<label x="99.06" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.092" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D18"/>
<wire x1="88.9" y1="60.96" x2="99.06" y2="60.96" width="0.1016" layer="91"/>
<label x="99.06" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D19"/>
<wire x1="88.9" y1="58.42" x2="99.06" y2="58.42" width="0.1016" layer="91"/>
<label x="99.06" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.093" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D20"/>
<wire x1="88.9" y1="55.88" x2="99.06" y2="55.88" width="0.1016" layer="91"/>
<label x="99.06" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D21"/>
<wire x1="88.9" y1="53.34" x2="99.06" y2="53.34" width="0.1016" layer="91"/>
<label x="99.06" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.094" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D22"/>
<wire x1="88.9" y1="50.8" x2="99.06" y2="50.8" width="0.1016" layer="91"/>
<label x="99.06" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D24"/>
<wire x1="88.9" y1="45.72" x2="99.06" y2="45.72" width="0.1016" layer="91"/>
<label x="99.06" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D25"/>
<wire x1="88.9" y1="43.18" x2="99.06" y2="43.18" width="0.1016" layer="91"/>
<label x="99.06" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.109" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D26"/>
<wire x1="88.9" y1="40.64" x2="99.06" y2="40.64" width="0.1016" layer="91"/>
<label x="99.06" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.109" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D27"/>
<wire x1="88.9" y1="38.1" x2="99.06" y2="38.1" width="0.1016" layer="91"/>
<label x="99.06" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D28"/>
<wire x1="88.9" y1="35.56" x2="99.06" y2="35.56" width="0.1016" layer="91"/>
<label x="99.06" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="D29"/>
<wire x1="88.9" y1="33.02" x2="99.06" y2="33.02" width="0.1016" layer="91"/>
<label x="99.06" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E3"/>
<wire x1="114.3" y1="99.06" x2="124.46" y2="99.06" width="0.1016" layer="91"/>
<label x="124.46" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.100" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E4"/>
<wire x1="114.3" y1="96.52" x2="124.46" y2="96.52" width="0.1016" layer="91"/>
<label x="124.46" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E5"/>
<wire x1="114.3" y1="93.98" x2="124.46" y2="93.98" width="0.1016" layer="91"/>
<label x="124.46" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E6"/>
<wire x1="114.3" y1="91.44" x2="124.46" y2="91.44" width="0.1016" layer="91"/>
<label x="124.46" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.108" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E7"/>
<wire x1="114.3" y1="88.9" x2="124.46" y2="88.9" width="0.1016" layer="91"/>
<label x="124.46" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E8"/>
<wire x1="114.3" y1="86.36" x2="124.46" y2="86.36" width="0.1016" layer="91"/>
<label x="124.46" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E10"/>
<wire x1="114.3" y1="81.28" x2="124.46" y2="81.28" width="0.1016" layer="91"/>
<label x="124.46" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E11"/>
<wire x1="114.3" y1="78.74" x2="124.46" y2="78.74" width="0.1016" layer="91"/>
<label x="124.46" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.110" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E12"/>
<wire x1="114.3" y1="76.2" x2="124.46" y2="76.2" width="0.1016" layer="91"/>
<label x="124.46" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.109" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E13"/>
<wire x1="114.3" y1="73.66" x2="124.46" y2="73.66" width="0.1016" layer="91"/>
<label x="124.46" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.104" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E14"/>
<wire x1="114.3" y1="71.12" x2="124.46" y2="71.12" width="0.1016" layer="91"/>
<label x="124.46" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E15"/>
<wire x1="114.3" y1="68.58" x2="124.46" y2="68.58" width="0.1016" layer="91"/>
<label x="124.46" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.112" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E17"/>
<wire x1="114.3" y1="63.5" x2="124.46" y2="63.5" width="0.1016" layer="91"/>
<label x="124.46" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.117" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E18"/>
<wire x1="114.3" y1="60.96" x2="124.46" y2="60.96" width="0.1016" layer="91"/>
<label x="124.46" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.112" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E19"/>
<wire x1="114.3" y1="58.42" x2="124.46" y2="58.42" width="0.1016" layer="91"/>
<label x="124.46" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E20"/>
<wire x1="114.3" y1="55.88" x2="124.46" y2="55.88" width="0.1016" layer="91"/>
<label x="124.46" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.116" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E21"/>
<wire x1="114.3" y1="53.34" x2="124.46" y2="53.34" width="0.1016" layer="91"/>
<label x="124.46" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E22"/>
<wire x1="114.3" y1="50.8" x2="124.46" y2="50.8" width="0.1016" layer="91"/>
<label x="124.46" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E24"/>
<wire x1="114.3" y1="45.72" x2="124.46" y2="45.72" width="0.1016" layer="91"/>
<label x="124.46" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E25"/>
<wire x1="114.3" y1="43.18" x2="124.46" y2="43.18" width="0.1016" layer="91"/>
<label x="124.46" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E26"/>
<wire x1="114.3" y1="40.64" x2="124.46" y2="40.64" width="0.1016" layer="91"/>
<label x="124.46" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E27"/>
<wire x1="114.3" y1="38.1" x2="124.46" y2="38.1" width="0.1016" layer="91"/>
<label x="124.46" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.110" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E28"/>
<wire x1="114.3" y1="35.56" x2="124.46" y2="35.56" width="0.1016" layer="91"/>
<label x="124.46" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E29"/>
<wire x1="114.3" y1="33.02" x2="124.46" y2="33.02" width="0.1016" layer="91"/>
<label x="124.46" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.098" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F3"/>
<wire x1="139.7" y1="99.06" x2="149.86" y2="99.06" width="0.1016" layer="91"/>
<label x="149.86" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.101" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F4"/>
<wire x1="139.7" y1="96.52" x2="149.86" y2="96.52" width="0.1016" layer="91"/>
<label x="149.86" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.103" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F5"/>
<wire x1="139.7" y1="93.98" x2="149.86" y2="93.98" width="0.1016" layer="91"/>
<label x="149.86" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F6"/>
<wire x1="139.7" y1="91.44" x2="149.86" y2="91.44" width="0.1016" layer="91"/>
<label x="149.86" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.109" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F7"/>
<wire x1="139.7" y1="88.9" x2="149.86" y2="88.9" width="0.1016" layer="91"/>
<label x="149.86" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F8"/>
<wire x1="139.7" y1="86.36" x2="149.86" y2="86.36" width="0.1016" layer="91"/>
<label x="149.86" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.100" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F10"/>
<wire x1="139.7" y1="81.28" x2="149.86" y2="81.28" width="0.1016" layer="91"/>
<label x="149.86" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F11"/>
<wire x1="139.7" y1="78.74" x2="149.86" y2="78.74" width="0.1016" layer="91"/>
<label x="149.86" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F12"/>
<wire x1="139.7" y1="76.2" x2="149.86" y2="76.2" width="0.1016" layer="91"/>
<label x="149.86" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.110" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F13"/>
<wire x1="139.7" y1="73.66" x2="149.86" y2="73.66" width="0.1016" layer="91"/>
<label x="149.86" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F14"/>
<wire x1="139.7" y1="71.12" x2="149.86" y2="71.12" width="0.1016" layer="91"/>
<label x="149.86" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.100" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F15"/>
<wire x1="139.7" y1="68.58" x2="149.86" y2="68.58" width="0.1016" layer="91"/>
<label x="149.86" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F17"/>
<wire x1="139.7" y1="63.5" x2="149.86" y2="63.5" width="0.1016" layer="91"/>
<label x="149.86" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.117" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F18"/>
<wire x1="139.7" y1="60.96" x2="149.86" y2="60.96" width="0.1016" layer="91"/>
<label x="149.86" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F19"/>
<wire x1="139.7" y1="58.42" x2="149.86" y2="58.42" width="0.1016" layer="91"/>
<label x="149.86" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F20"/>
<wire x1="139.7" y1="55.88" x2="149.86" y2="55.88" width="0.1016" layer="91"/>
<label x="149.86" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.116" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F21"/>
<wire x1="139.7" y1="53.34" x2="149.86" y2="53.34" width="0.1016" layer="91"/>
<label x="149.86" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F22"/>
<wire x1="139.7" y1="50.8" x2="149.86" y2="50.8" width="0.1016" layer="91"/>
<label x="149.86" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.101" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F24"/>
<wire x1="139.7" y1="45.72" x2="149.86" y2="45.72" width="0.1016" layer="91"/>
<label x="149.86" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F25"/>
<wire x1="139.7" y1="43.18" x2="149.86" y2="43.18" width="0.1016" layer="91"/>
<label x="149.86" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.100" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F26"/>
<wire x1="139.7" y1="40.64" x2="149.86" y2="40.64" width="0.1016" layer="91"/>
<label x="149.86" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.105" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F27"/>
<wire x1="139.7" y1="38.1" x2="149.86" y2="38.1" width="0.1016" layer="91"/>
<label x="149.86" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.110" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F28"/>
<wire x1="139.7" y1="35.56" x2="149.86" y2="35.56" width="0.1016" layer="91"/>
<label x="149.86" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.099" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F29"/>
<wire x1="139.7" y1="33.02" x2="149.86" y2="33.02" width="0.1016" layer="91"/>
<label x="149.86" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.096" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E2"/>
<wire x1="114.3" y1="101.6" x2="124.46" y2="101.6" width="0.1016" layer="91"/>
<label x="124.46" y="101.6" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F2"/>
<wire x1="139.7" y1="101.6" x2="149.86" y2="101.6" width="0.1016" layer="91"/>
<label x="149.86" y="101.6" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.101" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E9"/>
<wire x1="114.3" y1="83.82" x2="124.46" y2="83.82" width="0.1016" layer="91"/>
<label x="124.46" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.111" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F9"/>
<wire x1="139.7" y1="83.82" x2="149.86" y2="83.82" width="0.1016" layer="91"/>
<label x="149.86" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.107" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E16"/>
<wire x1="114.3" y1="66.04" x2="124.46" y2="66.04" width="0.1016" layer="91"/>
<label x="124.46" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.106" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F16"/>
<wire x1="139.7" y1="66.04" x2="149.86" y2="66.04" width="0.1016" layer="91"/>
<label x="149.86" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.101" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="E23"/>
<wire x1="114.3" y1="48.26" x2="124.46" y2="48.26" width="0.1016" layer="91"/>
<label x="124.46" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.100" class="0">
<segment>
<pinref part="U$13" gate="G$1" pin="F23"/>
<wire x1="139.7" y1="48.26" x2="149.86" y2="48.26" width="0.1016" layer="91"/>
<label x="149.86" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B3"/>
<wire x1="38.1" y1="12.7" x2="48.26" y2="12.7" width="0.1016" layer="91"/>
<label x="48.26" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B4"/>
<wire x1="38.1" y1="10.16" x2="48.26" y2="10.16" width="0.1016" layer="91"/>
<label x="48.26" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B5"/>
<wire x1="38.1" y1="7.62" x2="48.26" y2="7.62" width="0.1016" layer="91"/>
<label x="48.26" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B6"/>
<wire x1="38.1" y1="5.08" x2="48.26" y2="5.08" width="0.1016" layer="91"/>
<label x="48.26" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B7"/>
<wire x1="38.1" y1="2.54" x2="48.26" y2="2.54" width="0.1016" layer="91"/>
<label x="48.26" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B8"/>
<wire x1="38.1" y1="0" x2="48.26" y2="0" width="0.1016" layer="91"/>
<label x="48.26" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B10"/>
<wire x1="38.1" y1="-5.08" x2="48.26" y2="-5.08" width="0.1016" layer="91"/>
<label x="48.26" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B11"/>
<wire x1="38.1" y1="-7.62" x2="48.26" y2="-7.62" width="0.1016" layer="91"/>
<label x="48.26" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B12"/>
<wire x1="38.1" y1="-10.16" x2="48.26" y2="-10.16" width="0.1016" layer="91"/>
<label x="48.26" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B13"/>
<wire x1="38.1" y1="-12.7" x2="48.26" y2="-12.7" width="0.1016" layer="91"/>
<label x="48.26" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B14"/>
<wire x1="38.1" y1="-15.24" x2="48.26" y2="-15.24" width="0.1016" layer="91"/>
<label x="48.26" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B15"/>
<wire x1="38.1" y1="-17.78" x2="48.26" y2="-17.78" width="0.1016" layer="91"/>
<label x="48.26" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B17"/>
<wire x1="38.1" y1="-22.86" x2="48.26" y2="-22.86" width="0.1016" layer="91"/>
<label x="48.26" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B18"/>
<wire x1="38.1" y1="-25.4" x2="48.26" y2="-25.4" width="0.1016" layer="91"/>
<label x="48.26" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B19"/>
<wire x1="38.1" y1="-27.94" x2="48.26" y2="-27.94" width="0.1016" layer="91"/>
<label x="48.26" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B20"/>
<wire x1="38.1" y1="-30.48" x2="48.26" y2="-30.48" width="0.1016" layer="91"/>
<label x="48.26" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B21"/>
<wire x1="38.1" y1="-33.02" x2="48.26" y2="-33.02" width="0.1016" layer="91"/>
<label x="48.26" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B22"/>
<wire x1="38.1" y1="-35.56" x2="48.26" y2="-35.56" width="0.1016" layer="91"/>
<label x="48.26" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B24"/>
<wire x1="38.1" y1="-40.64" x2="48.26" y2="-40.64" width="0.1016" layer="91"/>
<label x="48.26" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B25"/>
<wire x1="38.1" y1="-43.18" x2="48.26" y2="-43.18" width="0.1016" layer="91"/>
<label x="48.26" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B26"/>
<wire x1="38.1" y1="-45.72" x2="48.26" y2="-45.72" width="0.1016" layer="91"/>
<label x="48.26" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B27"/>
<wire x1="38.1" y1="-48.26" x2="48.26" y2="-48.26" width="0.1016" layer="91"/>
<label x="48.26" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B28"/>
<wire x1="38.1" y1="-50.8" x2="48.26" y2="-50.8" width="0.1016" layer="91"/>
<label x="48.26" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="B29"/>
<wire x1="38.1" y1="-53.34" x2="48.26" y2="-53.34" width="0.1016" layer="91"/>
<label x="48.26" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C3"/>
<wire x1="63.5" y1="12.7" x2="73.66" y2="12.7" width="0.1016" layer="91"/>
<label x="73.66" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C4"/>
<wire x1="63.5" y1="10.16" x2="73.66" y2="10.16" width="0.1016" layer="91"/>
<label x="73.66" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C5"/>
<wire x1="63.5" y1="7.62" x2="73.66" y2="7.62" width="0.1016" layer="91"/>
<label x="73.66" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C6"/>
<wire x1="63.5" y1="5.08" x2="73.66" y2="5.08" width="0.1016" layer="91"/>
<label x="73.66" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C7"/>
<wire x1="63.5" y1="2.54" x2="73.66" y2="2.54" width="0.1016" layer="91"/>
<label x="73.66" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C8"/>
<wire x1="63.5" y1="0" x2="73.66" y2="0" width="0.1016" layer="91"/>
<label x="73.66" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C10"/>
<wire x1="63.5" y1="-5.08" x2="73.66" y2="-5.08" width="0.1016" layer="91"/>
<label x="73.66" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C11"/>
<wire x1="63.5" y1="-7.62" x2="73.66" y2="-7.62" width="0.1016" layer="91"/>
<label x="73.66" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C12"/>
<wire x1="63.5" y1="-10.16" x2="73.66" y2="-10.16" width="0.1016" layer="91"/>
<label x="73.66" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C13"/>
<wire x1="63.5" y1="-12.7" x2="73.66" y2="-12.7" width="0.1016" layer="91"/>
<label x="73.66" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C14"/>
<wire x1="63.5" y1="-15.24" x2="73.66" y2="-15.24" width="0.1016" layer="91"/>
<label x="73.66" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C15"/>
<wire x1="63.5" y1="-17.78" x2="73.66" y2="-17.78" width="0.1016" layer="91"/>
<label x="73.66" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C17"/>
<wire x1="63.5" y1="-22.86" x2="73.66" y2="-22.86" width="0.1016" layer="91"/>
<label x="73.66" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C18"/>
<wire x1="63.5" y1="-25.4" x2="73.66" y2="-25.4" width="0.1016" layer="91"/>
<label x="73.66" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C19"/>
<wire x1="63.5" y1="-27.94" x2="73.66" y2="-27.94" width="0.1016" layer="91"/>
<label x="73.66" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C20"/>
<wire x1="63.5" y1="-30.48" x2="73.66" y2="-30.48" width="0.1016" layer="91"/>
<label x="73.66" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C21"/>
<wire x1="63.5" y1="-33.02" x2="73.66" y2="-33.02" width="0.1016" layer="91"/>
<label x="73.66" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C22"/>
<wire x1="63.5" y1="-35.56" x2="73.66" y2="-35.56" width="0.1016" layer="91"/>
<label x="73.66" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.107" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C24"/>
<wire x1="63.5" y1="-40.64" x2="73.66" y2="-40.64" width="0.1016" layer="91"/>
<label x="73.66" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C25"/>
<wire x1="63.5" y1="-43.18" x2="73.66" y2="-43.18" width="0.1016" layer="91"/>
<label x="73.66" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C26"/>
<wire x1="63.5" y1="-45.72" x2="73.66" y2="-45.72" width="0.1016" layer="91"/>
<label x="73.66" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C27"/>
<wire x1="63.5" y1="-48.26" x2="73.66" y2="-48.26" width="0.1016" layer="91"/>
<label x="73.66" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C28"/>
<wire x1="63.5" y1="-50.8" x2="73.66" y2="-50.8" width="0.1016" layer="91"/>
<label x="73.66" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="C29"/>
<wire x1="63.5" y1="-53.34" x2="73.66" y2="-53.34" width="0.1016" layer="91"/>
<label x="73.66" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D3"/>
<wire x1="88.9" y1="12.7" x2="99.06" y2="12.7" width="0.1016" layer="91"/>
<label x="99.06" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D4"/>
<wire x1="88.9" y1="10.16" x2="99.06" y2="10.16" width="0.1016" layer="91"/>
<label x="99.06" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D5"/>
<wire x1="88.9" y1="7.62" x2="99.06" y2="7.62" width="0.1016" layer="91"/>
<label x="99.06" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D6"/>
<wire x1="88.9" y1="5.08" x2="99.06" y2="5.08" width="0.1016" layer="91"/>
<label x="99.06" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D7"/>
<wire x1="88.9" y1="2.54" x2="99.06" y2="2.54" width="0.1016" layer="91"/>
<label x="99.06" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D8"/>
<wire x1="88.9" y1="0" x2="99.06" y2="0" width="0.1016" layer="91"/>
<label x="99.06" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D10"/>
<wire x1="88.9" y1="-5.08" x2="99.06" y2="-5.08" width="0.1016" layer="91"/>
<label x="99.06" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D11"/>
<wire x1="88.9" y1="-7.62" x2="99.06" y2="-7.62" width="0.1016" layer="91"/>
<label x="99.06" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.102" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D12"/>
<wire x1="88.9" y1="-10.16" x2="99.06" y2="-10.16" width="0.1016" layer="91"/>
<label x="99.06" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D13"/>
<wire x1="88.9" y1="-12.7" x2="99.06" y2="-12.7" width="0.1016" layer="91"/>
<label x="99.06" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D14"/>
<wire x1="88.9" y1="-15.24" x2="99.06" y2="-15.24" width="0.1016" layer="91"/>
<label x="99.06" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D15"/>
<wire x1="88.9" y1="-17.78" x2="99.06" y2="-17.78" width="0.1016" layer="91"/>
<label x="99.06" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D17"/>
<wire x1="88.9" y1="-22.86" x2="99.06" y2="-22.86" width="0.1016" layer="91"/>
<label x="99.06" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D18"/>
<wire x1="88.9" y1="-25.4" x2="99.06" y2="-25.4" width="0.1016" layer="91"/>
<label x="99.06" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D19"/>
<wire x1="88.9" y1="-27.94" x2="99.06" y2="-27.94" width="0.1016" layer="91"/>
<label x="99.06" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D20"/>
<wire x1="88.9" y1="-30.48" x2="99.06" y2="-30.48" width="0.1016" layer="91"/>
<label x="99.06" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D21"/>
<wire x1="88.9" y1="-33.02" x2="99.06" y2="-33.02" width="0.1016" layer="91"/>
<label x="99.06" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D22"/>
<wire x1="88.9" y1="-35.56" x2="99.06" y2="-35.56" width="0.1016" layer="91"/>
<label x="99.06" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D24"/>
<wire x1="88.9" y1="-40.64" x2="99.06" y2="-40.64" width="0.1016" layer="91"/>
<label x="99.06" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D25"/>
<wire x1="88.9" y1="-43.18" x2="99.06" y2="-43.18" width="0.1016" layer="91"/>
<label x="99.06" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D26"/>
<wire x1="88.9" y1="-45.72" x2="99.06" y2="-45.72" width="0.1016" layer="91"/>
<label x="99.06" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D27"/>
<wire x1="88.9" y1="-48.26" x2="99.06" y2="-48.26" width="0.1016" layer="91"/>
<label x="99.06" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D28"/>
<wire x1="88.9" y1="-50.8" x2="99.06" y2="-50.8" width="0.1016" layer="91"/>
<label x="99.06" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.098" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="D29"/>
<wire x1="88.9" y1="-53.34" x2="99.06" y2="-53.34" width="0.1016" layer="91"/>
<label x="99.06" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E3"/>
<wire x1="114.3" y1="12.7" x2="124.46" y2="12.7" width="0.1016" layer="91"/>
<label x="124.46" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E4"/>
<wire x1="114.3" y1="10.16" x2="124.46" y2="10.16" width="0.1016" layer="91"/>
<label x="124.46" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E5"/>
<wire x1="114.3" y1="7.62" x2="124.46" y2="7.62" width="0.1016" layer="91"/>
<label x="124.46" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E6"/>
<wire x1="114.3" y1="5.08" x2="124.46" y2="5.08" width="0.1016" layer="91"/>
<label x="124.46" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E7"/>
<wire x1="114.3" y1="2.54" x2="124.46" y2="2.54" width="0.1016" layer="91"/>
<label x="124.46" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E8"/>
<wire x1="114.3" y1="0" x2="124.46" y2="0" width="0.1016" layer="91"/>
<label x="124.46" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E10"/>
<wire x1="114.3" y1="-5.08" x2="124.46" y2="-5.08" width="0.1016" layer="91"/>
<label x="124.46" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E11"/>
<wire x1="114.3" y1="-7.62" x2="124.46" y2="-7.62" width="0.1016" layer="91"/>
<label x="124.46" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E12"/>
<wire x1="114.3" y1="-10.16" x2="124.46" y2="-10.16" width="0.1016" layer="91"/>
<label x="124.46" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E13"/>
<wire x1="114.3" y1="-12.7" x2="124.46" y2="-12.7" width="0.1016" layer="91"/>
<label x="124.46" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.108" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E14"/>
<wire x1="114.3" y1="-15.24" x2="124.46" y2="-15.24" width="0.1016" layer="91"/>
<label x="124.46" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E15"/>
<wire x1="114.3" y1="-17.78" x2="124.46" y2="-17.78" width="0.1016" layer="91"/>
<label x="124.46" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E17"/>
<wire x1="114.3" y1="-22.86" x2="124.46" y2="-22.86" width="0.1016" layer="91"/>
<label x="124.46" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E18"/>
<wire x1="114.3" y1="-25.4" x2="124.46" y2="-25.4" width="0.1016" layer="91"/>
<label x="124.46" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E19"/>
<wire x1="114.3" y1="-27.94" x2="124.46" y2="-27.94" width="0.1016" layer="91"/>
<label x="124.46" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E20"/>
<wire x1="114.3" y1="-30.48" x2="124.46" y2="-30.48" width="0.1016" layer="91"/>
<label x="124.46" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E21"/>
<wire x1="114.3" y1="-33.02" x2="124.46" y2="-33.02" width="0.1016" layer="91"/>
<label x="124.46" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E22"/>
<wire x1="114.3" y1="-35.56" x2="124.46" y2="-35.56" width="0.1016" layer="91"/>
<label x="124.46" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E24"/>
<wire x1="114.3" y1="-40.64" x2="124.46" y2="-40.64" width="0.1016" layer="91"/>
<label x="124.46" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E25"/>
<wire x1="114.3" y1="-43.18" x2="124.46" y2="-43.18" width="0.1016" layer="91"/>
<label x="124.46" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E26"/>
<wire x1="114.3" y1="-45.72" x2="124.46" y2="-45.72" width="0.1016" layer="91"/>
<label x="124.46" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E27"/>
<wire x1="114.3" y1="-48.26" x2="124.46" y2="-48.26" width="0.1016" layer="91"/>
<label x="124.46" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E28"/>
<wire x1="114.3" y1="-50.8" x2="124.46" y2="-50.8" width="0.1016" layer="91"/>
<label x="124.46" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E29"/>
<wire x1="114.3" y1="-53.34" x2="124.46" y2="-53.34" width="0.1016" layer="91"/>
<label x="124.46" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F3"/>
<wire x1="139.7" y1="12.7" x2="149.86" y2="12.7" width="0.1016" layer="91"/>
<label x="149.86" y="12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F4"/>
<wire x1="139.7" y1="10.16" x2="149.86" y2="10.16" width="0.1016" layer="91"/>
<label x="149.86" y="10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F5"/>
<wire x1="139.7" y1="7.62" x2="149.86" y2="7.62" width="0.1016" layer="91"/>
<label x="149.86" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F6"/>
<wire x1="139.7" y1="5.08" x2="149.86" y2="5.08" width="0.1016" layer="91"/>
<label x="149.86" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F7"/>
<wire x1="139.7" y1="2.54" x2="149.86" y2="2.54" width="0.1016" layer="91"/>
<label x="149.86" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F8"/>
<wire x1="139.7" y1="0" x2="149.86" y2="0" width="0.1016" layer="91"/>
<label x="149.86" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F10"/>
<wire x1="139.7" y1="-5.08" x2="149.86" y2="-5.08" width="0.1016" layer="91"/>
<label x="149.86" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.096" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F11"/>
<wire x1="139.7" y1="-7.62" x2="149.86" y2="-7.62" width="0.1016" layer="91"/>
<label x="149.86" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.103" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F12"/>
<wire x1="139.7" y1="-10.16" x2="149.86" y2="-10.16" width="0.1016" layer="91"/>
<label x="149.86" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F13"/>
<wire x1="139.7" y1="-12.7" x2="149.86" y2="-12.7" width="0.1016" layer="91"/>
<label x="149.86" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.109" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F14"/>
<wire x1="139.7" y1="-15.24" x2="149.86" y2="-15.24" width="0.1016" layer="91"/>
<label x="149.86" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F15"/>
<wire x1="139.7" y1="-17.78" x2="149.86" y2="-17.78" width="0.1016" layer="91"/>
<label x="149.86" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F17"/>
<wire x1="139.7" y1="-22.86" x2="149.86" y2="-22.86" width="0.1016" layer="91"/>
<label x="149.86" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F18"/>
<wire x1="139.7" y1="-25.4" x2="149.86" y2="-25.4" width="0.1016" layer="91"/>
<label x="149.86" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F19"/>
<wire x1="139.7" y1="-27.94" x2="149.86" y2="-27.94" width="0.1016" layer="91"/>
<label x="149.86" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F20"/>
<wire x1="139.7" y1="-30.48" x2="149.86" y2="-30.48" width="0.1016" layer="91"/>
<label x="149.86" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.105" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F21"/>
<wire x1="139.7" y1="-33.02" x2="149.86" y2="-33.02" width="0.1016" layer="91"/>
<label x="149.86" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F22"/>
<wire x1="139.7" y1="-35.56" x2="149.86" y2="-35.56" width="0.1016" layer="91"/>
<label x="149.86" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F24"/>
<wire x1="139.7" y1="-40.64" x2="149.86" y2="-40.64" width="0.1016" layer="91"/>
<label x="149.86" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F25"/>
<wire x1="139.7" y1="-43.18" x2="149.86" y2="-43.18" width="0.1016" layer="91"/>
<label x="149.86" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F26"/>
<wire x1="139.7" y1="-45.72" x2="149.86" y2="-45.72" width="0.1016" layer="91"/>
<label x="149.86" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.104" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F27"/>
<wire x1="139.7" y1="-48.26" x2="149.86" y2="-48.26" width="0.1016" layer="91"/>
<label x="149.86" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.110" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F28"/>
<wire x1="139.7" y1="-50.8" x2="149.86" y2="-50.8" width="0.1016" layer="91"/>
<label x="149.86" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.099" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F29"/>
<wire x1="139.7" y1="-53.34" x2="149.86" y2="-53.34" width="0.1016" layer="91"/>
<label x="149.86" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E2"/>
<wire x1="114.3" y1="15.24" x2="124.46" y2="15.24" width="0.1016" layer="91"/>
<label x="124.46" y="15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F2"/>
<wire x1="139.7" y1="15.24" x2="149.86" y2="15.24" width="0.1016" layer="91"/>
<label x="149.86" y="15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.097" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E9"/>
<wire x1="114.3" y1="-2.54" x2="124.46" y2="-2.54" width="0.1016" layer="91"/>
<label x="124.46" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.106" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F9"/>
<wire x1="139.7" y1="-2.54" x2="149.86" y2="-2.54" width="0.1016" layer="91"/>
<label x="149.86" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E16"/>
<wire x1="114.3" y1="-20.32" x2="124.46" y2="-20.32" width="0.1016" layer="91"/>
<label x="124.46" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.111" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F16"/>
<wire x1="139.7" y1="-20.32" x2="149.86" y2="-20.32" width="0.1016" layer="91"/>
<label x="149.86" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.101" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="E23"/>
<wire x1="114.3" y1="-38.1" x2="124.46" y2="-38.1" width="0.1016" layer="91"/>
<label x="124.46" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.100" class="0">
<segment>
<pinref part="U$14" gate="G$1" pin="F23"/>
<wire x1="139.7" y1="-38.1" x2="149.86" y2="-38.1" width="0.1016" layer="91"/>
<label x="149.86" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$15" gate="G$1" x="58.42" y="73.66" smashed="yes"/>
<instance part="U$16" gate="G$1" x="58.42" y="-17.78" smashed="yes"/>
<instance part="SUPPLY85" gate="GND" x="22.86" y="22.86" smashed="yes">
<attribute name="VALUE" x="20.955" y="19.685" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY86" gate="GND" x="48.26" y="20.32" smashed="yes">
<attribute name="VALUE" x="46.355" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY87" gate="GND" x="71.12" y="20.32" smashed="yes">
<attribute name="VALUE" x="69.215" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY88" gate="GND" x="96.52" y="20.32" smashed="yes">
<attribute name="VALUE" x="94.615" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY89" gate="GND" x="121.92" y="20.32" smashed="yes">
<attribute name="VALUE" x="120.015" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY90" gate="GND" x="147.32" y="20.32" smashed="yes">
<attribute name="VALUE" x="145.415" y="17.145" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY91" gate="GND" x="20.32" y="-68.58" smashed="yes">
<attribute name="VALUE" x="18.415" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY92" gate="GND" x="48.26" y="-68.58" smashed="yes">
<attribute name="VALUE" x="46.355" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY93" gate="GND" x="71.12" y="-68.58" smashed="yes">
<attribute name="VALUE" x="69.215" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY94" gate="GND" x="96.52" y="-68.58" smashed="yes">
<attribute name="VALUE" x="94.615" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY95" gate="GND" x="124.46" y="-68.58" smashed="yes">
<attribute name="VALUE" x="122.555" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY96" gate="GND" x="149.86" y="-68.58" smashed="yes">
<attribute name="VALUE" x="147.955" y="-71.755" size="1.778" layer="96"/>
</instance>
<instance part="U$24" gate="G$1" x="38.1" y="20.32" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="101.6" x2="22.86" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY85" gate="GND" pin="GND"/>
<wire x1="22.86" y1="101.6" x2="22.86" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$15" gate="G$1" pin="A29"/>
<wire x1="22.86" y1="99.06" x2="22.86" y2="96.52" width="0.1524" layer="91"/>
<wire x1="22.86" y1="96.52" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<wire x1="22.86" y1="93.98" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<wire x1="22.86" y1="91.44" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<wire x1="22.86" y1="88.9" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<wire x1="22.86" y1="86.36" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<wire x1="22.86" y1="83.82" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<wire x1="22.86" y1="81.28" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<wire x1="22.86" y1="78.74" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<wire x1="22.86" y1="76.2" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<wire x1="22.86" y1="73.66" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<wire x1="22.86" y1="71.12" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<wire x1="22.86" y1="68.58" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<wire x1="22.86" y1="66.04" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="63.5" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="60.96" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="58.42" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="55.88" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="53.34" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="50.8" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="48.26" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="45.72" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="43.18" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="40.64" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="38.1" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="35.56" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="33.02" x2="22.86" y2="30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="30.48" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<wire x1="15.24" y1="30.48" x2="22.86" y2="30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="30.48"/>
<pinref part="U$15" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="33.02" x2="22.86" y2="33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="33.02"/>
<pinref part="U$15" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="35.56" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="35.56"/>
<pinref part="U$15" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="38.1" x2="22.86" y2="38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="38.1"/>
<pinref part="U$15" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="40.64" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="40.64"/>
<pinref part="U$15" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="43.18" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="43.18"/>
<pinref part="U$15" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="45.72" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="45.72"/>
<pinref part="U$15" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="48.26" x2="22.86" y2="48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="48.26"/>
<pinref part="U$15" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="50.8" x2="22.86" y2="50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="50.8"/>
<pinref part="U$15" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="53.34" x2="22.86" y2="53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="53.34"/>
<pinref part="U$15" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="55.88" x2="22.86" y2="55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="55.88"/>
<pinref part="U$15" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="58.42" x2="22.86" y2="58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="58.42"/>
<pinref part="U$15" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="60.96" x2="22.86" y2="60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="60.96"/>
<pinref part="U$15" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="63.5" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<junction x="22.86" y="63.5"/>
<pinref part="U$15" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="66.04" x2="22.86" y2="66.04" width="0.1524" layer="91"/>
<junction x="22.86" y="66.04"/>
<pinref part="U$15" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="68.58" x2="22.86" y2="68.58" width="0.1524" layer="91"/>
<junction x="22.86" y="68.58"/>
<pinref part="U$15" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="71.12" x2="22.86" y2="71.12" width="0.1524" layer="91"/>
<junction x="22.86" y="71.12"/>
<pinref part="U$15" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="73.66" x2="22.86" y2="73.66" width="0.1524" layer="91"/>
<junction x="22.86" y="73.66"/>
<pinref part="U$15" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="76.2" x2="22.86" y2="76.2" width="0.1524" layer="91"/>
<junction x="22.86" y="76.2"/>
<pinref part="U$15" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="78.74" x2="22.86" y2="78.74" width="0.1524" layer="91"/>
<junction x="22.86" y="78.74"/>
<pinref part="U$15" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="81.28" x2="22.86" y2="81.28" width="0.1524" layer="91"/>
<junction x="22.86" y="81.28"/>
<pinref part="U$15" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="83.82" x2="22.86" y2="83.82" width="0.1524" layer="91"/>
<junction x="22.86" y="83.82"/>
<pinref part="U$15" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="86.36" x2="22.86" y2="86.36" width="0.1524" layer="91"/>
<junction x="22.86" y="86.36"/>
<pinref part="U$15" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="88.9" x2="22.86" y2="88.9" width="0.1524" layer="91"/>
<junction x="22.86" y="88.9"/>
<pinref part="U$15" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="91.44" x2="22.86" y2="91.44" width="0.1524" layer="91"/>
<junction x="22.86" y="91.44"/>
<pinref part="U$15" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="93.98" x2="22.86" y2="93.98" width="0.1524" layer="91"/>
<junction x="22.86" y="93.98"/>
<pinref part="U$15" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="96.52" x2="22.86" y2="96.52" width="0.1524" layer="91"/>
<junction x="22.86" y="96.52"/>
<pinref part="U$15" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="99.06" x2="22.86" y2="99.06" width="0.1524" layer="91"/>
<junction x="22.86" y="99.06"/>
<wire x1="22.86" y1="25.4" x2="30.48" y2="25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="25.4"/>
<wire x1="30.48" y1="25.4" x2="30.48" y2="20.32" width="0.1524" layer="91"/>
<pinref part="U$24" gate="G$1" pin="GND"/>
<wire x1="30.48" y1="20.32" x2="38.1" y2="20.32" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$15" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="101.6" x2="48.26" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY86" gate="GND" pin="GND"/>
<wire x1="48.26" y1="101.6" x2="48.26" y2="99.06" width="0.1524" layer="91"/>
<pinref part="U$15" gate="G$1" pin="B2"/>
<wire x1="48.26" y1="99.06" x2="48.26" y2="81.28" width="0.1524" layer="91"/>
<wire x1="48.26" y1="81.28" x2="48.26" y2="63.5" width="0.1524" layer="91"/>
<wire x1="48.26" y1="63.5" x2="48.26" y2="45.72" width="0.1524" layer="91"/>
<wire x1="48.26" y1="45.72" x2="48.26" y2="27.94" width="0.1524" layer="91"/>
<wire x1="48.26" y1="27.94" x2="48.26" y2="22.86" width="0.1524" layer="91"/>
<wire x1="40.64" y1="99.06" x2="48.26" y2="99.06" width="0.1524" layer="91"/>
<junction x="48.26" y="99.06"/>
<pinref part="U$15" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="81.28" x2="48.26" y2="81.28" width="0.1524" layer="91"/>
<junction x="48.26" y="81.28"/>
<pinref part="U$15" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="63.5" x2="48.26" y2="63.5" width="0.1524" layer="91"/>
<junction x="48.26" y="63.5"/>
<pinref part="U$15" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="45.72" x2="48.26" y2="45.72" width="0.1524" layer="91"/>
<junction x="48.26" y="45.72"/>
<pinref part="U$15" gate="G$1" pin="B30"/>
<wire x1="40.64" y1="27.94" x2="48.26" y2="27.94" width="0.1524" layer="91"/>
<junction x="48.26" y="27.94"/>
</segment>
<segment>
<pinref part="U$15" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="101.6" x2="73.66" y2="101.6" width="0.1524" layer="91"/>
<pinref part="SUPPLY87" gate="GND" pin="GND"/>
<wire x1="73.66" y1="101.6" x2="73.66" y2="99.06" width="0.1524" layer="91"/>
<wire x1="73.66" y1="99.06" x2="73.66" y2="81.28" width="0.1524" layer="91"/>
<wire x1="73.66" y1="81.28" x2="73.66" y2="63.5" width="0.1524" layer="91"/>
<wire x1="73.66" y1="63.5" x2="73.66" y2="45.72" width="0.1524" layer="91"/>
<wire x1="73.66" y1="45.72" x2="73.66" y2="27.94" width="0.1524" layer="91"/>
<wire x1="73.66" y1="27.94" x2="73.66" y2="22.86" width="0.1524" layer="91"/>
<wire x1="73.66" y1="22.86" x2="71.12" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$15" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="99.06" x2="73.66" y2="99.06" width="0.1524" layer="91"/>
<junction x="73.66" y="99.06"/>
<pinref part="U$15" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="81.28" x2="73.66" y2="81.28" width="0.1524" layer="91"/>
<junction x="73.66" y="81.28"/>
<pinref part="U$15" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="63.5" x2="73.66" y2="63.5" width="0.1524" layer="91"/>
<junction x="73.66" y="63.5"/>
<pinref part="U$15" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="45.72" x2="73.66" y2="45.72" width="0.1524" layer="91"/>
<junction x="73.66" y="45.72"/>
<pinref part="U$15" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="27.94" x2="73.66" y2="27.94" width="0.1524" layer="91"/>
<junction x="73.66" y="27.94"/>
</segment>
<segment>
<pinref part="U$15" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="101.6" x2="99.06" y2="101.6" width="0.1524" layer="91"/>
<wire x1="99.06" y1="101.6" x2="99.06" y2="99.06" width="0.1524" layer="91"/>
<pinref part="SUPPLY88" gate="GND" pin="GND"/>
<wire x1="99.06" y1="99.06" x2="99.06" y2="81.28" width="0.1524" layer="91"/>
<wire x1="99.06" y1="81.28" x2="99.06" y2="63.5" width="0.1524" layer="91"/>
<wire x1="99.06" y1="63.5" x2="99.06" y2="45.72" width="0.1524" layer="91"/>
<wire x1="99.06" y1="45.72" x2="99.06" y2="27.94" width="0.1524" layer="91"/>
<wire x1="99.06" y1="27.94" x2="99.06" y2="22.86" width="0.1524" layer="91"/>
<wire x1="99.06" y1="22.86" x2="96.52" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$15" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="99.06" x2="99.06" y2="99.06" width="0.1524" layer="91"/>
<junction x="99.06" y="99.06"/>
<pinref part="U$15" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="81.28" x2="99.06" y2="81.28" width="0.1524" layer="91"/>
<junction x="99.06" y="81.28"/>
<pinref part="U$15" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="63.5" x2="99.06" y2="63.5" width="0.1524" layer="91"/>
<junction x="99.06" y="63.5"/>
<pinref part="U$15" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="45.72" x2="99.06" y2="45.72" width="0.1524" layer="91"/>
<junction x="99.06" y="45.72"/>
<pinref part="U$15" gate="G$1" pin="D30"/>
<wire x1="91.44" y1="27.94" x2="99.06" y2="27.94" width="0.1524" layer="91"/>
<junction x="99.06" y="27.94"/>
</segment>
<segment>
<pinref part="U$15" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="101.6" x2="124.46" y2="101.6" width="0.1524" layer="91"/>
<wire x1="124.46" y1="101.6" x2="124.46" y2="27.94" width="0.1524" layer="91"/>
<pinref part="SUPPLY89" gate="GND" pin="GND"/>
<wire x1="124.46" y1="27.94" x2="124.46" y2="22.86" width="0.1524" layer="91"/>
<wire x1="124.46" y1="22.86" x2="121.92" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$15" gate="G$1" pin="E30"/>
<wire x1="116.84" y1="27.94" x2="124.46" y2="27.94" width="0.1524" layer="91"/>
<junction x="124.46" y="27.94"/>
</segment>
<segment>
<pinref part="U$15" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="101.6" x2="149.86" y2="101.6" width="0.1524" layer="91"/>
<wire x1="149.86" y1="101.6" x2="149.86" y2="27.94" width="0.1524" layer="91"/>
<pinref part="SUPPLY90" gate="GND" pin="GND"/>
<wire x1="149.86" y1="27.94" x2="149.86" y2="22.86" width="0.1524" layer="91"/>
<wire x1="149.86" y1="22.86" x2="147.32" y2="22.86" width="0.1524" layer="91"/>
<pinref part="U$15" gate="G$1" pin="F30"/>
<wire x1="142.24" y1="27.94" x2="149.86" y2="27.94" width="0.1524" layer="91"/>
<junction x="149.86" y="27.94"/>
</segment>
<segment>
<pinref part="U$16" gate="G$1" pin="F1"/>
<wire x1="142.24" y1="10.16" x2="149.86" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY96" gate="GND" pin="GND"/>
<wire x1="149.86" y1="10.16" x2="149.86" y2="-63.5" width="0.1524" layer="91"/>
<pinref part="U$16" gate="G$1" pin="F30"/>
<wire x1="149.86" y1="-63.5" x2="149.86" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="142.24" y1="-63.5" x2="149.86" y2="-63.5" width="0.1524" layer="91"/>
<junction x="149.86" y="-63.5"/>
</segment>
<segment>
<pinref part="U$16" gate="G$1" pin="E1"/>
<wire x1="116.84" y1="10.16" x2="124.46" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY95" gate="GND" pin="GND"/>
<wire x1="124.46" y1="10.16" x2="124.46" y2="-63.5" width="0.1524" layer="91"/>
<pinref part="U$16" gate="G$1" pin="E30"/>
<wire x1="124.46" y1="-63.5" x2="124.46" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="116.84" y1="-63.5" x2="124.46" y2="-63.5" width="0.1524" layer="91"/>
<junction x="124.46" y="-63.5"/>
</segment>
<segment>
<pinref part="U$16" gate="G$1" pin="D1"/>
<wire x1="91.44" y1="10.16" x2="99.06" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY94" gate="GND" pin="GND"/>
<wire x1="99.06" y1="10.16" x2="99.06" y2="7.62" width="0.1524" layer="91"/>
<wire x1="99.06" y1="7.62" x2="99.06" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-10.16" x2="99.06" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-27.94" x2="99.06" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-45.72" x2="99.06" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-63.5" x2="99.06" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="99.06" y1="-66.04" x2="96.52" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$16" gate="G$1" pin="D30"/>
<wire x1="91.44" y1="-63.5" x2="99.06" y2="-63.5" width="0.1524" layer="91"/>
<junction x="99.06" y="-63.5"/>
<pinref part="U$16" gate="G$1" pin="D2"/>
<wire x1="91.44" y1="7.62" x2="99.06" y2="7.62" width="0.1524" layer="91"/>
<junction x="99.06" y="7.62"/>
<pinref part="U$16" gate="G$1" pin="D9"/>
<wire x1="91.44" y1="-10.16" x2="99.06" y2="-10.16" width="0.1524" layer="91"/>
<junction x="99.06" y="-10.16"/>
<pinref part="U$16" gate="G$1" pin="D16"/>
<wire x1="91.44" y1="-27.94" x2="99.06" y2="-27.94" width="0.1524" layer="91"/>
<junction x="99.06" y="-27.94"/>
<pinref part="U$16" gate="G$1" pin="D23"/>
<wire x1="91.44" y1="-45.72" x2="99.06" y2="-45.72" width="0.1524" layer="91"/>
<junction x="99.06" y="-45.72"/>
</segment>
<segment>
<pinref part="U$16" gate="G$1" pin="C1"/>
<wire x1="66.04" y1="10.16" x2="73.66" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY93" gate="GND" pin="GND"/>
<wire x1="73.66" y1="10.16" x2="73.66" y2="7.62" width="0.1524" layer="91"/>
<wire x1="73.66" y1="7.62" x2="73.66" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-10.16" x2="73.66" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-27.94" x2="73.66" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-45.72" x2="73.66" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-63.5" x2="73.66" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="73.66" y1="-66.04" x2="71.12" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$16" gate="G$1" pin="C30"/>
<wire x1="66.04" y1="-63.5" x2="73.66" y2="-63.5" width="0.1524" layer="91"/>
<junction x="73.66" y="-63.5"/>
<pinref part="U$16" gate="G$1" pin="C2"/>
<wire x1="66.04" y1="7.62" x2="73.66" y2="7.62" width="0.1524" layer="91"/>
<junction x="73.66" y="7.62"/>
<pinref part="U$16" gate="G$1" pin="C9"/>
<wire x1="66.04" y1="-10.16" x2="73.66" y2="-10.16" width="0.1524" layer="91"/>
<junction x="73.66" y="-10.16"/>
<pinref part="U$16" gate="G$1" pin="C16"/>
<wire x1="66.04" y1="-27.94" x2="73.66" y2="-27.94" width="0.1524" layer="91"/>
<junction x="73.66" y="-27.94"/>
<pinref part="U$16" gate="G$1" pin="C23"/>
<wire x1="66.04" y1="-45.72" x2="73.66" y2="-45.72" width="0.1524" layer="91"/>
<junction x="73.66" y="-45.72"/>
</segment>
<segment>
<pinref part="U$16" gate="G$1" pin="B1"/>
<wire x1="40.64" y1="10.16" x2="48.26" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY92" gate="GND" pin="GND"/>
<wire x1="48.26" y1="10.16" x2="48.26" y2="7.62" width="0.1524" layer="91"/>
<pinref part="U$16" gate="G$1" pin="B30"/>
<wire x1="48.26" y1="7.62" x2="48.26" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-10.16" x2="48.26" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-27.94" x2="48.26" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-45.72" x2="48.26" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="48.26" y1="-63.5" x2="48.26" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-63.5" x2="48.26" y2="-63.5" width="0.1524" layer="91"/>
<junction x="48.26" y="-63.5"/>
<pinref part="U$16" gate="G$1" pin="B2"/>
<wire x1="40.64" y1="7.62" x2="48.26" y2="7.62" width="0.1524" layer="91"/>
<junction x="48.26" y="7.62"/>
<pinref part="U$16" gate="G$1" pin="B9"/>
<wire x1="40.64" y1="-10.16" x2="48.26" y2="-10.16" width="0.1524" layer="91"/>
<junction x="48.26" y="-10.16"/>
<pinref part="U$16" gate="G$1" pin="B16"/>
<wire x1="40.64" y1="-27.94" x2="48.26" y2="-27.94" width="0.1524" layer="91"/>
<junction x="48.26" y="-27.94"/>
<pinref part="U$16" gate="G$1" pin="B23"/>
<wire x1="40.64" y1="-45.72" x2="48.26" y2="-45.72" width="0.1524" layer="91"/>
<junction x="48.26" y="-45.72"/>
</segment>
<segment>
<pinref part="U$16" gate="G$1" pin="A1"/>
<wire x1="15.24" y1="10.16" x2="22.86" y2="10.16" width="0.1524" layer="91"/>
<pinref part="SUPPLY91" gate="GND" pin="GND"/>
<wire x1="22.86" y1="10.16" x2="22.86" y2="7.62" width="0.1524" layer="91"/>
<wire x1="22.86" y1="7.62" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="5.08" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="2.54" x2="22.86" y2="0" width="0.1524" layer="91"/>
<wire x1="22.86" y1="0" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-2.54" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-5.08" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-7.62" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-10.16" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-12.7" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-15.24" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-17.78" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-20.32" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-22.86" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-25.4" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-27.94" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-30.48" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-33.02" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-35.56" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-38.1" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-40.64" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-43.18" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-45.72" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-48.26" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-50.8" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-53.34" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-55.88" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-58.42" x2="22.86" y2="-60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-60.96" x2="22.86" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-66.04" x2="20.32" y2="-66.04" width="0.1524" layer="91"/>
<pinref part="U$16" gate="G$1" pin="A29"/>
<wire x1="15.24" y1="-60.96" x2="22.86" y2="-60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="-60.96"/>
<pinref part="U$16" gate="G$1" pin="A28"/>
<wire x1="15.24" y1="-58.42" x2="22.86" y2="-58.42" width="0.1524" layer="91"/>
<junction x="22.86" y="-58.42"/>
<pinref part="U$16" gate="G$1" pin="A27"/>
<wire x1="15.24" y1="-55.88" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<junction x="22.86" y="-55.88"/>
<pinref part="U$16" gate="G$1" pin="A26"/>
<wire x1="15.24" y1="-53.34" x2="22.86" y2="-53.34" width="0.1524" layer="91"/>
<junction x="22.86" y="-53.34"/>
<pinref part="U$16" gate="G$1" pin="A25"/>
<wire x1="15.24" y1="-50.8" x2="22.86" y2="-50.8" width="0.1524" layer="91"/>
<junction x="22.86" y="-50.8"/>
<pinref part="U$16" gate="G$1" pin="A24"/>
<wire x1="15.24" y1="-48.26" x2="22.86" y2="-48.26" width="0.1524" layer="91"/>
<junction x="22.86" y="-48.26"/>
<pinref part="U$16" gate="G$1" pin="A23"/>
<wire x1="15.24" y1="-45.72" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="-45.72"/>
<pinref part="U$16" gate="G$1" pin="A22"/>
<wire x1="15.24" y1="-43.18" x2="22.86" y2="-43.18" width="0.1524" layer="91"/>
<junction x="22.86" y="-43.18"/>
<pinref part="U$16" gate="G$1" pin="A21"/>
<wire x1="15.24" y1="-40.64" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="-40.64"/>
<pinref part="U$16" gate="G$1" pin="A20"/>
<wire x1="15.24" y1="-38.1" x2="22.86" y2="-38.1" width="0.1524" layer="91"/>
<junction x="22.86" y="-38.1"/>
<pinref part="U$16" gate="G$1" pin="A19"/>
<wire x1="15.24" y1="-35.56" x2="22.86" y2="-35.56" width="0.1524" layer="91"/>
<junction x="22.86" y="-35.56"/>
<pinref part="U$16" gate="G$1" pin="A18"/>
<wire x1="15.24" y1="-33.02" x2="22.86" y2="-33.02" width="0.1524" layer="91"/>
<junction x="22.86" y="-33.02"/>
<pinref part="U$16" gate="G$1" pin="A17"/>
<wire x1="15.24" y1="-30.48" x2="22.86" y2="-30.48" width="0.1524" layer="91"/>
<junction x="22.86" y="-30.48"/>
<pinref part="U$16" gate="G$1" pin="A16"/>
<wire x1="15.24" y1="-27.94" x2="22.86" y2="-27.94" width="0.1524" layer="91"/>
<junction x="22.86" y="-27.94"/>
<pinref part="U$16" gate="G$1" pin="A15"/>
<wire x1="15.24" y1="-25.4" x2="22.86" y2="-25.4" width="0.1524" layer="91"/>
<junction x="22.86" y="-25.4"/>
<pinref part="U$16" gate="G$1" pin="A14"/>
<wire x1="15.24" y1="-22.86" x2="22.86" y2="-22.86" width="0.1524" layer="91"/>
<junction x="22.86" y="-22.86"/>
<pinref part="U$16" gate="G$1" pin="A13"/>
<wire x1="15.24" y1="-20.32" x2="22.86" y2="-20.32" width="0.1524" layer="91"/>
<junction x="22.86" y="-20.32"/>
<pinref part="U$16" gate="G$1" pin="A12"/>
<wire x1="15.24" y1="-17.78" x2="22.86" y2="-17.78" width="0.1524" layer="91"/>
<junction x="22.86" y="-17.78"/>
<pinref part="U$16" gate="G$1" pin="A11"/>
<wire x1="15.24" y1="-15.24" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<junction x="22.86" y="-15.24"/>
<pinref part="U$16" gate="G$1" pin="A10"/>
<wire x1="15.24" y1="-12.7" x2="22.86" y2="-12.7" width="0.1524" layer="91"/>
<junction x="22.86" y="-12.7"/>
<pinref part="U$16" gate="G$1" pin="A9"/>
<wire x1="15.24" y1="-10.16" x2="22.86" y2="-10.16" width="0.1524" layer="91"/>
<junction x="22.86" y="-10.16"/>
<pinref part="U$16" gate="G$1" pin="A8"/>
<wire x1="15.24" y1="-7.62" x2="22.86" y2="-7.62" width="0.1524" layer="91"/>
<junction x="22.86" y="-7.62"/>
<pinref part="U$16" gate="G$1" pin="A7"/>
<wire x1="15.24" y1="-5.08" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="-5.08"/>
<pinref part="U$16" gate="G$1" pin="A6"/>
<wire x1="15.24" y1="-2.54" x2="22.86" y2="-2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="-2.54"/>
<pinref part="U$16" gate="G$1" pin="A5"/>
<wire x1="15.24" y1="0" x2="22.86" y2="0" width="0.1524" layer="91"/>
<junction x="22.86" y="0"/>
<pinref part="U$16" gate="G$1" pin="A4"/>
<wire x1="15.24" y1="2.54" x2="22.86" y2="2.54" width="0.1524" layer="91"/>
<junction x="22.86" y="2.54"/>
<pinref part="U$16" gate="G$1" pin="A3"/>
<wire x1="15.24" y1="5.08" x2="22.86" y2="5.08" width="0.1524" layer="91"/>
<junction x="22.86" y="5.08"/>
<pinref part="U$16" gate="G$1" pin="A2"/>
<wire x1="15.24" y1="7.62" x2="22.86" y2="7.62" width="0.1524" layer="91"/>
<junction x="22.86" y="7.62"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="A30"/>
<wire x1="15.24" y1="27.94" x2="33.02" y2="27.94" width="0.1524" layer="91"/>
<pinref part="U$24" gate="G$1" pin="IO"/>
<wire x1="33.02" y1="27.94" x2="33.02" y2="22.86" width="0.1524" layer="91"/>
<wire x1="33.02" y1="22.86" x2="38.1" y2="22.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ZZ.09.112" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="96.52" x2="50.8" y2="96.52" width="0.1016" layer="91"/>
<label x="50.8" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="93.98" x2="50.8" y2="93.98" width="0.1016" layer="91"/>
<label x="50.8" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.117" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="91.44" x2="50.8" y2="91.44" width="0.1016" layer="91"/>
<label x="50.8" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="88.9" x2="50.8" y2="88.9" width="0.1016" layer="91"/>
<label x="50.8" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="86.36" x2="50.8" y2="86.36" width="0.1016" layer="91"/>
<label x="50.8" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="83.82" x2="50.8" y2="83.82" width="0.1016" layer="91"/>
<label x="50.8" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.112" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="78.74" x2="50.8" y2="78.74" width="0.1016" layer="91"/>
<label x="50.8" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="76.2" x2="50.8" y2="76.2" width="0.1016" layer="91"/>
<label x="50.8" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="73.66" x2="50.8" y2="73.66" width="0.1016" layer="91"/>
<label x="50.8" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="71.12" x2="50.8" y2="71.12" width="0.1016" layer="91"/>
<label x="50.8" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.117" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="68.58" x2="50.8" y2="68.58" width="0.1016" layer="91"/>
<label x="50.8" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.112" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="66.04" x2="50.8" y2="66.04" width="0.1016" layer="91"/>
<label x="50.8" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.108" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="60.96" x2="50.8" y2="60.96" width="0.1016" layer="91"/>
<label x="50.8" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="58.42" x2="50.8" y2="58.42" width="0.1016" layer="91"/>
<label x="50.8" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.109" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="55.88" x2="50.8" y2="55.88" width="0.1016" layer="91"/>
<label x="50.8" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="53.34" x2="50.8" y2="53.34" width="0.1016" layer="91"/>
<label x="50.8" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.110" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="50.8" x2="50.8" y2="50.8" width="0.1016" layer="91"/>
<label x="50.8" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="48.26" x2="50.8" y2="48.26" width="0.1016" layer="91"/>
<label x="50.8" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.112" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="43.18" x2="50.8" y2="43.18" width="0.1016" layer="91"/>
<label x="50.8" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="40.64" x2="50.8" y2="40.64" width="0.1016" layer="91"/>
<label x="50.8" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="38.1" x2="50.8" y2="38.1" width="0.1016" layer="91"/>
<label x="50.8" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="35.56" x2="50.8" y2="35.56" width="0.1016" layer="91"/>
<label x="50.8" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="33.02" x2="50.8" y2="33.02" width="0.1016" layer="91"/>
<label x="50.8" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.112" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="30.48" x2="50.8" y2="30.48" width="0.1016" layer="91"/>
<label x="50.8" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.112" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="96.52" x2="76.2" y2="96.52" width="0.1016" layer="91"/>
<label x="76.2" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="93.98" x2="76.2" y2="93.98" width="0.1016" layer="91"/>
<label x="76.2" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="91.44" x2="76.2" y2="91.44" width="0.1016" layer="91"/>
<label x="76.2" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="88.9" x2="76.2" y2="88.9" width="0.1016" layer="91"/>
<label x="76.2" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="86.36" x2="76.2" y2="86.36" width="0.1016" layer="91"/>
<label x="76.2" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="83.82" x2="76.2" y2="83.82" width="0.1016" layer="91"/>
<label x="76.2" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="78.74" x2="76.2" y2="78.74" width="0.1016" layer="91"/>
<label x="76.2" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="76.2" x2="76.2" y2="76.2" width="0.1016" layer="91"/>
<label x="76.2" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="73.66" x2="76.2" y2="73.66" width="0.1016" layer="91"/>
<label x="76.2" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="71.12" x2="76.2" y2="71.12" width="0.1016" layer="91"/>
<label x="76.2" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="68.58" x2="76.2" y2="68.58" width="0.1016" layer="91"/>
<label x="76.2" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="66.04" x2="76.2" y2="66.04" width="0.1016" layer="91"/>
<label x="76.2" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="60.96" x2="76.2" y2="60.96" width="0.1016" layer="91"/>
<label x="76.2" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="58.42" x2="76.2" y2="58.42" width="0.1016" layer="91"/>
<label x="76.2" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="55.88" x2="76.2" y2="55.88" width="0.1016" layer="91"/>
<label x="76.2" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="53.34" x2="76.2" y2="53.34" width="0.1016" layer="91"/>
<label x="76.2" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="50.8" x2="76.2" y2="50.8" width="0.1016" layer="91"/>
<label x="76.2" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="48.26" x2="76.2" y2="48.26" width="0.1016" layer="91"/>
<label x="76.2" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="43.18" x2="76.2" y2="43.18" width="0.1016" layer="91"/>
<label x="76.2" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="40.64" x2="76.2" y2="40.64" width="0.1016" layer="91"/>
<label x="76.2" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="38.1" x2="76.2" y2="38.1" width="0.1016" layer="91"/>
<label x="76.2" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="35.56" x2="76.2" y2="35.56" width="0.1016" layer="91"/>
<label x="76.2" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="33.02" x2="76.2" y2="33.02" width="0.1016" layer="91"/>
<label x="76.2" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="30.48" x2="76.2" y2="30.48" width="0.1016" layer="91"/>
<label x="76.2" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="96.52" x2="101.6" y2="96.52" width="0.1016" layer="91"/>
<label x="101.6" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.116" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="93.98" x2="101.6" y2="93.98" width="0.1016" layer="91"/>
<label x="101.6" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="91.44" x2="101.6" y2="91.44" width="0.1016" layer="91"/>
<label x="101.6" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="88.9" x2="101.6" y2="88.9" width="0.1016" layer="91"/>
<label x="101.6" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="86.36" x2="101.6" y2="86.36" width="0.1016" layer="91"/>
<label x="101.6" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="83.82" x2="101.6" y2="83.82" width="0.1016" layer="91"/>
<label x="101.6" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="78.74" x2="101.6" y2="78.74" width="0.1016" layer="91"/>
<label x="101.6" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="76.2" x2="101.6" y2="76.2" width="0.1016" layer="91"/>
<label x="101.6" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="73.66" x2="101.6" y2="73.66" width="0.1016" layer="91"/>
<label x="101.6" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="71.12" x2="101.6" y2="71.12" width="0.1016" layer="91"/>
<label x="101.6" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="68.58" x2="101.6" y2="68.58" width="0.1016" layer="91"/>
<label x="101.6" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="66.04" x2="101.6" y2="66.04" width="0.1016" layer="91"/>
<label x="101.6" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.118" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="60.96" x2="101.6" y2="60.96" width="0.1016" layer="91"/>
<label x="101.6" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.108" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="58.42" x2="101.6" y2="58.42" width="0.1016" layer="91"/>
<label x="101.6" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="55.88" x2="101.6" y2="55.88" width="0.1016" layer="91"/>
<label x="101.6" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.109" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="53.34" x2="101.6" y2="53.34" width="0.1016" layer="91"/>
<label x="101.6" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="50.8" x2="101.6" y2="50.8" width="0.1016" layer="91"/>
<label x="101.6" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.110" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="48.26" x2="101.6" y2="48.26" width="0.1016" layer="91"/>
<label x="101.6" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="43.18" x2="101.6" y2="43.18" width="0.1016" layer="91"/>
<label x="101.6" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="40.64" x2="101.6" y2="40.64" width="0.1016" layer="91"/>
<label x="101.6" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="38.1" x2="101.6" y2="38.1" width="0.1016" layer="91"/>
<label x="101.6" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="35.56" x2="101.6" y2="35.56" width="0.1016" layer="91"/>
<label x="101.6" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="33.02" x2="101.6" y2="33.02" width="0.1016" layer="91"/>
<label x="101.6" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="30.48" x2="101.6" y2="30.48" width="0.1016" layer="91"/>
<label x="101.6" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.113" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="96.52" x2="127" y2="96.52" width="0.1016" layer="91"/>
<label x="127" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.116" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="93.98" x2="127" y2="93.98" width="0.1016" layer="91"/>
<label x="127" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="91.44" x2="127" y2="91.44" width="0.1016" layer="91"/>
<label x="127" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="88.9" x2="127" y2="88.9" width="0.1016" layer="91"/>
<label x="127" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="86.36" x2="127" y2="86.36" width="0.1016" layer="91"/>
<label x="127" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="83.82" x2="127" y2="83.82" width="0.1016" layer="91"/>
<label x="127" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="78.74" x2="127" y2="78.74" width="0.1016" layer="91"/>
<label x="127" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="76.2" x2="127" y2="76.2" width="0.1016" layer="91"/>
<label x="127" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="73.66" x2="127" y2="73.66" width="0.1016" layer="91"/>
<label x="127" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="71.12" x2="127" y2="71.12" width="0.1016" layer="91"/>
<label x="127" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.120" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="68.58" x2="127" y2="68.58" width="0.1016" layer="91"/>
<label x="127" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="66.04" x2="127" y2="66.04" width="0.1016" layer="91"/>
<label x="127" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="60.96" x2="127" y2="60.96" width="0.1016" layer="91"/>
<label x="127" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="58.42" x2="127" y2="58.42" width="0.1016" layer="91"/>
<label x="127" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="55.88" x2="127" y2="55.88" width="0.1016" layer="91"/>
<label x="127" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="53.34" x2="127" y2="53.34" width="0.1016" layer="91"/>
<label x="127" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="50.8" x2="127" y2="50.8" width="0.1016" layer="91"/>
<label x="127" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="48.26" x2="127" y2="48.26" width="0.1016" layer="91"/>
<label x="127" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="43.18" x2="127" y2="43.18" width="0.1016" layer="91"/>
<label x="127" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="40.64" x2="127" y2="40.64" width="0.1016" layer="91"/>
<label x="127" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="38.1" x2="127" y2="38.1" width="0.1016" layer="91"/>
<label x="127" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="35.56" x2="127" y2="35.56" width="0.1016" layer="91"/>
<label x="127" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="33.02" x2="127" y2="33.02" width="0.1016" layer="91"/>
<label x="127" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="30.48" x2="127" y2="30.48" width="0.1016" layer="91"/>
<label x="127" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="96.52" x2="152.4" y2="96.52" width="0.1016" layer="91"/>
<label x="152.4" y="96.52" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.117" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="93.98" x2="152.4" y2="93.98" width="0.1016" layer="91"/>
<label x="152.4" y="93.98" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.119" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="91.44" x2="152.4" y2="91.44" width="0.1016" layer="91"/>
<label x="152.4" y="91.44" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="88.9" x2="152.4" y2="88.9" width="0.1016" layer="91"/>
<label x="152.4" y="88.9" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.125" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="86.36" x2="152.4" y2="86.36" width="0.1016" layer="91"/>
<label x="152.4" y="86.36" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="83.82" x2="152.4" y2="83.82" width="0.1016" layer="91"/>
<label x="152.4" y="83.82" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.116" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="78.74" x2="152.4" y2="78.74" width="0.1016" layer="91"/>
<label x="152.4" y="78.74" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="76.2" x2="152.4" y2="76.2" width="0.1016" layer="91"/>
<label x="152.4" y="76.2" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="73.66" x2="152.4" y2="73.66" width="0.1016" layer="91"/>
<label x="152.4" y="73.66" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="71.12" x2="152.4" y2="71.12" width="0.1016" layer="91"/>
<label x="152.4" y="71.12" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="68.58" x2="152.4" y2="68.58" width="0.1016" layer="91"/>
<label x="152.4" y="68.58" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.116" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="66.04" x2="152.4" y2="66.04" width="0.1016" layer="91"/>
<label x="152.4" y="66.04" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="60.96" x2="152.4" y2="60.96" width="0.1016" layer="91"/>
<label x="152.4" y="60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="58.42" x2="152.4" y2="58.42" width="0.1016" layer="91"/>
<label x="152.4" y="58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="55.88" x2="152.4" y2="55.88" width="0.1016" layer="91"/>
<label x="152.4" y="55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.13.124" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="53.34" x2="152.4" y2="53.34" width="0.1016" layer="91"/>
<label x="152.4" y="53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="50.8" x2="152.4" y2="50.8" width="0.1016" layer="91"/>
<label x="152.4" y="50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="48.26" x2="152.4" y2="48.26" width="0.1016" layer="91"/>
<label x="152.4" y="48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.117" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="43.18" x2="152.4" y2="43.18" width="0.1016" layer="91"/>
<label x="152.4" y="43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="40.64" x2="152.4" y2="40.64" width="0.1016" layer="91"/>
<label x="152.4" y="40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.116" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="38.1" x2="152.4" y2="38.1" width="0.1016" layer="91"/>
<label x="152.4" y="38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.15.121" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="35.56" x2="152.4" y2="35.56" width="0.1016" layer="91"/>
<label x="152.4" y="35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.126" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="33.02" x2="152.4" y2="33.02" width="0.1016" layer="91"/>
<label x="152.4" y="33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.115" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="30.48" x2="152.4" y2="30.48" width="0.1016" layer="91"/>
<label x="152.4" y="30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.08.114" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="99.06" x2="127" y2="99.06" width="0.1016" layer="91"/>
<label x="127" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.09.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="99.06" x2="152.4" y2="99.06" width="0.1016" layer="91"/>
<label x="152.4" y="99.06" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.10.117" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="81.28" x2="127" y2="81.28" width="0.1016" layer="91"/>
<label x="127" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.11.127" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="81.28" x2="152.4" y2="81.28" width="0.1016" layer="91"/>
<label x="152.4" y="81.28" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.123" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="63.5" x2="127" y2="63.5" width="0.1016" layer="91"/>
<label x="127" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.12.122" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="63.5" x2="152.4" y2="63.5" width="0.1016" layer="91"/>
<label x="152.4" y="63.5" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.117" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="45.72" x2="127" y2="45.72" width="0.1016" layer="91"/>
<label x="127" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.14.116" class="0">
<segment>
<pinref part="U$15" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="45.72" x2="152.4" y2="45.72" width="0.1016" layer="91"/>
<label x="152.4" y="45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B3"/>
<wire x1="40.64" y1="5.08" x2="50.8" y2="5.08" width="0.1016" layer="91"/>
<label x="50.8" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B4"/>
<wire x1="40.64" y1="2.54" x2="50.8" y2="2.54" width="0.1016" layer="91"/>
<label x="50.8" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B5"/>
<wire x1="40.64" y1="0" x2="50.8" y2="0" width="0.1016" layer="91"/>
<label x="50.8" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B6"/>
<wire x1="40.64" y1="-2.54" x2="50.8" y2="-2.54" width="0.1016" layer="91"/>
<label x="50.8" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B7"/>
<wire x1="40.64" y1="-5.08" x2="50.8" y2="-5.08" width="0.1016" layer="91"/>
<label x="50.8" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B8"/>
<wire x1="40.64" y1="-7.62" x2="50.8" y2="-7.62" width="0.1016" layer="91"/>
<label x="50.8" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B10"/>
<wire x1="40.64" y1="-12.7" x2="50.8" y2="-12.7" width="0.1016" layer="91"/>
<label x="50.8" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B11"/>
<wire x1="40.64" y1="-15.24" x2="50.8" y2="-15.24" width="0.1016" layer="91"/>
<label x="50.8" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B12"/>
<wire x1="40.64" y1="-17.78" x2="50.8" y2="-17.78" width="0.1016" layer="91"/>
<label x="50.8" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B13"/>
<wire x1="40.64" y1="-20.32" x2="50.8" y2="-20.32" width="0.1016" layer="91"/>
<label x="50.8" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B14"/>
<wire x1="40.64" y1="-22.86" x2="50.8" y2="-22.86" width="0.1016" layer="91"/>
<label x="50.8" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B15"/>
<wire x1="40.64" y1="-25.4" x2="50.8" y2="-25.4" width="0.1016" layer="91"/>
<label x="50.8" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B17"/>
<wire x1="40.64" y1="-30.48" x2="50.8" y2="-30.48" width="0.1016" layer="91"/>
<label x="50.8" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B18"/>
<wire x1="40.64" y1="-33.02" x2="50.8" y2="-33.02" width="0.1016" layer="91"/>
<label x="50.8" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B19"/>
<wire x1="40.64" y1="-35.56" x2="50.8" y2="-35.56" width="0.1016" layer="91"/>
<label x="50.8" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B20"/>
<wire x1="40.64" y1="-38.1" x2="50.8" y2="-38.1" width="0.1016" layer="91"/>
<label x="50.8" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B21"/>
<wire x1="40.64" y1="-40.64" x2="50.8" y2="-40.64" width="0.1016" layer="91"/>
<label x="50.8" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B22"/>
<wire x1="40.64" y1="-43.18" x2="50.8" y2="-43.18" width="0.1016" layer="91"/>
<label x="50.8" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B24"/>
<wire x1="40.64" y1="-48.26" x2="50.8" y2="-48.26" width="0.1016" layer="91"/>
<label x="50.8" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B25"/>
<wire x1="40.64" y1="-50.8" x2="50.8" y2="-50.8" width="0.1016" layer="91"/>
<label x="50.8" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B26"/>
<wire x1="40.64" y1="-53.34" x2="50.8" y2="-53.34" width="0.1016" layer="91"/>
<label x="50.8" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B27"/>
<wire x1="40.64" y1="-55.88" x2="50.8" y2="-55.88" width="0.1016" layer="91"/>
<label x="50.8" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B28"/>
<wire x1="40.64" y1="-58.42" x2="50.8" y2="-58.42" width="0.1016" layer="91"/>
<label x="50.8" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="B29"/>
<wire x1="40.64" y1="-60.96" x2="50.8" y2="-60.96" width="0.1016" layer="91"/>
<label x="50.8" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C3"/>
<wire x1="66.04" y1="5.08" x2="76.2" y2="5.08" width="0.1016" layer="91"/>
<label x="76.2" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C4"/>
<wire x1="66.04" y1="2.54" x2="76.2" y2="2.54" width="0.1016" layer="91"/>
<label x="76.2" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C5"/>
<wire x1="66.04" y1="0" x2="76.2" y2="0" width="0.1016" layer="91"/>
<label x="76.2" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C6"/>
<wire x1="66.04" y1="-2.54" x2="76.2" y2="-2.54" width="0.1016" layer="91"/>
<label x="76.2" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C7"/>
<wire x1="66.04" y1="-5.08" x2="76.2" y2="-5.08" width="0.1016" layer="91"/>
<label x="76.2" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C8"/>
<wire x1="66.04" y1="-7.62" x2="76.2" y2="-7.62" width="0.1016" layer="91"/>
<label x="76.2" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.112" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C10"/>
<wire x1="66.04" y1="-12.7" x2="76.2" y2="-12.7" width="0.1016" layer="91"/>
<label x="76.2" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C11"/>
<wire x1="66.04" y1="-15.24" x2="76.2" y2="-15.24" width="0.1016" layer="91"/>
<label x="76.2" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C12"/>
<wire x1="66.04" y1="-17.78" x2="76.2" y2="-17.78" width="0.1016" layer="91"/>
<label x="76.2" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C13"/>
<wire x1="66.04" y1="-20.32" x2="76.2" y2="-20.32" width="0.1016" layer="91"/>
<label x="76.2" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C14"/>
<wire x1="66.04" y1="-22.86" x2="76.2" y2="-22.86" width="0.1016" layer="91"/>
<label x="76.2" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C15"/>
<wire x1="66.04" y1="-25.4" x2="76.2" y2="-25.4" width="0.1016" layer="91"/>
<label x="76.2" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C17"/>
<wire x1="66.04" y1="-30.48" x2="76.2" y2="-30.48" width="0.1016" layer="91"/>
<label x="76.2" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C18"/>
<wire x1="66.04" y1="-33.02" x2="76.2" y2="-33.02" width="0.1016" layer="91"/>
<label x="76.2" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C19"/>
<wire x1="66.04" y1="-35.56" x2="76.2" y2="-35.56" width="0.1016" layer="91"/>
<label x="76.2" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C20"/>
<wire x1="66.04" y1="-38.1" x2="76.2" y2="-38.1" width="0.1016" layer="91"/>
<label x="76.2" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C21"/>
<wire x1="66.04" y1="-40.64" x2="76.2" y2="-40.64" width="0.1016" layer="91"/>
<label x="76.2" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C22"/>
<wire x1="66.04" y1="-43.18" x2="76.2" y2="-43.18" width="0.1016" layer="91"/>
<label x="76.2" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.123" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C24"/>
<wire x1="66.04" y1="-48.26" x2="76.2" y2="-48.26" width="0.1016" layer="91"/>
<label x="76.2" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C25"/>
<wire x1="66.04" y1="-50.8" x2="76.2" y2="-50.8" width="0.1016" layer="91"/>
<label x="76.2" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C26"/>
<wire x1="66.04" y1="-53.34" x2="76.2" y2="-53.34" width="0.1016" layer="91"/>
<label x="76.2" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C27"/>
<wire x1="66.04" y1="-55.88" x2="76.2" y2="-55.88" width="0.1016" layer="91"/>
<label x="76.2" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C28"/>
<wire x1="66.04" y1="-58.42" x2="76.2" y2="-58.42" width="0.1016" layer="91"/>
<label x="76.2" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="C29"/>
<wire x1="66.04" y1="-60.96" x2="76.2" y2="-60.96" width="0.1016" layer="91"/>
<label x="76.2" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D3"/>
<wire x1="91.44" y1="5.08" x2="101.6" y2="5.08" width="0.1016" layer="91"/>
<label x="101.6" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D4"/>
<wire x1="91.44" y1="2.54" x2="101.6" y2="2.54" width="0.1016" layer="91"/>
<label x="101.6" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D5"/>
<wire x1="91.44" y1="0" x2="101.6" y2="0" width="0.1016" layer="91"/>
<label x="101.6" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D6"/>
<wire x1="91.44" y1="-2.54" x2="101.6" y2="-2.54" width="0.1016" layer="91"/>
<label x="101.6" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D7"/>
<wire x1="91.44" y1="-5.08" x2="101.6" y2="-5.08" width="0.1016" layer="91"/>
<label x="101.6" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D8"/>
<wire x1="91.44" y1="-7.62" x2="101.6" y2="-7.62" width="0.1016" layer="91"/>
<label x="101.6" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D10"/>
<wire x1="91.44" y1="-12.7" x2="101.6" y2="-12.7" width="0.1016" layer="91"/>
<label x="101.6" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D11"/>
<wire x1="91.44" y1="-15.24" x2="101.6" y2="-15.24" width="0.1016" layer="91"/>
<label x="101.6" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.118" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D12"/>
<wire x1="91.44" y1="-17.78" x2="101.6" y2="-17.78" width="0.1016" layer="91"/>
<label x="101.6" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D13"/>
<wire x1="91.44" y1="-20.32" x2="101.6" y2="-20.32" width="0.1016" layer="91"/>
<label x="101.6" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D14"/>
<wire x1="91.44" y1="-22.86" x2="101.6" y2="-22.86" width="0.1016" layer="91"/>
<label x="101.6" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D15"/>
<wire x1="91.44" y1="-25.4" x2="101.6" y2="-25.4" width="0.1016" layer="91"/>
<label x="101.6" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D17"/>
<wire x1="91.44" y1="-30.48" x2="101.6" y2="-30.48" width="0.1016" layer="91"/>
<label x="101.6" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D18"/>
<wire x1="91.44" y1="-33.02" x2="101.6" y2="-33.02" width="0.1016" layer="91"/>
<label x="101.6" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D19"/>
<wire x1="91.44" y1="-35.56" x2="101.6" y2="-35.56" width="0.1016" layer="91"/>
<label x="101.6" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D20"/>
<wire x1="91.44" y1="-38.1" x2="101.6" y2="-38.1" width="0.1016" layer="91"/>
<label x="101.6" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D21"/>
<wire x1="91.44" y1="-40.64" x2="101.6" y2="-40.64" width="0.1016" layer="91"/>
<label x="101.6" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D22"/>
<wire x1="91.44" y1="-43.18" x2="101.6" y2="-43.18" width="0.1016" layer="91"/>
<label x="101.6" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D24"/>
<wire x1="91.44" y1="-48.26" x2="101.6" y2="-48.26" width="0.1016" layer="91"/>
<label x="101.6" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D25"/>
<wire x1="91.44" y1="-50.8" x2="101.6" y2="-50.8" width="0.1016" layer="91"/>
<label x="101.6" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D26"/>
<wire x1="91.44" y1="-53.34" x2="101.6" y2="-53.34" width="0.1016" layer="91"/>
<label x="101.6" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D27"/>
<wire x1="91.44" y1="-55.88" x2="101.6" y2="-55.88" width="0.1016" layer="91"/>
<label x="101.6" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D28"/>
<wire x1="91.44" y1="-58.42" x2="101.6" y2="-58.42" width="0.1016" layer="91"/>
<label x="101.6" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="D29"/>
<wire x1="91.44" y1="-60.96" x2="101.6" y2="-60.96" width="0.1016" layer="91"/>
<label x="101.6" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E3"/>
<wire x1="116.84" y1="5.08" x2="127" y2="5.08" width="0.1016" layer="91"/>
<label x="127" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E4"/>
<wire x1="116.84" y1="2.54" x2="127" y2="2.54" width="0.1016" layer="91"/>
<label x="127" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E5"/>
<wire x1="116.84" y1="0" x2="127" y2="0" width="0.1016" layer="91"/>
<label x="127" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E6"/>
<wire x1="116.84" y1="-2.54" x2="127" y2="-2.54" width="0.1016" layer="91"/>
<label x="127" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E7"/>
<wire x1="116.84" y1="-5.08" x2="127" y2="-5.08" width="0.1016" layer="91"/>
<label x="127" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E8"/>
<wire x1="116.84" y1="-7.62" x2="127" y2="-7.62" width="0.1016" layer="91"/>
<label x="127" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.113" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E10"/>
<wire x1="116.84" y1="-12.7" x2="127" y2="-12.7" width="0.1016" layer="91"/>
<label x="127" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E11"/>
<wire x1="116.84" y1="-15.24" x2="127" y2="-15.24" width="0.1016" layer="91"/>
<label x="127" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E12"/>
<wire x1="116.84" y1="-17.78" x2="127" y2="-17.78" width="0.1016" layer="91"/>
<label x="127" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E13"/>
<wire x1="116.84" y1="-20.32" x2="127" y2="-20.32" width="0.1016" layer="91"/>
<label x="127" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.124" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E14"/>
<wire x1="116.84" y1="-22.86" x2="127" y2="-22.86" width="0.1016" layer="91"/>
<label x="127" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E15"/>
<wire x1="116.84" y1="-25.4" x2="127" y2="-25.4" width="0.1016" layer="91"/>
<label x="127" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E17"/>
<wire x1="116.84" y1="-30.48" x2="127" y2="-30.48" width="0.1016" layer="91"/>
<label x="127" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E18"/>
<wire x1="116.84" y1="-33.02" x2="127" y2="-33.02" width="0.1016" layer="91"/>
<label x="127" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E19"/>
<wire x1="116.84" y1="-35.56" x2="127" y2="-35.56" width="0.1016" layer="91"/>
<label x="127" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E20"/>
<wire x1="116.84" y1="-38.1" x2="127" y2="-38.1" width="0.1016" layer="91"/>
<label x="127" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E21"/>
<wire x1="116.84" y1="-40.64" x2="127" y2="-40.64" width="0.1016" layer="91"/>
<label x="127" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E22"/>
<wire x1="116.84" y1="-43.18" x2="127" y2="-43.18" width="0.1016" layer="91"/>
<label x="127" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E24"/>
<wire x1="116.84" y1="-48.26" x2="127" y2="-48.26" width="0.1016" layer="91"/>
<label x="127" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E25"/>
<wire x1="116.84" y1="-50.8" x2="127" y2="-50.8" width="0.1016" layer="91"/>
<label x="127" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E26"/>
<wire x1="116.84" y1="-53.34" x2="127" y2="-53.34" width="0.1016" layer="91"/>
<label x="127" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E27"/>
<wire x1="116.84" y1="-55.88" x2="127" y2="-55.88" width="0.1016" layer="91"/>
<label x="127" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E28"/>
<wire x1="116.84" y1="-58.42" x2="127" y2="-58.42" width="0.1016" layer="91"/>
<label x="127" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E29"/>
<wire x1="116.84" y1="-60.96" x2="127" y2="-60.96" width="0.1016" layer="91"/>
<label x="127" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F3"/>
<wire x1="142.24" y1="5.08" x2="152.4" y2="5.08" width="0.1016" layer="91"/>
<label x="152.4" y="5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F4"/>
<wire x1="142.24" y1="2.54" x2="152.4" y2="2.54" width="0.1016" layer="91"/>
<label x="152.4" y="2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F5"/>
<wire x1="142.24" y1="0" x2="152.4" y2="0" width="0.1016" layer="91"/>
<label x="152.4" y="0" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F6"/>
<wire x1="142.24" y1="-2.54" x2="152.4" y2="-2.54" width="0.1016" layer="91"/>
<label x="152.4" y="-2.54" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F7"/>
<wire x1="142.24" y1="-5.08" x2="152.4" y2="-5.08" width="0.1016" layer="91"/>
<label x="152.4" y="-5.08" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F8"/>
<wire x1="142.24" y1="-7.62" x2="152.4" y2="-7.62" width="0.1016" layer="91"/>
<label x="152.4" y="-7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F10"/>
<wire x1="142.24" y1="-12.7" x2="152.4" y2="-12.7" width="0.1016" layer="91"/>
<label x="152.4" y="-12.7" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F11"/>
<wire x1="142.24" y1="-15.24" x2="152.4" y2="-15.24" width="0.1016" layer="91"/>
<label x="152.4" y="-15.24" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.119" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F12"/>
<wire x1="142.24" y1="-17.78" x2="152.4" y2="-17.78" width="0.1016" layer="91"/>
<label x="152.4" y="-17.78" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F13"/>
<wire x1="142.24" y1="-20.32" x2="152.4" y2="-20.32" width="0.1016" layer="91"/>
<label x="152.4" y="-20.32" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.125" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F14"/>
<wire x1="142.24" y1="-22.86" x2="152.4" y2="-22.86" width="0.1016" layer="91"/>
<label x="152.4" y="-22.86" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F15"/>
<wire x1="142.24" y1="-25.4" x2="152.4" y2="-25.4" width="0.1016" layer="91"/>
<label x="152.4" y="-25.4" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F17"/>
<wire x1="142.24" y1="-30.48" x2="152.4" y2="-30.48" width="0.1016" layer="91"/>
<label x="152.4" y="-30.48" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F18"/>
<wire x1="142.24" y1="-33.02" x2="152.4" y2="-33.02" width="0.1016" layer="91"/>
<label x="152.4" y="-33.02" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F19"/>
<wire x1="142.24" y1="-35.56" x2="152.4" y2="-35.56" width="0.1016" layer="91"/>
<label x="152.4" y="-35.56" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F20"/>
<wire x1="142.24" y1="-38.1" x2="152.4" y2="-38.1" width="0.1016" layer="91"/>
<label x="152.4" y="-38.1" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.121" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F21"/>
<wire x1="142.24" y1="-40.64" x2="152.4" y2="-40.64" width="0.1016" layer="91"/>
<label x="152.4" y="-40.64" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F22"/>
<wire x1="142.24" y1="-43.18" x2="152.4" y2="-43.18" width="0.1016" layer="91"/>
<label x="152.4" y="-43.18" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F24"/>
<wire x1="142.24" y1="-48.26" x2="152.4" y2="-48.26" width="0.1016" layer="91"/>
<label x="152.4" y="-48.26" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F25"/>
<wire x1="142.24" y1="-50.8" x2="152.4" y2="-50.8" width="0.1016" layer="91"/>
<label x="152.4" y="-50.8" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F26"/>
<wire x1="142.24" y1="-53.34" x2="152.4" y2="-53.34" width="0.1016" layer="91"/>
<label x="152.4" y="-53.34" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.07.120" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F27"/>
<wire x1="142.24" y1="-55.88" x2="152.4" y2="-55.88" width="0.1016" layer="91"/>
<label x="152.4" y="-55.88" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.126" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F28"/>
<wire x1="142.24" y1="-58.42" x2="152.4" y2="-58.42" width="0.1016" layer="91"/>
<label x="152.4" y="-58.42" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.115" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F29"/>
<wire x1="142.24" y1="-60.96" x2="152.4" y2="-60.96" width="0.1016" layer="91"/>
<label x="152.4" y="-60.96" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.01.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E2"/>
<wire x1="116.84" y1="7.62" x2="127" y2="7.62" width="0.1016" layer="91"/>
<label x="127" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.00.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F2"/>
<wire x1="142.24" y1="7.62" x2="152.4" y2="7.62" width="0.1016" layer="91"/>
<label x="152.4" y="7.62" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.02.114" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E9"/>
<wire x1="116.84" y1="-10.16" x2="127" y2="-10.16" width="0.1016" layer="91"/>
<label x="127" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.03.122" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F9"/>
<wire x1="142.24" y1="-10.16" x2="152.4" y2="-10.16" width="0.1016" layer="91"/>
<label x="152.4" y="-10.16" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.04.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E16"/>
<wire x1="116.84" y1="-27.94" x2="127" y2="-27.94" width="0.1016" layer="91"/>
<label x="127" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.05.127" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F16"/>
<wire x1="142.24" y1="-27.94" x2="152.4" y2="-27.94" width="0.1016" layer="91"/>
<label x="152.4" y="-27.94" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.117" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="E23"/>
<wire x1="116.84" y1="-45.72" x2="127" y2="-45.72" width="0.1016" layer="91"/>
<label x="127" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
<net name="ZZ.06.116" class="0">
<segment>
<pinref part="U$16" gate="G$1" pin="F23"/>
<wire x1="142.24" y1="-45.72" x2="152.4" y2="-45.72" width="0.1016" layer="91"/>
<label x="152.4" y="-45.72" size="1.016" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
