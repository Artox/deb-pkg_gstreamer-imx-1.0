#
# spec file for package gstreamer-imx
#
# Copyright (c) 2014 Josua Mayer <privacy@not.given>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

Name: gstreamer-plugins-imx
Version: 0.11.1
Release: 1
License: GPL-2.0
Group: Productivity/Multimedia/Other
Summary: GStreamer plugins for i.MX SoC
Source: %{name}-%{version}.tar.gz
Source10: rpmlintrc
BuildRequires: python pkg-config
BuildRequires: pkgconfig(gstreamer-1.0) >= 1.2.0
BuildRequires: pkgconfig(gstreamer-base-1.0) >= 1.2.0
BuildRequires: pkgconfig(gstreamer-audio-1.0) >= 1.2.0
BuildRequires: pkgconfig(gstreamer-video-1.0) >= 1.2.0
BuildRequires: libfslvpuwrap-devel >= 1.0.45
BuildRequires: libfslcodec-devel
BuildRequires: gpu-viv-bin-x11-devel

%description
Provides plugins for GStreamer-1.0 to make use of the VPU in i.MX SoCs.

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%prep
%setup -q

%build
./waf configure --egl-platform=x11 --prefix=/usr
./waf build %{?_smp_mflags}

%install
./waf install --destdir=%{buildroot}

%files
/usr/lib/gstreamer-1.0/*.so
/usr/lib/*.so*

%changelog
