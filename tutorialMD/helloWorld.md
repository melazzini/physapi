@page helloWorld Hello World

In this section you are going to see how to configure physapi and test that everything is working properly.
In the beta version I simply give you the source code, probably in future versions, I will also give you the API as a library(.dll, .so, etc).
We're going to use <a href="https://visualstudio.microsoft.com/" title="go to visual studio page" target="_blank">vs2019</a> . One reason to use visual studio is that it really simplifies the way you combine CUDA code with c++ code, in addition
it handles the compilers(for the CPU and GPU) integration.

@tableofcontents

@section helloWorldSection1 Hardware

In order to be able to use physapi you need to be sure that you have at least the following components:

* 8GB of ram.
* CUDA-capable GPU, for example: RTX-2080.

@section helloWorldSection2 Software

@subsection helloWorldSection2Subsection1 Software installation

I've created physapi in Windows and Linux at same time, but at the moment it has only been tested with full capabilities(CPU + CUDA) in windows.
Therefore, I recommend you, if you have the possibilities, to work in windows and follow along the whole tutorial, specially because
I will not show you how to work with the nvic compiler, which is needed to compile the GPU code. With that being said, the next piece of software
is required to use physapi:

* Windows 10.
* <a href="https://visualstudio.microsoft.com/vs/community/" target="_blank" title="go to vs community download page"> Visual Studio 2019(enterprise or community edition).</a>
	Download and install visual studio.
* <a href="https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html" target="_blank" title="go to CUDA toolkit main page"> CUDA-toolkit.</a>
	Download and install the CUDA toolkit.
* <a href="https://github.com/melazzini/physapi" target="_blank" title="go to physapi download page on github">physapi.</a>
	Download the physapi source code.

@subsection helloWorldSection2Subsection2 Setting up a physapi project in visual studio.

Now that you have installed all the needed software, it's time to create a project that can use physapi.

1. Open up VS and go to "Create New Project".
2. In the "Search For Templates" box enter "CUDA" and select the project template that uses CUDA[>=10] runtime.
3. Click "Next".
4. Give your project a name, for example <em>"helloWorldWithPhysapi"</em> and choose a location.
5. Click "Create".
6. Build your project(<kbd>CTL+SHIFT+B</kbd>).
7. Run your application project(Go to Debug menu and click on "Start Without Debugging").
	At this point you will see a CUDA template application that runs on your platform.
8. Copy the physapi source folder(include + src) into your project folder.
9. Delete the templates created by visual studio(it may be "kernel.cu") and create a main.cpp file.
10. Write the next piece of code in main.cpp to test that everything is working OK with C++:
```cpp
#include<iostream>	

using namespace std;

int main()
{
	cout << "Hello world!" << endl;

	return 0;
}
```
11. In the "Solution Explorer" window click on "Show All Files". Be sure to see the physapi source folders included in the tree view.
12. In the "Project" menu click on "Properties".
13. Select "All Configurations" in the configurations drop down menu.
14. Go to "C/C++" section and click on the "Additional Include Directories", then click "Edit".
15. Include the physapi modules that you need(try including all the sub-folders inside the include folder).
16. Go to "VC++" section and click on the "Source Directories", then click "Edit".
17. Add the path to the source( src ) folders that you need.
18. In the "Language" subsection of the "C/C++" select "C++17" and also enable the latest features.
19. Click "Apply" and then click "OK".
20. In the solution explorer right click on the physapi src folder and select "Include In Project".
    Similarly you can include different files as needed if you have some trouble with linking.
21. Build and run your project(in debug mode and then also in release mode). You should see the Windows console displaying you message.

If you have some trouble please refer to <a href="../../include/examples/helloWorldWithPhysapi">helloWorldWithPhysapi</a> folder. It is a visual studio solution and you can use it. Simply copy it
to some location and try building it.


Great!
In this tutorial you've seen how to create a simple project to work with physapi. To use physapi in other platforms(for example Linux) the procedure has to be similar.
Of course you can use different tools like make, etc.
