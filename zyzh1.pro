QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#目标信息
TARGET_INFO=""
greaterThan(QT_MAJOR_VERSION,4){
        TARGET_ARCH=$${QT_ARCH}
}else{
        TARGET_ARCH=$${QMAKE_HOST.arch}
}

contains(TARGET_ARCH, x86_64){
    TARGET_INFO+='i686'
}
contains(TARGET_ARCH, i386){
    TARGET_INFO+='i386'
}
contains(TARGET_ARCH, arm){
    TARGET_INFO+='arm'
    DEFINES += TARGET_ARCH_ARM
}

TARGET_ARCH+=$${TARGET_ARCH}

msvc{
##### windows-msvc
    TARGET_INFO += 'windows'
    TARGET_INFO += 'MSVC'
    #msvc编译时需要确保代码文件为utf-8
    QMAKE_CXXFLAGS += /utf-8
    #INCLUDEPATH += G:/app/openCV/opencv/build/include/
    CONFIG(debug, debug|release) {
        #LIBS += G:/app/opencv/opencv/build/x64/vc15/lib/opencv_world460d.lib
        TARGET_INFO+="debug"
    }else{
        #LIBS += G:/app/opencv/opencv/build/x64/vc15/lib/opencv_world460.lib
        TARGET_INFO+="release"
    }
}
mingw{
##### windows-mingw
    TARGET_INFO += 'windows'
    TARGET_INFO += 'mingw'
    #INCLUDEPATH += G:/app/openCV/opencv/build/include/
    CONFIG(debug, debug|release) {
        #LIBS += G:/app/opencv/opencv/build/x64/vc15/lib/opencv_world460d.lib
        TARGET_INFO+="debug"
    }else{
        #LIBS += G:/app/opencv/opencv/build/x64/vc15/lib/opencv_world460.lib
        TARGET_INFO+="release"
    }
}
unix{
##### linux
    TARGET_INFO += 'linux'
    #INCLUDEPATH += /usr/include/opencv4
    #LIBS += /usr/lib/libopencv_*.so
    CONFIG(debug, debug|release) {
        TARGET_INFO+="debug"
    }else{
        TARGET_INFO+="release"
    }
}
#输出编译平台信息
message($$TARGET_INFO)

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
