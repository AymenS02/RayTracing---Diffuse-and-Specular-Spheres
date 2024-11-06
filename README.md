# Ray Tracing Project

This project implements a ray tracing algorithm in C++ to render 3D scenes with realistic lighting and material interactions. It simulates the way light rays interact with objects, specifically spheres, to produce high-quality images of scenes with diffuse and specular materials.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Rendering Scenes](#rendering-scenes)
- [Sample Output](#sample-output)
- [Credits](#credits)

## Project Overview
This project was developed using the provided `main.cpp` and several header files: `Vector3D.h`, `Camera.h`, `Ray.h`, `World.h`, `Sphere.h`, and `Material.h`. The goal was to complete missing pieces of the ray tracing algorithm by implementing specific functions for handling ray-object interactions, material properties, and rendering scenes with varying material properties.

## Features
- **Ray-Sphere Intersection**: Determines if a ray hits a sphere within a certain range and calculates the intersection details.
- **World Collision Detection**: Manages multiple spheres in a scene and identifies the closest intersection for each ray.
- **Diffuse Reflection**: Simulates scattering of rays in random directions for diffuse materials.
- **Specular Reflection**: Generates mirrored rays for specular materials to simulate reflective surfaces.
- **Scene Rendering**: Renders and outputs scenes with different combinations of diffuse and specular materials to `.ppm` image files.

## Installation
1. Clone this repository to your local machine:
    ```bash
    git clone https://github.com/yourusername/ray-tracing-project.git
    ```
2. Open the project in your C++ IDE or set up a new C++ project.
3. Include all provided header files and `main.cpp` in your project.
4. Build the project to ensure there are no initial errors.

## Usage
1. **Set Output Path**: Update the output path for rendered images in the `main` function in `main.cpp`.
2. **Run the Program**: Execute the project. The ray tracer will render images of various scenes as configured in `main.cpp`.
3. **Save Outputs**: Uncomment the predefined worlds in `main.cpp` to render different scenes and save the results as `.ppm` files.

### Example Command (if using CLI):
```bash
./ray-tracer
