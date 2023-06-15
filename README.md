# **FPS - OpenGL**

    CTMM Group - Game Programming 1st year
    Chiclet Guillaume - Tadjer Rayane - Maréchal Lancelot - Mouchel Noé  

<br>  

# **NOTE!**

This is an old project from our first year so it is quite dirty, but it is a good showcase of our first steps with OpenGl and GLSL as well as creating our physics engine.  

To launch the project from the solution in *sources* folder, the *.dll* from the *Dll* folder need to be copied into the build folder after first build. Assets and Resources folders that are located near the executable (in *bin* folder) should be copied in the sources folder alongside the .sln solution.

Otherwise, the built executable is provided for simpler use and that the best way to launch the program. Source files are mainly there to be reviewed.

An audio output is required to launch.

***This program should be compiled in 64 bits only (x64)***  
In Visual studio you need to compile in **RELEASE** mode, debug mode can freeze on huge scenes.

### **Inputs**

    Press 'W', 'S', 'A' and 'D' to move (or ZQSD in FR keyboard)
    Press 'Space' to jump
    Press 'Left Click' to shoot
    Press 'R' to reload
    Press 'E' to interact
    Press 'Esc' to open pause menu

# **Features**  

- Timer
- Particle system
- Sprite and Spritebillboard
- PostProcess - Bloom but disabled
- Physics: Colliders, rigidbody, raycast
- Dynamic Logs for debug
- Level editor

# **Bugs**
Player can get through colliders when they hit the corners of the hitboxes
