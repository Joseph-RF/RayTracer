<a id="readme-top"></a>

<br />
<div align="center">

  <h3 align="center">RayTracer</h3>

  <p align="center">
    . 
    <br />
    <br />
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Raytracing program based on the [Ray Tracing in One Weekend](https://raytracing.github.io/) series which implements path tracing in order to create (what I believe to be) pretty images mimicking the behaviour of light in the real world.

The raytracer supports spheres, quads (quadrilateral) and triangles. Using the latter together with a .obj loading function, the following render was made:

<img src="/READMEImages/cornell_box_glass_cow.png" alt="Image of a cornell box with a glass cow sitting inside." width="750"/>

Here is a more standard version of the Cornell Box:

<img src="/READMEImages/cornell_box_glass_ball.png" alt="Image of a cornell box with a glass cow sitting inside." width="750"/>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

This project was built using SFML.

In order to run this code SFML must be installed first.

I would recommend the following quick tutorial by [EOD - Ethan](https://www.youtube.com/watch?v=lFzpkvrscs4) on YouTube. 

From here it should be straightforward. Just running the code in Visual Studio should produce the raytraced image (after some time).

* [![SFML][SFML-logo]][SFML-url]

I plan on using CMake for the build to allow for anyone with a C++ compiler and CMake to be able to use the project.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- FUTURE FEATURES -->
## Future Features

One of the things that drew me the most to the raytracer as a project was how simple it is at the beginning and yet the many ways it can be extended.

I am listing a few below that I believe would be fun to add.

* Depth of field for the camera
* Beer's Law for dielectric materials
* Expanding the .obj loader to account for colour. For now, the object is assumed to be of the same single material
* As mentioned above, using CMake for the build to allow for users to quickly and easily be able to use the program themselves. Need to find a way to make it run faster as using CMake for now is noticably slower thatn the current build method.
* A user friendly way to add hittables to the scene

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LIMITATIONS -->
## Limitations

I am using this section to list the limitations a user may encounter when using this project which may not be immediately obvious. Note that this section does not cover everything, just the things I have realised or encountered during testing.

* Dielectric within a dielectric won't behave accurately

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Project Link: [https://github.com/Joseph-RF/RayTracer](https://github.com/Joseph-RF/2DPhysics)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Find below a list of resources that were vital/helpful for the development of this project.

* [Ray Tracing in One Weekend Series](https://raytracing.github.io/)
* [SFML Documentation](https://www.sfml-dev.org/documentation/2.6.2/)
* [Computer Graphics from Scratch](https://www.gabrielgambetta.com/computer-graphics-from-scratch/)
* [How to build a BVH Article](https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/)
* [Physically Based Rendering](https://pbr-book.org/)
* [The blog at the bottom of the sea - Raytracing Reflection, Refraction, Fresnel, Total Internal Reflection, and Beer's Law](https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/)
* [Badges for README](https://github.com/alexandresanlim/Badges4-README.md-Profile)
* [README Template](https://github.com/othneildrew/Best-README-Template)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[SFML-url]: https://www.sfml-dev.org/
[SFML-logo]: https://img.shields.io/badge/SFML-8CC445?style=for-the-badge&logo=sfml&logoColor=white