
![COSMO388 ROUGH](https://github.com/Paulappaul/COSMO388---Tape-Emulator/assets/38223440/0dad1c8b-67fd-4011-b359-b8f9115f3007)

Review the video demo here: https://youtu.be/jBR6MgT6xkM?si=OdtMVN2DJuPFwmmS

Cosmo 388 is the first DAW from Simulacra Sound. This fully functional workstation is designed to replicate the legendary Tascam 388 reel-to-reel tape machine. With its 8 channels for recording and playback, animated VU meters, and lifelike reel-to-reel movement, Cosmo 388 immerses you in the world of authentic analog recording.

Through advanced convolution techniques, this powerful DAW faithfully captures the warmth, saturation, and dynamic response of an analog tape machine. As you play, record, and mix your music, Cosmo 388 reacts dynamically to your input signal's amplitude, just like a genuine tape machine would.

Designed with simplicity in mind, Cosmo 388 is a perfect choice for musicians at any level. Whether you're a beginner, intimidated by the complexities of professional DAWs like Pro Tools, Logic, or Ableton, or a seasoned musician seeking a streamlined workflow, Cosmo 388 delivers a user-friendly interface that empowers you to quickly record and produce your music.

With two dedicated send dials for Reverb and Tape Saturation, Cosmo 388 offers effortless control over the depth and character of your sound. The immersive, out-of-the-box experience ensures that your recordings sound incredible from the moment you hit the play button.


This project is currently still in development, but is available for BETA testing. You'll need the FLTK and PortAudio binaries as well as AudioFile.H primarly for operation. 


Things still to do 7/13/2023:


*Adjust Digital Reverb Settings: Sounds more like an Echo currently. I'll need to redesign the algorithm most likely. 

*Implement Concurrency on Write Functions: Convoling and Writting to disk are expensive and can take a long time depending on the file size. I can fix this by spiliting up the recordSamples vector into many vectors and ordering different threads to convolve their own part of the recordSamples Vector. 

*Implement Load Bars (FillSliders?) or Load Dials: Fetch the recordSamples Size, this will be the total value of the fill slider or dial. As we process samples, we update the load bar to fill up until its completely full. When writting is complete we ping a message to the user and allow Recording and Playback to occur. 
