/* G2D-based i.MX video sink class
 * Copyright (C) 2014  Carlos Rafael Giani
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef GST_IMX_G2D_SINK_H
#define GST_IMX_G2D_SINK_H

#include <gst/gst.h>
#include "../common/blitter_video_sink.h"
#include "blitter.h"


G_BEGIN_DECLS


typedef struct _GstImxG2DVideoSink GstImxG2DVideoSink;
typedef struct _GstImxG2DVideoSinkClass GstImxG2DVideoSinkClass;
typedef struct _GstImxG2DVideoSinkPrivate GstImxG2DVideoSinkPrivate;


#define GST_TYPE_IMX_G2D_VIDEO_SINK             (gst_imx_g2d_video_sink_get_type())
#define GST_IMX_G2D_VIDEO_SINK(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_IMX_G2D_VIDEO_SINK, GstImxG2DVideoSink))
#define GST_IMX_G2D_VIDEO_SINK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_IMX_G2D_VIDEO_SINK, GstImxG2DVideoSinkClass))
#define GST_IS_IMX_G2D_VIDEO_SINK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_IMX_G2D_VIDEO_SINK))
#define GST_IS_IMX_G2D_VIDEO_SINK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_IMX_G2D_VIDEO_SINK))


struct _GstImxG2DVideoSink
{
	GstImxBlitterVideoSink parent;
	GstImxG2DBlitter *blitter;
	GstImxG2DBlitterRotationMode output_rotation;
};


struct _GstImxG2DVideoSinkClass
{
	GstImxBlitterVideoSinkClass parent_class;
};


GType gst_imx_g2d_video_sink_get_type(void);


G_END_DECLS


#endif
