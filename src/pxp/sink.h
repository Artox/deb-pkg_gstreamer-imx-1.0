/* PxP-based i.MX video sink class
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


#ifndef GST_IMX_PXP_SINK_H
#define GST_IMX_PXP_SINK_H

#include <gst/gst.h>
#include "../common/blitter_video_sink.h"
#include "blitter.h"


G_BEGIN_DECLS


typedef struct _GstImxPxPVideoSink GstImxPxPVideoSink;
typedef struct _GstImxPxPVideoSinkClass GstImxPxPVideoSinkClass;
typedef struct _GstImxPxPVideoSinkPrivate GstImxPxPVideoSinkPrivate;


#define GST_TYPE_IMX_PXP_VIDEO_SINK             (gst_imx_pxp_video_sink_get_type())
#define GST_IMX_PXP_VIDEO_SINK(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_IMX_PXP_VIDEO_SINK, GstImxPxPVideoSink))
#define GST_IMX_PXP_VIDEO_SINK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_IMX_PXP_VIDEO_SINK, GstImxPxPVideoSinkClass))
#define GST_IS_IMX_PXP_VIDEO_SINK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_IMX_PXP_VIDEO_SINK))
#define GST_IS_IMX_PXP_VIDEO_SINK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_IMX_PXP_VIDEO_SINK))


struct _GstImxPxPVideoSink
{
	GstImxBlitterVideoSink parent;
	GstImxPxPBlitter *blitter;
	GstImxPxPBlitterRotationMode output_rotation;
	gboolean input_crop;
};


struct _GstImxPxPVideoSinkClass
{
	GstImxBlitterVideoSinkClass parent_class;
};


GType gst_imx_pxp_video_sink_get_type(void);


G_END_DECLS


#endif
