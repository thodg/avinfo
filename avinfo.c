/*
 *  avinfo  -  audio and video streams info
 *
 *  Copyright 2010 Thomas de Grivel <billitch@gmail.com>
 *
 *  Permission to use, copy, modify, and distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>


void print_quoted(const char *str)
{
  putchar('"');
  while (*str) {
    if (*str == '"')
      putchar('\\');
    putchar(*str);
    str++;
  }
  putchar('"');
}


int error (const char *msg, const char *append)
{
  fputs(" :error ", stdout);
  print_quoted(msg);
  fputs(append, stdout);
  return 1;
}

void avinfo_sample_fmt(enum AVSampleFormat fmt) {
  if (fmt < -1 || fmt >= AV_SAMPLE_FMT_NB)
    printf("%d", fmt);
  else {
    static const char *name[AV_SAMPLE_FMT_NB + 1] = {
      ":NONE",
      ":U8",
      ":S16",
      ":S32",
      ":FLT",
      ":DBL"
    };
    fputs(name[fmt + 1], stdout);
  }
}

void avinfo_pix_fmt(int pix_fmt) {
  if (pix_fmt < -1 || pix_fmt >= AV_PIX_FMT_NB)
    printf("%d", pix_fmt);
  else {
    static const char *name[AV_PIX_FMT_NB + 1] = {
      ":NONE",
      ":YUV420P",
      ":YUYV422",
      ":RGB24",
      ":BGR24",
      ":YUV422P",
      ":YUV444P",
      ":YUV410P",
      ":YUV411P",
      ":GRAY8",
      ":MONOWHITE",
      ":MONOBLACK",
      ":PAL8",
      ":YUVJ420P",
      ":YUVJ422P",
      ":YUVJ444P",
      ":XVMC_MPEG2_MC",
      ":XVMC_MPEG2_IDCT",
      ":UYVY422",
      ":UYYVYY411",
      ":BGR8",
      ":BGR4",
      ":BGR4_BYTE",
      ":RGB8",
      ":RGB4",
      ":RGB4_BYTE",
      ":NV12",
      ":NV21",
      ":ARGB",
      ":RGBA",
      ":ABGR",
      ":BGRA",
      ":GRAY16BE",
      ":GRAY16LE",
      ":YUV440P",
      ":YUVJ440P",
      ":YUVA420P",
      ":VDPAU_H264",
      ":VDPAU_MPEG1",
      ":VDPAU_MPEG2",
      ":VDPAU_WMV3",
      ":VDPAU_VC1",
      ":RGB48BE",
      ":RGB48LE",
      ":RGB565BE",
      ":RGB565LE",
      ":RGB555BE",
      ":RGB555LE",
      ":BGR565BE",
      ":BGR565LE",
      ":BGR555BE",
      ":BGR555LE",
      ":VAAPI_MOCO",
      ":VAAPI_IDCT",
      ":VAAPI_VLD",
      ":YUV420P16LE",
      ":YUV420P16BE",
      ":YUV422P16LE",
      ":YUV422P16BE",
      ":YUV444P16LE",
      ":YUV444P16BE",
      ":VDPAU_MPEG4",
      ":DXVA2_VLD",
      ":RGB444BE",
      ":RGB444LE",
      ":BGR444BE",
      ":BGR444LE",
      ":Y400A" };
    fputs(name[pix_fmt + 1], stdout);
  }
}


int avinfo_codec(AVCodecParameters *cp) {
  AVCodec *codec;
  AVCodecContext *ctx;
  printf("\n   (:codec");
  codec = avcodec_find_decoder(cp->codec_id);
  if (codec == NULL)
    return error("codec not found", ")");
  ctx = avcodec_alloc_context3(codec);
  if (codec->capabilities & CODEC_CAP_TRUNCATED)
    ctx->flags |= CODEC_FLAG_TRUNCATED;
  if(avcodec_open2(ctx, codec, NULL) < 0)
    error("avcodec_open2 failed", ")");
  printf("\n    :bit-rate %lld", ctx->bit_rate);
  switch (ctx->codec_type) {
  case AVMEDIA_TYPE_AUDIO:
    printf("\n    :codec-type :audio");
    printf("\n    :sample-rate %d", ctx->sample_rate);
    printf("\n    :channels %d", ctx->channels);
    printf("\n    :sample-format ");
    avinfo_sample_fmt(ctx->sample_fmt);
    printf("\n    :channel-layout %lld", ctx->channel_layout);
    break;
  case AVMEDIA_TYPE_VIDEO:
    printf("\n    :codec-type :video");
    printf("\n    :width %d", ctx->width);
    printf("\n    :height %d", ctx->height);
    printf("\n    :pixel-format ");
    avinfo_pix_fmt(ctx->pix_fmt);
    printf("\n    :has-b-frames %s", ctx->has_b_frames ? "t" : "nil");
    printf("\n    :sample-aspect-ratio %d/%d",
	   ctx->sample_aspect_ratio.num,
	   ctx->sample_aspect_ratio.den);
    break;
  default:
    printf("\n    :codec-type :unknown");
    break;
  }
  printf(")");
  //av_free(pFrame);
  avcodec_close(ctx);
  return 0;
}


int avinfo_stream(AVStream *stream) {
  printf("\n  (:stream");
  printf("\n   :id %d", stream->id);
  printf("\n   :codec");
  avinfo_codec(stream->codecpar);
  printf("\n   :start-time %llu", stream->start_time);
  printf("\n   :duration %llu", stream->duration);
  printf("\n   :nb-frames %llu", stream->nb_frames);
  printf("\n   :sample-aspect-ratio %d/%d",
	 stream->sample_aspect_ratio.num,
	 stream->sample_aspect_ratio.den);
  printf("\n   :average-frame-rate %d/%d",
	 stream->avg_frame_rate.num,
	 stream->avg_frame_rate.den);
  printf(")");
  return 0;
}


int avinfo_file(const char *path)
{
  AVFormatContext *fmt;
  int i;
  if (av_open_input_file(&fmt, path, NULL, 0, NULL) != 0)
    return error("av_open_input_file failed", "");
  if (av_find_stream_info(fmt) < 0)
    return error("av_find_stream_info failed", "");

  printf("\n :start-time %lld", fmt->start_time);
  printf("\n :duration %lld", fmt->duration);
  printf("\n :file-size %lld", fmt->file_size);
  printf("\n :bit-rate %lld", fmt->bit_rate);
  printf("\n :streams (");
  for (i = 0; i < fmt->nb_streams; i++)
    avinfo_stream(fmt->streams[i]);
  printf(")");

  // Close the video file
  av_close_input_file(fmt);

  return 0;
}


int main (int argc,
	  char *argv[])
{
  av_register_all();

  while (--argc) {
    argv++;
    fputs("(:file\n :path #P", stdout);
    print_quoted(*argv);
    avinfo_file(*argv);
    puts(")");
  }

  return 0;
}
