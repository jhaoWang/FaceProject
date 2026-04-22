#include "Lafin.h"
#include <onnxruntime_cxx_api.h>

using namespace cv;
using namespace std;

#define MODEL_PATH L"E:/pythonCode/lafin-master/lafin-master/inpaint.onnx"

__declspec(dllexport) uint32_t LaFin_Inpaint(const vector<Mat>& srcImages, Mat& result)
{
    try {
        // ========== 1. 检查图像数量 ==========
        if (srcImages.size() != 3) {
            return LAFIN_ERR_IMAGE_COUNT;
        }

        Mat img = srcImages[0];
        Mat landmark = srcImages[1];
        Mat mask = srcImages[2];

        // ========== 2. 检查图像是否为空 ==========
        if (img.empty() || landmark.empty() || mask.empty()) {
            return LAFIN_ERR_EMPTY_IMAGE;
        }

        // ========== 3. 预处理 ==========
        Mat img_rgb, img_resized;
        cvtColor(img, img_rgb, COLOR_BGR2RGB);
        resize(img_rgb, img_resized, Size(256, 256));

        Mat lmk_resized, mask_resized;
        resize(landmark, lmk_resized, Size(256, 256));
        resize(mask, mask_resized, Size(256, 256));
        threshold(mask_resized, mask_resized, 0, 255, THRESH_BINARY);

        // ========== 4. 构造输入 ==========
        vector<float> input_img(1 * 3 * 256 * 256);
        vector<float> input_lmk(1 * 1 * 256 * 256);
        vector<float> input_mask(1 * 1 * 256 * 256);

        for (int h = 0; h < 256; h++) {
            for (int w = 0; w < 256; w++) {
                Vec3b pix = img_resized.at<Vec3b>(h, w);
                input_img[0 * 256 * 256 + h * 256 + w] = pix[0] / 127.5f - 1.0f;
                input_img[1 * 256 * 256 + h * 256 + w] = pix[1] / 127.5f - 1.0f;
                input_img[2 * 256 * 256 + h * 256 + w] = pix[2] / 127.5f - 1.0f;
            }
        }

        for (int h = 0; h < 256; h++)
            for (int w = 0; w < 256; w++)
                input_lmk[h * 256 + w] = lmk_resized.at<uchar>(h, w) / 255.0f;

        for (int h = 0; h < 256; h++)
            for (int w = 0; w < 256; w++)
                input_mask[h * 256 + w] = mask_resized.at<uchar>(h, w) / 255.0f;

        // ========== 5. 推理 ==========
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "LaFin");
        Ort::Session session(env, MODEL_PATH, Ort::SessionOptions{});

        const char* input_names[] = { "onnx::Mul_0", "onnx::Concat_1", "masks" };
        const char* output_names[] = { "770" };

        vector<int64_t> shape_img = { 1,3,256,256 };
        vector<int64_t> shape_lmk = { 1,1,256,256 };
        vector<int64_t> shape_mask = { 1,1,256,256 };

        auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        auto t_img = Ort::Value::CreateTensor<float>(memory_info, input_img.data(), input_img.size(), shape_img.data(), 4);
        auto t_lmk = Ort::Value::CreateTensor<float>(memory_info, input_lmk.data(), input_lmk.size(), shape_lmk.data(), 4);
        auto t_msk = Ort::Value::CreateTensor<float>(memory_info, input_mask.data(), input_mask.size(), shape_mask.data(), 4);

        Ort::Value inputs[3] = { move(t_img), move(t_lmk), move(t_msk) };
        auto outputs = session.Run(Ort::RunOptions{}, input_names, inputs, 3, output_names, 1);

        // ========== 6. 后处理 ==========
        float* out_data = outputs[0].GetTensorMutableData<float>();
        Mat res(256, 256, CV_32FC3);

        for (int c = 0; c < 3; c++)
            for (int h = 0; h < 256; h++)
                for (int w = 0; w < 256; w++)
                    res.at<Vec3f>(h, w)[c] = out_data[c * 256 * 256 + h * 256 + w];

        res.convertTo(res, CV_8UC3, 255);
        cvtColor(res, result, COLOR_RGB2BGR);

        // 成功
        return LAFIN_OK;
    }
    catch (const Ort::Exception&) {
        return LAFIN_ERR_INFERENCE;
    }
    catch (...) {
        return LAFIN_ERR_UNKNOWN;
    }
}