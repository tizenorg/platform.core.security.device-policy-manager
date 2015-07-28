Name:    device-policy-manager
Summary: Tizen Device Policy Manger
Version: 0.0.1
Release: 1
Group:   Security/Other
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz

BuildRequires: gcc
BuildRequires: cmake
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(libgum)
BuildRequires: pkgconfig(libsystemd-daemon)
BuildRequires: pkgconfig(libsystemd-journal)

%description
Tizen Device Policy Manger service, libraries, devel packages.
dpm-server, libdpm, libdpm-devel.

%package -n libdpm 
Summary : Tizen DPM client library.
Group   : Development/Libraries

%description -n libdpm 
Tizen Device Policy manager client library.

%post -n libdpm -p /sbin/ldconfig
%postun -n libdpm -p /sbin/ldconfig

%package -n libdpm-devel
Summary : Tizen DPM development package.
Group   : Development/Libraries
Requires : %{name} = %{version}-%{release}

%description -n libdpm-devel
Tizen Device Policy manager development package

%post -n libdpm-devel -p /sbin/ldconfig
%postun -n libdpm-devel -p /sbin/ldconfig

%prep
%setup -q

%preun
/sbin/ldconfig

%build

%cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%clean
rm -rf %{buildroot}

%post


%files -n libdpm 
%defattr(-,root,root,-)
/usr/lib/*.so.*

%files -n libdpm-devel
/usr/include/*.h
/usr/lib/*.so
/usr/lib/pkgconfig/*.pc

%files 
%defattr(-,root,root,-)
/usr/bin/dpm-server
