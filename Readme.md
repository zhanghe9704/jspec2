# Jlab Simulation Package for Electron Cooling

## About JSPEC 2

JSPEC is an open source C++ package for numerical simulations on the electron cooling process, including the intrabeam scattering (IBS) effect, developed at [Jefferson Lab (JLab)](http://www.jlab.org). 

This is the second version of JSPEC. The repository of the first version can be found [here](https://github.com/zhanghe9704/electroncooling).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## How to compile

You need cmake and a C++ compiler that supports C++11 standard. GCC is recommended,  which is used by the author to compile the codes. JSPEC 2 depends on two libraries: the GSL library (a scientific computation library) and the muParser library (a math parser). The muParser library is included in this repository as a submodule. After you downloaded or cloned this repository, go to the root folder and run the following command to obtain the muParser source files:

```shell
 git submodule update --init
```

### Compile in Linux (Ubuntu)

In the following, we will discuss how to compile JSPEC 2 in Linux, using Ubuntu 20.04 as an example. 

1. If you do not have the compiling tools, use the following commands to install them. 
   
   ```shell
   sudo apt install build-essential
   sudo apt install cmake
   ```

2. Install GSL using the following command
   
   ```shell
   sudo apt install libgsl-dev
   ```

3. Now we are ready to compile JSPEC 2. Go into the root folder, jspec2, and make a folder called `build`.  Go into the folder `build` and run cmake there. 
   
   ```shell
   mkdir build
   cd build
   cmake ..
   ```
   
   A Makefile should be created. Then use
   
   ```shell
   make install
   ```
   
    to compile and install JSPEC2. The default installation directory is your home directory. The executable file will be installed in the `bin` folder under the home directory. Some other files will be installed to the `lib` and `include` folders under the home directory. Please check the output in the terminal. 

   If you would like to change the installation directory, use the following command:

```shell
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/directory ..
make install
```

   To compile the tests, use the following command:

```
cmake -DBUILD_TESTS=ON ..
make
```

   The test files in the `tests` folder will be compiled. A copy of the executable file `jspec2` will be copied to the `tests` folder, which is needed to run the tests. 

### Compile in Windows

The most convenient way to compile JSPEC 2 in Widows is to compile it in **WSL2** following the above instruction for Linux. But if you want to create an executive file that runs in Windows, please consider the following steps. 

1. Obtain the GCC compiler. I use  [*mingw-builds*](https://github.com/niXman/mingw-builds-binaries). Just download the respective binaries for your system and expand it. 

2. Download and install [*cmake*](https://cmake.org/download/).

3. Install the GSL lib. One possible way is to use **vcpkg**. First, download the vcpkg repo from github. 
   
   ```shell
   git clone https://github.com/microsoft/vcpkg.git
   ```
   
   Then we go into the vcpkg folder, set up vcpkg,  and install GSL. 
   
   ```shell
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   .\vcpkg install gsl gsl:x64-windows
   ```

4. Now we are ready to compile JSPEC2. Go to the root folder and run
   
   ```shell
   mkdir build
   cd build
   cmake -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -G "MinGW Makefiles" ..
   ```
   
   A Makefile for MinGW GCC will be generated.  The following command will compile the source files. A folder called `jspec2` will be created. The executable file `jsepc.exe` and the three dynamic libraries, `gsl.dll`, `gslcblas.dll`, and `libmuparser.dll` will be copied to the folder. 
   
   ```shell
   mingw32-make  install
   ```
   
   To compile the tests, run the following commands:
   
   ```shell
   cmake -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -G "MinGW Makefiles" -DBUILD_TESTS=ON ..
   
   mingw32-make
   ```

## How to run

To run JSPEC, you can put your input file in the same folder with the JSPEC executable file and run the following commands in the folder:

> ` jspec.exe inputfilename` 

You also need another file in [*MAD X*](https://madx.web.cern.ch/madx/) tfs format, which defines the ion ring optics. You can put it in the same folder too. About how to write your input file, please refer to the JSPEC User Manual. 

Besides running the program in the command line as aforementioned, a windows batch file "jspec-dragfile.bat" is also provided. Putting this bat file together with the executable jspec file "jspec.exe", the math parser dynamic library file "muparser.dll", the tfs format lattice file and the input script file in the same folder, one can drag the  input script file onto the batch file to run the computation/simulation. 

If you are running the parallel version of JSPEC, in your script file you can set the number of thread in "section_run" using the command: 

```
set_n_thread your_thread_number
```

If you do not set the thread number, OPENMP will use all the available threads. 

## Acknowledgement

Authors of [**BETACOOL**](http://betacool.jinr.ru/), we learned a lot from BETACOOL. 

Authors of [**muParser**](http://beltoforion.de/article.php?a=muparser),  which is used in building the text-based UI. 

Dr. David Bruwihler, Dr. Paul Moeller and Dr. Stephen Coleman at [*Radiasoft*](http://radiasoft.net/), who developed an [*online version of JSPEC with GUI*](https://beta.sirepo.com/#/jspec) on their cloud server, [*Sirepo*](https://beta.sirepo.com/). 

My colleagues at Jefferson Lab. 

## Contact the authors

Dr. He Zhang at [*Jefferson Lab*](www.jlab.org) by hezhang.AT.jlab.org. 