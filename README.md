# Ketone
A simple demo application(WIP) based on OptiX7_Utility 
https://github.com/shocker-0x15/OptiX7_Utility

### Requirements
<ul> 
  <li>CUDA 11.0 Update 1 </li>
  <li>OptiX 7.1.0 (requires Maxwell or later generation NVIDIA GPU) </li>
  <li>Windows 10 (1909) & Visual Studio 2019 (16.7.2)</li>
</ul>

### Installing
1. Go to the thirdparty folder and unzip the precompiled.zp file. Make sure to unzip to a folder called precompiled
2. Run the generateVS2019.bat file to create the third party solution
3. Go to the newly created thirdparty/builds folder and open the visual studio solution and compile all projects
4. Go back to the root folder and run the generateVS2019.bat file
5. Go to the newly created /builds folder and open the visual studio solution and compile all projects

### Features
<ul> 
  <li>Mouse picking </li>
  <li>Geometry instancing with multiple materials </li>
</ul>

### Usage
<ul> 
  <li>A few meshes ( mostly by William Vaughn  http://pushingpoints.com/v2/ ) can be added from the gui or you can drag and drop files or folders of gltf2.0 files into the window. Currently there is no material supprt.
 </li>
  <li>Use LMB for camera trackball and RMB for mesh picking</li>
  <li>To make a clump of instances, first load 1 object and select it by picking. Then press the MeshOps->Clump of Instances button</li>
</ul>


![screen grab 1](
https://github.com/Hurleyworks/Ketone/blob/master/resources/ActiveKetone/screen_grab_1.png)

![screen grab 2](
https://github.com/Hurleyworks/Ketone/blob/master/resources/ActiveKetone/screen_grab_2.png)
