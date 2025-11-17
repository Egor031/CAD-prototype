# CAD-prototype: Учебный CAD-редактор на базе OpenCASCADE

## Описание проекта

**CAD-prototype** — это дипломный проект, представляющий собой учебный прототип системы автоматизированного проектирования (CAD), разработанный с использованием современных технологий 3D-моделирования. Проект создан как демонстрационная реализация базовых функций CAD-системы в рамках выпускной квалификационной работы.

## Цель проекта

Основная цель проекта — разработка прототипа CAD-системы, демонстрирующего основные принципы работы с 3D-моделями. Проект направлен на:

* Освоение технологий 3D-моделирования и визуализации
* Реализацию базовых функций CAD-системы
* Демонстрацию взаимодействия различных программных компонентов
* Создание удобного пользовательского интерфейса

## Основополагающие технологии

Проект базируется на форке существующего решения [OcctImgui](https://github.com/eryar/OcctImgui) и использует следующие технологии:

* **OpenCASCADE** — платформа для 3D-моделирования и визуализации
* **IMGUI** — библиотека для создания графического интерфейса
* **GLFW** — библиотека для работы с окнами и вводом
* **Visual Studio** — среда разработки

 ## Подключение библиотек к проекту

Для успешного подключения библиотек к проекту необходимо выполнить следующие шаги:

1. **Скачать необходимые библиотеки:**
* с официального сайта OpenCASCADE (https://dev.opencascade.org/release) — предварительно скомпилированные бинарные файлы для Windows (Windows package and 3rd-party VC++ 2022 64 bit: occt-vc14-64-combined.zip);
* с официального сайта GLFW (https://www.glfw.org/download.html) — предварительно скомпилированные бинарные файлы для Windows (64-bit Windows binaries);
* с официального репозитория ImGUI (https://github.com/ocornut/imgui) — библиотеку ImGUI.

2. **Настроить пути в Visual Studio:**
* открыть «Проект — Свойства»;
* в появившемся окне перейти на вкладку **«С/С++ — Общее — Дополнительные каталоги включаемых файлов»** и добавить пути к папкам:
  * `Путь\до\библиотеки\occt-vc14-64\inc`;
  * `Путь\до\библиотеки\glfw-3.4.bin.WIN64\include`;
  * `Путь\до\библиотеки\imgui-1.92.4`;
  * `Путь\до\библиотеки\imgui-1.92.4\backends`.
* перейти в **«Компоновщик — Общее — Дополнительные каталоги библиотек»** и добавить пути к папкам:
  * `Путь\до\библиотеки\occt-vc14-64\win64\vc14\lib`;
  * `Путь\до\библиотеки\glfw-3.4.bin.WIN64\lib-vc2022`.
* перейти в **«Компоновщик — Ввод — Дополнительные зависимости»** и подключить файлы `.lib`:
  * от OpenCascade: список библиотек `TKBin.lib;TKBinL.lib;TKBinTObj.lib;TKBinXCAF.lib;TKBO.lib;TKBool.lib;TKBRep.lib;TKCAF.lib;TKCDF.lib;TKD3DHost.lib;TKD3DHostTest.lib;TKDCAF.lib;TKDE.lib;TKDECascade.lib;TKDEGLTF.lib;TKDEIGES.lib;TKDEOBJ.lib;TKDEPLY.lib;TKDESTEP.lib;TKDESTL.lib;TKDEVRML.lib;TKDFBrowser.lib;TKDraw.lib;TKernel.lib;TKExpress.lib;TKFeat.lib;TKFillet.lib;TKG2d.lib;TKG3d.lib;TKGeomAlgo.lib;TKGeomBase.lib;TKHLR.lib;TKIVtk.lib;TKIVtkDraw.lib;TKLCAF.lib;TKMath.lib;TKMesh.lib;TKMeshVS.lib;TKMessageModel.lib;TKMessageView.lib;TKOffset.lib;TKOpenGl.lib;TKOpenGles.lib;TKOpenGlesTest.lib;TKOpenGlTest.lib;TKPrim.lib;TKQADraw.lib;TKRWMesh.lib;TKService.lib;TKShapeView.lib;TKShHealing.lib;TKStd.lib;TKStdL.lib;TKTInspector.lib;TKTInspectorAPI.lib;TKTObj.lib;TKTObjDRAW.lib;TKToolsDraw.lib;TKTopAlgo.lib;TKTopTest.lib;TKTreeModel.lib;TKV3d.lib;TKVCAF.lib;TKView.lib;TKViewerTest.lib;TKVInspector.lib;TKXCAF.lib;TKXDEDRAW.lib;TKXMesh.lib;TKXml.lib;TKXmlL.lib;TKXmlTObj.lib;TKXmlXCAF.lib;TKXSBase.lib;TKXSDRAW.lib;TKXSDRAWDE.lib;TKXSDRAWGLTF.lib;TKXSDRAWIGES.lib;TKXSDRAWOBJ.lib;TKXSDRAWPLY.lib;TKXSDRAWSTEP.lib;TKXSDRAWSTL.lib;TKXSDRAWVRML.lib;`
  * от GLFW: `glfw3.lib;glfw3_mt.lib;glfw3dll.lib;`
  * статическая библиотека для работы с OpenGL: `opengl32.lib` (входящая в состав Microsoft SDK).

3. **Добавить файлы в обозреватель решений:**
* из папки `Путь\до\библиотеки\imgui-1.92.4` добавить следующие файлы:
  * `imconfig.h`;
  * `imgui.cpp`;
  * `imgui.h`;
  * `imgui_demo.cpp`;
  * `imgui_draw.cpp`;
  * `imgui_internal.h`;
  * `imgui_tables.cpp`;
  * `imgui_widgets.cpp`;
  * `imstb_rectpack.h`;
  * `imstb_textedit.h`;
  * `imstb_truetype.h`.
* из папки `Путь\до\библиотеки\imgui-1.92.4\backends` добавить следующие файлы:
  * `imgui_impl_glfw.cpp`;
  * `imgui_impl_glfw.h`;
  * `imgui_impl_opengl3.cpp`;
  * `imgui_impl_opengl3.h`;
  * `imgui_impl_opengl3_loader.h`.

  
  <img width="312" height="525" alt="image" src="https://github.com/user-attachments/assets/542cbf31-ffe3-4585-97a8-bd0368e4b3e8" />

4. **Скопировать файлы в папку с .exe файлом проекта:**
* скопировать содержимое папок `bin` с библиотек:
  * из `Путь\до\библиотеки\occt-vc14-64\win64\vc14\bin`;
  * из `Путь\до\библиотеки\3rdparty-vc14-64\tbb-2021.13.0-x64\bin`;
  * из `Путь\до\библиотеки\3rdparty-vc14-64\jemalloc-vc14-64\bin`;
  * из `Путь\до\библиотеки\3rdparty-vc14-64\freetype-2.13.3-x64\bin`;
  * из `Путь\до\библиотеки\3rdparty-vc14-64\openvr-1.14.15-64\bin\win64`;
  * из `Путь\до\библиотеки\3rdparty-vc14-64\freeimage-3.18.0-x64\bin`;
  * из `Путь\до\библиотеки\3rdparty-vc14-64\ffmpeg-3.3.4-64\bin`;
  * из `Путь\до\библиотеки\glfw-3.4.bin.WIN64\lib-vc2022`;
* вставить все файлы в `Путь\до\проекта\x64\Debug` (или `Release`).
