#ifndef __LIBOD_H
#define __LIBOD_H

/**
 * @brief Image pixel format
 *
 */
typedef enum {
    IMAGE_FORMAT_GRAY8,
    IMAGE_FORMAT_RGB888,
    IMAGE_FORMAT_RGBA8888,
    IMAGE_FORMAT_YUV420SP_NV21,
    IMAGE_FORMAT_YUV420SP_NV12,
} image_format_t;

/**
 * @brief Image buffer
 *
 */
typedef struct {
    int width;
    int height;
    int width_stride;
    int height_stride;
    image_format_t format;
    unsigned char* virt_addr;
    int size;
    int fd;
} image_buffer_t;

#ifdef __cplusplus
extern "C" {
#endif

int init_od_model(const char *model_path, const char *coco_path);
int detect_and_draw(image_buffer_t *src_image);
int release_od_model(void);

#ifdef __cplusplus
}
#endif

#endif
