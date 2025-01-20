# scop
SCOP is a 42 project where you create a simple 3D model viewer from scratch. The goal is to load .obj files, apply transformations like scaling, rotation, and translation, and render the result using OpenGL. It’s an introduction to graphics programming, focusing on shaders, matrices, and basic rendering techniques.

# How to use
#### compile with: 
- make
#### run the project with:
- make run
- make run FILE=[filepath]
- ./build/gl [filepath]

# controls
````
|       wasd | camera movement
|      mouse | camera direction
|left, right | change 3D model
|   up, down | change color palette
|          f | show grid & axis
|          c | camera orbit
|          t | apply texture
|          p | pause rotation
|     scroll | change camera speed
|ctrl+scroll | change fov
````
