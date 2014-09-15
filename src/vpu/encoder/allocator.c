/* VPU decoder specific allocator
 * Copyright (C) 2013  Carlos Rafael Giani
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

#include <string.h>
#include <vpu_wrapper.h>
#include "allocator.h"
#include "base_enc.h"


GST_DEBUG_CATEGORY_STATIC(imx_vpu_enc_allocator_debug);
#define GST_CAT_DEFAULT imx_vpu_enc_allocator_debug



static void gst_imx_vpu_enc_allocator_finalize(GObject *object);

static gboolean gst_imx_vpu_enc_alloc_phys_mem(GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory, gssize size);
static gboolean gst_imx_vpu_enc_free_phys_mem(GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory);
static gpointer gst_imx_vpu_enc_map_phys_mem(GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory, gssize size, GstMapFlags flags);
static void gst_imx_vpu_enc_unmap_phys_mem(GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory);


G_DEFINE_TYPE(GstImxVpuEncAllocator, gst_imx_vpu_enc_allocator, GST_TYPE_IMX_PHYS_MEM_ALLOCATOR)




static void gst_imx_vpu_enc_mem_init(void)
{
	GstAllocator *allocator = g_object_new(gst_imx_vpu_enc_allocator_get_type(), NULL);
	gst_allocator_register(GST_IMX_VPU_ENC_ALLOCATOR_MEM_TYPE, allocator);
}


GstAllocator* gst_imx_vpu_enc_allocator_obtain(void)
{
	static GOnce dmabuf_allocator_once = G_ONCE_INIT;
	GstAllocator *allocator;

	g_once(&dmabuf_allocator_once, (GThreadFunc)gst_imx_vpu_enc_mem_init, NULL);

	allocator = gst_allocator_find(GST_IMX_VPU_ENC_ALLOCATOR_MEM_TYPE);
	if (allocator == NULL)
		GST_WARNING("No allocator named %s found", GST_IMX_VPU_ENC_ALLOCATOR_MEM_TYPE);

	return allocator;
}


static gboolean gst_imx_vpu_enc_alloc_phys_mem(GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory, gssize size)
{
	VpuEncRetCode ret;
	VpuMemDesc mem_desc;

	if (!gst_imx_vpu_base_enc_load())
		return FALSE;

	memset(&mem_desc, 0, sizeof(VpuMemDesc));
	mem_desc.nSize = size;
	ret = VPU_EncGetMem(&mem_desc);

	if (ret == VPU_ENC_RET_SUCCESS)
	{
		memory->mem.size         = mem_desc.nSize;
		memory->mapped_virt_addr = (gpointer)(mem_desc.nVirtAddr);
		memory->phys_addr        = (gst_imx_phys_addr_t)(mem_desc.nPhyAddr);
		memory->internal         = (gpointer)(mem_desc.nCpuAddr);

		GST_DEBUG_OBJECT(allocator, "addresses: virt: %p phys: %" GST_IMX_PHYS_ADDR_FORMAT " cpu: %p", memory->mapped_virt_addr, memory->phys_addr, memory->internal);

		return TRUE;
	}
	else
		return FALSE;
}


static gboolean gst_imx_vpu_enc_free_phys_mem(GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory)
{
        VpuEncRetCode ret;
        VpuMemDesc mem_desc;

	memset(&mem_desc, 0, sizeof(VpuMemDesc));
	mem_desc.nSize     = memory->mem.size;
	mem_desc.nVirtAddr = (unsigned long)(memory->mapped_virt_addr);
	mem_desc.nPhyAddr  = (unsigned long)(memory->phys_addr);
	mem_desc.nCpuAddr  = (unsigned long)(memory->internal);

	GST_DEBUG_OBJECT(allocator, "addresses: virt: %p phys: %" GST_IMX_PHYS_ADDR_FORMAT " cpu: %p", memory->mapped_virt_addr, memory->phys_addr, memory->internal);

	ret = VPU_EncFreeMem(&mem_desc);

	gst_imx_vpu_base_enc_unload();

	return (ret == VPU_ENC_RET_SUCCESS);
}


static gpointer gst_imx_vpu_enc_map_phys_mem(G_GNUC_UNUSED GstImxPhysMemAllocator *allocator, GstImxPhysMemory *memory, G_GNUC_UNUSED gssize size, G_GNUC_UNUSED GstMapFlags flags)
{
	return memory->mapped_virt_addr;
}


static void gst_imx_vpu_enc_unmap_phys_mem(G_GNUC_UNUSED GstImxPhysMemAllocator *allocator, G_GNUC_UNUSED GstImxPhysMemory *memory)
{
}




static void gst_imx_vpu_enc_allocator_class_init(GstImxVpuEncAllocatorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GstImxPhysMemAllocatorClass *parent_class = GST_IMX_PHYS_MEM_ALLOCATOR_CLASS(klass);

	object_class->finalize       = GST_DEBUG_FUNCPTR(gst_imx_vpu_enc_allocator_finalize);
	parent_class->alloc_phys_mem = GST_DEBUG_FUNCPTR(gst_imx_vpu_enc_alloc_phys_mem);
	parent_class->free_phys_mem  = GST_DEBUG_FUNCPTR(gst_imx_vpu_enc_free_phys_mem);
	parent_class->map_phys_mem   = GST_DEBUG_FUNCPTR(gst_imx_vpu_enc_map_phys_mem);
	parent_class->unmap_phys_mem = GST_DEBUG_FUNCPTR(gst_imx_vpu_enc_unmap_phys_mem);

	GST_DEBUG_CATEGORY_INIT(imx_vpu_enc_allocator_debug, "imxvpuencallocator", 0, "Freescale i.MX VPU encoder physical memory/allocator");
}


static void gst_imx_vpu_enc_allocator_init(GstImxVpuEncAllocator *allocator)
{
	GstAllocator *base = GST_ALLOCATOR(allocator);
	base->mem_type = GST_IMX_VPU_ENC_ALLOCATOR_MEM_TYPE;
}


static void gst_imx_vpu_enc_allocator_finalize(GObject *object)
{
	GST_INFO_OBJECT(object, "shutting down IMX VPU encoder allocator");
	G_OBJECT_CLASS(gst_imx_vpu_enc_allocator_parent_class)->finalize(object);
}
