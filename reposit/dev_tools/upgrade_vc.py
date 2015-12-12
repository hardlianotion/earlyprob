
import shutil
import re

OLD_VER = "vc9"
NEW_VER = "vc12"
#ROOT_DIR = "C:/Users/erik/Documents/repos/reposit/quantlib/"
ROOT_DIR = "C:/Users/erik/Documents/repos/reposit_v1.7.x_etuka/quantlib/"

FILES = (

    #"swig_vcxx.sln",
    #"swig_vcxx.vcproj"

    "log4cxx/msvc/apr_vcxx.vcproj",
#    #"log4cxx/msvc/apr_vcxx.vcxproj.filters",
    "log4cxx/msvc/aprutil_vcxx.vcproj",
#    #"log4cxx/msvc/aprutil_vcxx.vcxproj.filters",
    "log4cxx/msvc/log4cxx_vcxx.sln",
    "log4cxx/msvc/log4cxx_vcxx.vcproj",
#    #"log4cxx/msvc/log4cxx_vcxx.vcxproj.filters",
#
#    "reposit/Docs/docs_vcxx.vcxproj",
#    #"reposit/Docs/docs_vcxx.vcxproj.filters",
#    "reposit/Examples/C++/ExampleCpp_vcxx.vcxproj",
#    #"reposit/Examples/C++/ExampleCpp_vcxx.vcxproj.filters",
#    "reposit/Examples/ExampleObjects/ExampleObjects_vcxx.vcxproj",
#    #"reposit/Examples/ExampleObjects/ExampleObjects_vcxx.vcxproj.filters",
#    "reposit/Examples/xl/ExampleXllDynamic1_vcxx.vcxproj",
#    #"reposit/Examples/xl/ExampleXllDynamic1_vcxx.vcxproj.filters",
#    "reposit/Examples/xl/ExampleXllDynamic2_vcxx.vcxproj",
#    #"reposit/Examples/xl/ExampleXllDynamic2_vcxx.vcxproj.filters",
#    "reposit/Examples/xl/ExampleXllStatic_vcxx.vcxproj",
#    #"reposit/Examples/xl/ExampleXllStatic_vcxx.vcxproj.filters",
#    "reposit/gensrc/rpgensrc_vcxx.vcxproj",
#    #"reposit/gensrc/rpgensrc_vcxx.vcxproj.filters",
#    "reposit/reposit_vcxx.sln",
#    "reposit/rplib_vcxx.vcxproj",
#    #"reposit/rplib_vcxx.vcxproj.filters",
#    "reposit/rpxl/rpxll/rpxll_vcxx.vcxproj",
#    #"reposit/rpxl/rpxll/rpxll_vcxx.vcxproj.filters",
    "reposit/rpxl/rpxllib/rpxllib_vcxx.vcproj",
#    #"reposit/rpxl/rpxllib/rpxllib_vcxx.vcxproj.filters",
#    "reposit/rpxl/rpxllib/rpxllib2_vcxx.vcxproj",
#    #"reposit/rpxl/rpxllib/rpxllib2_vcxx.vcxproj.filters",
    "reposit/xlsdk/xlsdk_vcxx.vcproj",
#    #"reposit/xlsdk/xlsdk_vcxx.vcxproj.filters",
#
#    "QuantLibAddin/Addins/Cpp/AddinCpp_vcxx.vcxproj",
#    #"QuantLibAddin/Addins/Cpp/AddinCpp_vcxx.vcxproj.filters",
#    "QuantLibAddin/Clients/Cpp/ClientCppDemo_vcxx.vcxproj",
#    #"QuantLibAddin/Clients/Cpp/ClientCppDemo_vcxx.vcxproj.filters",
#    "QuantLibAddin/Clients/CppInstrumentIn/CppInstrumentIn_vcxx.vcxproj",
#    #"QuantLibAddin/Clients/CppInstrumentIn/CppInstrumentIn_vcxx.vcxproj.filters",
#    "QuantLibAddin/Clients/CppSwapOut/ClientCppSwapOut_vcxx.vcxproj",
#    #"QuantLibAddin/Clients/CppSwapOut/ClientCppSwapOut_vcxx.vcxproj.filters",
#    "QuantLibAddin/Docs/docs-QuantLibAddin_vcxx.vcxproj",
#    #"QuantLibAddin/Docs/docs-QuantLibAddin_vcxx.vcxproj.filters",
    "QuantLibAddin2/swig/swigrun_xl_vcxx.vcproj",
#    #"QuantLibAddin/swig/qlswig_vcxx.vcxproj.filters",
#    "QuantLibAddin/QuantLibAddin_vcxx.sln",
    "QuantLibAddin2/QuantLibObjects_vcxx.vcproj",
#    #"QuantLibAddin/QuantLibObjects_vcxx.vcxproj.filters",
#    "QuantLibAddin/QuantLibObjects2_vcxx.vcxproj",
#    #"QuantLibAddin/QuantLibObjects2_vcxx.vcxproj.filters",
#    "QuantLibAddin/QuantLibObjects3_vcxx.vcxproj",
#    #"QuantLibAddin/QuantLibObjects3_vcxx.vcxproj.filters",
#    "QuantLibAddin/QuantLibObjects4_vcxx.vcxproj",
#    #"QuantLibAddin/QuantLibObjects4_vcxx.vcxproj.filters",
#
#    "QuantLibXL/Docs/docs-QuantLibXL_vcxx.vcxproj",
#    #"QuantLibXL/Docs/docs-QuantLibXL_vcxx.vcxproj.filters",
#    "QuantLibXL/qlxl/QuantLibXLDynamic_vcxx.vcxproj",
#    #"QuantLibXL/qlxl/QuantLibXLDynamic_vcxx.vcxproj.filters",
    "QuantLibXL2/qlxl/QuantLibXLStatic_vcxx.vcproj",
#    #"QuantLibXL/qlxl/QuantLibXLStatic_vcxx.vcxproj.filters",
#    "QuantLibXL/qlxl/QuantLibXLStatic2_vcxx.vcxproj",
#    #"QuantLibXL/qlxl/QuantLibXLStatic2_vcxx.vcxproj.filters",
#    "QuantLibXL/QuantLibAllDynamic_vcxx.sln",
#    "QuantLibXL/QuantLibXL_basic_vcxx.sln",
    "QuantLibXL2/QuantLibXL_full_vcxx.sln",
)

for fileName in FILES:
    fileNameOld = re.sub("vcxx", OLD_VER, fileName)
    fileNameNew = re.sub("vcxx", NEW_VER, fileName)
    filePathOld = ROOT_DIR + fileNameOld
    filePathNew = ROOT_DIR + fileNameNew
    print filePathNew
    shutil.copy(filePathOld, filePathNew)
    if ".vcxproj.filters" == fileName[-16:]:
        continue
    f = open(filePathNew, 'r+')
    buf = f.read()
    buf = re.sub(OLD_VER, NEW_VER, buf)
    f.seek(0)
    f.write(buf)
    f.close()

