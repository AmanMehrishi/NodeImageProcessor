# Node-Based Image Processor

## Overview

The Node-Based Image Processor is a desktop application written in C++ that provides a visual, node-based interface for performing image manipulations. Inspired by node editors such as Substance Designer, this project uses a node graph where each node represents an image operation (input, processing, or output). The application is built using [Qt6](https://www.qt.io) for its graphical user interface and [OpenCV](https://opencv.org) for underlying image processing tasks.

## Features

- **Node-Based Editor:**  
  Create and connect nodes visually on a canvas. Drag and drop connections between nodes to define image processing pipelines.
- **Image Input Node:**  
  Load images from disk and display metadata (dimensions, file size, format).
- **Processing Nodes:**  
  - **Brightness/Contrast Node:**  
    Adjust brightness and contrast using sliders.
  - **Edge Detection Node:**  
    Supports both Canny and Sobel edge detection methods with configurable thresholds, kernel size (for Sobel), and an overlay option.
  - **Color Channel Splitter Node:**  
    Split an RGB/RGBA image into separate channel outputs with an option to display them in grayscale.
  - **Blur Node:**  
    Apply Gaussian blur with a configurable blur radius and an option for directional blur.
  - **Threshold Node:**  
    Convert an image into a binary representation using various thresholding techniques (binary, binary inverse, adaptive mean, Otsu).
  - **Blend Node:**  
    Blend two input images using a variety of blend modes (normal, multiply, screen, overlay, difference) with an adjustable opacity slider.
  - **Noise Generation Node:**  
    Generate procedural noise (Perlin, Simplex, or Worley) with configuration options for noise scale, octaves, and persistence; optionally use the noise as a displacement map.
  - **Convolution Filter Node:**  
    Apply a user-defined convolution kernel or select from common presets (sharpen, emboss, edge enhance).
- **Output Node:**  
  Save the final processed image to disk, with options for format and quality settings. A real-time preview of the final result is displayed.
- **Dynamic Properties Panel:**  
  When a node is selected, the properties panel shows controls specific to that node type. Changes are applied in real time to update processing and previews.
- **Robust Error Handling:**  
  The node graph validates connections and prevents crashes due to missing input or invalid operations.

## Project Structure

```plaintext
NodeBasedImageProcessor/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── NodeGraph.cpp
│   ├── NodeEdge.cpp
│   ├── NodeItem.cpp
│   ├── ImageInputNode.cpp
│   ├── BrightnessContrastNode.cpp
│   ├── EdgeDetectionNode.cpp
│   ├── OutputNode.cpp
│   ├── ColorChannelSplitterNode.cpp
│   ├── BlurNode.cpp
│   ├── ThresholdNode.cpp
│   ├── BlendNode.cpp
│   ├── NoiseGenerationNode.cpp
│   └── ConvolutionFilterNode.cpp
└── include/
    ├── MainWindow.h
    ├── Node.h
    ├── NodeGraph.h
    ├── NodeEdge.h
    ├── NodeItem.h
    ├── ImageInputNode.h
    ├── BrightnessContrastNode.h
    ├── EdgeDetectionNode.h
    ├── OutputNode.h
    ├── ColorChannelSplitterNode.h
    ├── BlurNode.h
    ├── ThresholdNode.h
    ├── BlendNode.h
    ├── NoiseGenerationNode.h
    └── ConvolutionFilterNode.h
```
## CMakeLists.txt

Configures the project with C++17, enables AUTOMOC (for Qt’s meta–object compiler), locates Qt6 and OpenCV libraries, and defines the build targets.

## src/

Contains implementation files for the main application logic, the GUI (MainWindow, NodeItem, NodeEdge), and all node processing code.

## include/

Contains header files for the GUI components and node classes.

## Build Instructions

### Prerequisites

- **C++ Compiler:** Microsoft Visual Studio 2022 (MSVC) or another modern C++ compiler.
- **CMake:** Version 3.19 or newer.
- **Qt6:** Installed and configured (e.g., under `D:/Qt2/6.10.0/msvc2022_64`).
- **OpenCV:** Prebuilt binaries for Windows (e.g., configured in `D:/opencv/build/x64/vc16/lib`).

### Building with CMake

1. **Create a Build Directory and Configure:**
   ```bash
   mkdir build
   cd build
   cmake -G "Visual Studio 17 2022" ..
   ```
2. Open the Generated Solution: Open NodeBasedImageProcessor.sln in Visual Studio.
3. Build the Project: Select Build → Rebuild Solution in Visual Studio, or run:
   ```bash
   cmake --build . --config Debug
   ```

### Usage Instructions
- Launch the Application: The main window appears with a canvas and a properties panel (docked on the right).

- Add Nodes: Use the Add Node menu to insert nodes into the canvas:

  - Input Node: Prompts for an image file to load.

  - Brightness/Contrast Node: Adjust the brightness and contrast.

  - Edge Detection Node: Configure edge detection (choose Canny or Sobel, adjust thresholds, etc.).

  - Output Node: Save the processed image.

  - Additional nodes for color channel splitting, blur, threshold, blend, noise generation, and convolution filtering are also available.

- Connect Nodes: Drag from a node’s output port (right side) and drop it onto another node’s input port. Valid connections are drawn as edges on the canvas, and the underlying node graph is updated.

- Adjust Node Properties: Select a node to display its properties in the panel. Use the provided controls (sliders, combo boxes, checkboxes) to modify parameters in real time. The node processes its input immediately and updates its preview.

- View Real-Time Updates: As changes are made through the properties panel, processing occurs in real time, and the updated preview is displayed on the corresponding node.

- Save the Final Image: Use the Save Image option from the File menu to export the final processed image.
