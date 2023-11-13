# SDF_Erosion
Travail scolaire en C++ sur les Signed Distance Field, et le maillage de SDF via l'algorithme de Marching Cube, manipulation de technique d'érosion
Travail scolaire en C++ sur le maillage de surface de Bézier, surfaces de révolutions, et la déformation globale ou locale de maillages.

### Voir les deux PDF pour visualiser des images de mon travail
### Pour compiler le projet, utilisez Qt et selon votre OS : 
## Windows
- First, you must install Qt6 (https://www.qt.io/download, use the open source version). Install the msvc2019_64 compiler only (*Note: other compilers can work too*).
- Set the "QTDIR64" environment variable to your Qt installation folder (example: "E:\Code\Qt\6.3.0\msvc2019_64")
- Set the "GLEW_DIR" environment variable to the path "XXX/TinyMesh/Libs/", with XXX the path to your repository.
- Run the script TinyMesh/Script/AppTinyMeshDeploy.bat

Finally, open the Visual Studio solution files with Visual Studio 2022 Community, or open the QtCreatorProject.pro with QtCreator. Compile/Build and execute, everything should work.
*Note: For other IDE, you will have to use the provided CMakeLists.txt to generate the solution files yourself.*

## Linux
- First, you must install Qt6 (https://www.qt.io/download, use the open source version). Install the gcc compiler only (*Note: other compilers can work too*).
- Install the following packages
```
sudo apt-get install g++ libglew-dev freeglut3-dev
```
Finally, open the QtCreatorProject.pro with QtCreator. Click on configure, then build and execute, everything should work.
*Note: For other IDE, you will have to use the provided CMakeLists.txt to generate the solution files yourself.*

## Mac OS/X
- First, you must install Qt6 (https://www.qt.io/download, use the open source version). Install the gcc compiler only (*Note: other compilers can work too*).
-  Install the following packages (*Note: Depending on your Mac OS/X version, other packages might be needed. Shoot me an email if anything is missing.*
```
brew install glew
```
Finally, open the QtCreatorProject.pro with QtCreator. Click on configure, then build and execute, everything should work.
*Note: For other IDE, you will have to use the provided CMakeLists.txt to generate the solution files yourself.*
