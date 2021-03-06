<Cabbage>
form size(712, 290), caption("File Player"), pluginID("plu1")
soundfiler bounds(16, 12, 685, 220), identchannel("filer1"),  colour("white"), channel("pos1","end1")
image bounds(18, 238, 35, 35), file("openfile.png")
filebutton bounds(18, 238, 40, 35), channel("filename"), text("", ""), colour(100, 0, 0, 0) 
image bounds(58, 238, 35, 35), file("play.png"), identchannel("playImage")
button bounds(58, 238, 40, 35), channel("startStop"), text("", ""), fontcolour:1("lime"), fontcolour:0("lime"), colour:0(0,0,0,0), colour:1(0,0,0,0)
</Cabbage>
<CsoundSynthesizer>
<CsOptions>  
-n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2
0dbfs = 1

;RW 2014
;based on code from Iain McCurdy's sound file players
gichans   init  0
giFileLen init  0
giReady   init  0
gSfilepath  init  ""
chn_S "filer1", 3

instr 1 
gSfilepath  chnget  "filename"
kNewFileTrg changed gSfilepath    ; if a new file is loaded generate a trigger
if kNewFileTrg==1 then        ; if a new file has been loaded...
event "i",99,0,0        ; call instrument to update sample storage function table 
endif  

gkPlayStop chnget "startStop"
if changed:k(gkPlayStop)==1 then
	if gkPlayStop==1 then
		chnset "file(\"pause.png\")", "playImage"
	else
		chnset "file(\"play.png\")", "playImage"	
	endif
endif
	
ktrig trigger gkPlayStop,0.5,0    ; if play button changes to 'play', generate a trigger
schedkwhen  ktrig,0,0,2,0,-1    ; start instr 2 playing a held note
endin


instr 99  ; load sound file
 gichans  filenchnls  gSfilepath      ; derive the number of channels (mono=1,stereo=2) in the sound file
 gitableL ftgen 1,0,0,1,gSfilepath,0,0,1
 giFileLen  filelen   gSfilepath      ; derive the file duration
 if gichans==2 then
  gitableR  ftgen 2,0,0,1,gSfilepath,0,0,2
 endif
 giReady  = 1         ; if no string has yet been loaded giReady will be zero
 prints gSfilepath

 Smessage sprintf "file(\"%s\")", gSfilepath      ;send file name to soundfiler widget on identchannel "filer1"
 chnset Smessage, "filer1"
  
endin

instr 2             ;playback instrument
if gkPlayStop==0 then
  turnoff
endif

kStart init 0
kStart chnget "pos1"
kLoopLength chnget "end1"
if(changed:k(kStart)==1 || changed:k(kLoopLength)==1) then
reinit rePosition
endif

rePosition:
iLenInSamples = giFileLen*sr
iStartPoint = i(kStart)/iLenInSamples
iRegion = (i(kLoopLength)==0 ? 1 : i(kLoopLength)/iLenInSamples)


prints "StartPos:%f", iStartPoint
aphasor phasor (1/(iLenInSamples/sr)/(iRegion))

if gichans==1 then            ; if mono...
  a1  table3  ((aphasor*iRegion)+iStartPoint)%1, gitableL, 1
  outs  a1, a1            ; send mono audio to both outputs 
elseif gichans==2 then          ; otherwise, if stereo...
  a1  table3  ((aphasor*iRegion)+iStartPoint)%1, gitableL, 1
  a2  table3  ((aphasor*iRegion)+iStartPoint)%1, gitableR, 1
  outs  a1, a2      ; send stereo signal to outputs

endif

if(metro(10)==1) then     ;send scrubber position to soundfiler widget
 kPos downsamp aphasor*iRegion
 Smessage sprintfk "scrubberposition(%d)", ((kPos*iLenInSamples)+kStart)%(iLenInSamples/iRegion)
 chnset Smessage, "filer1"
endif

endin

</CsInstruments>  
<CsScore>
f1 0 1024 10 1
i1 0 3600
</CsScore>
</CsoundSynthesizer>
</CsoundSynthesizer>