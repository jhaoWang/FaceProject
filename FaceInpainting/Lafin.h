#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#define LAFIN_OK               0
#define LAFIN_ERR_IMAGE_COUNT  1
#define LAFIN_ERR_EMPTY_IMAGE  2
#define LAFIN_ERR_INFERENCE    3
#define LAFIN_ERR_UNKNOWN      0xFFFFFFFF

    __declspec(dllexport) uint32_t LaFin_Inpaint(
        const std::vector<cv::Mat>& srcImages,
        std::vector<cv::Mat>& result
    );

#ifdef __cplusplus
}
#endif
