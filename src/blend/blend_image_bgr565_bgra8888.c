﻿/**
 * File:   blend_image_bgr565_bgra8888.c
 * Author: AWTK Develop Team
 * Brief:  blend bgra8888 on bgr565
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-10-10 Generated by gen.sh(DONT MODIFY IT)
 *
 */
#include "tkc/rect.h"
#include "base/pixel.h"
#include "base/bitmap.h"
#include "base/pixel_pack_unpack.h"

#define pixel_dst_t pixel_bgr565_t
#define pixel_dst_format pixel_bgr565_format
#define pixel_dst_to_rgba pixel_bgr565_to_rgba
#define pixel_dst_from_rgb pixel_bgr565_from_rgb
#define pixel_dst_from_rgba pixel_bgr565_from_rgba

#define pixel_src_t pixel_bgra8888_t
#define pixel_src_format pixel_bgra8888_format
#define pixel_src_to_rgba pixel_bgra8888_to_rgba

#define pixel_t pixel_dst_t
#define pixel_from_rgb pixel_dst_from_rgb
#define pixel_from_rgba pixel_dst_from_rgba
#define pixel_to_rgba pixel_dst_to_rgba

static inline void blend_a_bgr565_bgra8888(uint8_t* dst, uint8_t* src, uint8_t alpha, bool_t premulti_alpha) {
  uint32_t color = *(uint32_t*)src;
  uint8_t sa = color >> 24;
  uint8_t sr = color >> 16;
  uint8_t sg = color >> 8;
  uint8_t sb = color & 0xff;
  uint8_t a = alpha > 0xf8 ? sa : ((sa * alpha) >> 8);

  if (a > 0xf8) {
    *(uint16_t*)dst = ((sr >> 3) << 11) | ((sg >> 2) << 5) | (sb >> 3);
  } else if (a > 8) {
    rgba_t rgba = {.a = a, .r = sr, .g = sg, .b = sb};
    if (premulti_alpha) {
      rgba.a = 0xff - a;
      if(alpha <= 0xf8) {
        rgba.r = (sr * alpha) >> 8;
        rgba.g = (sg * alpha) >> 8;
        rgba.b = (sb * alpha) >> 8;
      }
      pixel_bgr565_blend_rgba_premulti(dst, rgba);
    } else  {
      pixel_bgr565_blend_rgba(dst, rgba);
    }
  }
}

#define blend_a blend_a_bgr565_bgra8888

#include "pixel_ops.inc"
#include "blend_image.inc"

ret_t blend_image_bgr565_bgra8888(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r,
                                  uint8_t a) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL && dst_r != NULL,
                       RET_BAD_PARAMS);
  return_value_if_fail(dst->format == BITMAP_FMT_BGR565 && src->format == BITMAP_FMT_BGRA8888,
                       RET_BAD_PARAMS);

  if (a > 0xf8) {
    return blend_image_without_alpha(dst, src, dst_r, src_r);
  } else if (a > 8) {
    return blend_image_with_alpha(dst, src, dst_r, src_r, a);
  } else {
    return RET_OK;
  }
}
