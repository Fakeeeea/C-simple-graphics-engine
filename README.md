# C-simple-graphics-engine

This is a project I have had for a long time, finally had the time to put my hands into it, though could not get to the point I initially wanted to get to.

# UI

<ul>
  <li>
    <b>light</b>: whenever or not the graphics engine should shade the triangles
  </li>
  <li>
    <b>borders:</b> whenever or not the graphics engine should draw black borders around the triangles
  </li>
  <li>
    <b>xray:</b> disables normals check, can see mesh that is normally hidden
  </li>
  <li>
    <b>screenxy:</b> draws a x and y axis relative to the SCREEN coordinates
  </li>
  <li>
    <b>x,y,z rot:</b> enables rotation on the selected axex
  </li>
  <li>
    <b>model:</b> filename of the model to render (include extension)
  </li>
</ul>

# Controls

<ul>
  <li>
    <b>W</b>: forward;
  </li>
  <li>
    <b>S</b>: backwards;
  </li>
  <li>
    <b>SPACE</b>: up;
  </li>
  <li>
    <b>SHIFT</b>: down;
  </li>
  <li>
    <b>Arrow right</b>: rotation += PI/32
  </li>
  <li>
    <b>Arrow left</b>: rotation -= PI/32
  </li>
</ul>

# File structure

<b>Folders:</b><br>
<ul>
  <li>
    models: where models should be placed
  </li>
  <li>
    src: contains source code of the engine
  </li>
</ul>

<b>Source files:</b><br>
<ul>
  <li>
    types.c & types.h: types,constants used inside of the graphics engine
  </li>
  <li>
    windows_math.c & windows_math.h: functions related to windows.h
  </li>
  <li>
    model.c & model.h: functions related to parsing the obj files
  </li>
  <li>
    tridimensional_math.c & tridimensional_math.h: math vec3 & mat4 functions needed for the graphics engine
  </li>
</ul>

# Quick notes

The graphics engine does NOT use a look-at matrix for camera movement. The rotations are applied to the whole world<br>
A simple painters algorithm is used to draw the triangles, not z-depth buffer for simplicty<br>
Any constructive criticism is appreticiated<br>
