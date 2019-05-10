QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp

INCLUDEPATH += /usr/local/include \
/usr/local/include/opencv \
/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_aruco.so \
/usr/local/lib/libopencv_aruco.so.3.4 \
/usr/local/lib/libopencv_aruco.so.3.4.1 \
/usr/local/lib/libopencv_bgsegm.so \
/usr/local/lib/libopencv_bgsegm.so.3.4 \
/usr/local/lib/libopencv_bgsegm.so.3.4.1 \
/usr/local/lib/libopencv_bioinspired.so \
/usr/local/lib/libopencv_bioinspired.so.3.4 \
/usr/local/lib/libopencv_bioinspired.so.3.4.1 \
/usr/local/lib/libopencv_calib3d.so \
/usr/local/lib/libopencv_calib3d.so.3.4 \
/usr/local/lib/libopencv_calib3d.so.3.4.1 \
/usr/local/lib/libopencv_ccalib.so \
/usr/local/lib/libopencv_ccalib.so.3.4 \
/usr/local/lib/libopencv_ccalib.so.3.4.1 \
/usr/local/lib/libopencv_core.so \
/usr/local/lib/libopencv_core.so.3.4 \
/usr/local/lib/libopencv_core.so.3.4.1 \
/usr/local/lib/libopencv_datasets.so \
/usr/local/lib/libopencv_datasets.so.3.4 \
/usr/local/lib/libopencv_datasets.so.3.4.1 \
/usr/local/lib/libopencv_dnn.so \
/usr/local/lib/libopencv_dnn.so.3.4 \
/usr/local/lib/libopencv_dnn.so.3.4.1 \
/usr/local/lib/libopencv_dpm.so \
/usr/local/lib/libopencv_dpm.so.3.4 \
/usr/local/lib/libopencv_dpm.so.3.4.1 \
/usr/local/lib/libopencv_face.so \
/usr/local/lib/libopencv_face.so.3.4 \
/usr/local/lib/libopencv_face.so.3.4.1 \
/usr/local/lib/libopencv_features2d.so \
/usr/local/lib/libopencv_features2d.so.3.4 \
/usr/local/lib/libopencv_features2d.so.3.4.1 \
/usr/local/lib/libopencv_flann.so \
/usr/local/lib/libopencv_flann.so.3.4 \
/usr/local/lib/libopencv_flann.so.3.4.1 \
/usr/local/lib/libopencv_freetype.so \
/usr/local/lib/libopencv_freetype.so.3.4 \
/usr/local/lib/libopencv_freetype.so.3.4.1 \
/usr/local/lib/libopencv_fuzzy.so \
/usr/local/lib/libopencv_fuzzy.so.3.4 \
/usr/local/lib/libopencv_fuzzy.so.3.4.1 \
/usr/local/lib/libopencv_highgui.so \
/usr/local/lib/libopencv_highgui.so.3.4 \
/usr/local/lib/libopencv_highgui.so.3.4.1 \
/usr/local/lib/libopencv_imgcodecs.so \
/usr/local/lib/libopencv_imgcodecs.so.3.4 \
/usr/local/lib/libopencv_imgcodecs.so.3.4.1 \
/usr/local/lib/libopencv_imgproc.so \
/usr/local/lib/libopencv_imgproc.so.3.4 \
/usr/local/lib/libopencv_imgproc.so.3.4.1 \
/usr/local/lib/libopencv_ml.so \
/usr/local/lib/libopencv_ml.so.3.4 \
/usr/local/lib/libopencv_ml.so.3.4.1 \
/usr/local/lib/libopencv_objdetect.so \
/usr/local/lib/libopencv_objdetect.so.3.4 \
/usr/local/lib/libopencv_objdetect.so.3.4.1 \
/usr/local/lib/libopencv_optflow.so \
/usr/local/lib/libopencv_optflow.so.3.4 \
/usr/local/lib/libopencv_optflow.so.3.4.1 \
/usr/local/lib/libopencv_phase_unwrapping.so \
/usr/local/lib/libopencv_phase_unwrapping.so.3.4 \
/usr/local/lib/libopencv_phase_unwrapping.so.3.4.1 \
/usr/local/lib/libopencv_photo.so \
/usr/local/lib/libopencv_photo.so.3.4 \
/usr/local/lib/libopencv_photo.so.3.4.1 \
/usr/local/lib/libopencv_plot.so \
/usr/local/lib/libopencv_plot.so.3.4 \
/usr/local/lib/libopencv_plot.so.3.4.1 \
/usr/local/lib/libopencv_reg.so \
/usr/local/lib/libopencv_reg.so.3.4 \
/usr/local/lib/libopencv_reg.so.3.4.1 \
/usr/local/lib/libopencv_rgbd.so \
/usr/local/lib/libopencv_rgbd.so.3.4 \
/usr/local/lib/libopencv_rgbd.so.3.4.1 \
/usr/local/lib/libopencv_saliency.so \
/usr/local/lib/libopencv_saliency.so.3.4 \
/usr/local/lib/libopencv_saliency.so.3.4.1 \
/usr/local/lib/libopencv_shape.so \
/usr/local/lib/libopencv_shape.so.3.4 \
/usr/local/lib/libopencv_shape.so.3.4.1 \
/usr/local/lib/libopencv_stereo.so \
/usr/local/lib/libopencv_stereo.so.3.4 \
/usr/local/lib/libopencv_stereo.so.3.4.1 \
/usr/local/lib/libopencv_stitching.so \
/usr/local/lib/libopencv_stitching.so.3.4 \
/usr/local/lib/libopencv_stitching.so.3.4.1 \
/usr/local/lib/libopencv_structured_light.so \
/usr/local/lib/libopencv_structured_light.so.3.4 \
/usr/local/lib/libopencv_structured_light.so.3.4.1 \
/usr/local/lib/libopencv_superres.so \
/usr/local/lib/libopencv_superres.so.3.4 \
/usr/local/lib/libopencv_superres.so.3.4.1 \
/usr/local/lib/libopencv_surface_matching.so \
/usr/local/lib/libopencv_surface_matching.so.3.4 \
/usr/local/lib/libopencv_surface_matching.so.3.4.1 \
/usr/local/lib/libopencv_text.so \
/usr/local/lib/libopencv_text.so.3.4 \
/usr/local/lib/libopencv_text.so.3.4.1 \
/usr/local/lib/libopencv_tracking.so \
/usr/local/lib/libopencv_tracking.so.3.4 \
/usr/local/lib/libopencv_tracking.so.3.4.1 \
/usr/local/lib/libopencv_video.so \
/usr/local/lib/libopencv_video.so.3.4 \
/usr/local/lib/libopencv_video.so.3.4.1 \
/usr/local/lib/libopencv_videoio.so \
/usr/local/lib/libopencv_videoio.so.3.4 \
/usr/local/lib/libopencv_videoio.so.3.4.1 \
/usr/local/lib/libopencv_videostab.so \
/usr/local/lib/libopencv_videostab.so.3.4 \
/usr/local/lib/libopencv_videostab.so.3.4.1 \
/usr/local/lib/libopencv_xfeatures2d.so \
/usr/local/lib/libopencv_xfeatures2d.so.3.4 \
/usr/local/lib/libopencv_xfeatures2d.so.3.4.1 \
/usr/local/lib/libopencv_ximgproc.so \
/usr/local/lib/libopencv_ximgproc.so.3.4 \
/usr/local/lib/libopencv_ximgproc.so.3.4.1 \
/usr/local/lib/libopencv_xobjdetect.so \
/usr/local/lib/libopencv_xobjdetect.so.3.4 \
/usr/local/lib/libopencv_xobjdetect.so.3.4.1 \
/usr/local/lib/libopencv_xphoto.so \
/usr/local/lib/libopencv_xphoto.so.3.4 \
/usr/local/lib/libopencv_xphoto.so.3.4.1 \
