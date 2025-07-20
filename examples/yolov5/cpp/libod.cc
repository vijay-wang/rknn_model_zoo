#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yolov5.h"
#include "image_utils.h"
#include "file_utils.h"
#include "image_drawing.h"

#if defined(RV1106_1103)
    #include "dma_alloc.hpp"
#endif

static rknn_app_context_t rknn_app_ctx;

#ifdef __cplusplus
extern "C" {
#endif

int init_od_model(const char *model_path, const char *coco_path)
{
        int ret;

        memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));
        init_post_process(coco_path);
        ret = init_yolov5_model(model_path, &rknn_app_ctx);
        if (ret != 0)
        {
                printf("init_yolov5_model fail! ret=%d model_path=%s\n", ret, model_path);
                deinit_post_process();
        }

        return ret;
}

int detect_and_draw(image_buffer_t *src_image)
{
        int ret;
        char text[256];

        object_detect_result_list od_results;

        ret = inference_yolov5_model(&rknn_app_ctx, src_image, &od_results);
        if (ret != 0)
        {
                printf("init_yolov5_model fail! ret=%d\n", ret);
                return ret;
        }

        // 画框和概率
        for (int i = 0; i < od_results.count; i++)
        {
                object_detect_result *det_result = &(od_results.results[i]);
                printf("%s @ (%d %d %d %d) %.3f\n", coco_cls_to_name(det_result->cls_id),
                                det_result->box.left, det_result->box.top,
                                det_result->box.right, det_result->box.bottom,
                                det_result->prop);
                int x1 = det_result->box.left;
                int y1 = det_result->box.top;
                int x2 = det_result->box.right;
                int y2 = det_result->box.bottom;

                draw_rectangle(src_image, x1, y1, x2 - x1, y2 - y1, COLOR_BLUE, 3);

                sprintf(text, "%s %.1f%%", coco_cls_to_name(det_result->cls_id), det_result->prop * 100);
                draw_text(src_image, text, x1, y1 - 20, COLOR_RED, 32);
        }

        return 0;
}

int release_od_model(void)
{
        int ret;

        deinit_post_process();

        ret = release_yolov5_model(&rknn_app_ctx);
        if (ret != 0)
        {
                printf("release_yolov5_model fail! ret=%d\n", ret);
        }

        return ret;
}
#ifdef __cplusplus
}
#endif

