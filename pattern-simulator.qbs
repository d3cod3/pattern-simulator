import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '../../..'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "src/matrixAreas.cpp",
            "src/matrixAreas.h",
            "src/ofxFolding/AMFMSineWave.cpp",
            "src/ofxFolding/AMFMSineWave.h",
            "src/ofxFolding/AbstractWave.cpp",
            "src/ofxFolding/AbstractWave.h",
            "src/ofxFolding/ConstantWave.cpp",
            "src/ofxFolding/ConstantWave.h",
            "src/ofxFolding/CosineWave.cpp",
            "src/ofxFolding/CosineWave.h",
            "src/ofxFolding/FMHarmonicSquareWave.cpp",
            "src/ofxFolding/FMHarmonicSquareWave.h",
            "src/ofxFolding/FMSawtoothWave.cpp",
            "src/ofxFolding/FMSawtoothWave.h",
            "src/ofxFolding/FMSineWave.cpp",
            "src/ofxFolding/FMSineWave.h",
            "src/ofxFolding/FMSquareWave.cpp",
            "src/ofxFolding/FMSquareWave.h",
            "src/ofxFolding/FMTriangleWave.cpp",
            "src/ofxFolding/FMTriangleWave.h",
            "src/ofxFolding/SineWave.cpp",
            "src/ofxFolding/SineWave.h",
            "src/ofxFolding/WaveState.cpp",
            "src/ofxFolding/WaveState.h",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
            "src/ofxFolding/ofxFolding.h",
        ]

        of.addons: [
            'ofxDatGui','ofxOpenCv','ofxXmlSettings'
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
    // will compile OF only once for all your projects
    // otherwise compiled per project with qbs

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
